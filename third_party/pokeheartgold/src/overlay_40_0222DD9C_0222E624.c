#include "global.h"
struct UnkStruct_0208C06C;

#include "sprite_system.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222E624(void *arg0, struct UnkStruct_0208C06C *arg1);

void ov40_0222E624(void *arg0, UnkStruct_0208C06C *arg1) {
    SpriteManager_UnloadCharObjById(arg1->unk_14.unk_08, 0x2869FU);
    SpriteManager_UnloadPlttObjById(arg1->unk_14.unk_08, 0x2869FU);
    SpriteManager_UnloadCellObjById(arg1->unk_14.unk_08, 0x2869FU);
    SpriteManager_UnloadAnimObjById(arg1->unk_14.unk_08, 0x2869FU);
    Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)arg0 + 8));
}
