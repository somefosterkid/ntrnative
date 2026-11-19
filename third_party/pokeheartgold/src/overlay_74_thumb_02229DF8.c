#include "global.h"

#include "heap.h"
#include "unk_02037C94.h"
#include "unk_02033AE0.h"

extern void *ov74_0223D0A4;

void ov74_02229DF8(void);

extern s32 _0223B368;
void ov74_02229E14(void *arg0);

void ov74_02229DF8(void) {
    Heap_FreeExplicit(HEAP_ID_15, ov74_0223D0A4);
    ov74_0223D0A4 = NULL;
    sub_02037E9C();
}

void ov74_02229E14(void *arg0) {
    sub_0203410C(&_0223B368, 2, arg0);
}
