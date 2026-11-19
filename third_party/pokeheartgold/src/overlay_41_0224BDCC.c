#include "global.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "gf_3d_loader.h"
#include "filesystem.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct BgConfig;
struct DressUpPhoto;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct Window;
typedef struct DressUpPhoto DressUpPhoto;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void SetBgPriority(u8 bgLayer, u8 priority);
void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);
void LoadFontPal0(enum GFPalLoadLocation palLocation, u32 palSlotOffset, enum HeapID heapID);
void RemoveWindow(Window *window);
void ClearWindowTilemapAndCopyToVram(Window *window);

void ov41_0224BDCC(UnkStruct_ov22_0225B85C *param0);
void ov41_0224BE34(UnkStruct_ov22_0225B85C *param0);

void ov41_0224BDCC(UnkStruct_ov22_0225B85C *param0) {
    param0->unk_200 = AllocWindows(HEAP_ID_14, 1);

    AddWindowParameterized(param0->unk_14.unk_40, param0->unk_200, 3, 0, 18, 32, 6, 5, 1);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, ((5) * ((16) * sizeof(u16))), HEAP_ID_14);
    SetBgPriority(GF_BG_LYR_MAIN_3, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_1, 1);
    BgSetPosTextAndCommit(param0->unk_14.unk_40, 3, BG_POS_OP_SET_Y, 0);
}

void ov41_0224BE34(UnkStruct_ov22_0225B85C *param0) {
    ClearWindowTilemapAndCopyToVram(param0->unk_200);
    RemoveWindow(param0->unk_200);
    WindowArray_Delete(param0->unk_200, 1);
}
