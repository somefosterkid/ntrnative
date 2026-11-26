#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "field_types_def.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2330(void *, s32, s32);
s32 ov14_021F2490(void *, s32, s32);
u32 ov14_021F6A14(void);

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7588(void *arg0, s32 arg1);
s32 ov14_021E765C(void *);
void ov14_021E76B8(void *);
s32 ov14_021F0244(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F083C(void *, u8);
#else
s32 ov14_021F083C(void *, s32);
#endif
s32 ov14_021F0D34(void *, s32);
s32 ov14_021F0D58(void *, s32);
s32 ov14_021F0EE8(void *, s32);
s32 ov14_021F1004(void *, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov14_021F1580(void *, u8);
#else
s32 ov14_021F1580(void *, u32);
#endif
s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
u32 ov14_021F70C0(void *);
s32 ov14_021F7B7C(void *);

#define ST(p) ((void *)((UnkStruct_ov14_boxapp *)(p))->sys)

#define GH(p) (((UnkStruct_ov14_boxapp *)(p))->sys->unk_2C)

#define EX(p) (((UnkStruct_ov14_boxapp *)(p))->sys->unk_2F0)

#define NEXT(p) GridInputHandler_SetNextInput(GH(p), (u8)(*((u8 *)(p) + 0x25) % 6 + 0x25))

s32 ov14_021EC8D0(UnkStruct_ov14_boxapp *arg0) {
    s32 r5;
    u32 r1;
    u32 r6;

    r5 = ov14_021F6A34(arg0);
    if (r5 != -1) {
        if (ov14_021E6070(arg0, r5 + 0x1E, 0xAC, 0) != 0) {
            void *t;
            PlaySE(0x5EB);
            t = ST(arg0);
            System_GetTouchNewCoords((s32 *)((u8 *)t + 0x40B8), (s32 *)((u8 *)t + 0x40BC));
            ov14_021E7588(arg0, r5 + 0x1E);
            ov14_021F2A18(ST(arg0), 9, 0);
            return ov14_021F083C(arg0, r5 + 0x1E);
        }
        ov14_021E765C(arg0);
        GridInputHandler_SetNextInput(GH(arg0), (u8)(r5 + 0x1E));
        GridInputHandler_SetButtonInputMode(GH(arg0), 1);
        if (ov14_021E85E4(EX(arg0)) == 1) {
            return ov14_021F0EE8(arg0, 0x29);
        }
        if (ov14_021E8648(EX(arg0)) == 1) {
            return ov14_021F0D34(arg0, 0x29);
        }
        return 0x29;
    }
    r6 = ov14_021F6A14();
    if (r6 != -1) {
        if (ov14_021E6070(arg0, r6, 0xAC, 0) != 0) {
            void *t;
            PlaySE(0x5EB);
            t = ST(arg0);
            System_GetTouchNewCoords((s32 *)((u8 *)t + 0x40B8), (s32 *)((u8 *)t + 0x40BC));
            ov14_021E7588(arg0, r6);
            ov14_021F2A18(ST(arg0), 9, 0);
            return ov14_021F083C(arg0, r6);
        }
        ov14_021E765C(arg0);
        GridInputHandler_SetNextInput(GH(arg0), (u8)r6);
        GridInputHandler_SetButtonInputMode(GH(arg0), 1);
        if (ov14_021E85E4(EX(arg0)) == 1) {
            return ov14_021F0EE8(arg0, 0x29);
        }
        if (ov14_021E8648(EX(arg0)) == 1) {
            return ov14_021F0D34(arg0, 0x29);
        }
        return 0x29;
    }
    if (ov14_021F7B7C(arg0) == 1) {
        r1 = GridInputHandler_GetNextInput(GH(arg0));
        if (ov14_021E6070(arg0, r1, 0xAC, 0) != 0) {
            PlaySE(0x5DD);
            arg0->unk21 = r1;
            arg0->unk26[0] = 1;
            return ov14_021F2330(arg0, 0xF, 0x97);
        }
        return 0x29;
    }
    r6 = ov14_021F70C0(arg0);
    switch (r6) {
    case 0x25:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        return ov14_021F0D58(arg0, 0);
    case 0x26:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        return ov14_021F0D58(arg0, 1);
    case 0x27:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        return ov14_021F0D58(arg0, 2);
    case 0x28:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        return ov14_021F0D58(arg0, 3);
    case 0x29:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        return ov14_021F0D58(arg0, 4);
    case 0x2A:
        PlaySE(0x5DD);
        ov14_021E76B8(arg0);
        return ov14_021F0D58(arg0, 5);
    case 0x2B:
        PlaySE(0x5DC);
        ov14_021E76B8(arg0);
        ov14_021F1004(arg0, -1);
        NEXT(arg0);
        GridInputHandler_SetButtonInputMode(GH(arg0), 1);
        if (ov14_021E85E4(EX(arg0)) == 1) {
            return ov14_021F0EE8(arg0, 0x29);
        }
        if (ov14_021E8648(EX(arg0)) == 1) {
            return ov14_021F0D34(arg0, 0x29);
        }
        return 0x29;
    case 0x2C:
        PlaySE(0x5DC);
        ov14_021E76B8(arg0);
        ov14_021F1004(arg0, 1);
        NEXT(arg0);
        GridInputHandler_SetButtonInputMode(GH(arg0), 1);
        if (ov14_021E85E4(EX(arg0)) == 1) {
            return ov14_021F0EE8(arg0, 0x29);
        }
        if (ov14_021E8648(EX(arg0)) == 1) {
            return ov14_021F0D34(arg0, 0x29);
        }
        return 0x29;
    case 0x2D:
        PlaySE(0x5DC);
        NEXT(arg0);
        return ov14_021F2270(arg0, 0xE, 0xA0);
    case -3:
        r1 = GridInputHandler_GetNextInput(GH(arg0));
        if (r1 < 0x25) {
            if (ov14_021E7588(arg0, r1) == 1) {
                if (ov14_021E8648(EX(arg0)) == 0) {
                    ov14_021F6408(arg0, 0);
                    ov14_021E8620(EX(arg0));
                }
            } else if (ov14_021E8648(EX(arg0)) == 1) {
                ov14_021E8634(EX(arg0));
            }
        } else {
            ov14_021E765C(arg0);
            if (ov14_021E8648(EX(arg0)) == 1) {
                ov14_021E8634(EX(arg0));
            }
        }
        PlaySE(0x5DC);
        if (arg0->sys->unk_8 != NULL) {
            return ov14_021F0244(arg0, 0x4B);
        }
        break;
    case 0x24:
        PlaySE(0x633);
        ov14_021F29E4(ST(arg0), 9, 8);
        return ov14_021F2490(arg0, 1, 0xA1);
    case -2:
        if (ov14_021E85E4(EX(arg0)) == 0) {
            PlaySE(0x633);
            return ov14_021F2490(arg0, 1, 0xA1);
        }
        PlaySE(0x5DC);
        NEXT(arg0);
        GridInputHandler_SetButtonInputMode(GH(arg0), 1);
        return ov14_021F0EE8(arg0, 0x29);
    default:
        if (ov14_021E6070(arg0, r6, 0xAC, 0) != 0) {
            PlaySE(0x5EB);
            ov14_021E7588(arg0, r6);
            return ov14_021F1580(arg0, r6);
        }
        break;
    case -4:
    case -1:
        break;
    }
    return 0x29;
}
