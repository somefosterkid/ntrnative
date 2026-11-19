#include "global.h"

#include "unk_02005D10.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"

void ov41_022463FC(void);

void ov41_02248020(void *);

void ov41_02248158(void *);

void ov41_02249418(void *);

unsigned int ov41_022492B0(u8 **param_1);

unsigned int ov41_022492B0(u8 **param_1) {
    u8 *iVar1;
    iVar1 = *param_1;
    if ((*((s32 *)&((UnkStruct_ov22_02258EE4 *)iVar1)->unk_10)) != 0) {
        ov41_02248020(((UnkStruct_ov22_02258EE4 *)iVar1)->unk_04);
        ov41_02248158(((UnkStruct_ov22_02258EE4 *)iVar1)->unk_04);
        ov41_022463FC();
        ((UnkStruct_ov22_02258EE4 *)iVar1)->unk_30 = 0;
        PlaySE(0x5eb);
        ov41_02249418(iVar1);
    }
    return;
}
