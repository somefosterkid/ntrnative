#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "obj_char_transfer.h"
#include "overlay_93_thumb_2.h"
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
void ov93_0225E144(UnkStruct_ov117_02261280_t *param0, int param1, const UnkStruct_ov117_022621D4 *param2);
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
BOOL ov93_02262724(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022621D4 *param1);

int ov93_02262710(void) {
    return sizeof(UnkStruct_ov117_022621D4);
}

int ov93_02262714(void) {
    return sizeof(u32);
}

void ov93_02262718(int param0, int param1, void *param2, void *param3) {
    UnkStruct_ov117_02261280_t *v0 = param3;
    ov93_0225E144(v0, param0, param2);
}

BOOL ov93_02262724(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022621D4 *param1) {
    if (sub_02037030(24, param1, sizeof(UnkStruct_ov117_022621D4)) == 1) {
        return 1;
    }

    return 0;
}
