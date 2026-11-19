#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_OV01_021FDA14_WORK_A_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_OV01_021FDA14_WORK_A_H

#include "global.h"
#include "billboard.h"
#include "field_types_def.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"

// userData of the ov01_02208F74 / ov01_02208F88 renderer anim managers (ov01_021FDF88 / ov01_021FDA74 build it on
// the stack): 0x10 bytes at 32 bits.
typedef struct UnkOv01_021FDA14_DataA {
    FieldSystem *fieldSystem;
    FieldEffectManager *fieldEffMan;
    void *renderer; // ov01_021F1450(fieldEffMan, 1)
    LocalMapObject *mapObject;
} UnkOv01_021FDA14_DataA;

// The context of those anim managers (0x38 bytes at 32 bits; word 0 of both tables is its size, so the host tables
// must use sizeof(UnkOv01_021FDA14_WorkA)).
typedef struct UnkOv01_021FDA14_WorkA {
    u32 objectId;
    u32 mapId;
    u32 spriteId;
    BOOL hasBillboard;
    u32 type; // userInt
    UnkOv01_021FDA14_DataA data;
    Billboard *billboard;
    VecFx32 scale;
    fx32 scaleDelta;
} UnkOv01_021FDA14_WorkA;

void ov01_021FDA74(LocalMapObject *mapObject, int type);
BOOL ov01_021FDAC0(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDB34(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDB44(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDBCC(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDC7C(UnkOv01_021FDA14_WorkA *work, LocalMapObject *mapObject, VecFx32 *out);
void ov01_021FDD48(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDF88(LocalMapObject *mapObject, int type);
BOOL ov01_021FDFD4(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FE058(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FE0EC(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FE190(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_OV01_021FDA14_WORK_A_H
