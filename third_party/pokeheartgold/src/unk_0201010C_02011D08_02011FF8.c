#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "unk_0201010C.h"
#include "screen_fade_fx.h"

s32 sub_02012090(UnkStruct_0201184C *, const UnkStruct_02011738 *, s32, s32, s32, HardwareWindowSettings *, ScreenFadeHBlanks *, enum HeapID);

void sub_02011FF8(ScreenFade *arg0, const UnkStruct_02011738 *arg1) {
    arg0->data = Heap_Alloc(arg0->heapID, sizeof(UnkStruct_0201184C));
    sub_02012090(arg0->data, arg1, arg0->steps, arg0->framesPerStep, arg0->screen, arg0->hwSettings, arg0->hblanks, arg0->heapID);
    arg0->state++;
}

void *sub_02010EC8(void *);

void sub_02010F34(s32, HardwareWindowSettings *, enum DSScreen);

s32 sub_020121A4(UnkStruct_0201184C *);

s32 sub_020121F4(UnkStruct_0201184C *);

s32 sub_02012030(ScreenFade *arg0) {
    s32 v0;
    s32 v2;
    UnkStruct_0201184C *v1;

    v0 = arg0->state;
    v2 = 0;
    v1 = arg0->data;

    switch (v0) {
    case 1:
        if (sub_020121A4(v1) == 1) {
            sub_02010F34(v1->unk_20, v1->unk_24, arg0->screen);
            arg0->state++;
        }
        break;
    case 2:
        sub_020121F4(v1);
        sub_02010EC8(&v1->unk_00);
        Heap_Free(arg0->data);
        arg0->data = NULL;
        v2 = 1;
        arg0->state++;
        break;
    case 3:
        v2 = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    return v2;
}
