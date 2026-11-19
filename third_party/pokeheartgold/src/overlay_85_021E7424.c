#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E7424(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E749C(UnkStruct_ov109_021D0F70 *param0);
enum TouchPadExternalBufferWriteMethod {
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING = 1,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_NO_WRITE,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITH_WRAPPING,
};
u32 sub_02021280(TouchPadDataBuffer *outBuffer, enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 changeThreshold);
void ov85_021E74AC(UnkStruct_ov109_021D0F70 *param0);
BOOL ov85_021E74F0(UnkStruct_ov109_021D294C *param0);
BOOL ov85_021E750C(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E7424(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D28C4 *v0 = &param0->unk_D0C;

    v0->camera = Camera_New(HEAP_ID_102);

    {
        VecFx32 v1;

        v0->unk_08.x = (FX32_ONE * 0);
        v0->unk_08.y = (FX32_ONE * 0);
        v0->unk_08.z = (FX32_ONE * 0);

        v0->cameraAngle.x = ((-32 * 0xffff) / 360);
        v0->cameraAngle.y = ((0 * 0xffff) / 360);
        v0->cameraAngle.z = ((0 * 0xffff) / 360);

        v0->unk_00 = (0x143 << FX32_SHIFT);
        v0->unk_04 = ((6 * 0xffff) / 360);

        Camera_Init_FromTargetDistanceAndAngle(&v0->unk_08, v0->unk_00, &v0->cameraAngle, v0->unk_04, 0, 1, v0->camera);

        v1.x = 0;
        v1.y = FX32_ONE;
        v1.z = 0;

        Camera_SetLookAtCamUp(&v1, v0->camera);
        Camera_SetStaticPtr(v0->camera);
    }
}

void ov85_021E749C(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D28C4 *v0 = &param0->unk_D0C;
    Camera_Delete(v0->camera);
}

void ov85_021E74AC(UnkStruct_ov109_021D0F70 *param0) {
    int v0 = 0, v1 = 4 - 1;
    UnkStruct_ov109_021D294C *v2 = &param0->unk_D2C;

    sub_02021280(&v2->unk_10, TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_NO_WRITE, 0);

    while (v1 >= 0) {
        if (v2->unk_10.buffer[v1].touch == 1) {
            v2->unk_00 = v2->unk_10.buffer[v1].x;
            v2->unk_04 = v2->unk_10.buffer[v1].y;
            v0 = 1;
        }

        v1--;
    }

    v2->unk_08 = v0 & (v0 ^ v2->unk_0C);
    v2->unk_0C = v0;
}

BOOL ov85_021E74F0(UnkStruct_ov109_021D294C *param0) {
    int v0, v1, v2, v3, v4, v5;

    v4 = param0->unk_00;
    v5 = param0->unk_04;
    v0 = 128 - (128 / 2);
    v1 = 96 - (128 / 2);
    v2 = 128 + (128 / 2) - 1;
    v3 = 96 + (128 / 2) - 1;

    if (((v5 >= v1) && (v5 <= v3)) && ((v4 >= v0) && (v4 <= v2))) {
        return 1;
    }

    return 0;
}

BOOL ov85_021E750C(UnkStruct_ov109_021D0F70 *param0) {
    UnkStruct_ov109_021D294C *v0 = &param0->unk_D2C;

    if (v0->unk_08) {
        if (ov85_021E74F0(v0) == 1) {
            return 1;
        }
    }

    return 0;
}
