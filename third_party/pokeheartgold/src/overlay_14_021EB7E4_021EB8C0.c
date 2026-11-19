#include "global.h"

#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E8664(void *arg0);
void ov14_021E86E0(UnkStruct_0201956C *arg0);
s32 ov14_021E9554(void *arg0);
void ov14_021F0BF4(void *arg0);
void ov14_021F4720(void *arg0);
s32 ov14_021E7588(void *, s32, s32, s32);
s32 ov14_021EA1F0(void *);
s32 ov14_021F01D8(void *, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F3F6C(void *);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F4848(void *);
s32 ov14_021F48B4(void *);
s32 ov14_021F57B8(void *);
s32 ov14_021F685C(void *, u32, s32, s32);
s32 ov14_021F6AC0(void *, s32, u32);

extern s32 ov14_021F7D1C;

extern s32 ov14_021F7D2C;

extern s32 ov14_021F7D3C;

void ov14_021EB8C0(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;
    s32 var_r4;
    u8 temp_r1;

    ov14_021E7588(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, arg2, arg3);
    if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[0]) == 0) {
        temp_r0 = *(s32 *)((u8 *)(*(void **)arg0) + 8);
        if (temp_r0 == 1) {
            ov14_021F5EE4(arg0, &ov14_021F7D2C, 4);
        } else if (temp_r0 == 0) {
            ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0);
            ov14_021F3488(arg0, 1, 1);
            ov14_021F5EE4(arg0, &ov14_021F7D1C, 4);
        } else {
            if ((u32) ((UnkStruct_ov14_boxapp *)arg0)->unk21 >= 0x1EU) {
                ov14_021F3488(arg0, 1, 1);
                if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk23[1]) != 0) {
                    ov14_021E8664(arg0);
                }
            } else if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk23[1]) != 0) {
                ov14_021E8664(arg0);
            }
            ov14_021F5EE4(arg0, &ov14_021F7D3C, 5);
            ov14_021E87F4(arg0);
        }
        ov14_021E83C4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        temp_r1 = ((UnkStruct_ov14_boxapp *)arg0)->unk21;
        if ((u32)temp_r1 < 0x1EU) {
            if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 1) {
                ov14_021F685C(arg0, temp_r1, 1, 0x27);
                var_r4 = 0x51;
            } else {
                var_r4 = 0xC;
            }
        } else if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 0) {
            ov14_021F0BF4(arg0);
            ov14_021F685C(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, 1, 0x27);
            var_r4 = 0x5B;
        } else {
            ov14_021F0B70(arg0);
            ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0);
            ov14_021F6AC0(arg0, 5, 9U);
            var_r4 = 0x24;
        }
        ov14_021F3F6C(arg0);
        if (*(s32 *)((u8 *)(*(void **)arg0) + 8) != 3) {
            ov14_021E8874(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
        }
    } else {
        ov14_021F0BB4(arg0);
        ov14_021E8610(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        ov14_021F4720(arg0);
        ov14_021F4848(arg0);
        ov14_021F48B4(arg0);
        ov14_021F57B8(arg0);
        ov14_021E86E0(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
        do {

        } while (ov14_021E9554(arg0) != 0);
        if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[1]) == 0) {
            ov14_021F6AC0(arg0, 4, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
        } else {
            ov14_021F6AC0(arg0, 4, *((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[2]));
            ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_44B = 1;
            ov14_021EA1F0(arg0);
        }
        ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_43C = (s32)(((s32) ((UnkStruct_ov14_boxapp *)arg0)->unk25 % 6) + 0x25);
        if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 3) {
            ov14_021F3488(arg0, 0x81, 1);
            ov14_021F3488(arg0, 0x82, 1);
            var_r4 = 0x82;
        } else if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[1]) == 0) {
            var_r4 = 0x29;
        } else {
            var_r4 = 0x73;
        }
        if ((*(s32 *)((u8 *)(*(void **)arg0) + 8) != 3) && (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[1]) != 0)) {
            ov14_021E8874(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
        }
        if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[3]) == 1) {
            ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0);
        }
        *((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[0]) = 0U;
        *((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[2]) = 0U;
        *((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[1]) = 0U;
    }
    ov14_021F01D8(arg0, var_r4);
}
