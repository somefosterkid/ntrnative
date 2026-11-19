#include "global.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE590.h"

struct FieldEffectManager;
void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    FieldEffectManager *unk_08;
} UnkStruct_ov5_021F5A34;

void *ov01_021FE590(FieldEffectManager *param0) {
    UnkStruct_ov5_021F5A34 *v0 = ov01_021F1430(param0, (sizeof(UnkStruct_ov5_021F5A34)), 0, 0);
    v0->unk_08 = param0;

    return v0;
}
