#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "palette.h"
#include "sprite.h"
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

void ov93_02262484(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266344 *param1);

void ov93_022612E0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov117_02266BC4;
extern const UnkStruct_ov117_02266BC4 ov93_02262CC4[5];
void ov93_02261310(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1);

void ov93_022612E0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        Sprite_DeleteAndFreeResources(param1->unk_00[v0].unk_00);
        Sprite_DeleteAndFreeResources(param1->unk_00[v0].unk_04);
        Sprite_DeleteAndFreeResources(param1->unk_00[v0].unk_08);
        ov93_02262484(param0, &param1->unk_00[v0].unk_30);
    }
}

void ov93_02261310(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1) {
    int v0;

    param1->unk_E8 = (360 << FX32_SHIFT) / 12 / ov93_02262CC4[param1->unk_F4].unk_00;

    for (v0 = 0; v0 < 3; v0++) {
        param1->unk_00[v0].unk_0C = (v0 * 90) << FX32_SHIFT;
        param1->unk_00[v0].unk_14 = 1 + v0;
    }

    param1->unk_00[0].unk_10 = 2;
    param1->unk_F1 = 1;
}
