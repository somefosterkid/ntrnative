#include "global.h"

#include "heap.h"
#include "pm_string.h"
#include "constants/heap.h"
#include "error_handling.h"
#include "overlay_75.h"

s32 ov75_02249838(String *arg0, enum HeapID arg1);

s32 ov75_02249838(String *string, enum HeapID heapID) {
    u16 *v0;
    s32 v2;
    u16 *v1;

    v2 = 1;
    v0 = Heap_Alloc(heapID, 0xC8U);
    CopyStringToU16Array(string, v0, 0x64U);
    if (*v0 != 0xFFFF) {
        v1 = v0;
loop_2:
        if (*v1 != 0x121) {
            v2 = 0;
        } else {
            v1 += 1;
            if (*v1 != 0xFFFF) {
                goto loop_2;
            }
        }
    }
    Heap_Free(v0);
    return v2;
}

void ov75_02249884(const char *param0, u32 *param1, int param2) {
    int v0, v1;
    int v2 = 0;
    const char *v3;

    for (v0 = 0; v0 < 4; v0++) {
        param1[v0] = 0;
        for (v1 = 0; v1 < 4; v1++) {
            while (param0[v2] < '0' || param0[v2] > '9') {
                if (param0[v2] == '\0') {
                    GF_ASSERT(param0[v2] != '\0');
                    break;
                }
                v2++;
            }
            v3 = &param0[v2];
            param1[v0] *= 10;
            param1[v0] += *v3 - '0';
            v2++;
        }
    }
}
