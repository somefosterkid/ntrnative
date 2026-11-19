#include "global.h"

typedef struct {
    u32 val : 16;
    u32 rank : 8;
    u32 idx : 4;
    u32 extra : 4;
} UnkStruct_ov96_021ECC7C;

s32 ov96_021ECC4C(void *, u8);

s32 ov96_021ECC58(void *elem1, void *elem2);

void ov96_021ECC7C(void *arg0, UnkStruct_ov96_021ECC7C *arr, u8 *arg2);

void ov96_021ECC7C(void *arg0, UnkStruct_ov96_021ECC7C *arr, u8 *arg2) {
    int i;
    int rank = 0;

    for (i = 0; i < 4; i++) {
        arr[i].idx = i;
        arr[i].val = ov96_021ECC4C(arg0, i);
        arr[i].extra = arg2[i];
    }
    MATH_QSort(arr, 4, 4, ov96_021ECC58, NULL);
    for (i = 0; i < 4; i++) {
        arr[i].rank = rank;
        if (i != 3 && arr[i].val != arr[i + 1].val) {
            rank = i + 1;
        }
    }
}
