#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "constants/heap.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_01_021F944C.h"
#include "sys_task.h"
#include "overlay_01_021FC4C4.h"

u8 sub_02023EB8(Billboard *);

int sub_02023EF4(Billboard *);

s32 sub_02023F30(Billboard *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02023EA4(Billboard *, u8);
#else
void sub_02023EA4(Billboard *, int);
#endif

struct GF_2DGfxRawResMan;
struct ResourceHeap;
struct TextureResourceManager;
struct UnkStruct_ov5_021ED0A4_t;
typedef struct TextureResourceManager TextureResourceManager;
void ov01_021F9698(UnkStruct_ov5_021ED0A4 *param0, int param1);

s32 ov01_021F9798(UnkStruct_ov5_021ED0A4 *param0);
void ov01_021F96E4(struct UnkStruct_ov5_021ED0A4_t *arg0);

void ov01_021F9610(Billboard *billboard, BillboardAnimState *state) {
    state->draw = sub_02023EB8(billboard);
    state->animNum = sub_02023EF4(billboard);
    state->frameNum = sub_02023F30(billboard);
}

s32 sub_02023EE0(Billboard *, int);

s32 sub_02023F1C(Billboard *, s32);

void ov01_021F9630(Billboard *billboard, const BillboardAnimState *state) {
    sub_02023EA4(billboard, (s32)(u8)state->draw);
    sub_02023EE0(billboard, (u16)state->animNum);
    sub_02023F1C(billboard, state->frameNum);
}

Billboard *ov01_021F9528(LocalMapObject *, s32);


void *ov01_021FA3DC(LocalMapObject *obj);

s32 ov01_021FA75C(void *, LocalMapObject *, Billboard **, s32);

void ov01_021F9654(LocalMapObject *arg0, Billboard **arg1, int arg2) {
    void *v0;

    v0 = ov01_021FA3DC(arg0);
    if (ov01_021F9974(v0, arg2) == 0) {
        *arg1 = ov01_021F9528(arg0, arg2);
        return;
    }
    ov01_021FA75C(v0, arg0, arg1, arg2);
}

s32 ov01_021FA798(UnkStruct_ov5_021ED0A4 *, const LocalMapObject *);

void ov01_021F9688(void *arg0);

void ov01_021F9688(void *arg0) {
    ov01_021FA798(ov01_021FA3DC(arg0), arg0);
}

void ov01_021F9698(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    int v0;
    BillboardResources *v2;
    UnkStruct_ov5_021ED110 *v3;

    v0 = sizeof(BillboardResources) * param1;
    v2 = Heap_Alloc(HEAP_ID_FIELD1, v0);

    param0->unk_E4 = v2;
    ((v2 != 0L) ? (void)0 : GF_AssertFail());

    v0 = (sizeof(UnkStruct_ov5_021ED110)) * param1;
    v3 = Heap_Alloc(HEAP_ID_FIELD1, v0);

    param0->unk_F4 = v3;
    ((v3 != 0L) ? (void)0 : GF_AssertFail());

    do {
        v3->unk_00 = 0xffff;
        v3->unk_04 = v2;
        v3++;
        v2++;
        param1--;
    } while (param1);
}

void ov01_021F96E4(struct UnkStruct_ov5_021ED0A4_t *arg0) {
    ov01_021F9798(arg0);
    Heap_FreeExplicit(HEAP_ID_FIELD1, *((void * *)&arg0->unk_E4));
    Heap_FreeExplicit(HEAP_ID_FIELD1, *((void * *)&arg0->unk_F4));
}
