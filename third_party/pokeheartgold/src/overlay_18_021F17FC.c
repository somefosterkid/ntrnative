#include "global.h"
struct PokedexAppData;

#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F17FC(struct PokedexAppData *arg0);

extern ManagedSpriteTemplate ov18_021FABC0;
extern ManagedSpriteTemplate ov18_021FABF4;
void ov18_021F1A30(PokedexAppData *arg0, s32 arg1);

void ov18_021F17FC(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObj(arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4C, 0, 2, 0xC58C);
    SpriteSystem_LoadCharResObj(arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4C, 0, 2, 0xC58D);
    SpriteSystem_LoadPaletteBuffer(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4B, 0, 1, 2, 0xC552);
    SpriteSystem_LoadPaletteBuffer(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4B, 0, 1, 2, 0xC553);
    SpriteSystem_LoadCellResObj(arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4D, 0, 0xC551);
    SpriteSystem_LoadAnimResObj(arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4E, 0, 0xC551);
}

void ov18_021F18E0(PokedexAppData *arg0);

void ov18_021F18E0(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC58CU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC58DU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC552U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC553U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC551U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC551U);
}

void ov18_021F193C(struct PokedexAppData *arg0);

void ov18_021F193C(PokedexAppData *arg0) {
    SpriteSystem_LoadCharResObj(arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4C, 0, 2, 0xC58E);
    SpriteSystem_LoadCharResObj(arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4C, 0, 2, 0xC58F);
    SpriteSystem_LoadPaletteBuffer(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4B, 0, 1, 2, 0xC554);
    SpriteSystem_LoadPaletteBuffer(arg0->paletteData, PLTTBUF_SUB_OBJ, arg0->unk_0668, arg0->unk_066C, NARC_a_0_0_8, 0x4B, 0, 1, 2, 0xC555);
}

void ov18_021F19EC(PokedexAppData *arg0);

void ov18_021F19EC(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC58EU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC58FU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC554U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC555U);
}

void ov18_021F1A30(PokedexAppData *arg0, s32 arg1) {
    arg0->unk_0670[arg1] = SpriteSystem_NewSpriteWithYOffset(arg0->unk_0668, arg0->unk_066C, &ov18_021FABC0, FX32_CONST(512));
    arg0->unk_0670[arg1 + 1] = SpriteSystem_NewSpriteWithYOffset(arg0->unk_0668, arg0->unk_066C, &ov18_021FABF4, FX32_CONST(512));
}
