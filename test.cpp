#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "sha256.h"

using namespace std;

int main () {

    int line = 0;
    string input, output, output0;

    ifstream fpin("in.dat");
    ifstream fpout("out.dat");

    getline(fpin, input);
    getline(fpout, output);

    do {

        line++;

        output0 = sha256(input);

        if (output.compare(output0) != 0) {
            cout << "Error at line " << line << ".\n";
            cout << "Expected:\t" << output << "\n";
            cout << "Obtained:\t" << output0 << "\n";
            return 1;
        }

        cout << line;

        getline(fpin, input);
        getline(fpout, output);

    } while(!(fpin.eof() || fpout.eof()));

    cout << "Noice, all " << line << " tests passed.\n";

    return 0;
}
