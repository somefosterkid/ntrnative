#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F49E0(void *);
struct UnkStruct_ov14_boxsys;

s32 sub_020137F0(TextOBJ *, s32);

void ov14_021F4BC0(UnkStruct_ov14_boxapp *arg0) {
    s16 sp4[2];
    s32 var_r4;
    u32 var_r6;
    ov14_021F4B90(*((void * *)&arg0->sys), 4, 0xC, 0x54, 1);
    ov14_021F4B90(*((void * *)&arg0->sys), 5, 0xF4, 0x54, 1);
    ov14_021F4B90(*((void * *)&arg0->sys), 6, 0x2B, 0x54, 1);
    ov14_021F4B90(*((void * *)&arg0->sys), 7, 0x80, 0x41, 1);
    ov14_021F4B90(*((void * *)&arg0->sys), 8, 0x80, 0x4D, 1);
    var_r6 = 0;
    var_r4 = 0;
    do {
        ManagedSprite_GetPositionXY(*((ManagedSprite **)(((u8 *)((*((void * *)&arg0->sys)) + var_r4)) + 0x338)), &sp4[1], &sp4[0]);
        ov14_021F4B90(*((void * *)&arg0->sys), var_r6 + 0xF, sp4[1], 0x54, 1);
        var_r4 += 4;
        var_r6 += 1;
    } while (var_r6 < 6U);
    ov14_021F49E0(arg0);
    ov14_021F29E4(*((void * *)&arg0->sys), 7, 5);
    ov14_021F46F4(*((void * *)&arg0->sys));
    TextOBJ_SetSpritesDrawFlag(*((TextOBJ **)(((u8 *)(*((void * *)&arg0->sys))) + 0x414)), 1);
    TextOBJ_SetSpritesDrawFlag(*((TextOBJ **)(((u8 *)(*((void * *)&arg0->sys))) + 0x424)), 1);
    sub_020137F0(*((TextOBJ **)(((u8 *)(*((void * *)&arg0->sys))) + 0x414)), 0);
    sub_020137F0(*((TextOBJ **)(((u8 *)(*((void * *)&arg0->sys))) + 0x424)), 0);
}
