#include <iostream>
#include "SHA256.h"
#include <ap_fixed.h>

using namespace std;

const bits32 K[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

const bits32 H0[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c, 0x1f83d9ab, 0x5be0cd19};


bits32 sigma_0lower (bits32 x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

bits32 sigma_1lower (bits32 x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

bits32 sigma_0upper (bits32 x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);

}

bits32 sigma_1upper (bits32 x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

bits32 rotr (bits32 a, bits5 shift) {
    return (a << shift) | (a >> (32 - shift));
}

bits32 ch (bits32 x, bits32 y, bits32 z) {
    return (x & y) ^ ((~x) & z);
}

bits32 maj (bits32 x, bits32 y, bits32 z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

void decomposition (bits6 &round, bits32 &firstsWi, bits512 block) {
    bits9 idx = 512 - (round * 32);
    firstsWi = block.range(idx, idx - 32);
}

void get_Wi(bits6 &round, bits32 &Wi, bits32 &firstsWi) {
    
    static bits32 previousWi[16] = {};  // Hay que usar RESET

    if (round < 16) {
        Wi = firstsWi;
    } else {
        Wi = sigma_1lower(previousWi[1]) + previousWi[6] + sigma_0lower(previousWi[4]) + previousWi[15];
    }

    for (short i = 15; i > 0; i--) {
		previousWi[i] = previousWi[i - 1];
	}

    previousWi[0] = Wi;
    
}

void blockprocessing(bits512 block, bits32[8] AH)
{												   //0 1 2 3 4 5 6 7
	//static bits32 AH [8] = H0; //a�adir reset,    AH{a,b,c,d,e,f,g,h}
	bits32 aux,wi, T1,T2;

	for(int i=0;i<64;i++) //64 rondas de procesado
	{
		decomposition(i, aux, block);
		get_Wi(i, wi, aux);

		T1 = AH[7] ^ sigma_1upper(AH[4]) + ch(AH[4], AH[5], AH[6]) + K[i] + wi;
		T2 = sigma_0upper(AH[0]) + maj(AH[0], AH[1], AH[2]);

		AH[7] = AH[6];;
		AH[6] = AH[5];
		AH[5] = AH[4];
		AH[4] = AH[3] + T1;
		AH[3] = AH[2];
		AH[2] = AH[1];
		AH[1] = AH[0];
		AH[0] = T1 + T2;
	}

    //hash = AH[0].concat(AH[1].concat(AH[2].concat(AH[3].concat(AH[4].concat(AH[5].concat(AH[6].concat(AH[7])))))));

	/*if(final)
	{
		
		AH = H0; //el reset
	}*/
}
