#include "global.h"

typedef u16 charcode_t;

typedef struct RangerPlatinumCharcodePair {
    u16 rangerCharcode;
    charcode_t platinumCharcode;
} RangerPlatinumCharcodePair;

extern RangerPlatinumCharcodePair ov74_0223BD68[291];

u16 ov74_02228C84(charcode_t platinumCharcode);

void ov74_IntToWstring5(u16 *dest, int id);

u16 ov74_02228C84(charcode_t platinumCharcode) {
    for (int i = 0; i < sizeof(ov74_0223BD68) / sizeof(RangerPlatinumCharcodePair); i++) {
        if (platinumCharcode == ov74_0223BD68[i].platinumCharcode) {
            return ov74_0223BD68[i].rangerCharcode;
        }
    }

    return 63; // Ranger question mark
}

void ov74_IntToWstring5(u16 *dest, int id) {

    dest[0] = id / 10000 + 48;
    id = id % 10000;
    dest[1] = id / 1000 + 48;
    id = id % 1000;
    dest[2] = id / 100 + 48;
    id = id % 100;
    dest[3] = id / 10 + 48;
    id = id % 10;
    dest[4] = id + 48;
}
