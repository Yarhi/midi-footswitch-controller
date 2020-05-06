// midi-footswitch-controller.cpp : définit le point d'entrée de l'application.
//

#include "midi-footswitch-controller.h"
#include "system-commands.h"
#include "ANALOG.h"

#include <iostream> 
#include <string> 
#include <fstream>
#include <vector>

using namespace std;

int** data;

int main()
{
	cout << "Bytes sizes : " << _BSIZE << endl;
	read_exampledt("C:\\Users\\ghuot\\arduino-midi\\midi-footswitch-controller\\src\\example.dt");
	return 0;
}

void read_exampledt(string filename) {
    string bytes;

    ifstream in;
    in.open(filename);

    if (in.good()) {
        getline(in, bytes);

        int bankCount = bytes.length() / (SWITCH_COUNT * _BSIZE);

        int** data = new int* [bankCount];

        for (int i = 0; i < bankCount; i++) {
            data[i] = new int[SWITCH_COUNT];
        }

        int* currentByte;
        int currentByteCursor = 0;

        int currentBankCursor = 0;
        int currentSwitchCursor = 0;
        for (int i = 0; i < bytes.length() - _BSIZE; i++) {
            if (currentByteCursor == 0) {
                currentByte = new int[_BSIZE];
            }

            currentByte[currentByteCursor] = (int)bytes[i] - '0';
            currentByteCursor++;

            if (currentByteCursor >= _BSIZE) {
                int byteForSwitch = 0;
                for (int j = 0; j < _BSIZE; j++) {
                    byteForSwitch += pow(10, _BSIZE - j - 1) * currentByte[j];
                }

                cout << currentBankCursor << " - " << currentSwitchCursor << " - " << byteForSwitch << endl;
                data[currentBankCursor][currentSwitchCursor] = byteForSwitch;

                currentSwitchCursor++;
                if (currentSwitchCursor >= SWITCH_COUNT) {
                    currentBankCursor++;
                    currentSwitchCursor = 0;
                }

                currentByteCursor = 0;
            }
        }

    }  


    if (!in.eof() && in.fail())
        cout << "error reading" << endl;

    in.close();

}
