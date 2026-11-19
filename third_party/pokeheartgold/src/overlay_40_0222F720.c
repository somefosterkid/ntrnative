#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "touchscreen.h"

struct MessageLoader;

typedef struct MessageLoader MessageLoader;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_02233310_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct {
    const UnkStruct_ov62_02249380 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    u32 unk_24;
    const TouchscreenHitbox *unk_28;
} UnkStruct_ov62_02248CDC;

typedef struct {
    int unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[2];
    int unk_0C;
    int unk_10;
    int unk_14;
    Window unk_18;
    const UnkStruct_ov62_02248CDC *unk_28;
    ManagedSprite *unk_2C[2];
    MessageLoader *unk_34;
    UnkStruct_ov62_02233310_sub1 unk_38;
    BOOL unk_48;
    u8 padding_4C[4];
} UnkStruct_ov62_02233310;

void ov40_0222F720(UnkStruct_ov62_02233310 *param0);

void ov40_0222F734(UnkStruct_ov62_02233310 *arg0);

void ov40_0222F720(UnkStruct_ov62_02233310 *param0) {
    ClearWindowTilemapAndCopyToVram(&param0->unk_18);
    RemoveWindow(&param0->unk_18);
}

void ov40_0222F734(UnkStruct_ov62_02233310 *arg0) {
    memset(arg0, 0, sizeof(UnkStruct_ov62_02233310));
}
