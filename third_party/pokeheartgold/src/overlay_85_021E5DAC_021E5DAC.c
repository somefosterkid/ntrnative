#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"
#include "unk_struct_0209_bdf8.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "sprite_system.h"
#include "overlay_85_ov109_021D5140.h"
#include "camera.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct BgConfig;

struct Camera;

struct CameraAngle;

struct GameStats;

struct JournalEntry;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct Simple3DAnimation;

struct Simple3DModel;

struct Simple3DRenderObj;

struct Simple3DRotationAngles;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TouchPadDataBuffer;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

struct Window;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct JournalEntry JournalEntry;

PlayerProfile *sub_02034818(int netId);

enum SoundScene {
    SOUND_SCENE_NONE = 0,
    SOUND_SCENE_TITLE_SCREEN,
    SOUND_SCENE_2,
    SOUND_SCENE_3,
    SOUND_SCENE_FIELD,
    SOUND_SCENE_BATTLE,
    SOUND_SCENE_CONTEST,
    SOUND_SCENE_7,
    SOUND_SCENE_8,
    SOUND_SCENE_9,
    SOUND_SCENE_10,
    SOUND_SCENE_11,
    SOUND_SCENE_12,
    SOUND_SCENE_13,
    SOUND_SCENE_14,
    SOUND_SCENE_15,
    SOUND_SCENE_16,
    SOUND_SCENE_17,
    SOUND_SCENE_18,
    SOUND_SCENE_19,
    SOUND_SCENE_20,
    SOUND_SCENE_21,
    SOUND_SCENE_VILLA_MUSIC_BOX,
    SOUND_SCENE_23,
    SOUND_SCENE_24,
    SOUND_SCENE_25,
    SOUND_SCENE_26,
    SOUND_SCENE_27,
    SOUND_SCENE_28,
    SOUND_SCENE_29,
    SOUND_SCENE_30,
    SOUND_SCENE_31,
    SOUND_SCENE_32,
    SOUND_SCENE_33,
    SOUND_SCENE_34,
    SOUND_SCENE_35,
    SOUND_SCENE_36,
    SOUND_SCENE_37,
    SOUND_SCENE_38,
    SOUND_SCENE_39,
    SOUND_SCENE_40,
    SOUND_SCENE_41,
    SOUND_SCENE_42,
    SOUND_SCENE_43,
    SOUND_SCENE_44,
    SOUND_SCENE_45,
    SOUND_SCENE_46,
    SOUND_SCENE_47,
    SOUND_SCENE_48,
    SOUND_SCENE_49,
    SOUND_SCENE_50,
    SOUND_SCENE_MAX,

    SOUND_SCENE_SUB_BAG = SOUND_SCENE_MAX,
    SOUND_SCENE_SUB_52,
    SOUND_SCENE_SUB_53,
    SOUND_SCENE_SUB_54,
    SOUND_SCENE_SUB_55,
    SOUND_SCENE_SUB_56,
    SOUND_SCENE_SUB_57,
    SOUND_SCENE_SUB_58,
    SOUND_SCENE_SUB_59,
    SOUND_SCENE_SUB_60,
    SOUND_SCENE_SUB_61,
    SOUND_SCENE_SUB_62,
    SOUND_SCENE_SUB_63,
    SOUND_SCENE_SUB_64,
    SOUND_SCENE_SUB_65,
    SOUND_SCENE_SUB_66,
    SOUND_SCENE_SUB_67,
    SOUND_SCENE_SUB_68,
};

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 bgmID, int unused);

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov109_021D122C;

void ov85_021E72E8(UnkStruct_ov109_021D0F70 *param0);

void *ov85_021E85F0(UnkStruct_ov109_021D0F70 *param0, u32 param1);

BOOL ov85_021E86B0(UnkStruct_ov109_021D0F70 *param0, int param1);

extern const u16 ov85_021EA788[5 + 1][5];

int ov85_021E5DAC(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E5DAC(UnkStruct_ov109_021D0F70 *param0) {
    int v0;
    UnkStruct_ov109_021D122C *v1 = ov85_021E85F0(param0, sizeof(UnkStruct_ov109_021D122C));

    v1->unk_10 = param0->unk_2C.unk_00;
    v1->unk_14 = param0->unk_2C.unk_04;
    v1->unk_04 = ov85_021EA788[v1->unk_14][param0->unk_2C.unk_00];

    for (v0 = 0; v0 < (7 + 1); v0++) {
        if (ov85_021E86B0(param0, v0)) {
            param0->unk_2C.unk_58[v0] = sub_02034818(v0);
            PlayerName_FlatToString(
                param0->unk_2C.unk_58[v0], param0->unk_2C.unk_6C[v0]);
        }
    }

    param0->unk_D4.unk_40 = (((fx32)0x0000000000001000L) * (v1->unk_04));
    param0->unk_00 = 17;

    Sound_SetSceneAndPlayBGM(SOUND_SCENE_15, 1155, 1);
    ov85_021E72E8(param0);

    return 1;
}
