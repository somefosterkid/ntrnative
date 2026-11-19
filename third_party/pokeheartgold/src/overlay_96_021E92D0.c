#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "fashion_case.h"
#include "heap.h"
#include "pm_string.h"
#include "save.h"

struct ImageClips;

typedef struct ImageClips ImageClips;

FashionCase *ObjCharTransfer_Destroy(ImageClips *imageClips);

u32 ObjPlttTransfer_Destroy(const FashionCase *fashionCase);

ImageClips *OamManager_Free(SaveData *saveData);

u32 ov96_021E92D0(SaveData *saveData);

u32 ov96_021E92D0(SaveData *saveData) {
    ImageClips *imageClips = OamManager_Free(saveData);
    FashionCase *fashionCase = ObjCharTransfer_Destroy(imageClips);
    return ObjPlttTransfer_Destroy(fashionCase);
}

void *ov96_021E92E0(enum HeapID heapId) {
    void *v0;
    s32 v2;
    void *dest;
    void *v1;

    dest = Heap_Alloc(heapId, 0x214U);
    v0 = dest;
    MI_CpuFill8(dest, 0U, 0x214U);
    v1 = v0;
    v2 = 0;
    do {
        *(String **)((u8 *)v1 + 0x34) = String_New(0x25U, heapId);
        *(String **)((u8 *)v1 + 0x38) = String_New(0xBU, heapId);
        v2 += 1;
        v1 += 0x18;
    } while (v2 < 0xC);
    return v0;
}

void ov96_021E9320(void *ptr) {
    s32 v0;
    void *v1;

    v0 = 0;
    v1 = ptr;
    do {
        String_Delete(*(String **)((u8 *)v1 + 0x34));
        String_Delete(*(String **)((u8 *)v1 + 0x38));
        v0 += 1;
        v1 += 0x18;
    } while (v0 < 0xC);
    Heap_Free(ptr);
}

void *ov96_021E9344(void *arg0);

void *ov96_021E9344(void *arg0) {
    s32 v0;

    if ((s32) * (s32 *)((u8 *)arg0 + 8) >= 4) {
        GF_AssertFail();
    }
    if ((s32) * (s32 *)((u8 *)arg0 + 8) >= (s32) * (s32 *)arg0) {
        GF_AssertFail();
    }
    v0 = *(s32 *)((u8 *)arg0 + 8);
    *(s32 *)((u8 *)arg0 + 8) = (s32)(v0 + 1);
    return arg0 + 0x24 + (0x18 * v0);
}
