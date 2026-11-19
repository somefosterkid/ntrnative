#include "global.h"

#include "constants/game_stats.h"
#include "constants/heap.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/dynamic_terrain_height.h"
#include "field/fog.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/overlay_01_02204004.h"
#include "field/signpost.h"

#include "bag.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "brightness.h"
#include "bug_contest_internal.h"
#include "camera.h"
#include "dex_mon_measures.h"
#include "field_bgm.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats.h"
#include "gear_phone.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "item.h"
#include "launch_application_internal.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "map_events_internal.h"
#include "map_matrix.h"
#include "map_object.h"
#include "math_util.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_01_021F1348.h"
#include "overlay_manager.h"
#include "party.h"
#include "photo_types_def.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "sav_chatot.h"
#include "save.h"
#include "save_link_ruleset.h"
#include "save_palpad.h"
#include "save_pokegear.h"
#include "save_special_ribbons.h"
#include "save_vars_flags.h"
#include "scrcmd_9.h"
#include "screen_fade.h"
#include "script.h"
#include "sound_chatot.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "sys_task.h"
#include "sys_vars.h"
#include "system.h"
#include "task.h"
#include "terrain_attributes.h"
#include "text.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "trainer_memo.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "unk_02088288.h"
#include "update_dex_received.h"
#include "vram_transfer_manager.h"
#include "wild_encounter.h"
#include "yes_no_prompt.h"
#include "constants/player_avatar.h"

struct AreaDataManager;

struct AreaLightManager;

struct AreaMapAnimation;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldCameraShift;

struct FieldEffect;

struct FieldEffectManager;

struct FieldMapSystems;

struct FieldProcessManager;

struct FieldSystem;

struct FieldTerrainQueries;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FollowMonInteraction;

struct FollowMonMood;

struct FriendRoster;

struct FrontierFieldSystem;

struct G2dRenderer;

struct GF_2DGfxResMan;

struct GearPhoneRingManager;

struct LinkBattleRuleset;

struct ListMenu;

struct ListMenuItem;

struct LocalMapObject;

struct Location;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimationManager;

struct MapPropAnimationPoolList;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct MessageFormat;

struct ModelAttributes;

struct MsgData;

struct NARC;

struct Options;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Party;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokedexResumeState;

struct PokegearRingingTask;

struct PokemonSummaryArgs;

struct PokepicTemplate;

struct SaveData;

struct SavePalPad;

struct SavePokegear;

struct SaveSpecialRibbons;

struct Signpost;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourcesHeader;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerBgGoTo;

struct TerrainAttributes;

struct TimeOfDayPropAnimManager;

struct TouchscreenListMenu;

struct TouchscreenListMenuHeader;

struct TouchscreenListMenuSpawner;

struct TouchscreenListMenuTemplate;

struct UnionRoom;

struct UnionRoomObjects;

struct WaitingIcon;

struct Window;

struct WirelessTradeChatotData;

struct WirelessTradeMonInfo;

struct WirelessTradeSelectMon;

struct WirelessTradeSelectMonArgs;

struct WirelessTradeSlide;

struct YesNoPrompt;

typedef struct FriendRoster FriendRoster;

typedef struct AreaMapAnimation AreaMapAnimation;

typedef struct MapPropAnimationPoolList MapPropAnimationPoolList;

typedef struct TimeOfDayPropAnimManager TimeOfDayPropAnimManager;

typedef void (*LocalMapObject_Callback)(LocalMapObject *object);

typedef struct TaskManagerBgGoTo TaskManagerBgGoTo;

typedef struct FieldEffect FieldEffect;

typedef struct PokedexResumeState PokedexResumeState;

typedef struct FieldMapSystems FieldMapSystems;

typedef struct FollowMonMood FollowMonMood;

typedef struct FollowMonInteraction FollowMonInteraction;

typedef struct WirelessTradeMonInfo {
    u16 species;
    u16 heldItem;
    u8 ball;
    u8 isEgg;
    u8 form;
    u8 padding_7;
    u16 gender;
    u16 flip;
    u32 capsule;
} WirelessTradeMonInfo;

typedef struct WirelessTradeSlide {
    fx32 startX;
    fx32 startY;
    u8 padding_08[4];
    fx32 endX;
    fx32 endY;
    u8 padding_14[4];
} WirelessTradeSlide;

typedef struct WirelessTradeChatotData {
    u32 data[0xFB];
} WirelessTradeChatotData;

typedef struct WirelessTradeSelectMon WirelessTradeSelectMon;

typedef int (*WirelessTradeSelectMonFunc)(WirelessTradeSelectMon *work);

struct WirelessTradeSelectMon {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    WirelessTradeSelectMonArgs *args;
    PokemonSummaryArgs summaryArgs;
    u32 summaryOfPartner;
    OverlayManager *summaryApp;
    BOOL summaryActive;
    u32 state;
    int exchangeState;
    int sentChunks;
    int commState;
    int receivedChunks;
    u32 tradeAccepted;
    u32 answers[2];
    u32 resultMsg;
    u8 padding_78[0x1C];
    int cursor;
    int partnerCursor;
    u16 cursorBlinkAngle;
    u8 padding_9E[0xAE];
    u32 selectResult;
    u8 padding_150[8];
    int dpadDir;
    int partnerDpadDir;
    u8 padding_160[0x18];
    u32 unk_178;
    u32 unk_17C;
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MessageFormat *nicknameFormat;
    MessageFormat *unk_18C;
    MsgData *msgData;
    u8 padding_194[4];
    String *playerName;
    String *itemLabel;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObjs[2][4];
    SpriteResourcesHeader resHeaders[2];
    Sprite *cursorSprites[2];
    u8 padding_34C[0x30];
    Sprite *iconSprites[12];
    Sprite *itemSprites[12];
    Sprite *capsuleSprites[12];
    Sprite *detailSprites[6];
    Sprite *tradeSprite;
    u8 padding_428[0x1C];
    Window windows[34];
    u8 unk_664[4];
    ListMenuItem *menuItems;
    u8 padding_66C[4];
    u32 yesNoState;
    u32 unk_674;
    u8 padding_678[0x24];
    WirelessTradeMonInfo mons[12 + 1];
    u8 padding_76C[0x30];
    void *iconCharBuffers[12];
    NNSG2dCharacterData *iconCharData[12];
    u8 monPics[2][0xC80];
    PokepicTemplate monPicTemplates[2];
    int pendingMonPic;
    u8 padding_2120[0x100];
    WirelessTradeSelectMonFunc callback;
    Party *party;
    Party *partnerParty;
    u32 unk_222C;
    SavePalPad *palPad;
    SavePalPad palPadPacket;
    u8 padding_22BC[8];
    int bgScrollY;
    u32 slideStep;
    int startDelay;
    WirelessTradeSlide slides[2];
    u8 partyChunks[2][0x590];
    WirelessTradeChatotData chatot[2];
    u32 friendStates[32];
    int friendNetId;
    int chosenRosterSlot;
    MessageFormat *rosterMsgFormat;
    MsgData *rosterMsgData;
    ListMenuItem *rosterItems;
    ListMenu *rosterMenu;
    Window rosterWindow;
    FriendRoster *roster;
    u32 tradesDone;
    s32 saveState;
    int lastSentValue;
    int lastSentCommand;
    WaitingIcon *waitingIcon;
    BOOL timeoutActive;
    int timeoutCounter;
    YesNoPrompt *yesNoPrompt;
    TouchscreenListMenuSpawner *touchMenuSpawner;
    TouchscreenListMenu *touchMenu;
};

void ov65_0221BFEC(WirelessTradeSelectMon *work);

void ov65_0221C08C(WirelessTradeSelectMon *work);

BOOL ov65_0221C5E0(WirelessTradeSelectMon *work);

void ov65_0221CB5C(WirelessTradeSelectMon *work);

int ov65_0221CC0C(WirelessTradeSelectMon *work);

void ov65_0221CCB0(WirelessTradeSelectMon *work);

void ov65_0221CD0C(WirelessTradeSelectMon *work);

void ov65_0221D1C8(BgConfig *bgConfig);

void ov65_0221DD34(int cursor, Sprite *sprite, int side);

void ov65_0221E06C(WirelessTradeSelectMon *work, u32 ofPartner);

void ov65_0221F714(WirelessTradeSelectMon *work);

enum WirelessTradeSelectState {
    WIRELESS_TRADE_SELECT_STATE_EXCHANGE_DATA,
    WIRELESS_TRADE_SELECT_STATE_SELECT,
    WIRELESS_TRADE_SELECT_STATE_EXIT,
    WIRELESS_TRADE_SELECT_STATE_START_SUMMARY,
    WIRELESS_TRADE_SELECT_STATE_OPEN_SUMMARY,
    WIRELESS_TRADE_SELECT_STATE_RUN_SUMMARY,
    WIRELESS_TRADE_SELECT_STATE_START_FADE_IN,
    WIRELESS_TRADE_SELECT_STATE_WAIT_FADE_IN,
};

void WirelessTradeSelectMon_LoadScreenData(NARC *narc, int memberNo, BgConfig *bgConfig, GFBgLayer layer, int unused, u32 size, BOOL isCompressed, enum HeapID heapID);

BOOL WirelessTradeSelectMon_Main(OverlayManager *man, int *state);

BOOL WirelessTradeSelectMon_Main(OverlayManager *man, int *state) {
    WirelessTradeSelectMon *work = OverlayManager_GetData(man);
    BOOL ret = FALSE;

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
            ov65_0221BFEC(work);
        }
        break;
    case 1:
        switch (work->state) {
        case WIRELESS_TRADE_SELECT_STATE_EXCHANGE_DATA:
            work->state = ov65_0221C5E0(work);
            break;
        case WIRELESS_TRADE_SELECT_STATE_SELECT:
            work->state = ov65_0221CC0C(work);
            break;
        case WIRELESS_TRADE_SELECT_STATE_EXIT:
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 8, 1, HEAP_ID_26);
            *state = 2;
            break;
        case WIRELESS_TRADE_SELECT_STATE_START_SUMMARY:
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 8, 1, HEAP_ID_26);
            work->state = WIRELESS_TRADE_SELECT_STATE_OPEN_SUMMARY;
            break;
        case WIRELESS_TRADE_SELECT_STATE_OPEN_SUMMARY:
            if (IsPaletteFadeFinished()) {
                ov65_0221CD0C(work);
                ov65_0221CCB0(work);
                ov65_0221D1C8(work->bgConfig);
                work->summaryActive = TRUE;
                work->state = WIRELESS_TRADE_SELECT_STATE_RUN_SUMMARY;
                ov65_0221E06C(work, work->cursor / 6);
            }
            break;
        case WIRELESS_TRADE_SELECT_STATE_RUN_SUMMARY:
            if (OverlayManager_Run(work->summaryApp)) {
                OverlayManager_Delete(work->summaryApp);
                ov65_0221C08C(work);
                work->summaryActive = FALSE;
                work->cursor = work->summaryArgs.partySlot + 6 * work->summaryOfPartner;
                ov65_0221CB5C(work);
                ov65_0221DD34(work->cursor, work->cursorSprites[0], 0);
                sub_0203A880();
                work->state = WIRELESS_TRADE_SELECT_STATE_START_FADE_IN;
            }
            break;
        case WIRELESS_TRADE_SELECT_STATE_START_FADE_IN:
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 8, 1, HEAP_ID_26);
            work->state = WIRELESS_TRADE_SELECT_STATE_WAIT_FADE_IN;
            break;
        case WIRELESS_TRADE_SELECT_STATE_WAIT_FADE_IN:
            if (IsPaletteFadeFinished()) {
                work->state = WIRELESS_TRADE_SELECT_STATE_SELECT;
            }
            break;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            ret = TRUE;
        }
        break;
    }
    if (!work->summaryActive) {
        ov65_0221F714(work);
        SpriteList_RenderAndAnimateSprites(work->spriteList);
    }
    sub_020399FC(HEAP_ID_26, work->bgConfig);
    return ret;
}
