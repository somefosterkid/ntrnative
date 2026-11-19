#ifndef POKEHEARTGOLD_OVERLAY_01_021FE780_H
#define POKEHEARTGOLD_OVERLAY_01_021FE780_H

#include "map_object_manager.h"
#include "overworld_anim_manager.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_simple_renderers.h"

// What renderer 3 (ov01_021FE780) keeps for the surf mount: the model and its render object.
typedef UnkStruct_ov5_021F4DD8 SurfMountResources;

// User data of the funcs table ov01_022090DC (ov01_021FE7DC); init copies it to `userData` of the context.
typedef struct SurfMountUserData {
    int dir;
    FieldEffectManager *fieldEffMan;
    SurfMountResources *resources;
    LocalMapObject *playerMapObj;
} SurfMountUserData;

// Context of the funcs table ov01_022090DC (0x34 bytes at 32 bits).
typedef struct SurfMountRenderer {
    BOOL inactive;
    int dirRaw;
    int dir;
    int mapObjLocalID;
    int mapHeaderID;
    int counter;
    BOOL syncPos;
    fx32 yOffset;
    fx32 yDelta;
    SurfMountUserData userData;
} SurfMountRenderer;

void *ov01_021FE780(FieldEffectManager *fieldEffMan);
void ov01_021FE79C(void *ctx);

OverworldAnimManager *ov01_021FE7DC(LocalMapObject *playerMapObj, int tileX, int tileZ, int dir, BOOL syncPos);
BOOL ov01_021FE868(OverworldAnimManager *animMan, SurfMountRenderer *renderer);
void ov01_021FE8B0(OverworldAnimManager *animMan, SurfMountRenderer *renderer);
void ov01_021FE8C8(OverworldAnimManager *animMan, SurfMountRenderer *renderer);
void ov01_021FE970(OverworldAnimManager *animMan, SurfMountRenderer *renderer);
void ov01_021FE9F4(OverworldAnimManager *animMan, BOOL syncPos);

#endif // POKEHEARTGOLD_OVERLAY_01_021FE780_H
