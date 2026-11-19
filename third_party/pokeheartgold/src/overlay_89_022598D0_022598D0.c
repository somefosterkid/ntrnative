#include "global.h"

#include "bg_window.h"
#include "overlay_89.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_89_unk_struct_ov113_0225_dbcc_t.h"

s32 ov89_02259A3C(int, int, ManagedSprite *, NARC *, NARC *, s32, s32);

s32 ov89_0225A1D8(void *, s32);

extern u16 _0225C9B0;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov89_022598D0(UnkStruct_ov113_0225DBCC_t *arg0) {
    s32 v6;
    s16 *v5;
    s32 v4;
    s32 x;
    u16 *v1;
    u16 *v0;
    s32 v9;
    s32 v10;
    u16 *v7;
    u16 *v11;
    u16 v8;

    v4 = 0;
    v7 = PaletteData_GetUnfadedBuf(arg0->unk_0C, PLTTBUF_MAIN_OBJ);
    v5 = v7;
    v0 = PaletteData_GetFadedBuf(arg0->unk_0C, PLTTBUF_MAIN_OBJ);
    v6 = 0;
    x = 0;
    v1 = v7;
loop_1:
    v8 = arg0->unk_8D8[v6].unk_02;
    if ((v8 == 0) || ((u32)v8 > 0x1EDU)) {
        ManagedSprite_SetDrawFlag(arg0->unk_924[v6], 0);
        ManagedSprite_SetDrawFlag(arg0->unk_93C[v6], 0);
        ManagedSprite_SetDrawFlag(arg0->unk_954[v6], 0);
        v11 = &_0225C9B0;
        v10 = 0;
        do {
            FillBgTilemapRect(arg0->unk_08, 2U, *v11, (u8)x, (u8)(u8)(v10 + 0x14), 4U, 1U, 0x11U);
            v10 += 1;
            v11 += 1;
        } while (v10 < 4);
    } else {
        ov89_02259A3C(v8, arg0->unk_8D8[v6].unk_08, arg0->unk_954[v6], arg0->unk_160, arg0->unk_164, 1, arg0->unk_19E0);
        *(u16 *)((u8 *)v1 + 0x22) = arg0->unk_8D8[v6].unk_00;
        *(u16 *)((u8 *)v0 + 0x22) = arg0->unk_8D8[v6].unk_00;
        BlendPalette(&v5[v6 + 0x11], &v5[v6 + 0x21], 1U, 0xCU, 0U);
        *(u16 *)((u8 *)v0 + 0x42) = (u16) * (u16 *)((u8 *)v1 + 0x42);
        if (v4 == 0) {
            v4 += 1;
            arg0->unk_920 = (s8)v6;
        }
    }
    x += 4;
    v1 += 1;
    v0 += 1;
    v9 = v6 + 1;
    v6 = v9;
    if (v9 < 6) {
        goto loop_1;
    }
    ov89_0225A1D8(arg0, 0);
    ScheduleBgTilemapBufferTransfer(arg0->unk_08, 2U);
}
#else
void ov89_022598D0(UnkStruct_ov113_0225DBCC_t *arg0) {
    s32 v6;
    s16 *v5;
    s32 v4;
    void *v3;
    void *v2;
    s32 x;
    u16 *v1;
    u16 *v0;
    s32 v9;
    s32 v10;
    u16 *v7;
    u16 *v11;
    u16 v8;

    v4 = 0;
    v7 = PaletteData_GetUnfadedBuf(arg0->unk_0C, PLTTBUF_MAIN_OBJ);
    v5 = v7;
    v0 = PaletteData_GetFadedBuf(arg0->unk_0C, PLTTBUF_MAIN_OBJ);
    v6 = 0;
    x = 0;
    v3 = arg0;
    v2 = arg0;
    v1 = v7;
loop_1:
    v8 = *(u16 *)((u8 *)v3 + 0x8DA);
    if ((v8 == 0) || ((u32)v8 > 0x1EDU)) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v2 + 0x924), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v2 + 0x93C), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v2 + 0x954), 0);
        v11 = &_0225C9B0;
        v10 = 0;
        do {
            FillBgTilemapRect(arg0->unk_08, 2U, *v11, (u8)x, (u8)(u8)(v10 + 0x14), 4U, 1U, 0x11U);
            v10 += 1;
            v11 += 1;
        } while (v10 < 4);
    } else {
        ov89_02259A3C(v8, *(u8 *)((u8 *)v3 + 0x8E0), *(ManagedSprite **)((u8 *)v2 + 0x954), arg0->unk_160, arg0->unk_164, 1, arg0->unk_19E0);
        *(u16 *)((u8 *)v1 + 0x22) = (u16) * (u16 *)((u8 *)v3 + 0x8D8);
        *(u16 *)((u8 *)v0 + 0x22) = (u16) * (u16 *)((u8 *)v3 + 0x8D8);
        BlendPalette(&v5[v6 + 0x11], &v5[v6 + 0x21], 1U, 0xCU, 0U);
        *(u16 *)((u8 *)v0 + 0x42) = (u16) * (u16 *)((u8 *)v1 + 0x42);
        if (v4 == 0) {
            v4 += 1;
            arg0->unk_920 = (s8)v6;
        }
    }
    v3 += 0xC;
    v2 += 4;
    x += 4;
    v1 += 1;
    v0 += 1;
    v9 = v6 + 1;
    v6 = v9;
    if (v9 < 6) {
        goto loop_1;
    }
    ov89_0225A1D8(arg0, 0);
    ScheduleBgTilemapBufferTransfer(arg0->unk_08, 2U);
}
#endif
