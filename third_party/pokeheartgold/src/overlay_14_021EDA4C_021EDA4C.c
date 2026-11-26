#include "global.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021F6A14(void);

s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021E6070(void *, u32, s32, void *);
s32 ov14_021E7588(void *arg0, s32 arg1);
s32 ov14_021E765C(void *);
void ov14_021E76B8(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0244(void *, s32);
s32 ov14_021F028C(void *, s32);
s32 ov14_021F0314(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F0418(void *, u8);
#else
s32 ov14_021F0418(void *, u32);
#endif
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F04D4(void *, u8);
#else
s32 ov14_021F04D4(void *, u32);
#endif
s32 ov14_021F1128(void *);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021EA130(UnkStruct_ov14_boxapp *app);

extern s32 ov14_021F7D2C;

#define ST(p) ((void *)((UnkStruct_ov14_boxapp *)(p))->sys)

#define GH(p) (((UnkStruct_ov14_boxapp *)(p))->sys->unk_2C)

#define EX(p) (((UnkStruct_ov14_boxapp *)(p))->sys->unk_2F0)

static void ov14_prev_input(void *arg0) {
}

s32 ov14_021EDA4C(UnkStruct_ov14_boxapp *arg0) {
    u32 r5;
    u32 r1;

    r5 = ov14_021F6A14();
    if (r5 != -1) {
        if (ov14_021E6070(arg0, r5, 0xAC, 0) != 0) {
            void *t = ST(arg0);
            System_GetTouchNewCoords((s32 *)((u8 *)t + 0x40B8), (s32 *)((u8 *)t + 0x40BC));
            if (ov14_021E8544(EX(arg0)) == 0) {
                ov14_021F5EE4(arg0, &ov14_021F7D2C, 4);
            }
            PlaySE(0x5EB);
            ov14_021E7588(arg0, r5);
            ov14_021F2A18(ST(arg0), 9, 0);
            return ov14_021F0418(arg0, r5);
        }
        if (ov14_021E8544(EX(arg0)) == 1) {
            r5 = arg0->unk21;
            ov14_021F7AC4(ST(arg0), r5, GridInputHandler_GetNextInput(GH(arg0)));
            GridInputHandler_SetNextInput(GH(arg0), r5);
            ov14_021E84A4(EX(arg0));
            ov14_021E8328(EX(arg0));
            ov14_021E884C(ST(arg0));
            ov14_021F40E8(arg0, 0);
            return ov14_021F0234(arg0, ov14_021EA130, 0x59);
        }
        GridInputHandler_SetNextInput(GH(arg0), (u8)r5);
        GridInputHandler_SetButtonInputMode(GH(arg0), 1);
        ov14_021E765C(arg0);
        return 0x51;
    }
    r5 = ov14_021F6E8C(arg0);
    switch (r5) {
    case 0x22:
        if (Party_GetCount(arg0->party) != 6) {
            PlaySE(0x5DD);
        } else {
            PlaySE(0x5F3);
        }
        return ov14_021F2270(arg0, 4, 0xA7);
    case 0x23:
        PlaySE(0x5DD);
        arg0->unk2C = 0x23;
        return ov14_021F2270(arg0, 5, 0x97);
    case 0x24:
        PlaySE(0x5DD);
        ov14_021F6654(ST(arg0), 0x27);
        return ov14_021F2270(arg0, 6, 0x99);
    case 0x25:
        PlaySE(0x5DD);
        return ov14_021F2270(arg0, 7, 0x9B);
    case 0x26:
        r5 = arg0->unk21;
        ov14_021F7AC4(ST(arg0), r5, GridInputHandler_GetNextInput(GH(arg0)));
        GridInputHandler_SetNextInput(GH(arg0), r5);
        PlaySE(0x5DC);
        return ov14_021F2270(arg0, 0xB, 0xA8);
    case 0x1E:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        ov14_021F6654(ST(arg0), 0x27);
        return ov14_021F1128(arg0);
    case 0x1F:
        PlaySE(0x5DC);
        ov14_021E76B8(arg0);
        ov14_021F2A18(ST(arg0), 9, 0);
        GridInputHandler_SetNextInput(GH(arg0), 0x1E);
        ov14_021F685C(arg0, 0, 0, 0x27);
        return ov14_021F028C(arg0, 0x51);
    case 0x20:
        PlaySE(0x5DC);
        ov14_021E76B8(arg0);
        ov14_021F2A18(ST(arg0), 9, 0);
        GridInputHandler_SetNextInput(GH(arg0), 0x1E);
        ov14_021F685C(arg0, 0, 0, 0x27);
        return ov14_021F0314(arg0, 0x51);
    case -3:
        r1 = GridInputHandler_GetNextInput(GH(arg0));
        if (r1 < 0x1E) {
            ov14_021E7588(arg0, r1);
        } else if (r1 != 0x22 && r1 != 0x23 && r1 != 0x24 && r1 != 0x25 && r1 != 0x26) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DC);
        return ov14_021F0244(arg0, 0x58);
    case 0x21:
        PlaySE(0x5DD);
        ov14_021E765C(arg0);
        return ov14_021F2270(arg0, 0xA, 0x93);
    case -2:
        PlaySE(0x5DD);
        return ov14_021F2270(arg0, 0xA, 0x94);
    case -1:
        if (GridInputHandler_GetNextInput(GH(arg0)) == 0x1E) {
            if (gSystem.newKeys & 0x20) {
                PlaySE(0x5DC);
                ov14_021E76B8(arg0);
                return ov14_021F028C(arg0, 0x51);
            }
            if (gSystem.newKeys & 0x10) {
                PlaySE(0x5DC);
                ov14_021E76B8(arg0);
                return ov14_021F0314(arg0, 0x51);
            }
        }
        break;
    case -4:
        r1 = GridInputHandler_GetNextInput(GH(arg0));
        if (r1 < 0x1E) {
            ov14_021E7588(arg0, r1);
        } else if (r1 != 0x22 && r1 != 0x23 && r1 != 0x24 && r1 != 0x25 && r1 != 0x26) {
            ov14_021E765C(arg0);
        }
        PlaySE(0x5DC);
        break;
    default:
        if (ov14_021E6070(arg0, r5, 0xAC, 0) != 0) {
            PlaySE(0x5DD);
            ov14_021F5EE4(arg0, &ov14_021F7D2C, 4);
            ov14_021E7588(arg0, r5);
            ov14_021F7AC4(ST(arg0), 0x22, GridInputHandler_GetNextInput(GH(arg0)));
            GridInputHandler_SetNextInput(GH(arg0), 0x22);
            return ov14_021F04D4(arg0, r5);
        }
        break;
    }
    return 0x51;
}
