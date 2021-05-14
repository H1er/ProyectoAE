#include <iostream>
#include "SHA256.h"
#include "SHA256core.h"
#include <bitset>
#include <ap_fixed.h>

using namespace std;

const bits32 H0[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
//const bits32 H0[8] = {0x6a09e667bb67ae853c6ef372a54ff53a510e527f9b05688c1f83d9ab5be0cd19};

void sha256(string msg)
{
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
    //añadir el 1
    block += "1";
    auxBlock ++;
    
    //añadir los 0
    block.insert(tam + 1, K, '0');
    auxBlock += K;
            
    //añadir el tamaño
    block += bitset<64>(tam).to_string();
    auxBlock += 64;

    string blocks[numBlocks-1];

    for(int i=0; i<numBlocks; i++){
        blocks[i] = block.substr(i*512,(i+1)*512);
    }

    bits32 AH[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    for (int i = 0; i < numBlocks; i++){
        bits512 bloque (blocks[i], 2);
        blockprocessing(bloque, AH);
    }
    bits256 hash = AH[0].concat(AH[1].concat(AH[2].concat(AH[3].concat(AH[4].concat(AH[5].concat(AH[6].concat(AH[7])))))));

    return hash.toString;
    
}


















