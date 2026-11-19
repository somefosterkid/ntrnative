#ifndef POKEHEARTGOLD_OVERLAY_01_021FE200_H
#define POKEHEARTGOLD_OVERLAY_01_021FE200_H

#include "global.h"
#include "overworld_anim_manager.h"
#include "simple3dmodel.h"
#include "sys_task.h"
#include "overlay_01_021F1348.h"

void *ov01_021FE200(FieldEffectManager *fieldEffMan);

struct LocalMapObject;

// userData of the ov01_02208FE0 anim manager (built by ov01_021FE3F8): the kind it was made for and the model and render
// object it draws with. Both are pointers, so the record is 0xC bytes on the DS and 0x18 on the host.
typedef struct Ov01_021FE3F8_Args {
    s32 kind;
    Simple3DModel *model;
    Simple3DRenderObj *renderObj;
} Ov01_021FE3F8_Args;

// Its renderer context: DS size 0x1C, 0x28 on the host (ov01_02208FE0's first word must be sizeof(Ov01_021FE3F8_Work)).
typedef struct Ov01_021FE3F8_Work {
    s32 state;
    s32 timer;
    s32 unk_08;
    s32 alpha;
    s32 kind;
    Simple3DModel *model;
    Simple3DRenderObj *renderObj;
} Ov01_021FE3F8_Work;

void ov01_021FE220(void *arg0);

OverworldAnimManager *ov01_021FE3C4(struct LocalMapObject *mapObj);
OverworldAnimManager *ov01_021FE3D0(struct LocalMapObject *mapObj);
OverworldAnimManager *ov01_021FE3DC(struct LocalMapObject *mapObj);
OverworldAnimManager *ov01_021FE3E8(struct LocalMapObject *mapObj);
OverworldAnimManager *ov01_021FE3F8(struct LocalMapObject *mapObj, u32 kind);

BOOL ov01_021FE4FC(OverworldAnimManager *manager, Ov01_021FE3F8_Work *work);
void ov01_021FE524(void);
void ov01_021FE528(OverworldAnimManager *manager, Ov01_021FE3F8_Work *work);
void ov01_021FE558(OverworldAnimManager *manager, Ov01_021FE3F8_Work *work);

#endif // POKEHEARTGOLD_OVERLAY_01_021FE200_H
