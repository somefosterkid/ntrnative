// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_acting.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_item_draw.h"
#include "app/musical/mus_poke_draw.h"
#include "app/musical/sta_act_audience.h"
#include "app/musical/sta_act_bg.h"
#include "app/musical/sta_act_button.h"
#include "app/musical/sta_act_effect.h"
#include "app/musical/sta_act_light.h"
#include "app/musical/sta_act_obj.h"
#include "app/musical/sta_act_poke.h"
#include "app/musical/sta_act_script.h"
#include "app/musical/sta_acting.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "field/musical.h"
#include "field/musical_program.h"
#include "field/musical_stage_sys.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/blact.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "app/musical/mus_item_data.h"
#include "app/musical/musical_system.h"
#include "field/mus_comm_func.h"

// Overlay 209's sta_acting.c: the musical's stage. It plays the program's scripts with the Pokémon, the background,
// the objects, the effects, the lights and the audience, follows the Pokémon in the limelight, and runs the props
// the Pokémon use, the player's from the buttons and the others' at random or over the connection

// The Pokémon in the limelight and the light the stage follows, when there is none
#define STA_ACTING_NO_POKE 4
#define STA_ACTING_NO_LIGHT 4

// The props held in each hand, whose use the other Pokémon plan
#define STA_ACTING_EQUIP_HAND_R 7
#define STA_ACTING_EQUIP_HAND_L 8
// What the communication reports when a Pokémon uses no prop
#define STA_ACTING_EQUIP_NONE 10

// MusicalPoke.unk78 of a Pokémon that uses a hand prop when it appeals, and at random, often or now and then;
// smaller values are the frame to use it on
#define STA_ACTING_USE_ON_APPEAL 0xfffd
#define STA_ACTING_USE_SOMETIMES 0xfffe
#define STA_ACTING_USE_OFTEN 0xffff

// How far the stage scrolls, and the curtain opens
#define STA_ACTING_SCROLL_MAX 256
#define STA_ACTING_CURTAIN_OPEN 0xe0

// The frames a Pokémon stays in the limelight after it uses a prop
#define STA_ACTING_LIGHT_UP_FRAMES 180

enum {
    STA_ACTING_SEQ_FADE_IN,
    STA_ACTING_SEQ_WAIT_FADE_IN,
    STA_ACTING_SEQ_FADE_OUT,
    STA_ACTING_SEQ_WAIT_FADE_OUT,
    STA_ACTING_SEQ_WAIT,
    STA_ACTING_SEQ_START,
    STA_ACTING_SEQ_PLAY,
    STA_ACTING_SEQ_SYNC,
    STA_ACTING_SEQ_END = 0xff,
};

struct StaActing {
    HeapID heapId;
    u16 waitCount;
    u16 scrollOffset;
    u16 scrollTarget;
    u16 curtainOffset;
    // The position of the player's Pokémon
    u8 playerPos;
    // Until the curtain is open, the stage doesn't follow the Pokémon
    BOOL curtainClosed;
    BOOL scrollFixed;
    BOOL scriptScroll;
    ArcTool *arc;
    u32 seq;
    TCB *vblankTcb;
    BOOL strmPlaying;
    BOOL seqPlaying;
    BOOL bgmPlaying;
    MusicalStageParam *param;
    MusPokeDrawSys *pokeDraw;
    MusItemDrawSys *itemDraw;
    StaActBg *bg;
    StaActPokeSys *pokeSys;
    StaActPoke *pokes[4];
    StaActObjSys *objSys;
    StaActObj *objs[5];
    StaActEffectSys *effectSys;
    StaActEffect *effects[8];
    StaActLightSys *lightSys;
    StaActLight *lights[4];
    StaActButton *button;
    G3DCamera *camera;
    BlActScene *blact;
    StaActScriptSys *scriptSys;
    u32 *script;
    StaActAudience *audience;
    StaActEffect *appealEffect;
    u32 unkC0[3];
    TCBExManager *tcbExMgr;
    BmpWin *msgWin;
    Font *font;
    PrintStream *printStream;
    MsgData *msgData;
    StrBuf *msgStr;
    // Whether a message waits in the print queue
    BOOL printing;
    PrintQueue *printQueue;
    // Whether the audience is to look again at what is happening
    BOOL audienceRefresh;
    u8 audienceFocusPos;
    // The Pokémon or the light the stage and the audience follow
    u8 followPos;
    u8 followLight;
    // How many Pokémon have more points than each
    u8 ranks[4];
    // The points each Pokémon won with its props during the show
    u8 bonusPoints[4];
    BOOL useRequest[4];
    u32 useEquip[4];
    // The Pokémon in the limelight after it used a prop, which one, for how long, and whether it scores then
    u8 lightUpPos;
    u8 lightUpEquip;
    u16 lightUpTimer;
    BOOL lightUpScores;
    // The frames until each of the other Pokémon uses a prop, and which
    u16 npcUseWait[4];
    u32 npcUseEquip[4];
    BOOL handPropUsed[4][2];
    // The player's prop to send over the connection
    BOOL commSendPending;
    u32 commSendEquip;
    u32 unk164;
    // The shimmer of the sub screen's palette, between its colors and those of the next palette
    u16 paletteAnimPhase;
    u16 paletteBase[16];
    s8 paletteDiffR[16];
    s8 paletteDiffG[16];
    s8 paletteDiffB[16];
    u16 paletteWork[16];
    void *sound[3];
    // The frames since the last update
    u32 updateCount;
    u32 lastVBlankCount;
};

void func_ov132_021f8994(TCB *tcb, void *work);
void func_ov132_021f899c(StaActing *stage);
void func_ov132_021f8bd4(StaActing *stage);
void func_ov132_021f8d88(StaActing *stage, u32 unused);
void func_ov132_021f8da0(const BGSetup *setup, u8 bg, u8 mode);
void func_ov132_021f8dc4(StaActing *stage);
void func_ov132_021f8e58(StaActing *stage);
void func_ov132_021f8e88(StaActing *stage);
void func_ov132_021f8ea8(StaActing *stage);
void func_ov132_021f90e0(StaActing *stage);
void func_ov132_021f91cc(StaActing *stage);
void func_ov132_021f92cc(StaActing *stage);
void func_ov132_021f9374(StaActing *stage);
void func_ov132_021f94f8(StaActing *stage);
void func_ov132_021f95f0(StaActing *stage);
static void func_ov132_021f9754(StaActing *stage, u8 pos, u32 equip);
static BOOL func_ov132_021f9774(StaActing *stage, u8 pos);
void func_ov132_021f9794(StaActing *stage);
u32 func_ov132_021f99f4(StaActing *stage, u8 pos, u8 equip);
void func_ov132_021f9a48(StaActing *stage);
void func_ov132_021f9cf4(StaActing *stage);
static void func_ov132_021f9d08(StaActing *stage);
void func_ov132_021f9e4c(StaActing *stage);

// Read by func_ov132_021f879c, which runs no script while it is set; nothing sets it
// pokebw2's sStaActingPause
static BOOL data_ov132_021fde80;

const u32 data_ov132_021fda78 = 0x01600002;

static const VecFx32 data_ov132_021fda7c = { 0, FX32_ONE, 0 };
static const VecFx32 data_ov132_021fda88 = { 0, 0, 0 };
static const VecFx32 data_ov132_021fda94 = { 0, 0, FX32_CONST(301) };

static const GXRgb data_ov132_021fdab0[8] = { 0 };

static const BGSetup data_ov132_021fdac0 = {
    0,
    0,
    0x1000,
    0,
    BGRES_512x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7000,
    GX_BG_CHARBASE_0x08000,
    0x8000,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSetup data_ov132_021fdae0 = {
    0,
    0,
    0x1000,
    0,
    BGRES_512x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x6000,
    GX_BG_CHARBASE_0x10000,
    0x8000,
    GX_BG_EXTPLTT_01,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSetup data_ov132_021fdb00 = {
    0,
    0,
    0x1000,
    0,
    BGRES_512x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x5000,
    GX_BG_CHARBASE_0x18000,
    0x8000,
    GX_BG_EXTPLTT_01,
    1,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSysLCDConfig data_ov132_021fdaa0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

static const BGSetup data_ov132_021fdb20 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x4800,
    GX_BG_CHARBASE_0x00000,
    0x1000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSetup data_ov132_021fdb40 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x4800,
    GX_BG_CHARBASE_0x08000,
    0x8000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSetup data_ov132_021fdb60 = {
    0,
    0,
    0x2000,
    0,
    BGRES_512x512,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x5000,
    GX_BG_CHARBASE_0x18000,
    0x8000,
    GX_BG_EXTPLTT_01,
    1,
    GX_BG_AREAOVER_REPEAT,
    FALSE,
};

static const BGSetup data_ov132_021fdb80 = {
    0,
    0,
    0x1000,
    0,
    BGRES_512x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7000,
    GX_BG_CHARBASE_0x10000,
    0x8000,
    GX_BG_EXTPLTT_23,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSysVRAMConfig data_ov132_021fdba0;

// pokebw2's StaActing_Term


// pokebw2's StaActing_Init
StaActing *func_ov132_021f8480(MusicalStageParam *param, HeapID heapId) {
    u8 i;
    StaActing *stage = GFL_HeapAllocate(heapId, sizeof(StaActing), TRUE, "sta_acting.c", 282);

    stage->heapId = heapId;
    stage->param = param;
    stage->waitCount = 0;
    stage->scrollOffset = 127;
    stage->scrollTarget = 128;
    stage->curtainOffset = 0;
    stage->script = NULL;
    stage->seq = STA_ACTING_SEQ_FADE_IN;
    stage->unk164 = 0;
    stage->curtainClosed = TRUE;
    stage->scriptScroll = FALSE;
    stage->scrollFixed = FALSE;
    stage->strmPlaying = FALSE;
    stage->seqPlaying = FALSE;
    stage->bgmPlaying = FALSE;
    stage->vblankTcb = GFL_VBlankTCBAdd(func_ov132_021f8994, stage, 64);
    if (stage->param->comm == NULL) {
        for (i = 0; i < 4; i++) {
            if (stage->param->pokes[i]->owner == 0) {
                stage->playerPos = i;
                break;
            }
        }
        if (i == 4) {
            stage->playerPos = 1;
        }
    } else {
        stage->playerPos = func_ov132_021fd8d4(stage->param->comm);
    }
    for (i = 0; i < 4; i++) {
        stage->bonusPoints[i] = 0;
    }
    func_ov132_021f95f0(stage);
    stage->audienceRefresh = TRUE;
    stage->audienceFocusPos = STA_ACTING_NO_POKE;
    stage->followPos = STA_ACTING_NO_POKE;
    stage->followLight = STA_ACTING_NO_LIGHT;
    stage->lightUpPos = STA_ACTING_NO_POKE;
    stage->lightUpTimer = 0;
    stage->commSendPending = FALSE;
    stage->paletteAnimPhase = 0;
    for (i = 0; i < 4; i++) {
        stage->useRequest[i] = FALSE;
        stage->npcUseWait[i] = 0;
        stage->handPropUsed[i][0] = FALSE;
        stage->handPropUsed[i][1] = FALSE;
    }
    stage->arc = GFL_ArcSysCreateFileHandle(ARCID_MUSICAL, stage->heapId);
    func_ov132_021f899c(stage);
    func_ov132_021f8bd4(stage);
    func_ov132_021f92cc(stage);
    func_ov132_021f8dc4(stage);
    func_ov132_021f8e58(stage);
    func_ov132_021f8e88(stage);
    stage->lightSys = func_ov112_021f6bd8(stage->heapId, stage);
    stage->audience = func_ov132_021f9fec(stage->heapId, stage, stage->param);
    stage->button = func_ov132_021fa5b0(stage->heapId, stage, stage->param->pokes[stage->playerPos]);
    stage->scriptSys = func_ov132_021faaf4(stage->heapId, stage);
    func_ov132_021f9cf4(stage);
    func_0203d34c(FALSE, stage->heapId);
    func_ov132_021f8d88(stage, 0);
    stage->sound[0] = stage->param->ov210->sound[0];
    stage->sound[1] = stage->param->ov210->sound[1];
    stage->sound[2] = stage->param->ov210->sound[2];
    stage->lastVBlankCount = OS_GetVBlankCount();
    return stage;
}

void func_ov132_021f8640(StaActing *stage) {
    if (stage->bgmPlaying == TRUE) {
        func_02005d28();
    }
    if (stage->seqPlaying == TRUE) {
        func_02006504();
    }
    if (stage->strmPlaying == TRUE) {
        func_02006518();
    }
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    G2_SetBlendAlpha(0, 0, 31, 31);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov132_021f9d08(stage);
    func_ov132_021fab58(stage->scriptSys);
    if (stage->msgStr != NULL) {
        GFL_StrBufFree(stage->msgStr);
    }
    func_0201c6ec(stage->printQueue);
    func_0201c4c0(stage->printQueue);
    GFL_MsgDataFree(stage->msgData);
    BmpWin_Free(stage->msgWin);
    GFL_FontFree(stage->font);
    GFL_TCBExMgrFree(stage->tcbExMgr);
    func_ov132_021fa64c(stage->button);
    func_ov132_021fa04c(stage->audience);
    func_ov112_021f6f4c(stage->bg);
    func_ov112_021f6c7c(stage->lightSys);
    func_ov132_021f9e94(stage->effectSys);
    func_ov112_021f6a24(stage->objSys);
    func_ov112_021f7120(stage->pokeSys);
    func_ov112_021f51b0(stage->pokeDraw);
    func_ov112_021f4948(stage->itemDraw);
    func_0204a5ac();
    func_0204ed24(stage->blact);
    GFL_G3DCameraFree(stage->camera);
    GFL_G3DSysFree();
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(7);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_TCBRemove(stage->vblankTcb);
    GFL_ArcToolFree(stage->arc);
    GFL_HeapFree(stage);
}

BOOL func_ov132_021f879c(StaActing *stage) {
    u32 vblankCount = OS_GetVBlankCount();

    stage->updateCount = vblankCount - stage->lastVBlankCount;
    stage->lastVBlankCount = vblankCount;
    switch (stage->seq) {
    case STA_ACTING_SEQ_FADE_IN:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, stage->heapId);
        stage->seq = STA_ACTING_SEQ_WAIT_FADE_IN;
        break;
    case STA_ACTING_SEQ_WAIT_FADE_IN:
        if (GFL_WipeIsFinished() == TRUE) {
            if (stage->param->comm != NULL) {
                stage->seq = STA_ACTING_SEQ_SYNC;
                func_ov132_021fcdec(stage->param->comm, 200);
            } else {
                stage->seq = STA_ACTING_SEQ_WAIT;
            }
        }
        break;
    case STA_ACTING_SEQ_FADE_OUT:
        if (GFL_SndPlayerIsActiveAny() == FALSE) {
            GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, stage->heapId);
            stage->seq = STA_ACTING_SEQ_WAIT_FADE_OUT;
        }
        break;
    case STA_ACTING_SEQ_WAIT_FADE_OUT:
        if (GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    case STA_ACTING_SEQ_WAIT:
        if (stage->waitCount++ >= 60) {
            stage->seq = STA_ACTING_SEQ_START;
        }
        break;
    case STA_ACTING_SEQ_START:
        if (stage->unk164 == 0) {
            stage->script = stage->param->ov210->script;
            func_ov132_021f90e0(stage);
            stage->seq = STA_ACTING_SEQ_PLAY;
        }
        break;
    case STA_ACTING_SEQ_PLAY:
        if (func_ov132_021fad7c(stage->scriptSys) == 0) {
            stage->script = NULL;
            if (stage->unk164 == 0) {
                stage->seq = STA_ACTING_SEQ_FADE_OUT;
            } else {
                stage->seq = STA_ACTING_SEQ_END;
            }
        }
        break;
    case STA_ACTING_SEQ_SYNC:
        if (func_ov132_021fce00(stage->param->comm, 200) == TRUE) {
            stage->seq = STA_ACTING_SEQ_WAIT;
        }
        break;
    }
    func_ov132_021f8ea8(stage);
    func_ov132_021fa6cc(stage->button);
    func_ov132_021f9794(stage);
    if (data_ov132_021fde80 == FALSE) {
        func_ov132_021fabd8(stage->scriptSys);
    }
    func_ov112_021f6f64(stage->bg);
    func_ov112_021f7160(stage->pokeSys);
    func_ov112_021f6a58(stage->objSys);
    func_ov132_021f9ecc(stage->effectSys);
    func_ov132_021f9374(stage);
    func_ov112_021f51e8(stage->pokeDraw);
    func_ov112_021f6cb0(stage->lightSys);
    func_ov132_021f94f8(stage);
    func_ov132_021fa064(stage->audience);
    func_ov132_021f91cc(stage);
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    func_ov112_021f6e74(stage->lightSys);
    func_ov112_021f75a0(stage->pokeSys);
    func_ov112_021f51f4(stage->pokeDraw);
    func_ov112_021f718c(stage->pokeSys);
    func_ov112_021f6f8c(stage->bg);
    BlActScene_Draw(stage->blact, stage->camera, NULL);
    func_ov132_021f9ed4(stage->effectSys);
    func_ov132_021f9e4c(stage);
    GFL_G3DSysReqSwapBuffers();
    func_0204a600();
    if (stage->param->comm != NULL && stage->seq != STA_ACTING_SEQ_FADE_IN &&
        stage->seq != STA_ACTING_SEQ_WAIT_FADE_IN && stage->seq != STA_ACTING_SEQ_FADE_OUT &&
        stage->seq != STA_ACTING_SEQ_WAIT_FADE_OUT && func_0200fb24() != FALSE) {
        stage->seq = STA_ACTING_SEQ_FADE_OUT;
    }
    return FALSE;
}


// pokebw2's StaActing_VBlank
void func_ov132_021f8994(TCB *tcb, void *work) {
    func_0204a648();
}

// pokebw2's StaActing_InitGraphics
void func_ov132_021f899c(StaActing *stage) {
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, WIPE_COLOR_BLACK);
    func_0202223c(1, WIPE_COLOR_BLACK);
    func_02022224(0);
    func_02022224(1);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetVRAMBanks(&data_ov132_021fdba0);
    GFL_BGSysCreate(stage->heapId);
    BmpWin_InitAllocator(stage->heapId);
    GFL_BGSysSetLCDConfig(&data_ov132_021fdaa0);
    func_ov132_021f8da0(&data_ov132_021fdb40, 1, 0);
    func_ov132_021f8da0(&data_ov132_021fdb60, 2, 0);
    func_ov132_021f8da0(&data_ov132_021fdb80, 3, 0);
    func_ov132_021f8da0(&data_ov132_021fdac0, 5, 0);
    func_ov132_021f8da0(&data_ov132_021fdae0, 6, 0);
    func_ov132_021f8da0(&data_ov132_021fdb00, 4, 0);
    func_ov132_021f8da0(&data_ov132_021fdb20, 7, 0);
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_G3DSysCreate(FALSE, 2, FALSE, 4, 0, stage->heapId, NULL);
    GFL_BGSysSet3DBGPriority(3);
    GFL_G3DSysSetSwapBufferParams(0, 1);
    stage->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(12), 0, 0, FX32_CONST(16), FX32_ONE, FX32_CONST(1000),
                            0, &data_ov132_021fda94, &data_ov132_021fda7c, &data_ov132_021fda88, stage->heapId);
    GFL_G3DCameraFlush(stage->camera);
    G3X_SetEdgeColorTable(data_ov132_021fdab0);
    G3X_EdgeMarking(FALSE);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);
    GFL_G3DSysSetSwapBufferParams(0, 0);
    {
        BlActSceneSetup setup = { 128, 128, { FX32_ONE, FX32_ONE, FX32_ONE }, 0, 0, 0, 0, 63, 0 };
        VecFx32 scale = { FX32_CONST(4), FX32_CONST(4), FX32_ONE };

        stage->blact = BlActScene_Create(&setup, stage->heapId);
        BlActScene_SetScale(stage->blact, &scale);
    }
    ClActSys_Create(&data_020a1448, &data_ov132_021fdba0, stage->heapId);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG0, 0, 10);
    GX_SetVisibleWnd(GX_WNDMASK_OW);
    G2_SetWndOBJInsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_PLANEMASK_ALL, TRUE);
}

// pokebw2's StaActing_InitBg
void func_ov132_021f8bd4(StaActing *stage) {
    u16 palette[16];
    u8 i;

    GFL_G2DIOLoadArcNCLR(stage->arc, 2, 0, 0x20, 0x20, 0x20, stage->heapId);
    GFL_BGSysLoadArcNCGRStatic(stage->arc, 8, 3, 0, 0, FALSE, stage->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(stage->arc, 14, 3, 0, 0, FALSE, stage->heapId);
    GFL_G2DIOLoadArcNCLR(stage->arc, 3, 0, 0, 0, 0x20, stage->heapId);
    GFL_BGSysLoadArcNCGRStatic(stage->arc, 9, 2, 0, 0, FALSE, stage->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(stage->arc, 15, 2, 0, 0, FALSE, stage->heapId);
    GFL_G2DIOLoadArcNCLRDefault(stage->arc, 5, 4, 0, 0, stage->heapId);
    GFL_BGSysLoadArcNCGRStatic(stage->arc, 13, 5, 0, 0, FALSE, stage->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(stage->arc, 17, 5, 0, 0, FALSE, stage->heapId);
    GFL_BGSysLoadArcNCGRStatic(stage->arc, 12, 6, 0, 0, FALSE, stage->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(stage->arc, 16, 6, 0, 0, FALSE, stage->heapId);
    GFL_BGSysLoadArcNCGRStatic(stage->arc, 11, 4, 0, 0, FALSE, stage->heapId);
    // The sub screen's first palette shimmers toward its second
    MI_CpuCopy16((void *)HW_DB_BG_PLTT, stage->paletteBase, 0x20);
    MI_CpuCopy16((u16 *)HW_DB_BG_PLTT + 16, palette, 0x20);
    for (i = 0; i < 16; i++) {
        u16 base = stage->paletteBase[i];
        u16 color = palette[i];

        stage->paletteDiffR[i] =
            (u8)((color & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT) - (u8)((base & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT);
        stage->paletteDiffG[i] =
            (u8)((color & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT) - (u8)((base & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT);
        stage->paletteDiffB[i] =
            (u8)((color & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT) - (u8)((base & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT);
    }
    GFL_BGSysLoadScr(3);
    GFL_BGSysLoadScr(5);
    stage->bg = func_ov112_021f6f24(stage->heapId, stage);
}

// pokebw2's StaActing_LoadBg
void func_ov132_021f8d88(StaActing *stage, u32 unused) {
    func_ov112_021f6fb0(stage->bg, func_ov010_02157364(stage->param->program));
}

// pokebw2's StaActing_InitBG
void func_ov132_021f8da0(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's StaActing_InitPoke
void func_ov132_021f8dc4(StaActing *stage) {
    VecFx32 pos = { FX32_CONST(256), FX32_CONST(160), FX32_CONST(170) };
    u8 i;

    stage->pokeDraw = func_ov112_021f5164(stage->heapId);
    func_ov112_021f5438(stage->pokeDraw, 0x20000);
    stage->itemDraw = func_ov112_021f48c0(stage->blact, 36, stage->heapId);
    stage->pokeSys = func_ov112_021f709c(stage->heapId, stage, stage->pokeDraw, stage->itemDraw, stage->blact);
    for (i = 0; i < 4; i++) {
        stage->pokes[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        stage->pokes[i] = func_ov112_021f760c(stage->pokeSys, stage->param->pokes[i]);
        func_ov112_021f8054(stage->pokeSys, stage->pokes[i], &pos);
        pos.z -= FX32_CONST(30);
    }
}

// pokebw2's StaActing_InitObj
void func_ov132_021f8e58(StaActing *stage) {
    // Never read, as in pokebw2's StaActing_InitObj: the code zeroes it all the same
    VecFx32 unused = { 0, 0, 0 };
    int i;

    for (i = 0; i < 5; i++) {
        stage->objs[i] = NULL;
    }
    stage->objSys = func_ov112_021f69e4(stage->heapId, stage->blact);
}

// pokebw2's StaActing_InitEffect
void func_ov132_021f8e88(StaActing *stage) {
    int i;

    stage->effectSys = func_ov132_021f9e50(stage->heapId);
    for (i = 0; i < 8; i++) {
        stage->effects[i] = NULL;
    }
}

// The stage follows the Pokémon in the limelight, or the player's, or the light the audience looks at, unless a
// script scrolls it; left and right scroll it by hand
// pokebw2's StaActing_UpdateScroll
void func_ov132_021f8ea8(StaActing *stage) {
    s16 target;
    VecFx32 pos;

    if (GCTX_HIDGetHeldKeys() & PAD_KEY_RIGHT) {
        if (stage->scrollTarget + 2 < STA_ACTING_SCROLL_MAX) {
            stage->scrollTarget += 2;
        } else {
            stage->scrollTarget = STA_ACTING_SCROLL_MAX;
        }
    }
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_LEFT) {
        if (stage->scrollTarget - 2 > 0) {
            stage->scrollTarget -= 2;
        } else {
            stage->scrollTarget = 0;
        }
    }
    if (!(GCTX_HIDGetHeldKeys() & (PAD_KEY_RIGHT | PAD_KEY_LEFT)) && stage->scriptScroll == FALSE &&
        stage->scrollFixed == FALSE && stage->curtainClosed == FALSE) {
        if (stage->followLight == STA_ACTING_NO_LIGHT) {
            u8 followPos = stage->followPos;

            if (followPos == STA_ACTING_NO_POKE) {
                followPos = stage->playerPos;
            }
            if (stage->pokes[followPos] != NULL) {
                func_ov112_021f8044(stage->pokeSys, stage->pokes[followPos], &pos);
                target = FX_FX32_TO_F32(pos.x) - 128.0f;
            }
        } else {
            func_ov112_021f6f04(stage->lightSys, stage->lights[stage->followLight], &pos);
            target = FX_FX32_TO_F32(pos.x) - 128.0f;
        }
        if (target > STA_ACTING_SCROLL_MAX) {
            target = STA_ACTING_SCROLL_MAX;
        } else if (target < 0) {
            target = 0;
        }
        if (target != stage->scrollOffset) {
            stage->scrollTarget = target;
        }
    }
    if (stage->scrollTarget != stage->scrollOffset) {
        VecFx32 camPos = { 0, 0, FX32_CONST(301) };
        VecFx32 camTarget = { 0, 0, 0 };

        if (stage->scrollOffset > stage->scrollTarget) {
            if (stage->scrollOffset > 4 && stage->scrollOffset - 4 > stage->scrollTarget) {
                stage->scrollOffset -= 4;
            } else {
                stage->scrollOffset = stage->scrollTarget;
            }
        } else if (stage->scrollOffset < stage->scrollTarget) {
            if (stage->scrollOffset + 4 < stage->scrollTarget) {
                stage->scrollOffset += 4;
            } else {
                stage->scrollOffset = stage->scrollTarget;
            }
        }
        camPos.x = FX_F32_TO_FX32(stage->scrollOffset / 16.0f);
        camTarget.x = FX_F32_TO_FX32(stage->scrollOffset / 16.0f);
        GFL_G3DCameraSetLookatPos(stage->camera, &camPos);
        GFL_G3DCameraSetLookatTarget(stage->camera, &camTarget);
        GFL_G3DCameraFlush(stage->camera);
        GFL_BGSysMoveBG(2, BG_MOVE_SET_X, stage->scrollOffset);
        func_ov112_021f7098(stage->bg, stage->scrollOffset);
        func_ov112_021f75dc(stage->pokeSys, stage->scrollOffset);
        func_ov112_021f6af0(stage->objSys, stage->scrollOffset);
        func_ov132_021fa57c(stage->audience, stage->scrollOffset);
    }
    if (stage->curtainOffset == STA_ACTING_CURTAIN_OPEN) {
        stage->curtainClosed = FALSE;
    }
    GFL_BGSysMoveBG(2, BG_MOVE_SET_Y, stage->curtainOffset);
}

static const BGSysVRAMConfig data_ov132_021fdba0 = {
    GX_VRAM_BG_128_D,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_16_F,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_0123_E,  GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// Runs the program's first script, synced with the scripts of the four positions that differ from it
// pokebw2's StaActing_StartScripts
void func_ov132_021f90e0(StaActing *stage) {
    u8 i;
    u32 *script = stage->script;
    u8 indexes[4] = { 2, 3, 4, 5 };

    func_ov132_021fac74(stage->scriptSys, (u8 *)script + script[0], TRUE);
    for (i = 0; i < 4; i++) {
        if (script[0] != script[indexes[i]]) {
            func_ov132_021fac74(stage->scriptSys, (u8 *)stage->script + script[indexes[i]], TRUE);
        }
    }
}

// pokebw2's StaActing_StartAppealScript
void func_ov132_021f9140(StaActing *stage, u8 file, u8 pos) {
    u32 *script = GFL_ArcToolReadHeapNewLZ(stage->arc, file + 55, FALSE, stage->heapId);

    if (stage->param->pokes[pos]->points >= 70) {
        func_ov132_021fad0c(stage->scriptSys, script, 0, 1 << pos);
    } else if (stage->param->pokes[pos]->points >= 40) {
        func_ov132_021fad0c(stage->scriptSys, script, 1, 1 << pos);
    } else {
        func_ov132_021fad0c(stage->scriptSys, script, 2, 1 << pos);
    }
}

// pokebw2's StaActing_StartPokeScript
void func_ov132_021f91a8(StaActing *stage, u8 file, u8 pokeMask) {
    u32 *script = GFL_ArcToolReadHeapNewLZ(stage->arc, file + 75, FALSE, stage->heapId);

    func_ov132_021fad0c(stage->scriptSys, script, 0, pokeMask);
}

// pokebw2's StaActing_UpdatePalette
void func_ov132_021f91cc(StaActing *stage) {
    u8 i;
    fx32 ratio;

    if (stage->paletteAnimPhase + 0x160 >= 0x10000) {
        stage->paletteAnimPhase = stage->paletteAnimPhase + 0x160 - 0x10000;
    } else {
        stage->paletteAnimPhase += 0x160;
    }
    ratio = (FX_SinIdx(stage->paletteAnimPhase) + FX32_ONE) / 2;
    for (i = 0; i < 16; i++) {
        stage->paletteWork[i] = GX_RGB((u8)((stage->paletteBase[i] & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT) +
                                           (s8)((stage->paletteDiffR[i] * ratio) >> FX32_SHIFT),
                                       (u8)((stage->paletteBase[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT) +
                                           (s8)((stage->paletteDiffG[i] * ratio) >> FX32_SHIFT),
                                       (u8)((stage->paletteBase[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT) +
                                           (s8)((stage->paletteDiffB[i] * ratio) >> FX32_SHIFT));
    }
    NNS_GfdRegisterNewVramTransferTask(31, 0, stage->paletteWork, 0x20);
}

void func_ov132_021f92cc(StaActing *stage) {
    stage->msgWin = BmpWin_CreateDynamic(1, 4, 20, 24, 4, 15, 1);
    BmpWin_FlushChar(stage->msgWin);
    BmpWin_FlushMap(stage->msgWin);
    GFL_BGSysLoadScr(1);
    stage->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, stage->heapId);
    stage->msgData = GFL_MsgDataCreateFromHandle(((struct Ov210Work *)stage->param->ov210)->msgArc, stage->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 15 * 0x20, 0x20, stage->heapId);
    stage->tcbExMgr = GFL_TCBExMgrCreate(stage->heapId, stage->heapId, 3, 0x100);
    stage->printStream = NULL;
    stage->msgStr = NULL;
    stage->printing = FALSE;
    stage->printQueue = func_0201c440(stage->heapId);
}


// pokebw2's StaActing_UpdateMessage
void func_ov132_021f9374(StaActing *stage) {
    GFL_TCBExMgrUpdate(stage->tcbExMgr);
    if (stage->printStream != NULL && func_0201ce30(stage->printStream) == 2) {
        func_0201ce48(stage->printStream);
        stage->printStream = NULL;
    }
    if (stage->printing == TRUE) {
        func_0201c4e4(stage->printQueue);
        if (func_0201c6b4(stage->printQueue) == TRUE) {
            BmpWin_FlushChar(stage->msgWin);
            stage->printing = FALSE;
        }
    }
}

// pokebw2's StaActing_PrintMessage
void func_ov132_021f93d8(StaActing *stage, u16 msgId, u32 wait) {
    if (stage->printStream != NULL) {
        func_0201ce48(stage->printStream);
        stage->printStream = NULL;
    }
    if (stage->msgStr != NULL) {
        GFL_StrBufFree(stage->msgStr);
        stage->msgStr = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(stage->msgWin), 0);
    stage->msgStr = GFL_MsgDataLoadStrbufNew(stage->msgData, msgId);
    if (wait != 0xff) {
        stage->printStream =
            func_0201cce4(stage->msgWin, 0, 0, stage->msgStr, stage->font, wait, stage->tcbExMgr, 2, stage->heapId, 0);
    } else {
        func_0201c6fc(stage->printQueue, BmpWin_GetBitmap(stage->msgWin), 0, 0, stage->msgStr, stage->font);
        stage->printing = TRUE;
        GFL_StrBufFree(stage->msgStr);
        stage->msgStr = NULL;
    }
}

// pokebw2's StaActing_ClearMessage
void func_ov132_021f94b0(StaActing *stage) {
    if (stage->printStream != NULL) {
        func_0201ce48(stage->printStream);
        stage->printStream = NULL;
    }
    if (stage->msgStr != NULL) {
        GFL_StrBufFree(stage->msgStr);
        stage->msgStr = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(stage->msgWin), 0);
    BmpWin_FlushChar(stage->msgWin);
}

// The audience cheers the Pokémon in the limelight or in focus, or looks at the only light, or cheers the leaders
// pokebw2's StaActing_UpdateAudience
void func_ov132_021f94f8(StaActing *stage) {
    if (stage->audienceRefresh == TRUE) {
        u8 lightCount = 0;
        u8 light;
        u8 i;

        for (i = 0; i < 4; i++) {
            func_ov132_021fa558(stage->audience, i, FALSE);
        }
        func_ov132_021fa56c(stage->audience, 0xff);
        stage->followPos = STA_ACTING_NO_POKE;
        stage->followLight = STA_ACTING_NO_LIGHT;
        for (i = 0; i < 4; i++) {
            if (stage->lights[i] != NULL) {
                light = i;
                lightCount++;
            }
        }
        if (stage->lightUpPos < STA_ACTING_NO_POKE) {
            stage->followPos = stage->lightUpPos;
            func_ov132_021fa558(stage->audience, stage->lightUpPos, TRUE);
        } else if (stage->audienceFocusPos < STA_ACTING_NO_POKE) {
            stage->followPos = stage->audienceFocusPos;
            func_ov132_021fa558(stage->audience, stage->audienceFocusPos, TRUE);
        } else if (lightCount == 1) {
            stage->followLight = light;
            func_ov132_021fa56c(stage->audience, light);
        } else {
            for (i = 0; i < 4; i++) {
                if (stage->ranks[i] == 0) {
                    func_ov132_021fa558(stage->audience, i, TRUE);
                }
            }
        }
        stage->audienceRefresh = FALSE;
    }
}

// pokebw2's StaActing_SetAudienceFocus
void func_ov132_021f95cc(StaActing *stage, u8 pos, BOOL focus) {
    if (focus == TRUE) {
        stage->audienceFocusPos = pos;
    } else {
        stage->audienceFocusPos = STA_ACTING_NO_POKE;
    }
    stage->audienceRefresh = TRUE;
}

// pokebw2's StaActing_RefreshAudience
void func_ov132_021f95e8(StaActing *stage) {
    stage->audienceRefresh = TRUE;
}

// pokebw2's StaActing_UpdateRanks
void func_ov132_021f95f0(StaActing *stage) {
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        stage->ranks[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        u16 points = stage->param->pokes[i]->points + stage->bonusPoints[i];

        for (j = 0; j < 4; j++) {
            if (i != j && (u16)(stage->param->pokes[j]->points + stage->bonusPoints[j]) > points) {
                stage->ranks[i]++;
            }
        }
    }
}

// pokebw2's StaActing_PlayStrm
void func_ov132_021f9664(StaActing *stage, u16 seq) {
    stage->strmPlaying = TRUE;
    func_02006448(stage->sound[0], stage->sound[1], seq);
}

// pokebw2's StaActing_PlaySeq
void func_ov132_021f9680(StaActing *stage, u16 seq) {
    stage->seqPlaying = TRUE;
    func_020064f4(seq);
}

// pokebw2's StaActing_StopSeq
void func_ov132_021f9690(StaActing *stage) {
    stage->seqPlaying = FALSE;
    func_02006504();
}

// pokebw2's StaActing_PlayWave
void func_ov132_021f969c(StaActing *stage, u16 waveArc, u16 a2, u16 a3) {
    func_020064b8(waveArc, a2, stage->sound[2], a3);
}

// pokebw2's StaActing_PlayBgm
void func_ov132_021f96b0(StaActing *stage, u32 bgm) {
    stage->bgmPlaying = TRUE;
    GFL_SndBGMPlay(bgm, 0xffff);
}

// pokebw2's StaActing_StopBgm
void func_ov132_021f96c4(StaActing *stage) {
    stage->bgmPlaying = FALSE;
    func_02005d28();
}

// pokebw2's StaActing_PlayApplause
void func_ov132_021f96d0(StaActing *stage) {
    u8 i;
    u8 minPoints = 0xff;

    for (i = 0; i < 4; i++) {
        u16 points = stage->param->pokes[i]->points + stage->bonusPoints[i];

        if (minPoints > points) {
            minPoints = points;
        }
    }
    if (minPoints >= 70) {
        GFL_SndSEPlay(SEQ_SE_MSCL_09);
    } else if (minPoints >= 30) {
        GFL_SndSEPlay(SEQ_SE_MSCL_10);
    } else if (minPoints >= 1) {
        GFL_SndSEPlay(SEQ_SE_MSCL_11);
    }
}

// pokebw2's StaActing_UseItem
void func_ov132_021f9730(StaActing *stage, u32 equipPos) {
    if (stage->param->comm == NULL) {
        func_ov132_021f9754(stage, stage->playerPos, equipPos);
    } else {
        stage->commSendPending = TRUE;
        stage->commSendEquip = equipPos;
    }
}

// pokebw2's StaActing_RequestItemUse
static void func_ov132_021f9754(StaActing *stage, u8 pos, u32 equip) {
    stage->useRequest[pos] = TRUE;
    stage->useEquip[pos] = equip;
}

// pokebw2's StaActing_IsUsingItem
BOOL func_ov132_021f9768(StaActing *stage) {
    return func_ov132_021f9774(stage, stage->playerPos);
}

// pokebw2's StaActing_IsPokeUsingItem
static BOOL func_ov132_021f9774(StaActing *stage, u8 pos) {
    if (func_ov112_021f7a80(stage->pokeSys, stage->pokes[pos]) == TRUE) {
        return TRUE;
    }
    return stage->useRequest[pos];
}

// Starts the props the Pokémon asked to use, puts one of them in the limelight, and when its time ends scores it
// pokebw2's StaActing_UpdateItemUse
void func_ov132_021f9794(StaActing *stage) {
    u8 userCount = 0;
    u8 users[4];
    u8 i;

    func_ov132_021f9a48(stage);
    if (stage->param->comm != NULL && stage->commSendPending == TRUE &&
        func_ov132_021fd8c4(stage->param->comm, stage->commSendEquip) == TRUE) {
        stage->commSendPending = FALSE;
    }
    if (stage->param->comm == NULL) {
        for (i = 0; i < 4; i++) {
            if (stage->useRequest[i] == TRUE) {
                func_ov112_021f79a8(stage->pokeSys, stage->pokes[i], stage->useEquip[i]);
                users[userCount] = i;
                userCount++;
                stage->useRequest[i] = FALSE;
                if (stage->useEquip[i] == STA_ACTING_EQUIP_HAND_R) {
                    stage->handPropUsed[i][0] = TRUE;
                } else if (stage->useEquip[i] == STA_ACTING_EQUIP_HAND_L) {
                    stage->handPropUsed[i][1] = TRUE;
                }
                func_ov132_021f9ad8(stage, 2, i);
            }
        }
        if (userCount != 0) {
            if (stage->lightUpPos == STA_ACTING_NO_POKE) {
                stage->lightUpScores = TRUE;
            } else {
                stage->lightUpScores = FALSE;
            }
            stage->lightUpTimer = STA_ACTING_LIGHT_UP_FRAMES;
            stage->lightUpPos = users[GFL_RandomLCAlt(userCount)];
            stage->lightUpEquip = stage->useEquip[stage->lightUpPos];
            stage->audienceRefresh = TRUE;
        }
    } else {
        u8 lightUpPos = func_ov132_021fd9fc(stage->param->comm);

        if (lightUpPos < STA_ACTING_NO_POKE) {
            if (stage->lightUpPos == STA_ACTING_NO_POKE) {
                stage->lightUpScores = TRUE;
            } else {
                stage->lightUpScores = FALSE;
            }
            stage->lightUpTimer = STA_ACTING_LIGHT_UP_FRAMES;
            stage->lightUpPos = lightUpPos;
            stage->lightUpEquip = func_ov132_021fd9a0(stage->param->comm, lightUpPos);
            stage->audienceRefresh = TRUE;
            func_ov132_021fda18(stage->param->comm);
        }
        for (i = 0; i < 4; i++) {
            u8 equip = func_ov132_021fd9a0(stage->param->comm, i);

            if (equip != STA_ACTING_EQUIP_NONE) {
                func_ov112_021f79a8(stage->pokeSys, stage->pokes[i], equip);
                func_ov132_021fd9d0(stage->param->comm, i);
                if (equip == STA_ACTING_EQUIP_HAND_R) {
                    stage->handPropUsed[i][0] = TRUE;
                } else if (equip == STA_ACTING_EQUIP_HAND_L) {
                    stage->handPropUsed[i][1] = TRUE;
                }
                func_ov132_021f9ad8(stage, 2, i);
            }
        }
    }
    if (stage->lightUpTimer != 0) {
        if (stage->lightUpTimer > stage->updateCount) {
            stage->lightUpTimer -= (u16)stage->updateCount;
        } else {
            stage->lightUpTimer = 0;
            if (stage->lightUpScores == TRUE) {
                if (stage->param->comm == NULL) {
                    u32 se = func_ov132_021f99f4(stage, stage->lightUpPos, stage->lightUpEquip);

                    stage->param->pokes[stage->lightUpPos]->unk54[stage->lightUpEquip] = TRUE;
                    GFL_SndSEPlay(se);
                } else if (func_0203d378() == TRUE) {
                    if (func_ov132_021f99f4(stage, stage->lightUpPos, stage->lightUpEquip) == SEQ_SE_MSCL_11) {
                        func_ov132_021fda24(stage->param->comm, stage->lightUpPos, stage->lightUpEquip, 1);
                    } else {
                        func_ov132_021fda24(stage->param->comm, stage->lightUpPos, stage->lightUpEquip, 2);
                    }
                }
            }
            stage->lightUpPos = STA_ACTING_NO_POKE;
            stage->audienceRefresh = TRUE;
        }
    }
}

// Adds the points of a prop to the Pokémon's, and returns the sound of the audience's reaction
// pokebw2's StaActing_ScoreItem
u32 func_ov132_021f99f4(StaActing *stage, u8 pos, u8 equip) {
    u8 points =
        func_ov010_02157394(stage->param->program, func_ov112_021f5140(func_ov132_021f9e0c(stage),
                                                                       stage->param->pokes[pos]->equips[equip].itemId));

    stage->bonusPoints[pos] += points;
    func_ov132_021f95f0(stage);
    if (points >= 7) {
        return SEQ_SE_MSCL_10;
    }
    return SEQ_SE_MSCL_11;
}

// pokebw2's StaActing_UpdateNpcItemUse
void func_ov132_021f9a48(StaActing *stage) {
    u8 i;

    if (stage->param->comm == NULL || func_0203d378() == TRUE) {
        for (i = 0; i < 4; i++) {
            if (stage->npcUseWait[i] != 0) {
                stage->npcUseWait[i]--;
                if (stage->npcUseWait[i] == 0) {
                    if (stage->param->comm == NULL) {
                        func_ov132_021f9754(stage, i, stage->npcUseEquip[i]);
                    } else {
                        func_ov132_021fd974(stage->param->comm, i, stage->npcUseEquip[i]);
                    }
                }
            }
        }
    }
}

static inline u8 StaActing_RandomHand(void) {
    if ((u8)GFL_RandomMTRange(2) == 0) {
        return STA_ACTING_EQUIP_HAND_R;
    }
    return STA_ACTING_EQUIP_HAND_L;
}

// pokebw2's StaActing_SetNpcItemTiming
void func_ov132_021f9ad8(StaActing *stage, u32 timing, u8 pos) {
    u8 i;

    if (stage->param->comm == NULL || func_0203d378() == TRUE) {
        for (i = 0; i < 4; i++) {
            BOOL hand[2] = { FALSE, FALSE };
            MusicalPoke *poke = stage->param->pokes[i];

            if (poke->owner == 2 && func_ov132_021f9774(stage, i) != TRUE && stage->npcUseWait[i] == 0) {
                if (poke->equips[STA_ACTING_EQUIP_HAND_R].itemId != 0xff && stage->handPropUsed[i][0] == FALSE) {
                    hand[0] = TRUE;
                }
                if (poke->equips[STA_ACTING_EQUIP_HAND_L].itemId != 0xff && stage->handPropUsed[i][1] == FALSE) {
                    hand[1] = TRUE;
                }
                if (hand[0] != FALSE || hand[1] != FALSE) {
                    u8 equip;

                    if (hand[0] == TRUE && hand[1] == FALSE) {
                        equip = STA_ACTING_EQUIP_HAND_R;
                    } else if (hand[1] == TRUE && hand[0] == FALSE) {
                        equip = STA_ACTING_EQUIP_HAND_L;
                    } else {
                        equip = StaActing_RandomHand();
                    }
                    switch (timing) {
                    case 1:
                        if (poke->unk78 == STA_ACTING_USE_ON_APPEAL && i == pos) {
                            stage->npcUseWait[i] = 30 + GFL_RandomMTRange(30);
                            stage->npcUseEquip[i] = equip;
                        }
                        break;
                    case 2:
                        if (poke->unk78 == STA_ACTING_USE_OFTEN && GFL_RandomMTRange(100) < 50) {
                            stage->npcUseWait[i] = 60 + GFL_RandomMTRange(60);
                            stage->npcUseEquip[i] = equip;
                        }
                        if (poke->unk78 == STA_ACTING_USE_SOMETIMES && GFL_RandomMTRange(100) < 25) {
                            stage->npcUseWait[i] = 60 + GFL_RandomMTRange(60);
                            stage->npcUseEquip[i] = equip;
                        }
                        break;
                    case 0:
                        if (poke->unk78 != 0 && poke->unk78 < STA_ACTING_USE_ON_APPEAL) {
                            s8 jitter = GFL_RandomMTRange(121) - 60;
                            int wait = poke->unk78 + jitter;

                            if (wait < 0) {
                                stage->npcUseWait[i] = 0;
                            } else {
                                stage->npcUseWait[i] = wait;
                            }
                            stage->npcUseEquip[i] = equip;
                        }
                        break;
                    }
                }
            }
        }
    }
}

// pokebw2's StaActing_InitAppealEffect
void func_ov132_021f9cf4(StaActing *stage) {
    stage->appealEffect = func_ov132_021f9edc(stage->effectSys, 54);
}

// pokebw2's StaActing_TermAppealEffect
static void func_ov132_021f9d08(StaActing *stage) {
    func_ov132_021f9f58(stage->effectSys, stage->appealEffect);
}

// pokebw2's StaActing_StartAppealEffect
void func_ov132_021f9d18(StaActing *stage, u8 pos) {
    VecFx32 pokePos;
    u8 i;

    func_ov112_021f8044(stage->pokeSys, stage->pokes[pos], &pokePos);
    pokePos.x = pokePos.x / 16;
    pokePos.y = (FX32_CONST(192) - (pokePos.y - FX32_CONST(32))) / 16;
    pokePos.z = FX32_CONST(180);
    for (i = 0; i < 3; i++) {
        func_ov132_021f9f74(stage->appealEffect, i, &pokePos);
    }
    GFL_SndSEPlay(SEQ_SE_MSCL_20);
}

// pokebw2's StaActing_EnableButtons
void func_ov132_021f9d80(StaActing *stage) {
    func_ov132_021faab8(stage->button, TRUE);
}

// pokebw2's StaActing_DisableButtons
void func_ov132_021f9d90(StaActing *stage) {
    func_ov132_021faab8(stage->button, FALSE);
}

// pokebw2's StaActing_GetPokeSys
StaActPokeSys *func_ov132_021f9da0(StaActing *stage) {
    return stage->pokeSys;
}

// pokebw2's StaActing_GetPoke
StaActPoke *func_ov132_021f9da4(StaActing *stage, u8 pos) {
    return stage->pokes[pos];
}

// pokebw2's StaActing_GetObjSys
StaActObjSys *func_ov132_021f9dac(StaActing *stage) {
    return stage->objSys;
}

// pokebw2's StaActing_GetObj
StaActObj *func_ov132_021f9db0(StaActing *stage, u8 index) {
    return stage->objs[index];
}

// pokebw2's StaActing_SetObj
void func_ov132_021f9db8(StaActing *stage, StaActObj *obj, u8 index) {
    stage->objs[index] = obj;
}

// pokebw2's StaActing_GetEffectSys
StaActEffectSys *func_ov132_021f9dc0(StaActing *stage) {
    return stage->effectSys;
}

// pokebw2's StaActing_GetEffect
StaActEffect *func_ov132_021f9dc4(StaActing *stage, u8 index) {
    return stage->effects[index];
}

// pokebw2's StaActing_SetEffect
void func_ov132_021f9dcc(StaActing *stage, StaActEffect *effect, u8 index) {
    stage->effects[index] = effect;
}

// pokebw2's StaActing_GetLightSys
StaActLightSys *func_ov132_021f9dd4(StaActing *stage) {
    return stage->lightSys;
}

// pokebw2's StaActing_GetLight
StaActLight *func_ov132_021f9ddc(StaActing *stage, u8 index) {
    return stage->lights[index];
}

// pokebw2's StaActing_SetLight
void func_ov132_021f9de8(StaActing *stage, StaActLight *light, u8 index) {
    stage->lights[index] = light;
}

// pokebw2's StaActing_GetAudience
StaActAudience *func_ov132_021f9df4(StaActing *stage) {
    return stage->audience;
}

// pokebw2's StaActing_GetCurtainOffset
u16 func_ov132_021f9dfc(StaActing *stage) {
    return stage->curtainOffset;
}

// pokebw2's StaActing_SetCurtainOffset
void func_ov132_021f9e00(StaActing *stage, u16 offset) {
    stage->curtainOffset = offset;
}

// pokebw2's StaActing_GetScrollOffset
u16 func_ov132_021f9e04(StaActing *stage) {
    return stage->scrollOffset;
}

// pokebw2's StaActing_SetScrollTarget
void func_ov132_021f9e08(StaActing *stage, u16 target) {
    stage->scrollTarget = target;
}

void *func_ov132_021f9e0c(StaActing *stage) {
    return func_ov112_021f4f10(stage->itemDraw);
}


// pokebw2's StaActing_GetLightUpPoke
u8 func_ov132_021f9e18(StaActing *stage) {
    return stage->lightUpPos;
}

// pokebw2's StaActing_GetPoints
u8 func_ov132_021f9e20(StaActing *stage, u8 pos) {
    return stage->param->pokes[pos]->points;
}

// pokebw2's StaActing_GetBonusPoints
u8 func_ov132_021f9e34(StaActing *stage, u8 pos) {
    return stage->bonusPoints[pos];
}

// pokebw2's StaActing_SetScrollFixed
void func_ov132_021f9e3c(StaActing *stage, BOOL fixed) {
    stage->scrollFixed = fixed;
}

// pokebw2's StaActing_SetScriptScroll
void func_ov132_021f9e40(StaActing *stage, BOOL scroll) {
    stage->scriptScroll = scroll;
}

// pokebw2's StaActing_GetUpdateCount
u32 func_ov132_021f9e44(StaActing *stage) {
    return stage->updateCount;
}

// pokebw2's StaActing_DrawDebug
void func_ov132_021f9e4c(StaActing *stage) {
}
