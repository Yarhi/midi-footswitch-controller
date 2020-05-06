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

int main()
{
	cout << "Bytes sizes : " << _BSIZE << endl;

    // Just to start working, we just put databytes into a simple file
	read_exampledt("C:\\Users\\ghuot\\arduino-midi\\midi-footswitch-controller\\src\\example.dt");
	return 0;
}

void read_exampledt(string filename) {
    string bytes;
    ifstream in;

    in.open(filename);

    if (in.good()) {
        // First we read the file
        getline(in, bytes);

        // We count the number of banks in order to allow memory
        int bankCount = bytes.length() / (SWITCH_COUNT * _BSIZE);


        // Allocating memory
        int** data = new int* [bankCount];
        for (int i = 0; i < bankCount; i++) {
            data[i] = new int[SWITCH_COUNT];
        }

        // Usefull variables
        int* currentByte; // The current "command" readed
        int currentByteCursor = 0; // The current command readed cursot

        int currentBankCursor = 0; // Current bank cursor
        int currentSwitchCursor = 0; // Current switch (in the bank) cursor

        // Let's start parsing
        for (int i = 0; i < bytes.length() - _BSIZE; i++) {

            // Reset the currentByte if the cursor is at zero
            if (currentByteCursor == 0) {
                currentByte = new int[_BSIZE];
            }

            // Parsing the char from bytes into an int.
            // The char is gonna be "5", but we want the real "5" number, and not the ascii code of "5"
            currentByte[currentByteCursor] = (int)bytes[i] - '0';
            
            // Incrementing the cursor
            currentByteCursor++;

            // Current byte is full, adding it to data
            if (currentByteCursor >= _BSIZE) {

                // Here, we calculate the current command int.
                // We got something like {1,2,7}. We just have to do (1 * 10^2) + (2 * 10^1) + (7 * 10^0)
                int byteForSwitch = 0;
                for (int j = 0; j < _BSIZE; j++) {
                    byteForSwitch += pow(10, _BSIZE - j - 1) * currentByte[j];
                }

                // Associate the command to the switch in current bank
                data[currentBankCursor][currentSwitchCursor] = byteForSwitch;

                // Going to next switch
                currentSwitchCursor++;
                
                // Reached switchs limit, going to next bank and reseting switch
                if (currentSwitchCursor >= SWITCH_COUNT) {
                    currentBankCursor++;
                    currentSwitchCursor = 0;
                }

                // Reseting the currentByteCursor
                currentByteCursor = 0;
            }
        }

    }  


    if (!in.eof() && in.fail())
        cout << "error reading" << endl;

    in.close();
}
