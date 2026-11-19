#include "global.h"

#include "overlay_manager.h"
#include "filesystem_files_def.h"
#include "msgdata.h"
#include "bg_window.h"
#include "unk_02095EAC.h"
#include "overlay_41_sprite_resource_collection.h"
#include "unk_02037C94.h"

struct BgConfig;

struct G2dRenderer;

struct JournalEntry;

struct MsgData;

struct NARC;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_02095EAC_t;

struct Window;

struct YesNoPrompt;

enum HeapSize {
    HEAP_SIZE_SYSTEM = 0xD200,
    HEAP_SIZE_SAVE = 0x20E00,
    HEAP_SIZE_DEBUG = 0x10,
    HEAP_SIZE_APPLICATION = 0x10D800,

    HEAP_SIZE_BATTLE_HALL_APP = 0x20000,
    HEAP_SIZE_BATTLE_FACTORY_APP = 0x20000,
    HEAP_SIZE_BATTLE_ARCADE_APP = 0x20000,
    HEAP_SIZE_FRONTIER_RECORDS_APP = 0x20000,
    HEAP_SIZE_BAG = 0x30000,

    HEAP_SIZE_PRE_POKETCH_SUBSCREEN = 0x18000,
    HEAP_SIZE_POKETCH_MAIN = 0xC000,
    HEAP_SIZE_POKETCH_APP = 0xC000,
    HEAP_SIZE_BOX_DATA = 0x4000,
    HEAP_SIZE_BOX_GRAPHICS = 0x3C000,

    HEAP_SIZE_FIELD1 = 0x20000,
    HEAP_SIZE_FIELD2 = 0x1C000,
    HEAP_SIZE_FIELD3 = 0x4000,

    HEAP_SIZE_TOWN_MAP = 0x20000,

    HEAP_SIZE_EGG_HATCH = 0x40000,
    HEAP_SIZE_CHOOSE_STARTER_APP = 0x40000,
    HEAP_SIZE_LIBRARY_TV = 0x40000,
    HEAP_SIZE_TRAINER_CASE = 0x28000,
    HEAP_SIZE_MINING = 0x50000,
    HEAP_SIZE_DISTORTION_WORLD_WARP = 0x50000,
    HEAP_SIZE_UNDERGROUND = 0xE800,
    HEAP_SIZE_DRAWING = 0x40000,
    HEAP_SIZE_MAIL = 0x1000,
    HEAP_SIZE_MAIL_VIEWER = 0x20000,
    HEAP_SIZE_BOAT_CUTSCENE = 0x20000,
    HEAP_SIZE_EVOLUTION = 0x30000,
    HEAP_SIZE_GAME_START = 0x20000,
    HEAP_SIZE_RECORD_LIST = 0x6000,
    HEAP_SIZE_MAIN_MENU = 0x40000,
    HEAP_SIZE_MIGRATE_FROM_GBA = 0x38000,
    HEAP_SIZE_RANGER_LINK = 0x20000,
    HEAP_SIZE_MYSTERY_GIFT_APP = 0x30000,
    HEAP_SIZE_WONDER_CARDS_APP = 0x30000,
    HEAP_SIZE_NETWORK_ICON = 0x300,
};

typedef struct JournalEntry JournalEntry;

BgConfig *BgConfig_Alloc(enum HeapID heapID);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

struct UnkStruct_02095EAC_t {
    BgConfig *unk_00;
    BOOL unk_04;
    UnkStruct_0203DDFC *unk_08;
    MessageFormat *unk_0C;
    MsgData *unk_10;
    String *unk_14[5];
    String *unk_28;
    String *unk_2C;
    int unk_30;
    SpriteList *unk_34;
    G2dRenderer unk_38;
    SpriteResourceCollection *unk_1C4[4];
    SpriteResource *unk_1D4[2][4];
    SpriteResourcesHeader unk_1F4;
    SpriteResourcesHeader unk_218;
    Sprite *unk_23C[14];
    Sprite *unk_274[14];
    Sprite *unk_2AC[12];
    Window unk_2DC[5];
    Window unk_32C;
    Window unk_33C;
    Window unk_34C;
    Window *unk_35C[2];
    int unk_364;
    int unk_368;
    int unk_36C;
    int unk_370;
    int unk_374;
    int unk_378;
    int unk_37C;
    int unk_380;
    int unk_384;
    u8 unk_388[8][2];
    PlayerProfile *unk_398[8][2];
    u8 unk_3D8[16384];
    u16 unk_43D8;
    u8 unk_43DA;
    u8 unk_43DB;
    UnkStruct_ov58_021D2820 unk_43DC;
    UnkStruct_ov58_021D2820 unk_43E6[5];
    UnkStruct_ov58_021D2754 unk_4418[5];
    u8 *unk_442C;
    int unk_4430;
    u8 unk_4434[14400];
    UnkStruct_02095EAC_sub1 unk_7C74;
    UnkStruct_02095EAC_sub1 unk_8064[5];
    s32 unk_9414;
    u32 unk_9418;
    u16 unk_941C;
    s16 unk_941E;
    u8 unk_9420;
    UnkStruct_ov58_021D2820 unk_9421[5];
    YesNoPrompt *unk_9454;
    int unk_9458;
    int unk_945C;
    int unk_9460;
    int unk_9464;
    int unk_9468;
    int unk_946C;
};

void sub_02038C1C(s8 maxNumConnection);


u16 sub_0203769C(void);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_DisableEngineBPlanes(void);

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

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

void sub_0200FBF4(enum GFScreen screen, u16 color);

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

void EnableTouchPad(void);

BOOL InitializeTouchPad(u32 autoSamplingFrequency);

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

void sub_020356EC(BOOL param0);

void sub_0208F814(void *param0);

void ov37_021E5CC8(void *param0);

void ov37_021E5CF0(void);

void ov37_021E5D10(BgConfig *param0);

void ov37_021E5E30(UnkStruct_02095EAC *param0);

void ov37_021E5F98(UnkStruct_02095EAC *param0, NARC *param1);

void ov37_021E6090(void);

void ov37_021E60C0(UnkStruct_02095EAC *param0, NARC *param1);

void ov37_021E6244(UnkStruct_02095EAC *param0);

void ov37_021E6418(UnkStruct_02095EAC *param0, OverlayManager *appMan);

int ov37_021E5900(OverlayManager *appMan, BOOL *param1);

int ov37_021E5900(OverlayManager *appMan, BOOL *param1) {
    UnkStruct_02095EAC *v0;
    NARC *v1;

    switch (*param1) {
    case 0:
        Main_SetVBlankIntrCB(0L, 0L);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();

        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);

        Heap_Create(HEAP_ID_3, HEAP_ID_39, HEAP_SIZE_DRAWING);

        v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_02095EAC), HEAP_ID_39);
        memset(v0, 0, sizeof(UnkStruct_02095EAC));
        v0->unk_00 = BgConfig_Alloc(HEAP_ID_39);

        v0->unk_0C = MessageFormat_New(HEAP_ID_39);
        v0->unk_10 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (enum NarcId)27, 252, HEAP_ID_39);

        SetKeyRepeatTimers(4, 8);

        ov37_021E5CF0();
        ov37_021E5D10(v0->unk_00);

        sub_0200FBF4(SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
        sub_0200FBF4(SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CIRCLE_IN, FADE_TYPE_CIRCLE_IN, (((0) << 10) | ((0) << 5) | (0)), 16, 1, HEAP_ID_39);

        {
            UnkStruct_0203DDFC *v2 = (UnkStruct_0203DDFC *)OverlayManager_GetArgs(appMan);
            v0->unk_08 = v2;
        }

        v1 = NARC_New(NARC_a_0_7_8, HEAP_ID_39);

        ov37_021E5F98(v0, v1);

        EnableTouchPad();
        InitializeTouchPad(2);
        Main_SetVBlankIntrCB(ov37_021E5CC8, v0->unk_00);

        ov37_021E5E30(v0);
        ov37_021E6090();
        ov37_021E60C0(v0, v1);
        ov37_021E6244(v0);
        ov37_021E6418(v0, appMan);

        Sound_SetSceneAndPlayBGM(SOUND_SCENE_SUB_52, 0, 0);

        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

        sub_0208F814(v0);
        sub_020398D4(0, 1);

        if (sub_0203769C() == 0) {
            sub_02038C1C(3);
        }

        sub_0203A880();

        if (sub_0203769C() == 0) {
            sub_0205A904(UR_STATUS_DRAWING);
            sub_020356EC(1);
        }

        NARC_Delete(v1);
        (*param1)++;
        break;
    case 1:
        v0 = OverlayManager_GetData(appMan);
        (*param1) = 0;
        return 1;
        break;
    }

    return 0;
}
