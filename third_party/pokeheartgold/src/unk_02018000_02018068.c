#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "unk_02018000.h"
#include "gf_gfx_loader.h"
#include "constants/heap.h"
#include "overlay_49_unk_struct_ov70_02263344_t.h"


void sub_020182F8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);
void sub_020180BC(Field3DModelAnimation *arg0, const Field3dModel *arg1, NARC *arg2, s32 arg3, enum HeapID arg4, NNSFndAllocator *arg5);

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02263344_t;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;
typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);
typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;
typedef struct {
    const UnkStruct_ov70_0226DA18 *unk_00;
    void *unk_04;
    void *unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0226308C;
void *sub_020181A0(UnkStruct_ov70_02263344 *param0);

void sub_02018068(Field3dModel *model) {
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey paletteKey;

    if (model->texture) {
        NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
        NNS_GfdFreeTexVram(texKey);
        NNS_GfdFreeTexVram(tex4x4Key);

        paletteKey = NNS_G3dPlttReleasePlttKey(model->texture);
        NNS_GfdFreePlttVram(paletteKey);
    }

    if (model->dataRaw) {
        Heap_Free(model->dataRaw);
    }

    memset(model, 0, sizeof(Field3dModel));
}

void sub_020180BC(Field3DModelAnimation *arg0, const Field3dModel *arg1, NARC *narc, s32 fileId, enum HeapID heapID, NNSFndAllocator *arg5) {
    sub_020182F8(arg0, arg1, GfGfxLoader_LoadFromOpenNarc(narc, fileId, 0, heapID, 0), arg5);
    *((s32 *)&arg0->shouldRetainData) = 0;
}

void sub_020180E8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator) {
    sub_020182F8(anim, model, data, allocator);
    anim->shouldRetainData = 1;
}

void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator) {
    if (anim->fileDataRaw) {
        NNS_G3dFreeAnmObj(allocator, anim->animObj);

        if (anim->shouldRetainData == 0) {
            Heap_Free(anim->fileDataRaw);
        }
    }

    memset(anim, 0, sizeof(Field3DModelAnimation));
}

void sub_02018124(Field3DModelAnimation *anim, fx32 frameDelta) {
    fx32 frameCount = NNS_G3dAnmObjGetNumFrame(anim->animObj);

    if (frameDelta > 0) {
        anim->frame = (anim->frame + frameDelta) % frameCount;
    } else {
        anim->frame = anim->frame + frameDelta;

        if (anim->frame < 0) {
            anim->frame += frameCount;
        }
    }

    NNS_G3dAnmObjSetFrame(anim->animObj, anim->frame);
}

BOOL sub_0201815C(Field3DModelAnimation *anim, fx32 frameDelta) {
    fx32 frameCount = NNS_G3dAnmObjGetNumFrame(anim->animObj);
    BOOL finished = 0;

    if (frameDelta > 0) {
        if (anim->frame + frameDelta < frameCount) {
            anim->frame += frameDelta;
        } else {
            anim->frame = frameCount;
            finished = 1;
        }
    } else {
        if (anim->frame + frameDelta >= 0) {
            anim->frame += frameDelta;
        } else {
            anim->frame = 0;
            finished = 1;
        }
    }

    NNS_G3dAnmObjSetFrame(anim->animObj, anim->frame);
    return finished;
}

void sub_02018198(Field3DModelAnimation *anim, fx32 frame) {
    anim->frame = frame;
    NNS_G3dAnmObjSetFrame(anim->animObj, frame);
}

void *sub_020181A0(UnkStruct_ov70_02263344 *param0) {
    return param0->unk_04.unk_08;
}
