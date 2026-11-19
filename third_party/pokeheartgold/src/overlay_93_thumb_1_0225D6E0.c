#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "library/spl_emitter.h"
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

void sub_02014EBC(ParticleSystem *particleSystem);

void *sub_020154D0(ParticleSystem *particleSystem);

void ov93_0225D6E0(UnkStruct_ov117_02261280_t *param0);

typedef void (*SPLEmitterCallback)(SPLEmitter *emtr);
SPLEmitter *sub_02015494(ParticleSystem *particleSystem, int resourceID, SPLEmitterCallback callback, void *param);
void ov93_0225D700(UnkStruct_ov117_02261280_t *param0, int param1);

void ov93_0225D6E0(UnkStruct_ov117_02261280_t *param0) {
    void *v0 = sub_020154D0(param0->unk_A4);

    sub_02014EBC(param0->unk_A4);
    Heap_Free(v0);
}

void ov93_0225D700(UnkStruct_ov117_02261280_t *param0, int param1) {
    switch (param1) {
    case 0:
        sub_02015494(param0->unk_A4, 0, NULL, param0);
        break;
    case 1:
        sub_02015494(param0->unk_A4, 1, NULL, param0);
        break;
    case 2:
        sub_02015494(param0->unk_A4, 2, NULL, param0);
        break;
    default:
        GF_ASSERT(FALSE);
        return;
    }
}
