// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/sb_crypt.h, with Black's names.
#ifndef BWPORT_DWC_SB_CRYPT_H
#define BWPORT_DWC_SB_CRYPT_H

#include "dwc/nonport.h"

// The stream cipher of GameSpy's server browser (sb_crypt.c), in overlay 11, that the master server encrypts its
// lists with: a permutation of the bytes, which every byte shuffles a little more, and five indexes

typedef struct _GOACryptState {
    gsi_u8 cards[256];  // a permutation of 0 to 255
    gsi_u8 rotor;       // moves on by one
    gsi_u8 ratchet;     // moves erratically
    gsi_u8 avalanche;   // depends on the data
    gsi_u8 last_plain;  // the last byte of plain text
    gsi_u8 last_cipher; // the last byte of cipher text
} GOACryptState;

// Sets the cipher up from a key, or as the hash's fixed permutation for a key of no bytes
void GOACryptInit(GOACryptState *state, unsigned char *key, unsigned char keysize);
void GOAHashInit(GOACryptState *state);
// Decrypts one byte, and a buffer in place
unsigned char GOADecryptByte(GOACryptState *state, unsigned char b);
void GOADecrypt(GOACryptState *state, unsigned char *bp, int len);

#endif // BWPORT_DWC_SB_CRYPT_H
