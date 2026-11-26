#include "global.h"

#include "screen_fade.h"
#include "field/overlay_01_021FB878.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "overlay_91_unk_struct_ov115_02260440.h"

s32 ov92_0225D868(enum FadeType arg0);

struct G3DPipelineBuffers;
struct PlayerProfile;
struct UnkStruct_ov114_02025CAD4_t;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
typedef struct {
    u8 unk_00;
    u8 padding_01[3];
    u32 unk_04;
} UnkStruct_ov116_0226048C;
typedef struct {
    u32 unk_00;
    u8 padding_04[4];
} UnkStruct_ov116_02260494;
typedef struct {
    UnkStruct_ov116_02260494 unk_00[4];
} UnkStruct_ov116_02260498;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void ov92_0225D88C(UnkStruct_ov116_0226139C *param0, BOOL param1, int param2, int *param3);

s32 ov92_0225D868(enum FadeType arg0) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, arg0, arg0, 0U, 6, 1, HEAP_ID_113);
    return 1;
}

void ov92_0225D88C(UnkStruct_ov116_0226139C *param0, BOOL param1, int param2, int *param3) {
    if (param1) {
        *param3 = param2;
    }
}
