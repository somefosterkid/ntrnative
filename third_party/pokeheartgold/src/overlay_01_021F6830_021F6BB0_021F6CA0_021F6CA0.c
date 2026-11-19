#include "global.h"
struct UnkStruct_ov5_021EAE78;

#include "field_types_def.h"
#include "overlay_01.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"

s32 ov01_021F6C4C(void *arg0);

s32 ov01_021F6C7C(void *arg0);

s32 ov01_021F6CA0(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6CA0(UnkStruct_ov5_021EAE78 *arg0) {
    u16 v0;

    v0 = arg0->unk_98;
    switch (v0) {
    case 0:
        ov01_021F6A9C(*((FieldSystem * *)&arg0->fieldSystem), 3, NULL);
        arg0->unk_98 = (u16)(arg0->unk_98 + 1);
    default:
        break;
    case 1:
        if (ov01_021F6C4C(arg0) == 1) {
            arg0->unk_98 = (u16)(arg0->unk_98 + 1);
        }
        break;
    case 2:
        if (ov01_021F6C7C(arg0) == 1) {
            arg0->unk_98 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
