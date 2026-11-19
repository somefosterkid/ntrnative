#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "heap.h"

void ov96_021E8988(s32 *param_1, s32 param_2, void *param_3, size_t param_4);

void sub_02036FD8(s32, void *, size_t);

void ov96_021E87EC(s32 param_1, void *param_2, size_t param_3, u16 *param_4);

s32 ov96_021E88FC(void *);

void ov96_021E87EC(s32 param_1, void *param_2, size_t param_3, u16 *param_4) {
    if (*param_4 == 1) {
        sub_02036FD8(param_1, param_2, param_3);
        return;
    }
    ov96_021E8988(*(s32 **)(param_4 + 2), param_1, param_2, param_3);
    return;
}

void ov96_021E8810(void *ptr) {
    if (*(u16 *)ptr == 0) {
        ov96_021E88FC(*(void **)((u8 *)ptr + 4));
    }
    Heap_Free(ptr);
}
