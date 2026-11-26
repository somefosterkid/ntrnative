#include "global.h"

#include "overlay_49.h"
#include "camera.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_49_unk_struct_ov70_0226da18.h"

s32 ov45_0222A4B8(s32);

s32 ov45_0222A520(s32, s32);

s32 ov45_0222D844(void);

s32 ov45_0222EB94(void);

s32 ov45_0222EBC4(void);

s32 ov49_02258DAC();

s32 ov49_02258E60(s32, s32);

s32 ov49_02258EAC(s32, s32, s32, int);

s32 ov49_02258EEC(s32, s32, s32);

s32 ov49_02259FE8(const struct UnkStruct_ov70_0225DEE8_t *);

void *ov49_02259FEC(const struct UnkStruct_ov70_0225DEE8_t *);

s32 ov49_02259FF0(void *);

s32 ov49_0225A010(void *);

s32 ov49_0225A018(void *, s32);

u8 ov49_0225A02C(void *);

s32 ov49_0225A030(void *);

s32 ov49_0225A08C(void *, const String *);

s32 ov49_0225A0AC(void *);

s32 ov49_0225A0EC(void *);

String *ov49_0225A30C(void *, s32, s32);

String *ov49_0225A37C(void *, int, s32);

u32 ov49_0225EF88(void *);

s32 ov49_0225EF8C(void *, s32);

s32 ov49_0225EF90(void *);

s32 ov49_0225EF98(s32, s32, const UnkStruct_ov70_0226DA18 *, s32);

s32 ov49_0225F334(void *, s32, s32, s32, s32);

extern const UnkStruct_ov70_0226DA18 ov49_02269B38;

BOOL ov49_0225FA14(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2) {
    s32 v2;
    s32 v1;
    s32 v0;
    s32 v4;
    s32 v7;
    u32 v3;
    void *v5;
    void *v6;

    v0 = arg2;
    v1 = ov49_02259FE8(arg1);
    v5 = ov49_02259FEC(arg1);
    v2 = ov49_02259FF0(arg1);
    v7 = ov49_02258DAC();
    v3 = ov49_0225EF88(arg0);
    switch (v3) {
    case 0:
        ov49_0225F334(arg1, v0, *(u16 *)v5, *(u16 *)((u8 *)v5 + 2), 1);
        if (ov45_0222D844() == 0) {
            ov45_0222EB94();
        }
        ov45_0222A520(v1, 0);
        ov49_0225EF90(arg0);
        break;
    case 1:
        if ((ov45_0222D844() != 0) || (ov45_0222EBC4() != 0)) {
            v6 = ov49_02259FEC(arg1);
            if (ov49_0225A030(arg1) == 0) {
                ov49_0225EF90(arg0);
                ov49_02258EAC(v2, v7, 2, *(u16 *)((u8 *)v6 + 4));
            }
        }
        break;
    case 2:
        if (ov49_02258E60(v7, 5) == 0) {
            ov49_0225A018(arg1, 1);
            if (ov45_0222A4B8(v1) == 1) {
                ov49_0225EF90(arg0);
            } else {
                ov49_0225EF8C(arg0, 5);
            }
        }
        break;
    case 3:
        ov49_0225A37C(arg1, *(u16 *)((u8 *)v5 + 8), 0);
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 0, 0x15));
        ov49_0225EF90(arg0);
        break;
    case 4:
        if (ov49_0225A0AC(arg1) == 1) {
            ov49_0225A0EC(arg1);
            ov49_0225EF90(arg0);
        }
        break;
    case 5:
        ov49_02258EEC(v2, v7, 1);
        v4 = ov49_0225A010(arg1);
        ov49_0225EF98(v4, ov49_0225A02C(arg1), &ov49_02269B38, 0);
        break;
    }
    return 0;
}

s32 ov45_0222A424(s32);

s32 ov45_0222A4C8(s32, s32);

s32 ov45_0222A4D0(s32);

s32 ov49_0225EFC4(s32, s32, const UnkStruct_ov70_0226DA18 *, void *);

extern const UnkStruct_ov70_0226DA18 ov49_02269B68;

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov49_02262AC4(struct UnkStruct_ov70_02263344_t * param0, struct UnkStruct_ov70_0225DEE8_t * param1, u32 param2);
BOOL ov49_0225F518(struct UnkStruct_ov70_02263344_t * param0, struct UnkStruct_ov70_0225DEE8_t * param1, u32 param2);
BOOL ov49_0225FA14(struct UnkStruct_ov70_02263344_t * param0, struct UnkStruct_ov70_0225DEE8_t * param1, u32 param2);
BOOL ov49_0225FB5C(struct UnkStruct_ov70_02263344_t * param0, struct UnkStruct_ov70_0225DEE8_t * param1, u32 param2);
BOOL ov49_0225FCA8(struct UnkStruct_ov70_02263344_t * param0, struct UnkStruct_ov70_0225DEE8_t * param1, u32 param2);

// asm keeps the DS copy (4-byte callback pointer and a literal 0); the 64-bit port emits 4-byte pointers (wordmix.py): the PC
// build uses this C definition instead.
const UnkStruct_ov70_0226DA18 ov49_02269B68 = { ov49_02262AC4, NULL };
const UnkStruct_ov70_0226DA18 ov49_02269BE0[4] = {
    { ov49_0225F518, NULL },
    { ov49_0225FA14, NULL },
    { ov49_0225FB5C, NULL },
    { ov49_0225FCA8, NULL },
};
#endif

BOOL ov49_0225FB5C(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2) {
    s32 v0;
    s32 v3;
    s32 v2;
    s32 v1;
    s32 v5;
    s32 v8;
    u32 v4;
    void *v6;
    void *v7;

    v1 = arg2;
    v2 = ov49_0225A010(arg1);
    v3 = ov49_02259FF0(arg1);
    v8 = ov49_02258DAC();
    v0 = ov49_02259FE8(arg1);
    v6 = ov49_02259FEC(arg1);
    v4 = ov49_0225EF88(arg0);
    switch (v4) {
    case 0:
        ov49_0225F334(arg1, v1, *(u16 *)v6, *(u16 *)((u8 *)v6 + 2), 1);
        ov49_0225EF90(arg0);
        ov49_0225EFC4(v2, v1, &ov49_02269B68, 0);
        ov45_0222A4C8(v0, 0);
        ov45_0222A4D0(v0);
        break;
    case 1:
        v7 = ov49_02259FEC(arg1);
        if (ov49_0225A030(arg1) == 0) {
            ov49_0225EF90(arg0);
            ov49_02258EAC(v3, v8, 2, *(u16 *)((u8 *)v7 + 4));
        }
        break;
    case 2:
        if (ov49_02258E60(v8, 5) == 0) {
            ov49_0225A018(arg1, 1);
            if (ov45_0222A424(v0) == 1) {
                ov49_0225EF8C(arg0, 3);
            } else {
                ov49_0225EF8C(arg0, 5);
            }
        }
        break;
    case 3:
        ov49_0225A37C(arg1, *(u16 *)((u8 *)v6 + 8), 0);
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 0, 8));
        ov49_0225EF90(arg0);
        break;
    case 4:
        if (ov49_0225A0AC(arg1) == 1) {
            ov49_0225A0EC(arg1);
            ov49_0225EF90(arg0);
        }
        break;
    case 5:
        ov49_02258EEC(v3, v8, 1);
        v5 = ov49_0225A010(arg1);
        ov49_0225EF98(v5, ov49_0225A02C(arg1), &ov49_02269B38, 0);
        break;
    }
    return 0;
}

BOOL ov49_0225FCA8(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2) {
    s32 v2;
    void *v1;
    s32 v0;
    s32 v4;
    s32 v5;
    s32 v6;
    u32 v3;
    void *v7;

    v0 = arg2;
    v6 = ov49_02259FE8(arg1);
    v1 = ov49_02259FEC(arg1);
    v2 = ov49_02259FF0(arg1);
    v5 = ov49_02258DAC();
    v3 = ov49_0225EF88(arg0);
    switch (v3) {
    case 0:
        ov49_0225F334(arg1, v0, *(u16 *)v1, *(u16 *)((u8 *)v1 + 2), 1);
        ov45_0222A520(v6, 0);
        ov49_0225EF90(arg0);
        break;
    case 1:
        v7 = ov49_02259FEC(arg1);
        if (ov49_0225A030(arg1) == 0) {
            ov49_0225EF90(arg0);
            ov49_02258EAC(v2, v5, 2, *(u16 *)((u8 *)v7 + 4));
        }
        break;
    case 2:
        if (ov49_02258E60(v5, 5) == 0) {
            ov49_0225A018(arg1, 1);
            if (ov45_0222A4B8(v6) == 1) {
                ov49_0225EF90(arg0);
            } else {
                ov49_0225EF8C(arg0, 5);
            }
        }
        break;
    case 3:
        ov49_0225A08C(arg1, ov49_0225A30C(arg1, 1, 0x5B));
        ov49_0225EF90(arg0);
        break;
    case 4:
        if (ov49_0225A0AC(arg1) == 1) {
            ov49_0225A0EC(arg1);
            ov49_0225EF90(arg0);
        }
        break;
    case 5:
        ov49_02258EEC(v2, v5, 1);
        v4 = ov49_0225A010(arg1);
        ov49_0225EF98(v4, ov49_0225A02C(arg1), &ov49_02269B38, 0);
        break;
    }
    return 0;
}
