#include "global.h"

#include "message_format.h"
#include "unk_0203170C.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

s32 ov75_02246CE8(void *);

s32 ov75_022494CC(void *, void *, s32, s32, u16);

s32 ov75_02249534(s32, s32, s32);

s32 ov75_02249884(Unk0203170C *, s32 *, s32);

s32 ov75_022495B0(void *arg0, s32 arg1, s32 arg2);

s32 ov75_022495B0(void *arg0, s32 arg1, s32 arg2) {
    s32 sp8[4];
    s32 temp_r0;
    s32 temp_r5;
    s32 var_r2;
    u32 var_r4;
    void *temp_r4;

    temp_r4 = *(void **)arg0;
    if (ov75_02249534(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_44), arg1, arg2) == 1) {
        return 0;
    }
    temp_r0 = *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94);
    switch (temp_r0) {
    case 0:
        temp_r5 = ov75_02246CE8(temp_r4);
        if (temp_r5 == sub_020317BC(((UnkStruct_ov98_02246E88 *)temp_r4)->saveData, 3U)) {
            s32 *var_r5;
            ov75_02249884(sub_02031774(((UnkStruct_ov98_02246E88 *)temp_r4)->saveData), sp8, 0x73);
            var_r4 = 0;
            var_r5 = sp8;
            do {
                BufferIntegerAsString(((MessageFormat *)((UnkStruct_ov98_02247704 *)arg0)->unk_20), var_r4, *var_r5, 4U, PRINTING_MODE_LEADING_ZEROS, 1);
                var_r4 += 1;
                var_r5 += 1;
            } while (var_r4 < 4U);
            var_r2 = 0x29;
        } else {
            var_r2 = 0x28;
        }
        ov75_022494CC(arg0, *((void **)&((UnkStruct_ov98_02247704 *)arg0)->unk_34), var_r2, 1, 0xF0F);
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) = (s32)(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) + 1);
        break;
    case 1:
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = 0;
        break;
    }
    return 0;
}
