#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
using namespace std;
//create a stream to a file stored in the Logbook object
string Logbook = "Log1.txt";
ofstream Recorder(Logbook);

int main() {
	DWORD tOurSeq = -1;//our current count
		while (1) {//continuously run
			// get the clipboard count
			DWORD ClipSeq = GetClipboardSequenceNumber();
			//see if the clipboard and our count are the same to till if it change since our last count
			bool same = (ClipSeq == tOurSeq);
			//set our count to the clipboard count
			tOurSeq = ClipSeq;

		
		try{
			//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboardformatnamea#return-value
			UINT formats = CountClipboardFormats();
			UINT next_format = 0;
			next_format = EnumClipboardFormats(next_format);
			const int max_count = 4096;
			wchar_t format_name[max_count];
			int name_length = GetClipboardFormatName(next_format, format_name, max_count);
			int format=(int) formats;
			//flags to check the clipboard format.
				bool UNICODETEXT = IsClipboardFormatAvailable(CF_UNICODETEXT);
				bool Text= IsClipboardFormatAvailable(CF_TEXT);
				bool TIFF = IsClipboardFormatAvailable(CF_TIFF);
				HANDLE in;
				if (!same) {
				 					std::cout << "==========================================================================================================================" << endl;
									Recorder << "==========================================================================================================================" << endl;

				switch (format) {
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
				//CF_BITMAP
				case 2:

					break;
				//CF_MetafilePict
				case 3:
					break;
				//Cf_Sylk
				case 4:
					break;
				//CF_Dif
				case 5:
					break;
				//CF_TiFF
				case 6:
					break;
				//CF_OEMTEXT
				case 7:
					break;
				//CF_DIB
				case 8:
					break;
				//CF_Palette
				case 9:
					break;
				//CF_PenDATA
				case 10:
					break;
				//CF_RIFF
				case 11:
					break;
				//CF_WAVE
				case 12:
					break;
				//CF_UNICODETEXT
				case 13:
					break;
				//CF_EMHMETAFILE
				case 14:
					break;
				//CF_HDROP
				case 15:
					break;
				//CF_LOCALE
				case 16:
					break;
				//CF_DIBV5
				case 17:
					break;
				//CF_DSPTEXT
				case 0x0081:
					break;
				//CF_DSPBITMAP
				case 0x0082:
					break;
				//CF_DSPENHMETAFILE
				case 0x008E:
					break;
				//CF_DSPMETAFILEPICT
				case 0x0083:
					break;
				//CF_GDIOBJFIRST
				case 0x0300:
					break;
				//CF_GDIOBJLAST
				case 0x03FF:
					break;
				//CF_OWNERDISPLAY
				case 0x0080:
					break;
				//CF_PRIVATEFIRST
				case 0x0200:
					break;
				//CF_PRIVATELAST
				case 0x02FF:
					break;
				

				default:
					break;
				}
				
				}
			}
		
		catch(exception e){
			cout << "this is not text" << endl;

		}
	}
	Recorder.close();

}