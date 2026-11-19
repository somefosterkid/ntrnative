#include "global.h"

#include "sys_task.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

void ov41_02247C7C(UnkStruct_ov22_0225A0E4 *arg0, s32 strno);
void ov41_02247D00(void *arg0);

void ov41_02247D34(UnkStruct_ov22_0225A0E4 *arg0, s32 strno);
void ov41_02247D3C(void *arg0);

s32 ov41_022482B4(u8 *);
s32 ov41_0224AC98(void *, s32);
void ov41_02247D44(u8 *arg0);

void ov41_02247D34(UnkStruct_ov22_0225A0E4 *arg0, s32 strno) {
    ov41_02247C7C(arg0, strno);
}

void ov41_02247D3C(void *arg0) {
    ov41_02247D00(arg0);
}

void ov41_02247D44(u8 *arg0) {
    ov41_0224AC98(arg0 + 0x568, ov41_022482B4(arg0 + 0x3F4));
}
