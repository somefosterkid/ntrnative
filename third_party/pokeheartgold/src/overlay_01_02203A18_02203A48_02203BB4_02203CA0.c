#include "global.h"

#include "heap.h"
#include "overlay_01_02203A18.h"

void sub_02023DA4(void *);

void ov01_02203CA0(void *param_1, UnkOv01_02203A18_Work *param_2);

void ov01_02203CA0(void *param_1, UnkOv01_02203A18_Work *param_2) {
    sub_02023DA4(param_2->unk3C);
    Heap_Free(param_2->anim.unk14);
    Heap_Free(param_2->anim.unk18);
    return;
}
