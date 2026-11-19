#include "global.h"
struct SpriteAnimResources;

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_71_sprite_anim_resources.h"

typedef struct UnkStruct_ov71_022472FC {
    void *unk_0;
    void *unk_4;
} UnkStruct_ov71_022472FC;

void ov71_022472C4(void *arg0, enum NarcId arg1, s32 arg2, s32 arg3);

void ov71_022472C4(void *arg0, enum NarcId narcId, s32 memberNo, s32 arg3) {
    *(void **)arg0 = GfGfxLoader_GetCellBank(narcId, memberNo, 1, arg0 + 8, HEAP_ID_57);
    *(void **)((u8 *)arg0 + 4) = GfGfxLoader_GetAnimBank(narcId, arg3, 1, arg0 + 0xC, HEAP_ID_57);
}

void ov71_022472FC(UnkStruct_ov71_022472FC *arg0);

void ov71_022472FC(UnkStruct_ov71_022472FC *arg0) {
    void *ptr;
    void *v0;

    ptr = arg0->unk_0;
    if (ptr != NULL) {
        Heap_Free(ptr);
        arg0->unk_0 = NULL;
    }
    v0 = arg0->unk_4;
    if (v0 != NULL) {
        Heap_Free(v0);
        arg0->unk_4 = NULL;
    }
}

void ov71_02247320(void *arg0, struct SpriteAnimResources *arg1, void *arg2, void *arg3, s32 arg4);

void ov71_02247320(void *arg0, SpriteAnimResources *arg1, void *arg2, void *arg3, s32 arg4) {
    *(void **)arg0 = arg2;
    *(void **)((u8 *)arg0 + 8) = arg3;
    *(s32 *)((u8 *)arg0 + 0xC) = (s32) *((s32 *)&arg1->cellBank);
    *(s32 *)((u8 *)arg0 + 0x10) = (s32) *((s32 *)&arg1->animBank);
    *(s8 *)((u8 *)arg0 + 0x20) = (s8)arg4;
    *(s32 *)((u8 *)arg0 + 4) = 0;
    *(s32 *)((u8 *)arg0 + 0x14) = 0;
    *(s32 *)((u8 *)arg0 + 0x18) = 0;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0;
}
