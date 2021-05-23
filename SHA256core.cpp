#include <iostream>
#include "SHA256core.h"
#include <ap_fixed.h>


using namespace std;

const bits32 K[64] = {	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
						0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
						0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
						0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
						0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
						0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
						0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
						0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

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
    return (a >> shift) | (a << (32 - shift));
}

bits32 ch (bits32 x, bits32 y, bits32 z) {
    return (x & y) ^ ((~x) & z);
}

bits32 maj (bits32 x, bits32 y, bits32 z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

void blockprocessing (bits512 block, bits32 H[8]) {
#pragma HLS ARRAY_PARTITION variable=H dim=1 complete
	bits32 aux, Wi, T1,T2;
	static bits32 previousWi[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	#pragma HLS ARRAY_PARTITION variable=previousWi dim=1 complete

	RoundLoop:
	for(bits7 round = 0; round<64; round++) {
#pragma HLS PIPELINE
 //64 rondas de procesado

		if (round == 0) {	// Reset previousWi
			ResetWLoop:
			for (bits5 i = 0; i < 16; i++) {
			#pragma HLS UNROLL factor=16
				previousWi[i] = 0;
			}
		}

		if (round < 16) {	// Block decomposition, fists 16 Wi
			//bits11 idx = 511 - (round * 32);//0xFFFFFFFF
			//Wi = block.range(idx, idx-31);
			Wi = (block >> (480 - (round * 32))) & 0xFFFFFFFF;
		} else {	// Remaining Wi
			Wi = sigma_1lower(previousWi[1]) + previousWi[6] + sigma_0lower(previousWi[14]) + previousWi[15];
		}

		WDelayLoop:
		for (bits4 i = 15; i > 0; i--) {
		#pragma HLS UNROLL factor=15
			previousWi[i] = previousWi[i - 1];
		}

		previousWi[0] = Wi;

		T1 = H[7] + sigma_1upper(H[4]) + ch(H[4], H[5], H[6]) + K[round] + Wi;
		T2 = sigma_0upper(H[0]) + maj(H[0], H[1], H[2]);

		//cout << i << "\t";
		//cout << "T1: " << T1.to_string(10) << "\t";
		//cout << "T2: " << T2.to_string(10) << endl;

		H[7] = H[6];
		H[6] = H[5];
		H[5] = H[4];
		H[4] = H[3] + T1;
		H[3] = H[2];
		H[2] = H[1];
		H[1] = H[0];
		H[0] = T1 + T2;

	}

}

