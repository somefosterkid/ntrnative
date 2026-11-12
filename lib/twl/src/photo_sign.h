// The DSi's photo signing, a part of the SSP library that TwlSDK's headers (and libntr's) don't have: its own header,
// beside its sources.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/photo_sign.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_SRC_PHOTO_SIGN_H
#define BWPORT_TWL_SRC_PHOTO_SIGN_H

#include "types.h"

#define PHOTO_SIGN_NONCE_SIZE 12
#define PHOTO_SIGN_MAC_SIZE 16

// What the ARM7 and the functions return
enum {
    PHOTO_SIGN_RESULT_SUCCESS = 1,
    PHOTO_SIGN_RESULT_MISMATCH = 2,
    PHOTO_SIGN_RESULT_BUSY = 4,
};

// pokebw2's PhotoSign_Sign
// Signs data, whose signature block is zeroed: writes a new nonce and the MAC into sig
int func_02707774(u8 *sig, const u8 *data, u32 size);
// pokebw2's PhotoSign_Verify
// Computes the MAC of data with sig's nonce and compares it with sig's; PHOTO_SIGN_RESULT_MISMATCH when they differ
int func_0270788c(const u8 *sig, const u8 *data, u32 size);

// pokebw2's PhotoSign_Init
// Sets up the ARM7's side once
void func_027073ac(void);

#endif // BWPORT_TWL_SRC_PHOTO_SIGN_H
