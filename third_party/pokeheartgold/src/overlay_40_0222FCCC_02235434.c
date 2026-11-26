#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_40.h"
#include "palette.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

SysTask *ov39_02227080(void *, void *);

s32 ov39_022271C0(void *, void *);

s32 ov40_0222BC54(UnkStruct_0208C06C *);

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222DA00(void *, void *, s32, s32);

s32 ov40_0222E79C(void *, void *);

s32 ov40_0222E7B8(void *, void *);

s32 ov40_0222E7DC(void *, s32);

s32 ov40_022307DC(void *, s32, s32);

s32 ov40_02230964(void *, s32);

s32 ov40_02235868(void *, s32);

s32 ov40_02235940(void);

s32 ov40_02235994(void);

s32 ov40_02235C7C(void *, s32);

void ov40_02236130(UnkStruct_0208C06C *param0);

void *ov41_0224B530(void *, void *);

s32 ov41_0224B57C();

s32 ov41_0224B5D0(void *, s32);

s32 ov40_02235434(UnkStruct_0208C06C *arg0) {
    s32 v1;
    u32 v0;
    void *v2;

    v2 = arg0->unk_860;
    v0 = (u32)arg0->unk_08;
    switch (v0) {
    case 0:
        if (*(s32 *)((u8 *)v2 + 0xC) == 0) {
            ov40_02236130(arg0);
            BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
        } else {
            ov40_02230964(arg0, 1);
            ov40_0222E7B8(v2 + 0x250, arg0);
            ov40_02230964(arg0, 0);
        }
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 1:
        if (!(ov40_0222DA00(v2, v2 + 4, 1, 1) == 0)) {
            if (*(s32 *)((u8 *)v2 + 0xC) == 0) {
                if (*(s32 *)((u8 *)v2 + 0x228) != 0) {
                    ov41_0224B57C();
                    ov40_0222BC54(arg0);
                    SetBgPriority(2U, 0U);
                    *(s32 *)((u8 *)v2 + 0x228) = 0;
                    ov40_02235994();
                }
                BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
                BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 3U);
                GfGfx_EngineATogglePlanes(4U, 0U);
            }
            arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        }
        break;
    case 2:
        if (*(s32 *)((u8 *)v2 + 0xC) == 0) {
            ov40_02230964(arg0, 1);
            ov39_022271C0(*(void **)((u8 *)(v2 + (*(s32 *)((u8 *)v2 + 0x234) * 4)) + 0x23C), *(void **)((u8 *)v2 + 0x250));
            ov40_0222E79C(v2 + 0x250, arg0);
            ov40_0222E7DC(v2 + 0x250, 0);
            ov40_02230964(arg0, 0);
            ov40_022307DC(arg0, 0x50, 3);
        } else {
            ov39_02227080(*(void **)((u8 *)(v2 + (*(s32 *)((u8 *)v2 + 0x234) * 4)) + 0x23C) + 0x80, *(void **)((u8 *)v2 + 0x238));
            *(void **)((u8 *)v2 + 0x228) = ov41_0224B530(v2 + 0x218, *(void **)((u8 *)v2 + 0x238));
            ov41_0224B5D0(*(void **)((u8 *)v2 + 0x228), 0);
            ov40_02235940();
            ov40_022307DC(arg0, 0x23, 3);
            GfGfx_EngineBTogglePlanes(8U, 1U);
            GfGfx_EngineATogglePlanes(8U, 1U);
            GfGfx_EngineATogglePlanes(4U, 0U);
            GfGfx_EngineATogglePlanes(1U, 0U);
        }
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 3:
        if (*(s32 *)((u8 *)v2 + 0xC) != 0) {
            PaletteData_LoadPaletteSlotFromHardware(((PaletteData *)arg0->unk_14.unk_14), PLTTBUF_MAIN_BG, 0U, 0x200U);
        }
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 4:
        if (ov40_0222DA00(v2, v2 + 4, 0, 1) != 0) {
            if (*(s32 *)((u8 *)v2 + 0xC) == 0) {
                ov40_0222E7DC(v2 + 0x250, 1);
                GfGfx_EngineATogglePlanes(4U, 1U);
            } else {
                ov40_02235C7C(arg0, *(s32 *)((u8 *)v2 + 0x234));
                ov41_0224B5D0(*(void **)((u8 *)v2 + 0x228), 1);
                GfGfx_EngineATogglePlanes(4U, 1U);
                GfGfx_EngineATogglePlanes(1U, 1U);
            }
            arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        }
        break;
    default:
        v1 = *(s32 *)((u8 *)v2 + 0xC) ^ 1;
        *(s32 *)((u8 *)v2 + 0xC) = v1;
        ov40_02235868(arg0, v1 + 0x79);
        ov40_0222BF80(arg0, 8);
        break;
    }
    return 0;
}

s32 ov40_02235644(UnkStruct_0208C06C *arg0) {
    u32 v0;
    void *v1;

    v1 = arg0->unk_860;
    v0 = (u32)arg0->unk_08;
    switch (v0) {
    case 0:
        if (*(s32 *)((u8 *)v1 + 0xC) == 0) {
            ov40_02236130(arg0);
            BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
        } else {
            ov40_02230964(arg0, 1);
            ov40_0222E7B8(v1 + 0x250, arg0);
            ov40_02230964(arg0, 0);
        }
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 1:
        if (!(ov40_0222DA00(v1, v1 + 4, 1, 1) == 0)) {
            if (*(s32 *)((u8 *)v1 + 0xC) == 0) {
                if (*(s32 *)((u8 *)v1 + 0x228) != 0) {
                    ov41_0224B57C();
                    ov40_0222BC54(arg0);
                    SetBgPriority(2U, 0U);
                    *(s32 *)((u8 *)v1 + 0x228) = 0;
                    ov40_02235994();
                }
                BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
                BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 3U);
                GfGfx_EngineATogglePlanes(4U, 0U);
            }
            arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        }
        break;
    case 2:
        ov40_02230964(arg0, 1);
        if (*(s32 *)((u8 *)v1 + 0xC) == 0) {
            ov39_02227080(*(void **)((u8 *)(v1 + (*(s32 *)((u8 *)v1 + 0x234) * 4)) + 0x23C) + 0x80, *(void **)((u8 *)v1 + 0x238));
            *(void **)((u8 *)v1 + 0x228) = ov41_0224B530(v1 + 0x218, *(void **)((u8 *)v1 + 0x238));
            ov41_0224B5D0(*(void **)((u8 *)v1 + 0x228), 0);
            ov40_02235940();
            ov40_022307DC(arg0, 0x23, 3);
            GfGfx_EngineBTogglePlanes(8U, 1U);
            GfGfx_EngineATogglePlanes(8U, 1U);
            GfGfx_EngineATogglePlanes(4U, 0U);
            GfGfx_EngineATogglePlanes(1U, 0U);
        } else {
            ov39_022271C0(*(void **)((u8 *)(v1 + (*(s32 *)((u8 *)v1 + 0x234) * 4)) + 0x23C), *(void **)((u8 *)v1 + 0x250));
            ov40_0222E79C(v1 + 0x250, arg0);
            ov40_0222E7DC(v1 + 0x250, 0);
            ov40_022307DC(arg0, 0x50, 3);
            GfGfx_EngineATogglePlanes(4U, 0U);
        }
        ov40_02230964(arg0, 0);
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 3:
        if (*(s32 *)((u8 *)v1 + 0xC) == 0) {
            PaletteData_LoadPaletteSlotFromHardware(((PaletteData *)arg0->unk_14.unk_14), PLTTBUF_MAIN_BG, 0U, 0x200U);
        }
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 4:
        if (ov40_0222DA00(v1, v1 + 4, 0, 1) != 0) {
            if (*(s32 *)((u8 *)v1 + 0xC) == 0) {
                ov40_02235C7C(arg0, *(s32 *)((u8 *)v1 + 0x234));
                ov41_0224B5D0(*(void **)((u8 *)v1 + 0x228), 1);
                GfGfx_EngineATogglePlanes(4U, 1U);
                GfGfx_EngineATogglePlanes(1U, 1U);
            } else {
                ov40_0222E7DC(v1 + 0x250, 1);
                GfGfx_EngineATogglePlanes(4U, 1U);
            }
            arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        }
        break;
    default:
        ov40_0222BF80(arg0, 8);
        break;
    }
    return 0;
}
