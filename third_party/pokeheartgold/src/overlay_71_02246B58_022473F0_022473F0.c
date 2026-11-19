#include "global.h"

#include "heap.h"

extern s32 _0224C040;

extern void *ov71_0224C044[32];

void ov71_022473F0(void);

void ov71_022473F0(void) {
    s32 iVar1;
    void **puVar2;

    if (_0224C040 != 0) {
        iVar1 = 0;
        if (0 < _0224C040) {
            puVar2 = ov71_0224C044;
            do {
                Heap_Free(*puVar2);
                iVar1 = iVar1 + 1;
                puVar2 = puVar2 + 1;
            } while (iVar1 < _0224C040);
        }
        _0224C040 = 0;
    }
    return;
}
