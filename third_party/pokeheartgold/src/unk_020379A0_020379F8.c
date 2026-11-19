#include "global.h"

#include "heap.h"
#include "comm_tool.h"


void sub_020379F8(void);

void sub_020379F8(void) {
    Heap_Free(_021D414C);
    _021D414C = NULL;
}

s32 sub_02037A10(void);

s32 sub_02037A10(void) {
    if (_021D414C != 0) {
        return 1;
    }
    return 0;
}
