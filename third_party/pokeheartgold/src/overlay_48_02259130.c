#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct UnkStruct_ov66_0222DFF8_t;
struct WiFiHistory;
struct YesNoPrompt;
typedef struct WiFiHistory WiFiHistory;
u16 ov45_0222D6B0(const UnkStruct_ov66_02231560 *param0, u8 param1);
u8 ov45_0222D6D4(const UnkStruct_ov66_02231560 *param0, u8 param1);
typedef struct {
    const UnkStruct_ov66_02231560 *unk_00;
    SaveData *saveData;
    UnkStruct_ov66_02231428 unk_08;
    WiFiPlazaSystem *unk_0C;
} UnkStruct_ov66_02230F50;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    MtxFx33 unk_04;
    u16 unk_28;
    u16 unk_2A;
    u16 unk_2C;
} UnkStruct_ov69_0225CA7C_sub1_sub1;
typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225CA7C_sub1_sub1 unk_04[1024];
} UnkStruct_ov69_0225CA7C_sub1;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;
typedef struct {
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;
typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Field3dObject unk_24;
    Field3dModel unk_9C;
} UnkStruct_ov69_0225D71C;
typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Field3dObject unk_B810[3];
    Field3dModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;
typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;
typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;
typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;
typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;
typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    Window unk_1C;
} UnkStruct_ov69_0225DC48;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov69_0225DA74 unk_04;
    WiFiHistory *wiFiHistory;
    Options *options;
    int unk_14;
    int unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    UnkStruct_ov69_0225D35C unk_20;
    UnkStruct_ov69_0225EF54 unk_1CC;
    UnkStruct_ov69_0225D71C unk_1DC;
    UnkStruct_ov69_0225CA7C unk_288;
    UnkStruct_ov69_0225DAEC unk_BC30;
    UnkStruct_ov69_0225DC48 unk_BC44;
    UnkStruct_ov69_0225E084 unk_BC70;
    UnkStruct_ov69_0225DDC8 unk_BF64;
} UnkStruct_ov69_0225CE64;
BOOL ov48_022593B4(UnkStruct_ov69_0225CE64 *param0, u8 param1, u8 param2);
u32 ov48_0225B13C(const UnkStruct_ov66_02231560 *param0);
u32 ov48_022591D8(const UnkStruct_ov69_0225CE64 *param0);
u32 ov48_0225932C(const UnkStruct_ov69_0225CE64 *param0, u32 param1, UnkStruct_ov66_02231428 param2);
u32 ov48_02259B84(const UnkStruct_ov69_0225CA7C *param0, int param1);
u32 ov48_02259BA0(const UnkStruct_ov69_0225CA7C *param0, int param1);
u32 ov48_02259BBC(const UnkStruct_ov69_0225CA7C *param0);

void ov48_02259130(UnkStruct_ov69_0225CE64 *param0, const UnkStruct_ov66_02230F50 *param1);
BOOL ov48_02259188(const UnkStruct_ov69_0225CE64 *param0, UnkStruct_ov69_0225E0C0 *param1);

void ov48_02259130(UnkStruct_ov69_0225CE64 *param0, const UnkStruct_ov66_02230F50 *param1) {
    int v0;
    int v1;
    int v2;
    BOOL v3 = ov48_022593B4(param0, param0->unk_14, param0->unk_18);

    if (v3 == 0) {
        v0 = ov48_0225B13C(param1->unk_00);

        if (v0 != 50) {
            v1 = ov45_0222D6B0(param1->unk_00, v0);
            v2 = ov45_0222D6D4(param1->unk_00, v0);
            v3 = ov48_022593B4(param0, v1, v2);

            ((v3 == 1) ? (void)0 : GF_AssertFail());
        } else {
            (void)0;
        }
    }
}

BOOL ov48_02259188(const UnkStruct_ov69_0225CE64 *param0, UnkStruct_ov69_0225E0C0 *param1) {
    u32 v0 = ov48_022591D8(param0);

    if (v0 >= ov48_02259BBC(&param0->unk_288)) {
        return 0;
    }

    param1->unk_00 = ov48_0225932C(param0, v0, param0->unk_1C);
    param1->unk_04 = ov48_02259B84(&param0->unk_288, v0);
    param1->unk_08 = ov48_02259BA0(&param0->unk_288, v0);

    return 1;
}
