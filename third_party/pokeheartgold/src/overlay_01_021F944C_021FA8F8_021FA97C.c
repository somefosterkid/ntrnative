#include "global.h"

#include "billboard.h"
#include "error_handling.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "sys_task_api.h"

// ov01_021FA97C stays asm for the DS build (asm/overlay_01_021F944C_021FA8F8.s): no typed form gives the DS stack-slot order of the
// five spilled locals (the DS has priority/srcBillboard/list/spriteId/fogEnabled at sp+0xC/0x10/0x14/0x18/0x1C; every ordering tried
// puts fogEnabled at the bottom), everything else (registers, calls, branches) is identical. The host build uses this typed version.
#if defined(SDK_PORT) || defined(NONMATCHING)

BOOL NNS_G3dMdlGetMdlFogEnableFlag(const NNSG3dResMdl *pMdl, u32 matID);

extern u16 ov01_02207294[];
extern u16 ov01_022072CC[];

UnkStruct_ov5_021ED0A4 *ov01_021FA3DC(const LocalMapObject *obj);
Billboard *ov01_021F72DC(LocalMapObject *obj);
void *FldObjSys_ReadMModelFromNarc(UnkStruct_ov5_021ED0A4 *gfx, u32 fileId, int arg2);
s32 GetMoveModelNoBySpriteId(u32 spriteId);
const BillboardAnim *ov01_021FA2AC(s32 id);
Billboard *ov01_021FA31C(BillboardList *list, const BillboardResources *resources, const VecFx32 *pos);
OverworldAnimManager *ov01_021FDE64(FieldEffectManager *fieldEffMan, const BillboardResources *resources, const Billboard *billboard, const VecFx32 *pos, int type, int priority);
void sub_02023E2C(BillboardResources *resources, void *modelRes, const NNSG3dResTex *texture, const BillboardAnim *anims, const BillboardGfxSequence *gfxSequence);
const VecFx32 *sub_02023E68(const Billboard *billboard);
void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023EE0(Billboard *billboard, int animNum);
int sub_02023EF4(const Billboard *billboard);
void sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F30(const Billboard *billboard);
void sub_02023F40(Billboard *billboard, fx32 animFrameNum);
fx32 sub_02023F70(const Billboard *billboard);
NNSG3dResMdl *sub_02023F90(const Billboard *billboard);
void sub_02023FC0(Billboard *billboard);
u32 sub_02060FA8(LocalMapObject *obj, s32 direction);

void ov01_021FAB9C(SysTask *task, void *work);

void ov01_021FA97C(LocalMapObject *obj, u32 sprite);

void ov01_021FA97C(LocalMapObject *obj, u32 sprite) {
    SysTask *task;
    s32 modelNo;
    u16 *row;
    s32 id;
    s32 spriteId;
    BOOL fogEnabled;
    ModelMapObjectWork *work;
    Billboard *srcBillboard;
    BillboardList *list;
    UnkStruct_ov5_021ED0A4 *gfx;

    gfx = ov01_021FA3DC(obj);
    list = ov01_021FA1D0(gfx);
    srcBillboard = ov01_021F72DC(obj);
    work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(ModelMapObjectWork));
    memset(work, 0, sizeof(ModelMapObjectWork));
    work->spriteId = sprite;
    work->mapObject = obj;
    if (MapObject_GetID(obj) == 0xFF) {
        fogEnabled = NNS_G3dMdlGetMdlFogEnableFlag(sub_02023F90(ov01_021F72DC(obj)), 0);
    } else {
        fogEnabled = FALSE;
    }
    spriteId = MapObject_GetSpriteID(obj);

    id = ov01_021FA28C(spriteId);
    row = ov01_02207294;
    do {
        if (row[0] == id) {
            break;
        }
        row += 2;
    } while (row[0] != 0xFF);
    if (row[0] == 0xFF) {
        GF_AssertFail();
    }
    work->modelData = FldObjSys_ReadMModelFromNarc(gfx, row[1], 0);

    id = ov01_021FA2A0(spriteId);
    row = ov01_022072CC;
    do {
        if (row[0] == id) {
            break;
        }
        row += 2;
    } while (row[0] != 0xFF);
    if (row[0] == 0xFF) {
        GF_AssertFail();
    }
    work->animData = FldObjSys_ReadMModelFromNarc(gfx, row[1], 0);
    BillboardGfxSequence_SetData(work->animData, &work->gfxSequence);

    modelNo = GetMoveModelNoBySpriteId(spriteId);
    if (modelNo < 0) {
        GF_AssertFail();
        modelNo = 0;
    }
    work->texData = FldObjSys_ReadMModelFromNarc(gfx, modelNo, 0);
    sub_02023E2C(&work->resources, work->modelData, NNS_G3dGetTex(work->texData), ov01_021FA2AC(spriteId), &work->gfxSequence);
    {
        VecFx32 pos = *sub_02023E68(srcBillboard);
        work->billboard = ov01_021FA31C(list, &work->resources, &pos);
    }
    if (work->billboard == NULL) {
        GF_AssertFail();
    }
    if (fogEnabled == FALSE) {
        NNS_G3dMdlSetMdlFogEnableFlagAll(sub_02023F90(work->billboard), 0);
    }
    sub_02023EE0(work->billboard, sub_02023EF4(srcBillboard));
    sub_02023F40(work->billboard, sub_02023F70(srcBillboard));
    sub_02023F1C(work->billboard, sub_02023F30(srcBillboard));
    sub_02023F04(work->billboard, 0);
    sub_02023EA4(work->billboard, 1);
    sub_02023FC0(work->billboard);
    if (MapObject_CheckFlag24(obj) == TRUE) {
        u32 priority;
        u8 behavior;
        int type;
        VecFx32 position;

        priority = MapObject_GetPriorityPlusValue(obj, 2);
        behavior = sub_02060FA8(obj, 1);
        if (MetatileBehavior_IsReflective(behavior) == TRUE) {
            type = 2;
        } else if (MetatileBehavior_IsPuddle(behavior) == TRUE) {
            type = 0;
        } else {
            type = 1;
        }
        MapObject_CopyPositionVector(obj, &position);
        work->animManager = ov01_021FDE64(ov01_021F146C(obj), &work->resources, work->billboard, &position, type, priority);
    }
    sub_0205E420(obj);
    ov01_021FA108(gfx->unk_104, spriteId, obj);
    sub_0205E38C(obj, sprite);
    MapObjectManager_GetPriority(gfx->unk_104);
    task = SysTask_CreateOnVWaitQueue(ov01_021FAB9C, work, 0xFF);
    if (task == NULL) {
        GF_AssertFail();
    }
    ov01_021FAB9C(task, work);
}
#endif
