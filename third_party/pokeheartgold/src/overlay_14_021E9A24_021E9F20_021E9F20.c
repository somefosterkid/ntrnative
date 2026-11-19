#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *arg0);


s32 sub_02019978(void *, s32);
s32 ov14_021E9F20(UnkStruct_ov14_boxapp *arg0) {
    u8 *data;
    s16 sp0[2];
    u32 n;
    data = arg0->sys->unk_C;
    sub_02019978(((u8 *)arg0->sys->unk_2F0), 0xF);
    n = (*((u32 *)(data + 4))) >> 2;
    if (n == 0) {
        ManagedSprite_SetPositionXY(arg0->sys->sprites[9], data[0], data[1]);
        if ((*((u8 *)&arg0->sys->unk_44B)) == 1) {
            if ((*((s32 *)((*((u8 **)arg0)) + 8))) == 3) {
                ManagedSprite_SetPositionXY(arg0->sys->sprites[11], data[0], (s16)(data[1] + 8));
            } else {
                ManagedSprite_SetPositionXY(arg0->sys->sprites[arg0->sys->unk_4094[arg0->unk21]], data[0], (s16)(data[1] + 4));
            }
        }
        return 0;
    }
    *((u32 *)(data + 4)) = ((n - 1) * 4) | ((*((u32 *)(data + 4))) & 3);
    ManagedSprite_GetPositionXY(arg0->sys->sprites[9], &sp0[1], &sp0[0]);
    n = (s32)((*((u32 *)(data + 4))) << 31);
    n = n >> 31;
    if (n == 0) {
        sp0[1] = sp0[1] + data[2];
    } else {
        sp0[1] = sp0[1] - data[2];
    }
    n = ((u32)((*((u32 *)(data + 4))) << 30)) >> 31;
    if (n == 0) {
        sp0[0] = sp0[0] + data[3];
    } else {
        sp0[0] = sp0[0] - data[3];
    }
    ManagedSprite_SetPositionXY(arg0->sys->sprites[9], sp0[1], sp0[0]);
    if ((*((u8 *)&arg0->sys->unk_44B)) == 1) {
        if ((*((s32 *)((*((u8 **)arg0)) + 8))) == 3) {
            ManagedSprite_SetPositionXY(arg0->sys->sprites[11], sp0[1], (s16)(sp0[0] + 8));
        } else {
            ManagedSprite_SetPositionXY(arg0->sys->sprites[arg0->sys->unk_4094[arg0->unk21]], sp0[1], (s16)(sp0[0] + 4));
        }
    }
    return 1;
}
