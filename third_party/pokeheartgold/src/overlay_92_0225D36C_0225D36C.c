#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "heap.h"
#include "palette.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02034354.h"
#include "unk_020379A0.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "unk_02037C94.h"
#include "overlay_91_unk_struct_ov115_02260440.h"

struct OverlayManager;

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

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);

void OverlayManager_FreeData(OverlayManager *appMan);


int sub_02037454(void);

u32 ov92_0225D8E4(UnkStruct_ov116_0226139C *param0);

int ov92_0225D36C(OverlayManager *appMan, int *param1);

int ov92_0225D36C(OverlayManager *appMan, int *param1) {
    switch (*param1) {
    case 0: {
        u32 v0;
        UnkStruct_ov116_0226139C *v1 = OverlayManager_GetData(appMan);

        v0 = ov92_0225D8E4(v1);

        OverlayManager_FreeData(appMan);
        Heap_Destroy(HEAP_ID_113);
        sub_020398D4(0, 1);

        if (v0 != 0) {
            return 1;
        }

        sub_02037AC0(999);
        (*param1)++;
    } break;
    default:
        if ((sub_02037B38(999) == 1) || (sub_02037454() < sub_020347A0())) {
            return 1;
        }
        break;
    }

    return 0;
}
