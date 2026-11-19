#include "global.h"

#include "overlay_41_unk_struct_ov22_02259560.h"

void ov41_02249A40(UnkStruct_ov22_02259560 *);

void ov41_02249A60(UnkStruct_ov22_02259560 *);

void ov41_02249A70(UnkStruct_ov22_02259560 *param_1);

void ov41_02249A70(UnkStruct_ov22_02259560 *param_1) {
    UnkStruct_ov22_02259560 *iVar1;
    UnkStruct_ov22_02259560 *iVar2;

    iVar1 = param_1->unk_08;
    while (iVar1 != param_1) {
        iVar2 = iVar1->unk_08;
        ov41_02249A60(iVar1);
        ov41_02249A40(iVar1);
        iVar1 = iVar2;
    }
    return;
}
