#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"

#include "overlay_40_unk_struct_ov62_02241204.h"

void ov40_02230638(UnkStruct_ov62_02233F74 *, UnkStruct_0208C06C *);

void ov40_0223064C(UnkStruct_ov62_02233F74 *, UnkStruct_0208C06C *);

void ov40_022306A0(UnkStruct_ov62_02233F74 *, int);

void ov40_0222E79C(UnkStruct_ov62_022323B8 *, UnkStruct_0208C06C *);

void ov40_0222E7B8(UnkStruct_ov62_022323B8 *, UnkStruct_0208C06C *);

void ov40_0222E7DC(UnkStruct_ov62_022323B8 *, int);

int ov40_02230410(UnkStruct_ov62_02233F74 *);

void ov40_022307DC(UnkStruct_0208C06C *, int, int);

void ov40_02230964(UnkStruct_0208C06C *, BOOL);

void ov40_02241A34(UnkStruct_0208C06C *);

void ov40_02241AB0(UnkStruct_0208C06C *);

BOOL ov40_0222DA84(int *, int);

BOOL ov40_0222DA00(int *, int *, int, int);

void ov40_0222D66C(UnkStruct_ov62_022312B0 *, void *, int);

u32 ov40_022428D4(UnkStruct_0208C06C *param_1);

u32 ov40_022428D4(UnkStruct_0208C06C *param_1) {
    u32 uVar1;
    s32 iVar2;
    UnkStruct_ov62_02241204 *iVar3;

    iVar3 = param_1->unk_860;
    switch (param_1->unk_08) {
    case 0:
        BgClearTilemapBufferAndCommit(param_1->unk_14.unk_10, 2);
        BgClearTilemapBufferAndCommit(param_1->unk_14.unk_10, 6);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineATogglePlanes(4, 0);
        param_1->unk_08 = param_1->unk_08 + 1;
        break;
    case 1:
        ov40_0222DA84(&iVar3->unk_08, 1);
        iVar2 = ov40_0222DA00(&iVar3->unk_00, &iVar3->unk_04, 1, 0);
        if (iVar2 != 0) {
            param_1->unk_08 = param_1->unk_08 + 1;
        }
        PaletteData_BlendPalettes(param_1->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xc, (u8)iVar3->unk_08, (u16)param_1->unk_14.unk_44);
        break;
    case 2:
        if (param_1->unk_86C == 0xd2) {
            ov40_0222D66C(&iVar3->unk_10[0], &param_1->unk_14, 3);
            ov40_0222D66C(&iVar3->unk_10[1], &param_1->unk_14, 0x5e);
            ManagedSprite_SetAnim(iVar3->unk_10[0].unk_08, 0);
            ManagedSprite_SetAnim(iVar3->unk_10[1].unk_08, 3);
        }
        ov40_02241A34(param_1);
        ov40_02241AB0(param_1);
        GfGfxLoader_LoadCharDataFromOpenNarc(param_1->unk_14.unk_00, 0x3e, param_1->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0, 0, 0, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(param_1->unk_14.unk_00, 0x3e, param_1->unk_14.unk_10, GF_BG_LYR_SUB_3, 0, 0, 0, HEAP_ID_109);
        ov40_02230964(param_1, 1);
        if (iVar3->unk_1CC == 0) {
            ov40_0223064C(&iVar3->unk_10C, param_1);
            ov40_02230638(&iVar3->unk_10C, param_1);
            ov40_022306A0(&iVar3->unk_10C, 0);
            uVar1 = ov40_02230410(&iVar3->unk_10C);
            ov40_022307DC(param_1, uVar1, 3);
        } else {
            ov40_0222E7B8(&iVar3->unk_80, param_1);
            ov40_0222E79C(&iVar3->unk_80, param_1);
            ov40_0222E7DC(&iVar3->unk_80, 0);
            GfGfxLoader_LoadScrnDataFromOpenNarc(param_1->unk_14.unk_00, 0x50, param_1->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0, 0, 0, HEAP_ID_109);
        }
        ov40_02230964(param_1, 0);
        GfGfxLoader_LoadScrnDataFromOpenNarc(param_1->unk_14.unk_00, 6, param_1->unk_14.unk_10, GF_BG_LYR_SUB_3, 0, 0, 0, HEAP_ID_109);
        param_1->unk_08 = param_1->unk_08 + 1;
        break;
    case 3:
        ov40_0222DA84(&iVar3->unk_08, 0);
        iVar2 = ov40_0222DA00(&iVar3->unk_00, &iVar3->unk_04, 0, 0);
        if (iVar2 != 0) {
            if (iVar3->unk_1CC == 0) {
                ov40_022306A0(&iVar3->unk_10C, 1);
            } else {
                ov40_0222E7DC(&iVar3->unk_80, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            param_1->unk_08 = param_1->unk_08 + 1;
        }
        PaletteData_BlendPalettes(param_1->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xc, (u8)iVar3->unk_08, (u16)param_1->unk_14.unk_44);
        break;
    default:
        return 1;
    }
    return 0;
}
