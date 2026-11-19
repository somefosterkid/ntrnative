#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"

void ov40_0222D66C(void *, s32, u32);

s32 ov40_0222DA00(void *, void *, u32, u32);

void ov40_0222DA84(void *, u32);

void ov40_0222E79C(void *, s32);

void ov40_0222E7B8(void *, s32);

void ov40_0222E7DC(void *, u32);

u32 ov40_02230410(void *);

void ov40_02230638(void *, s32);

void ov40_0223064C(void *, s32);

void ov40_022306A0(void *, u32);

void ov40_022307DC(s32, u32, u32);

void ov40_02230964(s32, u32);

void ov40_02241A34(s32);

void ov40_02241AB0(s32);

u32 ov40_022428D4(s32 param_1);

u32 ov40_022428D4(s32 param_1) {
    u32 uVar1;
    s32 iVar2;
    void *iVar3;

    iVar3 = *(void **)(param_1 + 0x860);
    switch (*(u32 *)(param_1 + 8)) {
    case 0:
        BgClearTilemapBufferAndCommit(*(BgConfig **)(param_1 + 0x24), 2);
        BgClearTilemapBufferAndCommit(*(BgConfig **)(param_1 + 0x24), 6);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        *(s32 *)(param_1 + 8) = *(s32 *)(param_1 + 8) + 1;
        break;
    case 1:
        ov40_0222DA84(iVar3 + 8, 1);
        iVar2 = ov40_0222DA00(iVar3, iVar3 + 4, 1, 0);
        if (iVar2 != 0) {
            *(s32 *)(param_1 + 8) = *(s32 *)(param_1 + 8) + 1;
        }
        PaletteData_BlendPalettes(*(PaletteData **)(param_1 + 0x28), PLTTBUF_SUB_OBJ, 0xc, (u8) * (u32 *)(iVar3 + 8), (u16) * (u32 *)(param_1 + 0x58));
        break;
    case 2:
        if (*(s32 *)(param_1 + 0x86c) == 0xd2) {
            ov40_0222D66C(iVar3 + 0x10, param_1 + 0x14, 3);
            ov40_0222D66C(iVar3 + 0x2c, param_1 + 0x14, 0x5e);
            ManagedSprite_SetAnim(*(ManagedSprite **)(iVar3 + 0x14), 0);
            ManagedSprite_SetAnim(*(ManagedSprite **)(iVar3 + 0x30), 3);
        }
        ov40_02241A34(param_1);
        ov40_02241AB0(param_1);
        GfGfxLoader_LoadCharDataFromOpenNarc(*(NARC **)(param_1 + 0x14), 0x3e, *(BgConfig **)(param_1 + 0x24), GF_BG_LYR_MAIN_3, 0, 0, 0, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(*(NARC **)(param_1 + 0x14), 0x3e, *(BgConfig **)(param_1 + 0x24), GF_BG_LYR_SUB_3, 0, 0, 0, HEAP_ID_109);
        ov40_02230964(param_1, 1);
        if (*(s32 *)(iVar3 + 0x1cc) == 0) {
            ov40_0223064C(iVar3 + 0x10c, param_1);
            ov40_02230638(iVar3 + 0x10c, param_1);
            ov40_022306A0(iVar3 + 0x10c, 0);
            uVar1 = ov40_02230410(iVar3 + 0x10c);
            ov40_022307DC(param_1, uVar1, 3);
        } else {
            ov40_0222E7B8(iVar3 + 0x80, param_1);
            ov40_0222E79C(iVar3 + 0x80, param_1);
            ov40_0222E7DC(iVar3 + 0x80, 0);
            GfGfxLoader_LoadScrnDataFromOpenNarc(*(NARC **)(param_1 + 0x14), 0x50, *(BgConfig **)(param_1 + 0x24), GF_BG_LYR_MAIN_3, 0, 0, 0, HEAP_ID_109);
        }
        ov40_02230964(param_1, 0);
        GfGfxLoader_LoadScrnDataFromOpenNarc(*(NARC **)(param_1 + 0x14), 6, *(BgConfig **)(param_1 + 0x24), GF_BG_LYR_SUB_3, 0, 0, 0, HEAP_ID_109);
        *(s32 *)(param_1 + 8) = *(s32 *)(param_1 + 8) + 1;
        break;
    case 3:
        ov40_0222DA84(iVar3 + 8, 0);
        iVar2 = ov40_0222DA00(iVar3, iVar3 + 4, 0, 0);
        if (iVar2 != 0) {
            if (*(s32 *)(iVar3 + 0x1cc) == 0) {
                ov40_022306A0(iVar3 + 0x10c, 1);
            } else {
                ov40_0222E7DC(iVar3 + 0x80, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            *(s32 *)(param_1 + 8) = *(s32 *)(param_1 + 8) + 1;
        }
        PaletteData_BlendPalettes(*(PaletteData **)(param_1 + 0x28), PLTTBUF_SUB_OBJ, 0xc, (u8) * (u32 *)(iVar3 + 8), (u16) * (u32 *)(param_1 + 0x58));
        break;
    default:
        return 1;
    }
    return 0;
}
