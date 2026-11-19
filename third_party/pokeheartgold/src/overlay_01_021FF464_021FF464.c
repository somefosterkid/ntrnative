#include "global.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF464.h"

struct FieldEffectManager;
void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);
void ov01_021FF490(FieldEffectRendererContext *param0);

void *ov01_021FF464(FieldEffectManager *param0) {
    FieldEffectRendererContext *v0 = ov01_021F1430(param0, (sizeof(FieldEffectRendererContext)), 0, 0);
    v0->fieldEffMan = param0;

    ov01_021FF490(v0);
    return v0;
}
