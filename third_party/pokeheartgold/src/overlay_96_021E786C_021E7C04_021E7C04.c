#include "global.h"

BOOL ov96_021E7D18(s32 a, s32 b, u32 c);

void ov96_021E7C04(u32 param0, u32 *param1, u8 *param2);

typedef struct {
    u32 w[8];
} UnkStruct_ov96_021E7C04;

void ov96_021E7C04(u32 param0, u32 *param1, u8 *param2) {
    int i;
    u32 *ptr;
    UnkStruct_ov96_021E7C04 tmp;

    if (!ov96_021E7D18((*(u16 *)((u8 *)(param2) + (0x80))), (*(u16 *)((u8 *)(param1) + (0))), param0)) {
        return;
    }
    *(UnkStruct_ov96_021E7C04 *)(param2 + 0x80) = *(UnkStruct_ov96_021E7C04 *)param1;
    ptr = (u32 *)(param2 + 0x80);
    for (i = 4; i > 0; i--) {
        if (!ov96_021E7D18((*(u16 *)((u8 *)(ptr) + (-0x20))), (*(u16 *)((u8 *)(ptr) + (0))), param0)) {
            return;
        }
        tmp = *(UnkStruct_ov96_021E7C04 *)(ptr - 8);
        *(UnkStruct_ov96_021E7C04 *)(ptr - 8) = *(UnkStruct_ov96_021E7C04 *)ptr;
        *(UnkStruct_ov96_021E7C04 *)ptr = tmp;
        ptr -= 8;
    }
}
