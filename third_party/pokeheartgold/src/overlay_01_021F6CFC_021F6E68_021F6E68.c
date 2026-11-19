#include "global.h"
struct UnkStruct_ov5_021EAE78;

#include "unk_0202C034.h"
#include "unk_02037C94.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"

s32 ov01_021F6CA0(UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F71C4(void *, s32);

s32 sub_0202C2DC(WiFiList *, s32);

s32 ov01_021F6E68(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6E68(UnkStruct_ov5_021EAE78 *arg0) {
    s32 var_r4;
    WiFiList *temp_r5;
    if (ov01_021F6CA0(arg0) == 0) {
        if ((*((s32 *)&arg0->unk_90)) == 0) {
            temp_r5 = sub_0202C6F4(arg0->saveData);
            for (var_r4 = 0; var_r4 < 0x20; var_r4++) {
                if (sub_0202C2DC(temp_r5, var_r4) == 0) {
                    sub_0203A280(arg0->saveData, *((s32 *)&arg0->unk_6C[7]), var_r4, HEAP_ID_FIELD1, 0U);
                    break;
                }
            }

            if (var_r4 == 0x20) {
                ov01_021F71C4(arg0, 0x3F);
                *((s32 *)&arg0->unk_44) = 4;
                return 0;
            }
        }
        *((s32 *)&arg0->unk_44) = 1;
        return 0;
    }
    return 0;
}
