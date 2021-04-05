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
			
			UINT formats = CountClipboardFormats();
			UINT next_format = 0;
			next_format = EnumClipboardFormats(next_format);
			const int max_count = 4096;
			wchar_t format_name[max_count];
			int name_length = GetClipboardFormatName(next_format, format_name, max_count);
			//flags to check the clipboard format.

				bool UNICODETEXT = IsClipboardFormatAvailable(CF_UNICODETEXT);
				bool Text= IsClipboardFormatAvailable(CF_TEXT);
				bool TIFF = IsClipboardFormatAvailable(CF_TIFF);
				if (UNICODETEXT && Text && !same) {//if the clipboard has the format we want and it change recently

					OpenClipboard(0);
					//grab text from the clipboard
					HANDLE in = GetClipboardData(CF_TEXT);
					//using said text display it to the console and store into the Logbook
					std::cout << "==========================================================================================================================" << endl;
					std::cout << (char*)in << endl;
					Recorder << "==========================================================================================================================" << endl;
					Recorder << (char*)in << endl;
					CloseClipboard();
				}
			}
		
		catch(exception e){
			cout << "this is not text" << endl;

		}
	}
	Recorder.close();

}