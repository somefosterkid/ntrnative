#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_00_thumb.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "save.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"
#include "vram_transfer_manager.h"
#include "wild_encounter.h"
#include "yes_no_prompt.h"
#include "frontier_data.h"
#include "sav_system_info.h"
#include "overlay_72.h"
#include "overlay_72_wfc_connect.h"
#include "scrcmd_9.h"

typedef enum Font Font;

struct AsyncWriteManager;

struct Background;

struct BattleRoomDownload;

struct BattleRoomDownloadData;

struct BattleRoomLeader;

struct BattleRoomTrainer;

struct BgConfig;

struct DwcBackupPage;

struct DwcBackupWork;

struct FrontierMon;

struct FrontierWifiUploadRecord;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResMan;

struct GraphicsModes;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct NintendoWfcButtonFlash;

struct NintendoWfcConnect;

struct NintendoWfcGlobal;

struct NintendoWfcNumberInput;

struct NintendoWfcPaletteCycle;

struct NintendoWfcYesNoMenu;

struct NintendoWifiConnectArgs;

struct Options;

struct OverlayManager;

struct OverlayManagerTemplate;

struct SaveArrayHeader;

struct SaveData;

struct SaveSlotSpec;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourcesHeader;

struct String;

struct SysInfo;

struct SysInfo_RTC;

struct SysTask;

struct SysTaskQueue;

struct TouchHitboxController;

struct TouchHitboxControllerTemplate;

struct WaitingIcon;

struct Window;

struct YesNoPrompt;

struct YesNoPromptButton;

struct YesNoPromptSubstruct00_18;

struct _MAT;

struct _MAT_ENTRY;

void *memset(void *data, int value, size_t size);

typedef struct DwcBackupPage {
    u8 unk_00[0x40];
    u8 ssid[32];
    u8 unk_60[0x60];
    u8 ip[4];
    u8 gateway[4];
    u8 dns[2][4];
    u8 subnetMaskLen;
    u8 unk_D1[0x16];
    u8 state;
    u8 unk_E8[7];
    u8 apSetUp;
    u8 wifiInfo[14];
    u16 crc;
} DwcBackupPage;

typedef struct DwcBackupWork {
    DwcBackupPage pages[4];
    u8 verifyBuffer[0x100];
    MATHCRC16Table crcTable;
} DwcBackupWork;

int init(DwcBackupWork *work);

int Comm_InitDwc(enum HeapID heapID);

void CommWireless_StartUp(void);

typedef u8 FontID;

typedef struct BattleRoomDownload {
    u32 markDate;
    u8 roomMarks[250];
    u8 hasTrainers, hasLeaders;
    u8 trainersRoom, trainersRank, leadersRoom, leadersRank;
    BattleRoomTrainer trainers[7];
    BattleRoomLeader leaders[30];
} BattleRoomDownload;

enum NintendoWfcInitState {
    NINTENDO_WFC_INIT_SETUP,
    NINTENDO_WFC_INIT_START_WIRELESS,
};

enum NintendoWfcMainState {
    NINTENDO_WFC_MAIN_WAIT_STARTUP,
    NINTENDO_WFC_MAIN_SESSION_INIT,
    NINTENDO_WFC_MAIN_WAIT_FADE_IN,
    NINTENDO_WFC_MAIN_SESSION_MAIN,
    NINTENDO_WFC_MAIN_SESSION_EXIT,
    NINTENDO_WFC_MAIN_DONE,
};

typedef struct NintendoWfcGlobal {
    NNSFndHeapHandle heap;
    NintendoWfcConnect *wfc;
    u8 unk_8[0xC];
} NintendoWfcGlobal;

extern NintendoWfcGlobal ov72_0223B92C;

extern const GraphicsModes ov72_0223B354;

void NintendoWfc_InitArgs(NintendoWfcConnect *wfc, OverlayManager *man);

void NintendoWfc_InitGraphics(NintendoWfcConnect *wfc);

void NintendoWfc_SetState(NintendoWfcConnect *wfc, enum NintendoWfcState state, enum NintendoWfcState nextState);

enum NintendoWfcMainState NintendoWfc_SessionInit(NintendoWfcConnect *wfc, int state);

enum NintendoWfcMainState NintendoWfc_AskConnect(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_AskLogOut(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_InitHttp(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_RequestSetProfile(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_AskChallengeRoom(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_StartChallengeRoomInput(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintReceivingTrainers(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintRoomUnavailable(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_RequestTrainersDownload(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_WaitTrainersDownload(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintTrainersReceived(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintSendingRecord(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintRecordSent(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_AskLeadersRank(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_StartLeadersRankInput(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_AskLeadersRoom(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_StartLeadersRoomInput(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintReceivingLeaders(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_RequestLeadersDownload(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_WaitLeadersDownload(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintLeadersReceived(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_StartDisconnect(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_AskCancelChallenge(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_AskCancelDownload(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintDisconnecting(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_PrintDisconnected(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_StartExit(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_Disconnect(NintendoWfcConnect *wfc);

enum NintendoWfcMainState NintendoWfc_ShowDwcError(NintendoWfcConnect *wfc);

void ov72_02238144(void *arg0, OverlayManager *man);

void ov72_022387D8(NintendoWfcConnect *arg);

BOOL NintendoWfc_Init(OverlayManager *man, int *state);

BOOL NintendoWfc_Init(OverlayManager *man, int *state) {
    int new_var;
    switch (*state) {
    case NINTENDO_WFC_INIT_SETUP:
        new_var = 0;
        {
            GraphicsModes modes;
            NintendoWfcConnect *wfc;
            Main_SetVBlankIntrCB((void *)new_var, (void *)new_var);
            HBlankInterruptDisable();
            GfGfx_DisableEngineAPlanes();
            GfGfx_DisableEngineBPlanes();
            reg_GX_DISPCNT &= ~0x1F00;
            reg_GXS_DB_DISPCNT &= ~0x1F00;
            Heap_Create(HEAP_ID_3, HEAP_ID_67, 0x50000);
            LoadDwcOverlay();
            LoadOVY38();
            sub_02039FD8(HEAP_ID_67);
            wfc = OverlayManager_CreateAndGetData(man, sizeof(NintendoWfcConnect), HEAP_ID_67);
            memset(wfc, new_var, sizeof(NintendoWfcConnect));
            wfc->bgConfig = BgConfig_Alloc(HEAP_ID_67);
            ov72_0223B92C.wfc = wfc;
            modes = ov72_0223B354;
            SetBothScreensModesAndDisable(&modes);
            wfc->msgFormat = MessageFormat_New_Custom(11, 32, HEAP_ID_67);
            wfc->msgDataTower = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 774, HEAP_ID_67);
            wfc->msgDataWfc = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 778, HEAP_ID_67);
            wfc->msgDataError = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 800, HEAP_ID_67);
            SetKeyRepeatTimers(4, 8);
            ov72_02238144(wfc, man);
            ov72_022387D8(wfc);
            Sound_SetSceneAndPlayBGM(52, new_var, new_var);
            wfc->heapMem = Heap_Alloc(HEAP_ID_67, 0x20020);
            wfc->expHeap = NNS_FndCreateExpHeapEx((void *)((((u32)wfc->heapMem) + 31) & (~31)), 0x20000, new_var);
            Sound_SetSceneAndPlayBGM(11, 1149, 1);
            TextFlags_SetCanTouchSpeedUpPrint(1);
            reg_GX_POWCNT |= 0x8000;
            *state = NINTENDO_WFC_INIT_START_WIRELESS;
            break;
        }

    case NINTENDO_WFC_INIT_START_WIRELESS:
        sub_02034D8C();
        FontID_Alloc((Font)4, HEAP_ID_67);
        *state = NINTENDO_WFC_INIT_SETUP;
        return 1;
    }

    return new_var;
}
