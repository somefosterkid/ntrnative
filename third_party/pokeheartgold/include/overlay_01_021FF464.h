#ifndef POKEHEARTGOLD_OVERLAY_01_021FF464_H
#define POKEHEARTGOLD_OVERLAY_01_021FF464_H

#include "global.h"
#include "billboard.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"

void *ov01_021FF464(FieldEffectManager *fieldEffMan);

// userData of the ov01_022091C0 renderer anim managers (ov01_021FF4FC builds it on the stack): 0x10 bytes at 32 bits.
typedef struct UnkOv01_021FF464Args {
    FieldSystem *fieldSystem;
    FieldEffectManager *fieldEffMan;
    FieldEffectRendererContext *renderer; // ov01_021F1450(fieldEffMan, 10)
    LocalMapObject *mapObject;
} UnkOv01_021FF464Args;

// The anim managers own context (0x28 bytes at 32 bits).
typedef struct UnkOv01_021FF464Work {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    int unk_10;
    UnkOv01_021FF464Args args;
    Billboard *billboard;
} UnkOv01_021FF464Work;

void ov01_021FF480(void *arg0);
void ov01_021FF4D4(FieldEffectRendererContext *ctx);
void ov01_021FF4FC(LocalMapObject *mapObject, int userInt);
BOOL ov01_021FF54C(OverworldAnimManager *manager, UnkOv01_021FF464Work *work);
void ov01_021FF5B8(OverworldAnimManager *manager, UnkOv01_021FF464Work *work);
void ov01_021FF5C4(OverworldAnimManager *manager, UnkOv01_021FF464Work *work);
void ov01_021FF658(OverworldAnimManager *manager, UnkOv01_021FF464Work *work);

#endif // POKEHEARTGOLD_OVERLAY_01_021FF464_H
