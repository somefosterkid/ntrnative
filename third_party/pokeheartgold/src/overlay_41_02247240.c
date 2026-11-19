#include "global.h"
#include "sys_task.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

void ov41_02246130(void);

void ov41_02246170(void *);

void ov41_02246250(void *, u8 *);

void ov41_022463B0(void *, u8 *);

void ov41_022463D4(u8 *);

void ov41_02247240(void *param_1);

void ov41_0224621C(UnkStruct_ov22_0225A0E4 *arg0);
void ov41_0224726C(UnkStruct_ov22_0225A0E4 *arg0);

s32 ov41_02246150(void);
s32 ov41_022461D0(void *);
s32 ov41_0224626C(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02247274(void *arg0);

void ov41_02247240(void *param_1) {
    u8 auStack_20[24];

    ov41_02246130();
    ov41_02246170(param_1);
    ov41_022463B0(param_1, auStack_20);
    ov41_02246250(param_1, auStack_20);
    ov41_022463D4(auStack_20);
    return;
}

void ov41_0224726C(UnkStruct_ov22_0225A0E4 *arg0) {
    ov41_0224621C(arg0);
}

void ov41_02247274(void *arg0) {
    ov41_0224626C(arg0);
    ov41_022461D0(arg0);
    ov41_02246150();
}
