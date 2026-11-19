#include "global.h"

#include "sys_task.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_struct_02013034.h"
#include "unk_02013534_font_system.h"

SysTask *sub_02013E24(SysTaskFunc callback, void *param, u32 priority);

SysTask *sub_02013948(SysTaskFunc callback, void *param, int priority);

struct FontOAM;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;
typedef struct {
    const UnkStruct_02012744 *unk_00;
    const Window *unk_04;
    SpriteList *unk_08;
    const NNSG2dImagePaletteProxy *unk_0C;
    const Sprite *unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_020127E8;
struct UnkStruct_02012B20_t {
    UnkStruct_02013034 unk_00;
    int unk_14;
};
FontOAM *sub_02013AD0(const UnkStruct_02012744 *param0);
void sub_02013C5C(const Window *param0, const UnkStruct_02013034 *param1, NNSG2dImageProxy *param2, int param3, int param4, enum HeapID heapID);
void sub_02013E78(const UnkStruct_020127E8 *param0, const UnkStruct_02013034 *param1, const NNSG2dImageProxy *param2, FontOAM *param3);
FontOAM *TextOBJ_Create(const UnkStruct_020127E8 *param0, const UnkStruct_02012B20 *param1);

SysTask *FontOAM_Delete(SysTaskFunc callback, void *param, u32 priority);
SysTask *TextOBJ_Destroy(SysTaskFunc callback, void *param, int priority);

SysTask *sub_02013948(SysTaskFunc callback, void *param, int priority) {
    return sub_02013E24(callback, param, priority);
}

FontOAM *TextOBJ_Create(const UnkStruct_020127E8 *param0, const UnkStruct_02012B20 *param1) {
    FontOAM *v0;
    NNSG2dImageProxy *v1;

    ((param0) ? (void)0 : GF_AssertFail());
    v0 = sub_02013AD0(param0->unk_00);

    ((v0) ? (void)0 : GF_AssertFail());

    v0->unk_08 = param0->unk_10;
    v0->x = param0->unk_18;
    v0->y = param0->unk_1C;

    v1 = Heap_AllocAtEnd(param0->heapID, sizeof(NNSG2dImageProxy) * param1->unk_14);

    v0->unk_00 = Heap_Alloc(param0->heapID, sizeof(UnkStruct_02012CE0_sub1) * param1->unk_14);
    v0->unk_04 = param1->unk_14;

    sub_02013C5C(param0->unk_04, &param1->unk_00, v1, param0->unk_14, param0->unk_28, param0->heapID);
    sub_02013E78(param0, &param1->unk_00, v1, v0);
    Heap_Free(v1);

    return v0;
}

SysTask *TextOBJ_Destroy(SysTaskFunc callback, void *param, int priority) {
    return FontOAM_Delete(callback, param, priority);
}
