// A dirty sidtune format converter hack with almost no security code.
// TMP should be used.
 
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <iomanip.h>
#include "sidtune.h"
#include "fformat.h"

static void printUsage()
{
    cerr << "Usage: sidcon [--psid|--sidplay] <files>" << endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printUsage();
        exit(-1);
    }
    
    int a = 0;
    bool toPSID = true,
        toSIDPLAY = false;
    
    while (a++ < argc-1)
    {
        if (strnicmp(argv[a],"--psid",6) == 0)
        {
            toPSID = true;
            toSIDPLAY = false;
        }
        else if (strnicmp(argv[a],"--sidplay",9) == 0)
        {
            toPSID = false;
            toSIDPLAY = true;
        }
        else if (strnicmp(argv[a],"--",2) == 0)
        {
            printUsage();
            exit(-1);
        }
    }

    bool success = true;
    
    a = 0;
    while (a++ < argc-1)
    {
        success = true;  // reset flag for each new file
        
        if (strnicmp(argv[a],"--",2) != 0)
        {
            cout << argv[a] << ' ';
            
            char* outFileName = 0;
            sidTune mySidTune(argv[a]);
            sidTuneInfo mySTI;
            
            success &= mySidTune.getStatus();

            if (success)
            {
                cout << '.';
                success &= mySidTune.getInfo(mySTI);
				if (mySTI.fixLoad)
				{
					cout << endl << "Duplicate C64 load address detected. Fix it? (y/n) " << flush;
					char c;
					cin >> c;
					if (tolower(c)=='y')
					{
						mySidTune.fixLoadAddress();
					}
				}
                success &= (mySTI.dataFileName!=0);
                if (success)
                {
                    success &= (0!=(outFileName = new char[strlen(mySTI.path)+strlen(mySTI.dataFileName)+4+1]));
                    if (success)
                    {
						strcpy(outFileName,mySTI.path);
                        strcat(outFileName,mySTI.dataFileName);
                    }
                }
            }
            
            if (success)
            {
                cout << '.';
                success &= (remove(outFileName)==0);  // here = path+dataFileName
                if (mySTI.infoFileName != 0)
                {
					strcpy(outFileName,mySTI.path);
					strcat(outFileName,mySTI.infoFileName);
                    success &= (remove(outFileName)==0);
                }
            }
            
            if (success)
            {
                cout << '.';
                if (toPSID)
                {
                    strcpy(fileExtOfPath(outFileName),".sid");
                    success &= (mySidTune.savePSIDfile(outFileName));
                }
                else if (toSIDPLAY)
                {
                    strcpy(fileExtOfPath(outFileName),".c64");
                    success &= (mySidTune.saveC64dataFile(outFileName));
                    strcpy(fileExtOfPath(outFileName),".sid");
                    success &= (mySidTune.saveSIDfile(outFileName));
                }
            }

            if (success)
            {
                cout << " OK" << endl;
            }
            else
            {
                cout << " ERROR" << endl;
            }

            if (outFileName != 0)
                delete[] outFileName;
        }
    }
}
