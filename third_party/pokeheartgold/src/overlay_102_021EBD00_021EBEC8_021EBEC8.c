#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov102_021EBEC8 {
    u8 unk_0[0x40];
    u8 unk_40;
    u8 unk_41[0x3F];
    s32 unk_80;
} UnkStruct_ov102_021EBEC8;

s32 ov102_021EC3AC(void *);

u32 ov102_021EBEC8(UnkStruct_ov102_021EBEC8 *param_1);

u32 ov102_021EBEC8(UnkStruct_ov102_021EBEC8 *param_1) {
    s32 iVar1;
    if (param_1->unk_80 == 0) {
        iVar1 = ov102_021EC3AC((u8 *)&param_1->unk_40);
        if (iVar1 != 0) {
            ToggleBgLayer(2, 0);
            return 1;
        }
    } else {
        return 1;
    }
    return 0;
}
