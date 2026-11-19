#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02085604.h"
#include "unk_struct_02089688.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct TextOBJ;
struct TouchHitboxController;
struct UnkStruct_02012744_t;
typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;
typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchHitboxController *unk_14;
    TouchscreenHitbox unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    FontOAM *unk_90[2];
    UnkStruct_02021AC8 unk_98[2];
    Window unk_B0;
} UnkStruct_02089688_sub1;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02089688_sub2;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_0208AF44_sub1;
typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchscreenHitbox *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;
void sub_02085F80(UnkStruct_02089688 *param0);
void sub_02085FFC(UnkStruct_02089688 *param0);
void sub_02086AB4(UnkStruct_02089688 *param0, int param1, BOOL param2);
void sub_02086B2C(UnkStruct_02089688 *param0, int param1);
void sub_02086B6C(UnkStruct_02089688 *param0, int param1, int param2);
int sub_02086D98(int param0, BOOL param1);
void sub_02085604(UnkStruct_02089688 *param0, int param1);
void sub_02085808(UnkStruct_02089688 *param0, int param1);
void sub_02086384(UnkStruct_02089688 *param0);
void sub_02086AE4(UnkStruct_02089688 *param0, int param1);
void sub_02086DA4(UnkStruct_02089688 *param0);
void sub_02086DE4(UnkStruct_02089688 *param0, int param1);

void sub_02086328(UnkStruct_02089688 *param0);
void sub_02086384(UnkStruct_02089688 *param0);
int sub_02086398(UnkStruct_02089688 *param0, int param1);
int sub_020863C0(UnkStruct_02089688 *param0, int param1);

void sub_02086180(u32 param0, enum TouchScreenButtonState param1, void *param2) {
    UnkStruct_02089688 *v0 = param2;

    if (v0->unk_2C0 != 1) {
        return;
    }

    if (v0->unk_2EC.unk_88 != 1) {
        v0->unk_2EC.unk_88 = 1;
    }

    if (param1 == TOUCH_BUTTON_PRESSED) {
        if ((param0 >= 0) && (param0 < 16)) {
            if (param0 < v0->unk_3F0) {
                return;
            }

            if (v0->unk_00[param0].unk_08 == 1) {
                v0->unk_3AC.unk_00 = 2;
                v0->unk_3AC.unk_04 = param0;
            } else {
                v0->unk_3AC.unk_00 = 1;
                v0->unk_3AC.unk_04 = v0->unk_00[param0].unk_04;
            }

            PlaySE(1509);
        } else {
            if (param0 == 26) {
                v0->unk_214[1].unk_14.unk_00 = 0;
                v0->unk_214[1].unk_14.unk_02 = 2;
                PlaySE(1509);
            } else if (param0 == 27) {
                v0->unk_214[1].unk_14.unk_00 = 3;
                v0->unk_214[1].unk_14.unk_02 = 2;
                PlaySE(1506);
            } else {
                v0->unk_214[1].unk_14.unk_00 = (param0 - 16) % 5;
                v0->unk_214[1].unk_14.unk_02 = (param0 - 16) / 5;
                PlaySE(1509);
            }

            if ((param0 >= 16) && (param0 <= 25)) {
                int v1;
                int v2;
                int v3;

                if (v0->unk_2D4 == 0) {
                    return;
                }

                v1 = v0->unk_214[0].unk_00;
                v0->unk_00[v1].unk_00 = param0 - 16 + 1;

                ManagedSprite_SetAnim(v0->unk_00[v1].unk_0C, sub_02086D98(v0->unk_00[v1].unk_00, v0->unk_00[v1].unk_08));
                sub_02086AB4(v0, 1, 1);
                sub_02086B2C(v0, param0 - 16);
                sub_02086AB4(v0, 1, 0);
                sub_02086AB4(v0, 2, 1);
                sub_02086B6C(v0, param0 - 16, 2);
                ManagedSprite_SetAnim(v0->unk_214[2].unk_0C, 3);

                v2 = v0->unk_00[v1].unk_04;
                v1++;

                if (v1 == v0->unk_2D0) {
                    v0->unk_3AC.unk_00 = 1;
                    v0->unk_3AC.unk_04 = 0;
                    v0->unk_3AC.unk_08 = 0;
                } else {
                    v3 = v0->unk_00[v1].unk_04;

                    if (v2 != v3) {
                        v0->unk_3AC.unk_00 = 1;
                        v0->unk_3AC.unk_04 = v3;
                        v0->unk_3AC.unk_08 = 0;
                    } else {
                        v0->unk_3AC.unk_00 = 2;
                        v0->unk_3AC.unk_04 = v1;
                    }
                }
            } else {
                if (param0 == 26) {
                    sub_02085FFC(v0);
                } else {
                    sub_02085F80(v0);
                }
            }
        }
    }
}

void sub_02086328(UnkStruct_02089688 *param0) {
    switch (param0->unk_3AC.unk_00) {
    case 0:
        break;
    case 1:
        sub_02085604(param0, param0->unk_3AC.unk_04);
        sub_02086DA4(param0);
        sub_02086DE4(param0, 1);
        sub_02085808(param0, 2);
        param0->unk_3AC.unk_00 = 0xFF;
        break;
    case 2:
        sub_02086AE4(param0, param0->unk_3AC.unk_04);
        sub_02086384(param0);
        break;
    case 0xFF:
        break;
    }
}

void sub_02086384(UnkStruct_02089688 *param0) {
    param0->unk_3AC.unk_00 = 0;
    param0->unk_3AC.unk_04 = 0;
    param0->unk_3AC.unk_08 = 0;
}

int sub_02086398(UnkStruct_02089688 *param0, int param1) {
    int v0;

    for (v0 = 0; v0 < param0->unk_2D0; v0++) {
        if (param0->unk_00[v0].unk_04 == param1) {
            return v0;
        }
    }

    return 0;
}

int sub_020863C0(UnkStruct_02089688 *param0, int param1) {
    int v0;
    int v1 = 0;
    int v2 = 0;

    for (v0 = 0; v0 < param0->unk_2D0; v0++) {
        if (param0->unk_00[v0].unk_04 == param1) {
            v2 = 1;
        } else {
            if (v2 == 1) {
                return v0 - 1;
            }
        }
    }

    return param0->unk_2D0 - 1;
}
