#ifndef POKEHEARTGOLD_OVERLAY_01_022051EC_H
#define POKEHEARTGOLD_OVERLAY_01_022051EC_H

#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "heap.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"

// The context of field effect renderer 22 (sizeof == 0xC4 at 32 bits): the manager first, then the model, its two
// animations and the render object of the 3D effect (a/1/0/3 files 0x84, 0xA8, 0xA6).
typedef struct UnkOv01_022051EC {
    FieldEffectManager *fieldEffMan;
    NNSFndAllocator allocator;
    Field3dModel model;
    Field3DModelAnimation anim[2];
    Field3dObject obj;
} UnkOv01_022051EC;

// userData of the anim manager made by ov01_022052C4 (read back by ov01_02205300)
typedef struct UnkOv01_022051EC_Data {
    FieldEffectManager *fieldEffMan;
    UnkOv01_022051EC *ctx;
} UnkOv01_022051EC_Data;

// the context of that anim manager
typedef struct UnkOv01_022051EC_Work {
    int state;
    UnkOv01_022051EC *ctx;
} UnkOv01_022051EC_Work;

void *ov01_022051EC(FieldEffectManager *fieldEffMan);
void ov01_02205208(void *a0);
void ov01_02205218(UnkOv01_022051EC *ctx);
void ov01_022052A4(UnkOv01_022051EC *ctx);

OverworldAnimManager *ov01_022052C4(FieldEffectManager *fieldEffMan, VecFx32 *pos);

s32 ov01_022052F4(OverworldAnimManager *manager);

BOOL ov01_02205300(void *param0, UnkOv01_022051EC_Work *work);

void ov01_02205350(void);

void ov01_02205354(void *param0, UnkOv01_022051EC_Work *work);
void ov01_02205364(void *param0, UnkOv01_022051EC_Work *work);

BOOL ov01_02205388(Field3DModelAnimation *anims, u32 count);
void ov01_022053C4(Field3DModelAnimation *anims, u32 count, fx32 frame);

#endif // POKEHEARTGOLD_OVERLAY_01_022051EC_H
