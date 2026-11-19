#include "global.h"

#include "error_handling.h"

extern void *ov83_02247EE0;

extern u32 ov83_02247F16;

extern void *ov83_02247F88;

extern u32 ov83_02247FC8;

u16 ov83_02240EF8(u32 param_1);

u16 ov83_02240EF8(u32 param_1) {
    u32 uVar1;
    u16 *puVar2;

    puVar2 = (u16 *)&ov83_02247EE0;
    uVar1 = 0;
    do {
        if (param_1 == *puVar2) {
            return *(u16 *)(((u8 *)&ov83_02247F16) + uVar1 * 2);
        }
        uVar1 = uVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (uVar1 < 0x1b);
    puVar2 = (u16 *)&ov83_02247F88;
    uVar1 = 0;
    do {
        if (param_1 == *puVar2) {
            return *(u16 *)(((u8 *)&ov83_02247FC8) + uVar1 * 2);
        }
        uVar1 = uVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (uVar1 < 0x20);
    GF_AssertFail();
    return 0;
}
