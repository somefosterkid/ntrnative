// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_mcss.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle view's Pokémon sprites, drawn by MCSS. The name is the ROM's string, from GFL_HeapAllocate's asserts. The
// names are ours; swan has none for this file

#include "battle/btlv_mcss.h"
#include "types.h"
#include "battle/btl_main.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_effvm.h"
#include "battle/btlv_gauge.h"
#include "battle/btlv_stage.h"
#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "pml/personal.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "system/hp_gauge.h"
#include "system/mcss.h"

// The data of the Pokémon a sprite shows, copied whole when a Pokémon transforms
typedef struct {
    u32 species;     // 0x00
    u32 form;        // 0x04
    u32 personality; // 0x08
    u16 weight;      // 0x0c
    u16 hpColor;     // 0x0e  func_0202d034's
} BtlvMcssMon;

// A sprite, 0x5c bytes
typedef struct {
    MCSS *mcss;         // 0x00
    MCSSLoadInfo info;  // 0x04
    TCB *idleTask;      // 0x28  func_ov094_02201304, the idle animation
    BtlvMcssMon mon;    // 0x2c
    u32 ball;           // 0x3c
    u32 flags;          // 0x40  BTLV_MCSS_FLAG_*
    BOOL animSet;       // 0x44  an animation was set and hasn't ended
    s32 pos;            // 0x48  0xff for none
    u32 frozen : 1;     // 0x4c  the animation was stopped for good (freeze)
    u32 statusAnim : 1; //       a status condition's animation runs
    u32 flat : 1;       //       BtlvMcss.flat when added
    u32 idleCount : 2;  //       the chance of an idle animation when the animation ends goes up as it counts down
    u32 hidden : 1;     //       hidden by func_ov094_021ff374's mode 3, shown back by mode 4
    u32 moved : 1;      //       func_ov094_022004bc put the sprite back
    u32 fadeDone : 1;   //       the palette fade has ended
    u32 fading : 1;     //       func_ov094_021ffdb4 started a palette fade
    u32 changed : 1;    //       func_ov094_021ffe48 changed the Pokémon
    u32 vanished : 1;   //       saved by func_ov094_02200608
    u32 animEnded : 1;  //       restart the animation next frame
    s32 statusEvy;      // 0x50  the status condition's palette blend, swinging between 0 and 12
    s32 statusEvyStep;  // 0x54
    s32 statusTimer;    // 0x58  frames until the next step
} BtlvMcssEntry;

// The sprites' work, 0x5b4 bytes. Each kind of task keeps a mask of the positions it runs on, and a counter by position
// that a new task of the kind bumps, so that an older task on the position stops
struct BtlvMcss {
    TCBManager *tcbMgr;                       // 0x000
    MCSSSystem *mcssSys;                      // 0x004
    BtlvMcssEntry entries[BTLV_MCSS_POS_MAX]; // 0x008  slots, found by their pos
    u32 triple : 1;                           // 0x510
    u32 rotation : 1;                         //
    u32 rotating : 1;                         //        func_ov094_02200344's task runs
    u32 moveTasks;                            // 0x514  the position, a circle or a shake
    u32 unk518Tasks;                          // 0x518  func_02015cdc's vector
    u32 unk51cTasks;                          // 0x51c  func_02015d14's vector
    u32 scaleTasks;                           // 0x520
    u32 unk524Tasks;                          // 0x524  func_02015d64's vector
    u32 blinkTasks;                           // 0x528
    u32 alphaTasks;                           // 0x52c
    u32 levelTasks;                           // 0x530  func_02016078's level
    u32 flat;                                 // 0x534
    fx32 scaleNear;                           // 0x538  the even positions' scale
    fx32 scaleFar;                            // 0x53c  the odd positions'
    HeapID heapId;                            // 0x540
    u8 moveSeq[BTLV_MCSS_POS_MAX];            // 0x542
    u8 unk518Seq[BTLV_MCSS_POS_MAX];          // 0x550
    u8 scaleSeq[BTLV_MCSS_POS_MAX];           // 0x55e
    u8 unk524Seq[BTLV_MCSS_POS_MAX];          // 0x56c
    u8 unk51cSeq[BTLV_MCSS_POS_MAX];          // 0x57a
    u8 blinkSeq[BTLV_MCSS_POS_MAX];           // 0x588
    u8 alphaSeq[BTLV_MCSS_POS_MAX];           // 0x596
    u8 levelSeq[BTLV_MCSS_POS_MAX];           // 0x5a4
};

// A move of a value of a sprite toward a goal, or a swing, or a blink, 0x54 bytes
typedef struct {
    BtlvMcss *work;       // 0x00
    s32 pos;              // 0x04
    VecFx32 value;        // 0x08
    BtlvEffToolMove move; // 0x14
    u32 seq;              // 0x50
} BtlvMcssMove;

// The rotation of a side in a rotation battle, 0x24 bytes
typedef struct {
    BtlvMcss *work; // 0x00
    u32 seq : 31;   // 0x04
    u32 playSe : 1; //
    s32 side;       // 0x08
    s32 dir;        // 0x0c
    s32 angle[3];   // 0x10
    s32 frames;     // 0x1c
    s32 speed;      // 0x20
} BtlvMcssRotate;

// The idle animation of a sprite, 0xc bytes
typedef struct {
    s32 seq;   // 0x0
    s32 index; // 0x4
    s32 wait;  // 0x8
} BtlvMcssIdle;

// The shadow of a species' sprite, where it differs
typedef struct {
    u16 species : 14;
    u16 enemy : 1;  // the front sprite's, otherwise the back sprite's
    u16 shadow : 1; // whether it has one
    s16 x;
    s16 z;
    u16 unk6;
} BtlvMcssShadow;

static void func_ov094_021ff260(BtlvMcss *work, int pos, int mode);
BOOL func_ov094_021ff2d8(BtlvMcss *work, int pos);
void func_ov094_021ffdf8(BtlvMcss *work, int pos, u8 evy, GXRgb color);
static void func_ov094_021ffe24(BtlvMcss *work, int pos);
static void func_ov094_02200108(BtlvMcss *work, int pos, int level);
void func_ov094_022006a0(u32 trainerType, MCSSLoadInfo *info, int pos);
static void func_ov094_022006b4(BtlvMcss *work, int pos);
static void func_ov094_0220074c(BtlvMcss *work, int pos, int type, VecFx32 *start, VecFx32 *end, s32 frames,
                                   s32 wait, s32 count, TCBFunc func, void (*endFunc)(TCB *tcb), BOOL mirror, u8 seq);
static void func_ov094_02200904(TCB *tcb, void *data);
static void func_ov094_02200990(TCB *tcb);
static void func_ov094_022009c8(TCB *tcb, void *data);
static void func_ov094_02200a50(TCB *tcb);
static void func_ov094_02200a84(TCB *tcb, void *data);
static void func_ov094_02200b10(TCB *tcb);
static void func_ov094_02200b48(TCB *tcb, void *data);
static void func_ov094_02200bd4(TCB *tcb);
static void func_ov094_02200c0c(TCB *tcb, void *data);
static void func_ov094_02200c98(TCB *tcb);
static void func_ov094_02200cd0(TCB *tcb, void *data);
static void func_ov094_02200d30(TCB *tcb);
static void func_ov094_02200d68(TCB *tcb, void *data);
static void func_ov094_02200de4(TCB *tcb);
static void func_ov094_02200e1c(TCB *tcb, void *data);
static void func_ov094_02201098(TCB *tcb);
static void func_ov094_022010d0(TCB *tcb, void *data);
static void func_ov094_022011fc(TCB *tcb);
static void func_ov094_02201234(TCB *tcb, void *data);
static void func_ov094_022012cc(TCB *tcb);
static void func_ov094_02201304(TCB *tcb, void *data);
static void func_ov094_02201408(TCB *tcb, void *data);
static void func_ov094_02201700(TCB *tcb);
static void func_ov094_02201718(uintptr_t index, fx32 frame);
static BOOL func_ov094_02201810(u32 param, const NNSG2dMultiCellHierarchyData *node,
                                      NNSG2dCellAnimation *cellAnim, u16 nodeIdx);
static void func_ov094_02201894(uintptr_t index, fx32 frame);
static void func_ov094_022018a8(BtlvMcss *work, VecFx32 *out, int pos);
static fx32 func_ov094_02201934(BtlvMcss *work, int pos, BOOL flat);
static int func_ov094_022019f4(BtlvMcss *work, int pos);
BOOL func_ov094_02201a20(u32 species, int pos, VecFx32 *offset, u16 *unk6, BOOL *shadow);
BOOL func_ov094_021ffd9c(BtlvMcss *work, int pos);
void func_ov094_022003ec(BtlvMcss *work, PartyPkm *pkm, MCSSLoadInfo *info, int pos);
void func_ov094_0220055c(BtlvMcss *work, int pos, BOOL check);
u8 func_0202d034(u32 hp, u32 maxHp);
void func_ov094_021ff304(BtlvMcss *work, int pos, int mode);
void func_ov094_021ff870(BtlvMcss *work, int pos, int type, s32 wait, s32 count);
u32 func_ov094_021fff38(BtlvMcss *work, int pos);
void func_ov094_021ff498(BtlvMcss *work, int pos, fx32 speed);

// The cries' pan, the sprites' scale and their positions, by position: singles from 0, doubles, triples and rotation
// from 2, and the trainers from 8, as func_ov094_02200218, func_ov094_02201934 and func_ov094_022018a8
// pick them. The declaration order lays them out as in the ROM
static const union { u8 raw[8]; s32 t[2]; } data_ov094_02209fc0_raw = { {
#include "asset/btlv_mcss_data_ov094_02209fc0.inc"
} };
typedef char data_ov094_02209fc0_fits[sizeof(data_ov094_02209fc0_raw.t) <= sizeof(data_ov094_02209fc0_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_02209fc0 (data_ov094_02209fc0_raw.t)
static const union { u8 raw[8]; fx32 t[2]; } data_ov094_02209fc8_raw = { {
#include "asset/btlv_mcss_data_ov094_02209fc8.inc"
} };
typedef char data_ov094_02209fc8_fits[sizeof(data_ov094_02209fc8_raw.t) <= sizeof(data_ov094_02209fc8_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_02209fc8 (data_ov094_02209fc8_raw.t)
static const union { u8 raw[16]; fx32 t[4]; } data_ov094_02209fd0_raw = { {
#include "asset/btlv_mcss_data_ov094_02209fd0.inc"
} };
typedef char data_ov094_02209fd0_fits[sizeof(data_ov094_02209fd0_raw.t) <= sizeof(data_ov094_02209fd0_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_02209fd0 (data_ov094_02209fd0_raw.t)
static const union { u8 raw[16]; s32 t[4]; } data_ov094_02209fe0_raw = { {
#include "asset/btlv_mcss_data_ov094_02209fe0.inc"
} };
typedef char data_ov094_02209fe0_fits[sizeof(data_ov094_02209fe0_raw.t) <= sizeof(data_ov094_02209fe0_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_02209fe0 (data_ov094_02209fe0_raw.t)
static const union { u8 raw[24]; fx32 t[6]; } data_ov094_0220a020_raw = { {
#include "asset/btlv_mcss_data_ov094_0220a020.inc"
} };
typedef char data_ov094_0220a020_fits[sizeof(data_ov094_0220a020_raw.t) <= sizeof(data_ov094_0220a020_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_0220a020 (data_ov094_0220a020_raw.t)
static const union { u8 raw[24]; s32 t[6]; } data_ov094_02209ff0_raw = { {
#include "asset/btlv_mcss_data_ov094_02209ff0.inc"
} };
typedef char data_ov094_02209ff0_fits[sizeof(data_ov094_02209ff0_raw.t) <= sizeof(data_ov094_02209ff0_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_02209ff0 (data_ov094_02209ff0_raw.t)
static const VecFx32 data_ov094_0220a080[] = {
#include "asset/btlv_mcss_data_ov094_0220a080.inc"
};
static const union { u8 raw[24]; fx32 t[6]; } data_ov094_0220a008_raw = { {
#include "asset/btlv_mcss_data_ov094_0220a008.inc"
} };
typedef char data_ov094_0220a008_fits[sizeof(data_ov094_0220a008_raw.t) <= sizeof(data_ov094_0220a008_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_0220a008 (data_ov094_0220a008_raw.t)
static const union { u8 raw[24]; fx32 t[6]; } data_ov094_0220a038_raw = { {
#include "asset/btlv_mcss_data_ov094_0220a038.inc"
} };
typedef char data_ov094_0220a038_fits[sizeof(data_ov094_0220a038_raw.t) <= sizeof(data_ov094_0220a038_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_0220a038 (data_ov094_0220a038_raw.t)
static const union { u8 raw[24]; VecFx32 t[2]; } data_ov094_0220a050_raw = { {
#include "asset/btlv_mcss_data_ov094_0220a050.inc"
} };
typedef char data_ov094_0220a050_fits[sizeof(data_ov094_0220a050_raw.t) <= sizeof(data_ov094_0220a050_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_0220a050 (data_ov094_0220a050_raw.t)
static const union { u8 raw[24]; s32 t[6]; } data_ov094_0220a068_raw = { {
#include "asset/btlv_mcss_data_ov094_0220a068.inc"
} };
typedef char data_ov094_0220a068_fits[sizeof(data_ov094_0220a068_raw.t) <= sizeof(data_ov094_0220a068_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_0220a068 (data_ov094_0220a068_raw.t)
static const VecFx32 data_ov094_0220a1a0[] = {
#include "asset/btlv_mcss_data_ov094_0220a1a0.inc"
};
static const VecFx32 data_ov094_0220a110[] = {
#include "asset/btlv_mcss_data_ov094_0220a110.inc"
};
static const VecFx32 data_ov094_0220a158[] = {
#include "asset/btlv_mcss_data_ov094_0220a158.inc"
};

static const union { u8 raw[384]; BtlvMcssShadow t[48]; } data_ov094_0220a1e8_raw = { {
#include "asset/btlv_mcss_data_ov094_0220a1e8.inc"
} };
typedef char data_ov094_0220a1e8_fits[sizeof(data_ov094_0220a1e8_raw.t) <= sizeof(data_ov094_0220a1e8_raw.raw) ? 1 : -1];   // the ROM's bytes are its layout
#define data_ov094_0220a1e8 (data_ov094_0220a1e8_raw.t)

// The frames an idle animation waits for
static u8 data_ov094_0220ae78[] = { 0x30, 0x38, 0x40 };

// pokebw2's BtlvMcss_Delete

BtlvMcss *func_ov094_021feac0(u32 battleStyle, TCBManager *tcbMgr, HeapID heapId) {
    BtlvMcss *work = GFL_HeapAllocate(heapId, sizeof(BtlvMcss), TRUE, "btlv_mcss.c", 0x184);
    int i;

    work->mcssSys = func_02014b08(BTLV_MCSS_POS_MAX, heapId);
    work->tcbMgr = tcbMgr;
    work->heapId = heapId;
    func_020163b0(work->mcssSys, 0x100);
    func_020163b4(work->mcssSys, TRUE);
    work->flat = TRUE;
    work->scaleNear = FX32_ONE;
    work->scaleFar = FX32_ONE;
    work->triple = battleStyle == BTL_STYLE_TRIPLE;
    work->rotation = battleStyle == BTL_STYLE_ROTATION;
    for (i = 0; i < BTLV_MCSS_POS_MAX; i++) {
        work->entries[i].pos = 0xff;
    }
    return work;
}

void func_ov094_021feb78(BtlvMcss *work) {
    BtlvMcssPos pos;

    for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_MAX; pos++) {
        if (func_ov094_021ffd9c(work, pos)) {
            func_ov094_021ff0f8(work, pos);
        }
    }
    func_02014ba8(work->mcssSys);
    GFL_HeapFree(work);
}

void func_ov094_021febac(BtlvMcss *work) {
    BtlvMcssPos pos;
    u32 color;
    u32 status;
    u32 rand;
    fx32 speed = FX32_ONE / 3;

    for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_TRAINER; pos++) {
        if (func_ov094_021ffd9c(work, pos)) {
            int index = func_ov094_022019f4(work, pos);

            if (work->entries[index].animEnded) {
                work->entries[index].animEnded = FALSE;
                func_02016374(work->entries[index].mcss);
            }
        }
    }
    func_02014c08(work->mcssSys);
    if (!work->rotating) {
        for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_TRAINER; pos++) {
            if (func_ov094_021ffd9c(work, pos)) {
                rand = (u64)GFL_RandomMT() * 100 >> 32;
                if (rand == 0 && !(func_ov094_021f935c(pos) & work->blinkTasks) && !func_ov094_021ff2d8(work, pos)) {
                    func_ov094_021ff870(work, pos, 2, 4, 1);
                }
            }
            if (func_ov094_021ffd9c(work, pos)) {
                int index;
                BOOL flag;

                index = func_ov094_022019f4(work, pos);
                flag = FALSE;
                if (func_ov094_021f7eb4() && work->entries[index].fading) {
                    work->entries[index].fading = FALSE;
                    work->entries[index].fadeDone = TRUE;
                }
                if (func_ov094_021f7eb4() && work->entries[index].fadeDone) {
                    continue;
                }
                work->entries[index].fadeDone = FALSE;
                if (func_ov094_021fff38(work, pos) & BTLV_MCSS_FLAG_SUBSTITUTE) {
                    continue;
                }
                if (!func_ov094_021f821c(pos, &color, &status)) {
                    continue;
                }
                if (work->entries[index].statusAnim == TRUE) {
                    if (work->entries[index].statusEvyStep == 0) {
                        work->entries[index].statusEvyStep = 1;
                    }
                    if (work->entries[index].statusTimer++ > 0x18) {
                        flag = TRUE;
                        work->entries[index].statusTimer = 0;
                        work->entries[index].statusEvy += work->entries[index].statusEvyStep;
                        if (work->entries[index].statusEvy == 0 || work->entries[index].statusEvy == 0xc) {
                            work->entries[index].statusEvyStep *= -1;
                        }
                    }
                }
                switch (status) {
                case 3:
                    func_ov094_021ffe24(work, pos);
                    func_ov094_021ff260(work, pos, 3);
                    func_ov094_021ff304(work, pos, 3);
                    func_ov094_021ff498(work, pos, speed);
                    work->entries[index].statusAnim = TRUE;
                    break;
                case 1:
                    if (flag == TRUE) {
                        func_ov094_021ffdf8(work, pos, (u8)work->entries[index].statusEvy, 0x1ef);
                    }
                    func_ov094_021ff260(work, pos, 4);
                    func_ov094_021ff304(work, pos, 3);
                    work->entries[index].statusAnim = TRUE;
                    break;
                case 2:
                    if (flag == TRUE) {
                        func_ov094_021ffdf8(work, pos, 8, 0x7def);
                    }
                    func_ov094_021ff260(work, pos, 4);
                    func_ov094_021ff304(work, pos, 2);
                    work->entries[index].statusAnim = TRUE;
                    break;
                case 5:
                    if (flag == TRUE) {
                        func_ov094_021ffdf8(work, pos, (u8)work->entries[index].statusEvy, 0xf);
                    }
                    func_ov094_021ff260(work, pos, 4);
                    func_ov094_021ff304(work, pos, 3);
                    work->entries[index].statusAnim = TRUE;
                    break;
                case 4:
                case 7:
                    if (flag == TRUE) {
                        func_ov094_021ffdf8(work, pos, (u8)work->entries[index].statusEvy, 0x3c0f);
                    }
                    func_ov094_021ff260(work, pos, 4);
                    func_ov094_021ff304(work, pos, 3);
                    work->entries[index].statusAnim = TRUE;
                    break;
                case 0:
                case 6:
                case 8:
                default:
                    if (work->entries[index].statusAnim == TRUE) {
                        func_ov094_021ffe24(work, pos);
                        func_ov094_021ff260(work, pos, 4);
                        func_ov094_021ff304(work, pos, 3);
                        func_ov094_021ff498(work, pos, FX32_ONE);
                        work->entries[index].statusAnim = FALSE;
                        work->entries[index].statusEvyStep = 0;
                        work->entries[index].statusEvy = 0;
                        work->entries[index].statusTimer = 0;
                    }
                    break;
                }
                if (status == 8) {
                    if (color - 2 <= 1) {
                        func_ov094_021ff498(work, pos, speed);
                    } else {
                        func_ov094_021ff498(work, pos, FX32_ONE);
                    }
                }
            }
        }
    }
}


// pokebw2's BtlvMcss_Draw
void func_ov094_021fee9c(BtlvMcss *work) {
    func_02014d2c(work->mcssSys);
}

void func_ov094_021feea8(BtlvMcss *work, PartyPkm *pkm, int pos) {
    int index;
    BOOL shadow;
    u16 unk6;
    VecFx32 pos3d;
    VecFx32 offset;
    u32 hp;

    for (index = 0; index < BTLV_MCSS_POS_MAX; index++) {
        if (work->entries[index].mcss == NULL) {
            break;
        }
    }
    if (pos >= BTLV_MCSS_POS_MAX || index >= BTLV_MCSS_POS_MAX) {
        return;
    }
    work->entries[index].flags = 0;
    work->entries[index].frozen = FALSE;
    work->entries[index].statusAnim = FALSE;
    work->entries[index].hidden = FALSE;
    work->entries[index].flat = work->flat;
    work->entries[index].pos = pos;
    work->entries[index].mon.personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    work->entries[index].ball = PokeParty_GetParam(pkm, PKM_PARAM_POKEBALL, NULL);
    if (PokeParty_IsRare(pkm) == TRUE) {
        work->entries[index].flags |= BTLV_MCSS_FLAG_RARE;
    }
    func_ov094_022003ec(work, pkm, &work->entries[index].info, pos);
    func_ov094_022018a8(work, &pos3d, pos);
    work->entries[index].mcss = func_020159c8(work->mcssSys, pos3d.x, pos3d.y, pos3d.z, &work->entries[index].info);
    func_ov094_0220055c(work, pos, TRUE);
    work->entries[index].mon.species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    work->entries[index].mon.form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    work->entries[index].mon.weight = PML_PersonalGetParamSingle(work->entries[index].mon.species,
                                                                 work->entries[index].mon.form, PERSONAL_WEIGHT);
    hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    work->entries[index].mon.hpColor = func_0202d034(hp, PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL));
    if (!func_ov094_02201a20(work->entries[index].mon.species, pos, &offset, &unk6, &shadow)) {
        offset.x = -0x400;
        offset.y = 0;
        offset.z = -0x600;
        unk6 = 0xf800;
        shadow = TRUE;
    }
    if (!shadow) {
        func_02015fd4(work->entries[index].mcss, 0);
    } else {
        func_02015d94(work->entries[index].mcss, &offset);
        func_02015d88(work->entries[index].mcss, unk6);
    }
    work->entries[index].idleCount = 3;
    func_ov094_022006b4(work, pos);
    func_02015f2c(work->entries[index].mcss, index, func_ov094_02201718, 1);
}


void func_ov094_021ff088(BtlvMcss *work, u32 trainerType, int pos) {
    int index;
    MCSSLoadInfo info;
    VecFx32 pos3d;

    for (index = 0; index < BTLV_MCSS_POS_MAX; index++) {
        if (work->entries[index].mcss == NULL) {
            break;
        }
    }
    if (pos < BTLV_MCSS_POS_MAX && index < BTLV_MCSS_POS_MAX) {
        work->entries[index].flat = TRUE;
        work->entries[index].pos = pos;
        func_ov094_022006a0(trainerType, &info, pos);
        func_ov094_022018a8(work, &pos3d, pos);
        work->entries[index].mcss = func_020159c8(work->mcssSys, pos3d.x, pos3d.y, pos3d.z, &info);
        func_ov094_022006b4(work, pos);
    }
}


// pokebw2's BtlvMcss_Remove
void func_ov094_021ff0f8(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    func_02015ba4(work->mcssSys, work->entries[index].mcss);
    work->entries[index].mcss = NULL;
    if (work->entries[index].idleTask != NULL) {
        GFL_HeapFree(GFL_TCBGetData(work->entries[index].idleTask));
        GFL_TCBRemove(work->entries[index].idleTask);
        work->entries[index].idleTask = NULL;
    }
}

// pokebw2's BtlvMcss_SetSideScale
void func_ov094_021ff134(BtlvMcss *work, fx32 scaleNear, fx32 scaleFar) {
    work->scaleNear = scaleNear;
    work->scaleFar = scaleFar;
}

// pokebw2's BtlvMcss_SetPosition
void func_ov094_021ff144(BtlvMcss *work, int pos, fx32 x, fx32 y, fx32 z) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 pos3d;

    pos3d.x = x;
    pos3d.y = y;
    pos3d.z = z;
    func_02015c48(work->entries[index].mcss, &pos3d);
}

// pokebw2's BtlvMcss_SetFlatAll
void func_ov094_021ff16c(BtlvMcss *work) {
    int pos;
    int index;

    func_02015bf0(work->mcssSys);
    work->flat = TRUE;
    for (pos = 0; pos < BTLV_MCSS_POS_MAX; pos++) {
        index = func_ov094_022019f4(work, pos);
        if (index != -1) {
            work->entries[index].flat = TRUE;
            func_ov094_022006b4(work, pos);
        }
    }
}

// pokebw2's BtlvMcss_ClearFlatAll
void func_ov094_021ff1b0(BtlvMcss *work) {
    int pos;
    int index;

    func_02015c00(work->mcssSys);
    work->flat = FALSE;
    for (pos = 0; pos < BTLV_MCSS_POS_MAX; pos++) {
        index = func_ov094_022019f4(work, pos);
        if (index != -1) {
            work->entries[index].flat = FALSE;
            func_ov094_022006b4(work, pos);
        }
    }
}

// pokebw2's BtlvMcss_SetFlat
void func_ov094_021ff1f4(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index != -1) {
        work->entries[index].flat = TRUE;
        func_02015c0c(work->entries[index].mcss);
        func_ov094_022006b4(work, pos);
    }
}

// pokebw2's BtlvMcss_ClearFlat
void func_ov094_021ff228(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index != -1) {
        work->entries[index].flat = FALSE;
        func_02015c1c(work->entries[index].mcss);
        func_ov094_022006b4(work, pos);
    }
}

// pokebw2's BtlvMcss_SetAnimStop
static void func_ov094_021ff260(BtlvMcss *work, int pos, int mode) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    if (mode == 4) {
        func_02015dc0(work->entries[index].mcss);
        work->entries[index].frozen = FALSE;
    }
    if (work->entries[index].frozen) {
        return;
    }
    if (mode == 2) {
        func_02015dd4(work->entries[index].mcss);
    } else if (mode == 1) {
        func_02015db0(work->entries[index].mcss);
    } else if (mode == 3) {
        func_02015db0(work->entries[index].mcss);
        work->entries[index].frozen = TRUE;
    } else {
        func_02015dc0(work->entries[index].mcss);
    }
}

// pokebw2's BtlvMcss_IsAnimStopped
BOOL func_ov094_021ff2d8(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return FALSE;
    }
    if (work->entries[index].mcss == NULL) {
        return FALSE;
    }
    return func_02015df8(work->entries[index].mcss);
}

// pokebw2's BtlvMcss_SetAnimPause
void func_ov094_021ff304(BtlvMcss *work, int pos, int mode) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    if (mode == 1) {
        func_02015e04(work->entries[index].mcss);
    } else if (mode == 2) {
        func_02015e28(work->entries[index].mcss);
    } else if (mode == 3) {
        func_02015e70(work->entries[index].mcss);
    } else {
        func_02015e4c(work->entries[index].mcss);
    }
}

// pokebw2's BtlvMcss_GetVanish
BOOL func_ov094_021ff348(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return FALSE;
    }
    if (work->entries[index].mcss == NULL) {
        return FALSE;
    }
    return func_02015e94(work->entries[index].mcss);
}

// pokebw2's BtlvMcss_SetVanish
void func_ov094_021ff374(BtlvMcss *work, int pos, int mode) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    switch (mode) {
    case 2:
        func_02015ec4(work->entries[index].mcss);
        break;
    case 1:
        func_02015ea0(work->entries[index].mcss);
        break;
    case 0:
        func_02015eb0(work->entries[index].mcss);
        break;
    case 3:
        if (func_02015e94(work->entries[index].mcss)) {
            work->entries[index].hidden = TRUE;
        }
        func_02015ea0(work->entries[index].mcss);
        break;
    case 4:
        if (work->entries[index].hidden) {
            work->entries[index].hidden = FALSE;
        } else {
            func_02015eb0(work->entries[index].mcss);
        }
        break;
    }
}

// pokebw2's BtlvMcss_GetDefaultPos
void func_ov094_021ff3f4(BtlvMcss *work, VecFx32 *out, int pos) {
    func_ov094_022018a8(work, out, pos);
}

// pokebw2's BtlvMcss_GetDefaultPosForStyle
void func_ov094_021ff3fc(BtlvMcss *work, VecFx32 *out, int pos, u32 battleStyle) {
    const VecFx32 *src;

    switch (pos) {
    case 0:
    case 1:
        src = &data_ov094_0220a050[pos];
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        if (battleStyle == BTL_STYLE_ROTATION) {
            src = &data_ov094_0220a1a0[pos - 2];
        } else if (battleStyle == BTL_STYLE_TRIPLE) {
            src = &data_ov094_0220a158[pos - 2];
        } else {
            src = &data_ov094_0220a080[pos - 2];
        }
        break;
    case 6:
    case 7:
        if (battleStyle == BTL_STYLE_ROTATION) {
            src = &data_ov094_0220a1a0[pos - 2];
        } else {
            src = &data_ov094_0220a158[pos - 2];
        }
        break;
    }
    out->x = src->x;
    out->y = src->y;
    out->z = src->z;
}

fx32 func_ov094_021ff468(BtlvMcss *work, int pos, BOOL flat) {
    return func_ov094_02201934(work, pos, flat);
}


void func_ov094_021ff470(BtlvMcss *work, int pos, u32 value) {
    int index = func_ov094_022019f4(work, pos);

    if (index != -1 && work->entries[index].mcss != NULL) {
        func_02015ff0(work->entries[index].mcss, value);
    }
}

void func_ov094_021ff498(BtlvMcss *work, int pos, fx32 speed) {
    int index = func_ov094_022019f4(work, pos);

    if (index != -1 && work->entries[index].mcss != NULL) {
        func_020160c4(work->entries[index].mcss, speed);
    }
}


fx32 BtlvMcss_GetDefaultScale(BtlvMcss *work, int pos, BOOL flat) {
    return func_ov094_02201934(work, pos, flat);
}

// pokebw2's BtlvMcss_MovePosition
void func_ov094_021ff4c0(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_02015c30(work->entries[index].mcss, &start);
    if (type == 1) {
        if (pos & 1) {
            dest->x = start.x - dest->x;
        } else {
            dest->x += start.x;
        }
        dest->y += start.y;
        dest->z += start.z;
    }
    if (type == 5) {
        func_ov094_022018a8(work, dest, pos);
        type = 1;
    }
    if (type == 6) {
        func_ov094_022018a8(work, dest, pos);
        type = 0;
    }
    if (type == 0) {
        func_02015c48(work->entries[index].mcss, dest);
        return;
    }
    work->moveSeq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, dest, frames, wait, count, func_ov094_02200904,
                           func_ov094_02200990, TRUE, work->moveSeq[pos]);
    work->moveTasks |= func_ov094_021f935c(pos);
}

// pokebw2's BtlvMcss_MoveVec518
void func_ov094_021ff5b0(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_02015cc4(work->entries[index].mcss, &start);
    work->unk518Seq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, dest, frames, wait, count, func_ov094_022009c8,
                           func_ov094_02200a50, FALSE, work->unk518Seq[pos]);
    work->unk518Tasks |= func_ov094_021f935c(pos);
}

// pokebw2's BtlvMcss_MoveScale
void func_ov094_021ff630(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_02015c94(work->entries[index].mcss, &start);
    work->scaleSeq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, dest, frames, wait, count, func_ov094_02200a84,
                           func_ov094_02200b10, FALSE, work->scaleSeq[pos]);
    work->scaleTasks |= func_ov094_021f935c(pos);
}

// BUG: the start is func_02015cc4's vector, where the task steps func_02015d4c's
// pokebw2's BtlvMcss_MoveVec524
void func_ov094_021ff6b0(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_02015cc4(work->entries[index].mcss, &start);
    work->unk524Seq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, dest, frames, wait, count, func_ov094_02200b48,
                           func_ov094_02200bd4, FALSE, work->unk524Seq[pos]);
    work->unk524Tasks |= func_ov094_021f935c(pos);
}

// pokebw2's BtlvMcss_MoveVec51c
void func_ov094_021ff730(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_02015cf8(work->entries[index].mcss, &start);
    work->unk51cSeq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, dest, frames, wait, count, func_ov094_02200c0c,
                           func_ov094_02200c98, TRUE, work->unk51cSeq[pos]);
    work->unk51cTasks |= func_ov094_021f935c(pos);
}

// pokebw2's BtlvMcss_SwapPositions
void func_ov094_021ff7b0(BtlvMcss *work, int pos1, int pos2) {
    int pos[2];
    int index[2];
    VecFx32 dest;
    int i;

    pos[0] = pos1;
    pos[1] = pos2;
    for (i = 0; i < 2; i++) {
        index[i] = func_ov094_022019f4(work, pos[i]);
        if (func_ov094_021ffd9c(work, pos[i])) {
            func_ov094_022018a8(work, &dest, pos[i ^ 1]);
            func_ov094_021ff4c0(work, pos[i], 0, &dest, 0, 0, 0);
            func_ov094_022089bc(func_ov094_021f86b4(), pos[i]);
            if (work->entries[index[i]].moved) {
                work->entries[index[i]].moved = FALSE;
                func_ov094_021ff374(work, pos[i], 0);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (index[i] != -1) {
            work->entries[index[i]].pos = pos[i ^ 1];
        }
    }
}

// pokebw2's BtlvMcss_Blink
void func_ov094_021ff870(BtlvMcss *work, int pos, int type, s32 wait, s32 count) {
    BtlvMcssMove *task;

    switch (type) {
    case 3:
        func_ov094_021ff260(work, pos, 1);
        break;
    case 4:
        func_ov094_021ff260(work, pos, 0);
        break;
    case 2:
        task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvMcssMove), FALSE, "btlv_mcss.c", 0x5e5);
        task->work = work;
        task->pos = pos;
        task->move.type = type;
        task->move.wait = 0;
        task->move.waitReset = wait;
        task->move.count = count * 2;
        work->blinkSeq[pos]++;
        task->seq = work->blinkSeq[pos];
        func_ov094_021f896c(GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02200cd0, task, 0),
                           func_ov094_02200d30, 2);
        work->blinkTasks |= func_ov094_021f935c(pos);
        break;
    }
}

// pokebw2's BtlvMcss_MoveAlpha
void func_ov094_021ff924(BtlvMcss *work, int pos, int type, int alpha, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;
    VecFx32 end;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    start.x = FX32_CONST(func_02015fac(work->entries[index].mcss));
    start.y = 0;
    start.z = 0;
    end.x = FX32_CONST(alpha);
    end.y = 0;
    end.z = 0;
    work->alphaSeq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, &end, frames, wait, count, func_ov094_02200d68,
                           func_ov094_02200de4, FALSE, work->alphaSeq[pos]);
    work->alphaTasks |= func_ov094_021f935c(pos);
}

// pokebw2's BtlvMcss_MoveCircle
void func_ov094_021ffa18(BtlvMcss *work, BtlvMcssCircle *param) {
    BtlvMcssCircle *task =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvMcssCircle), FALSE, "btlv_mcss.c", 0x627);

    task->work = work;
    task->pos = param->pos;
    task->mode = param->mode;
    task->start = param->start;
    task->radius1 = param->radius1;
    task->radius2 = param->radius2;
    task->frames = param->frames;
    task->wait = param->wait;
    task->count = param->count;
    task->turnWait = param->turnWait;
    task->waitCount = 0;
    task->turnWaitLeft = 0;
    // Mirrored for the enemy side, but for the starts 2 and 3 on the y-z and x-y planes
    if (task->pos & 1) {
        if (!((task->mode == 0 || task->mode == 1 || task->mode == 4 || task->mode == 5) &&
              (param->start == 2 || param->start == 3))) {
            task->start ^= 1;
        }
    }
    task->angle = (task->mode & 1) ? 0x10000 : 0;
    task->speed = 0x10000 / param->frames;
    work->moveSeq[task->pos]++;
    task->seq = work->moveSeq[task->pos];
    func_ov094_021f896c(GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02200e1c, task, 0), func_ov094_02201098,
                       2);
    work->moveTasks |= func_ov094_021f935c(task->pos);
}

// pokebw2's BtlvMcss_Shake
void func_ov094_021ffb04(BtlvMcss *work, BtlvMcssShake *param) {
    BtlvMcssShake *task =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvMcssShake), FALSE, "btlv_mcss.c", 0x660);
    int index = func_ov094_022019f4(work, param->pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    *task = *param;
    task->work = work;
    func_02015c30(work->entries[index].mcss, &task->start);
    work->moveSeq[task->pos]++;
    task->seq = work->moveSeq[task->pos];
    func_ov094_021f896c(GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_022010d0, task, 0), func_ov094_022011fc,
                       2);
    work->moveTasks |= func_ov094_021f935c(task->pos);
}

// pokebw2's BtlvMcss_MoveLevel
void func_ov094_021ffbbc(BtlvMcss *work, int pos, int type, int level, s32 frames, s32 wait, s32 count) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 start;
    VecFx32 end;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    start.x = FX32_CONST(func_02016068(work->entries[index].mcss));
    start.y = 0;
    start.z = 0;
    end.x = FX32_CONST(level);
    end.y = 0;
    end.z = 0;
    work->levelSeq[pos]++;
    func_ov094_0220074c(work, pos, type, &start, &end, frames, wait, count, func_ov094_02201234,
                           func_ov094_022012cc, FALSE, work->levelSeq[pos]);
    work->levelTasks |= func_ov094_021f935c(pos);
}

// pokebw2's BtlvMcss_IsBusy
BOOL func_ov094_021ffcb0(BtlvMcss *work, int pos) {
    BOOL fading = FALSE;

    if (func_ov094_021ffd9c(work, pos)) {
        fading = func_0201600c(work->entries[func_ov094_022019f4(work, pos)].mcss);
    }
    if ((func_ov094_021f935c(pos) & work->moveTasks) || (func_ov094_021f935c(pos) & work->unk518Tasks) ||
        (func_ov094_021f935c(pos) & work->unk524Tasks) || (func_ov094_021f935c(pos) & work->scaleTasks) ||
        (func_ov094_021f935c(pos) & work->unk51cTasks) || (func_ov094_021f935c(pos) & work->blinkTasks) ||
        (func_ov094_021f935c(pos) & work->alphaTasks) || (func_ov094_021f935c(pos) & work->levelTasks) || work->rotating ||
        fading) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvMcss_IsAnyBusy
BOOL func_ov094_021ffd6c(BtlvMcss *work) {
    BtlvMcssPos pos;

    for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_TRAINER; pos++) {
        if (func_ov094_021ffd9c(work, pos) && func_ov094_021ffcb0(work, pos)) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BtlvMcss_Exists
BOOL func_ov094_021ffd9c(BtlvMcss *work, int pos) {
    if (func_ov094_022019f4(work, pos) != -1) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvMcss_StartPaletteFade
void func_ov094_021ffdb4(BtlvMcss *work, int pos, u8 startEvy, u8 endEvy, u8 wait, u32 color) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    // The callers pass the wait as a u8, and func_02015f50 takes it signed
    func_02015f50(work->entries[index].mcss, startEvy, endEvy, (s8)wait, color);
    work->entries[index].fading = TRUE;
}

// pokebw2's BtlvMcss_SetPaletteBlend
void func_ov094_021ffdf8(BtlvMcss *work, int pos, u8 evy, GXRgb color) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_020160d4(work->mcssSys, work->entries[index].mcss, evy, color);
}

// pokebw2's BtlvMcss_ResetPaletteBlend
static void func_ov094_021ffe24(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_0201624c(work->mcssSys, work->entries[index].mcss);
}

// pokebw2's BtlvMcss_ChangePokemon
void func_ov094_021ffe48(BtlvMcss *work, int pos, PartyPkm *pkm) {
    int index = func_ov094_022019f4(work, pos);
    u32 hp;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    work->entries[index].mon.species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    work->entries[index].mon.form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    work->entries[index].mon.weight =
        PML_PersonalGetParamSingle(work->entries[index].mon.species, work->entries[index].mon.form, PERSONAL_WEIGHT);
    work->entries[index].mon.personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    work->entries[index].ball = PokeParty_GetParam(pkm, PKM_PARAM_POKEBALL, NULL);
    if (PokeParty_IsRare(pkm) == TRUE) {
        work->entries[index].flags |= BTLV_MCSS_FLAG_RARE;
    }
    hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    work->entries[index].mon.hpColor = func_0202d034(hp, PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL));
    work->entries[index].changed = TRUE;
}

// pokebw2's BtlvMcss_GetWeight
u16 func_ov094_021fff0c(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return 0;
    }
    if (work->entries[index].mcss == NULL) {
        return 0;
    }
    return work->entries[index].mon.weight;
}

// pokebw2's BtlvMcss_GetFlags
u32 func_ov094_021fff38(BtlvMcss *work, int pos) {
    int index;

    if (pos == 0xff) {
        return 0;
    }
    index = func_ov094_022019f4(work, pos);
    if (index == -1) {
        return 0;
    }
    if (work->entries[index].mcss == NULL) {
        return 0;
    }
    return work->entries[index].flags;
}

// pokebw2's BtlvMcss_IsNoBounce
BOOL func_ov094_021fff68(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return FALSE;
    }
    if (work->entries[index].mcss == NULL) {
        return FALSE;
    }
    if (PML_PersonalGetParamSingle(work->entries[index].mon.species, work->entries[index].mon.form,
                                   PERSONAL_NO_BOUNCE)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvMcss_GetUnk
u8 func_ov094_021fffac(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return 0;
    }
    if (work->entries[index].mcss == NULL) {
        return 0;
    }
    return func_02015f24(work->entries[index].mcss);
}

// pokebw2's BtlvMcss_SetSubstitute
void func_ov094_021fffd8(BtlvMcss *work, int pos, BOOL substitute, BOOL flag) {
    int index = func_ov094_022019f4(work, pos);
    MCSSLoadInfo info;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    if (!substitute) {
        func_020172ac(work->mcssSys, work->entries[index].mon.personality);
        func_02014cf8(work->mcssSys, work->entries[index].mcss, &work->entries[index].info);
        func_02015f2c(work->entries[index].mcss, index, func_ov094_02201718, 1);
        if (flag) {
            work->entries[index].flags &= ~BTLV_MCSS_FLAG_UNK0;
        }
        work->entries[index].flags &= ~BTLV_MCSS_FLAG_SUBSTITUTE;
    } else {
        info.arcId = GetPokemonGraphicsARCID();
        info.character = (pos & 1) ? 0x37a0 : 0x37a6;
        info.palette = 0x37ac;
        info.cells = (pos & 1) ? 0x37a1 : 0x37a7;
        info.cellAnime = (pos & 1) ? 0x37a2 : 0x37a8;
        info.multiCells = (pos & 1) ? 0x37a3 : 0x37a9;
        info.multiCellAnime = (pos & 1) ? 0x37a4 : 0x37aa;
        info.bin = (pos & 1) ? 0x37a5 : 0x37ab;
        info.unk20 = 0;
        func_02014cf8(work->mcssSys, work->entries[index].mcss, &info);
        func_ov094_021ffe24(work, pos);
        if (flag) {
            work->entries[index].flags |= BTLV_MCSS_FLAG_UNK0;
        }
        work->entries[index].flags |= BTLV_MCSS_FLAG_SUBSTITUTE;
    }
}

// pokebw2's BtlvMcss_SetLevel
static void func_ov094_02200108(BtlvMcss *work, int pos, int level) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_020172ac(work->mcssSys, work->entries[index].mon.personality);
    func_02016078(work->mcssSys, work->entries[index].mcss, level);
}

// NONMATCHING: 6 bytes short; the original keeps work + 0xc for the copy's first store and the last call, which spills
// targetPos
// pokebw2's BtlvMcss_Transform
void func_ov094_02200140(BtlvMcss *work, int targetPos, int pos) {
    int target = func_ov094_022019f4(work, targetPos);
    int index = func_ov094_022019f4(work, pos);
    MCSSLoadInfo *dst;

    work->entries[index].info.arcId = work->entries[target].info.arcId;
    work->entries[index].info.palette = work->entries[target].info.palette;
    // The other side's sprites, front for back
    if (targetPos & 1) {
        work->entries[index].info.character = work->entries[target].info.character + 9;
        work->entries[index].info.cells = work->entries[target].info.cells + 9;
        work->entries[index].info.cellAnime = work->entries[target].info.cellAnime + 9;
        work->entries[index].info.multiCells = work->entries[target].info.multiCells + 9;
        work->entries[index].info.multiCellAnime = work->entries[target].info.multiCellAnime + 9;
        work->entries[index].info.bin = work->entries[target].info.bin + 9;
    } else {
        work->entries[index].info.character = work->entries[target].info.character - 9;
        work->entries[index].info.cells = work->entries[target].info.cells - 9;
        work->entries[index].info.cellAnime = work->entries[target].info.cellAnime - 9;
        work->entries[index].info.multiCells = work->entries[target].info.multiCells - 9;
        work->entries[index].info.multiCellAnime = work->entries[target].info.multiCellAnime - 9;
        work->entries[index].info.bin = work->entries[target].info.bin - 9;
    }
    work->entries[index].mon = work->entries[target].mon;
    // dst must be assigned here, not at its declaration: only this form CSEs &entries[index] with the stores above
    dst = &work->entries[index].info;
    func_02014cf8(work->mcssSys, work->entries[index].mcss, dst);
}

// pokebw2's BtlvMcss_ReloadSprite
void func_ov094_022001dc(BtlvMcss *work, int pos, const MCSSLoadInfo *info) {
    int index = func_ov094_022019f4(work, pos);
    MCSSLoadInfo *dst = &work->entries[index].info;

    *dst = *info;
    func_020172ac(work->mcssSys, work->entries[index].mon.personality);
    func_02014cf8(work->mcssSys, work->entries[index].mcss, dst);
}

// pokebw2's BtlvMcss_PlayCry
u32 func_ov094_02200218(BtlvMcss *work, int pos, s32 speed, int volume, int arg4, int arg5, int arg6) {
    int index = func_ov094_022019f4(work, pos);
    s32 pan;
    BtlMainModule *mainModule;
    PokeVoiceChatterInfo chatter;
    PokeVoiceChatterInfo *chatterInfo;
    BOOL flag;
    u32 handle;

    if (index == -1) {
        return 0;
    }
    if (work->entries[index].mcss == NULL) {
        return 0;
    }
    switch (pos) {
    case 0:
    case 1:
        pan = data_ov094_02209fc0[pos];
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        if (work->rotation) {
            pan = data_ov094_02209ff0[pos - 2];
        } else if (work->triple) {
            pan = data_ov094_0220a068[pos - 2];
        } else {
            pan = data_ov094_02209fe0[pos - 2];
        }
        break;
    case 6:
    case 7:
        if (work->rotation) {
            pan = data_ov094_02209ff0[pos - 2];
        } else {
            pan = data_ov094_0220a068[pos - 2];
        }
        break;
    }
    mainModule = func_ov094_021f8754();
    chatterInfo = &chatter;
    flag = FALSE;
    if (mainModule != NULL) {
        if (!func_ov093_021b994c(mainModule, pos, &chatterInfo->chatter)) {
            chatterInfo = NULL;
        }
    } else {
        chatterInfo = NULL;
    }
    if (arg4 != 0 || arg5 != 0) {
        flag = TRUE;
    }
    // By the HP: lower in the yellow
    if (speed == (s32)0x80000000) {
        speed = work->entries[index].mon.hpColor == HP_GAUGE_COLOR_YELLOW ? -0xa00 : 0;
    }
    handle = PokeVoice_Play(work->entries[index].mon.species, work->entries[index].mon.form, (u8)pan, flag, arg4, arg5,
                            arg6, chatterInfo);
    PokeVoice_AdjustVolume(handle, volume);
    PokeVoice_AdjustSpeed(handle, speed);
    return handle;
}

// pokebw2's BtlvMcss_StartRotation
void func_ov094_02200344(BtlvMcss *work, int side, int dir, BOOL playSe) {
    BtlvMcssRotate *task =
        GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvMcssRotate), FALSE, "btlv_mcss.c", 0x8f2);

    task->work = work;
    task->seq = 0;
    task->playSe = playSe;
    task->side = side;
    task->dir = dir;
    work->rotating = TRUE;
    func_ov094_021f896c(GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02201408, task, 0), func_ov094_02201700, 2);
}

// pokebw2's BtlvMcss_GetBall
u32 func_ov094_022003c4(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return 0;
    }
    if (work->entries[index].mcss == NULL) {
        return 0;
    }
    return work->entries[index].ball;
}

// pokebw2's BtlvMcss_SetupPokemonLoadInfo
void func_ov094_022003ec(BtlvMcss *work, PartyPkm *pkm, MCSSLoadInfo *info, int pos) {
    BOOL back = TRUE;

    if (pos & 1) {
        back = FALSE;
    }
    func_02017100(pkm, info, back);
    func_020172ac(work->mcssSys, PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL));
}

// pokebw2's BtlvMcss_SetAnimation
BOOL func_ov094_0220041c(BtlvMcss *work, int pos, int anim) {
    BOOL set = FALSE;
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return set;
    }
    if (work->entries[index].mcss == NULL) {
        return set;
    }
    if (anim < func_020160cc(work->entries[index].mcss)) {
        func_02016028(work->entries[index].mcss, anim);
        set = TRUE;
        work->entries[index].animSet = set;
    }
    return set;
}

// pokebw2's BtlvMcss_WatchAnimationEnd
void func_ov094_02200468(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_02015f2c(work->entries[index].mcss, index, func_ov094_02201894, 1);
}

// pokebw2's BtlvMcss_IsAnimationSet
BOOL func_ov094_02200494(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return FALSE;
    }
    if (work->entries[index].mcss == NULL) {
        return FALSE;
    }
    return work->entries[index].animSet;
}

// pokebw2's BtlvMcss_ResetPosition
void func_ov094_022004bc(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 home;
    VecFx32 cur;
    VecFx32 offset;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    func_ov094_022018a8(work, &home, pos);
    func_02015c30(work->entries[index].mcss, &cur);
    func_02015c60(work->entries[index].mcss, &offset);
    work->entries[index].moved = FALSE;
    if (home.x != cur.x || home.y != cur.y || home.z != cur.z || offset.x != 0 || offset.y != 0 || offset.z != 0) {
        work->entries[index].moved = TRUE;
        offset.x = 0;
        offset.y = 0;
        offset.z = 0;
        func_02015c48(work->entries[index].mcss, &home);
        func_02015c78(work->entries[index].mcss, &offset);
        func_02015ea0(work->entries[index].mcss);
    }
}

// pokebw2's BtlvMcss_SetUnkB270
void func_ov094_0220055c(BtlvMcss *work, int pos, BOOL check) {
    BOOL flag = FALSE;
    int index = func_ov094_022019f4(work, pos);

    if (index == -1) {
        return;
    }
    if (work->entries[index].mcss == NULL) {
        return;
    }
    if (check == TRUE) {
        switch (func_ov094_021f8710()) {
        case 0:
            break;
        case 1:
            switch (pos) {
            case 3:
            case 4:
                flag = TRUE;
                break;
            }
            break;
        case 2:
            switch (pos) {
            case 2:
            case 5:
            case 6:
                flag = TRUE;
                break;
            }
            break;
        case 3:
            break;
        }
    }
    func_02016394(work->entries[index].mcss, flag);
}

// pokebw2's BtlvMcss_CheckChanged
BOOL func_ov094_022005c4(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);
    BOOL changed;

    if (index == -1) {
        return FALSE;
    }
    if (work->entries[index].mcss == NULL) {
        return FALSE;
    }
    changed = work->entries[index].changed ? TRUE : FALSE;
    work->entries[index].changed = FALSE;
    return changed;
}

// pokebw2's BtlvMcss_SaveVanish
void func_ov094_02200608(BtlvMcss *work) {
    BtlvMcssPos pos;
    int index;

    for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_TRAINER; pos++) {
        if (func_ov094_021ffd9c(work, pos)) {
            index = func_ov094_022019f4(work, pos);
            work->entries[index].vanished = func_ov094_021ff348(work, pos);
        }
    }
}

// pokebw2's BtlvMcss_RestoreVanish
void func_ov094_02200654(BtlvMcss *work) {
    BtlvMcssPos pos;
    int index;

    for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_TRAINER; pos++) {
        if (func_ov094_021ffd9c(work, pos)) {
            index = func_ov094_022019f4(work, pos);
            func_ov094_021ff374(work, pos, work->entries[index].vanished);
            work->entries[index].hidden = FALSE;
        }
    }
}

// pokebw2's BtlvMcss_SetupTrainerLoadInfo
void func_ov094_022006a0(u32 trainerType, MCSSLoadInfo *info, int pos) {
    BOOL back = TRUE;

    if (pos & 1) {
        back = FALSE;
    }
    GetTrainerSpriteResIDs(trainerType, info, back);
}

// pokebw2's BtlvMcss_UpdateScale
static void func_ov094_022006b4(BtlvMcss *work, int pos) {
    int index = func_ov094_022019f4(work, pos);
    VecFx32 scale;
    fx32 value;

    if (index == -1 || work->entries[index].mcss == NULL) {
        return;
    }
    value = func_ov094_02201934(work, pos, work->entries[index].flat);
    if (pos & 1) {
        value = FX_Mul(value, work->scaleFar);
    } else {
        value = FX_Mul(value, work->scaleNear);
    }
    scale.x = value;
    scale.y = value;
    scale.z = FX32_ONE;
    func_02015cac(work->entries[index].mcss, &scale);
    value = func_ov094_02201934(work, pos, FALSE);
    scale.x = value;
    scale.y = value;
    scale.z = FX32_ONE;
    func_02015d30(work->entries[index].mcss, &scale);
}

static void func_ov094_0220074c(BtlvMcss *work, int pos, int type, VecFx32 *start, VecFx32 *end, s32 frames,
                                   s32 wait, s32 count, TCBFunc func, void (*endFunc)(TCB *tcb), BOOL mirror, u8 seq) {
    BtlvMcssMove *task = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvMcssMove), FALSE, "btlv_mcss.c", 0xa4e);

    task->work = work;
    task->pos = pos;
    task->move.type = type;
    task->move.stepTime = frames;
    task->move.stepTimeReset = frames;
    task->move.wait = 0;
    task->move.waitReset = wait;
    task->move.count = count * 2;
    task->move.start.x = start->x;
    task->move.start.y = start->y;
    task->move.start.z = start->z;
    task->move.end.x = end->x;
    task->move.end.y = end->y;
    task->move.end.z = end->z;
    task->value.x = start->x;
    task->value.y = start->y;
    task->value.z = start->z;
    task->seq = seq;
    switch (type) {
    case 1:
    case 4:
        func_ov094_021f918c(&task->move.start, end, &task->move.step, FX32_CONST(frames));
        break;
    case 3:
        task->move.count *= 2;
    case 2:
        task->move.step.x = FX_Div(end->x, FX32_CONST(frames));
        task->move.step.y = FX_Div(end->y, FX32_CONST(frames));
        task->move.step.z = FX_Div(end->z, FX32_CONST(frames));
        // Mirrored for the enemy side
        if ((pos & 1) && mirror) {
            task->move.step.x *= -1;
            task->move.step.z *= -1;
        }
        break;
    }
    func_ov094_021f896c(GFL_TCBMgrAddTask(work->tcbMgr, func, task, 0), endFunc, 2);
}

// pokebw2's BtlvMcss_MoveTask_Position
static void func_ov094_02200904(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    VecFx32 value;
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->moveSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    func_02015c30(work->entries[index].mcss, &value);
    done = func_ov094_021f9260(&task->move, &value);
    func_02015c48(work->entries[index].mcss, &value);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Position
static void func_ov094_02200990(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->moveSeq[task->pos]) {
        task->work->moveTasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Vec518
static void func_ov094_022009c8(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    VecFx32 value;
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->unk518Seq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    func_02015cc4(work->entries[index].mcss, &value);
    done = func_ov094_021f9260(&task->move, &value);
    func_02015cdc(work->entries[index].mcss, &value);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Vec518
static void func_ov094_02200a50(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->unk518Seq[task->pos]) {
        task->work->unk518Tasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Scale
static void func_ov094_02200a84(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    VecFx32 value;
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->scaleSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    func_02015c94(work->entries[index].mcss, &value);
    done = func_ov094_021f9260(&task->move, &value);
    func_02015cac(work->entries[index].mcss, &value);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Scale
static void func_ov094_02200b10(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->scaleSeq[task->pos]) {
        task->work->scaleTasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Vec524
static void func_ov094_02200b48(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    VecFx32 value;
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->unk524Seq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    func_02015d4c(work->entries[index].mcss, &value);
    done = func_ov094_021f9260(&task->move, &value);
    func_02015d64(work->entries[index].mcss, &value);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Vec524
static void func_ov094_02200bd4(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->unk524Seq[task->pos]) {
        task->work->unk524Tasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Vec51c
static void func_ov094_02200c0c(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    VecFx32 value;
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->unk51cSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    func_02015cf8(work->entries[index].mcss, &value);
    done = func_ov094_021f9260(&task->move, &value);
    func_02015d14(work->entries[index].mcss, &value);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Vec51c
static void func_ov094_02200c98(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->unk51cSeq[task->pos]) {
        task->work->unk51cTasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Blink
static void func_ov094_02200cd0(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;

    if (!func_ov094_021ffd9c(work, task->pos) || task->seq != work->blinkSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->move.wait == 0) {
        task->move.wait = task->move.waitReset;
        func_ov094_021ff260(task->work, task->pos, 2);
        if (--task->move.count == 0) {
            func_ov094_021ff260(task->work, task->pos, 0);
            func_ov094_021f89bc(tcb);
        }
    } else {
        task->move.wait--;
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Blink
static void func_ov094_02200d30(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->blinkSeq[task->pos]) {
        task->work->blinkTasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Alpha
static void func_ov094_02200d68(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->alphaSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    done = func_ov094_021f9260(&task->move, &task->value);
    func_02015fbc(work->entries[index].mcss, task->value.x >> FX32_SHIFT);
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Alpha
static void func_ov094_02200de4(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->alphaSeq[task->pos]) {
        task->work->alphaTasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Circle
static void func_ov094_02200e1c(TCB *tcb, void *data) {
    BtlvMcssCircle *task = data;
    BtlvMcss *work = task->work;
    VecFx32 offset = { 0, 0, 0 };
    int index = func_ov094_022019f4(work, task->pos);
    int idx;
    fx32 a;
    fx32 b;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->moveSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->turnWaitLeft == 0) {
        if (task->waitCount == task->wait) {
            task->waitCount = 0;
            if (task->mode & 1) {
                task->angle -= task->speed;
            } else {
                task->angle += task->speed;
            }
            if (task->angle & 0xffff0000) {
                task->angle = (u16)task->angle;
                task->count--;
                task->turnWaitLeft = task->turnWait;
            }
            if (task->count != 0) {
                switch (task->start) {
                case 0:
                    idx = (u16)(task->angle + 0x4000);
                    a = -FX_Mul(FX_SinIdx(idx), task->radius1);
                    b = -FX_Mul(FX_CosIdx(idx), task->radius2);
                    a += task->radius1;
                    break;
                case 1:
                    idx = (u16)(task->angle + 0x4000);
                    a = FX_Mul(FX_SinIdx(idx), task->radius1) - task->radius1;
                    b = FX_Mul(FX_CosIdx(idx), task->radius2);
                    break;
                case 2:
                    idx = task->angle;
                    a = -FX_Mul(FX_SinIdx(idx), task->radius1);
                    b = -FX_Mul(FX_CosIdx(idx), task->radius2);
                    b += task->radius2;
                    break;
                case 3:
                    idx = task->angle;
                    a = FX_Mul(FX_SinIdx(idx), task->radius1);
                    b = FX_Mul(FX_CosIdx(idx), task->radius2) - task->radius2;
                    break;
                }
                switch ((task->mode & 7) >> 1) {
                case 0:
                default:
                    offset.z = a;
                    offset.y = b;
                    break;
                case 1:
                    offset.x = a;
                    offset.z = b;
                    break;
                case 2:
                    offset.x = a;
                    offset.y = b;
                    break;
                }
            }
            func_02015c78(work->entries[index].mcss, &offset);
        } else {
            task->waitCount++;
        }
    } else {
        task->turnWaitLeft--;
    }
    if (task->count == 0) {
        func_ov094_021f89bc(tcb);
    }
}

// NONMATCHING: loads the task's position before its work, where the original loads the work first
// pokebw2's BtlvMcss_MoveTaskEnd_Circle
static void func_ov094_02201098(TCB *tcb) {
    BtlvMcssCircle *task = GFL_TCBGetData(tcb);

    if (task->work->moveSeq[task->pos] == task->seq) {
        task->work->moveTasks &= func_ov094_021f935c((task = task)->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Shake
static void func_ov094_022010d0(TCB *tcb, void *data) {
    BtlvMcssShake *task = data;
    BtlvMcss *work = task->work;
    VecFx32 offset = { 0, 0, 0 };
    int index = func_ov094_022019f4(work, task->pos);
    fx16 sin;
    fx32 value;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->moveSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    task->angle += task->speed;
    sin = FX_SinIdx((task->angle & 0xffff000) >> FX32_SHIFT);
    value = FX_F32_TO_FX32(FX_FX16_TO_F32(sin));
    value = FX_Mul(value, task->amplitude);
    if (task->axis) {
        offset.y = value;
    } else {
        offset.x = value;
    }
    func_02015c78(work->entries[index].mcss, &offset);
    if (--task->frames == 0) {
        offset.x = 0;
        offset.y = 0;
        offset.z = 0;
        func_02015c78(work->entries[index].mcss, &offset);
        func_ov094_021f89bc(tcb);
    }
}

// NONMATCHING: loads the task's position before its work, where the original loads the work first
// pokebw2's BtlvMcss_MoveTaskEnd_Shake
static void func_ov094_022011fc(TCB *tcb) {
    BtlvMcssShake *btlvMcssShake;
    BtlvMcssShake *task = GFL_TCBGetData(tcb);
    BtlvMcssShake **taskPtr;

    taskPtr = &task;
    if ((*taskPtr)->work->moveSeq[(*taskPtr)->pos] == (*taskPtr)->seq) {
        btlvMcssShake = *taskPtr;
        btlvMcssShake->work->moveTasks &= func_ov094_021f935c((*taskPtr)->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_MoveTask_Level
static void func_ov094_02201234(TCB *tcb, void *data) {
    BtlvMcssMove *task = data;
    BtlvMcss *work = task->work;
    int index = func_ov094_022019f4(work, task->pos);
    BOOL done;

    if (index == -1) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        func_ov094_021f89bc(tcb);
        return;
    }
    if (task->seq != work->levelSeq[task->pos]) {
        func_ov094_021f89bc(tcb);
        return;
    }
    done = func_ov094_021f9260(&task->move, &task->value);
    func_ov094_02200108(work, task->pos, task->value.x >> FX32_SHIFT);
    if (done == TRUE) {
        func_02015f2c(work->entries[index].mcss, index, func_ov094_02201718, 1);
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvMcss_MoveTaskEnd_Level
static void func_ov094_022012cc(TCB *tcb) {
    BtlvMcssMove *task = GFL_TCBGetData(tcb);

    if (task->seq == task->work->levelSeq[task->pos]) {
        task->work->levelTasks &= func_ov094_021f935c(task->pos) ^ 0xffffffff;
    }
}

// pokebw2's BtlvMcss_IdleTask
static void func_ov094_02201304(TCB *tcb, void *data) {
    BtlvMcssIdle *idle = data;
    BtlvMcss *work = func_ov094_021f8694();
    int index = idle->index;

    if (index == -1) {
        GFL_HeapFree(idle);
        GFL_TCBRemove(tcb);
        return;
    }
    if (work->entries[index].mcss == NULL) {
        GFL_HeapFree(idle);
        GFL_TCBRemove(tcb);
        return;
    }
    switch (idle->seq) {
    case 0:
        func_02015ee8(work->entries[index].mcss)->animCtrl.bActive = FALSE;
        func_02015f40(work->entries[index].mcss, index | 0x80000000, func_ov094_02201810);
        idle->seq++;
        idle->wait = data_ov094_0220ae78[GFL_RandomMTRange(3)];
        break;
    case 1:
        if (--idle->wait == 0 || work->unk51cTasks != 0 || !func_ov094_021f9824(func_ov094_021f86c4())) {
            func_02016028(work->entries[index].mcss, 0);
            func_02015ee8(work->entries[index].mcss)->animCtrl.bActive = TRUE;
            func_02015f40(work->entries[index].mcss, index, func_ov094_02201810);
            idle->seq++;
        }
        break;
    case 2:
        func_02015f2c(work->entries[index].mcss, index, func_ov094_02201718, 1);
        GFL_HeapFree(idle);
        GFL_TCBRemove(tcb);
        work->entries[index].idleTask = NULL;
        break;
    }
}

// pokebw2's BtlvMcss_RotationTask
static void func_ov094_02201408(TCB *tcb, void *data) {
    BtlvMcssRotate *task = data;
    int positions[2][3] = {
        { 2, 4, 6 },
        { 3, 5, 7 },
    };
    s32 angles[2][3] = {
        { 0x8000, 0xd555, 0x2aab },
        { 0, 0x5555, 0xaaaa },
    };
    int dests[2][2][3] = {
#include "asset/btlv_mcss_dests.inc"
};
    VecFx32 pos3d;
    VecFx32 dest;
    int index[3];
    int i;

    switch (task->seq) {
    case 0:
        for (i = 0; i < 3; i++) {
            if (task->work->entries[positions[task->side][i]].idleTask != NULL) {
                break;
            }
        }
        if (i != 3) {
            break;
        }
        for (i = 0; i < 3; i++) {
            task->angle[i] = angles[task->side][i];
        }
        func_ov094_021f6aa8(func_ov094_021f86a4(), task->side, 0, task->dir == 0 ? FX32_ONE : -FX32_ONE, 60);
        task->speed = task->dir == 0 ? 0x16c : -0x16c;
        task->frames = 60;
        if (task->playSe) {
            GFL_SEPlayKeepVol(SEQ_SE_FLD_59, 2);
        }
        task->seq++;
        break;
    case 1:
        for (i = 0; i < 3; i++) {
            if (func_ov094_021ffd9c(task->work, positions[task->side][i])) {
                pos3d.x = FX_Mul(FX_SinIdx((u16)task->angle[i]), FX32_CONST(5));
                pos3d.y = FX32_CONST(0.4);
                pos3d.z = task->side == 0 ? FX32_CONST(12) : FX32_CONST(-13);
                pos3d.z += FX_Mul(FX_CosIdx((u16)task->angle[i]), FX32_CONST(5));
                func_ov094_021ff4c0(task->work, positions[task->side][i], 0, &pos3d, 0, 0, 0);
            }
            task->angle[i] += task->speed;
        }
        if (--task->frames == 0) {
            for (i = 0; i < 3; i++) {
                if (func_ov094_021ffd9c(task->work, positions[task->side][i])) {
                    func_ov094_022018a8(task->work, &dest, dests[task->dir][task->side][i]);
                    func_ov094_021ff4c0(task->work, positions[task->side][i], 0, &dest, 0, 0, 0);
                }
            }
            task->seq++;
        }
        break;
    case 2:
        if (task->work->moveTasks != 0) {
            break;
        }
        for (i = 0; i < 3; i++) {
            index[i] = func_ov094_022019f4(task->work, positions[task->side][i]);
            func_ov094_022089bc(func_ov094_021f86b4(), positions[task->side][i]);
        }
        for (i = 0; i < 3; i++) {
            if (index[i] != -1) {
                task->work->entries[index[i]].pos = dests[task->dir][task->side][i];
            }
        }
        if (task->playSe) {
            GFL_SndPlayerStop(2);
            GFL_SEPlayKeepVol(SEQ_SE_ROTATION_B, 1);
        }
        func_ov094_021f89bc(tcb);
        break;
    }
}

// pokebw2's BtlvMcss_RotationTaskEnd
static void func_ov094_02201700(TCB *tcb) {
    BtlvMcssRotate *task = GFL_TCBGetData(tcb);

    task->work->rotating = FALSE;
}

// pokebw2's BtlvMcss_OnAnimationEnd
static void func_ov094_02201718(uintptr_t index, fx32 frame) {
    BtlvMcss *work = func_ov094_021f8694();
    BtlvMcssIdle *idle;
    u32 count;
    u64 value;

    if (index == -1) {
        return;
    }
    if (work->entries[index].mcss == NULL) {
        return;
    }
    if (!func_ov094_021f9824(func_ov094_021f86c4())) {
        return;
    }
    if (work->rotating) {
        return;
    }
    if (func_02015ef4(work->entries[index].mcss) == 0xff) {
        work->entries[index].animEnded = TRUE;
        return;
    }
    count = work->entries[index].idleCount;
    work->entries[index].idleCount = count - 1;
    value = GFL_RandomMT();
    value *= count;
    value >>= 32;
    if ((u32)value == 0) {
        work->entries[index].idleCount = 3;
        idle = GFL_HeapAllocate(HEAPID_TAIL(work->heapId), sizeof(BtlvMcssIdle), TRUE, "btlv_mcss.c", 0xd5e);
        idle->index = index;
        if (func_02015eec(work->entries[index].mcss) > 1) {
            func_02016028(work->entries[index].mcss, 1);
        }
        work->entries[index].idleTask = GFL_TCBMgrAddTask(work->tcbMgr, func_ov094_02201304, idle, 0);
    }
}

static BOOL func_ov094_02201810(u32 param, const NNSG2dMultiCellHierarchyData *node,
                                      NNSG2dCellAnimation *cellAnim, u16 nodeIdx) {
    BtlvMcss *work = func_ov094_021f8694();
    u16 index = param;
    BOOL found;
    int count;
    int i;

    if (index == -1) {
        return FALSE;
    }
    if (work->entries[index].mcss == NULL) {
        return FALSE;
    }
    if (param & 0x80000000) {
        found = FALSE;
        count = func_02015ef4(work->entries[index].mcss);
        for (i = 0; i < count; i++) {
            if (nodeIdx == func_02015efc(work->entries[index].mcss, i)) {
                found = TRUE;
            }
        }
        if (!found) {
            cellAnim->animCtrl.bActive = FALSE;
        } else {
            cellAnim->animCtrl.bActive = TRUE;
        }
    } else {
        cellAnim->animCtrl.bActive = TRUE;
    }
    return TRUE;
}

// pokebw2's BtlvMcss_OnSetAnimationEnd
static void func_ov094_02201894(uintptr_t index, fx32 frame) {
    BtlvMcss *work = func_ov094_021f8694();

    work->entries[index].animSet = FALSE;
}

// pokebw2's BtlvMcss_GetDefaultPosImpl
static void func_ov094_022018a8(BtlvMcss *work, VecFx32 *out, int pos) {
    const VecFx32 *src;

    switch (pos) {
    case 0:
    case 1:
        src = &data_ov094_0220a050[pos];
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        if (work->rotation) {
            src = &data_ov094_0220a1a0[pos - 2];
        } else if (work->triple) {
            src = &data_ov094_0220a158[pos - 2];
        } else {
            src = &data_ov094_0220a080[pos - 2];
        }
        break;
    case 6:
    case 7:
        if (work->rotation) {
            src = &data_ov094_0220a1a0[pos - 2];
        } else {
            src = &data_ov094_0220a158[pos - 2];
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        src = &data_ov094_0220a110[pos - 8];
        break;
    }
    out->x = src->x;
    out->y = src->y;
    out->z = src->z;
}

// pokebw2's BtlvMcss_GetDefaultScaleImpl
static fx32 func_ov094_02201934(BtlvMcss *work, int pos, BOOL flat) {
    if (!flat) {
        switch (pos) {
        case 0:
        case 1:
            return data_ov094_02209fc8[pos];
        case 2:
        case 3:
        case 4:
        case 5:
            if (work->rotation) {
                return data_ov094_0220a008[pos - 2];
            }
            if (work->triple) {
                return data_ov094_0220a038[pos - 2];
            }
            return data_ov094_02209fd0[pos - 2];
        case 6:
        case 7:
            if (work->rotation) {
                return data_ov094_0220a008[pos - 2];
            }
            return data_ov094_0220a038[pos - 2];
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return data_ov094_0220a020[pos - 8];
        }
    } else if (pos < 8) {
        if (pos & 1) {
            return FX32_CONST(16);
        }
        return FX32_CONST(32);
    } else {
        return FX32_CONST(16);
    }
}

// pokebw2's BtlvMcss_GetIndex
static int func_ov094_022019f4(BtlvMcss *work, int pos) {
    int i;

    for (i = 0; i < BTLV_MCSS_POS_MAX; i++) {
        if (work->entries[i].mcss != NULL && pos == work->entries[i].pos) {
            return i;
        }
    }
    return -1;
}

// pokebw2's BtlvMcss_GetShadowOffset
BOOL func_ov094_02201a20(u32 species, int pos, VecFx32 *offset, u16 *unk6, BOOL *shadow) {
    BOOL found = FALSE;
    u32 i;

    for (i = 0; i < NELEMS(data_ov094_0220a1e8); i++) {
        if (species == data_ov094_0220a1e8[i].species && (pos & 1) == data_ov094_0220a1e8[i].enemy) {
            offset->x = data_ov094_0220a1e8[i].x;
            offset->y = 0;
            offset->z = data_ov094_0220a1e8[i].z;
            *unk6 = data_ov094_0220a1e8[i].unk6;
            *shadow = data_ov094_0220a1e8[i].shadow;
            found = TRUE;
            break;
        }
    }
    return found;
}
