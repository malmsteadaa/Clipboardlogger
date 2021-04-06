#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include<map>
using namespace std;
//create a stream to a file stored in the Logbook object
string Logbook = "Log1.txt";
ofstream Recorder(Logbook);
//if visibility is set to 1 then it will not run in the background, if it was 0 then the program runs in the background

map<int, string>Format_names = { {1,"CF_TEXT"},{2,"CF_BITMAP"},{3,"CF_METAFILEPICT"},{4,"CF_STLK"},{5,"CF_DIF"},{6,"CF_TIFF"},{7,"CF_OEMTEXT"},{8,"CF_DIB"},{9,"CF_PALETTE"}, {10,"CF_PENDATA"}, {11,"CF_RIFF"},{12,"CF_WAVE"},{13,"UNICODETEXT"}, {14,"CF_ENHMETAFILE"}, {15,"CF_HDROP"},{16,"CF_LOCALE"},{17,"CF_DIBV5"} };
void Run_in_Background(int visibility) {
	HWND window;
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-findwindowa
	window = FindWindowA("ConsoleWindowClass", NULL);
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	ShowWindow(window, visibility);
}

int main() {
	//run it in the background
	Run_in_Background(1);
	DWORD tOurSeq = -1;//our current count
	while (1) {//continuously run
		// get the clipboard count
		DWORD ClipSeq = GetClipboardSequenceNumber();
		//see if the clipboard and our count are the same to till if it change since our last count
		bool same = (ClipSeq == tOurSeq);
		//set our count to the clipboard count
		tOurSeq = ClipSeq;

		try {
			//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboardformatnamea#return-value
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
				std::cout << "Format :" << Format_names.at(format) << " " << endl;
				Recorder << "==========================================================================================================================" << endl;
				Recorder << "Format :" << Format_names.at(format) << " " << endl;
				std::cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
				Recorder << "-------------------------------------------------------------------------------------------------------------------------" << endl;

				switch (format) {
					//Data type: ANSI text
					//Use Case:text
					//CF_TEXT
				case 1:
					OpenClipboard(0);
					//grab text from the clipboard
					in = GetClipboardData(CF_TEXT);
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
			cout << "Something went wrong with clipboard " << endl;
		}
	}
	Recorder.close();
}