#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_85_ov109_021D5140.h"
#include "overlay_manager.h"
#include "save.h"
#include "options.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "message_format.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"
#include "filesystem_files_def.h"
#include "msgdata.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_02037C94.h"

struct BgConfig;

struct JournalEntry;

struct ListMenu2D;

struct NARC;

struct SysTask;

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

void NARC_Delete(NARC *narc);

typedef struct JournalEntry JournalEntry;

BgConfig *BgConfig_Alloc(enum HeapID heapID);

typedef struct ListMenu2D ListMenu2D;

void sub_0203897C(void);


u16 sub_0203769C(void);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_DisableEngineBPlanes(void);

void sub_0203A880(void);

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

typedef void (*Callback)(void *);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void SetKeyRepeatTimers(int rate, int delay);

enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

SysTask * SysTask_CreateOnVBlankQueue(SysTaskFunc func, void *data, u32 priority);

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

enum UnionRoomStatus {
    UR_STATUS_NONE = 0,
    UR_STATUS_DRAWING,
    UR_STATUS_MIXING_RECORDS,
    UR_STATUS_UNK_03,
    UR_STATUS_BUSY,
    UR_STATUS_SHOWING_TRAINER_CASE,
    UR_STATUS_BATTLING,
    UR_STATUS_TRADING,
    UR_STATUS_UNK_08,
    UR_STATUS_UNK_09,
    UR_STATUS_UNK_10,
    UR_STATUS_UNK_11,
    UR_STATUS_UNK_12,
    UR_STATUS_SPIN_TRADING,
};

void sub_0205A904(enum UnionRoomStatus status);

void sub_02096CE0(UnkStruct_0209BDF8 *param0);

void ov85_021E8C14(void *param0);

void ov85_021E8C3C(void);

void ov85_021E8C5C(BgConfig *param0);

void ov85_021E8D64(UnkStruct_ov109_021D5140 *param0, NARC *param1);

void ov85_021E8E6C(UnkStruct_ov109_021D5140 *param0, NARC *param1);

void ov85_021E8F58(void);

void ov85_021E8F88(UnkStruct_ov109_021D5140 *param0, NARC *param1);

void ov85_021E9084(UnkStruct_ov109_021D5140 *param0);

void ov85_021E9160(UnkStruct_ov109_021D5140 *param0);

void ov85_021E8BB0(SysTask *param0, void *param1);

int ov85_021E88AC(OverlayManager *appMan, BOOL *param1);

int ov85_021E88AC(OverlayManager *appMan, BOOL *param1) {
    UnkStruct_ov109_021D5140 *v0;
    NARC *v1;
    UnkStruct_0209C194 *v2 = OverlayManager_GetArgs(appMan);

    switch (*param1) {
    case 0:
        Main_SetVBlankIntrCB(0L, 0L);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();

        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);

        Heap_Create(HEAP_ID_3, HEAP_ID_102, 0x80000);

        v1 = NARC_New((NarcId)217, HEAP_ID_102);
        ((v1) ? (void)0 : GF_AssertFail());

        v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_ov109_021D5140), HEAP_ID_102);
        memset(v0, 0, sizeof(UnkStruct_ov109_021D5140));

        v2->unk_38 = v0;
        v0->unk_0C = v2;
        v0->unk_10 = v2->unk_30;
        v0->unk_14 = BgConfig_Alloc(HEAP_ID_102);
        v0->unk_34 = MessageFormat_New(HEAP_ID_102);
        v0->unk_38 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (enum NarcId)27, 207, HEAP_ID_102);

        SetKeyRepeatTimers(4, 8);
        ov85_021E8C3C();
        ov85_021E8C5C(v0->unk_14);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CIRCLE_IN, FADE_TYPE_CIRCLE_IN, (((0) << 10) | ((0) << 5) | (0)), 16, 1, HEAP_ID_102);
        ov85_021E8E6C(v0, v1);
        Main_SetVBlankIntrCB(ov85_021E8C14, v0);
        ov85_021E8D64(v0, v1);
        ov85_021E8F58();
        ov85_021E8F88(v0, v1);
        ov85_021E9084(v0);
        ov85_021E9160(v0);
        Sound_SetSceneAndPlayBGM(SOUND_SCENE_SUB_52, 0, 0);
        sub_02096CE0(v2->unk_30);
        sub_0203897C();
        sub_02038C1C(3);

        if (sub_0203769C() == 0) {
            sub_0205A904(UR_STATUS_SPIN_TRADING);
        }

        sub_0203A880();
        v0->unk_30 = SysTask_CreateOnVBlankQueue(ov85_021E8BB0, v0, 5);
        NARC_Delete(v1);
        (*param1)++;
        break;
    case 1:
        (*param1) = 0;
        return 1;
    }

    return 0;
}
