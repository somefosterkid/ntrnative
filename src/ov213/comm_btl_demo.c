// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov305/comm_btl_demo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/comm_btl_demo.h"
#include "app/comm_btl_demo_graphic.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/particle.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g3d.h"
#include "save/player_info.h"
#include "save/records.h"
#include "system/gf_font.h"
#include "pml/poke_party.h"
#include "system/printsys.h"
#include "work_area.h"

// The screen before and after a link battle. The ROM has no name for this file; "comm_btl_demo.c" is a guess from
// "comm_btl_demo_graphic.c", the name of the file after it, as wifi_bsubway.c is to wifi_bsubway_graphic.c.
//
// Before the battle the trainers' names slide in around a versus mark with their parties as rows of Poké Balls. After it the
// balls show what became of each Pokémon, then each side's number of Pokémon left and the result's label (win, lose or draw)

#define TRAINER_MAX 4
#define BALL_MAX 6
// The Poké Balls of a trainer in a battle of four
#define BALL_MAX_MULTI 3
#define SIDE_COUNT 2
// The SPL manager's work, whose emitters and particles hold pointers
#define PARTICLE_WORK_SIZE WORK_AREA_SIZE(0x4800)

// The scenes of the screen, steps of sCommBtlDemoSteps
enum {
    SCENE_START,
    SCENE_RESULT,
    SCENE_DRAW,
    SCENE_COUNT,
};

// The 3D scenes, each a model with two animations
enum {
    G3D_SCENE_SINGLE_START,
    G3D_SCENE_SINGLE_END,
    G3D_SCENE_MULTI_START,
    G3D_SCENE_MULTI_END,
    G3D_SCENE_VS,
    G3D_SCENE_DRAW,
    G3D_SCENE_COUNT,
};

// The files of the screen's archive
enum {
    FILE_BG_CHARS,
    FILE_BG_PALETTE,
    FILE_BG_SCREEN,
    FILE_SUB_BG_CHARS,
    FILE_SUB_BG_PALETTE,
    FILE_SUB_BG_SCREEN,
    FILE_OBJ_ANIMS,
    FILE_OBJ_CELLS,
    FILE_OBJ_CHARS,
    FILE_OBJ_PALETTE,
    FILE_PARTICLES_VS,
    FILE_PARTICLES_VS_COMPETITION,
    FILE_PARTICLES_RESULT,
    FILE_PARTICLES_BALL,
    FILE_BG_COMPETITION_CHARS,
    FILE_BG_COMPETITION_PALETTE,
    FILE_BG_COMPETITION_SCREEN,
    FILE_SINGLE_START_ANIM1,
    FILE_SINGLE_START_ANIM2,
    FILE_SINGLE_START_MODEL,
    FILE_SINGLE_END_ANIM1,
    FILE_SINGLE_END_ANIM2,
    FILE_SINGLE_END_MODEL,
    FILE_MULTI_START_ANIM1,
    FILE_MULTI_START_ANIM2,
    FILE_MULTI_START_MODEL,
    FILE_MULTI_END_ANIM1,
    FILE_MULTI_END_ANIM2,
    FILE_MULTI_END_MODEL,
    FILE_DRAW_ANIM1,
    FILE_DRAW_ANIM2,
    FILE_DRAW_MODEL,
    FILE_VS_ANIM2,
    FILE_VS_ANIM1,
    FILE_VS_MODEL,
};

// The sequences of the OBJ: the numbers 0 to 6 of the Pokémon left, the labels of the result and the Poké Balls
enum {
    ANIM_COUNT_0,
    ANIM_WIN = 7,
    ANIM_LOSE,
    ANIM_DRAW,
    ANIM_BALL_HEALTHY,
    ANIM_BALL_SICK,
    ANIM_BALL_FAINTED,
    ANIM_BALL_NONE,
};

// The labels of the result: one on each side, and the one for a draw in the middle
enum {
    LABEL_SIDE0,
    LABEL_SIDE1,
    LABEL_MIDDLE,
    LABEL_COUNT,
};

// The sounds of sCommBtlDemoSE
enum {
    SE_BALL,
    SE_VS,
    SE_RESULT,
    SE_VS_END,
    SE_DRAW,
    SE_COUNT,
};

typedef struct {
    u32 unk0;
} CommBtlDemoBack;

// The screen's OBJ
typedef struct {
    CommBtlDemoGraphic *graphic;
    u16 heapId;
    UIObjRes res;
} CommBtlDemoObj;

// The 3D scene and the particles in front of it
typedef struct {
    CommBtlDemoGraphic *graphic;
    u16 heapId;
    G3DManager *manager;
    u16 scene;
    u8 animEnded;
    u8 sceneLoaded;
    ParticleSystem *particles;
    u8 particleWork[PARTICLE_WORK_SIZE];
    u8 emitterCount;
} CommBtlDemoG3D;

// A trainer's row of Poké Balls, which appear one by one from a frame that depends on the trainer
typedef struct {
    PokeParty *party;
    CommBtlDemoG3D *g3d;
    u8 type;
    u8 index;
    u8 pkmCount;
    u8 ballCount;
    ClActor *actors[BALL_MAX];
    u32 timer;
    u8 started;
    const u8 *states;
} CommBtlDemoBalls;

typedef struct {
    BattlePlayer *player;
    CommBtlDemoObj *obj;
    CommBtlDemoG3D *g3d;
    Font *font;
    u8 type;
    u8 index;
    u32 gender;
    StrBuf *name;
    BmpWin *nameWindow;
    CommBtlDemoBalls balls;
    u32 timer;
} CommBtlDemoTrainer;

// The numbers of Pokémon left on each side and the labels of the result
typedef struct {
    u32 unk0;
    CommBtlDemoObj *obj;
    CommBtlDemoG3D *g3d;
    u16 counts[SIDE_COUNT];
    ClActor *countActors[SIDE_COUNT];
    ClActor *labels[LABEL_COUNT];
    u16 timer;
    u8 type;
} CommBtlDemoResult;

typedef struct {
    u16 heapId;
    CommBtlDemoParam *param;
    CommBtlDemoBack back;
    CommBtlDemoObj obj;
    CommBtlDemoG3D g3d;
    CommBtlDemoTrainer trainers[TRAINER_MAX];
    CommBtlDemoResult result;
    CommBtlDemoGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    MsgData *msgData;
    UIScene *scene;
    u8 type;
    u32 battleResult;
    int timer;
    int scroll;
} CommBtlDemoWork;

typedef struct {
    u32 palette;
    u32 chars;
    u32 screen;
} CommBtlDemoBackFiles;

BOOL func_ov213_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov213_021b96c0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov213_021b9740(GameProc *proc, u32 *state, void *param, void *work);
void func_ov213_021b9794(CommBtlDemoBack *back, HeapID heapId, BOOL competition);
void func_ov213_021b9830(UIScene *scene, CommBtlDemoWork *wk);
BOOL func_ov213_021b9858(UIScene *scene, void *param);
void func_ov213_021b9864(CommBtlDemoWork *wk);
void func_ov213_021b98a0(CommBtlDemoWork *wk);
u32 func_ov213_021b98dc(u32 se, BOOL competition);
BOOL func_ov213_021b98ec(UIScene *scene, void *param);
BOOL func_ov213_021b9a68(UIScene *scene, void *param);
BOOL func_ov213_021b9a84(UIScene *scene, void *param);
BOOL func_ov213_021b9b0c(UIScene *scene, void *param);
BOOL func_ov213_021b9d4c(UIScene *scene, void *param);
BOOL func_ov213_021b9d80(UIScene *scene, void *param);
BOOL func_ov213_021b9e08(UIScene *scene, void *param);
u16 func_ov213_021b9e28(CommBtlDemoParam *param);
BOOL func_ov213_021b9e50(u8 type);
BOOL func_ov213_021b9e80(u8 type);
u32 func_ov213_021b9eb0(PartyPkm *pkm, u32 state);
void func_ov213_021b9ee4(CommBtlDemoBalls *balls, PokeParty *party, u8 type, u8 index, CommBtlDemoObj *obj,
                                  CommBtlDemoG3D *g3d, const u8 *states);
void func_ov213_021ba014(CommBtlDemoBalls *balls);
void func_ov213_021ba034(CommBtlDemoBalls *balls, u32 start);
void func_ov213_021ba14c(CommBtlDemoBalls *balls);
void func_ov213_021ba1c0(CommBtlDemoBalls *balls);
void func_ov213_021ba1c8(CommBtlDemoBalls *balls, PokeParty *party, const u8 *states, int index);
BmpWin *func_ov213_021ba200(u8 type, u8 index);
void func_ov213_021ba268(CommBtlDemoTrainer *trainer, u8 type, u8 index, BattlePlayer *player,
                                    CommBtlDemoObj *obj, CommBtlDemoG3D *g3d, Font *font);
void func_ov213_021ba2e0(CommBtlDemoTrainer *trainer);
void func_ov213_021ba2f0(CommBtlDemoTrainer *trainer);
void func_ov213_021ba30c(CommBtlDemoTrainer *trainer);
void func_ov213_021ba330(CommBtlDemoTrainer *trainer, Font *font);
void func_ov213_021ba38c(CommBtlDemoWork *wk);
void func_ov213_021ba408(CommBtlDemoWork *wk);
void func_ov213_021ba444(CommBtlDemoWork *wk);
void func_ov213_021ba480(CommBtlDemoWork *wk);
void func_ov213_021ba4bc(GameRecords *records, u32 id, u32 result);
void func_ov213_021ba514(CommBtlDemoWork *wk);
void func_ov213_021ba5a0(CommBtlDemoResult *result, CommBtlDemoObj *obj, CommBtlDemoG3D *g3d, u16 count0,
                                   u16 count1, u8 type);
void func_ov213_021ba624(CommBtlDemoResult *result);
void func_ov213_021ba650(CommBtlDemoResult *result);
void func_ov213_021ba68c(CommBtlDemoResult *result, u32 label, u32 kind);
void func_ov213_021ba6c8(CommBtlDemoObj *obj, CommBtlDemoGraphic *graphic, HeapID heapId);
void func_ov213_021ba710(CommBtlDemoObj *obj);
ClActor *func_ov213_021ba71c(CommBtlDemoObj *obj, s16 x, s16 y, u16 anim);
ClActor *func_ov213_021ba750(CommBtlDemoObj *obj, u32 side, u8 count);
void func_ov213_021ba780(CommBtlDemoG3D *g3d, CommBtlDemoGraphic *graphic, HeapID heapId);
void func_ov213_021ba7b8(CommBtlDemoG3D *g3d);
static void func_ov213_021ba7d0(CommBtlDemoG3D *g3d);
void func_ov213_021ba81c(CommBtlDemoG3D *g3d, u32 file);
void func_ov213_021ba8bc(CommBtlDemoG3D *g3d);
void func_ov213_021ba8d4(CommBtlDemoG3D *g3d);
void func_ov213_021ba8e8(CommBtlDemoG3D *g3d, u16 emitter, const VecFx32 *pos);
void func_ov213_021ba8f4(CommBtlDemoG3D *g3d, const VecFx32 *pos);
BOOL func_ov213_021ba920(G3DActor *actor, u32 plttToMatListIdx, u32 plttNameIdx);
void func_ov213_021ba95c(CommBtlDemoG3D *g3d, u32 scene);
void func_ov213_021ba9ac(CommBtlDemoG3D *g3d);
void func_ov213_021ba9c0(CommBtlDemoG3D *g3d);
BOOL func_ov213_021ba9d0(CommBtlDemoG3D *g3d);
void func_ov213_021baa20(CommBtlDemoParam *param);
u32 func_ov213_021baa8c(BattlePlayer *player);

// The 3D scenes: the model and its two animations, and the actor that plays them
static const G3DSceneResourceSetup sCommBtlDemoSingleStartResources[] = {
    { ARCID_COMM_BTL_DEMO, FILE_SINGLE_START_MODEL, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_SINGLE_START_ANIM1, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_SINGLE_START_ANIM2, 0, 0 },
};

// pokebw2's sCommBtlDemoSingleStartAnims
static const G3DSceneAnimationSetup data_ov213_021baeac[] = { { 1, 0 }, { 2, 0 } };

static const G3DSceneActorSetup sCommBtlDemoSingleStartActor = {
    0, 0, 0, 0, data_ov213_021baeac, NELEMS(data_ov213_021baeac)
};

static const G3DSceneResourceSetup sCommBtlDemoSingleEndResources[] = {
    { ARCID_COMM_BTL_DEMO, FILE_SINGLE_END_MODEL, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_SINGLE_END_ANIM1, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_SINGLE_END_ANIM2, 0, 0 },
};

// pokebw2's sCommBtlDemoSingleEndAnims
static const G3DSceneAnimationSetup data_ov213_021bae94[] = { { 1, 0 }, { 2, 0 } };

static const G3DSceneActorSetup sCommBtlDemoSingleEndActor = {
    0, 0, 0, 0, data_ov213_021bae94, NELEMS(data_ov213_021bae94)
};

static const G3DSceneResourceSetup sCommBtlDemoMultiStartResources[] = {
    { ARCID_COMM_BTL_DEMO, FILE_MULTI_START_MODEL, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_MULTI_START_ANIM1, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_MULTI_START_ANIM2, 0, 0 },
};

// pokebw2's sCommBtlDemoMultiStartAnims
static const G3DSceneAnimationSetup data_ov213_021bae8c[] = { { 1, 0 }, { 2, 0 } };

static const G3DSceneActorSetup sCommBtlDemoMultiStartActor = {
    0, 0, 0, 0, data_ov213_021bae8c, NELEMS(data_ov213_021bae8c)
};

static const G3DSceneResourceSetup sCommBtlDemoMultiEndResources[] = {
    { ARCID_COMM_BTL_DEMO, FILE_MULTI_END_MODEL, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_MULTI_END_ANIM1, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_MULTI_END_ANIM2, 0, 0 },
};

// pokebw2's sCommBtlDemoMultiEndAnims
static const G3DSceneAnimationSetup data_ov213_021bae9c[] = { { 1, 0 }, { 2, 0 } };

static const G3DSceneActorSetup sCommBtlDemoMultiEndActor = {
    0, 0, 0, 0, data_ov213_021bae9c, NELEMS(data_ov213_021bae9c)
};

static const G3DSceneResourceSetup sCommBtlDemoVsResources[] = {
    { ARCID_COMM_BTL_DEMO, FILE_VS_MODEL, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_VS_ANIM1, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_VS_ANIM2, 0, 0 },
};

// pokebw2's sCommBtlDemoVsAnims
static const G3DSceneAnimationSetup data_ov213_021bae84[] = { { 1, 0 }, { 2, 0 } };

static const G3DSceneActorSetup sCommBtlDemoVsActor = { 0, 0, 0, 0, data_ov213_021bae84, NELEMS(data_ov213_021bae84) };

static const G3DSceneResourceSetup sCommBtlDemoDrawResources[] = {
    { ARCID_COMM_BTL_DEMO, FILE_DRAW_MODEL, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_DRAW_ANIM1, 0, 0 },
    { ARCID_COMM_BTL_DEMO, FILE_DRAW_ANIM2, 0, 0 },
};

static const G3DSceneAnimationSetup sCommBtlDemoDrawAnims[] = { { 1, 0 }, { 2, 0 } };

// pokebw2's sCommBtlDemoDrawActor
static const G3DSceneActorSetup data_ov213_021baec4 = {
    0, 0, 0, 0, sCommBtlDemoDrawAnims, NELEMS(sCommBtlDemoDrawAnims)
};

static const G3DSceneSetup sCommBtlDemoScenes[G3D_SCENE_COUNT] = {
    { sCommBtlDemoSingleStartResources, NELEMS(sCommBtlDemoSingleStartResources), &sCommBtlDemoSingleStartActor, 1 },
    { sCommBtlDemoSingleEndResources, NELEMS(sCommBtlDemoSingleEndResources), &sCommBtlDemoSingleEndActor, 1 },
    { sCommBtlDemoMultiStartResources, NELEMS(sCommBtlDemoMultiStartResources), &sCommBtlDemoMultiStartActor, 1 },
    { sCommBtlDemoMultiEndResources, NELEMS(sCommBtlDemoMultiEndResources), &sCommBtlDemoMultiEndActor, 1 },
    { sCommBtlDemoVsResources, NELEMS(sCommBtlDemoVsResources), &sCommBtlDemoVsActor, 1 },
    { sCommBtlDemoDrawResources, NELEMS(sCommBtlDemoDrawResources), &data_ov213_021baec4, 1 },
};

// The sounds, and those of the Wi-Fi competition
static const u32 sCommBtlDemoSE[SE_COUNT][2] = {
    { SEQ_SE_SYS_65, SEQ_SE_SYS_65 },   { SEQ_SE_SYS_66, SEQ_SE_WCS_02 },     { SEQ_SE_SYS_67, SEQ_SE_WCS_03 },
    { SEQ_SE_TDEMO_009, SEQ_SE_WCS_04 }, { SEQ_SE_W235_03, SEQ_SE_W235_03 },
};

static const UISceneStep sCommBtlDemoSteps[SCENE_COUNT] = {
    { func_ov213_021b9858, NULL, func_ov213_021b98ec, NULL, func_ov213_021b9a68 },
    { func_ov213_021b9a84, NULL, func_ov213_021b9b0c, NULL, func_ov213_021b9d4c },
    { NULL, NULL, func_ov213_021b9d80, NULL, func_ov213_021b9e08 },
};

// pokebw2's CommBtlDemoProc_End
// pokebw2's CommBtlDemoProc_Init
// pokebw2's CommBtlDemoProc_Main
const GameProcFunctions data_ov213_021bb0b0 = { func_ov213_021b95a0, func_ov213_021b9740, func_ov213_021b96c0 };

// The background of the main screen: the usual one and the Wi-Fi competition's
// pokebw2's sCommBtlDemoBackFiles
static const CommBtlDemoBackFiles data_ov213_021baf14[] = {
    { FILE_BG_PALETTE, FILE_BG_CHARS, FILE_BG_SCREEN },
    { FILE_BG_COMPETITION_PALETTE, FILE_BG_COMPETITION_CHARS, FILE_BG_COMPETITION_SCREEN },
};



BOOL func_ov213_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    CommBtlDemoParam *prm = param;
    CommBtlDemoWork *wk;

    if (*state == 1 && GFL_FadeIsRunning() == TRUE) {
        return TRUE;
    }
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_COMM_BTL_DEMO, 0x80000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(CommBtlDemoWork), HEAPID_COMM_BTL_DEMO);
    MI_CpuFill8(wk, 0, sizeof(CommBtlDemoWork));
    wk->heapId = HEAPID_COMM_BTL_DEMO;
    wk->param = prm;
    wk->type = prm->battle.unk44;
    wk->battleResult = wk->param->battle.result;
    func_ov213_021baa20(prm);
    wk->graphic = func_ov213_021bab64(0, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_INTRO_BTL_SERVER_FLOW, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    wk->scene = func_ov067_021b6100(wk->heapId, sCommBtlDemoSteps, 3, func_ov213_021b9e28(prm), wk);
    func_ov213_021b9794(&wk->back, wk->heapId, wk->param->competition);
    func_ov213_021ba780(&wk->g3d, wk->graphic, wk->heapId);
    func_ov213_021ba6c8(&wk->obj, wk->graphic, wk->heapId);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    func_0203d34c(FALSE, wk->heapId);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 1);
    *state = 1;
    return FALSE;
}

BOOL func_ov213_021b96c0(GameProc *proc, u32 *state, void *param, void *work) {
    CommBtlDemoParam *prm = param;
    CommBtlDemoWork *wk = work;
    HeapID heapId;

    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_X, 0);
    if (prm->battle.unk44 == COMM_BTL_DEMO_TYPE_SINGLE_END || prm->battle.unk44 == COMM_BTL_DEMO_TYPE_MULTI_END) {
        func_ov213_021ba514(wk);
    }
    func_ov067_021b613c(wk->scene);
    GFL_MsgDataFree(wk->msgData);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov213_021ba710(&wk->obj);
    func_ov213_021ba7b8(&wk->g3d);
    func_ov213_021bac0c(wk->graphic);
    heapId = wk->heapId;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(heapId);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

static BOOL func_ov213_021b9740(GameProc *proc, u32 *state, void *param, void *work) {
    CommBtlDemoWork *wk = work;

    func_ov213_021ba7d0(&wk->g3d);
    if (func_ov067_021b6144(wk->scene)) {
        return TRUE;
    }
    func_0201c4e4(wk->printQueue);
    func_ov213_021bac68(wk->graphic);
    wk->scroll++;
    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_X, wk->scroll / 2);
    return FALSE;
}

// pokebw2's CommBtlDemo_LoadBG
void func_ov213_021b9794(CommBtlDemoBack *back, HeapID heapId, BOOL competition) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_COMM_BTL_DEMO, heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov213_021baf14[competition].palette, PALTYPE_MAIN_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, data_ov213_021baf14[competition].chars, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, data_ov213_021baf14[competition].screen, 3, 0, 0, FALSE, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, FILE_SUB_BG_PALETTE, PALTYPE_SUB_BG, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, FILE_SUB_BG_CHARS, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, FILE_SUB_BG_SCREEN, 6, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
}

void func_ov213_021b9830(UIScene *scene, CommBtlDemoWork *wk) {
    func_ov213_021ba408(wk);
    func_ov213_021ba624(&wk->result);
    func_ov067_021b61f8(scene, UI_SCENE_END);
}


// pokebw2's CommBtlDemoStart_Begin
BOOL func_ov213_021b9858(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;

    func_ov213_021ba38c(wk);
    return TRUE;
}

// Gives the trainers of the scene's model the palette of their sex
// pokebw2's CommBtlDemo_BindSinglePalettes
void func_ov213_021b9864(CommBtlDemoWork *wk) {
    CommBtlDemoG3D *g3d = &wk->g3d;
    G3DActor *actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
    int i;

    for (i = 0; i < 2; i++) {
        func_ov213_021ba920(actor, i + 1, wk->trainers[i].gender == 0 ? 1 : 2);
    }
}

// pokebw2's CommBtlDemo_BindMultiPalettes
void func_ov213_021b98a0(CommBtlDemoWork *wk) {
    CommBtlDemoG3D *g3d = &wk->g3d;
    G3DActor *actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
    int i;

    for (i = 0; i < 4; i++) {
        func_ov213_021ba920(actor, i + 1, wk->trainers[i].gender == 0 ? 1 : 3);
    }
}

// pokebw2's CommBtlDemo_GetSE
u32 func_ov213_021b98dc(u32 se, BOOL competition) {
    return sCommBtlDemoSE[se][competition];
}

BOOL func_ov213_021b98ec(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;
    u8 counter = func_ov067_021b61fc(scene);

    func_ov213_021ba444(wk);
    switch (counter) {
    case 0:
        func_ov213_021ba81c(&wk->g3d, FILE_PARTICLES_BALL);
        if (func_ov213_021b9e50(wk->type)) {
            func_ov213_021ba95c(&wk->g3d, G3D_SCENE_SINGLE_START);
            func_ov213_021b9864(wk);
        } else {
            func_ov213_021ba95c(&wk->g3d, G3D_SCENE_MULTI_START);
            func_ov213_021b98a0(wk);
        }
        func_ov213_021ba480(wk);
        func_ov067_021b6200(scene);
        break;
    case 1:
        if (!func_ov213_021ba9d0(&wk->g3d)) {
            func_ov213_021ba95c(&wk->g3d, G3D_SCENE_VS);
            if (wk->param->competition) {
                func_ov213_021ba81c(&wk->g3d, FILE_PARTICLES_VS_COMPETITION);
            } else {
                func_ov213_021ba81c(&wk->g3d, FILE_PARTICLES_VS);
            }
            wk->timer = 0;
            func_ov067_021b6200(scene);
        }
        break;
    case 2:
        func_ov213_021ba9d0(&wk->g3d);
        if (wk->timer == 15) {
            GFL_SndSEPlay(func_ov213_021b98dc(1, wk->param->competition));
            {
                VecFx32 pos = { 0, 0, -100 };

                func_ov213_021ba8e8(&wk->g3d, 0, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -100 };

                func_ov213_021ba8e8(&wk->g3d, 1, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -100 };

                func_ov213_021ba8e8(&wk->g3d, 2, &pos);
            }
        } else if (wk->timer == 25) {
            VecFx32 pos = { 0, 0, -100 };

            func_ov213_021ba8e8(&wk->g3d, 3, &pos);
        } else if (wk->timer == 160) {
            GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 0, 16, -3);
        } else if (wk->timer == 150) {
            GFL_SndSEPlay(func_ov213_021b98dc(3, wk->param->competition));
        } else if (wk->timer == 168) {
            return TRUE;
        }
        wk->timer++;
        break;
    }
    return FALSE;
}


BOOL func_ov213_021b9a68(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;

    func_ov213_021ba408(wk);
    func_ov067_021b61f8(scene, UI_SCENE_END);
    return TRUE;
}


// pokebw2's CommBtlDemoResult_Begin
BOOL func_ov213_021b9a84(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;
    u16 count0;
    u16 count1;

    func_ov213_021ba38c(wk);
    if (func_ov213_021b9e50(wk->type)) {
        count0 = func_ov213_021baa8c(&wk->param->battle.players[0]);
        count1 = func_ov213_021baa8c(&wk->param->battle.players[1]);
    } else {
        count0 = func_ov213_021baa8c(&wk->param->battle.players[0]);
        count0 += func_ov213_021baa8c(&wk->param->battle.players[1]);
        count1 = func_ov213_021baa8c(&wk->param->battle.players[2]);
        count1 += func_ov213_021baa8c(&wk->param->battle.players[3]);
    }
    func_ov213_021ba5a0(&wk->result, &wk->obj, &wk->g3d, count0, count1, wk->type);
    return TRUE;
}

BOOL func_ov213_021b9b0c(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;
    u8 counter = func_ov067_021b61fc(scene);

    func_ov213_021ba444(wk);
    func_ov213_021ba650(&wk->result);
    switch (counter) {
    case 0:
        if (func_ov213_021b9e50(wk->type)) {
            func_ov213_021ba95c(&wk->g3d, G3D_SCENE_SINGLE_END);
        } else {
            func_ov213_021ba95c(&wk->g3d, G3D_SCENE_MULTI_END);
        }
        func_ov213_021ba81c(&wk->g3d, FILE_PARTICLES_BALL);
        func_ov213_021ba480(wk);
        func_ov067_021b6200(scene);
        break;
    case 1:
        if (!func_ov213_021ba9d0(&wk->g3d)) {
            if (wk->battleResult == COMM_BTL_DEMO_RESULT_DRAW) {
                return TRUE;
            }
            wk->timer = 0;
            func_ov213_021ba81c(&wk->g3d, FILE_PARTICLES_RESULT);
            func_ov067_021b6200(scene);
        }
        break;
    case 2:
        if (wk->timer == 15) {
            u32 side = wk->battleResult == COMM_BTL_DEMO_RESULT_WIN ? 0 : 1;

            GFL_SndSEPlay(func_ov213_021b98dc(2, wk->param->competition));
            func_ov213_021ba68c(&wk->result, side, 0);
        } else if (wk->timer == 24) {
            func_0204b3dc(wk->result.countActors[wk->battleResult != COMM_BTL_DEMO_RESULT_WIN ? 1 : 0], FALSE);
        } else if (wk->timer == 45) {
            func_ov213_021ba68c(&wk->result, wk->battleResult == COMM_BTL_DEMO_RESULT_LOSE ? 0 : 1, 1);
        } else if (wk->timer == 54) {
            func_0204b3dc(wk->result.countActors[wk->battleResult == COMM_BTL_DEMO_RESULT_LOSE ? 0 : 1], FALSE);
        } else if (wk->timer == 120) {
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, -3);
        } else if (wk->timer == 128) {
            return TRUE;
        }
        if (wk->timer % 180 == 0) {
            fx32 x;
            fx32 y;

            if (wk->battleResult == COMM_BTL_DEMO_RESULT_LOSE) {
                x = -0x2800;
                y = 0x2000;
            } else {
                x = 0x2800;
                y = -0x1800;
            }
            {
                VecFx32 pos = { 0, 0, -300 };

                pos.x = x;
                pos.y = y;
                func_ov213_021ba8e8(&wk->g3d, 0, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -300 };

                pos.x = x;
                pos.y = y;
                func_ov213_021ba8e8(&wk->g3d, 1, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -100 };

                pos.x = x;
                pos.y = y;
                func_ov213_021ba8e8(&wk->g3d, 2, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -100 };

                pos.x = x;
                pos.y = y;
                func_ov213_021ba8e8(&wk->g3d, 3, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -100 };

                pos.x = x;
                pos.y = y;
                func_ov213_021ba8e8(&wk->g3d, 4, &pos);
            }
            {
                VecFx32 pos = { 0, 0, -100 };

                pos.x = x;
                pos.y = y;
                func_ov213_021ba8e8(&wk->g3d, 5, &pos);
            }
        }
        wk->timer++;
        break;
    }
    return FALSE;
}


BOOL func_ov213_021b9d4c(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;

    if (wk->battleResult == COMM_BTL_DEMO_RESULT_DRAW) {
        GFL_SndSEPlay(func_ov213_021b98dc(4, wk->param->competition));
        func_ov067_021b61f8(scene, SCENE_DRAW);
        return TRUE;
    }
    func_ov213_021b9830(scene, wk);
    return TRUE;
}

BOOL func_ov213_021b9d80(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;
    u8 counter = func_ov067_021b61fc(scene);

    func_ov213_021ba444(wk);
    func_ov213_021ba650(&wk->result);
    switch (counter) {
    case 0:
        func_ov213_021ba95c(&wk->g3d, G3D_SCENE_DRAW);
        wk->timer = 0;
        func_ov067_021b6200(scene);
        break;
    case 1:
        if (wk->timer++ == 30) {
            func_ov213_021ba68c(&wk->result, 2, 2);
        }
        if (!func_ov213_021ba9d0(&wk->g3d)) {
            GFL_FadeSet(3, 0, 16, -3);
            return TRUE;
        }
        break;
    case 2:
        break;
    }
    return FALSE;
}



// pokebw2's CommBtlDemoDraw_End
BOOL func_ov213_021b9e08(UIScene *scene, void *param) {
    CommBtlDemoWork *wk = param;

    if (GFL_FadeIsRunning() == TRUE) {
        return FALSE;
    }
    func_ov213_021b9830(scene, wk);
    return TRUE;
}

// pokebw2's CommBtlDemo_GetFirstScene
u16 func_ov213_021b9e28(CommBtlDemoParam *param) {
    switch (param->battle.unk44) {
    case COMM_BTL_DEMO_TYPE_SINGLE_START:
    case COMM_BTL_DEMO_TYPE_MULTI_START:
        return SCENE_START;
    case COMM_BTL_DEMO_TYPE_SINGLE_END:
    case COMM_BTL_DEMO_TYPE_MULTI_END:
        return SCENE_RESULT;
    default:
        return SCENE_START;
    }
}

// Whether the battle is one of two players
// pokebw2's CommBtlDemo_IsSingle
BOOL func_ov213_021b9e50(u8 type) {
    switch (type) {
    case COMM_BTL_DEMO_TYPE_SINGLE_START:
        return TRUE;
    case COMM_BTL_DEMO_TYPE_SINGLE_END:
        return TRUE;
    case COMM_BTL_DEMO_TYPE_MULTI_START:
        return FALSE;
    case COMM_BTL_DEMO_TYPE_MULTI_END:
        return FALSE;
    default:
        return TRUE;
    }
}

// Whether the screen is the one before the battle
// pokebw2's CommBtlDemo_IsStart
BOOL func_ov213_021b9e80(u8 type) {
    switch (type) {
    case COMM_BTL_DEMO_TYPE_SINGLE_START:
        return TRUE;
    case COMM_BTL_DEMO_TYPE_SINGLE_END:
        return FALSE;
    case COMM_BTL_DEMO_TYPE_MULTI_START:
        return TRUE;
    case COMM_BTL_DEMO_TYPE_MULTI_END:
        return FALSE;
    default:
        return TRUE;
    }
}

// pokebw2's CommBtlDemo_GetBallAnim
u32 func_ov213_021b9eb0(PartyPkm *pkm, u32 state) {
    if (pkm == NULL) {
        return ANIM_BALL_NONE;
    }
    switch (state) {
    case COMM_BTL_DEMO_POKE_NONE:
        return ANIM_BALL_NONE;
    case COMM_BTL_DEMO_POKE_HEALTHY:
        return ANIM_BALL_HEALTHY;
    case COMM_BTL_DEMO_POKE_SICK:
        return ANIM_BALL_SICK;
    case COMM_BTL_DEMO_POKE_FAINTED:
        return ANIM_BALL_FAINTED;
    }
    return ANIM_BALL_HEALTHY;
}

void func_ov213_021b9ee4(CommBtlDemoBalls *balls, PokeParty *party, u8 type, u8 index, CommBtlDemoObj *obj,
                                  CommBtlDemoG3D *g3d, const u8 *states) {
    int i;

    balls->type = type;
    balls->index = index;
    balls->party = party;
    balls->pkmCount = PokeParty_GetPkmCount(party);
    balls->ballCount = func_ov213_021b9e50(type) ? BALL_MAX : BALL_MAX_MULTI;
    balls->g3d = g3d;
    balls->timer = 0;
    balls->states = states;
    for (i = 0; i < balls->ballCount; i++) {
        s16 x;
        s16 y;
        PartyPkm *pkm;
        u32 anim;

        if (func_ov213_021b9e50(type)) {
            x = i * 16 + 136;
            y = index == 0 ? 168 : 56;
            // The actor's position is a byte, so this is 120 - i * 16
            if (index == 0) {
                x *= -1;
            }
        } else {
            switch (index) {
            case 0:
                x = i * -16 + 80;
                y = 136;
                break;
            case 1:
                x = i * -16 + 144;
                y = 176;
                break;
            case 2:
                x = i * 16 + 128;
                y = 48;
                break;
            case 3:
                x = i * 16 + 192;
                y = 88;
                break;
            }
        }
        pkm = NULL;
        if (i < balls->pkmCount) {
            pkm = PokeParty_GetPkm(party, i);
        }
        anim = func_ov213_021b9eb0(pkm, states[i]);
        // Before the battle, nothing shows what state the Pokémon are in
        if (!func_ov213_021b9e80(type) && anim != ANIM_BALL_NONE) {
            anim = ANIM_BALL_HEALTHY;
        }
        balls->actors[i] = func_ov213_021ba71c(obj, x, y, anim);
        if (func_ov213_021b9e80(type)) {
            func_0204b3dc(balls->actors[i], FALSE);
        } else {
            func_0204b3dc(balls->actors[i], TRUE);
        }
    }
}

// pokebw2's CommBtlDemoBalls_Free
void func_ov213_021ba014(CommBtlDemoBalls *balls) {
    int i;

    for (i = 0; i < balls->ballCount; i++) {
        func_0204b3b4(balls->actors[i]);
    }
}

// Shows the next ball before the battle, or the state of the next Pokémon after it, with particles where the row of
// balls is, start frames after the row began
// pokebw2's CommBtlDemoBalls_Step
void func_ov213_021ba034(CommBtlDemoBalls *balls, u32 start) {
    int frame = balls->timer - start;
    int i;

    if (frame < balls->ballCount) {
        fx32 x;
        fx32 y;

        if (frame == 0 && balls->index == 0) {
            GFL_SndSEPlay(func_ov213_021b98dc(SE_BALL, FALSE));
        }
        if (func_ov213_021b9e50(balls->type)) {
            if (balls->index == 0) {
                x = -0x400 - frame * 0x800;
                y = -0x2500;
            } else if (balls->index == 1) {
                x = frame * 0x800 + 0x300;
                y = 0x1300;
            }
        } else {
            if (balls->index == 0) {
                x = -0x1800 - frame * 0x800;
                y = -0x1500;
            } else if (balls->index == 1) {
                x = 0x800 - frame * 0x800;
                y = -0x2800;
            } else if (balls->index == 2) {
                x = frame * 0x800 - 0x100;
                y = 0x1800;
            } else if (balls->index == 3) {
                x = frame * 0x800 + 0x1f00;
                y = 0x300;
            }
        }
        {
            VecFx32 pos = { 0, 0, -100 };

            pos.x = x;
            pos.y = y;
            func_ov213_021ba8f4(balls->g3d, &pos);
        }
        if (func_ov213_021b9e80(balls->type)) {
            func_0204b3dc(balls->actors[frame], TRUE);
        } else {
            func_ov213_021ba1c8(balls, balls->party, balls->states, balls->ballCount - 1 - frame);
        }
    } else {
        for (i = 0; i < balls->ballCount; i++) {
            func_0204b84c(balls->actors[i], 1);
        }
        balls->started = FALSE;
    }
}

// The values the screen was tuned with. The order of the ROM's data puts all of them here, after the last function with
// a local initializer and before the one that reads the frames. First the camera of the particles
// pokebw2's sCommBtlDemoParticleCameraPosition
static VecFx32 data_ov213_021bb330 = { 0, 0, FX32_CONST(128) };
// pokebw2's sCommBtlDemoParticleCameraUp
static VecFx32 data_ov213_021bb324 = { 0, FX32_ONE, 0 };
// pokebw2's sCommBtlDemoParticleCameraTarget
static VecFx32 data_ov213_021bb360 = { 0, 0, 0 };

// How many frames the 3D animations advance each frame
// pokebw2's sCommBtlDemoAnimSpeed
static int data_ov213_021bb320 = 1;

// The frames a trainer's first Poké Ball appears at. Their reads are folded into the code, and the ROM still has all
// six, which it would not if they were static
const u32 CommBtlDemoSingleStartDelay = 12;
const u32 CommBtlDemoMultiStartDelay1 = 15;
const u32 CommBtlDemoMultiStartDelay2 = 19;
const u32 CommBtlDemoSingleEndDelay = 8;
const u32 CommBtlDemoMultiEndDelay1 = 11;
const u32 CommBtlDemoMultiEndDelay2 = 15;

// pokebw2's CommBtlDemoBalls_Update
void func_ov213_021ba14c(CommBtlDemoBalls *balls) {
    u32 start;

    if (!balls->started) {
        return;
    }
    // BUG: start is not set when the screen was called with a type of 0
    switch (balls->type) {
#ifdef BUGFIX
    default:
#endif
    case COMM_BTL_DEMO_TYPE_SINGLE_START:
        start = CommBtlDemoSingleStartDelay;
        break;
    case COMM_BTL_DEMO_TYPE_MULTI_START:
        if (balls->index == 0 || balls->index == 2) {
            start = CommBtlDemoMultiStartDelay1;
        } else if (balls->index == 1 || balls->index == 3) {
            start = CommBtlDemoMultiStartDelay2;
        }
        break;
    case COMM_BTL_DEMO_TYPE_SINGLE_END:
        start = CommBtlDemoSingleEndDelay;
        break;
    case COMM_BTL_DEMO_TYPE_MULTI_END:
        if (balls->index == 0 || balls->index == 2) {
            start = CommBtlDemoMultiEndDelay1;
        } else if (balls->index == 1 || balls->index == 3) {
            start = CommBtlDemoMultiEndDelay2;
        }
        break;
    }
    if (balls->timer >= start) {
        func_ov213_021ba034(balls, start);
    }
    balls->timer++;
}

// pokebw2's CommBtlDemoBalls_Start
void func_ov213_021ba1c0(CommBtlDemoBalls *balls) {
    balls->started = TRUE;
}

// pokebw2's CommBtlDemoBalls_SetState
void func_ov213_021ba1c8(CommBtlDemoBalls *balls, PokeParty *party, const u8 *states, int index) {
    PartyPkm *pkm = NULL;

    if (index < PokeParty_GetPkmCount(party)) {
        pkm = PokeParty_GetPkm(balls->party, index);
    }
    func_0204b8a0(balls->actors[index], func_ov213_021b9eb0(pkm, states[index]));
}

// pokebw2's CommBtlDemo_CreateNameWindow
BmpWin *func_ov213_021ba200(u8 type, u8 index) {
    int x;
    int y;

    if (func_ov213_021b9e50(type)) {
        if (index == 0) {
            x = 4;
            y = 17;
        } else {
            x = 16;
            y = 3;
        }
    } else {
        switch (index) {
        case 0:
            x = 5;
            y = 13;
            break;
        case 1:
            x = 13;
            y = 19;
            break;
        case 2:
            x = 15;
            y = 1;
            break;
        case 3:
            x = 23;
            y = 7;
            break;
        }
    }
    return BmpWin_CreateDynamic(1, x, y, 10, 3, 1, TRUE);
}

void func_ov213_021ba268(CommBtlDemoTrainer *trainer, u8 type, u8 index, BattlePlayer *player,
                                    CommBtlDemoObj *obj, CommBtlDemoG3D *g3d, Font *font) {
    trainer->player = player;
    trainer->obj = obj;
    trainer->g3d = g3d;
    trainer->font = font;
    trainer->type = type;
    trainer->index = index;
    trainer->timer = 0;
    func_ov213_021b9ee4(&trainer->balls, player->party, type, index, obj, g3d, player->order);
    trainer->name = GFL_StrBufCreate(16, HEAPID_COMM_BTL_DEMO);
    textCopy(trainer->player->info->name, trainer->name);
    trainer->gender = getTrainerGender(trainer->player->info);
    trainer->nameWindow = func_ov213_021ba200(type, index);
    if (!func_ov213_021b9e80(trainer->type)) {
        func_ov213_021ba330(trainer, trainer->font);
    }
}

// Before the battle, the name appears when the balls have
// pokebw2's CommBtlDemoTrainer_UpdateName
void func_ov213_021ba2e0(CommBtlDemoTrainer *trainer) {
    if (trainer->timer == 45) {
        func_ov213_021ba330(trainer, trainer->font);
    }
}

// pokebw2's CommBtlDemoTrainer_Free
void func_ov213_021ba2f0(CommBtlDemoTrainer *trainer) {
    GFL_StrBufFree(trainer->name);
    func_ov213_021ba014(&trainer->balls);
    BmpWin_Free(trainer->nameWindow);
}

// pokebw2's CommBtlDemoTrainer_Update
void func_ov213_021ba30c(CommBtlDemoTrainer *trainer) {
    func_ov213_021ba14c(&trainer->balls);
    if (func_ov213_021b9e80(trainer->type)) {
        func_ov213_021ba2e0(trainer);
    }
    trainer->timer++;
}

// pokebw2's CommBtlDemoTrainer_PrintName
void func_ov213_021ba330(CommBtlDemoTrainer *trainer, Font *font) {
    GFL_BitmapFill(BmpWin_GetBitmap(trainer->nameWindow), 0);
    if (trainer->gender == 0) {
        GFL_TextRndUpdateColorIndexLUT(5, 6, 0);
    } else {
        GFL_TextRndUpdateColorIndexLUT(3, 4, 0);
    }
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(trainer->nameWindow), 1, 1, trainer->name, font);
    BmpWin_Transfer(trainer->nameWindow);
}

// pokebw2's CommBtlDemo_InitTrainers
void func_ov213_021ba38c(CommBtlDemoWork *wk) {
    int count = func_ov213_021b9e50(wk->type) ? 2 : 4;
    int i;

    for (i = 0; i < count; i++) {
        func_ov213_021ba268(&wk->trainers[i], wk->type, i, &wk->param->battle.players[i], &wk->obj, &wk->g3d,
                                wk->font);
    }
}

// pokebw2's CommBtlDemo_FreeTrainers
void func_ov213_021ba408(CommBtlDemoWork *wk) {
    int count = func_ov213_021b9e50(wk->type) ? 2 : 4;
    int i;

    for (i = 0; i < count; i++) {
        func_ov213_021ba2f0(&wk->trainers[i]);
    }
}

// pokebw2's CommBtlDemo_UpdateTrainers
void func_ov213_021ba444(CommBtlDemoWork *wk) {
    int count = func_ov213_021b9e50(wk->type) ? 2 : 4;
    int i;

    for (i = 0; i < count; i++) {
        func_ov213_021ba30c(&wk->trainers[i]);
    }
}

// pokebw2's CommBtlDemo_StartBalls
void func_ov213_021ba480(CommBtlDemoWork *wk) {
    int count = func_ov213_021b9e50(wk->type) ? 2 : 4;
    int i;

    for (i = 0; i < count; i++) {
        func_ov213_021ba1c0(&wk->trainers[i].balls);
    }
}

// Counts a battle of a kind, which is one of records 13, 17 and 37, in that record and in the record of its result
// pokebw2's CommBtlDemo_AddRecord
void func_ov213_021ba4bc(GameRecords *records, u32 id, u32 result) {
    switch (id) {
    case 13:
        if (result == COMM_BTL_DEMO_RESULT_WIN) {
            RecordAddOne(records, 14);
        } else if (result == COMM_BTL_DEMO_RESULT_LOSE) {
            RecordAddOne(records, 15);
        }
        break;
    case 37:
        if (result == COMM_BTL_DEMO_RESULT_WIN) {
            RecordAddOne(records, 38);
        } else if (result == COMM_BTL_DEMO_RESULT_LOSE) {
            RecordAddOne(records, 39);
        }
        break;
    case 17:
        if (result == COMM_BTL_DEMO_RESULT_WIN) {
            RecordAddOne(records, 18);
        } else if (result == COMM_BTL_DEMO_RESULT_LOSE) {
            RecordAddOne(records, 19);
        } else if (result == COMM_BTL_DEMO_RESULT_DRAW) {
            RecordAddOne(records, 20);
        }
        break;
    }
    RecordAddOne(records, id);
}

// Counts the battle in the records of the kind of connection it was fought over
// pokebw2's CommBtlDemo_AddRecords
void func_ov213_021ba514(CommBtlDemoWork *wk) {
    if (wk->param->records == NULL) {
        return;
    }
    switch (func_0203d5d4()) {
    case 12:
    case 31:
    case 37:
        func_ov213_021ba4bc(wk->param->records, 37, wk->battleResult);
        break;
    case 10:
        func_ov213_021ba4bc(wk->param->records, 17, wk->battleResult);
        break;
    case 20:
    case 21:
    case 23:
    case 24:
    case 25:
    case 26:
        func_ov213_021ba4bc(wk->param->records, 13, wk->battleResult);
        break;
    }
}

void func_ov213_021ba5a0(CommBtlDemoResult *result, CommBtlDemoObj *obj, CommBtlDemoG3D *g3d, u16 count0,
                                   u16 count1, u8 type) {
    int i;

    result->obj = obj;
    result->g3d = g3d;
    result->type = type;
    result->counts[0] = count0;
    result->counts[1] = count1;
    result->countActors[0] = func_ov213_021ba750(obj, 0, result->counts[0]);
    result->countActors[1] = func_ov213_021ba750(obj, 1, result->counts[1]);
    for (i = 0; i < LABEL_COUNT; i++) {
        s16 x;
        s16 y;

        switch (i) {
        case LABEL_SIDE0:
            x = 206;
            y = 152;
            break;
        case LABEL_SIDE1:
            x = 50;
            y = 40;
            break;
        case LABEL_MIDDLE:
            x = 128;
            y = 96;
            break;
        }
        result->labels[i] = func_ov213_021ba71c(obj, x, y, ANIM_WIN);
        func_0204b3dc(result->labels[i], FALSE);
    }
}

// pokebw2's CommBtlDemoResult_Free
void func_ov213_021ba624(CommBtlDemoResult *result) {
    int i;

    for (i = 0; i < LABEL_COUNT; i++) {
        func_0204b3b4(result->labels[i]);
    }
    for (i = 0; i < SIDE_COUNT; i++) {
        func_0204b3b4(result->countActors[i]);
    }
}

// pokebw2's CommBtlDemoResult_Update
void func_ov213_021ba650(CommBtlDemoResult *result) {
    func_ov213_021b9e50(result->type);
    if (result->timer == 30) {
        func_0204b3dc(result->countActors[0], TRUE);
        func_0204b92c(result->countActors[0], TRUE);
        func_0204b3dc(result->countActors[1], TRUE);
        func_0204b92c(result->countActors[1], TRUE);
    }
    result->timer++;
}

// Shows a label as that of the winner (0), of the loser (1) or of a draw (2)
// pokebw2's CommBtlDemoResult_ShowLabel
void func_ov213_021ba68c(CommBtlDemoResult *result, u32 label, u32 kind) {
    u32 anim;

    switch (kind) {
    case 0:
        anim = ANIM_WIN;
        break;
    case 1:
        anim = ANIM_LOSE;
        break;
    case 2:
        anim = ANIM_DRAW;
        break;
    }
    func_0204b3dc(result->labels[label], TRUE);
    func_0204b92c(result->labels[label], TRUE);
    func_0204b8a0(result->labels[label], anim);
}

// pokebw2's CommBtlDemoObj_Init
void func_ov213_021ba6c8(CommBtlDemoObj *obj, CommBtlDemoGraphic *graphic, HeapID heapId) {
    UIObjResSetup setup;

    obj->graphic = graphic;
    obj->heapId = heapId;
    setup.vramType = 0;
    setup.flags = 0;
    setup.arcId = ARCID_COMM_BTL_DEMO;
    setup.paletteFile = FILE_OBJ_PALETTE;
    setup.charFile = FILE_OBJ_CHARS;
    setup.cellFile = FILE_OBJ_CELLS;
    setup.animFile = FILE_OBJ_ANIMS;
    setup.paletteOffset = 0;
    setup.paletteStart = 0;
    setup.paletteCount = 4;
    func_ov067_021b6208(&obj->res, &setup, func_ov213_021bac94(graphic), heapId);
}

// pokebw2's CommBtlDemoObj_Free
void func_ov213_021ba710(CommBtlDemoObj *obj) {
    func_ov067_021b6284(&obj->res);
}

// pokebw2's CommBtlDemoObj_CreateActor
ClActor *func_ov213_021ba71c(CommBtlDemoObj *obj, s16 x, s16 y, u16 anim) {
    return func_ov067_021b629c(&obj->res, func_ov213_021bac94(obj->graphic), x, y, anim, obj->heapId);
}

// The number of Pokémon a side has left, hidden until the balls have shown what became of them
// pokebw2's CommBtlDemoObj_CreateCount
ClActor *func_ov213_021ba750(CommBtlDemoObj *obj, u32 side, u8 count) {
    s16 x;
    s16 y;
    ClActor *actor;

    if (side == 0) {
        x = 206;
        y = 152;
    } else {
        x = 50;
        y = 40;
    }
    actor = func_ov213_021ba71c(obj, x, y, ANIM_COUNT_0 + count);
    func_0204b3dc(actor, FALSE);
    func_0204b800(actor, 1);
    return actor;
}

// pokebw2's CommBtlDemoG3D_Init
void func_ov213_021ba780(CommBtlDemoG3D *g3d, CommBtlDemoGraphic *graphic, HeapID heapId) {
    G2_SetBlendAlpha(GX_PLANEMASK_BG0, 0x1f, 0, 0);
    g3d->graphic = graphic;
    g3d->heapId = heapId;
    g3d->manager = GFL_G3DMgrCreate(16, 20, heapId);
    func_02050b80(heapId);
}

// pokebw2's CommBtlDemoG3D_Free
void func_ov213_021ba7b8(CommBtlDemoG3D *g3d) {
    func_ov213_021ba9c0(g3d);
    func_ov213_021ba8d4(g3d);
    GFL_G3DMgrFree(g3d->manager);
}

// pokebw2's CommBtlDemoG3D_Draw
static void func_ov213_021ba7d0(CommBtlDemoG3D *g3d) {
    func_ov213_021bac7c(g3d->graphic);
    if (g3d->particles != NULL) {
        func_02050be8();
    }
    if (g3d->sceneLoaded) {
        SRTMatrix matrix;

        matrix.translation.x = 0;
        matrix.translation.y = 0;
        matrix.translation.z = 0;
        matrix.scale.x = FX32_ONE;
        matrix.scale.y = FX32_ONE;
        matrix.scale.z = FX32_ONE;
        MTX_Identity33_(&matrix.rotation);
        GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(g3d->manager, g3d->scene), &matrix);
    }
    func_ov213_021bac88(g3d->graphic);
}

// Replaces the particles with those of a file
// pokebw2's CommBtlDemoG3D_LoadParticles
void func_ov213_021ba81c(CommBtlDemoG3D *g3d, u32 file) {
    G3DCameraProjection projection;
    void *resource;

    if (g3d->particles != NULL) {
        func_ov213_021ba8bc(g3d);
    }
    g3d->particles = func_02050c04(g3d->particleWork, PARTICLE_WORK_SIZE, TRUE, g3d->heapId);
    projection.type = G3DCAM_PROJECTION_ORTHO;
    projection.param1 = FX32_CONST(3);
    projection.param2 = -FX32_CONST(3);
    projection.param3 = -FX32_CONST(4);
    projection.param4 = FX32_CONST(4);
    projection.near = FX32_ONE;
    projection.far = FX32_CONST(1024);
    projection.ndcRangeOverride = 0;
    func_02051758(g3d->particles);
    func_02051648(g3d->particles, &projection, FX32_CONST(2), &data_ov213_021bb330,
                  &data_ov213_021bb324, &data_ov213_021bb360, g3d->heapId);
    resource = func_0205121c(ARCID_COMM_BTL_DEMO, file, g3d->heapId);
    g3d->emitterCount = func_02051b04(resource);
    func_02051230(g3d->particles, resource, FALSE, GFL_VBlankGetTCBMgr());
}

// pokebw2's CommBtlDemoG3D_ClearParticles
void func_ov213_021ba8bc(CommBtlDemoG3D *g3d) {
    func_02051620(g3d->particles);
    func_02050db8(g3d->particles);
    g3d->particles = NULL;
}

// pokebw2's CommBtlDemoG3D_FreeParticles
void func_ov213_021ba8d4(CommBtlDemoG3D *g3d) {
    if (g3d->particles != NULL) {
        func_02050db8(g3d->particles);
    }
    func_02050ed4();
}

// pokebw2's CommBtlDemoG3D_AddEmitter
void func_ov213_021ba8e8(CommBtlDemoG3D *g3d, u16 emitter, const VecFx32 *pos) {
    func_020515b4(g3d->particles, emitter, pos);
}

// Adds every emitter of the particle file
// pokebw2's CommBtlDemoG3D_AddEmitters
void func_ov213_021ba8f4(CommBtlDemoG3D *g3d, const VecFx32 *pos) {
    int i;

    for (i = 0; i < g3d->emitterCount; i++) {
        func_ov213_021ba8e8(g3d, i, pos);
    }
}

// Binds one of the texture's palettes to the materials of an actor's model that use another
// pokebw2's CommBtlDemoG3D_BindPalette
BOOL func_ov213_021ba920(G3DActor *actor, u32 plttToMatListIdx, u32 plttNameIdx) {
    G3DModel *model = GFL_G3DActorGetMdl(actor);
    void *texResource = GFL_G3DMdlGetTexResource(model);
    NNSG3dResMdl *resMdl = GFL_G3DMdlGetEngineModel(model)->resMdl;
    NNSG3dResTex *tex = GFL_G3DResGetTexData(texResource);

    NNS_G3dReleaseMdlPltt(resMdl);
    return NNS_G3dForceBindMdlPltt(resMdl, tex, plttToMatListIdx, plttNameIdx) & 1;
}

// pokebw2's CommBtlDemoG3D_LoadScene
void func_ov213_021ba95c(CommBtlDemoG3D *g3d, u32 scene) {
    G3DActor *actor;
    int i;
    int count;

    if (g3d->sceneLoaded) {
        func_ov213_021ba9ac(g3d);
    }
    g3d->scene = GFL_G3DMgrNewScene(g3d->manager, &sCommBtlDemoScenes[scene]);
    g3d->sceneLoaded = TRUE;
    actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
    count = GFL_G3DActorGetAnmCount(actor);
    for (i = 0; i < count; i++) {
        GFL_G3DActorBindAnm(actor, i);
    }
}

// pokebw2's CommBtlDemoG3D_DeleteScene
void func_ov213_021ba9ac(CommBtlDemoG3D *g3d) {
    GFL_G3DMgrDeleteScene(g3d->manager, g3d->scene);
    g3d->sceneLoaded = FALSE;
    g3d->animEnded = FALSE;
}

// pokebw2's CommBtlDemoG3D_FreeScene
void func_ov213_021ba9c0(CommBtlDemoG3D *g3d) {
    if (g3d->sceneLoaded) {
        func_ov213_021ba9ac(g3d);
    }
}

// Steps the scene's animations, and returns FALSE once they have ended
// pokebw2's CommBtlDemoG3D_UpdateAnim
BOOL func_ov213_021ba9d0(CommBtlDemoG3D *g3d) {
    BOOL playing = TRUE;
    G3DActor *actor;
    int count;
    int i;

    if (g3d->animEnded) {
        return FALSE;
    }
    actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
    count = GFL_G3DActorGetAnmCount(actor);
    for (i = 0; i < count; i++) {
        playing = GFL_G3DActorStepAnmFrameLoop(actor, i, data_ov213_021bb320 * FX32_ONE);
        if (!playing) {
            g3d->animEnded = TRUE;
        }
    }
    return playing;
}

// Before the battle, every Pokémon is healthy
// pokebw2's CommBtlDemo_ResetStates
void func_ov213_021baa20(CommBtlDemoParam *param) {
    int i;
    int j;

    if (param->battle.unk44 == COMM_BTL_DEMO_TYPE_SINGLE_START) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < PokeParty_GetPkmCount(param->battle.players[i].party); j++) {
                param->battle.players[i].order[j] = COMM_BTL_DEMO_POKE_HEALTHY;
            }
        }
    } else if (param->battle.unk44 == COMM_BTL_DEMO_TYPE_MULTI_START) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < PokeParty_GetPkmCount(param->battle.players[i].party); j++) {
                param->battle.players[i].order[j] = COMM_BTL_DEMO_POKE_HEALTHY;
            }
        }
    }
}

// How many of a player's Pokémon have not fainted
// pokebw2's CommBtlDemo_CountLeft
u32 func_ov213_021baa8c(BattlePlayer *player) {
    PokeParty *party = player->party;
    int i;
    u32 count = 0;
    const u8 *states = player->order;

    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        if (states[i] == COMM_BTL_DEMO_POKE_HEALTHY || states[i] == COMM_BTL_DEMO_POKE_SICK) {
            count++;
        }
    }
    return count;
}
