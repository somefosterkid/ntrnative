// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro_cmd.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/narc_intro.h"
#include "constants/sound.h"
#include "demo/intro.h"
#include "demo/intro_script.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "system/brightness.h"
#include "system/mcss.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "constants/pokemon.h"
#include "save/config.h"
#include "save/save_control_intr.h"

// Runs the intro's scripts (intro_script.c)

// Up to this many commands run at once
#define RUNNING_MAX 8

typedef struct {
    u32 state;
    s32 counter;
    u32 unk8;
} IntroCmdWork;

struct IntroCmd {
    HeapID heapId;
    IntroParam *param;
    IntroMcss *mcss;
    IntroG3d *g3d;
    IntroParticle *particle;
    IntroGraphic *graphic;
    IntroMsg *msg;
    u32 script;
    const IntroCmdEntry *running[RUNNING_MAX];
    IntroCmdWork work[RUNNING_MAX];
    // The next command of the script
    u32 pc;
    SaveControlIntr *saveTask;
    // The screen of the band, which INTRO_CMD_OPEN_BAND copies to BG 2 a few rows at a time
    u16 bandScreen[64 * 32];
    ClActor *obj[3];
    u32 objChars[3];
    u32 objPalette[3];
    u32 objCellAnims[3];
};

// Returns TRUE once the command has ended. The arguments are not const: loads through a const pointer are scheduled
// differently, so the scripts' entries are cast when passed
typedef BOOL (*IntroCmdFunc)(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
typedef BOOL (*IntroCondFunc)(IntroCmd *cmd);
typedef void (*IntroWordFunc)(IntroCmd *cmd, u32 slot);

// Each version has its own palette for the BGs
#ifdef BLACK2
#define BG_PALETTE_FILE 2
#else
#define BG_PALETTE_FILE 1
#endif

// The planes that INTRO_CMD_SET_BRIGHTNESS and the transition darken: all but BG 1, which has the messages
#define BRIGHTNESS_PLANES (GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ)

static BOOL func_ov204_021be108(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be10c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be11c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be17c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be1f0(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be23c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be308(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be314(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be328(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be360(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be374(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be398(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be3a4(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be3b8(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be3cc(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be3dc(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be3ec(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be420(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be430(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be44c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be460(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be478(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be4ac(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be4b8(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be510(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be524(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be53c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be54c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be560(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be578(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be61c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be6a4(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
BOOL func_ov204_021be6b4(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be6e8(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be6f8(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be708(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be770(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be78c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be7a8(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be838(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be864(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be874(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be884(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be898(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be970(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021be984(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021beb00(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
BOOL func_ov204_021bebec(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
BOOL func_ov204_021bed6c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021bedb8(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021bee5c(IntroCmd *cmd, IntroCmdWork *work, s32 *args);
static BOOL func_ov204_021beef0(IntroCmd *cmd);
static void func_ov204_021bef08(IntroCmd *cmd, u32 slot);
static BOOL func_ov204_021bf00c(IntroCmd *cmd, const IntroCmdEntry *entry);
static BOOL func_ov204_021bf030(IntroCmd *cmd);
static void func_ov204_021bf080(IntroCmd *cmd, u8 slot);

static const ClActorSetup sObjSetup = { 128, 72, 0, 0, 1 };

// INTRO_COND_*
static IntroCondFunc data_ov204_021c09d0[] = {
    func_ov204_021beef0,
};

// INTRO_WORD_*
static IntroWordFunc data_ov204_021c09d4[] = {
    func_ov204_021bef08,
};

static IntroCmdFunc data_ov204_021c09d8[] = {
    [INTRO_CMD_NONE] = NULL,
    [INTRO_CMD_GOTO] = func_ov204_021be10c,
    [INTRO_CMD_GOTO_MODE_SCRIPT] = func_ov204_021be11c,
    [INTRO_CMD_YES_NO] = func_ov204_021be17c,
    [INTRO_CMD_IF] = func_ov204_021be1f0,
    [INTRO_CMD_LOAD_GRAPHICS] = func_ov204_021be23c,
    [INTRO_CMD_SET_RESULT] = func_ov204_021be308,
    [INTRO_CMD_WAIT] = func_ov204_021be314,
    [INTRO_CMD_FADE] = func_ov204_021be328,
    [INTRO_CMD_SET_BRIGHTNESS] = func_ov204_021be360,
    [INTRO_CMD_START_BRIGHTNESS_TRANSITION] = func_ov204_021be374,
    [INTRO_CMD_CHECK_BRIGHTNESS] = func_ov204_021be398,
    [INTRO_CMD_BGM_PLAY] = func_ov204_021be3a4,
    [INTRO_CMD_BGM_FADE_OUT] = func_ov204_021be3cc,
    [INTRO_CMD_BGM_FADE_IN] = func_ov204_021be3dc,
    [INTRO_CMD_WAIT_BGM_PLAYING] = func_ov204_021be3b8,
    [INTRO_CMD_SE_PLAY] = func_ov204_021be3ec,
    [INTRO_CMD_SE_STOP] = func_ov204_021be420,
    [INTRO_CMD_WAIT_INPUT] = func_ov204_021be430,
    [INTRO_CMD_LOAD_MESSAGES] = func_ov204_021be44c,
    [INTRO_CMD_SET_WORD] = func_ov204_021be460,
    [INTRO_CMD_MESSAGE] = func_ov204_021be478,
    [INTRO_CMD_CLEAR_MESSAGE] = func_ov204_021be4ac,
    [INTRO_CMD_ADD_SPRITE] = func_ov204_021be4b8,
    [INTRO_CMD_SET_SPRITE_VISIBLE] = func_ov204_021be510,
    [INTRO_CMD_SET_SPRITE_ANIMATION] = func_ov204_021be524,
    [INTRO_CMD_WAIT_SPRITE_ANIMATION] = func_ov204_021be53c,
    [INTRO_CMD_CLEAR_SPRITE_ANIMATION_ENDED] = func_ov204_021be54c,
    [INTRO_CMD_MOVE_SPRITE_X] = func_ov204_021be560,
    [INTRO_CMD_FADE_SPRITE] = func_ov204_021be578,
    [INTRO_CMD_TALK] = func_ov204_021be61c,
    [INTRO_CMD_SET_MSG_LANGUAGE] = func_ov204_021be770,
    [INTRO_CMD_SET_GENDER] = func_ov204_021be78c,
    [INTRO_CMD_POKEMON_APPEAR] = func_ov204_021be7a8,
    [INTRO_CMD_PARTICLES] = func_ov204_021be838,
    [INTRO_CMD_SET_MODEL_VISIBLE] = func_ov204_021be864,
    [INTRO_CMD_SET_MODEL_FRAME] = func_ov204_021be874,
    [INTRO_CMD_OPEN_MODEL] = func_ov204_021be884,
    [INTRO_CMD_CHOOSE_GENDER] = func_ov204_021be898,
    [INTRO_CMD_MODEL_BACK] = func_ov204_021be970,
    [INTRO_CMD_SAVE_START] = func_ov204_021be6a4,
    [INTRO_CMD_SAVE_PAUSE] = func_ov204_021be6b4,
    [INTRO_CMD_SAVE_RESUME] = func_ov204_021be6e8,
    [INTRO_CMD_SAVE_43] = func_ov204_021be6f8,
    [INTRO_CMD_SAVE_WAIT] = func_ov204_021be708,
    [INTRO_CMD_OPEN_BAND] = func_ov204_021be984,
    [INTRO_CMD_CLOSE_BAND] = func_ov204_021beb00,
    [INTRO_CMD_CREATE_OBJ] = func_ov204_021bebec,
    [INTRO_CMD_FREE_OBJ] = func_ov204_021bed6c,
    [INTRO_CMD_OBJ_FADE_IN] = func_ov204_021bedb8,
    [INTRO_CMD_OBJ_FADE_OUT] = func_ov204_021bee5c,
    [INTRO_CMD_NOP] = func_ov204_021be108,
    [INTRO_CMD_END] = NULL,
};

// pokebw2's IntroCmd_Nop
static BOOL func_ov204_021be108(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    return TRUE;
}

// pokebw2's IntroCmd_GoTo
static BOOL func_ov204_021be10c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    cmd->script = args[0];
    cmd->pc = 0;
    return TRUE;
}

// Mode 0 starts from the beginning too
// pokebw2's IntroCmd_GoToModeScript
static BOOL func_ov204_021be11c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    s32 script = cmd->param->mode;

    if (script == 0) {
        script++;
    }
    func_ov204_021be10c(cmd, NULL, &script);
    return TRUE;
}

// Starts the command after the question for yes, or the one after that for no, and goes on past both
// pokebw2's IntroCmd_FinishYesNo
static void func_ov204_021be138(IntroCmd *cmd, s32 choice) {
    const IntroCmdEntry *entry = &func_ov204_021bf094(cmd->script)[cmd->pc];

    func_ov204_021bf4e8(cmd->msg);
    if (choice == 0) {
        if (entry != NULL) {
            func_ov204_021bf00c(cmd, entry);
        }
    } else if (++entry != NULL) {
        func_ov204_021bf00c(cmd, entry);
    }
    cmd->pc += 2;
}

// pokebw2's IntroCmd_YesNo
static BOOL func_ov204_021be17c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    IntroMenuItem items[2];
    s32 choice;
    u32 result;

    switch (work->state) {
    case 0:
        items[0].message = args[0];
        items[0].value = 0;
        items[1].message = args[1];
        items[1].value = 1;
        func_ov204_021bf384(cmd->msg, items, 2, args[2]);
        work->state++;
        break;
    case 1:
        func_ov204_021bf528(cmd->msg);
        result = func_ov204_021bf538(cmd->msg, &choice);
        if (result == INTRO_MENU_CHOSEN) {
            func_ov204_021be138(cmd, choice);
            return TRUE;
        } else if (result == INTRO_MENU_CANCELLED) {
            func_ov204_021be138(cmd, 1);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Starts the next command if the condition holds, or the one after it if not, and goes on past both
// pokebw2's IntroCmd_If
static BOOL func_ov204_021be1f0(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    const IntroCmdEntry *entry = &func_ov204_021bf094(cmd->script)[cmd->pc];

    if (!data_ov204_021c09d0[args[0]](cmd)) {
        entry++;
    }
    if (entry != NULL) {
        func_ov204_021bf00c(cmd, entry);
    }
    cmd->pc += 2;
    return TRUE;
}

// pokebw2's IntroCmd_LoadGraphics
BOOL func_ov204_021be23c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    HeapID heapId = cmd->heapId;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_INTRO, heapId);
    NNSG2dScreenData *screen;
    void *file;

    GFL_G2DIOLoadArcNCLRDefault(arc, BG_PALETTE_FILE, 0, 0, 0x40, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, BG_PALETTE_FILE, 4, 0, 0x20, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 6, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 3, 3, 0, 0, FALSE, heapId);
    if (args[0] == TRUE) {
        GFL_BGSysLoadArcNCGRStatic(arc, 12, 2, 0, 0, FALSE, heapId);
        file = GFL_G2DIOReadNSCRArc(arc, 14, FALSE, &screen, heapId);
        MIi_CpuCopy16(screen->rawData, cmd->bandScreen, sizeof(cmd->bandScreen));
        GFL_HeapFree(file);
    }
    GFL_ArcToolFree(arc);
    return TRUE;
}

// pokebw2's IntroCmd_SetResult
static BOOL func_ov204_021be308(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    cmd->param->result = args[0];
    return TRUE;
}

// pokebw2's IntroCmd_Wait
static BOOL func_ov204_021be314(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    if (work->counter > args[0]) {
        return TRUE;
    }
    work->counter++;
    return FALSE;
}

// pokebw2's IntroCmd_Fade
static BOOL func_ov204_021be328(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        GFL_FadeSet(args[0], args[1], args[2], args[3]);
        work->state++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_SetBrightness
static BOOL func_ov204_021be360(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_02023f60(args[0], BRIGHTNESS_PLANES, BRIGHTNESS_MAIN_SCREEN);
    return TRUE;
}

// pokebw2's IntroCmd_StartBrightnessTransition
static BOOL func_ov204_021be374(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_02023ef4(args[0], args[1], args[2], BRIGHTNESS_PLANES, BRIGHTNESS_MAIN_SCREEN);
    return TRUE;
}

// pokebw2's IntroCmd_CheckBrightness
static BOOL func_ov204_021be398(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_02024004(BRIGHTNESS_MAIN_SCREEN);
    return TRUE;
}

// pokebw2's IntroCmd_BGMPlay
static BOOL func_ov204_021be3a4(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    GFL_SndBGMPlay(args[0], SND_CHANNEL_MASK_ALL);
    return TRUE;
}

// pokebw2's IntroCmd_WaitBGMPlaying
static BOOL func_ov204_021be3b8(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    if (GFL_SndBGMIsPlaying()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IntroCmd_BGMFadeOut
static BOOL func_ov204_021be3cc(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    GFL_SndBGMFadeOut(args[0]);
    return TRUE;
}

// pokebw2's IntroCmd_BGMFadeIn
static BOOL func_ov204_021be3dc(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    GFL_SndBGMFadeIn(args[0]);
    return TRUE;
}

// pokebw2's IntroCmd_SEPlay
static BOOL func_ov204_021be3ec(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    s32 player;

    GFL_SndSEPlay(args[0]);
    player = GFL_SndSeqGetPlayerIndex(args[0]);
    if (args[1] != 0) {
        GFL_SndPlayerSetVolume(player, args[1]);
    }
    if (args[2] != 0) {
        GFL_SndPlayerSetParams(player, -1, -1, args[2]);
    }
    return TRUE;
}

// pokebw2's IntroCmd_SEStop
static BOOL func_ov204_021be420(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    GFL_SndPlayerStop(GFL_SndSeqGetPlayerIndex(args[0]));
    return TRUE;
}

// pokebw2's IntroCmd_WaitInput
static BOOL func_ov204_021be430(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    if (func_02035c60() || GCTX_HIDGetPressedKeys()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IntroCmd_LoadMessages
static BOOL func_ov204_021be44c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bf204(cmd->msg, args[0], args[1]);
    return TRUE;
}

// pokebw2's IntroCmd_SetWord
static BOOL func_ov204_021be460(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    data_ov204_021c09d4[args[0]](cmd, args[1]);
    return TRUE;
}

// pokebw2's IntroCmd_Message
static BOOL func_ov204_021be478(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        func_ov204_021bf23c(cmd->msg, args[0], args[1]);
        work->state++;
        break;
    case 1:
        if (func_ov204_021bf2f4(cmd->msg)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_ClearMessage
static BOOL func_ov204_021be4ac(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bf2d4(cmd->msg);
    return TRUE;
}

static const MCSSLoadInfo data_ov204_021bffe4 = { 0xaa, 0x1f, 0x22, 0x21, 0x1e, 0x24, 0x23, 0x20, 0 };

// pokebw2's IntroCmd_AddSprite
static BOOL func_ov204_021be4b8(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    if (args[1] == 0) {
        func_ov204_021bf624(cmd->mcss, args[2], args[3], 0, &data_ov204_021bffe4, args[0]);
        func_ov204_021bf7cc(cmd->mcss, args[0], TRUE);
    } else {
        func_ov204_021bf67c(cmd->mcss, args[2], args[3], FX32_ONE, args[1], args[0]);
    }
    return TRUE;
}

// pokebw2's IntroCmd_SetSpriteVisible
static BOOL func_ov204_021be510(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bf720(cmd->mcss, args[1], args[0]);
    return TRUE;
}

// pokebw2's IntroCmd_SetSpriteAnimation
static BOOL func_ov204_021be524(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bf744(cmd->mcss, args[0], args[1], args[2]);
    return TRUE;
}

// pokebw2's IntroCmd_WaitSpriteAnimation
static BOOL func_ov204_021be53c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    return func_ov204_021bf77c(cmd->mcss, args[0]);
}

// pokebw2's IntroCmd_ClearSpriteAnimationEnded
static BOOL func_ov204_021be54c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bf788(cmd->mcss, args[0]);
    return TRUE;
}

// pokebw2's IntroCmd_MoveSpriteX
static BOOL func_ov204_021be560(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    return func_ov204_021bf804(cmd->mcss, args[0], args[1], args[2]);
}

// pokebw2's IntroCmd_FadeSprite
static BOOL func_ov204_021be578(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        if (args[1]) {
            func_ov204_021bf794(cmd->mcss, args[0], 1);
        } else {
            func_ov204_021bf794(cmd->mcss, args[0], 31);
        }
        func_ov204_021bf720(cmd->mcss, TRUE, args[0]);
        work->counter = 31;
        work->state++;
        break;
    case 1:
        work->counter -= 2;
        if (work->counter <= 0) {
            func_ov204_021bf794(cmd->mcss, args[0], 31);
            func_ov204_021bf720(cmd->mcss, args[1], args[0]);
            return TRUE;
        }
        if (args[1] == TRUE) {
            func_ov204_021bf794(cmd->mcss, args[0], (u8)(31 - work->counter));
        } else if (args[1] == FALSE) {
            func_ov204_021bf794(cmd->mcss, args[0], (u8)work->counter);
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_Talk
static BOOL func_ov204_021be61c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    s32 message = args[0];
    s32 sprite = args[1];

    switch (work->state) {
    case 0:
        func_ov204_021bf23c(cmd->msg, message, args[2]);
        work->state++;
        break;
    case 1:
        if (func_ov204_021bf2f4(cmd->msg)) {
            func_ov204_021bf7cc(cmd->mcss, sprite, TRUE);
            return TRUE;
        }
        switch (func_ov204_021bf2e0(cmd->msg)) {
        case PRINT_STREAM_PAUSED:
            func_ov204_021bf7cc(cmd->mcss, sprite, TRUE);
            work->counter = 0;
            break;
        case PRINT_STREAM_RUNNING:
            func_ov204_021bf7cc(cmd->mcss, sprite, work->counter % 12 < 6 ? TRUE : FALSE);
            work->counter++;
            break;
        case PRINT_STREAM_DONE:
            break;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_SaveStart
static BOOL func_ov204_021be6a4(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov088_021bd2b0(cmd->saveTask);
    return TRUE;
}

BOOL func_ov204_021be6b4(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        func_ov088_021bd368(cmd->saveTask);
        work->state++;
        break;
    case 1:
        if (func_ov088_021bd380(cmd->saveTask) == 1) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// pokebw2's IntroCmd_SaveResume
static BOOL func_ov204_021be6e8(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov088_021bd374(cmd->saveTask);
    return TRUE;
}

// pokebw2's IntroCmd_Save43
static BOOL func_ov204_021be6f8(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov088_021bd39c(cmd->saveTask);
    return TRUE;
}

// pokebw2's IntroCmd_SaveWait
static BOOL func_ov204_021be708(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        if (func_ov088_021bd3a0(cmd->saveTask) == TRUE) {
            return TRUE;
        }
        // The message shown while the first save is made
        func_ov204_021bf23c(cmd->msg, 5, FALSE);
        work->state++;
        break;
    case 1:
        if (func_ov204_021bf2f4(cmd->msg) == TRUE) {
            func_ov204_021bf560(cmd->msg);
            work->state++;
        }
        break;
    case 2:
        if (func_ov088_021bd3a0(cmd->saveTask) == TRUE) {
            func_ov204_021bf580(cmd->msg);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_SetMsgLanguage
static BOOL func_ov204_021be770(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    Config *config = cmd->param->config;

    if (args[0] == FALSE) {
        func_020083ec(config, FALSE);
    } else {
        func_020083ec(config, TRUE);
    }
    return TRUE;
}

// pokebw2's IntroCmd_SetGender
static BOOL func_ov204_021be78c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    PlayerInfo *info = cmd->param->playerInfo;

    if (args[0] == 0) {
        setTrainerGender(info, GENDER_MALE);
    } else {
        setTrainerGender(info, GENDER_FEMALE);
    }
    return TRUE;
}

// The Pokémon drops to its place, then plays its animation and cries
// pokebw2's IntroCmd_PokemonAppear
static BOOL func_ov204_021be7a8(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        if (work->counter == 3) {
            work->counter = 0;
            work->state++;
        } else {
            work->counter++;
        }
        break;
    case 1:
        if (func_ov204_021bf864(cmd->mcss, FX32_ONE, FX32_CONST(-11.5)) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_TOUJOU_INTRO);
            work->state++;
        }
        break;
    case 2:
        if (!GFL_SndPlayerIsActiveAny()) {
            func_ov204_021bf744(cmd->mcss, INTRO_SPRITE_POKEMON, 0, TRUE);
            PokeVoice_StartPlayback(cmd->param->pokeVoice);
            work->state++;
        }
        break;
    case 3:
        if (!PokeVoice_IsPlaying(cmd->param->pokeVoice)) {
            work->state = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_Particles
static BOOL func_ov204_021be838(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    G2_SetBlendAlpha(1, 9, 0, 0);
    func_ov204_021bfce8(cmd->particle, args[0], args[1], 0);
    return TRUE;
}

// pokebw2's IntroCmd_SetModelVisible
static BOOL func_ov204_021be864(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bfae4(cmd->g3d, args[0]);
    return TRUE;
}

// pokebw2's IntroCmd_SetModelFrame
static BOOL func_ov204_021be874(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    func_ov204_021bfbc8(cmd->g3d, args[0]);
    return TRUE;
}

// pokebw2's IntroCmd_OpenModel
static BOOL func_ov204_021be884(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    if (func_ov204_021bfa40(cmd->g3d)) {
        return TRUE;
    }
    return FALSE;
}

// The model turns left for the boy and right for the girl. work->counter is the choice, 0 for the boy
// pokebw2's IntroCmd_ChooseGender
static BOOL func_ov204_021be898(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        func_ov204_021bfae8(cmd->g3d, 0);
        work->state = 4;
        break;
    case 1:
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
            if (work->counter == 1) {
                break;
            }
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov204_021bfae8(cmd->g3d, 2);
            work->counter = 1;
            work->state = 4;
        } else if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
            if (work->counter == 0) {
                break;
            }
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov204_021bfae8(cmd->g3d, 1);
            work->counter = 0;
            work->state = 4;
        } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            GFL_SndSEPlay(SEQ_SE_DECIDE2);
            work->state++;
        }
        break;
    case 2: {
        PlayerInfo *info = cmd->param->playerInfo;

        if (work->counter == 0) {
            setTrainerGender(info, GENDER_MALE);
            func_ov204_021bfae8(cmd->g3d, 3);
        } else {
            setTrainerGender(info, GENDER_FEMALE);
            func_ov204_021bfae8(cmd->g3d, 4);
        }
        work->state++;
        break;
    }
    case 3:
        if (func_ov204_021bfb2c(cmd->g3d)) {
            return TRUE;
        }
        break;
    case 4:
        if (func_ov204_021bfb2c(cmd->g3d)) {
            work->state = 1;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_ModelBack
static BOOL func_ov204_021be970(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    if (func_ov204_021bfba0(cmd->g3d)) {
        return TRUE;
    }
    return FALSE;
}

// Copies the band to BG 2 from its middle rows outwards, two rows a frame
// pokebw2's IntroCmd_OpenBand
static BOOL func_ov204_021be984(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysLoadScrAreaLarge(2, 0, 8, 64, 2, cmd->bandScreen, 0, 8, 64, 32);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 1:
        GFL_BGSysLoadScrAreaLarge(2, 0, 7, 64, 1, cmd->bandScreen, 0, 7, 64, 32);
        GFL_BGSysLoadScrAreaLarge(2, 0, 10, 64, 1, cmd->bandScreen, 0, 10, 64, 32);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 2:
        GFL_BGSysLoadScrAreaLarge(2, 0, 6, 64, 1, cmd->bandScreen, 0, 6, 64, 32);
        GFL_BGSysLoadScrAreaLarge(2, 0, 11, 64, 1, cmd->bandScreen, 0, 11, 64, 32);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 3:
        GFL_BGSysLoadScrAreaLarge(2, 0, 5, 64, 1, cmd->bandScreen, 0, 5, 64, 32);
        GFL_BGSysLoadScrAreaLarge(2, 0, 12, 64, 1, cmd->bandScreen, 0, 12, 64, 32);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 4:
        GFL_BGSysLoadScrAreaLarge(2, 0, 4, 64, 1, cmd->bandScreen, 0, 4, 64, 32);
        GFL_BGSysLoadScrAreaLarge(2, 0, 13, 64, 1, cmd->bandScreen, 0, 13, 64, 32);
        GFL_BGSysQueueScrLoad(2);
        work->state = 0;
        return TRUE;
    }
    return FALSE;
}

// Clears BG 2 two rows a frame, then hides it. The rows do not all match the ones that INTRO_CMD_OPEN_BAND fills
// pokebw2's IntroCmd_CloseBand
static BOOL func_ov204_021beb00(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    switch (work->state) {
    case 0:
        GFL_BGSysFillScrArea(2, 0, 0, 7, 64, 1, 0);
        GFL_BGSysFillScrArea(2, 0, 0, 16, 64, 1, 0);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 1:
        GFL_BGSysFillScrArea(2, 0, 0, 8, 64, 1, 0);
        GFL_BGSysFillScrArea(2, 0, 0, 15, 64, 1, 0);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 2:
        GFL_BGSysFillScrArea(2, 0, 0, 6, 64, 1, 0);
        GFL_BGSysFillScrArea(2, 0, 0, 14, 64, 1, 0);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 3:
        GFL_BGSysFillScrArea(2, 0, 0, 5, 64, 1, 0);
        GFL_BGSysFillScrArea(2, 0, 0, 13, 64, 1, 0);
        GFL_BGSysQueueScrLoad(2);
        work->state++;
        break;
    case 4:
        GFL_BGSysSetBGEnabled(2, FALSE);
        work->state = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov204_021bebec(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    ArcTool *arc;
    u32 i;

    arc = GFL_ArcSysCreateFileHandle(ARCID_INTRO, cmd->heapId);
    if (getTrainerGender(cmd->param->playerInfo) == 0) {
        cmd->objChars[0] = func_0204a6c8(arc, 6, FALSE, 0, cmd->heapId);
        cmd->objPalette[0] = func_0204abf0(arc, 7, 0, 0, cmd->heapId);
        cmd->objCellAnims[0] = func_0204af28(arc, 5, 4, cmd->heapId);
    } else {
        cmd->objChars[0] = func_0204a6c8(arc, 10, FALSE, 0, cmd->heapId);
        cmd->objPalette[0] = func_0204abf0(arc, 11, 0, 0, cmd->heapId);
        cmd->objCellAnims[0] = func_0204af28(arc, 9, 8, cmd->heapId);
    }
    cmd->objChars[1] = func_0204a6c8(arc, 0x11, FALSE, 0, cmd->heapId);
    cmd->objPalette[1] = func_0204abf0(arc, 0x12, 0, 0x20, cmd->heapId);
    cmd->objCellAnims[1] = func_0204af28(arc, 0x10, 0xf, cmd->heapId);
    cmd->objChars[2] = func_0204a6c8(arc, 0x15, FALSE, 0, cmd->heapId);
    cmd->objPalette[2] = func_0204abf0(arc, 0x16, 0, 0x40, cmd->heapId);
    cmd->objCellAnims[2] = func_0204af28(arc, 0x14, 0x13, cmd->heapId);
    GFL_ArcToolFree(arc);

    {
        // The actors of the three objects
        ClActorSetup setup[3] = {
            { 128, 72, 0, 0, 1 },
            { 64, 72, 0, 10, 1 },
            { 192, 72, 0, 10, 1 },
        };

        for (i = 0; i < 3; i++) {
            cmd->obj[i] = func_0204b294(func_ov204_021bdd78(cmd->graphic), cmd->objChars[i], cmd->objPalette[i], cmd->objCellAnims[i], &setup[i], 0, cmd->heapId);
            func_0204b3dc(cmd->obj[i], FALSE);
        }
    }
    return TRUE;
}


// pokebw2's IntroCmd_FreeObj
BOOL func_ov204_021bed6c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    u32 i;

    for (i = 0; i < 3; i++) {
        func_0204a8d4(cmd->objChars[i]);
        func_0204ada4(cmd->objPalette[i]);
        func_0204afd8(cmd->objCellAnims[i]);
        func_0204b3b4(cmd->obj[i]);
    }
    return TRUE;
}

// Fades the actor in over 16 frames with the blend registers
// pokebw2's IntroCmd_ObjFadeIn
BOOL func_ov204_021bedb8(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    u32 i;

    switch (work->state) {
    case 0:
        G2_SetBlendAlpha(0, 12, 0, 16);
        func_0204b688(cmd->obj[args[0]], GX_OAM_MODE_XLU);
        func_0204b3dc(cmd->obj[args[0]], TRUE);
        work->state++;
        break;
    case 16:
        for (i = 0; i < 3; i++) {
            if (func_0204b6e4(cmd->obj[i]) == GX_OAM_MODE_XLU) {
                func_0204b688(cmd->obj[i], GX_OAM_MODE_NORMAL);
            }
        }
        work->state = 0;
        return TRUE;
    default:
        work->state++;
        reg_G2_BLDALPHA = work->state | ((16 - work->state) << 8);
        break;
    }
    return FALSE;
}

// pokebw2's IntroCmd_ObjFadeOut
BOOL func_ov204_021bee5c(IntroCmd *cmd, IntroCmdWork *work, s32 *args) {
    u32 i;

    switch (work->state) {
    case 0:
        G2_SetBlendAlpha(0, 12, 16, 0);
        for (i = 0; i < 3; i++) {
            func_0204b688(cmd->obj[i], GX_OAM_MODE_XLU);
        }
        work->state++;
        break;
    case 16:
        for (i = 0; i < 3; i++) {
            func_0204b688(cmd->obj[i], GX_OAM_MODE_NORMAL);
            func_0204b3dc(cmd->obj[i], FALSE);
        }
        work->state = 0;
        return TRUE;
    default:
        work->state++;
        reg_G2_BLDALPHA = (16 - work->state) | (work->state << 8);
        break;
    }
    return FALSE;
}

// pokebw2's IntroCond_PlayerIsMale
static BOOL func_ov204_021beef0(IntroCmd *cmd) {
    if (getTrainerGender(cmd->param->playerInfo) == GENDER_MALE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IntroWord_PlayerName
static void func_ov204_021bef08(IntroCmd *cmd, u32 slot) {
    copyVarForText(func_ov204_021bf55c(cmd->msg), slot, cmd->param->playerInfo);
}

IntroCmd *func_ov204_021bef20(IntroG3d *g3d, IntroParticle *particle, IntroMcss *mcss, IntroParam *param, IntroGraphic *graphic, HeapID heapId) {
    IntroCmd *cmd = GFL_HeapAllocate(heapId, sizeof(IntroCmd), TRUE, "intro_cmd.c", 1778);

    cmd->heapId = heapId;
    cmd->param = param;
    cmd->mcss = mcss;
    cmd->g3d = g3d;
    cmd->particle = particle;
    cmd->graphic = graphic;
    cmd->saveTask = param->saveTask;
    cmd->msg = func_ov204_021bf0a0(heapId);
    return cmd;
}


// pokebw2's IntroCmd_Free
void func_ov204_021bef74(IntroCmd *cmd) {
    func_ov204_021bf1a4(cmd->msg);
    GFL_HeapFree(cmd);
}

// Runs the commands, and when they have all ended, starts the next one, and the ones that run with it. A jump to
// another script happens at once
// pokebw2's IntroCmd_Update
BOOL func_ov204_021bef88(IntroCmd *cmd) {
    int i;
    const IntroCmdEntry *entry;

    func_ov204_021bf228(cmd->msg);
    if (func_ov204_021bf030(cmd) == FALSE) {
        for (i = 0; i < RUNNING_MAX + 1; i++) {
            entry = &func_ov204_021bf094(cmd->script)[cmd->pc];
            if (entry->cmd == INTRO_CMD_GOTO) {
                data_ov204_021c09d8[entry->cmd](cmd, &cmd->work[i], (s32 *)entry->args);
                entry = func_ov204_021bf094(cmd->script);
            }
            if (entry->cmd == INTRO_CMD_END) {
                return FALSE;
            }
            cmd->pc++;
            if (!func_ov204_021bf00c(cmd, entry)) {
                break;
            }
        }
    }
    return TRUE;
}

// Puts a command in a free slot, and returns whether the next one starts with it
// pokebw2's IntroCmd_Start
static BOOL func_ov204_021bf00c(IntroCmd *cmd, const IntroCmdEntry *entry) {
    int i;

    for (i = 0; i < RUNNING_MAX; i++) {
        if (cmd->running[i] == NULL) {
            cmd->running[i] = entry;
            break;
        }
    }
    return entry->runNext;
}

// Returns TRUE while a command is still running
// pokebw2's IntroCmd_RunCommands
static BOOL func_ov204_021bf030(IntroCmd *cmd) {
    BOOL running = FALSE;
    int i;
    const IntroCmdEntry *entry;

    for (i = 0; i < RUNNING_MAX; i++) {
        entry = cmd->running[i];
        if (entry != NULL) {
            if (data_ov204_021c09d8[entry->cmd](cmd, &cmd->work[i], (s32 *)entry->args) == FALSE) {
                running = TRUE;
            } else {
                func_ov204_021bf080(cmd, i);
            }
        }
    }
    return running;
}

// pokebw2's IntroCmd_End
static void func_ov204_021bf080(IntroCmd *cmd, u8 slot) {
    cmd->running[slot] = NULL;
    cmd->work[slot].state = 0;
    cmd->work[slot].counter = 0;
}
