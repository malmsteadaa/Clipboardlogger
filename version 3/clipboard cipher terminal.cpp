#include <Richedit.h>
#include <string>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <winuser.h>
//cipher
char* someFunc(char* test, int rot)
{
    int i = 0;
    char* someText = test;
    while (someText[i] != 0)
    {
        someText[i] = (char)(test[i] + rot);
        i++;
    }
    return someText;
}
//decipher
char* someFunc2(char* test, int rot)
{
    int i = 0;
    char* someText = test;
    while (someText[i] != 0)
    {
        someText[i] = (char)(test[i] - rot);
        i++;
    }
    return someText;
}

using namespace std;
int main() {
    

    int choice = -1;
    HANDLE in;
    char* buffer= new char[256];
    string temp;
    while (true) {
        cout << "Choices:" << endl;
        cout << "Choice 1: copy into clipboard" << endl;
        cout << "Choice 2: pull from clipboard" << endl;
        cout << "Pick a number:";
        cin >> choice;
        cout << endl;
        switch (choice) {
        case 1:
 
        {
            cout << "enter a string to cipher and put in clipboard" << endl;
            cin >> buffer;
            const char* output = someFunc(buffer,5);
            const size_t len = strlen(output) + 1;
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
            memcpy(GlobalLock(hMem), output, len);
            GlobalUnlock(hMem);
            OpenClipboard(0);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, hMem);
            CloseClipboard();

            break; 
        }
        case 2:
            OpenClipboard(0);
            //grab text from the clipboard
            in = GetClipboardData(CF_TEXT);
            //using said text display it to the console and store into the Logbook
            cout << "The cypher text in clipboard is : ";
            cout << someFunc2((char*)in, 5) << endl;
            CloseClipboard();
            break;
        default:
            cout << "incorrect input" << endl;
            break;
        }
       
    }
}
