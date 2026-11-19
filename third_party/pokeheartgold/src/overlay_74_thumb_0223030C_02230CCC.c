#include "global.h"

s32 ov74_02230BB4(void);

s32 ov74_02230C10(void);

void *ov74_02231184(void);

static void ov74_02230CCC(void);

static void ov74_02230CCC(void) {
    u8 v0;

    v0 = *(u8 *)((u8 *)(ov74_02231184()) + 0x18);
    switch (v0) {
    case 1:
        ov74_02230BB4();
        return;
    case 2:
        ov74_02230C10();
        return;
    }
}

u8 ov74_02231064(void);

void *ov74_02231154(void);

static void ov74_02230CEC(void);

static void ov74_02230CEC(void) {
    s32 v0;
    u16 v1;
    void *v2;

    v2 = ov74_02231154();
    v0 = ov74_02231064();
    switch (v0) {
    case 6:
        v1 = *(u16 *)((u8 *)v2 + 0x1C0);
        if (v1 != 0) {
            *(u16 *)((u8 *)v2 + 0x1C0) = (u16)(v1 - 1);
            return;
        }
        return;
    case 11:
        ov74_02230CCC();
        break;
    }
}

static void ov74_02230D18(void);

static void ov74_02230D18(void) {
    if (ov74_02231064() == 0xA) {
        ov74_02230CCC();
    }
}

s32 *ov74_0223105C(void);

u8 ov74_0223107C(void);

u8 ov74_02231094(void);

s32 ov74_02231724(void);

void ov74_02230D28(void);

void ov74_02230D28(void) {
    s32 v0;

    if (ov74_02231064() != 0xC) {
        if (ov74_0223107C() == 0xC) {
            if (ov74_02231064() == 7) {
                ov74_02231724();
            }
        } else {
            v0 = ov74_02231094();
            switch (v0) {
            case 1:
                ov74_02230CEC();
                return;
            case 2:
                ov74_02230D18();
                return;
            default:
                ov74_0223105C();
                ov74_02231064();
                break;
            }
        }
    }
}

s32 ov74_02230A34(void);

s32 ov74_022310A0(s8);

s32 ov74_02231164(void);

void ov74_02230D6C(void);

void ov74_02230D6C(void) {
    ov74_02231164();
    ov74_022310A0(1);
    ov74_02230A34();
}
