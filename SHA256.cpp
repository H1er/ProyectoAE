#include <iostream>
#include "SHA256.h"
#include "SHA256core.h"
#include <bitset>
#include <string>
#include <ap_fixed.h>

using namespace std;

#define H0 {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c, 0x1f83d9ab, 0x5be0cd19}
const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}


string sha256(string msg) {
    uint64_t numBlocks = (msg.size()+72)/64;
    uint64_t tam = msg.size() * 8;

    int K = 0;
    K = (448 - tam - 1) % 512;

    if (K < 0){
        K += 512;
    }

    int auxBlock = 0;

    string block = "";
    for(int j=0; j<(tam/8); j++){

        block += bitset<8>(msg[j]).to_string();
        auxBlock += 8;
    }

    //a�adir el 1
    block += "1";
    auxBlock ++;

    //a�adir los 0
    block.insert(tam + 1, K, '0');
    auxBlock += K;


    //a�adir el tama�o
    block += bitset<64>(tam).to_string();
    auxBlock += 64;

    bits512 blocks[numBlocks];

    int idx=0;

    for(int i=0; i<numBlocks; i++){
        for(int j = 0; j<512; j++) {
        	blocks[i][511-j] = (bits1) block[idx];
        	idx++;
        }
        cout << blocks[i].to_string(2).erase(0,2);
    }
    cout << endl << block << endl;


    bits32 prevH[8] = H0, H[8];

    for (int i = 0; i < numBlocks; i++) {

    	for (short j=0; j<8; j++) {
    		H[j] = prevH[j];
    	}

        bits512 bloque = blocks[i];

        blockprocessing(bloque, H);

        for (short j=0; j<8; j++) {
        	prevH[j] += H[j];
		}
        cout << endl;

    }


    string s = "";

    for (int i=0; i<8; i++) {
    	for (int j=0; j<32; j+=4) {
    		s.push_back(hec[prevH[i].range(j+4, j)]);
    	}
    }

    return s;
}








