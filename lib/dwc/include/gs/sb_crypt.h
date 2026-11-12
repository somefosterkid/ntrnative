// GameSpy's crypt (sb_crypt.c): the functions the server list calls.
#ifndef BWPORT_GS_SB_CRYPT_H
#define BWPORT_GS_SB_CRYPT_H

#include "types.h"

typedef struct GOACryptState {
    u8 cards[0x100];
    u8 rotor;
    u8 ratchet;
    u8 avalanche;
    u8 last_plain;
    u8 last_cipher;
} GOACryptState;

void GOAHashInit(GOACryptState *state);
void GOACryptInit(GOACryptState *state, char *key, u8 keysize);
u8 GOADecryptByte(GOACryptState *state, u8 b);
void GOADecrypt(GOACryptState *state, void *buf, int len);

#endif // BWPORT_GS_SB_CRYPT_H
