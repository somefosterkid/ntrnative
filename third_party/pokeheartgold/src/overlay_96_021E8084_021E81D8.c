#include "global.h"

#include "sprite.h"
#include "sys_task.h"

typedef struct UnkStruct_ov96_021E81D8_param_2 {
    u8 unk_0[0x4];
    void *unk_4;
    Sprite *unk_8;
} UnkStruct_ov96_021E81D8_param_2;

void ov96_021EAEC8(void *, s32 *, s32 *);

s32 ov96_021EB120(s32 *);

void ov96_021E81D8(SysTask *task, void *param);

void ov96_021E81D8(SysTask *task, void *param) {
    UnkStruct_ov96_021E81D8_param_2 *param_2 = param;
    s32 iVar1;
    s32 iStack_1c;
    s32 iStack_20;
    VecFx32 VStack_18;

    ov96_021EAEC8(param_2->unk_4, &iStack_1c, &iStack_20);
    Sprite_SetDrawFlag(param_2->unk_8, 1);
    VStack_18.z = 0;
    VStack_18.x = iStack_1c << 0xc;
    VStack_18.y = iStack_20 << 0xc;
    Sprite_SetMatrix(param_2->unk_8, &VStack_18);
    iVar1 = ov96_021EB120(param_2->unk_4);
    if (iVar1 != 0) {
        Sprite_SetDrawFlag(param_2->unk_8, 1);
        return;
    }
    Sprite_SetDrawFlag(param_2->unk_8, 0);
    return;
}
