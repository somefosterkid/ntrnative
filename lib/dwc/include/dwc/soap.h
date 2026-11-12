// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/soap.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_SOAP_H
#define BWPORT_DWC_SOAP_H

#include "types.h"

typedef struct SoapWriter SoapWriter;
typedef struct SoapReader SoapReader;

// A big number of 0x104 bytes, as the signature check reads and writes them
typedef struct BigNum {
    u32 words[0x41];
} BigNum;

// The public key of the signature check, a modulus and an exponent
typedef struct RsaPublicKey {
    BigNum modulus;
    BigNum exponent;
} RsaPublicKey;

// The state of the hash that gives 16 bytes, and its digest
typedef struct HashContext {
    u32 words[0x16];
} HashContext;

#endif // BWPORT_DWC_SOAP_H
