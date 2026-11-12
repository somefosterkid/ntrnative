// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_effect.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle effect manager: it creates the battle view's 3D stage and field, its camera, the Pokémon sprites, the cell
// actors, the gauges, the timer and the BG, runs them each frame, starts the effect scripts of btlv_effvm.c, queueing
// those started while another runs, and keeps the tasks of the view's effects. The name is the ROM's string, from
// GFL_HeapAllocate's asserts. BtlvEffect_Create is swan's name; the rest are ours

#include "battle/btlv_effect.h"
#include "types.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_setup.h"
#include "battle/btlv_b_gauge.h"
#include "battle/btlv_bg.h"
#include "battle/btlv_camera.h"
#include "battle/btlv_clact.h"
#include "battle/btlv_effvm.h"
#include "battle/btlv_field.h"
#include "battle/btlv_gauge.h"
#include "battle/btlv_mcss.h"
#include "battle/btlv_stage.h"
#include "battle/btlv_timer.h"
#include "field/field_light_status.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/particle.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nnsys/g3d.h"
#include "nnsys/gfd.h"
#include "pml/waza.h"
#include "save/player_info.h"
#include "system/mcss.h"
#include "system/palanm.h"
#include "system/vm.h"
#include "constants/battle.h"

// Built by BtlvEffect_Create (swan's name), 0x2c bytes, copied into the work by the init
struct BtlvEffectSetup {
    u32 battleStyle;           // 0x00
    u32 battleType;            // 0x04
    BtlFieldEnv fieldEnv;      // 0x08
    u16 trainerTypes[4];       // 0x18
    BOOL multi;                // 0x20
    BtlvScu *scu;              // 0x24
    BtlMainModule *mainModule; // 0x28
};

typedef void (*BtlvEffectTaskCallback)(TCB *task);

#define BTLV_EFFECT_TASK_MAX 32

// The battle effect work, 0x24c bytes, allocated by the init and pointed to by data_ov094_0220af20
struct BtlvEffect {
    TCBManager *tcbManager;                                     // 0x000
    void *tcbManagerBuffer;                                     // 0x004
    TCB *tasks[BTLV_EFFECT_TASK_MAX];                           // 0x008
    BtlvEffectTaskCallback taskCallbacks[BTLV_EFFECT_TASK_MAX]; // 0x088
    u32 taskGroups[BTLV_EFFECT_TASK_MAX];                       // 0x108
    VM *effvm;                                                  // 0x188
    PaletteFade *palFade;                                       // 0x18c
    BtlvMcss *mcss;                                             // 0x190
    BtlvStage *stage;                                           // 0x194
    BtlvField *field;                                           // 0x198
    BtlvCamera *camera;                                         // 0x19c
    BtlvClact *clact;                                           // 0x1a0
    BtlvGauge *gauge;                                           // 0x1a4
    BtlvBGauge *ballGauges[2];                                  // 0x1a8  by side
    BtlvTimer *timer;                                           // 0x1b0
    BtlvBg *bg;                                                 // 0x1b4
    TCB *vblankTask;                                            // 0x1b8
    BtlvEffectSetup setup;                                      // 0x1bc
    BOOL executing;                                             // 0x1e8
    // 0x1ec
    u32 damageTasks : 8; // bits 0-7: one per view position
    u32 unkTasks8 : 8;   // bits 8-15: one per view position
    u32 rotateTasks : 2; // bits 16-17: one per side
    u32 state : 8;       // bits 18-25
    u32 unk1EC_26 : 1;   // bit 26
    u32 : 5;
    HeapID heapId;       // 0x1f0
    BOOL bgmChanged;     // 0x1f4
    s32 trainers[8];     // 0x1f8  by pos - 8, -1 when none
    u32 abilities[8];    // 0x218  by view position
    u32 idleEffectMode;  // 0x238
    u32 idleEffectSeq;   // 0x23c
    s32 idleEffectTimer; // 0x240
    s32 idleEffectWait;  // 0x244
    s32 idleEffectLast;  // 0x248
}; // 0x24c

// An entry of file 0 of archive 152, by the field environment's bgType; the terrain picks the field and the stage
typedef struct {
    u8 light;        // 0x00  the zone's light and time of day light the battle
    u8 seasonal;     // 0x01  the season picks the variant
    u8 fieldIds[17]; // 0x02
    u8 stageIds[17]; // 0x13
} BtlvEffectBgData;

// The colors FieldLightStatus_Get fills in. The frame of func_ov094_021f7544 gives them 4 more bytes, which nothing reads
typedef struct {
    GXRgb colors[8];
    u32 unk10;
} BtlvEffectLightColors;

// An effect started while another one runs, which waits in a task for its turn
typedef struct {
    u32 unk00;
    u32 atkPos;
    u32 defPos;
    u32 effNo;
    BtlvEffvmParam *param; // freed by func_ov094_021f9028
} BtlvEffectQueued;

// The flashes of a damaged Pokémon, a blink task
typedef struct {
    u32 pos;     // 0x00
    u32 seq;     // 0x04
    u32 count;   // 0x08  blinks left
    u32 wait;    // 0x0c
    u32 color;   // 0x10
    BOOL hidden; // 0x14  the sprite is hidden, so only the timing runs
} BtlvEffectDamage;

// The change of a Pokémon's sprite
typedef struct {
    u32 viewPos;       // 0x0
    u32 seq;           // 0x4
    MCSSLoadInfo info; // 0x8
} BtlvEffectChange;

// The work of the rotation task that func_ov094_021f8414 starts, 0x10 bytes
typedef struct {
    u32 seq;  // 0x0
    u8 arg4;  // 0x4  mode != 3
    u32 side; // 0x8
    u32 argC; // 0xc
} BtlvEffectRotateWork;

static u32 BtlvEffect_GetStudioMode(BtlMainModule *mainModule);
static u32 BtlvEffect_GetStudioParam(BtlMainModule *mainModule);
static void func_ov094_021f7bec(void);
static void func_ov094_021f8628(u32 viewPos, VecFx32 *camPos, VecFx32 *camTarget);
static int func_ov094_021f899c(TCB *tcb);
int func_ov094_021f8b40(void);
static void func_ov094_021f8b6c(void);
void func_ov094_021f8b90(TCB *tcb, void *data);
static void func_ov094_021f8bac(TCB *tcb, void *data);
static void func_ov094_021f8c78(TCB *tcb);
void func_ov094_021f8cb0(TCB *tcb, void *data);
static void func_ov094_021f8ef8(TCB *tcb);
static void func_ov094_021f8f30(TCB *tcb, void *data);
static void func_ov094_021f8fb0(TCB *tcb);
static void func_ov094_021f8fe8(TCB *tcb, void *data);
static void func_ov094_021f9028(TCB *tcb);
static void func_ov094_021f903c(void);

// Which trainer types use the second sprite set
static const u8 data_ov094_02209d0c[] = {
#include "asset/btlv_effect_data_ov094_02209d0c.inc"
};

// The idle effects func_ov094_021f903c picks from: in mode 3 of a battle of type 0x400, and in the others
static u32 data_ov094_0220ac70[] = { 0x24a, 0x24b };
static u32 data_ov094_0220ac78[] = { 0x235, 0x24c, 0x24d, 0x24e, 0x24f, 0x250 };

// pokebw2's sBtlvEffect
static BtlvEffect *data_ov094_0220af20;

void func_ov094_02208918(BtlvGauge *gauge, u32 value);
void func_ov094_02208924(BtlvGauge *gauge, u32 value);

// pokebw2's BtlvEffect_Exit

BtlvEffectSetup *BtlvEffect_Create(u32 battleStyle, u32 battleType, const BtlFieldEnv *fieldEnv, BOOL multi,
                                   const u16 *trainerTypes, BtlMainModule *mainModule, BtlvScu *scu, HeapID heapId) {
    BtlvEffectSetup *setup =
        GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(BtlvEffectSetup), FALSE, "btlv_effect.c", 0xca);
    int i;

    setup->battleStyle = battleStyle;
    setup->battleType = battleType;
    setup->fieldEnv = *fieldEnv;
    setup->multi = multi;
    setup->scu = scu;
    setup->mainModule = mainModule;
    for (i = 0; i < 4; i++) {
        setup->trainerTypes[i] = trainerTypes[i];
    }
    return setup;
}

BtlvEffectSetup *func_ov094_021f745c(BtlMainModule *mainModule, BtlvScu *scu, HeapID heapId) {
    u16 trainerTypes[4];
    u32 i;

    for (i = 0; i < 4; i++) {
        trainerTypes[i] = 0;
    }
    if (func_ov093_021b86c8(mainModule) == TRUE) {
        u8 side = func_ov093_021b8fe4(mainModule);
        u8 playerId = GetPlayerClientID(mainModule);
        u8 partnerId = func_ov093_021b9000(mainModule);

        trainerTypes[side * 2] = func_ov093_021ba020(mainModule, playerId);
        if (partnerId != 4) {
            trainerTypes[(side ^ 1) * 2] = func_ov093_021ba020(mainModule, partnerId);
        }
        trainerTypes[1] = func_ov093_021ba020(mainModule, func_ov093_021b904c(mainModule, 0));
        trainerTypes[3] = func_ov093_021ba020(mainModule, func_ov093_021b904c(mainModule, 1));
    } else {
        trainerTypes[0] = func_ov093_021ba020(mainModule, GetPlayerClientID(mainModule));
        trainerTypes[1] = func_ov093_021ba020(mainModule, func_ov093_021b904c(mainModule, 0));
        trainerTypes[2] = 0;
        trainerTypes[3] = 0;
    }
    return BtlvEffect_Create(BtlSetup_GetBattleStyle(mainModule), BtlSetup_GetBattleType(mainModule),
                             &GetFieldEffectData(mainModule)->env, func_ov093_021b86c8(mainModule), trainerTypes,
                             mainModule, scu, heapId);
}

void func_ov094_021f7544(BtlvEffectSetup *setup, Font *font, HeapID heapId) {
    BtlvEffectBgData *bgData;
    u8 season;
    int i;

    data_ov094_0220af20 = GFL_HeapAllocate(heapId, sizeof(BtlvEffect), TRUE, "btlv_effect.c", 0x116);
    data_ov094_0220af20->setup = *setup;
    data_ov094_0220af20->heapId = heapId;
    data_ov094_0220af20->tcbManagerBuffer =
        GFL_HeapAllocate(heapId, GFL_TCBMgrCalcAllocSize(BTLV_EFFECT_TASK_MAX), TRUE, "btlv_effect.c", 0x11c);
    data_ov094_0220af20->tcbManager = GFL_TCBMgrCreate(BTLV_EFFECT_TASK_MAX, data_ov094_0220af20->tcbManagerBuffer);
    data_ov094_0220af20->effvm = func_ov094_021f93f4(data_ov094_0220af20->tcbManager, heapId);
    data_ov094_0220af20->palFade = func_02021518(heapId);
    func_02021ee4(data_ov094_0220af20->palFade, TRUE);
    func_0202155c(data_ov094_0220af20->palFade, 0, 0x200, heapId);
    func_0202155c(data_ov094_0220af20->palFade, 1, 0x1e0, heapId);
    func_0202155c(data_ov094_0220af20->palFade, 2, 0x200, heapId);
    func_0202155c(data_ov094_0220af20->palFade, 3, 0x1e0, heapId);
    data_ov094_0220af20->mcss = func_ov094_021feac0(setup->battleStyle, data_ov094_0220af20->tcbManager, heapId);

    bgData = GFL_ArcSysReadHeapNew(0x98, 0, HEAPID_TAIL(data_ov094_0220af20->heapId));
    season = 0;
    if (bgData[setup->fieldEnv.bgType].seasonal) {
        season = setup->fieldEnv.season;
    }
    data_ov094_0220af20->stage =
        func_ov094_021f6560(setup->battleStyle, bgData[setup->fieldEnv.bgType].stageIds[setup->fieldEnv.terrain], season, heapId);
    if (data_ov094_0220af20->setup.mainModule != NULL) {
        data_ov094_0220af20->field = BtlvField_Create(BtlSetup_IsBattleType(data_ov094_0220af20->setup.mainModule, 0x400), bgData[setup->fieldEnv.bgType].fieldIds[setup->fieldEnv.terrain], season, heapId);
    } else {
        data_ov094_0220af20->field = BtlvField_Create(FALSE, bgData[setup->fieldEnv.bgType].fieldIds[setup->fieldEnv.terrain], season, heapId);
    }
    if (bgData[setup->fieldEnv.bgType].light) {
        Light light;
        BtlvEffectLightColors colors;

        func_0201493c(setup->fieldEnv.zoneId, setup->fieldEnv.hour, setup->fieldEnv.minute, setup->fieldEnv.weather,
                      setup->fieldEnv.season, colors.colors, data_ov094_0220af20->heapId);
        light.color = colors.colors[0];
        light.direction.x = 0;
        light.direction.y = -FX16_ONE;
        light.direction.z = 0;
        GFL_G3DSysLightSet(0, &light);
    }
    GFL_HeapFree(bgData);

    data_ov094_0220af20->camera = BtlvCamera_Create(data_ov094_0220af20->tcbManager, heapId);
    data_ov094_0220af20->clact = func_ov094_02201a8c(data_ov094_0220af20->tcbManager, heapId);
    data_ov094_0220af20->gauge = func_ov094_02206bc8(font, heapId);
    data_ov094_0220af20->timer = func_ov094_02209224(heapId);
    data_ov094_0220af20->bg = func_ov094_0220975c(data_ov094_0220af20->tcbManager, heapId);
    func_ov094_021ff16c(data_ov094_0220af20->mcss);
    func_02050b80(heapId);
    for (i = 0; i < 8; i++) {
        data_ov094_0220af20->trainers[i] = -1;
    }
    if (data_ov094_0220af20->setup.mainModule != NULL) {
        if (BtlSetup_IsBattleType(data_ov094_0220af20->setup.mainModule, 0x800)) {
            data_ov094_0220af20->idleEffectWait = 300;
        } else {
            data_ov094_0220af20->idleEffectWait = 900;
        }
    }
    data_ov094_0220af20->vblankTask = GFL_VBlankTCBAdd(func_ov094_021f8b90, NULL, 1);
    PokeVoice_AllocMulti(2, data_ov094_0220af20->heapId);
}

void func_ov094_021f7808(void) {
    if (data_ov094_0220af20 != NULL) {
        PokeVoice_FreeMulti();
        func_ov094_021f8b6c();
        func_020215a0(data_ov094_0220af20->palFade, 0);
        func_020215a0(data_ov094_0220af20->palFade, 1);
        func_020215a0(data_ov094_0220af20->palFade, 2);
        func_020215a0(data_ov094_0220af20->palFade, 3);
        func_02021540(data_ov094_0220af20->palFade);
        func_ov094_021feb78(data_ov094_0220af20->mcss);
        func_ov094_021f6898(data_ov094_0220af20->stage);
        func_ov094_021f6cf0(data_ov094_0220af20->field);
        func_ov094_021f6ec0(data_ov094_0220af20->camera);
        func_ov094_02201ac4(data_ov094_0220af20->clact);
        func_ov094_02206e28(data_ov094_0220af20->gauge);
        func_ov094_022092e8(data_ov094_0220af20->timer);
        func_ov094_0220977c(data_ov094_0220af20->bg);
        func_02050ed4();
        func_ov094_021f94d8(data_ov094_0220af20->effvm);
        GFL_TCBRemove(data_ov094_0220af20->vblankTask);
        func_02030da4(data_ov094_0220af20->tcbManager);
        GFL_HeapFree(data_ov094_0220af20->tcbManagerBuffer);
        GFL_HeapFree(data_ov094_0220af20);
        data_ov094_0220af20 = NULL;
    }
}

// pokebw2's BtlvEffect_Main
void func_ov094_021f78e4(void) {
    if (data_ov094_0220af20 != NULL) {
        data_ov094_0220af20->executing = func_ov094_021f9478(data_ov094_0220af20->effvm);
        func_ov094_021f903c();
        GFL_TCBMgrUpdate(data_ov094_0220af20->tcbManager);
        func_ov094_021febac(data_ov094_0220af20->mcss);
        func_ov094_021f6934(data_ov094_0220af20->stage);
        func_ov094_021f6d5c(data_ov094_0220af20->field);
        func_ov094_021f6ed4(data_ov094_0220af20->camera);
        func_ov094_02206eb4(data_ov094_0220af20->gauge);
        func_ov094_02201af0(data_ov094_0220af20->clact);
        GFL_G3DSysReset();
        GFL_G3DSysMtxViewFlush();
        func_ov094_021f69b0(data_ov094_0220af20->stage);
        func_ov094_021f6dd4(data_ov094_0220af20->field);
        func_ov094_021fee9c(data_ov094_0220af20->mcss);
        // The game reads the polygon list RAM count before each of these and drops it, likely what is left of a debug
        // print of the counts
        reg_G3X_LISTRAM_COUNT;
        func_02050be8();
        reg_G3X_LISTRAM_COUNT;
        GFL_G3DSysReqSwapBuffers();
    }
}

void func_ov094_021f7998(u32 effNo) {
    BtlvEffectQueued *queued;

    if (!func_ov094_021f7eb4()) {
        func_ov094_021f94f8(data_ov094_0220af20->effvm, 0xff, 0xff, effNo, NULL);
        data_ov094_0220af20->executing = TRUE;
        return;
    }
    queued = GFL_HeapAllocate(HEAPID_TAIL(data_ov094_0220af20->heapId), sizeof(BtlvEffectQueued), TRUE, "btlv_effect.c",
                              0x210);
    queued->atkPos = 0xff;
    queued->defPos = 0xff;
    queued->effNo = effNo;
    func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8fe8, queued, 0),
                        func_ov094_021f9028, 0);
}

void func_ov094_021f7a28(u32 pos, int effect) {
    BtlvEffectQueued *queued;

    if (!func_ov094_021f7eb4()) {
        func_ov094_021f94f8(data_ov094_0220af20->effvm, pos, 0xff, effect, NULL);
        data_ov094_0220af20->executing = TRUE;
    } else {
        queued = GFL_HeapAllocate(data_ov094_0220af20->heapId, sizeof(BtlvEffectQueued), TRUE, "btlv_effect.c", 0x22c);
        queued->atkPos = pos;
        queued->defPos = 0xff;
        queued->effNo = effect;
        func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8fe8, queued, 0),
                            func_ov094_021f9028, 0);
    }
}

void func_ov094_021f7aac(u32 atkPos, u32 defPos, int effect) {
    BtlvEffectQueued *queued;

    if (!func_ov094_021f7eb4()) {
        func_ov094_021f94f8(data_ov094_0220af20->effvm, atkPos, defPos, effect, NULL);
        data_ov094_0220af20->executing = TRUE;
    } else {
        queued = GFL_HeapAllocate(data_ov094_0220af20->heapId, sizeof(BtlvEffectQueued), TRUE, "btlv_effect.c", 0x249);
        queued->atkPos = atkPos;
        queued->defPos = defPos;
        queued->effNo = effect;
        func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8fe8, queued, 0),
                            func_ov094_021f9028, 0);
    }
}

void func_ov094_021f7b2c(const BtlvMoveEffectParam *param) {
    BtlvEffectQueued *queued;
    BtlvEffvmParam vmParam;

    if (!func_ov094_021f7eb4()) {
        func_ov094_021f9844(&vmParam);
        vmParam.unk0 = param->unk10;
        vmParam.variant = param->unk0C;
        vmParam.unk2 = param->unk0D;
        func_ov094_021f94f8(data_ov094_0220af20->effvm, param->attackerPos, param->targetPos, param->move, &vmParam);
        data_ov094_0220af20->executing = TRUE;
        return;
    }
    queued = GFL_HeapAllocate(data_ov094_0220af20->heapId, sizeof(BtlvEffectQueued), TRUE, "btlv_effect.c", 0x26b);
    queued->param = GFL_HeapAllocate(data_ov094_0220af20->heapId, sizeof(BtlvEffvmParam), TRUE, "btlv_effect.c", 0x26c);
    queued->atkPos = param->attackerPos;
    queued->defPos = param->targetPos;
    queued->effNo = param->move;
    queued->param->unk0 = param->unk10;
    queued->param->variant = param->unk0C;
    queued->param->unk2 = param->unk0D;
    func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8fe8, queued, 0),
                        func_ov094_021f9028, 0);
}

// pokebw2's BtlvEffect_Stop
static void func_ov094_021f7bec(void) {
    func_ov094_021f97e4(data_ov094_0220af20->effvm);
    data_ov094_0220af20->executing = 0;
}

// pokebw2's BtlvEffect_Resume
void func_ov094_021f7c0c(void) {
    func_ov094_021f9814(data_ov094_0220af20->effvm);
    data_ov094_0220af20->executing = 1;
}

// pokebw2's BtlvEffect_StartDamage
void func_ov094_021f7c2c(u32 pos, u16 move) {
    BtlvEffectDamage *damage =
        GFL_HeapAllocate(HEAPID_TAIL(data_ov094_0220af20->heapId), sizeof(BtlvEffectDamage), FALSE, "btlv_effect.c", 0x29b);

    damage->seq = 0;
    damage->pos = pos;
    damage->count = 3;
    damage->wait = 0;
    damage->hidden = func_ov094_021ff348(data_ov094_0220af20->mcss, pos);
    if (PML_MoveGetCategory(move) == MOVE_CATEGORY_PHYSICAL) {
        damage->color = GX_RGB(2, 2, 2);
    } else {
        damage->color = GX_RGB(24, 24, 24);
    }
    data_ov094_0220af20->damageTasks |= func_ov094_021f935c(pos);
    func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8bac, damage, 0),
                       func_ov094_021f8c78, 0);
}

// pokebw2's BtlvEffect_StartEffect23B
void func_ov094_021f7ce4(u8 viewPos) {
    func_ov094_021f7a28(viewPos, 0x23b);
    func_ov094_021f8ae8(viewPos, 0);
}

// pokebw2's BtlvEffect_StartEffect23A
void func_ov094_021f7cfc(u8 pos, u16 itemNo, u8 arg2, u32 arg3, u32 arg4) {
    BtlvEffvmParam vmParam;

    func_ov094_021f9844(&vmParam);
    vmParam.unk3 = arg2;
    vmParam.unk8 = arg4;
    vmParam.unk4 = arg3;
    vmParam.itemNo = itemNo;
    func_ov094_021f94f8(data_ov094_0220af20->effvm, 0, pos, 0x23a, &vmParam);
    data_ov094_0220af20->executing = TRUE;
}

// pokebw2's BtlvEffect_StartEffect23D
void func_ov094_021f7d44(u8 pos, u16 itemNo) {
    BtlvEffvmParam BStack_24;

    func_ov094_021f9844(&BStack_24);
    BStack_24.itemNo = itemNo;
    func_ov094_021f94f8(data_ov094_0220af20->effvm, 0, (u32)pos, 0x23d, &BStack_24);
    data_ov094_0220af20->executing = 1;
}

// pokebw2's BtlvEffect_StartChangeSprite
void func_ov094_021f7d80(PartyPkm *pkm, u32 viewPos) {
    BtlvEffectChange *change =
        GFL_HeapAllocate(HEAPID_TAIL(data_ov094_0220af20->heapId), sizeof(BtlvEffectChange), FALSE, "btlv_effect.c", 0x2f9);

    change->seq = 0;
    change->viewPos = viewPos;
    func_ov094_021ffe48(data_ov094_0220af20->mcss, viewPos, pkm);
    func_ov094_022003ec(data_ov094_0220af20->mcss, pkm, &change->info, viewPos);
    data_ov094_0220af20->unkTasks8 |= func_ov094_021f935c(viewPos);
    func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8cb0, change, 0),
                       func_ov094_021f8ef8, 0);
}

// pokebw2's BtlvEffect_ChangeSprite
void func_ov094_021f7e38(PartyPkm *pkm, u32 viewPos) {
    MCSSLoadInfo info;

    func_ov094_021ffe48(data_ov094_0220af20->mcss, viewPos, pkm);
    func_ov094_022003ec(data_ov094_0220af20->mcss, pkm, &info, viewPos);
    func_ov094_022001dc(data_ov094_0220af20->mcss, viewPos, &info);
}

// pokebw2's BtlvEffect_StartEffect285
void func_ov094_021f7e74(u8 pos) {
    func_ov094_021f7a28(pos, 0x285);
}

// pokebw2's BtlvEffect_StartEffect286
void func_ov094_021f7e84(u8 pos) {
    func_ov094_021f7a28(pos, 0x286);
}

// pokebw2's BtlvEffect_StartEffect26E
void func_ov094_021f7e94(u8 pos) {
    func_ov094_021f7a28(pos, 0x26e);
}

// pokebw2's BtlvEffect_StartEffect26F
void func_ov094_021f7ea4(u8 pos) {
    func_ov094_021f7a28(pos, 0x26f);
}

BOOL func_ov094_021f7eb4(void) {
    if ((data_ov094_0220af20->executing | data_ov094_0220af20->damageTasks | data_ov094_0220af20->unkTasks8 |
         data_ov094_0220af20->rotateTasks) != 0) {
        return TRUE;
    }
    return FALSE;
}


void func_ov094_021f7ee8(PartyPkm *pkm, u32 viewPos) {
    func_ov094_021feea8(data_ov094_0220af20->mcss, pkm, viewPos);
}

void func_ov094_021f7f04(u32 viewPos) {
    func_ov094_021ff0f8(data_ov094_0220af20->mcss, viewPos);
}

BOOL func_ov094_021f7f1c(int pos) {
    BOOL BVar1;

    if (pos < 8) {
        BVar1 = func_ov094_021ffd9c(data_ov094_0220af20->mcss, pos);
        return BVar1;
    }
    if (data_ov094_0220af20->trainers[pos - 8] != -1) {
        return 1;
    }
    return 0;
}

void func_ov094_021f7f58(s32 type, u32 pos, fx32 x, fx32 y, fx32 z) {
    if (!(pos & 1)) {
        switch (type) {
        case 0:
        case 1:
            if (func_ov094_021f88f4()) {
                type += 4;
            }
            break;
        case 0x3d:
            type = 2;
            break;
        case 0x25:
            type = 3;
            break;
        default:
            if (data_ov094_02209d0c[type] == 0) {
                type = 0;
            } else {
                type = 1;
            }
            break;
        }
    } else {
        switch (type) {
        case 0:
        case 1:
            if (func_ov094_021f88f4()) {
                type += 0x67;
            }
            break;
        }
    }
    func_ov094_021ff088(data_ov094_0220af20->mcss, type, pos);
    if (x != 0 || y != 0 || z != 0) {
        func_ov094_021ff144(data_ov094_0220af20->mcss, pos, x, y, z);
    }
    data_ov094_0220af20->trainers[pos - 8] = pos;
}


void func_ov094_021f8010(u32 pos) {
    func_ov094_021ff0f8(data_ov094_0220af20->mcss, pos);
    data_ov094_0220af20->trainers[pos - 8] = 0xffffffff;
}

void func_ov094_021f803c(BtlMainModule *mainModule, BattleMon *mon, u32 viewPos) {
    switch (data_ov094_0220af20->setup.battleStyle) {
    case 2:
        func_ov094_022070c0(data_ov094_0220af20->gauge, mainModule, mon, 2, viewPos);
        return;
    case 3:
        func_ov094_022070c0(data_ov094_0220af20->gauge, mainModule, mon, 3, viewPos);
        return;
    default:
        func_ov094_022070c0(data_ov094_0220af20->gauge, mainModule, mon, 0, viewPos);
        return;
    }
}

void func_ov094_021f8090(PokeDexSave *pokedex, PartyPkm *pkm, u32 viewPos) {
    switch (data_ov094_0220af20->setup.battleStyle) {
    case 2:
        func_ov094_02207288(data_ov094_0220af20->gauge, pokedex, pkm, 2, viewPos);
        return;
    case 3:
        func_ov094_02207288(data_ov094_0220af20->gauge, pokedex, pkm, 3, viewPos);
        return;
    default:
        func_ov094_02207288(data_ov094_0220af20->gauge, pokedex, pkm, 0, viewPos);
        return;
    }
}

void func_ov094_021f80e4(u32 viewPos) {
    func_ov094_02207960(data_ov094_0220af20->gauge, viewPos);
}

void func_ov094_021f80fc(u32 viewPos, u32 exp) {
    func_ov094_02207b4c(data_ov094_0220af20->gauge, viewPos, exp);
}

void func_ov094_021f8118(u32 viewPos, s32 hp) {
    func_ov094_02207b68(data_ov094_0220af20->gauge, viewPos, hp);
}

void func_ov094_021f8134(u32 viewPos, u32 exp) {
    func_ov094_02207b84(data_ov094_0220af20->gauge, viewPos, exp);
}

// pokebw2's BtlvEffect_CalcGaugeExpLevelUp
void func_ov094_021f8150(u32 viewPos, BattleMon *mon) {
    func_ov094_02207bac(data_ov094_0220af20->gauge, mon, viewPos);
}

void func_ov094_021f8168(void) {
    func_ov094_022089ac(data_ov094_0220af20->gauge);
}


// pokebw2's BtlvEffect_CheckExecuteGauge
BOOL func_ov094_021f8180(void) {
    return func_ov094_02207c94(data_ov094_0220af20->gauge);
}

// pokebw2's BtlvEffect_SetGaugeDrawEnableBySide
void func_ov094_021f8198(BOOL visible, u32 side) {
    func_ov094_02207cc4(data_ov094_0220af20->gauge, visible, side);
}

// pokebw2's BtlvEffect_SetGaugeDrawEnable
void func_ov094_021f81b4(BOOL visible, u32 viewPos) {
    func_ov094_02207d04(data_ov094_0220af20->gauge, visible, viewPos);
}

// pokebw2's BtlvEffect_SetGaugeStatus
void func_ov094_021f81d0(u32 status, u8 pos) {
    func_ov094_02208814(data_ov094_0220af20->gauge, status, pos);
}

// pokebw2's BtlvEffect_GaugeAction
void func_ov094_021f81ec(u32 viewPos) {
    func_ov094_02208870(data_ov094_0220af20->gauge, viewPos);
}

// pokebw2's BtlvEffect_CheckGaugeExist
BOOL func_ov094_021f8204(u32 viewPos) {
    return func_ov094_02207d60(data_ov094_0220af20->gauge, viewPos);
}

// pokebw2's BtlvEffect_GetGaugeStatus
BOOL func_ov094_021f821c(u32 viewPos, u32 *color, u32 *status) {
    return func_ov094_02208958(data_ov094_0220af20->gauge, viewPos, color, status);
}

// pokebw2's BtlvEffect_AddBallGauge
void func_ov094_021f823c(const BtlvBGaugeParam *heapId) {
    BtlvBGauge *pBVar1;

    pBVar1 = func_ov094_02208dac(heapId, data_ov094_0220af20->heapId);
    data_ov094_0220af20->ballGauges[heapId->side] = pBVar1;
}

// pokebw2's BtlvEffect_DelBallGauge
void func_ov094_021f8260(u32 side) {
    func_ov094_02209068(data_ov094_0220af20->ballGauges[side]);
}

// pokebw2's BtlvEffect_IsBallGaugeBusy
BOOL func_ov094_021f827c(u32 side) {
    return func_ov094_022090a4(data_ov094_0220af20->ballGauges[side]);
}

// target: 0 the stage, 1 the field, 2 both, 3 the palettes, 4 all
// pokebw2's BtlvEffect_StartPaletteFade
void func_ov094_021f8298(u32 target, u8 evy, u8 targetEvy, u8 wait, u16 color) {
    if (target == 0 || target == 2 || target == 4) {
        func_ov094_021f69e8(data_ov094_0220af20->stage, evy, targetEvy, wait, color);
    }
    if (target == 1 || target == 2 || target == 4) {
        func_ov094_021f6dec(data_ov094_0220af20->field, evy, targetEvy, wait, color);
    }
    if (target == 3 || target == 4) {
        func_0202173c(data_ov094_0220af20->palFade, 1, 0xff00, wait, evy, targetEvy, color, data_ov094_0220af20->tcbManager);
    }
}

// pokebw2's BtlvEffect_IsPaletteFading
BOOL func_ov094_021f8324(u32 target) {
    BOOL stage = FALSE;
    BOOL field = FALSE;
    BOOL palette = FALSE;

    if (target == 0 || target == 2 || target == 4) {
        stage = func_ov094_021f6a18(data_ov094_0220af20->stage);
    }
    if (target == 1 || target == 2 || target == 4) {
        field = func_ov094_021f6e1c(data_ov094_0220af20->field);
    }
    if (target == 3 || target == 4) {
        palette = func_02021ed8(data_ov094_0220af20->palFade) != 0;
    }
    if (stage == TRUE || field == TRUE || palette == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// target: 0 the stage, 1 the field, 2 BG 3, 3 and 4 the stage's sides
// pokebw2's BtlvEffect_SetVanish
void func_ov094_021f839c(u32 target, BOOL hide) {
    switch (target) {
    case 0:
        func_ov094_021f6a28(data_ov094_0220af20->stage, 2, hide);
        break;
    case 1:
        func_ov094_021f6e2c(data_ov094_0220af20->field, hide);
        break;
    case 2:
        GFL_BGSysSetBGEnabled(3, hide ^ 1);
        break;
    case 3:
        func_ov094_021f6a28(data_ov094_0220af20->stage, 0, hide);
        break;
    case 4:
        func_ov094_021f6a28(data_ov094_0220af20->stage, 1, hide);
        break;
    }
}

// pokebw2's BtlvEffect_StartRotation
void func_ov094_021f8414(u8 mode, u32 side, u32 arg2) {
    BtlvEffectRotateWork *work =
        GFL_HeapAllocate(HEAPID_TAIL(data_ov094_0220af20->heapId), sizeof(BtlvEffectRotateWork), FALSE, "btlv_effect.c", 0x538);

    // BUG: mode 1 starts no task, so nothing frees its work
    if (mode != 1) {
        work->seq = 0;
        work->arg4 = mode != 3;
        work->side = side;
        work->argC = arg2;
        data_ov094_0220af20->rotateTasks |= func_ov094_021f935c(side);
        func_ov094_021f896c(GFL_TCBMgrAddTask(data_ov094_0220af20->tcbManager, func_ov094_021f8f30, work, 0),
                           func_ov094_021f8fb0, 0);
#ifdef BUGFIX
    } else {
        GFL_HeapFree(work);
#endif
    }
}

// pokebw2's BtlvEffect_SwapPokemon
void func_ov094_021f84b4(u8 pos1, u8 pos2) {
    func_ov094_021ff7b0(data_ov094_0220af20->mcss, pos1, pos2);
}

// pokebw2's BtlvEffect_GetTrainerIndex
s32 func_ov094_021f84d0(u32 pos) {
    return data_ov094_0220af20->trainers[pos - 8];
}

void func_ov094_021f84e8(s32 gameLimitTime, s32 cmdLimitTime) {
    func_ov094_02209318(data_ov094_0220af20->timer, gameLimitTime, cmdLimitTime);
}


// pokebw2's BtlvEffect_SetTimerVisible
void func_ov094_021f8504(int which, BOOL visible, BOOL restart) {
    func_ov094_022094bc(data_ov094_0220af20->timer, which, visible, restart);
}

// pokebw2's BtlvEffect_IsTimeUp
BOOL func_ov094_021f8524(int which) {
    return func_ov094_02209500(data_ov094_0220af20->timer, which);
}

// pokebw2's BtlvEffect_CheckExistPokemon
BOOL func_ov094_021f853c(u8 pos) {
    return func_ov094_021ff348(data_ov094_0220af20->mcss, pos);
}

// pokebw2's BtlvEffect_ZoomCamera
void func_ov094_021f8554(u32 pos, u32 mode, int frames, int wait, int brakeFrames) {
    VecFx32 camPos;
    VecFx32 camTarget;

    if (pos & 1) {
        func_ov094_021f8628(pos, &camPos, &camTarget);
        switch (mode) {
        case 0:
            func_ov094_021f6edc(data_ov094_0220af20->camera, &camPos, &camTarget);
            break;
        case 1:
            func_ov094_021f6f48(data_ov094_0220af20->camera, &camPos, &camTarget, frames, wait, brakeFrames);
            break;
        }
    } else {
        switch (data_ov094_0220af20->setup.battleStyle) {
        case 0:
            func_ov094_021f7998(0x23f);
            break;
        case 1:
            if (pos == 2) {
                func_ov094_021f7998(0x240);
            } else {
                func_ov094_021f7998(0x241);
            }
            break;
        case 2:
            if (pos == 2) {
                func_ov094_021f7998(0x242);
            } else if (pos == 4) {
                func_ov094_021f7998(0x23f);
            } else {
                func_ov094_021f7998(0x243);
            }
            break;
        case 3:
            func_ov094_021f7998(0x245);
            break;
        }
    }
}

// pokebw2's BtlvEffect_GetZoomCameraPosTarget
static void func_ov094_021f8628(u32 viewPos, VecFx32 *camPos, VecFx32 *camTarget) {
    func_ov094_021ff3f4(data_ov094_0220af20->mcss, camTarget, viewPos);
    camPos->x = camTarget->x + 0x4b33;
    camPos->y = camTarget->y + 0x2299;
    camPos->z = camTarget->z + 0x99cd;
    camTarget->y += FX32_CONST(2);
}

// pokebw2's BtlvEffect_GetWork
BtlvEffect *func_ov094_021f8678(void) {
    return data_ov094_0220af20;
}

// pokebw2's BtlvEffect_GetCamera
BtlvCamera *func_ov094_021f8684(void) {
    return data_ov094_0220af20->camera;
}

// pokebw2's BtlvEffect_GetMcss
BtlvMcss *func_ov094_021f8694(void) {
    return data_ov094_0220af20->mcss;
}

// pokebw2's BtlvEffect_GetStage
BtlvStage *func_ov094_021f86a4(void) {
    return data_ov094_0220af20->stage;
}

// pokebw2's BtlvEffect_GetGauge
BtlvGauge *func_ov094_021f86b4(void) {
    return data_ov094_0220af20->gauge;
}

// pokebw2's BtlvEffect_GetEffvm
VM *func_ov094_021f86c4(void) {
    return data_ov094_0220af20->effvm;
}

// pokebw2's BtlvEffect_GetTCBManager
TCBManager *func_ov094_021f86d4(void) {
    return data_ov094_0220af20->tcbManager;
}

// pokebw2's BtlvEffect_GetPaletteFade
PaletteFade *func_ov094_021f86e0(void) {
    return data_ov094_0220af20->palFade;
}

// pokebw2's BtlvEffect_GetClact
BtlvClact *func_ov094_021f86f0(void) {
    return data_ov094_0220af20->clact;
}

// pokebw2's BtlvEffect_GetBg
BtlvBg *func_ov094_021f8700(void) {
    return data_ov094_0220af20->bg;
}

// pokebw2's BtlvEffect_GetBattleStyle
u32 func_ov094_021f8710(void) {
    return data_ov094_0220af20->setup.battleStyle;
}

// pokebw2's BtlvEffect_GetBattleType
u32 func_ov094_021f8720(void) {
    return data_ov094_0220af20->setup.battleType;
}

// pokebw2's BtlvEffect_IsMulti
BOOL func_ov094_021f8730(void) {
    return data_ov094_0220af20->setup.multi;
}

// pokebw2's BtlvEffect_GetTrainerType
u16 func_ov094_021f8740(u32 pos) {
    return data_ov094_0220af20->setup.trainerTypes[pos];
}

// pokebw2's BtlvEffect_GetMainModule
BtlMainModule *func_ov094_021f8754(void) {
    return data_ov094_0220af20->setup.mainModule;
}

// pokebw2's BtlvEffect_GetScu
BtlvScu *func_ov094_021f8764(void) {
    return data_ov094_0220af20->setup.scu;
}

BOOL func_ov094_021f8774(void) {
    return func_ov094_022088f4(data_ov094_0220af20->gauge);
}

void func_ov094_021f878c(u32 bgm) {
    if (func_ov094_022088f4(data_ov094_0220af20->gauge)) {
        func_ov094_02208900(data_ov094_0220af20->gauge, FALSE);
        GFL_SndBGMPop();
    }
    func_ov094_0220893c(data_ov094_0220af20->gauge, TRUE);
    GFL_SndBGMPlay(bgm, 0xffff);
}

void func_ov094_021f87d4(BOOL noPinchBgm) {
    func_ov094_0220893c(data_ov094_0220af20->gauge, noPinchBgm);
}

void func_ov094_021f87ec(u32 arg0) {
    data_ov094_0220af20->bgmChanged = 1;
    func_ov094_02208918(data_ov094_0220af20->gauge, arg0);
    if (func_ov094_022088f4(data_ov094_0220af20->gauge) == 0) {
        func_ov094_02208924(data_ov094_0220af20->gauge, 1);
    }
}

BOOL func_ov094_021f882c(void) {
    return data_ov094_0220af20->bgmChanged;
}

void func_ov094_021f883c(u32 bgm) {
    if (func_ov094_022088f4(data_ov094_0220af20->gauge) == TRUE) {
        GFL_SndBGMPop();
    }
    func_ov094_0220893c(data_ov094_0220af20->gauge, TRUE);
    func_ov094_02208900(data_ov094_0220af20->gauge, FALSE);
    GFL_SndBGMPlay(bgm, 0xffff);
}

// pokebw2's BtlvEffect_SetState
void func_ov094_021f8884(u32 state) {
    data_ov094_0220af20->state = state;
}

// pokebw2's BtlvEffect_GetState
u32 func_ov094_021f88a8(void) {
    return data_ov094_0220af20->state;
}

// pokebw2's BtlvEffect_SetFlag26
void func_ov094_021f88bc(u32 arg0) {
    data_ov094_0220af20->unk1EC_26 = arg0;
}

// pokebw2's BtlvEffect_GetFlag26
u32 func_ov094_021f88e0(void) {
    return data_ov094_0220af20->unk1EC_26;
}

BOOL func_ov094_021f88f4(void) {
    BOOL BVar1;

    BVar1 = 0;
    if (data_ov094_0220af20->setup.mainModule != (BtlMainModule *)0x0) {
        BVar1 = func_ov093_021ba1ec(data_ov094_0220af20->setup.mainModule);
    }
    return BVar1;
}

BOOL func_ov094_021f8914(void) {
    BOOL BVar1;

    BVar1 = 0;
    if (data_ov094_0220af20->setup.mainModule != (BtlMainModule *)0x0) {
        BVar1 = func_ov093_021ba20c(data_ov094_0220af20->setup.mainModule);
    }
    return BVar1;
}

// pokebw2's BtlvEffect_SetPokemonCheck
void func_ov094_021f8934(u32 arg0) {
    BtlvMcssPos pos;

    for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_TRAINER; pos++) {
        if (func_ov094_021ffd9c(data_ov094_0220af20->mcss, pos)) {
            func_ov094_0220055c(data_ov094_0220af20->mcss, pos, arg0);
        }
    }
}

// pokebw2's BtlvEffect_AddTask
void func_ov094_021f896c(TCB *tcb, void (*endFunc)(TCB *tcb), u32 group) {
    int slot = func_ov094_021f8b40();

    data_ov094_0220af20->tasks[slot] = tcb;
    data_ov094_0220af20->taskCallbacks[slot] = endFunc;
    data_ov094_0220af20->taskGroups[slot] = group;
}

// pokebw2's BtlvEffect_FindTask
static int func_ov094_021f899c(TCB *tcb) {
    int i;

    for (i = 0; i < BTLV_EFFECT_TASK_MAX; i++) {
        if (data_ov094_0220af20->tasks[i] == tcb) {
            break;
        }
    }
    return i;
}

// pokebw2's BtlvEffect_EndTask
void func_ov094_021f89bc(TCB *tcb) {
    int slot = func_ov094_021f899c(tcb);
    void *data;

    if (tcb != NULL) {
        data = GFL_TCBGetData(tcb);
        if (data_ov094_0220af20->taskCallbacks[slot] != NULL) {
            data_ov094_0220af20->taskCallbacks[slot](tcb);
        }
        if (data != NULL) {
            GFL_HeapFree(data);
        }
        GFL_TCBRemove(tcb);
        data_ov094_0220af20->tasks[slot] = NULL;
        data_ov094_0220af20->taskCallbacks[slot] = NULL;
    }
}

// pokebw2's BtlvEffect_EndTaskGroup
void func_ov094_021f8a10(u32 group) {
    int i;

    for (i = 0; i < BTLV_EFFECT_TASK_MAX; i++) {
        if (data_ov094_0220af20->tasks[i] != NULL && group == data_ov094_0220af20->taskGroups[i]) {
            func_ov094_021f89bc(data_ov094_0220af20->tasks[i]);
        }
    }
}

// pokebw2's BtlvEffect_SetIdleEffectMode
void func_ov094_021f8a40(u32 arg0) {
    data_ov094_0220af20->idleEffectMode = arg0;
    if (data_ov094_0220af20->idleEffectMode != 2) {
        data_ov094_0220af20->idleEffectSeq = 0;
        data_ov094_0220af20->idleEffectTimer = 0;
    }
    if (arg0 == 3) {
        func_ov094_02200608(data_ov094_0220af20->mcss);
    }
}

// pokebw2's BtlvEffect_StopIdleEffect
void func_ov094_021f8a78(void) {
    func_ov094_021f7bec();
    if (data_ov094_0220af20->idleEffectMode == 3 && BtlSetup_IsBattleType(data_ov094_0220af20->setup.mainModule, 0x400)) {
        func_ov094_021f7998(0x251);
    } else {
        func_ov094_021f7998(0x236);
    }
    data_ov094_0220af20->idleEffectMode = 0;
}

// pokebw2's BtlvEffect_RestartIdleEffect
void func_ov094_021f8ac0(u32 mode) {
    func_ov094_021f7bec();
    data_ov094_0220af20->idleEffectMode = mode;
    data_ov094_0220af20->idleEffectSeq = 0;
    data_ov094_0220af20->idleEffectTimer = 0;
}

// pokebw2's BtlvEffect_SetAbility
void func_ov094_021f8ae8(u32 viewPos, u32 ability) {
    data_ov094_0220af20->abilities[viewPos] = ability;
}

// pokebw2's BtlvEffect_GetAbility
u32 func_ov094_021f8afc(u8 viewPos) {
    return data_ov094_0220af20->abilities[viewPos];
}

// pokebw2's BtlvEffect_ReleaseVoices
void func_ov094_021f8b10(void) {
    func_ov094_021f9a7c(data_ov094_0220af20->effvm);
}

// pokebw2's BtlvEffect_ClearVoices
void func_ov094_021f8b28(void) {
    func_ov094_021f9aa4(data_ov094_0220af20->effvm);
}

// pokebw2's BtlvEffect_GetFreeTaskSlot
int func_ov094_021f8b40(void) {
    int i;

    for (i = 0; i < BTLV_EFFECT_TASK_MAX; i++) {
        if (data_ov094_0220af20->tasks[i] == NULL) {
            break;
        }
    }
    if (i == BTLV_EFFECT_TASK_MAX) {
        func_ov094_021f89bc(data_ov094_0220af20->tasks[0]);
        i = 0;
    }
    return i;
}

// pokebw2's BtlvEffect_EndAllTasks
static void func_ov094_021f8b6c(void) {
    int i;

    for (i = 0; i < BTLV_EFFECT_TASK_MAX; i++) {
        if (data_ov094_0220af20->tasks[i] != NULL) {
            func_ov094_021f89bc(data_ov094_0220af20->tasks[i]);
        }
    }
}

// pokebw2's BtlvEffect_VBlankTask
void func_ov094_021f8b90(TCB *tcb, void *data) {
    func_0204a648();
    func_02021d50(data_ov094_0220af20->palFade);
}

// pokebw2's BtlvEffect_BlinkTask
static void func_ov094_021f8bac(TCB *tcb, void *data) {
    BtlvEffectDamage *work = data;

    if (work->wait != 0) {
        work->wait--;
        return;
    }
    switch (work->seq) {
    case 0:
        if (!work->hidden) {
            func_ov094_021ffdb4(data_ov094_0220af20->mcss, work->pos, 16, 16, 0, work->color);
        }
        work->wait = 3;
        work->seq = 1;
        break;
    case 1:
        if (!work->hidden) {
            func_ov094_021ff374(data_ov094_0220af20->mcss, work->pos, TRUE);
        }
        work->wait = 3;
        work->seq = 2;
        break;
    case 2:
        work->seq = 0;
        if (!work->hidden) {
            func_ov094_021ff374(data_ov094_0220af20->mcss, work->pos, FALSE);
            func_ov094_021ffdb4(data_ov094_0220af20->mcss, work->pos, 0, 0, 0, 0x7fff);
        }
        work->wait = 3;
        if (--work->count == 0) {
            func_ov094_021f89bc(tcb);
        }
        break;
    }
}

// pokebw2's BtlvEffect_DamageTaskEnd
static void func_ov094_021f8c78(TCB *tcb) {
    u32 *viewPos = GFL_TCBGetData(tcb);

    data_ov094_0220af20->damageTasks &= func_ov094_021f935c(*viewPos) ^ 0xffffffff;
}

// pokebw2's BtlvEffect_SubstituteTask
void func_ov094_021f8cb0(TCB *tcb, void *data) {
    BtlvEffectChange *work = data;
    BOOL BVar1;
    u32 result;

    switch(work->seq) {
    case 0:
        func_ov094_021f94f8(data_ov094_0220af20->effvm, work->viewPos, 0xff, 0x253, NULL);
        data_ov094_0220af20->executing = 1;
        work->seq++;
        return;
    case 1:
        if (data_ov094_0220af20->executing == 0) {
            result = func_ov094_021fff38(data_ov094_0220af20->mcss, work->viewPos);
            if ((result & 1) != 0) {
                func_ov094_021f94f8(data_ov094_0220af20->effvm, work->viewPos, 0xff, 0x264, NULL);
                data_ov094_0220af20->executing = 1;
            }
            work->seq++;
            return;
        }
        break;
    case 2:
        if (data_ov094_0220af20->executing == 0) {
            func_ov094_021ff924(data_ov094_0220af20->mcss, work->viewPos, 0, 0x10, 0, 0, 0);
            func_ov094_021ffbbc(data_ov094_0220af20->mcss, work->viewPos, 1, 8, 8, 1, 0);
            func_ov094_021f9858(data_ov094_0220af20->effvm, 0x5aa, 1, 0xe, -500, 0x7f, 0, 0, 0);
            func_ov094_021f9948(data_ov094_0220af20->effvm, 1, 0, 1, 0x7f, 0, 0x1e, 10, 0, 0);
            work->seq++;
            return;
        }
        break;
    case 3:
        BVar1 = func_ov094_021ffcb0(data_ov094_0220af20->mcss, work->viewPos);
        if (BVar1 == 0) {
            func_ov094_022001dc(data_ov094_0220af20->mcss, work->viewPos, &work->info);
            work->seq++;
            return;
        }
        break;
    case 4:
        func_ov094_021ffbbc(data_ov094_0220af20->mcss, work->viewPos, 1, 0, 8, 1, 0);
        work->seq++;
        return;
    case 5:
        BVar1 = func_ov094_021ffcb0(data_ov094_0220af20->mcss, work->viewPos);
        if (BVar1 == 0) {
            func_ov094_021f9858(data_ov094_0220af20->effvm, 0x560, 1, 0xe, 0, 0, 0, 0, 0);
            func_ov094_021ff924(data_ov094_0220af20->mcss, work->viewPos, 0, 0x1f, 0, 0, 0);
            result = func_ov094_021fff38(data_ov094_0220af20->mcss, work->viewPos);
            if ((result & 1) != 0) {
                func_ov094_021f94f8(data_ov094_0220af20->effvm, work->viewPos, 0xff, 0x265, NULL);
                data_ov094_0220af20->executing = 1;
            }
            work->seq++;
            return;
        }
        break;
    case 6:
        if (data_ov094_0220af20->executing == 0) {
            func_ov094_021f94f8(data_ov094_0220af20->effvm, work->viewPos, 0xff, 0x254, NULL);
            data_ov094_0220af20->executing = 1;
            work->seq++;
            return;
        }
        break;
    case 7:
        if (data_ov094_0220af20->executing == 0) {
            func_ov094_021f89bc(tcb);
        }
    }
}

// pokebw2's BtlvEffect_Unk8TaskEnd
static void func_ov094_021f8ef8(TCB *tcb) {
    u32 *viewPos = GFL_TCBGetData(tcb);

    data_ov094_0220af20->unkTasks8 &= func_ov094_021f935c(*viewPos) ^ 0xffffffff;
}

// pokebw2's BtlvEffect_RotateTask
static void func_ov094_021f8f30(TCB *tcb, void *data) {
    BtlvEffectRotateWork *work = data;

    switch (work->seq) {
    case 0:
        if ((data_ov094_0220af20->damageTasks | data_ov094_0220af20->executing | data_ov094_0220af20->unkTasks8) != 0) {
            break;
        }
        work->seq++;
        // fallthrough
    case 1:
        func_ov094_02200344(data_ov094_0220af20->mcss, work->side, work->arg4, work->argC);
        work->seq++;
        break;
    case 2:
        if (!func_ov094_021f6ad0(data_ov094_0220af20->stage) && !func_ov094_021ffd6c(data_ov094_0220af20->mcss)) {
            func_ov094_021f89bc(tcb);
        }
        break;
    }
}

// pokebw2's BtlvEffect_RotateTaskEnd
static void func_ov094_021f8fb0(TCB *tcb) {
    BtlvEffectRotateWork *work = GFL_TCBGetData(tcb);

    data_ov094_0220af20->rotateTasks &= func_ov094_021f935c(work->side) ^ 0xffffffff;
}

// pokebw2's BtlvEffect_DelayedMoveTask
static void func_ov094_021f8fe8(TCB *tcb, void *data) {
    BtlvEffectQueued *work = data;

    if (!func_ov094_021f7eb4()) {
        func_ov094_021f94f8(data_ov094_0220af20->effvm, work->atkPos, work->defPos, work->effNo, work->param);
        data_ov094_0220af20->executing = TRUE;
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffect_DelayedMoveTaskEnd
static void func_ov094_021f9028(TCB *tcb) {
    BtlvEffectQueued *work = GFL_TCBGetData(tcb);

    if (work->param != NULL) {
        GFL_HeapFree(work->param);
    }
}

// pokebw2's BtlvEffect_UpdateIdleEffect
static void func_ov094_021f903c(void) {
    u32 keys = GCTX_HIDGetPressedKeys();
    BOOL touched = func_02035c60();
    s32 pick;
    u32 effect;

    if (data_ov094_0220af20->idleEffectMode == 0) {
        return;
    }
    if (data_ov094_0220af20->idleEffectMode != 2 && data_ov094_0220af20->idleEffectMode != 3 && (keys != 0 || touched)) {
        func_ov094_021f7bec();
        func_ov094_021f7998(0x252);
        data_ov094_0220af20->idleEffectSeq = 0;
        data_ov094_0220af20->idleEffectTimer = 0;
    }
    switch (data_ov094_0220af20->idleEffectSeq) {
    case 0:
        if (data_ov094_0220af20->idleEffectTimer < data_ov094_0220af20->idleEffectWait) {
            data_ov094_0220af20->idleEffectTimer++;
            return;
        }
        data_ov094_0220af20->idleEffectSeq = 1;
        // fallthrough
    case 1:
        if (func_ov094_021f7eb4()) {
            break;
        }
        if (data_ov094_0220af20->idleEffectMode == 3 && BtlSetup_IsBattleType(data_ov094_0220af20->setup.mainModule, 0x400)) {
            do {
                pick = GFL_RandomMTRange(2);
            } while (data_ov094_0220af20->idleEffectLast == pick);
            data_ov094_0220af20->idleEffectLast = pick;
            effect = data_ov094_0220ac70[data_ov094_0220af20->idleEffectLast];
        } else {
            do {
                pick = GFL_RandomMTRange(6);
            } while (data_ov094_0220af20->idleEffectLast == pick);
            data_ov094_0220af20->idleEffectLast = pick;
            effect = data_ov094_0220ac78[data_ov094_0220af20->idleEffectLast];
        }
        func_ov094_021f7998(effect);
        data_ov094_0220af20->executing = TRUE;
        break;
    }
}

// pokebw2's BtlvEffTool_CalcStep
void func_ov094_021f9154(fx32 start, fx32 end, fx32 *step, fx32 frames) {
    *step = 0;
    if (end - start != 0) {
        if (frames == 0) {
            frames = FX32_ONE;
        }
        *step = FX_Div(end - start, frames);
        if (*step == 0) {
            if (end > start) {
                *step = 1;
            } else {
                *step = -1;
            }
        }
    }
}

// pokebw2's BtlvEffTool_CalcStepVec
void func_ov094_021f918c(const VecFx32 *start, const VecFx32 *end, VecFx32 *step, fx32 frames) {
    step->x = 0;
    step->y = 0;
    step->z = 0;
    if (frames == 0) {
        frames = FX32_ONE;
    }
    if (end->x - start->x != 0) {
        step->x = FX_Div(end->x - start->x, frames);
        if (step->x == 0) {
            if (end->x > start->x) {
                step->x = 1;
            } else {
                step->x = -1;
            }
        }
    }
    if (end->y - start->y != 0) {
        step->y = FX_Div(end->y - start->y, frames);
        if (step->y == 0) {
            if (end->y > start->y) {
                step->y = 1;
            } else {
                step->y = -1;
            }
        }
    }
    if (end->z - start->z != 0) {
        step->z = FX_Div(end->z - start->z, frames);
        if (step->z == 0) {
            if (end->z > start->z) {
                step->z = 1;
            } else {
                step->z = -1;
            }
        }
    }
}

// pokebw2's BtlvEffTool_Step
void func_ov094_021f9220(fx32 *value, fx32 *step, const fx32 *end, BOOL *done) {
    *value += *step;
    if (*step < 0) {
        if (*value <= *end) {
            *value = *end;
        } else if (done != NULL) {
            *done = FALSE;
        }
    } else {
        if (*value >= *end) {
            *value = *end;
        } else if (done != NULL) {
            *done = FALSE;
        }
    }
}

// pokebw2's BtlvEffTool_Move
BOOL func_ov094_021f9260(BtlvEffToolMove *move, VecFx32 *value) {
    BOOL done = TRUE;

    switch (move->type) {
    case 0:
        value->x = move->end.x;
        value->y = move->end.y;
        value->z = move->end.z;
        break;
    case 1:
    case 4:
        if (move->wait == 0) {
            move->wait = move->waitReset;
            func_ov094_021f9220(&value->x, &move->step.x, &move->end.x, &done);
            func_ov094_021f9220(&value->y, &move->step.y, &move->end.y, &done);
            func_ov094_021f9220(&value->z, &move->step.z, &move->end.z, &done);
        } else {
            move->wait--;
            done = FALSE;
        }
        break;
    case 2:
    case 3:
        if (move->wait == 0) {
            move->wait = move->waitReset;
            value->x += move->step.x;
            value->y += move->step.y;
            value->z += move->step.z;
            if (--move->stepTime == 0) {
                move->count--;
                move->stepTime = move->stepTimeReset;
                if (move->type == 2 || (move->type == 3 && (move->count & 1))) {
                    move->step.x *= -1;
                    move->step.y *= -1;
                    move->step.z *= -1;
                }
            }
        } else {
            move->wait--;
        }
        if (move->count != 0) {
            done = FALSE;
        } else {
            value->x = move->start.x;
            value->y = move->start.y;
            value->z = move->start.z;
        }
        break;
    }
    return done;
}

// pokebw2's BtlvEffect_PosBit
u32 func_ov094_021f935c(u32 bit) {
    return 1 << bit;
}

void BtlvEffect_UpdateTexPaletteFade(BtlvTexPaletteFade *fade) {
    int i;
    NNSG3dResTex *tex;
    u32 size;
    u32 addr;

    if (fade->active) {
        if (fade->wait == 0) {
            fade->wait = fade->waitFrames;
            for (i = 0; i < fade->count; i++) {
                tex = NNS_G3dGetTex(GFL_G3DResGetResData(fade->resources[i]));
                size = tex->plttInfo.sizePltt << 3;
                addr = NNS_GfdGetPlttKeyAddr(tex->plttInfo.vramKey);
                func_02021f64((u16 *)((u8 *)tex + tex->plttInfo.ofsPlttData), fade->palettes[i], size / sizeof(u16),
                            fade->evy, fade->color);
                NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, addr, fade->palettes[i], size);
            }
            if (fade->evy == fade->targetEvy) {
                fade->active = FALSE;
            } else if (fade->evy > fade->targetEvy) {
                fade->evy--;
            } else {
                fade->evy++;
            }
        } else {
            fade->wait--;
        }
    }
}
