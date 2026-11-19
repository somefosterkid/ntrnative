#include "global.h"

u16 GetOakJohtoDexRating(u16 numCaught, u16 gender, u16 *var_p);

u16 GetOakJohtoDexRating(u16 numCaught, u16 gender, u16 *var_p) {
    *var_p = 0x4aa;
    if (numCaught <= 9) {
        return 0x1c;
    }
    if (numCaught <= 0x13) {
        return 0x1d;
    }
    if (numCaught <= 0x22) {
        return 0x1e;
    }
    if (numCaught <= 0x31) {
        return 0x1f;
    }
    if (numCaught <= 0x40) {
        return 0x20;
    }
    if (numCaught <= 0x4F) {
        return 0x21;
    }
    if (numCaught <= 0x5E) {
        return 0x22;
    }
    if (numCaught <= 0x6D) {
        return 0x23;
    }
    if (numCaught <= 0x7C) {
        return 0x24;
    }
    if (numCaught <= 0x8B) {
        return 0x25;
    }
    if (numCaught <= 0x9A) {
        return 0x26;
    }
    if (numCaught <= 0xA9) {
        return 0x27;
    }
    if (numCaught <= 0xB8) {
        return 0x28;
    }
    if (numCaught <= 0xC7) {
        return 0x29;
    }
    if (numCaught <= 0xD6) {
        return 0x2a;
    }
    if (numCaught <= 0xE5) {
        return 0x2b;
    }
    if (numCaught <= 0xF4) {
        return 0x2c;
    }
    if (numCaught <= 0xFD) {
        return 0x2d;
    }
    *var_p = 0x4af;
    if (gender != 0) {
        return 0x17;
    }
    return 0x16;
}

u16 GetOakNationalDexRating(u16 numCaught, u16 gender, u16 *var_p);

u16 GetOakNationalDexRating(u16 numCaught, u16 gender, u16 *var_p) {
    *var_p = 0x4aa;
    if (numCaught <= 0x64) {
        return 0x2e;
    }
    if (numCaught <= 0x96) {
        return 0x2f;
    }
    if (numCaught <= 0xC8) {
        return 0x30;
    }
    if (numCaught <= 0xFA) {
        return 0x31;
    }
    if (numCaught <= 0x12C) {
        return 0x32;
    }
    if (numCaught <= 0x15E) {
        return 0x33;
    }
    if (numCaught <= 0x190) {
        return 0x34;
    }
    if (numCaught <= 0x1B3) {
        return 0x35;
    }
    if (numCaught <= 0x1D1) {
        return 0x36;
    }
    if (numCaught <= 0x1DB) {
        return 0x37;
    }
    if (numCaught <= 0x1E3) {
        return 0x38;
    }
    *var_p = 0x4af;
    if (gender != 0) {
        return 0x19;
    }
    return 0x18;
}
