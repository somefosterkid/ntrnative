#include "global.h"

#include "overlay_01_021F1348.h"

void *ov01_021F141C(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd);

void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue) {
    void *v0;

    v0 = ov01_021F141C(fieldEffMan, size, allocAtEnd);
    memset(v0, initValue, size);
    return v0;
}
