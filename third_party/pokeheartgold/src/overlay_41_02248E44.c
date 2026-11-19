#include "global.h"

#include "system.h"

typedef struct UnkStruct_ov22_02258A48_t UnkStruct_ov22_02258A48;

struct UnkStruct_ov22_02258A48_t {
    void *unk_00;
    void (*unk_04)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_08)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_0C)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_10)(struct UnkStruct_ov22_02258A48_t *param0);
    s16 unk_14;
    s16 unk_16;
    u8 unk_18;
    u8 padding[3];
};

void ov41_02248E44(UnkStruct_ov22_02258A48 *param0);

void ov41_02248E80(void);

void ov41_02248E44(UnkStruct_ov22_02258A48 *param0) {
    if (gSystem.touchNew != 0) {
        param0->unk_04(param0);
    } else if (gSystem.touchHeld != 0) {
        param0->unk_0C(param0);
    } else if (param0->unk_18 != 0) {
        param0->unk_08(param0);
    }
    param0->unk_14 = (s16)gSystem.touchX;
    param0->unk_16 = (s16)gSystem.touchY;
    param0->unk_18 = (u8)gSystem.touchHeld;
}

void ov41_02248E80(void) {
}
