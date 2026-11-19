#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "palette.h"
#include "poke_overlay.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_022344D8_temp_r5 {
    u8 unk_0[0x4];
    u8 unk_4;
    u8 unk_5[0x3];
    s32 unk_8;
    u8 unk_C[0x21C];
    s32 unk_228;
    void *unk_22C;
    u8 unk_230[0x8];
    void *unk_238;
    u8 unk_23C[0x14];
    void *unk_250;
    u8 unk_254[0x88];
    void *unk_2DC;
} UnkStruct_ov40_022344D8_temp_r5;

s32 ov39_022273B0(u8 *, void *, void *);

s32 ov39_02227D44(u8 *, void **);

s32 ov40_0222BC54(UnkStruct_0208C06C *);

void ov40_0222BD04(void *);

s32 ov40_0222BF64(void *, s32, s32, s32 *);

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222D88C(void *);

s32 ov40_0222DA00(void *, void *, s32, s32);

s32 ov40_0222DA84(void *, s32);

s32 ov40_0222DAA8(void *);

s32 ov40_0222DD08(void *);

s32 ov40_0222DED0(void *, s32);

s32 ov40_0222DFB0(void *);

s32 ov40_0222FB28(void *, s32);

s32 ov40_0222FB90(void *, s32);

s32 ov40_0222FBB4(void *);

s32 ov40_0223077C(void *, void *, s32, s32);

s32 ov40_02230964(void *, s32);

s32 ov40_02230CDC(void *, s32, s32, s32);

s32 ov40_02235994(void);

s32 ov40_02235B10(void *);

s32 ov40_02236130(void *);

void *ov40_0223D540(void *);

s32 ov40_0223D5CC(void *);

s32 ov41_0224B57C();

void *sub_020314A4(s32);

SysTask *sub_020314BC(void *);

s32 sub_020314C4(void *, void *);

s32 sub_020879E0(void *, s32);

s32 sub_02087A08(void *, s32, s32);

FS_EXTERN_OVERLAY(OVY_41);

s32 ov40_022344D8(void *arg0);

s32 ov40_022344D8(void *arg0) {
    void *sp4;
    u32 temp_r0;
    UnkStruct_ov40_022344D8_temp_r5 *temp_r5;

    temp_r5 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    if (ov40_0223D5CC(arg0) == 0) {
        return 0;
    }
    temp_r0 = *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08);
    switch (temp_r0) {
    case 0:
        SetBgPriority(0U, 1U);
        SetBgPriority(1U, 3U);
        SetBgPriority(2U, 2U);
        SetBgPriority(3U, 1U);
        SetBgPriority(4U, 1U);
        SetBgPriority(5U, 3U);
        SetBgPriority(6U, 2U);
        SetBgPriority(7U, 1U);
        Heap_Free(temp_r5->unk_238);
        sub_020314BC(temp_r5->unk_250);
        BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 2U);
        BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 6U);
        ov40_02236130(arg0);
        *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        break;
    case 1:
        ov40_0222DA84((u8 *)&temp_r5->unk_8, 1);
        if (ov40_0222DA00(temp_r5, (u8 *)&temp_r5->unk_4, 1, 0) != 0) {
            ov40_02230964(arg0, 1);
            ov40_02235B10(arg0);
            ov40_02230964(arg0, 0);
            BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 3U);
            BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 7U);
            BgSetPosTextAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 2U, BG_POS_OP_SET_ROT, 0);
            SetBgPriority(2U, 0U);
            *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        }
        PaletteData_BlendPalettes(((PaletteData *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_14), PLTTBUF_SUB_OBJ, 0xCU, temp_r5->unk_8, (s32) ((UnkStruct_0208C06C *)arg0)->unk_14.unk_44);
        break;
    case 2:
        ov40_0223077C(arg0, *((void **)&((UnkStruct_0208C06C *)arg0)->unk_6F0), 0x80, 0x60);
        sub_020879E0(*((void **)&((UnkStruct_0208C06C *)arg0)->unk_6F0), 1);
        sub_02087A08(*((void **)&((UnkStruct_0208C06C *)arg0)->unk_6F0), 0x18, 0x18);
        ov40_0222DED0(arg0, 0x11E);
        temp_r5->unk_2DC = sub_020314A4(0x6D);
        sub_020314C4(temp_r5->unk_2DC, *((void **)&((UnkStruct_0208C06C *)arg0)->saveData));
        PlaySE(0x57DU);
        *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        break;
    case 3:
        if (ov39_022273B0(ov40_0223D540(arg0), temp_r5->unk_2DC, temp_r5->unk_22C) != 1) {

        } else {
            sub_020314BC(temp_r5->unk_2DC);
            *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        }
        break;
    case 4:
        ov40_0222DFB0(arg0);
        if (ov39_02227D44(ov40_0223D540(arg0), &sp4) == 1) {
            StopSE(0x57DU, 0);
            ov40_02230CDC(arg0, 0, *(s32 *)((u8 *)sp4 + 0xC), *(s32 *)((u8 *)sp4 + 4));
            *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        } else {
            StopSE(0x57DU, 0);
            ov40_0222FB28(arg0, 0x24);
            PlaySE(0x577U);
            *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        }
        break;
    case 5:
        sub_020879E0(*((void **)&((UnkStruct_0208C06C *)arg0)->unk_6F0), 0);
        sub_02087A08(*((void **)&((UnkStruct_0208C06C *)arg0)->unk_6F0), 0, 0);
        *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        break;
    case 6:
        ov40_02230964(arg0, 1);
        ov40_0222DAA8((u8 *)&temp_r5->unk_8);
        ov40_0222D88C(arg0);
        ov40_02230964(arg0, 0);
        BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 2U);
        BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 6U);
        BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 3U);
        BgClearTilemapBufferAndCommit(((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, 7U);
        ov40_0222FB90(arg0, 1);
        *((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) = (u32)(*((u32 *)&((UnkStruct_0208C06C *)arg0)->unk_08) + 1);
        break;
    default:
        if (ov40_0222FBB4(arg0) != 0) {
            if (ov40_0222DA84((u8 *)&temp_r5->unk_8, 0) != 0) {
                if (temp_r5->unk_228 != 0) {
                    ov41_0224B57C();
                    ov40_0222BC54(arg0);
                    SetBgPriority(2U, 0U);
                    ov40_02235994();
                }
                ov40_0222DD08(arg0);
                ov40_0222DAA8((u8 *)&temp_r5->unk_8);
                PaletteData_BlendPalettes(((PaletteData *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_14), PLTTBUF_MAIN_OBJ, 0xCU, 0x10U, (s32) ((UnkStruct_0208C06C *)arg0)->unk_14.unk_44);
                ov40_0222BF64(arg0, 1, 1, *((void **)&((UnkStruct_0208C06C *)arg0)->unk_10));
                ov40_0222BF80(arg0, 5);
                Heap_Free(temp_r5);
                UnloadOverlayByID(FS_OVERLAY_ID(OVY_41));
                Main_SetVBlankIntrCB(ov40_0222BD04, arg0);
            } else {
                PaletteData_BlendPalettes(((PaletteData *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_14), PLTTBUF_SUB_BG, 2U, temp_r5->unk_8, (s32) ((UnkStruct_0208C06C *)arg0)->unk_14.unk_44);
                PaletteData_BlendPalettes(((PaletteData *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_14), PLTTBUF_SUB_OBJ, 0xCU, temp_r5->unk_8, (s32) ((UnkStruct_0208C06C *)arg0)->unk_14.unk_44);
            }
        }
        break;
    }
    return 0;
}
