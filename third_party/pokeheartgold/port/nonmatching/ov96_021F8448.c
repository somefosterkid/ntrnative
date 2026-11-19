#include "sprite_system.h"
extern s32 ov96_0221C2C4;
extern s32 ov96_0221C2D8;
extern s32 ov96_0221C2F0;

static SpriteResourceCountsListUnion hgasset_ov96_021F8448_countsList __attribute__((section(BWPORT_DATA_SECTION))) = { { 0, 0, 0, 0, 0, 0 } };

static OamManagerParam hgasset_ov96_021F8448_oamManagerParam __attribute__((section(BWPORT_DATA_SECTION))) = { 0, 0, 0, 0, 0, 0, 0, 0 };

void ov96_021F8448(void *arg0)
{
    __typeof__(hgasset_ov96_021F8448_countsList) countsList; __builtin_memcpy((void *)&countsList, &hgasset_ov96_021F8448_countsList, sizeof(countsList));
    __typeof__(hgasset_ov96_021F8448_oamManagerParam) oamManagerParam; __builtin_memcpy((void *)&oamManagerParam, &hgasset_ov96_021F8448_oamManagerParam, sizeof(oamManagerParam));
    OamCharTransferParam oamCharTransferParam = { 0, 0x20000, 0x4000, (GXOBJVRamModeChar)0x00100010, (GXOBJVRamModeChar)0x00100010 };

    oamCharTransferParam.maxTasks = 0x1a;
    *(SpriteSystem **)((u8 *)arg0 + 0x94) = SpriteSystem_Alloc(*(enum HeapID *)arg0);
    *(SpriteManager **)((u8 *)arg0 + 0x98) = SpriteManager_New(*(SpriteSystem **)((u8 *)arg0 + 0x94));
    SpriteSystem_Init(*(SpriteSystem **)((u8 *)arg0 + 0x94), &oamManagerParam, &oamCharTransferParam, 0x20);
    SpriteSystem_InitSprites(*(SpriteSystem **)((u8 *)arg0 + 0x94), *(SpriteManager **)((u8 *)arg0 + 0x98), 2);
    SpriteSystem_InitManagerWithCapacities(*(SpriteSystem **)((u8 *)arg0 + 0x94), *(SpriteManager **)((u8 *)arg0 + 0x98), &countsList);
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_port_nonmatching_ov96_021F8448_c_0(void) {
    PORT_AssetAdd("IPKE", "port/nonmatching/ov96_021F8448.c:hgasset_ov96_021F8448_countsList", (void *)&hgasset_ov96_021F8448_countsList, sizeof(hgasset_ov96_021F8448_countsList), "u8");
    PORT_AssetAdd("IPGE", "port/nonmatching/ov96_021F8448.c:hgasset_ov96_021F8448_countsList", (void *)&hgasset_ov96_021F8448_countsList, sizeof(hgasset_ov96_021F8448_countsList), "u8");
    PORT_AssetAdd("IPKE", "port/nonmatching/ov96_021F8448.c:hgasset_ov96_021F8448_oamManagerParam", (void *)&hgasset_ov96_021F8448_oamManagerParam, sizeof(hgasset_ov96_021F8448_oamManagerParam), "u8");
    PORT_AssetAdd("IPGE", "port/nonmatching/ov96_021F8448.c:hgasset_ov96_021F8448_oamManagerParam", (void *)&hgasset_ov96_021F8448_oamManagerParam, sizeof(hgasset_ov96_021F8448_oamManagerParam), "u8");
}
