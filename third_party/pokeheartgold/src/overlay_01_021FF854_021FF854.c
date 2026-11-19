#include "global.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF854.h"

struct FieldEffectManager;
void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);
typedef struct UnkStruct_ov5_021F600C_t {
    FieldEffectManager *unk_00;
} UnkStruct_ov5_021F600C;
void ov01_021FF880(UnkStruct_ov5_021F600C *param0);

void *ov01_021FF854(FieldEffectManager *param0) {
    UnkStruct_ov5_021F600C *v0 = ov01_021F1430(param0, (sizeof(UnkStruct_ov5_021F600C)), 0, 0);
    v0->unk_00 = param0;

    ov01_021FF880(v0);
    return v0;
}
