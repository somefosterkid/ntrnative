#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02035900.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct TextOBJ;

struct GF3DVramMan;

struct MsgData;

struct ParticleSystem;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

extern void (*const ov93_02263114[8])(UnkStruct_ov117_02261280_t *, const UnkStruct_ov117_022621D4 *);

BOOL ov93_022627C0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022621D4 *param1);

void ov93_0225E230(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_022627E8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1);
BOOL ov93_0225FEC4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1);
void ov93_02262814(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);

void ov93_022629A8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_02262920(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_02262960(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_022629E4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_0225E48C(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2);
BOOL ov93_02260908(UnkStruct_ov117_02261280_t *param0);
void ov93_0225FBF0(BgConfig *param0, int param1, int param2, UnkStruct_ov117_02263DAC *param3);
void ov93_02262830(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2);
void ov93_02262860(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_02262884(UnkStruct_ov117_02261280_t *param0, int param1, int param2, u8 param3);
void ov93_022628B8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_022628F4(UnkStruct_ov117_02261280_t *param0, u8 param1);
void ov93_02262920(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_02262934(UnkStruct_ov117_02261280_t *param0, u8 param1);
void ov93_02262960(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_0226297C(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2);
void ov93_022629A8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);
void ov93_022629B8(UnkStruct_ov117_02261280_t *param0, u8 param1);
void ov93_022629E4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1);

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/overlay_93_thumb_2.s keeps the DS copy; there slot 0 is a literal .word 0 between .word SYMBOL slots, which the 64-bit port
// emits as 4 bytes among 8-byte pointers (wordmix.py): the PC build indexes this C table instead.
void (*const ov93_02263114[8])(UnkStruct_ov117_02261280_t *, const UnkStruct_ov117_022621D4 *) = {
    NULL,
    ov93_02262814,
    ov93_02262860,
    ov93_022628B8,
    ov93_02262920,
    ov93_02262960,
    ov93_022629A8,
    ov93_022629E4,
};
#endif

BOOL ov93_022627C0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022621D4 *param1) {
    if (ov93_02263114[param1->unk_00] != NULL) {
        ov93_02263114[param1->unk_00](param0, param1);
        MI_CpuClear8(param1, sizeof(UnkStruct_ov117_022621D4));
        return 1;
    }

    return 0;
}

void ov93_022627E8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 1;
    v0.unk_04_val4 = *param1;

    ov93_0225E230(param0, &v0);
}

void ov93_02262814(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    if (param1->unk_04_val4.unk_00 == sub_0203769C()) {
        return;
    }

    ov93_0225FEC4(param0, &param1->unk_04_val4);
}

void ov93_02262830(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 2;
    v0.unk_04_val5.unk_00 = param1;
    v0.unk_04_val5.unk_02 = 5;
    v0.unk_04_val5.unk_03 = param2;

    ov93_0225E230(param0, &v0);
}

void ov93_02262860(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    param0->unk_2FD0++;

    ov93_02260908(param0);
    ov93_0225E48C(param0, param1->unk_04_val5.unk_02, param1->unk_04_val5.unk_03);
}

void ov93_02262884(UnkStruct_ov117_02261280_t *param0, int param1, int param2, u8 param3) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 3;
    v0.unk_04_val6.unk_00 = param1;
    v0.unk_04_val6.unk_02 = param2;
    v0.unk_04_val6.unk_03 = 4;
    v0.unk_04_val6.unk_04 = param3;

    ov93_0225E230(param0, &v0);
}

void ov93_022628B8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    param0->unk_2FC8 = param1->unk_04_val6.unk_00;
    ov93_0225FBF0(param0->unk_2C, param0->unk_00->unk_30, param1->unk_04_val6.unk_02, &param0->unk_1428);
    param0->unk_2FCC = 1;

    ov93_0225E48C(param0, param1->unk_04_val6.unk_03, param1->unk_04_val6.unk_04);
}

void ov93_022628F4(UnkStruct_ov117_02261280_t *param0, u8 param1) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 4;
    v0.unk_04_val1.unk_00 = 2;
    v0.unk_04_val1.unk_01 = param1;

    ov93_0225E230(param0, &v0);
}

void ov93_02262920(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    param0->unk_20 = 3;
    ov93_0225E48C(param0, param1->unk_04_val2.unk_00, param1->unk_04_val2.unk_01);
}

void ov93_02262934(UnkStruct_ov117_02261280_t *param0, u8 param1) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 5;
    v0.unk_04_val3.unk_00 = 3;
    v0.unk_04_val3.unk_01 = param1;

    ov93_0225E230(param0, &v0);
}

void ov93_02262960(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    param0->unk_2FBC = 1;
    param0->unk_20 = 6;
    ov93_0225E48C(param0, param1->unk_04_val3.unk_00, param1->unk_04_val3.unk_01);
}

void ov93_0226297C(UnkStruct_ov117_02261280_t *param0, int param1, u8 param2) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 6;
    v0.unk_04_val1.unk_00 = param1;
    v0.unk_04_val1.unk_01 = param2;

    ov93_0225E230(param0, &v0);
}

void ov93_022629A8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    ov93_0225E48C(param0, param1->unk_04_val1.unk_00, param1->unk_04_val1.unk_01);
}

void ov93_022629B8(UnkStruct_ov117_02261280_t *param0, u8 param1) {
    UnkStruct_ov117_022621D4 v0;

    MI_CpuClear8(&v0, sizeof(UnkStruct_ov117_022621D4));

    v0.unk_00 = 7;
    v0.unk_04_val1.unk_00 = 6;
    v0.unk_04_val1.unk_01 = param1;

    ov93_0225E230(param0, &v0);
}

void ov93_022629E4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_022621D4 *param1) {
    param0->unk_20 = 1;
    ov93_0225E48C(param0, param1->unk_04_val2.unk_00, param1->unk_04_val2.unk_01);
}
