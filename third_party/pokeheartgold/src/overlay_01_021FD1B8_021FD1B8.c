#include "global.h"

#include "overlay_01_021FD1B8.h"
#include "overlay_01_021F1348.h"

void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);

void *ov01_021FD1B8(FieldEffectManager *param0) {
    UnkStruct_021F114C *v0 = ov01_021F1430(param0, sizeof(UnkStruct_021F114C), 0, 0);
    v0->unk_00 = param0;

    return v0;
}
