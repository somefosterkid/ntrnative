#include "global.h"

#include "unk_02034354.h"

void ov85_021E9FCC(void);

void ov85_021E9FCC(void) {
}

int ov85_021E9FD0(void);

int ov85_021E9FD0(void) {
    int v0, v1;

    for (v1 = 0, v0 = 0; v0 < 5; v0++) {
        if (sub_02034818(v0) != NULL) {
            v1++;
        }
    }

    return v1;
}

u32 ov85_021E9FEC(void);

u32 ov85_021E9FEC(void) {
    int v0;
    u32 v1;

    for (v1 = 0, v0 = 0; v0 < 5; v0++) {
        if (sub_02034818(v0) != NULL) {
            v1 |= 1 << v0;
        }
    }

    return v1;
}
