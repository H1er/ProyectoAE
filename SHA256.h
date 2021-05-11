#ifndef SHA256_H
#define SHA256_H

#include "ap_int.h"

typedef ap_uint<512> bits512;
typedef ap_uint<256> bits256;
typedef ap_uint<32> bits32;
typedef ap_uint<9> bits9;
typedef ap_uint<6> bits6;
typedef ap_uint<5> bits5;
typedef ap_uint<4> bits4;
typedef ap_uint<1> bits1;

void get_Wi(bits6 &round, bits32 &Wi, bits32 &firstsWi);

bits32 sigma_0lower (bits32 x);

bits32 sigma_1lower (bits32 x);

bits32 sigma_0upper (bits32 x);

bits32 sigma_1upper (bits32 x);

bits32 rotr (bits32 x, bits5 shift);

bits32 maj (bits32 x, bits32 y, bits32 z);

void decomposition (bits6 &round, bits32 &firstsWi, bits512 block);

void blockprocessing (bits512 block, bits1 final, bits256 &hash);

#endif
