#ifndef SHA256_H
#define SHA256_H

typedef ap_uint<512> bits512;
typedef ap_uint<512> bits256;
typedef ap_uint<64> bits64;
typedef ap_uint<32> bits32;

void sha256(string msg);

#endif
