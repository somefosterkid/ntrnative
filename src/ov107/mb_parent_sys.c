// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov181/mb_parent_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mb_parent.h"
#include "app/mb_parent/mb_comm_sys.h"
#include "app/mb_parent/mb_util_msg.h"
#include "app/mb_parent/mbp.h"
#include "constants/arc.h"
#include "constants/field_script.h"
#include "constants/items.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "gfl/wih.h"
#include "gfl/wm_icon.h"
#include "nitro/card.h"
#include "nitro/fs.h"
#include "nitro/gx.h"
#include "nitro/mb.h"
#include "nitro/os.h"
#include "nitro/wm.h"
#include "pml/met_data.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/box.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/app_menu_common.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/startmenu_gfl_net_err_disp_message.h"
#include "field/beacon_view.h"

static const TouchRect data_ov107_021f6af8[2] = {
    { 168, 192, 232, 255 },
    { 255, 0, 0, 0 },
};

// The DS Download Play parent (mb_parent_sys.c): sends a program to another system by DS Download Play and stores the
// Pokémon it sends back. The Poké Transfer Lab starts it to bring Pokémon over from the Gen 4 games, and the start
// menu's item starts it with a game data of its own; that this is the Pokémon Dream Radar's transfer is our inference,
// from the second child program, the Lock Capsule it gives and the Dream Radar flag it checks

// The game's GGID for DS Download Play
#define MB_PARENT_GGID 0x1380

// The states of func_ov107_021ee894
enum {
    MB_PARENT_STATE_WIPE_IN,
    MB_PARENT_STATE_WAIT_WIPE_IN,
    MB_PARENT_STATE_WIPE_OUT,
    MB_PARENT_STATE_WAIT_WIPE_OUT,
    MB_PARENT_STATE_ASK_START,
    MB_PARENT_STATE_WAIT_ASK_MSG,
    MB_PARENT_STATE_ASK_ANSWER,
    MB_PARENT_STATE_SETUP_DOWNLOAD,
    MB_PARENT_STATE_DOWNLOAD,
    MB_PARENT_STATE_END_DOWNLOAD,
    MB_PARENT_STATE_WAIT_NET_START,
    MB_PARENT_STATE_WAIT_CHILD,
    MB_PARENT_STATE_LOAD_CHILD_PROGRAM,
    MB_PARENT_STATE_CHILD_PROGRAM_LOADED,
    MB_PARENT_STATE_WAIT_CHILD_REQUEST,
    MB_PARENT_STATE_SEND_PROGRAM,
    MB_PARENT_STATE_WAIT_PROGRAM_SENT,
    MB_PARENT_STATE_WAIT_POKEMON,
    MB_PARENT_STATE_ACK_POKEMON,
    MB_PARENT_STATE_WAIT_ACK,
    MB_PARENT_STATE_STORE_POKEMON,
    MB_PARENT_STATE_SAVE,
    MB_PARENT_STATE_SAVED,
    MB_PARENT_STATE_SEND_BOX_SPACE,
    MB_PARENT_STATE_WAIT_CHILD_NEXT,
    MB_PARENT_STATE_FINISH,
    MB_PARENT_STATE_WAIT_FINISH,
    MB_PARENT_STATE_SEND_END,
    MB_PARENT_STATE_WAIT_END,
    MB_PARENT_STATE_WAIT_NET_END,
    MB_PARENT_STATE_TIMEOUT,
    MB_PARENT_STATE_WAIT_TIMEOUT,
    MB_PARENT_STATE_RESULT,
    MB_PARENT_STATE_NO_WIRELESS,
};

typedef struct {
    HeapID heapId;
    TCB *vblankTask;
    MBParentParam *param;
    MBCommSys *comm;
    BOOL netError;
    BOOL wirelessError;
    BOOL eventsPaused;
    BOOL wirelessStarted;
    u32 state;
    u8 distributionSeq;
    u8 resultSeq;
    u8 cancelSeq;
    u8 waitFrames;
    u8 startMenu;
    u16 waitConnectFrames;
    BOOL childRomSent;
    BOOL distributionDone;
    int answer;
    u16 highScore;
    u16 transferCount;
    u16 receivedCount;
    BOOL boxFull;
    BOOL receivedMore;
    BOOL receivedItem;
    TrainerGameInfoSave *trainerGameInfo;
    u16 *gameName;
    u16 *gameIntro;
    MBCommParentInfo parentInfo;
    u16 unk_5c;
    u8 unk_5e;
    void *childRom;
    u32 childRomSize;
    MBUtilMsg *msg;
    ClActUnit *actorUnit;
    u32 cancelButtonPalette;
    u32 cancelButtonChars;
    u32 cancelButtonCellAnims;
    ClActor *cancelButton;
    MBGameRegistry registry;
} MBParentWork;

// White's strings of the multiboot parent's message file sit one after Black's
#define MB_PARENT_MSG(id) ((id) + (GAME_VERSION == VERSION_WHITE))

void func_ov107_021ee740(MBParentWork *wk);
void func_ov107_021ee824(MBParentWork *wk);
BOOL func_ov107_021ee894(MBParentWork *wk);
void func_ov107_021eeeac(TCB *tcb, void *data);
void func_ov107_021eeeb4(void *data);
void func_ov107_021eeef8(void *data);
void func_ov107_021eef28(MBParentWork *wk);
void func_ov107_021ef080(MBParentWork *wk);
void func_ov107_021ef0d0(const BGSetup *setup, u8 bg, u8 mode);
void func_ov107_021ef0f4(MBParentWork *wk);
void func_ov107_021ef444(MBParentWork *wk);
void func_ov107_021ef588(MBParentWork *wk);
void func_ov107_021ef74c(MBParentWork *wk);
BOOL func_ov107_021ef79c(MBParentWork *wk);
void func_ov107_021ef96c(MBParentWork *wk);
void func_ov107_021efa14(MBParentWork *wk);
BOOL func_ov107_021efc10(BOOL success);
void func_ov107_021efc1c(MBParentWork *wk, u8 result);
void func_ov107_021efca8(void *work);
void func_ov107_021efd24(MBParentWork *wk);
void func_ov107_021efd98(MBParentWork *wk);
void func_ov107_021efe3c(MBParentWork *wk);
BOOL func_ov107_021f10fc(MBCommSys *comm);
u8 func_ov107_021f110c(MBCommSys *comm);
void func_ov107_021efe40(MBParentWork *wk);
void func_ov107_021effb0(MBParentWork *wk);
BOOL func_ov107_021f05dc(void);
BOOL func_ov107_021f0608(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov107_021f0694(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov107_021f0728(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov107_021f6c10 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's sBG6Setup
static const BGSetup data_ov107_021f6b10 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xd, 2, 0x8000, 1, 2, 0, 0 };

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov107_021f6b00 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sBG7Setup
static const BGSetup data_ov107_021f6b30 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xe, 6, 0x8000, 1, 3, 0, 0 };
// pokebw2's sBG0Setup
static const BGSetup data_ov107_021f6b50 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xb, 0, 0x5800, 0, 0, 0, 0 };
// pokebw2's sBG1Setup
static const BGSetup data_ov107_021f6b70 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xc, 6, 0x8000, 0, 1, 0, 0 };
// pokebw2's sBG2Setup
static const BGSetup data_ov107_021f6b90 = { 0, 0, 0x1000, 0, BGRES_512x256, GX_BG_COLORMODE_16, 0xd, 2, 0x8000, 1, 2, 1, 0 };
// pokebw2's sBG3Setup
static const BGSetup data_ov107_021f6bb0 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xf, 4, 0x8000, 1, 3, 0, 0 };
// pokebw2's sBG4Setup
static const BGSetup data_ov107_021f6bd0 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xf, 0, 0x6000, 1, 0, 0, 0 };
// pokebw2's sBG5Setup
static const BGSetup data_ov107_021f6bf0 = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, 0xc, 4, 0x8000, 1, 1, 0, 0 };

static const TouchRect sCancelButtonRect[] = {
    { 168, 192, 232, 255 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's MBParentProc_Exit
// pokebw2's MBParentProc_Init
// pokebw2's MBParentProc_Main
GameProcFunctions MB_PARENT_PROC_FUNCTIONS = { func_ov107_021f0608, func_ov107_021f0728, func_ov107_021f0694 };

// pokebw2's sScrollWait
static u8 data_ov107_021f6ea0;
static u16 sScrollFrames;
// Set by the wireless helper once it has shut down
static BOOL sWirelessDone;
// Whether the title is on the sub engine's BG 4, for the Poké Transfer Lab, rather than BG 2
static BOOL sTitleOnSub;

// pokebw2's MBParent_Init
void func_ov107_021ee740(MBParentWork *wk) {
    wk->resultSeq = 0;
    wk->state = MB_PARENT_STATE_WIPE_IN;
    wk->startMenu = wk->param->startMenu;
    func_ov107_021eef28(wk);
    func_ov107_021ef0f4(wk);
    if (wk->startMenu == FALSE) {
        wk->msg = func_ov107_021f1404(wk->heapId, 5, 5, 0x48, 0, 0);
    } else {
        wk->msg = func_ov107_021f1404(wk->heapId, 1, 1, 0x48, 0, 1);
    }
    wk->comm = func_ov107_021f0de8(wk->heapId);
    wk->childRomSent = FALSE;
    wk->childRom = NULL;
    func_ov107_021ef444(wk);
    if (wk->startMenu == FALSE) {
        wk->trainerGameInfo = getTrainerGameInfoAddress(GameData_GetSaveControl(wk->param->gameData));
        func_0200c1b0(wk->trainerGameInfo, 4);
    } else {
        wk->trainerGameInfo = NULL;
        sScrollFrames = 0;
    }
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov107_021eeeac, wk, 8);
    if (wk->startMenu == FALSE) {
        GFL_VBlankSetCallback(func_ov107_021eeeb4, NULL);
    } else {
        GFL_VBlankSetCallback(func_ov107_021eeef8, NULL);
        GFL_SndBGMPlay(SEQ_BGM_WIFI_PRESENT, SND_CHANNEL_MASK_ALL);
    }
    wk->wirelessStarted = FALSE;
    wk->receivedMore = FALSE;
    wk->receivedItem = FALSE;
    wk->highScore = 0;
    wk->transferCount = 0;
    wk->receivedCount = 0;
    func_02034f84(0x10);
}

// pokebw2's MBParent_Exit
void func_ov107_021ee824(MBParentWork *wk) {
    func_02034fb8(0x10);
    GFL_TCBRemove(wk->vblankTask);
    GFL_VBlankResetCallback();
    func_020370c8();
    if (wk->startMenu == TRUE) {
        func_02005d28();
    } else {
        EventWork *eventWork = GameData_GetEventWork(wk->param->gameData);
        u16 *highScore = EventWork_GetWkPtr(eventWork, VARS_START);
        u16 *transferCount = EventWork_GetWkPtr(eventWork, VARS_START + 1);
        *highScore = wk->highScore;
        *transferCount = wk->transferCount;
    }
    if (wk->childRom != NULL) {
        GFL_HeapFree(wk->childRom);
    }
    func_ov107_021f0e40(wk->comm);
    func_ov107_021f1520(wk->msg);
    func_ov107_021ef080(wk);
}

BOOL func_ov107_021ee894(MBParentWork *wk) {
    func_ov107_021f0e6c(wk->comm);
    if (wk->netError == TRUE && wk->state != 2 && wk->state != 3) {
        wk->eventsPaused = GameData_CheckEventsPaused(wk->param->gameData);
        if (wk->eventsPaused == TRUE) {
            func_02012e08(wk->param->gameData);
        }
        wk->state = 2;
        func_ov107_021efc1c(wk, 3);
    }
    switch (wk->state) {
    case 0:
        GFL_WipeSet(0, 1, 1, 0, 6, 1, wk->heapId);
        wk->state = 1;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            if (func_0202eb44() == TRUE) {
                if (wk->startMenu == FALSE) {
                    wk->state = 7;
                } else {
                    wk->state = 4;
                }
            } else {
                func_ov107_021f1628(wk->msg, 5);
                func_ov107_021f1a58(wk->msg, func_02012ffc());
                func_ov107_021f1b24(wk->msg, TRUE);
                wk->state = 33;
            }
        }
        break;
    case 2:
        if (wk->param->startMenu == FALSE) {
            GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
        } else {
            GFL_WipeSet(0, 0, 0, 0x7fff, 6, 1, wk->heapId);
            GFL_SndBGMFadeOut(6);
        }
        wk->state = 3;
        break;
    case 3:
        if (GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    case 4:
        func_ov107_021f1628(wk->msg, 5);
        func_ov107_021f1704(wk->msg, 0x2b, func_02012ffc());
        wk->state = 5;
        break;
    case 5:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            func_ov107_021f19e4(wk->msg, 0);
            wk->state = 6;
        }
        break;
    case 6: {
        int answer = func_ov107_021f1a2c(wk->msg);

        if (answer == 1) {
            wk->state = 7;
            func_ov107_021f1898(wk->msg);
            func_ov107_021f1a24(wk->msg);
        } else if (answer == 2) {
            wk->state = 2;
        }
        break;
    }
    case 7:
        func_ov107_021ef588(wk);
        wk->state = 8;
        break;
    case 8:
        if (func_ov107_021ef79c(wk) == 1) {
            wk->state = 9;
        }
        break;
    case 9:
        func_ov107_021ef74c(wk);
        if (wk->distributionDone == TRUE) {
            wk->state = 10;
            wk->waitConnectFrames = 0;
            func_ov107_021f0efc(wk->comm);
            if (wk->startMenu == FALSE) {
                func_0203d34c(FALSE, wk->heapId);
            } else {
                func_0203d34c(TRUE, wk->heapId);
            }
        } else {
            wk->state = 2;
        }
        break;
    case 10:
        if (func_ov107_021f1008(wk->comm) == TRUE) {
            wk->state = 11;
            func_ov107_021f1018(wk->comm);
        }
        break;
    case 11:
        wk->waitConnectFrames++;
        if (wk->waitConnectFrames >= 1800) {
            func_ov107_021efc1c(wk, 3);
            wk->state = 30;
        } else if (func_ov107_021f1050(wk->comm) == TRUE) {
            wk->parentInfo.textSpeed = func_02012ffc();
            wk->unk_5e = GFL_MsgDataGetDefaultLangID();
            if (wk->startMenu == FALSE) {
                wk->unk_5c = func_0200c184(wk->trainerGameInfo);
            }
            if (func_ov107_021f1370(wk->comm, &wk->parentInfo) == TRUE) {
                if (wk->startMenu == FALSE) {
                    wk->state = 12;
                } else {
                    wk->state = 32;
                }
            }
        }
        break;
    case 12: {
        FSFile file;
        const char *path = GFL_ArcSysGetResourcePath(0x8e);
        BOOL result;

        FS_InitFile(&file);
        result = FS_OpenFile(&file, path);
        GFL_ASSERT(result);
        FS_SeekFile(&file, 0, 0);
        wk->childRomSize = FS_GetLength(&file);
        wk->childRom = GFL_HeapAllocate(wk->heapId, wk->childRomSize, TRUE, "mb_parent_sys.c", 660);
        FS_ReadFile(&file, wk->childRom, wk->childRomSize);
        GFL_ASSERT(FS_CloseFile(&file));
        wk->state = 13;
        break;
    }
    case 13:
        wk->state = 14;
        break;
    case 14:
        if (func_ov107_021f1060(wk->comm) == 10) {
            func_ov107_021efc1c(wk, 3);
            func_ov107_021f1704(wk->msg, 8, func_02012ffc());
            func_ov107_021f1028(wk->comm);
            wk->state = 28;
        } else if (func_ov107_021f1060(wk->comm) == 2) {
            if (wk->childRomSent == FALSE) {
                wk->childRomSent = TRUE;
                func_ov107_021f1174(wk->comm, wk->childRom, wk->childRomSize);
            }
            func_ov107_021f1704(wk->msg, 2, func_02012ffc());
            wk->state = 15;
        }
        break;
    case 15:
        if (func_ov107_021f1060(wk->comm) == 3) {
            func_ov107_021f1704(wk->msg, 3, func_02012ffc());
            wk->state = 16;
        } else if (func_ov107_021f1060(wk->comm) == 13) {
            func_ov107_021efc1c(wk, 4);
            func_ov107_021f1704(wk->msg, 8, func_02012ffc());
            func_ov107_021f1028(wk->comm);
            wk->state = 28;
        }
        /* fallthrough */
    case 16:
        if (func_ov107_021f1060(wk->comm) == 4) {
            func_ov107_021f1184(wk->comm);
            func_ov107_021f1704(wk->msg, 4, func_02012ffc());
            wk->state = 17;
        }
        break;
    case 17:
        if (func_ov107_021f1060(wk->comm) == 5) {
            RecordAddOne(GameData_GetRecords(wk->param->gameData), 0x78);
            wk->transferCount++;
            wk->state = 18;
        } else if (func_ov107_021f1060(wk->comm) == 8) {
            RecordAddOne(GameData_GetRecords(wk->param->gameData), 0x78);
            wk->transferCount++;
            func_ov107_021efc1c(wk, 2);
            wk->state = 23;
        }
        break;
    case 18:
        if (func_ov107_021f11bc(wk->comm) == TRUE && func_ov107_021f11f0(wk->comm, MB_COMM_CMD_ACK, 0) == TRUE) {
            wk->state = 19;
        }
        break;
    case 19:
        if (func_ov107_021f11e8(wk->comm) == TRUE || wk->startMenu == TRUE) {
            if (func_ov107_021f1060(wk->comm) == 6) {
                wk->state = 20;
            } else if (func_ov107_021f1060(wk->comm) == 10) {
                if (wk->startMenu == FALSE) {
                    func_ov107_021efc1c(wk, 3);
                    func_ov107_021f1704(wk->msg, 8, func_02012ffc());
                    func_ov107_021f1028(wk->comm);
                    wk->state = 28;
                } else {
                    func_ov107_021f1704(wk->msg, 0x26, func_02012ffc());
                    func_ov107_021f1b24(wk->msg, TRUE);
                    wk->state = 26;
                }
            }
        }
        break;
    case 20:
        func_ov107_021efd24(wk);
        wk->state = 21;
        wk->distributionSeq = 9;
        break;
    case 21:
        func_ov107_021efe40(wk);
        break;
    case 22:
        func_ov107_021efe3c(wk);
        if (wk->startMenu == FALSE) {
            func_ov107_021f1704(wk->msg, 7, func_02012ffc());
            wk->state = 23;
        } else {
            func_ov107_021f1704(wk->msg, 0x27, func_02012ffc());
            func_ov107_021f1b24(wk->msg, TRUE);
            wk->state = 27;
        }
        break;
    case 23:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            u16 space = func_02007774(GameData_GetBoxSaveAccessor(wk->param->gameData));

            if (func_ov107_021f11f0(wk->comm, MB_COMM_CMD_BOX_SPACE, space) == TRUE) {
                func_ov107_021f1704(wk->msg, 0xb, func_02012ffc());
                wk->state = 24;
            }
        }
        break;
    case 24:
        if (func_ov107_021f1060(wk->comm) == 2) {
            func_ov107_021f1064(wk->comm);
            wk->state = 14;
        } else if (func_ov107_021f1060(wk->comm) == 9 || func_ov107_021f1060(wk->comm) == 10) {
            if (func_ov107_021f1060(wk->comm) == 10) {
                func_ov107_021efc1c(wk, 3);
            }
            func_ov107_021f1704(wk->msg, 8, func_02012ffc());
            wk->state = 27;
        }
        break;
    case 27:
        if (func_ov107_021f11f0(wk->comm, MB_COMM_CMD_END, 0) == TRUE) {
            func_ov107_021f1028(wk->comm);
            wk->state = 28;
        }
        break;
    case 28:
        if (func_ov107_021f1b14(wk->msg) == TRUE && func_ov107_021f1040(wk->comm) == TRUE) {
            func_ov107_021f0fe8(wk->comm);
            wk->state = 29;
        }
        break;
    case 29:
        if (func_ov107_021f1010(wk->comm) == TRUE) {
            if (wk->startMenu == FALSE) {
                wk->state = 2;
            } else {
                wk->state = 25;
            }
        }
        break;
    case 25:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            func_ov107_021f1898(wk->msg);
            func_ov107_021f1628(wk->msg, 1);
            func_ov107_021f1704(wk->msg, 0x23, func_02012ffc());
            func_ov107_021f1b24(wk->msg, TRUE);
            wk->state = 26;
        }
        break;
    case 26:
        if (func_ov107_021f1010(wk->comm) == TRUE && func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->state = 2;
        }
        break;
    case 30:
        func_ov107_021f1704(wk->msg, 0xa, func_02012ffc());
        func_ov107_021f1b24(wk->msg, TRUE);
        wk->state = 31;
        func_ov107_021f0fe8(wk->comm);
        break;
    case 31:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->state = 29;
        }
        /* fallthrough */
    case 32:
        func_ov107_021effb0(wk);
        break;
    case 33:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->state = 2;
        }
        break;
    }
    func_ov107_021f15b0(wk->msg);
    func_0204a600();
    return FALSE;
}

// pokebw2's MBParent_VBlankTask
void func_ov107_021eeeac(TCB *tcb, void *data) {
    func_0204a648();
}

// pokebw2's MBParent_ScrollBGs
void func_ov107_021eeeb4(void *data) {
    if (data_ov107_021f6ea0 > 1) {
        data_ov107_021f6ea0 = 0;
        GFL_BGSysMoveBG(3, BG_MOVE_LEFT, 1);
        GFL_BGSysMoveBG(3, BG_MOVE_UP, 1);
        GFL_BGSysMoveBG(7, BG_MOVE_LEFT, 1);
        GFL_BGSysMoveBG(7, BG_MOVE_UP, 1);
    } else {
        data_ov107_021f6ea0++;
    }
}

// pokebw2's MBParent_ScrollBGsSlow
void func_ov107_021eeef8(void *data) {
    sScrollFrames++;
    if (sScrollFrames >= 5) {
        sScrollFrames = 0;
        GFL_BGSysMoveBG(3, BG_MOVE_UP, 1);
        GFL_BGSysMoveBG(7, BG_MOVE_UP, 1);
    }
}

void func_ov107_021eef28(MBParentWork *wk) {
    ClActSysSetup clactSetup;

    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetVRAMBanks(&data_ov107_021f6c10);
    GFL_BGSysCreate(wk->heapId);
    BmpWin_InitAllocator(wk->heapId);
    GFL_BGSysSetLCDConfig(&data_ov107_021f6b00);
    func_ov107_021ef0d0(&data_ov107_021f6b50, 0, 0);
    func_ov107_021ef0d0(&data_ov107_021f6b70, 1, 0);
    func_ov107_021ef0d0(&data_ov107_021f6b90, 2, 0);
    func_ov107_021ef0d0(&data_ov107_021f6bb0, 3, 0);
    func_ov107_021ef0d0(&data_ov107_021f6bd0, 4, 0);
    func_ov107_021ef0d0(&data_ov107_021f6bf0, 5, 0);
    func_ov107_021ef0d0(&data_ov107_021f6b10, 6, 0);
    func_ov107_021ef0d0(&data_ov107_021f6b30, 7, 0);
    if (wk->startMenu == FALSE) {
        GFL_BGSysSetBGEnabled(6, TRUE);
    } else {
        GFL_BGSysSetBGEnabled(6, FALSE);
    }
    clactSetup = data_020a1448;
    clactSetup.charCount = 64;
    ClActSys_Create(&clactSetup, &data_ov107_021f6c10, wk->heapId);
    wk->actorUnit = func_0204b100(8, 0, wk->heapId);
    func_0204b270(wk->actorUnit);
    GFL_BGSysSetBGEnabledA(0x10, TRUE);
    GFL_BGSysSetBGEnabledB(0x10, TRUE);
}


// pokebw2's MBParent_FreeGraphics
void func_ov107_021ef080(MBParentWork *wk) {
    func_0204b3b4(wk->cancelButton);
    func_0204b1cc(wk->actorUnit);
    func_0204a5ac();
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's MBParent_CreateBG
void func_ov107_021ef0d0(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's MBParent_LoadGraphics
void func_ov107_021ef0f4(MBParentWork *wk) {
    ArcTool *arc;

    if (wk->startMenu == FALSE) {
        arc = GFL_ArcSysCreateFileHandle(ARCID_DATA_CONVERT, wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 1, 4, 0, 0, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 5, 7, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 9, 7, 0, 0, FALSE, wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0, 0, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 4, 3, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 8, 3, 0, 0, FALSE, wk->heapId);
        GFL_G2DIOLoadArcNCLR(arc, 2, 0, 0x120, 0x120, 0x80, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 6, 2, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 10, 2, 0, 0, FALSE, wk->heapId);
        GFL_G2DIOLoadArcNCLR(arc, 3, 0, 0x1a0, 0x1a0, 0x20, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 7, 0, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 11, 0, 0, 0, FALSE, wk->heapId);
        GFL_BGSysLoadScr(2);
        GFL_ArcToolFree(arc);
    } else {
        arc = GFL_ArcSysCreateFileHandle(ARCID_MYSTERY, wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, 4, 0, 0, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 15, 7, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 7, 0, 0, FALSE, wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0, 0, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 15, 3, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 31, 3, 0, 0, FALSE, wk->heapId);
        GFL_ArcToolFree(arc);
        arc = GFL_ArcSysCreateFileHandle(ARCID_DATA_CONVERT, wk->heapId);
        GFL_G2DIOLoadArcNCLR(arc, 2, 4, 0x120, 0x120, 0x80, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 6, 5, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 10, 5, 0, 0, FALSE, wk->heapId);
        GFL_G2DIOLoadArcNCLR(arc, 3, 4, 0x1a0, 0x1a0, 0x20, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, 7, 4, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 11, 4, 0, 0, FALSE, wk->heapId);
        GFL_BGSysLoadScr(5);
        GFL_BGSysSetBGEnabled(5, FALSE);
        GFL_BGSysSetBGEnabled(4, FALSE);
        GFL_ArcToolFree(arc);
    }
    {
        ClActorSetup setup;

        arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), wk->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 4, 0x100, 0x20, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), 6, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, func_0202762c(), 6, 0, 0, FALSE, wk->heapId);
        GFL_BGSysSetScrPaletteNo(6, 0, 21, 32, 3, 8);
        GFL_BGSysLoadScr(6);
        wk->cancelButtonPalette = func_0204ac18(arc, func_02027614(), CLACT_VRAM_SUB, 0, 0, 3, wk->heapId);
        wk->cancelButtonChars = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_SUB, wk->heapId);
        wk->cancelButtonCellAnims = func_0204af28(arc, func_0202761c(2), func_02027620(2), wk->heapId);
        GFL_ArcToolFree(arc);
        setup.x = 232;
        setup.y = 168;
        setup.sequence = 1;
        setup.bgPriority = 0;
        setup.priority = 0;
        wk->cancelButton = func_0204b294(wk->actorUnit, wk->cancelButtonChars, wk->cancelButtonPalette,
                                         wk->cancelButtonCellAnims, &setup, CLACT_SURFACE_SUB, wk->heapId);
    }
    if (wk->startMenu == FALSE) {
        func_0204b3dc(wk->cancelButton, TRUE);
    } else {
        func_0204b3dc(wk->cancelButton, FALSE);
    }
}

// pokebw2's MBParent_DrawTopScreen
void func_ov107_021ef444(MBParentWork *wk) {
    u8 bg;
    ArcTool *arc;
    BmpWin *window;
    MsgData *msgData;
    StrBuf *strbuf;

    if (wk->startMenu == FALSE) {
        sTitleOnSub = TRUE;
        bg = 4;
    } else {
        sTitleOnSub = FALSE;
        bg = 2;
    }
    GFL_BGSysSetBGEnabled(bg, FALSE);
    arc = GFL_ArcSysCreateFileHandle(22, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 4, 0xe0, 0x20, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0xe0, 0x20, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, bg, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, bg, 0, 0, FALSE, wk->heapId);
    GFL_BGSysSetScrPaletteNo(bg, 0, 0, 32, 24, 7);
    GFL_BGSysLoadScr(bg);
    GFL_ArcToolFree(arc);
    window = BmpWin_CreateDynamic(bg, 1, 4, 30, 8, 7, TRUE);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_STARTMENU_GFL_NET_ERR_DISP_MESSAGE, wk->heapId);
    strbuf = GFL_MsgDataLoadStrbufNew(msgData, StartmenuGflNetErrDispMessage_Text_001);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 7);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), 0, 0, strbuf, func_ov107_021f1b04(wk->msg), 0x1167);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    GFL_StrBufFree(strbuf);
    GFL_MsgDataFree(msgData);
    BmpWin_Free(window);
    ((u16 *)HW_BG_PLTT)[0] = GX_RGB(10, 23, 31);
    ((u16 *)HW_DB_BG_PLTT)[0] = GX_RGB(10, 23, 31);
}

static inline void CopyString(u16 *dest, const StrBuf *src, u16 length) {
    MIi_CpuCopy16(GFL_StrBufGetStringPtr(src), dest, length * 2);
}

void func_ov107_021ef588(MBParentWork *wk) {
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
    StrBuf *src;
    StrBuf *name;
    StrBuf *intro;
    u16 nameLength;
    u16 introLength;
    int i;

    wk->distributionSeq = 0;
    wk->cancelSeq = 0;
    wk->gameName = GFL_HeapAllocate(wk->heapId, 0x60, TRUE, "mb_parent_sys.c", 1363);
    wk->gameIntro = GFL_HeapAllocate(wk->heapId, 0xc0, TRUE, "mb_parent_sys.c", 1364);
    func_ov107_021f1628(wk->msg, 1);
    func_ov107_021f18d8(wk->msg);
    func_ov107_021f1918(wk->msg, 0, getTrainerID(playerInfo), 5);
    if (wk->startMenu == FALSE) {
        src = GFL_MsgDataLoadStrbufNew(func_ov107_021f1afc(wk->msg), 0xc);
    } else {
        src = GFL_MsgDataLoadStrbufNew(func_ov107_021f1afc(wk->msg), 0xf);
    }
    name = GFL_StrBufCreate(0x100, wk->heapId);
    GFL_WordSetFormatStrbuf(func_ov107_021f1b00(wk->msg), name, src);
    GFL_StrBufFree(src);
    if (wk->startMenu == FALSE) {
        intro = GFL_MsgDataLoadStrbufNew(func_ov107_021f1afc(wk->msg), MB_PARENT_MSG(0xd));
    } else {
        intro = GFL_MsgDataLoadStrbufNew(func_ov107_021f1afc(wk->msg), MB_PARENT_MSG(0x10));
    }
    nameLength = GFL_StrBufGetCharCount(name);
    introLength = GFL_StrBufGetCharCount(intro);
    MI_CpuCopy16(GFL_StrBufGetStringPtr(name), wk->gameName, nameLength * 2);
    MI_CpuCopy16(GFL_StrBufGetStringPtr(intro), wk->gameIntro, introLength * 2);
    GFL_StrBufFree(name);
    GFL_StrBufFree(intro);
    wk->gameName[nameLength] = 0;
    wk->gameIntro[introLength] = 0;
    for (i = 0; i < 0x60; i++) {
        if (wk->gameIntro[i] == 0xfffe) {
            wk->gameIntro[i] = 0xa;
        }
    }
    for (i = 0; i < 0x30; i++) {
        if (wk->gameName[i] == 0xff29) {
            wk->gameName[i] = 0x49;
        } else if (wk->gameName[i] == 0xff24) {
            wk->gameName[i] = 0x44;
        } else if (wk->gameName[i] >= 0xff10 && wk->gameName[i] <= 0xff19) {
            wk->gameName[i] -= 0xfee0;
        }
    }
    wk->distributionDone = FALSE;
    GFL_OvlLoad(OVERLAY_ID(14));
    if (wk->startMenu == FALSE) {
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysSetBGEnabled(0, TRUE);
    } else {
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysSetBGEnabled(4, TRUE);
    }
    CARD_SetPulledOutCallback(func_ov107_021f05dc);
}


void func_ov107_021ef74c(MBParentWork *wk) {
    GFL_OvlUnload(OVERLAY_ID(14));
    CARD_SetPulledOutCallback(NULL);
    GFL_HeapFree(wk->gameName);
    GFL_HeapFree(wk->gameIntro);
    if (wk->startMenu == FALSE) {
        GFL_BGSysSetBGEnabled(2, FALSE);
        GFL_BGSysSetBGEnabled(0, FALSE);
    } else {
        GFL_BGSysSetBGEnabled(5, FALSE);
        GFL_BGSysSetBGEnabled(4, FALSE);
    }
}


// pokebw2's MBParent_UpdateDistribution
BOOL func_ov107_021ef79c(MBParentWork *wk) {
    if (func_ov014_02178e4c() == WH_SYSSTATE_ERROR || func_ov014_02178e4c() == WH_SYSSTATE_FATAL) {
        wk->distributionDone = FALSE;
        wk->wirelessError = TRUE;
        if (func_ov014_02179498() == TRUE) {
            func_ov107_021f0db4();
            func_ov014_02179158();
            return TRUE;
        }
        return FALSE;
    }
    switch (wk->distributionSeq) {
    case 0:
        sWirelessDone = FALSE;
        wk->wirelessStarted = TRUE;
        func_ov014_021790e4(wk->heapId, func_ov107_021efc10, 0);
        func_02037010(240, 0, 0, wk->heapId);
        if (wk->startMenu == FALSE) {
            func_02037118(FALSE, wk->heapId);
        } else {
            func_02037118(TRUE, wk->heapId);
        }
        wk->distributionSeq = 1;
        GCTX_HIDSetSoftResetCallback(func_ov107_021efca8, wk);
        break;
    case 1:
        if (func_ov014_02178e4c() == WH_SYSSTATE_IDLE && func_ov014_02178e84() == TRUE) {
            wk->distributionSeq = 2;
        }
        break;
    case 2:
        if (func_ov014_02178e4c() == WH_SYSSTATE_MEASURECHANNEL) {
            PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);

            func_ov107_021ef96c(wk);
            func_ov107_021f1898(wk->msg);
            if (wk->startMenu == FALSE) {
                func_ov107_021f1628(wk->msg, 2);
            } else {
                func_ov107_021f1628(wk->msg, 3);
            }
            func_ov107_021f18d8(wk->msg);
            func_ov107_021f1918(wk->msg, 0, getTrainerID(playerInfo), 5);
            if (wk->startMenu == FALSE) {
                func_ov107_021f1704(wk->msg, 0, func_02012ffc());
            } else {
                func_ov107_021f1704(wk->msg, 0x14, func_02012ffc());
            }
            func_ov107_021f18ec(wk->msg);
            wk->distributionSeq = 3;
        }
        break;
    case 3:
        func_ov107_021efa14(wk);
        break;
    case 4:
        func_ov107_021f1898(wk->msg);
        func_ov107_021f1628(wk->msg, 1);
        if (wk->startMenu == FALSE) {
            func_ov107_021f17e0(wk->msg, 1);
        } else {
            func_ov107_021f17e0(wk->msg, 0x15);
        }
        func_ov107_021f1b38(wk->msg, 1);
        func_0204b878(wk->cancelButton, 15);
        wk->distributionSeq = 5;
        break;
    case 5:
        wk->distributionDone = TRUE;
        wk->distributionSeq = 6;
        break;
    case 8:
        wk->distributionSeq = 6;
        break;
    case 6:
        sWirelessDone = FALSE;
        func_ov014_02179570(func_ov107_021efc10);
        wk->wirelessStarted = FALSE;
        wk->distributionSeq = 7;
        break;
    case 7:
        if (sWirelessDone == TRUE) {
            GCTX_HIDSetSoftResetCallback(NULL, NULL);
            func_ov014_02179158();
            return TRUE;
        }
        break;
    }
    func_020370f4(WM_LINK_LEVEL_3 - WM_GetLinkLevel());
    func_02037154();
    return FALSE;
}

void func_ov107_021ef96c(MBParentWork *wk) {
    const char *romPaths[2] = { "/dl_rom/child_r_eng.srl", "/dl_rom/child2_r_eng.srl" };
    // White's icon, then Black's
    const char *iconCharPaths[2] = { "/dl_rom/icon_w.char", "/dl_rom/icon_b.char" };
    const char *iconPalettePaths[2] = { "/dl_rom/icon_w.plt", "/dl_rom/icon_b.plt" };
    MBGameRegistry registry = {
        NULL, NULL, NULL, NULL, NULL, MB_PARENT_GGID, 2,
    };
    u16 channel = func_ov014_02179024();

    MI_CpuCopy8(&registry, &wk->registry, sizeof(MBGameRegistry));
    if (wk->startMenu == FALSE) {
        wk->registry.romFilePathp = romPaths[0];
    } else {
        wk->registry.romFilePathp = romPaths[1];
    }
    wk->registry.iconCharPathp = iconCharPaths[GAME_VERSION == VERSION_BLACK];
    wk->registry.iconPalettePathp = iconPalettePaths[GAME_VERSION == VERSION_BLACK];
    wk->registry.gameNamep = wk->gameName;
    wk->registry.gameIntroductionp = wk->gameIntro;
    func_ov107_021f0754(MB_PARENT_GGID, MB_TGID_AUTO);
    func_ov107_021f080c(&wk->registry, channel);
}






static const char *data_ov107_021f6d60[2] = {
    "/dl_rom/icon_w.plt",
    "/dl_rom/icon_b.plt",
};

void func_ov107_021efa14(MBParentWork *wk) {
    func_ov107_021f0d28();
    switch (func_ov107_021f0d28()) {
    case 2:
        if (wk->cancelSeq == 0) {
            s32 hit = func_02035c08(data_ov107_021f6af8);

            if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B || (hit == 0 && wk->startMenu == FALSE)) {
                wk->cancelSeq = 1;
                func_0204b878(wk->cancelButton, 9);
                func_0204b92c(wk->cancelButton, TRUE);
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
            } else if (func_ov107_021f0a28()) {
                func_ov107_021f0a64();
            }
        }
        break;
    case 5:
        wk->distributionSeq = 4;
        break;
    case 7:
        func_ov107_021f0ae4();
        break;
    case 0:
        wk->distributionSeq = 8;
        break;
    }
    switch (wk->cancelSeq) {
    case 1:
        func_ov107_021f1898(wk->msg);
        func_0204b878(wk->cancelButton, 15);
        if (wk->startMenu == FALSE) {
            func_ov107_021f1628(wk->msg, 7);
            func_ov107_021f1704(wk->msg, 9, func_02012ffc());
        } else {
            func_ov107_021f1628(wk->msg, 5);
            func_ov107_021f1704(wk->msg, 0x2a, func_02012ffc());
        }
        wk->cancelSeq = 2;
        break;
    case 2:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            if (wk->startMenu == FALSE) {
                func_ov107_021f1930(wk->msg, 0);
            } else {
                func_ov107_021f19e4(wk->msg, 0);
            }
            wk->cancelSeq = 3;
        }
        break;
    case 3: {
        int answer;

        if (wk->startMenu == FALSE) {
            answer = func_ov107_021f19c8(wk->msg);
        } else {
            answer = func_ov107_021f1a2c(wk->msg);
        }
        if (answer == 1) {
            GFL_ASSERT(func_ov107_021f0d28() == 2);
            func_ov107_021efc1c(wk, 4);
            func_ov107_021f0ae4();
            wk->cancelSeq = 4;
        } else if (answer == 2) {
            PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);

            if (wk->startMenu == FALSE) {
                func_ov107_021f19b8(wk->msg);
            } else {
                func_ov107_021f1a24(wk->msg);
            }
            func_ov107_021f1898(wk->msg);
            if (wk->startMenu == FALSE) {
                func_ov107_021f1628(wk->msg, 2);
            } else {
                func_ov107_021f1628(wk->msg, 3);
            }
            func_ov107_021f18d8(wk->msg);
            func_ov107_021f1918(wk->msg, 0, getTrainerID(playerInfo), 5);
            func_0204b878(wk->cancelButton, 1);
            if (wk->startMenu == FALSE) {
                func_ov107_021f1704(wk->msg, 0, func_02012ffc());
            } else {
                func_ov107_021f1704(wk->msg, 0x14, func_02012ffc());
            }
            func_ov107_021f18ec(wk->msg);
            wk->cancelSeq = 0;
        }
        break;
    }
    }
}


// pokebw2's MBParent_OnWirelessDone
BOOL func_ov107_021efc10(BOOL success) {
    sWirelessDone = TRUE;
    return TRUE;
}

// pokebw2's MBParent_SetPalParkResult
void func_ov107_021efc1c(MBParentWork *wk, u8 result) {
    if (wk->startMenu == FALSE) {
        u8 current = func_0200c1a4(wk->trainerGameInfo);
        BOOL set = FALSE;

        switch (result) {
        case 0:
            if (current != 1 && current != 3 && current != 5) {
                set = TRUE;
            }
            break;
        case 1:
            if (current != 3 && current != 5) {
                set = TRUE;
            }
            break;
        case 2:
            if (current != 0 && current != 1 && current != 3 && current != 5) {
                set = TRUE;
            }
            break;
        case 3:
            set = TRUE;
            if (wk->receivedCount != 0) {
                result = 5;
            }
            break;
        case 4:
            if (current != 2 && current != 0 && current != 1 && current != 3 && current != 5) {
                set = TRUE;
            }
            break;
        }
        if (set == TRUE) {
            func_0200c1b0(wk->trainerGameInfo, result);
        }
    }
}

// pokebw2's MBParent_SoftResetCallback
void func_ov107_021efca8(void *work) {
    MBParentWork *wk = work;

    if (func_ov107_021f0ddc() == TRUE) {
        func_ov107_021f0ae4();
    }
    while (func_ov107_021f0d28() != MBP_STATE_STOP && func_ov107_021f0d28() != MBP_STATE_COMPLETE) {
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    }
    if (wk->wirelessStarted == TRUE) {
        sWirelessDone = FALSE;
        while (func_ov014_02179570(func_ov107_021efc10)) {
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        }
        while (sWirelessDone == FALSE) {
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        }
    }
}

// pokebw2's MBParent_ReceivePokemon
void func_ov107_021efd24(MBParentWork *wk) {
    u8 count = func_ov107_021f11c4(wk->comm);

    func_ov107_021f17e0(wk->msg, 6);
    func_ov107_021f1b38(wk->msg, 1);
    func_ov107_021efd98(wk);
    if (wk->startMenu == FALSE) {
        u16 highScore = func_0200c184(wk->trainerGameInfo);
        u16 score = func_ov107_021f11e0(wk->comm);

        if (highScore < score) {
            func_0200c18c(wk->trainerGameInfo, score);
            func_ov107_021efc1c(wk, 1);
        } else if (count == 0) {
            func_ov107_021efc1c(wk, 2);
        } else {
            func_ov107_021efc1c(wk, 0);
        }
        if (wk->highScore < score) {
            wk->highScore = score;
        }
        wk->receivedCount += count;
    }
}

void func_ov107_021efd98(MBParentWork *wk) {
    BoxSaveAccessor *boxes = GameData_GetBoxSaveAccessor(wk->param->gameData);
    PokeDexSave *pokedex = GameData_GetPokedex(wk->param->gameData);
    u8 count = func_ov107_021f11c4(wk->comm);
    u8 i;

    if (wk->startMenu == FALSE) {
        GameRecords *records = GameData_GetRecords(wk->param->gameData);

        RecordAdd(records, 7, count);
        RecordAdd(records, 0x54, count);
    }
    for (i = 0; i < count; i++) {
        BoxPkm *pkm = func_ov107_021f11cc(wk->comm, i);
        BOOL result;
        PartyPkm *partyPkm;

        setMetCurrentDateTime(pkm);
        result = BoxSaveAccessor_InsertPkm(boxes, pkm);
        partyPkm = boxPkmRegenToPartyPkm(pkm, wk->heapId);
        addPkmToDex(pokedex, partyPkm);
        GFL_HeapFree(partyPkm);
        GFL_ASSERT(result == TRUE);
    }
}


// pokebw2's MBParent_Dummy
void func_ov107_021efe3c(MBParentWork *wk) {
}

void func_ov107_021efe40(MBParentWork *wk) {
    switch (wk->distributionSeq) {
    case 9:
        if (func_ov107_021f1b08(wk->msg) == TRUE && func_ov107_021f1b14(wk->msg) == TRUE &&
            func_ov107_021f10d4(wk->comm) == TRUE && func_ov107_021f11f0(wk->comm, 1, GFL_RandomLC(20) + 10) == TRUE) {
            wk->distributionSeq = 10;
        }
        break;
    case 10:
        if (func_ov107_021f10f4(wk->comm) == TRUE) {
            wk->distributionSeq = 11;
        }
        break;
    case 11:
        func_02012dac(wk->param->gameData);
        wk->distributionSeq = 12;
        break;
    case 12:
        if (func_02012dd0(wk->param->gameData) == 1) {
            wk->distributionSeq = 13;
        }
        break;
    case 13:
        if (func_ov107_021f10dc(wk->comm) == TRUE && func_ov107_021f11f0(wk->comm, 2, GFL_RandomLC(20) + 10) == TRUE) {
            wk->distributionSeq = 14;
            wk->waitFrames = 0;
        }
        break;
    case 14:
        if (func_ov107_021f10fc(wk->comm) == TRUE) {
            wk->waitFrames++;
            if (wk->waitFrames > func_ov107_021f110c(wk->comm)) {
                wk->distributionSeq = 15;
            }
        }
        break;
    case 15:
        if (func_02012dd0(wk->param->gameData) == 2) {
            wk->distributionSeq = 16;
        }
        break;
    case 16:
        if (func_ov107_021f10e4(wk->comm) == TRUE && func_ov107_021f11f0(wk->comm, 3, 0) == TRUE) {
            wk->distributionSeq = 17;
            wk->waitFrames = 0;
        }
        break;
    case 17:
        if (func_ov107_021f10ec(wk->comm) == TRUE && func_ov107_021f11f0(wk->comm, 4, 0) == TRUE) {
            wk->distributionSeq = 19;
        }
        break;
    case 19:
        if (func_ov107_021f1104(wk->comm) == TRUE) {
            wk->state = 0x16;
        }
        break;
    }
}


// pokebw2's MBParent_UpdateResult
void func_ov107_021effb0(MBParentWork *wk) {
    switch (wk->resultSeq) {
    case 0:
        if (func_ov107_021f1060(wk->comm) == 10 || func_ov107_021f1060(wk->comm) == 12) {
            func_ov107_021f1704(wk->msg, 0x29, func_02012ffc());
            func_ov107_021f1b24(wk->msg, 1);
            func_ov107_021f1028(wk->comm);
            wk->state = MB_PARENT_STATE_SEND_END;
        }
        if (func_ov107_021f1060(wk->comm) == 11) {
            func_ov107_021f1704(wk->msg, 0x2c, func_02012ffc());
            func_ov107_021f1b24(wk->msg, 1);
            func_ov107_021f1028(wk->comm);
            wk->state = MB_PARENT_STATE_SEND_END;
        }
        if (func_ov107_021f1110(wk->comm) == TRUE) {
            u16 count = func_ov107_021f1118(wk->comm);
            u16 more = func_ov107_021f1120(wk->comm);

            if (count != 0) {
                func_ov107_021f1898(wk->msg);
                func_ov107_021f1628(wk->msg, 5);
                func_ov107_021f18d8(wk->msg);
                func_ov107_021f1900(wk->msg, 0, count + more, 3);
#ifdef BLACK2
                func_ov107_021f1704(wk->msg, MB_PARENT_MSG(0x16), func_02012ffc());
#else
                func_ov107_021f1704(wk->msg, MB_PARENT_MSG(0x16), func_02012ffc());
#endif
                func_ov107_021f1b24(wk->msg, 1);
                func_ov107_021f18ec(wk->msg);
                wk->resultSeq = 1;
            } else {
                wk->resultSeq = 19;
            }
        }
        break;
    case 1:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            func_ov107_021f19e4(wk->msg, 1);
            wk->resultSeq = 2;
        }
        break;
    case 2:
        wk->answer = func_ov107_021f1a2c(wk->msg);
        if (wk->answer == 1) {
            u16 count = func_ov107_021f1118(wk->comm);
            u16 space = func_02007774(GameData_GetBoxSaveAccessor(wk->param->gameData));

            func_ov107_021f1a24(wk->msg);
            if (space < count) {
                wk->boxFull = TRUE;
                wk->resultSeq = 13;
            } else {
                wk->boxFull = FALSE;
                wk->resultSeq = 3;
            }
        } else if (wk->answer == 2) {
            wk->resultSeq = 13;
        }
        break;
    case 3:
        if (func_ov107_021f1120(wk->comm) != 0) {
            u16 count = func_ov107_021f1118(wk->comm);

            func_ov107_021f1898(wk->msg);
            func_ov107_021f1628(wk->msg, 1);
            func_ov107_021f18d8(wk->msg);
            func_ov107_021f1900(wk->msg, 0, count, 3);
            func_ov107_021f1704(wk->msg, 0x24, func_02012ffc());
            func_ov107_021f1b24(wk->msg, 1);
            func_ov107_021f18ec(wk->msg);
            wk->resultSeq = 4;
        } else {
            wk->resultSeq = 5;
        }
        break;
    case 4:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->resultSeq = 5;
        }
        break;
    case 5:
        if (func_ov107_021f1134(wk->comm) == TRUE) {
            func_ov107_021f1898(wk->msg);
            func_ov107_021f1628(wk->msg, 5);
            func_ov107_021f1704(wk->msg, 0x1a, func_02012ffc());
            wk->resultSeq = 6;
        } else {
            wk->resultSeq = 13;
        }
        break;
    case 6:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            func_ov107_021f19e4(wk->msg, 1);
            wk->resultSeq = 7;
        }
        break;
    case 7: {
        int answer = func_ov107_021f1a2c(wk->msg);

        if (answer == 1) {
            if (func_ov107_021f1148(wk->comm) == TRUE) {
                func_ov107_021f1704(wk->msg, 0x1b, func_02012ffc());
                wk->resultSeq = 8;
            } else {
                wk->resultSeq = 13;
            }
        } else if (answer == 2) {
            wk->resultSeq = 10;
        }
        break;
    }
    case 8:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            func_ov107_021f19e4(wk->msg, 1);
            wk->resultSeq = 9;
        }
        break;
    case 9: {
        int answer = func_ov107_021f1a2c(wk->msg);

        if (answer == 1) {
            wk->resultSeq = 13;
        } else if (answer == 2) {
            wk->resultSeq = 10;
        }
        break;
    }
    case 10:
        func_ov107_021f1704(wk->msg, 0x1c, func_02012ffc());
        wk->resultSeq = 11;
        break;
    case 11:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            func_ov107_021f19e4(wk->msg, 1);
            wk->resultSeq = 12;
        }
        break;
    case 12: {
        int answer = func_ov107_021f1a2c(wk->msg);

        if (answer == 1) {
            wk->answer = 2;
            wk->resultSeq = 13;
        } else if (answer == 2) {
            wk->resultSeq = 5;
        }
        break;
    }
    case 13: {
        u32 reply;

        if (wk->answer == 1) {
            if (wk->boxFull == TRUE) {
                reply = 2;
            } else {
                reply = 0;
            }
        } else {
            reply = 1;
        }
        if (func_ov107_021f11f0(wk->comm, MB_COMM_CMD_ANSWER, reply) == TRUE) {
            if (reply == 0) {
                wk->resultSeq = 14;
                func_ov107_021f1898(wk->msg);
                func_ov107_021f1628(wk->msg, 1);
                func_ov107_021f17e0(wk->msg, 0x19);
                func_ov107_021f1b38(wk->msg, 1);
            } else if (reply == 2) {
                wk->resultSeq = 17;
            } else {
                wk->resultSeq = 19;
            }
        }
        break;
    }
    case 14:
        if (func_ov107_021f11bc(wk->comm) == TRUE) {
            func_ov107_021efd98(wk);
            wk->resultSeq = 15;
        }
        if (func_ov107_021f115c(wk->comm) == TRUE) {
            u16 count = func_ov107_021f1118(wk->comm);

            func_ov107_021f1898(wk->msg);
            func_ov107_021f1628(wk->msg, 1);
            func_ov107_021f18d8(wk->msg);
            func_ov107_021f1900(wk->msg, 0, count, 3);
            func_ov107_021f1704(wk->msg, 0x1e, func_02012ffc());
            func_ov107_021f1b24(wk->msg, 1);
            func_ov107_021f18ec(wk->msg);
            wk->resultSeq = 16;
            wk->receivedMore = TRUE;
        }
        break;
    case 15:
        if (func_ov107_021f11f0(wk->comm, MB_COMM_CMD_ACK, 0) == TRUE) {
            func_ov107_021f1184(wk->comm);
            wk->resultSeq = 14;
        }
        break;
    case 16:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->resultSeq = 19;
        }
        break;
    case 17:
        func_ov107_021f1898(wk->msg);
        func_ov107_021f1628(wk->msg, 5);
        func_ov107_021f1704(wk->msg, 0x18, func_02012ffc());
        wk->resultSeq = 18;
        break;
    case 18:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->resultSeq = 19;
        }
        break;
    case 19:
        if (func_ov107_021f1164(wk->comm) == TRUE) {
            if (func_ov107_021f116c(wk->comm) == TRUE) {
                func_ov107_021f1898(wk->msg);
                func_ov107_021f1628(wk->msg, 5);
                func_ov107_021f1704(wk->msg, 0x1f, func_02012ffc());
                wk->resultSeq = 20;
            } else {
                wk->resultSeq = 25;
            }
        }
        break;
    case 20:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->resultSeq = 21;
            func_ov107_021f19e4(wk->msg, 1);
        }
        break;
    case 21: {
        int answer = func_ov107_021f1a2c(wk->msg);

        if (answer == 1 || answer == 2) {
            // The bag is fetched here as for the gift below, but not used
            GameData_GetBag(wk->param->gameData);
            PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
            TrainerCardSave *trainerCard = getTrainerCardDataBlkAddress(wk->param->gameData);

            if (answer == 1 && isOneShotDRObtained(trainerCard, 0, playerInfo) == TRUE) {
                wk->answer = 2;
                wk->resultSeq = 22;
                func_ov107_021f1898(wk->msg);
                func_ov107_021f1628(wk->msg, 1);
                func_ov107_021f1704(wk->msg, 0x25, func_02012ffc());
                func_ov107_021f1b24(wk->msg, 1);
            } else {
                wk->answer = answer;
                wk->resultSeq = 23;
            }
        }
        break;
    }
    case 22:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->resultSeq = 23;
        }
        break;
    case 23: {
        BOOL accept = TRUE;

        if (wk->answer != 1) {
            accept = FALSE;
        }
        if (func_ov107_021f11f0(wk->comm, MB_COMM_CMD_ITEM_ANSWER, accept) == TRUE) {
            if (accept == TRUE) {
                BagSave *bag = GameData_GetBag(wk->param->gameData);
                PlayerInfo *playerInfo = GetGameDataPlayerInfo(wk->param->gameData);
                TrainerCardSave *trainerCard = getTrainerCardDataBlkAddress(wk->param->gameData);

                func_ov107_021f1898(wk->msg);
                func_ov107_021f1628(wk->msg, 1);
                func_ov107_021f1704(wk->msg, 0x21, func_02012ffc());
                func_ov107_021f1b24(wk->msg, 1);
                setOneShotDRObtained(trainerCard, 0, playerInfo);
                BagSave_AddItem(bag, ITEM_LOCK_CAPSULE, 1, wk->heapId);
                wk->receivedItem = TRUE;
                wk->resultSeq = 24;
            } else {
                wk->resultSeq = 25;
            }
        }
        break;
    }
    case 24:
        if (func_ov107_021f1b14(wk->msg) == TRUE) {
            wk->resultSeq = 25;
        }
        break;
    case 25:
        if (wk->receivedMore == TRUE || wk->receivedItem == TRUE) {
            if (func_ov107_021f11f0(wk->comm, MB_COMM_CMD_FINISH, 0) == TRUE) {
                wk->state = MB_PARENT_STATE_WAIT_ACK;
            }
        } else {
            if (func_ov107_021f1118(wk->comm) == 0 && func_ov107_021f116c(wk->comm) == FALSE) {
                func_ov107_021f1898(wk->msg);
                func_ov107_021f1628(wk->msg, 1);
                func_ov107_021f1704(wk->msg, 0x28, func_02012ffc());
                func_ov107_021f1b24(wk->msg, 1);
            }
            wk->state = MB_PARENT_STATE_SEND_END;
        }
        break;
    }
}

// pokebw2's MBParent_CardPulledOut
BOOL func_ov107_021f05dc(void) {
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    if (sTitleOnSub == TRUE) {
        GFL_BGSysSetBGEnabled(4, TRUE);
    } else {
        GFL_BGSysSetBGEnabled(2, TRUE);
    }
    return TRUE;
}

BOOL func_ov107_021f0608(GameProc *proc, u32 *state, void *param, void *work) {
    MBParentParam *mbParam = param;
    MBParentWork *wk;

    if (func_0203cdbc() == FALSE) {
        return FALSE;
    }
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MB_PARENT, 0x130000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(MBParentWork), HEAPID_MB_PARENT);
    if (mbParam == NULL) {
        mbParam = GFL_HeapAllocate(HEAPID_MB_PARENT, sizeof(MBParentParam), FALSE, "mb_parent_sys.c", 2788);
        if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_R) {
            mbParam->startMenu = TRUE;
        } else {
            mbParam->startMenu = FALSE;
            mbParam->gameData = GameData_Create(HEAPID_MB_PARENT);
        }
    }
    if (mbParam->startMenu == TRUE) {
        mbParam->gameData = GameData_Create(HEAPID_MB_PARENT);
    }
    wk->heapId = HEAPID_MB_PARENT;
    wk->param = mbParam;
    wk->netError = FALSE;
    wk->wirelessError = FALSE;
    wk->eventsPaused = FALSE;
    func_ov107_021ee740(wk);
    return TRUE;
}

BOOL func_ov107_021f0694(GameProc *proc, u32 *state, void *param, void *work) {
    MBParentWork *wk = work;
    BOOL netError = wk->netError;
    BOOL startMenu = FALSE;
    BOOL eventsPaused = wk->eventsPaused;
    BOOL wirelessError = wk->wirelessError;

    if (wk->param->startMenu == TRUE) {
        startMenu = TRUE;
    }
    func_ov107_021ee824(wk);
    if (param == NULL) {
        GameData_Free(wk->param->gameData);
        GFL_HeapFree(wk->param);
    } else if (wk->param->startMenu == TRUE) {
        GameData_Free(wk->param->gameData);
        GFL_HeapFree(wk->param);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_MB_PARENT);
    if (netError == TRUE) {
        if (startMenu != FALSE) {
            if (eventsPaused == TRUE || func_0200fb24() == TRUE) {
                func_0200fbb0();
                GFL_NetErrShow(TRUE);
            } else {
                GFL_NetErrShow(FALSE);
            }
        } else if (eventsPaused == TRUE) {
            func_0200fbb0();
            GFL_NetErrShow(TRUE);
        }
    }
    if (wirelessError == TRUE) {
        func_0200f83c();
    }
    if (startMenu == TRUE) {
        GFL_HIDDoSoftReset(0);
    }
    return TRUE;
}



static BOOL func_ov107_021f0728(GameProc *proc, u32 *state, void *param, void *work) {
    MBParentWork *wk = work;

    if (func_ov107_021ee894(wk) == TRUE) {
        return TRUE;
    }
    if (func_0200fb24() && wk->netError == FALSE) {
        func_0200fb38();
        wk->netError = TRUE;
    }
    return FALSE;
}
