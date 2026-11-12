// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btlv_scu.c, ported by tools/port_file.py.
#include "types.h"
#include "work_area.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_string.h"
#include "battle/btlv.h"
#include "battle/btlv_effect.h"
#include "constants/battle.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "battle/btlv_b_gauge.h"

// The work of btlv_scu.c, the message and command unit of the battle display, recovered from its accesses in
// func_ov093_021eaf7c through func_ov093_021ee544. func_ov093_021eaf7c allocates it with
// GFL_HeapAllocate(heapId, 0x180, TRUE, <btlv_scu.c>, 351). Swan has no names for this file; the names are ours, from
// use.

// A window faded in and out by the BG1 alpha blend, its coefficients in fx32 (func_ov093_021ed76c, 021d3718,
// 021d3748, 021d3798)
typedef struct BtlvScuWinFade {
    BmpWin *window; // 0x00
    fx32 ev1;       // 0x04 the coefficient of the window, 31 shown and 0 hidden
    fx32 ev2;       // 0x08 the coefficient of the background, 7 shown and 16 hidden
    fx32 ev1Step;   // 0x0C
    fx32 ev2Step;   // 0x10
    u8 state;       // 0x14 0 shown, 1 hiding, 2 hidden, 3 showing, 4 show requested
    u8 frames;      // 0x15 frames left of the fade, 6 at its start
} BtlvScuWinFade;   // size 0x18

// The HP gauge of a battle position (func_ov093_021ed9a4, 021d3950, 021d3974, 021d398c)
typedef struct BtlvScuGauge {
    BattleMon *mon; // 0x00
    BtlvScu *scu;   // 0x04
    u8 pos;         // 0x08
    u8 viewPos;     // 0x09 func_ov093_021b8e70 of pos
    u8 shown;       // 0x0A set once func_ov094_021f803c shows it, cleared with func_ov094_021f80e4
    u8 active;      // 0x0B the position exists in this battle style
} BtlvScuGauge;     // size 0x0C

// An ability pop-up, one for each side, on BG2 or BG3 (func_ov093_021edb3c, and 021d3bb4 through 021d3f4c)
typedef struct BtlvScuAbilityWin {
    BtlvScu *scu;      // 0x00
    GFLBitmap *bitmap; // 0x04 18x4 characters
    u16 ability;       // 0x08 GetBattleMonStat(mon, 0x10)
    u8 side;           // 0x0A the index, 0 or 1, from func_ov093_021eda3c
    u8 bg;             // 0x0B 2, or 3 for side 1
    u8 monId;          // 0x0C 0x1F while none
    u8 seq;            // 0x0D
    void *chars;       // 0x10 the 0x900 bytes of BtlvScu.abilityChars for this side
    fx32 scrollX;      // 0x14
    fx32 scrollStep;   // 0x18
    u32 charOffset;    // 0x1C abilityCharOffset + side * 0x48
    u8 timer;          // 0x20
    u8 count;          // 0x21 the line drawn, then the blend level
    u8 shown : 1;      // 0x22 bit 0
    u8 flash : 1;      // 0x22 bit 1, the last argument of func_ov093_021edc28: go on to the palette flash
} BtlvScuAbilityWin;   // size 0x24

// The size of BtlvScu's work, the running procedure's (BtlvScuSingleWork and the others below), which they still fit on
// a PC target
#define BTLV_SCU_WORK_SIZE 0x40

struct BtlvScu {
    BmpWin *msgWin;                    // 0x000 BG1
    GFLBitmap *msgBitmap;              // 0x004
    BmpWin *statWin;                   // 0x008 the level-up stats window, BG3
    GFLBitmap *statBitmap;             // 0x00C
    u32 statFrameCharOffset;           // 0x010 the characters of file 0x1EB, the frame of statWin
    u32 abilityCharOffset;             // 0x014 the characters of file 0x1E3, the ability pop-ups
    PrintQueue *printQueue;            // 0x018
    PrintWindow printWindow;           // 0x01C PrintWindow_Init(.., msgWin) runs before msgWin exists: window is NULL
    Font *font;                        // 0x024
    Font *smallFont;                   // 0x028 the font of the ability pop-ups
    TCBExManager *tcbManager;          // 0x02C
    PrintStream *printStream;          // 0x030
    StrBuf *strbuf;                    // 0x034 0x400 characters
    StrBuf *abilityStrbuf;             // 0x038 0x180 characters
    StrBuf *lineStrbuf;                // 0x03C 0x180 characters, a line of abilityStrbuf
    u8 taskCount;                      // 0x040 tasks of func_ov093_021ed590, 021d35e0, 021d363c, 021d3830
    u8 gaugeShowTaskCount;             // 0x041 tasks of func_ov093_021ed3d4
    u8 gaugeHideTaskCount;             // 0x042 tasks of func_ov093_021ed318
    u8 unk043[5];                      // 0x043 never accessed
    BtlvScuGauge gauges[6];            // 0x048 by battle position
    BtlvScuAbilityWin abilityWins[2];  // 0x090 by side
    void *abilityCharFile;             // 0x0D8 the file GFL_G2DIOReadBGNCGR reads, freed with GFL_HeapFree
    NNSG2dCharacterData *abilityChars; // 0x0DC
    BtlvSubProc subProc;               // 0x0E0
    u8 work[BTLV_SCU_WORK_SIZE];       // 0x0F0 the work of the running procedure, one of the types below
    BtlvCore *core;                    // 0x130 only stored
    BtlMainModule *mainModule;         // 0x134
    BtlPokeCon *pokeCon;               // 0x138
    HeapID heapId;                     // 0x13C
    u8 printedAtOnce;                  // 0x13E set by func_ov093_021ece60: the window is neither cleared nor streamed
    u8 msgSeq;                         // 0x13F the state of func_ov093_021ecf68, 7 when done
    u8 clientId;                       // 0x140 only stored
    u8 msgFinished;                    // 0x141 set when the stream reports PRINT_STREAM_DONE, read and cleared by
                                       //       func_ov093_021ecf50
    u32 streamState;                   // 0x144 func_020223b4
    u16 waitTimer;                     // 0x148
    u16 pageWait;                      // 0x14A the wait after a paused page
    u16 endWait;                       // 0x14C the wait after the last page
    u8 unk14E;                         // 0x14E only cleared, in func_ov093_021eaf7c
    u8 encountStep;                    // 0x14F the step through the tables of the encounter procedures
    u8 recPlay;                        // 0x150 the argument of func_ov093_021eb32c, a recorded battle: skip the encounter
    BtlvScuWinFade msgFade;            // 0x154
    BattleMon *levelUpMon;             // 0x16C
    BattleMonLevelUp levelUp;          // 0x170 the gains, then the new stats, which func_ov093_021d590c writes over
};                                     // size 0x180

// The works of the procedures, which cast scu->work

// One mon: func_ov093_021eb690 (wild single), 021d15f8 (trainer single), 021d215c, 021d26c4
typedef struct BtlvScuSingleWork {
    BattleMon *mon; // 0x00
    u8 pos;         // 0x04 func_ov093_021b8ed8 of viewPos
    u8 viewPos;     // 0x05
    u8 monId;       // 0x06
    u8 clientId;    // 0x07 of the trainer, or of the player
    u8 unk08;       // 0x08 func_ov093_021b9064(mainModule, playerClientId, 0), a client ID for the message
} BtlvScuSingleWork; // size 0x0C

WORK_AREA_CHECK(BtlvScuSingleWork, BTLV_SCU_WORK_SIZE);

// Two wild mons: func_ov093_021ebb20. The arrays are in another order than in BtlvScuDoubleWork
typedef struct BtlvScuWildDoubleWork {
    BattleMon *mons[2]; // 0x00
    u8 monIds[2];       // 0x08
    u8 pos[2];          // 0x0A
    u8 viewPos[2];      // 0x0C
    u8 unk0E;           // 0x0E never accessed
    u8 clientId;        // 0x0F
} BtlvScuWildDoubleWork; // size 0x10

WORK_AREA_CHECK(BtlvScuWildDoubleWork, BTLV_SCU_WORK_SIZE);

// Two mons sent out together: func_ov093_021ec328, 021d23d4, 021d2820, 021d29bc
typedef struct BtlvScuDoubleWork {
    BattleMon *mons[2]; // 0x00
    u8 pos[2];          // 0x08
    u8 viewPos[2];      // 0x0A
    u8 monIds[2];       // 0x0C
    u8 count;           // 0x0E of the mons not fainted
    u8 clientId;        // 0x0F
} BtlvScuDoubleWork; // size 0x10

WORK_AREA_CHECK(BtlvScuDoubleWork, BTLV_SCU_WORK_SIZE);

// Two mons, the ids before their positions: func_ov093_021ebb20 (its accesses at 0x08 are the ids)
typedef struct BtlvScuDoubleIdWork {
    BattleMon *mons[2]; // 0x00
    u8 monIds[2];       // 0x08
    u8 pos[2];          // 0x0A
    u8 viewPos[2];      // 0x0C
    u8 unk0E;           // 0x0E
    u8 clientId;        // 0x0F
} BtlvScuDoubleIdWork; // size 0x10

WORK_AREA_CHECK(BtlvScuDoubleIdWork, BTLV_SCU_WORK_SIZE);

// Three: func_ov093_021ec5d0, 021d2b88
typedef struct BtlvScuTripleWork {
    BattleMon *mons[3]; // 0x00
    u8 pos[3];          // 0x0C
    u8 viewPos[3];      // 0x0F
    u8 monIds[3];       // 0x12
    u8 count;           // 0x15
    u8 clientId;        // 0x16
} BtlvScuTripleWork; // size 0x18

WORK_AREA_CHECK(BtlvScuTripleWork, BTLV_SCU_WORK_SIZE);

// The sprites of the opposing trainers: func_ov093_021ec0dc uses both, func_ov093_021ebfdc only viewPos[1]
typedef struct BtlvScuTrainerWork {
    u8 viewPos[2];
} BtlvScuTrainerWork;

// The works of the TCBEx tasks, as GFL_TCBExMgrAddTask sizes them

// func_ov093_021ed318 and its task func_ov093_021ed384: an effect, then the gauge hidden
typedef struct BtlvScuGaugeHideTask {
    BtlvScuGauge *gauge; // 0x00
    u16 seq;             // 0x04
    u16 pos;             // 0x06
    u16 effect;          // 0x08 0x26C if func_ov093_021ed318 got 0
    u8 *count;           // 0x0C &scu->gaugeHideTaskCount
} BtlvScuGaugeHideTask;  // size 0x10

// func_ov093_021ed3d4 and func_ov093_021ed460: the gauge shown once the effect ends
typedef struct BtlvScuGaugeShowTask {
    BtlvScuGauge *gauge; // 0x00
    u16 seq;             // 0x04
    u8 *count;           // 0x08 &scu->gaugeShowTaskCount
} BtlvScuGaugeShowTask;  // size 0x0C

// func_ov093_021ed590 and func_ov093_021ed5e8
typedef struct BtlvScuMonTask {
    BtlvScu *scu; // 0x00
    u8 pos;       // 0x04
    u32 viewPos;  // 0x08
    s32 seq;      // 0x0C
    u32 arg2;     // 0x10 func_ov094_021f7e38 rather than 021df6b4 if set
    u8 *count;    // 0x14 &scu->taskCount
} BtlvScuMonTask; // size 0x18

// func_ov093_021ed660, func_ov093_021ed6bc and their task func_ov093_021ed724
typedef struct BtlvScuPkmTask {
    BtlvScu *scu;  // 0x00
    u32 arg1;      // 0x04 the second argument of BtlvEffect_StartChangeSprite or 021df76c
    PartyPkm *pkm; // 0x08
    s32 seq;       // 0x0C
    u8 unk10;      // 0x10 never accessed
    u8 flag;       // 0x11 func_ov094_021f7e38 rather than 021df6b4 if set
    u8 *count;     // 0x14 &scu->taskCount
} BtlvScuPkmTask;  // size 0x18

// func_ov093_021ed8ac and func_ov093_021ed8e8: msgFade hides the message window, which is then cleared
typedef struct BtlvScuMsgHideTask {
    BtlvScu *scu; // 0x00
    s32 seq;      // 0x04
    u8 *count;    // 0x08 &scu->taskCount
} BtlvScuMsgHideTask; // size 0x0C

// A step of an encounter sequence, run until it returns TRUE
typedef BOOL (*BtlvScuEncountStep)(BtlvScu *scu, s32 *seq);

// The index of an ability pop-up, by the side of the battle position, from func_ov093_021eda3c. It returns an enum,
// as two returns kept in this order show
typedef enum BtlvScuSide {
    BTLV_SCU_SIDE_PLAYER,
    BTLV_SCU_SIDE_ENEMY,
} BtlvScuSide;

u16 func_ov093_021eb1d8(BtlvScu *scu, u32 arcId, u32 fileId, u32 bg);
static BOOL func_ov093_021eb318(BtlvScu *scu);
static void func_ov093_021eb584(s32 wait);
static BOOL func_ov093_021eb5b4(BtlvScu *scu, s32 *seq, const u8 *viewPos, u16 count);
static BOOL func_ov093_021eb660(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021eb670(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021eb680(BtlvScu *scu, s32 *seq);
BOOL func_ov093_021eb690(s32 *seq, void *arg);
u16 func_ov093_021eb820(BtlvScu *scu);
BOOL func_ov093_021eb870(s32 *seq, void *arg);
static BOOL func_ov093_021ebac0(s32 *seq, void *arg);
BOOL func_ov093_021ebb20(s32 *seq, void *arg);
static BOOL func_ov093_021ebf80(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021ebfa0(BtlvScu *scu, s32 *seq);
BOOL func_ov093_021ebfdc(BtlvScu *scu, s32 *seq, u8 clientId);
BOOL func_ov093_021ec0dc(BtlvScu *scu, s32 *seq, u8 clientId0, u8 clientId1);
BOOL func_ov093_021ec200(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021ec2ec(BtlvScu *scu, s32 *seq);
BOOL func_ov093_021ec328(BtlvScu *scu, s32 *seq, u8 clientId);
BOOL func_ov093_021ec478(BtlvScu *scu, s32 *seq, u8 clientId, u8 message);
BOOL func_ov093_021ec5d0(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021ec768(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021ec88c(BtlvScu *scu, s32 *seq);
static BOOL func_ov093_021ec8c4(BtlvScu *scu, s32 *seq, u8 clientId);
static BOOL func_ov093_021eca60(BtlvScu *scu, s32 *seq, u8 clientId, u8 partnerId);
static BOOL func_ov093_021ecc2c(BtlvScu *scu, s32 *seq);
u16 func_ov093_021ece08(BtlvScu *scu, u8 count);
BOOL func_ov093_021ed158(s32 *seq, void *arg);
void func_ov093_021ed384(TCBEx *task, void *data);
void func_ov093_021ed460(TCBEx *task, void *data);
void func_ov093_021ed5e8(TCBEx *task, void *data);
void func_ov093_021ed724(TCBEx *task, void *data);
void func_ov093_021ed76c(BtlvScuWinFade *fade, BmpWin *window);
static void func_ov093_021ed798(BtlvScuWinFade *fade);
void func_ov093_021ed7c8(BtlvScuWinFade *fade, BOOL keepWindow);
static BOOL func_ov093_021ed814(BtlvScuWinFade *fade);
static void func_ov093_021ed8e8(TCBEx *task, void *data);
void func_ov093_021ed930(BtlvScu *scu);
static void func_ov093_021ed988(BtlvScu *scu);
void func_ov093_021ed9a4(BtlvScuGauge *gauge, BtlvScu *scu, u8 pos);
static void func_ov093_021ed9c8(BtlvScuGauge *gauge);
static void func_ov093_021ed9ec(BtlvScuGauge *gauge);
static void func_ov093_021eda04(BtlvScuGauge *gauge);
static BtlvScuSide func_ov093_021eda3c(BtlMainModule *mainModule, u8 pos);
void func_ov093_021edb3c(BtlvScu *scu, u32 charOffset);
static void func_ov093_021edbe8(BtlvScu *scu);
static void func_ov093_021edc28(BtlvScuAbilityWin *win, u8 pos, BOOL flash);
BOOL func_ov093_021edca8(BtlvScuAbilityWin *win);
static void func_ov093_021ede54(BtlvScuAbilityWin *win);
static void func_ov093_021edf4c(BtlvScuAbilityWin *win);
static BOOL func_ov093_021edf78(BtlvScuAbilityWin *win);
static void func_ov093_021edf7c(BtlvScuAbilityWin *win, u8 pos);
static BOOL func_ov093_021edfc4(BtlvScuAbilityWin *win);
void func_ov093_021ee104(BtlvScu *scu, BtlvBGaugeParam *param, u8 clientId, u8 otherClientId, u32 side);
static BOOL func_ov093_021ee3e8(s32 *seq, void *arg);
static BOOL func_ov093_021ee4b4(s32 *seq, void *arg);
static BOOL func_ov093_021ee544(s32 *seq, void *arg);

// The view positions whose mons a recorded battle shows at once, by style
// pokebw2's sSingleViewPos
static const u8 data_ov093_021f38ac[] = {1, 0};
// pokebw2's sDoubleViewPos
static const u8 data_ov093_021f38ae[] = {2, 3, 4, 5};
// pokebw2's sTripleViewPos
static const u8 data_ov093_021f38b2[] = {2, 3, 4, 5, 6, 7};

// The steps of the encounter sequences, by func_ov093_021eb32c's choice. The two-dimensional ones go by step, then by
// the player's client ID. Black's order of the definitions, which MWCC's size sort lays out as the game does
// pokebw2's sTripleSteps
static const BtlvScuEncountStep data_ov093_021f38c4[] = {func_ov093_021ebf80, func_ov093_021ec5d0, func_ov093_021ecc2c};
// pokebw2's sDoubleTrainerSteps
static const BtlvScuEncountStep data_ov093_021f38d0[] = {func_ov093_021ebf80, func_ov093_021ec2ec, func_ov093_021ec88c};
// pokebw2's sDoubleTrainerMultiSteps
static const BtlvScuEncountStep data_ov093_021f38dc[] = {func_ov093_021ebfa0, func_ov093_021ec2ec,
                                                              func_ov093_021ec88c};
// pokebw2's sDoubleCommMultiSteps
static const BtlvScuEncountStep data_ov093_021f38b8[] = {func_ov093_021ebfa0, func_ov093_021ec2ec,
                                                           func_ov093_021ec88c};
// pokebw2's sSingleCommSteps
static const BtlvScuEncountStep data_ov093_021f38e8[][2] = {
    {func_ov093_021ebf80, func_ov093_021ebf80},
    {func_ov093_021ec200, func_ov093_021ec768},
    {func_ov093_021ec768, func_ov093_021ec200},
};
// pokebw2's sDoubleCommSteps
static const BtlvScuEncountStep data_ov093_021f3900[][2] = {
    {func_ov093_021ebf80, func_ov093_021ebf80},
    {func_ov093_021ec2ec, func_ov093_021ec88c},
    {func_ov093_021ec88c, func_ov093_021ec2ec},
};
// pokebw2's sTripleCommSteps
static const BtlvScuEncountStep data_ov093_021f3918[][2] = {
    {func_ov093_021ebf80, func_ov093_021ebf80},
    {func_ov093_021ec5d0, func_ov093_021ecc2c},
    {func_ov093_021ecc2c, func_ov093_021ec5d0},
};

// The BG setups, declared in the only order found that lays them out as the game does: BG1, BG2, BG3
// BG1, the message window
// pokebw2's sMsgBGSetup
static const BGSetup data_ov093_021f3930 = {0, 0, 0x800, 0, 1, 0, 9, 4, 0x8000, 0, 0, 0, 0};
// BG2, the ability pop-up of side 0
// pokebw2's sAbilityBG2Setup
static const BGSetup data_ov093_021f3950 = {0x90, 0, 0x1000, 0, 3, 0, 10, 2, 0x8000, 0, 0, 0, 0};
// BG3, the ability pop-up of side 1 and the level-up stats
// pokebw2's sAbilityBG3Setup
static const BGSetup data_ov093_021f3970 = {-0x90, 0, 0x2000, 0, 4, 0, 12, 2, 0x8000, 0, 0, 0, 0};

BOOL func_ov093_021ebe28(s32 *seq, void *arg);
BOOL func_ov093_021ebe88(s32 *seq, void *arg);
BOOL func_ov093_021ebd90(s32 *seq, void *arg);
BOOL func_ov093_021ebddc(s32 *seq, void *arg);
BOOL func_ov093_021ebed4(s32 *seq, void *arg);
BOOL func_ov093_021ebf20(s32 *seq, void *arg);

static inline void BtlvScu_SetSubProc(BtlvScu *scu, BtlvSubProcFn main) {
    scu->subProc.init = NULL;
    scu->subProc.main = main;
    scu->subProc.arg = scu;
    scu->subProc.seq = 0;
}

static inline BOOL BtlvScu_RunSubProc(BtlvScu *scu) {
    if (scu->subProc.init != NULL) {
        if (scu->subProc.init(&scu->subProc.seq, scu->subProc.arg)) {
            scu->subProc.init = NULL;
            scu->subProc.seq = 0;
        }
        return FALSE;
    }
    if (scu->subProc.main != NULL) {
        if (scu->subProc.main(&scu->subProc.seq, scu->subProc.arg)) {
            scu->subProc.main = NULL;
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

BtlvScu *func_ov093_021eaf7c(BtlvCore *core, BtlMainModule *mainModule, BtlPokeCon *pokeCon, TCBExManager *tcbManager,
                             Font *font, Font *smallFont, u8 clientId, HeapID heapId) {
    BtlvScu *scu = GFL_HeapAllocate(heapId, sizeof(BtlvScu), TRUE, "btlv_scu.c", 337);

    scu->core = core;
    scu->mainModule = mainModule;
    scu->pokeCon = pokeCon;
    scu->heapId = heapId;
    scu->clientId = clientId;
    scu->font = font;
    scu->smallFont = smallFont;
    scu->recPlay = FALSE;
    scu->printStream = NULL;
    scu->tcbManager = tcbManager;
    scu->strbuf = GFL_StrBufCreate(0x400, heapId);
    scu->abilityStrbuf = GFL_StrBufCreate(0x180, heapId);
    scu->lineStrbuf = GFL_StrBufCreate(0x180, heapId);
    scu->unk14E = 0;
    scu->abilityCharFile = NULL;
    scu->printQueue = func_0201c440(scu->heapId);
    PrintWindow_Init(&scu->printWindow, scu->msgWin);
    return scu;
}

void func_ov093_021eb028(BtlvScu *scu) {
    u32 frameChar;
    u16 black;

    GFL_BGSysCreateBG(1, &data_ov093_021f3930, 0);
    GFL_BGSysCreateBG(2, &data_ov093_021f3950, 0);
    GFL_BGSysCreateBG(3, &data_ov093_021f3970, 0);
    func_020215ec(func_ov094_021f86e0(), 11, 0x5a, scu->heapId, 0, 0x20, 0, 0);
    func_020215ec(func_ov094_021f86e0(), 11, 0xd6, scu->heapId, 0, 0x60, 0x10, 0x10);
    black = 0;
    func_020215bc(func_ov094_021f86e0(), &black, 0, 0, sizeof(black));
    GFL_BGSysFillScrArea(1, 0, 0, 0, 32, 32, 0x11);
    GFL_BGSysFillChar(1, 0, 1, 0);
    frameChar = func_ov093_021eb1d8(scu, 11, 0x59, 1);
    scu->abilityCharOffset = func_ov093_021eb1d8(scu, 11, 0xd5, 3);
    loadBGScrToVramByNarcNoReserveNegAlign(11, 0xd7, 2, 0, 0, FALSE, scu->heapId);
    loadBGScrToVramByNarcNoReserveNegAlign(11, 0xd8, 3, 0, 0, FALSE, scu->heapId);
    scu->msgWin = BmpWin_CreateDynamic(1, 1, 19, 30, 4, 0, 0);
    scu->msgBitmap = BmpWin_GetBitmap(scu->msgWin);
    BmpWin_FlushMap(scu->msgWin);
    func_020454e0(scu->msgWin, frameChar, 0);
    GFL_BitmapFill(scu->msgBitmap, 0xc);
    BmpWin_FlushChar(scu->msgWin);
    scu->statFrameCharOffset = func_ov093_021eb1d8(scu, 11, 0xdd, 3);
    scu->statWin = BmpWin_CreateDynamic(3, 0x15, 0x25, 0xa, 0xc, 3, 0);
    scu->statBitmap = BmpWin_GetBitmap(scu->statWin);
    func_ov093_021ed76c(&scu->msgFade, scu->msgWin);
    func_ov093_021ed930(scu);
    func_ov093_021edb3c(scu, scu->abilityCharOffset);
    GFL_BGSysLoadScr(1);
    GFL_BGSysLoadScr(2);
    GFL_BGSysLoadScr(3);
    GFL_BGSysSetBGEnabled(0, 1);
    GFL_BGSysSetBGEnabled(1, 1);
    GFL_BGSysSetBGEnabled(2, 1);
    GFL_BGSysSetBGEnabled(3, 1);
    GFL_BGSysSetBGEnabledA(0x10, 1);
}


u16 func_ov093_021eb1d8(BtlvScu *scu, u32 arcId, u32 fileId, u32 bg) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(arcId, HEAPID_TAIL(scu->heapId));
    int offset = GFL_BGSysLoadArcNCGRDynamic(arc, fileId, bg, 0, FALSE, HEAPID_TAIL(scu->heapId));

    GFL_ArcToolFree(arc);
    return offset;
}

void func_ov093_021eb234(BtlvScu *scu) {
    func_ov093_021edbe8(scu);
    func_ov093_021ed988(scu);
    if (scu->statWin != NULL) {
        BmpWin_Free(scu->statWin);
        scu->statWin = NULL;
    }
    if (scu->msgWin != NULL) {
        BmpWin_Free(scu->msgWin);
        scu->msgWin = NULL;
    }
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    if (scu->printQueue != NULL) {
        func_0201c4c0(scu->printQueue);
        scu->printQueue = NULL;
    }
    if (scu->lineStrbuf != NULL) {
        GFL_StrBufFree(scu->lineStrbuf);
        scu->lineStrbuf = NULL;
    }
    if (scu->abilityStrbuf != NULL) {
        GFL_StrBufFree(scu->abilityStrbuf);
        scu->abilityStrbuf = NULL;
    }
    GFL_StrBufFree(scu->strbuf);
    GFL_HeapFree(scu);
}

void func_ov093_021eb2a8(const BtlvScu *scu) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0xb, HEAPID_TAIL(scu->heapId));

    GFL_BGSysLoadArcNCGRStatic(arc, 0xd5, 3, scu->abilityCharOffset, 0, FALSE, scu->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0xdd, 3, scu->statFrameCharOffset, 0, FALSE, scu->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysClearScr(3);
    loadBGScrToVramByNarcNoReserveNegAlign(0xb, 0xd8, 3, 0, 0, FALSE, scu->heapId);
}

static BOOL func_ov093_021eb318(BtlvScu *scu) {
    if (!GFL_FadeIsRunning()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021eb32c(BtlvScu *scu, BOOL recPlay) {
    u32 type = BtlSetup_GetBattleType(scu->mainModule);

    scu->encountStep = 0;
    scu->recPlay = recPlay;
    switch (BtlSetup_GetBattleStyle(scu->mainModule)) {
    case BTL_STYLE_SINGLE:
        switch (type) {
        case 0:
        case 4:
        default:
            BtlvScu_SetSubProc(scu, func_ov093_021eb690);
            break;
        case 1:
        case 2:
            BtlvScu_SetSubProc(scu, func_ov093_021eb870);
            break;
        case 3:
            BtlvScu_SetSubProc(scu, func_ov093_021ebac0);
            break;
        }
        break;
    case BTL_STYLE_DOUBLE:
        switch (type) {
        case 0:
            BtlvScu_SetSubProc(scu, func_ov093_021ebb20);
            break;
        case 3:
            if (!func_ov093_021b86c8(scu->mainModule)) {
                BtlvScu_SetSubProc(scu, func_ov093_021ebe28);
            } else {
                BtlvScu_SetSubProc(scu, func_ov093_021ebe88);
            }
            break;
        case 1:
        case 2:
            if (!func_ov093_021b86c8(scu->mainModule)) {
                BtlvScu_SetSubProc(scu, func_ov093_021ebd90);
            } else {
                BtlvScu_SetSubProc(scu, func_ov093_021ebddc);
            }
            break;
        }
        break;
    case BTL_STYLE_TRIPLE:
        if (type != 3) {
            BtlvScu_SetSubProc(scu, func_ov093_021ebed4);
        } else {
            BtlvScu_SetSubProc(scu, func_ov093_021ebf20);
        }
        break;
    case BTL_STYLE_ROTATION:
        if (type != 3) {
            BtlvScu_SetSubProc(scu, func_ov093_021ebed4);
        } else {
            BtlvScu_SetSubProc(scu, func_ov093_021ebf20);
        }
        break;
    }
}

BOOL func_ov093_021eb52c(BtlvScu *scu) {
    return BtlvScu_RunSubProc(scu);
}

static void func_ov093_021eb584(s32 wait) {
    if (GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR) <= 0) {
        GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, wait);
    } else {
        GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 16, 0, wait);
    }
}

// A recorded battle shows the mons and their gauges at once
static BOOL func_ov093_021eb5b4(BtlvScu *scu, s32 *seq, const u8 *viewPos, u16 count) {
    u8 view;
    u8 pos;
    u32 i;

    switch (*seq) {
    case 0:
        for (i = 0; i < count; i++) {
            view = viewPos[i];
            pos = func_ov093_021b8ed8(scu->mainModule, view);
            func_ov094_021f7ee8(func_ov093_021d5864(func_ov093_021b98ac(scu->pokeCon, pos)), view);
            func_ov093_021eda04(&scu->gauges[pos]);
        }
        (*seq)++;
        break;
    case 1:
        if (scu->recPlay) {
            return TRUE;
        }
        func_ov093_021eb584(-3);
        (*seq)++;
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL func_ov093_021eb660(BtlvScu *scu, s32 *seq) {
    return func_ov093_021eb5b4(scu, seq, data_ov093_021f38ac, NELEMS(data_ov093_021f38ac));
}

static BOOL func_ov093_021eb670(BtlvScu *scu, s32 *seq) {
    return func_ov093_021eb5b4(scu, seq, data_ov093_021f38ae, NELEMS(data_ov093_021f38ae));
}

static BOOL func_ov093_021eb680(BtlvScu *scu, s32 *seq) {
    return func_ov093_021eb5b4(scu, seq, data_ov093_021f38b2, NELEMS(data_ov093_021f38b2));
}

BOOL func_ov093_021eb690(s32 *seq, void *arg) {
    BtlvScu *scu = arg;
    BtlvScuSingleWork *work = (BtlvScuSingleWork *)scu->work;

    if (scu->recPlay) {
        return func_ov093_021eb660(scu, seq);
    }
    switch (*seq) {
    case 0:
        work->viewPos = 1;
        work->pos = func_ov093_021b8ed8(scu->mainModule, work->viewPos);
        work->mon = func_ov093_021b98ac(scu->pokeCon, work->pos);
        work->monId = GetMonID(work->mon);
        func_ov093_021ed798(&scu->msgFade);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov094_021f7ee8(func_ov093_021d5864(work->mon), work->viewPos);
            func_ov094_021f7a28(work->viewPos, 0x231);
            func_ov093_021eb584(2);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            u16 message = func_ov093_021eb820(scu);
            func_ov093_021eee24(scu->strbuf, message, 1, work->monId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021eda04(&scu->gauges[work->pos]);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade)) {
            int effect = BtlSetup_GetBattleType(scu->mainModule) == 4 ? 0x23c : 0x232;
            func_ov094_021f7998(effect);
            (*seq)++;
        }
        break;
    case 5:
        if (!func_ov094_021f7eb4()) {
            work->viewPos = 0;
            work->pos = func_ov093_021b8ed8(scu->mainModule, work->viewPos);
            work->mon = func_ov093_021b98ac(scu->pokeCon, work->pos);
            work->monId = GetMonID(work->mon);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mon), work->viewPos);
            func_ov094_021f7998(0x234);
            func_ov093_021eee24(scu->strbuf, 11, 1, work->monId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 6:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 7:
        if (func_ov093_021ed814(&scu->msgFade)) {
            (*seq)++;
        }
        break;
    case 8:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos]);
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// The message that a wild mon appeared, by the battle's type
u16 func_ov093_021eb820(BtlvScu *scu) {
    if (BtlSetup_IsBattleType(scu->mainModule, 4)) {
        return 6;
    }
    if (BtlSetup_IsBattleType(scu->mainModule, 8)) {
        return 3;
    }
    if (BtlSetup_IsBattleType(scu->mainModule, 0x80)) {
        return 4;
    }
    if (BtlSetup_IsBattleType(scu->mainModule, 0x10)) {
        return 5;
    }
    return 1;
}

BOOL func_ov093_021eb870(s32 *seq, void *arg) {
    BtlvScu *scu = arg;
    BtlvScuSingleWork *work = (BtlvScuSingleWork *)scu->work;
    u8 pos;
    u8 clientId;

    if (scu->recPlay) {
        return func_ov093_021eb660(scu, seq);
    }
    switch (*seq) {
    case 0:
        pos = func_ov093_021b8ed8(scu->mainModule, 1);
        clientId = func_ov093_021b8de4(scu->mainModule, pos);
        func_ov094_021f7f58(func_ov093_021ba020(scu->mainModule, clientId), 9, 0, 0, 0);
        work->viewPos = 1;
        work->clientId = clientId;
        work->pos = pos;
        work->mon = func_ov093_021b98ac(scu->pokeCon, work->pos);
        work->monId = GetMonID(work->mon);
        func_ov093_021ed798(&scu->msgFade);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov094_021f7a28(work->viewPos, 0x237);
            func_ov093_021eb584(2);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            BtlvBGaugeParam balls;

            func_ov093_021ee104(scu, &balls, work->clientId, 4, 1);
            func_ov094_021f823c(&balls);
            func_ov093_021eee24(scu->strbuf, 7, 1, work->clientId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f7a28(work->viewPos, 0x238);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eee24(scu->strbuf, 0xe, 2, 1, work->monId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f7ee8(func_ov093_021d5864(work->mon), work->viewPos);
            func_ov094_021f7a28(work->viewPos, 0x239);
            func_ov094_021f8260(1);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 6:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            BtlvBGaugeParam balls;

            clientId = GetPlayerClientID(scu->mainModule);
            func_ov093_021ee104(scu, &balls, clientId, 4, 0);
            func_ov094_021f823c(&balls);
            (*seq)++;
        }
        break;
    case 7:
        func_ov093_021eda04(&scu->gauges[work->pos]);
        func_ov094_021f7998(0x232);
        (*seq)++;
        break;
    case 8:
        if (!func_ov094_021f7eb4() && !func_ov094_021f827c(0)) {
            work->viewPos = 0;
            work->pos = func_ov093_021b8ed8(scu->mainModule, work->viewPos);
            work->mon = func_ov093_021b98ac(scu->pokeCon, work->pos);
            work->monId = GetMonID(work->mon);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mon), work->viewPos);
            func_ov094_021f7998(0x234);
            func_ov093_021eee24(scu->strbuf, 0xb, 1, work->monId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 9:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f8260(0);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 10:
        if (func_ov093_021ed814(&scu->msgFade)) {
            (*seq)++;
        }
        break;
    case 11:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos]);
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// A single battle over communication
static BOOL func_ov093_021ebac0(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb660(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f38e8)) {
        if (data_ov093_021f38e8[scu->encountStep][GetPlayerClientID(scu->mainModule)](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021ebb20(s32 *seq, void *arg) {
    BtlvScu *scu = arg;
    BtlvScuDoubleIdWork *work = (BtlvScuDoubleIdWork *)scu->work;
    u8 slot;
    u8 viewPos;
    u32 i;

    if (scu->recPlay) {
        return func_ov093_021eb670(scu, seq);
    }
    switch (*seq) {
    case 0:
        slot = func_ov093_021b8bec(scu->mainModule, scu->clientId, 0);
        work->pos[0] = func_ov093_021b8c50(scu->mainModule, slot, 0);
        work->mons[0] = func_ov093_021b98ac(scu->pokeCon, work->pos[0]);
        work->monIds[0] = GetMonID(work->mons[0]);
        work->pos[1] = func_ov093_021b8c50(scu->mainModule, slot, 1);
        work->mons[1] = func_ov093_021b98ac(scu->pokeCon, work->pos[1]);
        work->monIds[1] = GetMonID(work->mons[1]);
        func_ov093_021ed798(&scu->msgFade);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed814(&scu->msgFade)) {
            viewPos = func_ov093_021b8e70(scu->mainModule, work->pos[0]);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), viewPos);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            viewPos = func_ov093_021b8e70(scu->mainModule, work->pos[1]);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), viewPos);
            func_ov094_021f7a28(viewPos, 0x231);
            func_ov093_021eb584(2);
            (*seq)++;
        }
        break;
    case 3:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021eee24(scu->strbuf, 2, 2, work->monIds[0], work->monIds[1]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov094_021f7998(0x232);
            (*seq)++;
        }
        break;
    case 6:
        if (!func_ov094_021f7eb4()) {
            work->viewPos[0] = 2;
            work->viewPos[1] = 4;
            work->clientId = GetPlayerClientID(scu->mainModule);
            for (i = 0; i < 2; i++) {
                work->pos[i] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[i]);
                work->mons[i] = func_ov093_021b98ac(scu->pokeCon, work->pos[i]);
                if (work->mons[i] != NULL) {
                    work->monIds[i] = GetMonID(work->mons[i]);
                }
            }
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
            func_ov094_021f7998(0x234);
            func_ov093_021eee24(scu->strbuf, 12, 2, work->monIds[0], work->monIds[1]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 7:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 8:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            (*seq)++;
        }
        break;
    case 9:
        if (!func_ov094_021f7eb4()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021ebd90(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb670(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f38d0)) {
        if (data_ov093_021f38d0[scu->encountStep](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021ebddc(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb670(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f38dc)) {
        if (data_ov093_021f38dc[scu->encountStep](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021ebe28(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb670(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f3900)) {
        if (data_ov093_021f3900[scu->encountStep][GetPlayerClientID(scu->mainModule)](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021ebe88(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb670(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f38b8)) {
        if (data_ov093_021f38b8[scu->encountStep](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021ebed4(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb680(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f38c4)) {
        if (data_ov093_021f38c4[scu->encountStep](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021ebf20(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    if (scu->recPlay) {
        return func_ov093_021eb680(scu, seq);
    }
    if (scu->encountStep < NELEMS(data_ov093_021f3918)) {
        if (data_ov093_021f3918[scu->encountStep][GetPlayerClientID(scu->mainModule)](scu, seq)) {
            scu->encountStep++;
            *seq = 0;
        }
        return FALSE;
    }
    return TRUE;
}

static BOOL func_ov093_021ebf80(BtlvScu *scu, s32 *seq) {
    return func_ov093_021ebfdc(scu, seq, func_ov093_021b904c(scu->mainModule, 0));
}

static BOOL func_ov093_021ebfa0(BtlvScu *scu, s32 *seq) {
    u8 clientId1 = func_ov093_021b904c(scu->mainModule, 0);
    u8 clientId2 = func_ov093_021b904c(scu->mainModule, 1);

    if (clientId1 != clientId2) {
        return func_ov093_021ec0dc(scu, seq, clientId1, clientId2);
    }
    return func_ov093_021ebfdc(scu, seq, clientId1);
}

BOOL func_ov093_021ebfdc(BtlvScu *scu, s32 *seq, u8 clientId) {
    BtlvScuTrainerWork *work = (BtlvScuTrainerWork *)scu->work;
    BtlvBGaugeParam balls;
    u32 message;
    u32 type;

    switch (*seq) {
    case 0:
        work->viewPos[1] = 9;
        func_ov094_021f7f58(func_ov093_021ba020(scu->mainModule, clientId), work->viewPos[1], 0, 0, 0);
        func_ov093_021ed798(&scu->msgFade);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov094_021f7a28(work->viewPos[1], 0x237);
            func_ov093_021eb584(2);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            type = BtlSetup_GetBattleType(scu->mainModule);
            message = 7;
            if (type == 3) {
                message = 8;
            }
            func_ov093_021ee104(scu, &balls, clientId, 4, 1);
            func_ov094_021f823c(&balls);
            func_ov093_021eee24(scu->strbuf, message, 1, clientId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f7a28(work->viewPos[1], 0x238);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL func_ov093_021ec0dc(BtlvScu *scu, s32 *seq, u8 clientId0, u8 clientId1) {
    BtlvScuTrainerWork *work = (BtlvScuTrainerWork *)scu->work;
    BtlvBGaugeParam balls;
    s16 mode;

    switch (*seq) {
    case 0:
        work->viewPos[0] = 11;
        work->viewPos[1] = 13;
        func_ov094_021f7f58(func_ov093_021ba020(scu->mainModule, clientId0), work->viewPos[0], 0, 0, 0);
        func_ov094_021f7f58(func_ov093_021ba020(scu->mainModule, clientId1), work->viewPos[1], 0, 0, 0);
        func_ov093_021ed798(&scu->msgFade);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov094_021f7a28(work->viewPos[0], 0x237);
            func_ov093_021eb584(2);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            u32 battleType = BtlSetup_GetBattleType(scu->mainModule);
            u32 mode = 9;
            if (battleType == 3) {
                mode = 10;
            }
            func_ov093_021ee104(scu, &balls, clientId0, clientId1, 1);
            func_ov094_021f823c(&balls);
            func_ov093_021eee24(scu->strbuf, mode, 2, clientId0, clientId1);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f7a28(work->viewPos[0], 0x238);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL func_ov093_021ec200(BtlvScu *scu, s32 *seq) {
    BtlvScuSingleWork *work = (BtlvScuSingleWork *)scu->work;

    switch (*seq) {
    case 0:
        work->viewPos = 1;
        work->unk08 = func_ov093_021b9064(scu->mainModule, GetPlayerClientID(scu->mainModule), 0);
        work->pos = func_ov093_021b8ed8(scu->mainModule, work->viewPos);
        work->mon = func_ov093_021b98ac(scu->pokeCon, work->pos);
        work->monId = GetMonID(work->mon);
        func_ov093_021eee24(scu->strbuf, func_ov093_021ece08(scu, 1), 2, work->unk08, work->monId);
        func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 2:
        func_ov094_021f7ee8(func_ov093_021d5864(work->mon), work->viewPos);
        func_ov094_021f7a28(work->viewPos, 0x239);
        func_ov094_021f8260(1);
        (*seq)++;
        break;
    case 3:
        if (func_ov093_021ed814(&scu->msgFade)) {
            if (!func_ov094_021f7eb4()) {
                func_ov093_021eda04(&scu->gauges[work->pos]);
                (*seq)++;
            }
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}


// The opponents send out two mons, from one trainer or from two
static BOOL func_ov093_021ec2ec(BtlvScu *scu, s32 *seq) {
    u8 clientId1 = func_ov093_021b904c(scu->mainModule, 0);
    u8 clientId2 = func_ov093_021b904c(scu->mainModule, 1);

    if (clientId1 != clientId2) {
        return func_ov093_021ec478(scu, seq, clientId1, clientId2);
    }
    return func_ov093_021ec328(scu, seq, clientId1);
}

BOOL func_ov093_021ec328(BtlvScu *scu, s32 *seq, u8 clientId) {
    BtlvScuDoubleWork *work = (BtlvScuDoubleWork *)scu->work;
    u32 i;

    switch (*seq) {
    case 0:
        work->viewPos[0] = 3;
        work->viewPos[1] = 5;
        work->clientId = clientId;
        work->count = 0;
        for (i = 0; i < 2; i++) {
            work->pos[i] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[i]);
            work->mons[i] = func_ov093_021b98ac(scu->pokeCon, work->pos[i]);
            work->monIds[i] = GetMonID(work->mons[i]);
            if (!IsFainted(work->mons[i])) {
                work->count++;
            }
        }
        func_ov093_021eee24(scu->strbuf, func_ov093_021ece08(scu, work->count), 3, work->clientId, work->monIds[0],
                            work->monIds[1]);
        func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 2:
        func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
        if (work->count == 2) {
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
        }
        func_ov094_021f7a28(work->viewPos[0], 0x239);
        func_ov094_021f8260(1);
        (*seq)++;
        break;
    case 3:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            if (work->count == 2) {
                func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            }
            (*seq)++;
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021ec478(BtlvScu *scu, s32 *seq, u8 clientId, u8 message) {
    BtlvScuDoubleWork *work = (BtlvScuDoubleWork *)scu->work;

    switch (*seq) {
    case 0:
        work->viewPos[0] = 3;
        work->pos[0] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[0]);
        work->mons[0] = func_ov093_021b98ac(scu->pokeCon, work->pos[0]);
        work->monIds[0] = GetMonID(work->mons[0]);
        work->viewPos[1] = 5;
        work->pos[1] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[1]);
        work->mons[1] = func_ov093_021b98ac(scu->pokeCon, work->pos[1]);
        work->monIds[1] = GetMonID(work->mons[1]);
        func_ov093_021eee24(scu->strbuf, func_ov093_021ece08(scu, 1), 2, clientId, work->monIds[0]);
        func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov093_021eee24(scu->strbuf, func_ov093_021ece08(scu, 1), 2, message, work->monIds[1]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
            func_ov094_021f7a28(work->viewPos[0], 0x239);
            func_ov094_021f8260(1);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            (*seq)++;
        }
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021ec5d0(BtlvScu *scu, s32 *seq) {
    BtlvScuTripleWork *work = (BtlvScuTripleWork *)scu->work;
    u32 i;

    switch (*seq) {
    case 0:
        work->viewPos[0] = 3;
        work->viewPos[1] = 5;
        work->viewPos[2] = 7;
        work->count = 0;
        work->clientId = func_ov093_021b9064(scu->mainModule, GetPlayerClientID(scu->mainModule), 0);
        for (i = 0; i < 3; i++) {
            work->pos[i] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[i]);
            work->mons[i] = func_ov093_021b98ac(scu->pokeCon, work->pos[i]);
            work->monIds[i] = GetMonID(work->mons[i]);
            if (!IsFainted(work->mons[i])) {
                work->count++;
            }
        }
        func_ov093_021eee24(scu->strbuf, func_ov093_021ece08(scu, work->count), 4, work->clientId, work->monIds[0],
                            work->monIds[1], work->monIds[2]);
        func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 2:
        func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
        if (work->count > 1) {
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
        }
        if (work->count > 2) {
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[2]), work->viewPos[2]);
        }
        func_ov094_021f7a28(work->viewPos[0], 0x239);
        func_ov094_021f8260(1);
        (*seq)++;
        break;
    case 3:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            if (work->count > 1) {
                func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            }
            if (work->count > 2) {
                func_ov093_021eda04(&scu->gauges[work->pos[2]]);
            }
            (*seq)++;
        }
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}




// The player sends out one mon
static BOOL func_ov093_021ec768(BtlvScu *scu, s32 *seq) {
    BtlvScuSingleWork *work = (BtlvScuSingleWork *)scu->work;
    BtlvBGaugeParam balls;

    switch (*seq) {
    case 0:
        work->clientId = GetPlayerClientID(scu->mainModule);
        func_ov093_021ee104(scu, &balls, work->clientId, 4, 0);
        func_ov094_021f823c(&balls);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f7998(0x232);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4() && !func_ov094_021f827c(0)) {
            work->viewPos = 0;
            work->clientId = GetPlayerClientID(scu->mainModule);
            work->pos = func_ov093_021b8ed8(scu->mainModule, work->viewPos);
            work->mon = func_ov093_021b98ac(scu->pokeCon, work->pos);
            work->monId = GetMonID(work->mon);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mon), work->viewPos);
            func_ov094_021f7998(0x234);
            func_ov093_021eee24(scu->strbuf, 11, 1, work->monId);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f8260(0);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos]);
            (*seq)++;
        }
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}

// The player sends out two mons, alone or with a partner
static BOOL func_ov093_021ec88c(BtlvScu *scu, s32 *seq) {
    u8 clientId = GetPlayerClientID(scu->mainModule);
    u8 partnerId = func_ov093_021b9000(scu->mainModule);

    if (partnerId == 4) {
        return func_ov093_021ec8c4(scu, seq, clientId);
    }
    return func_ov093_021eca60(scu, seq, clientId, partnerId);
}

static BOOL func_ov093_021ec8c4(BtlvScu *scu, s32 *seq, u8 clientId) {
    BtlvScuDoubleWork *work = (BtlvScuDoubleWork *)scu->work;
    u32 i;
    BtlvBGaugeParam balls;

    switch (*seq) {
    case 0:
        work->clientId = clientId;
        func_ov093_021ee104(scu, &balls, work->clientId, 4, 0);
        func_ov094_021f823c(&balls);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f7998(0x232);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4() && !func_ov094_021f827c(0)) {
            u16 message;

            work->viewPos[0] = 2;
            work->viewPos[1] = 4;
            work->count = 0;
            work->clientId = GetPlayerClientID(scu->mainModule);
            for (i = 0; i < 2; i++) {
                BattleMon **slot;

                work->pos[i] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[i]);
                slot = &work->mons[i];
                *slot = func_ov093_021b98ac(scu->pokeCon, work->pos[i]);
                if (*slot != NULL) {
                    work->monIds[i] = GetMonID(*slot);
                    if (!IsFainted(*slot)) {
                        work->count++;
                    }
                }
            }
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
            if (work->count > 1) {
                func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
            }
            func_ov094_021f7998(0x234);
            message = work->count == 2 ? 12 : 11;
            func_ov093_021eee24(scu->strbuf, message, 2, work->monIds[0], work->monIds[1]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f8260(0);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            if (work->count > 1) {
                func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            }
            (*seq)++;
        }
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}

// The player and a partner each send out a mon
static BOOL func_ov093_021eca60(BtlvScu *scu, s32 *seq, u8 clientId, u8 partnerId) {
    BtlvScuDoubleWork *work = (BtlvScuDoubleWork *)scu->work;
    u8 partnerIdx;
    BtlvBGaugeParam balls;
    u32 i;
    u8 playerIdx;

    switch (*seq) {
    case 0:
        if (clientId < partnerId) {
            func_ov093_021ee104(scu, &balls, clientId, partnerId, 0);
        } else {
            func_ov093_021ee104(scu, &balls, partnerId, clientId, 0);
        }
        func_ov094_021f823c(&balls);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f7998(0x233);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4() && !func_ov094_021f827c(0)) {
            u16 message;

            work->viewPos[0] = 2;
            work->viewPos[1] = 4;
            work->clientId = GetPlayerClientID(scu->mainModule);
            partnerIdx = func_ov093_021b8fe4(scu->mainModule) ^ 1;
            for (i = 0; i < 2; i++) {
                work->pos[i] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[i]);
                work->mons[i] = func_ov093_021b98ac(scu->pokeCon, work->pos[i]);
                work->monIds[i] = GetMonID(work->mons[i]);
            }
            message = func_ov093_021b9f70(scu->mainModule, partnerId) ? 14 : 17;
            func_ov093_021eee24(scu->strbuf, message, 2, partnerId, work->monIds[partnerIdx]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f8260(0);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade)) {
            playerIdx = func_ov093_021b8fe4(scu->mainModule);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
            func_ov094_021f7998(0x234);
            func_ov093_021eee24(scu->strbuf, 11, 1, work->monIds[playerIdx]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov093_021ecf68(scu)) {
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 6:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// The player sends out three mons
static BOOL func_ov093_021ecc2c(BtlvScu *scu, s32 *seq) {
    BtlvScuTripleWork *work = (BtlvScuTripleWork *)scu->work;
    u32 i;
    BtlvBGaugeParam balls;

    switch (*seq) {
    case 0:
        work->clientId = GetPlayerClientID(scu->mainModule);
        func_ov093_021ee104(scu, &balls, work->clientId, 4, 0);
        func_ov094_021f823c(&balls);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f7998(0x232);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4() && !func_ov094_021f827c(0)) {
            u16 message;

            work->viewPos[0] = 2;
            work->viewPos[1] = 4;
            work->viewPos[2] = 6;
            work->count = 0;
            work->clientId = GetPlayerClientID(scu->mainModule);
            for (i = 0; i < 3; i++) {
                work->pos[i] = func_ov093_021b8ed8(scu->mainModule, work->viewPos[i]);
                work->mons[i] = func_ov093_021b98ac(scu->pokeCon, work->pos[i]);
                work->monIds[i] = GetMonID(work->mons[i]);
                if (!IsFainted(work->mons[i])) {
                    work->count++;
                }
            }
            func_ov094_021f7ee8(func_ov093_021d5864(work->mons[0]), work->viewPos[0]);
            if (work->count > 1) {
                func_ov094_021f7ee8(func_ov093_021d5864(work->mons[1]), work->viewPos[1]);
            }
            if (work->count > 2) {
                func_ov094_021f7ee8(func_ov093_021d5864(work->mons[2]), work->viewPos[2]);
            }
            func_ov094_021f7998(0x234);
            if (work->count == 3) {
                message = 13;
            } else {
                message = work->count == 2 ? 12 : 11;
            }
            func_ov093_021eee24(scu->strbuf, message, 3, work->monIds[0], work->monIds[1], work->monIds[2]);
            func_ov093_021eceb8(scu, scu->strbuf, 80, NULL);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ecf68(scu)) {
            func_ov094_021f8260(0);
            func_ov093_021ed798(&scu->msgFade);
            (*seq)++;
        }
        break;
    case 4:
        if (func_ov093_021ed814(&scu->msgFade) && !func_ov094_021f7eb4()) {
            func_ov093_021eda04(&scu->gauges[work->pos[0]]);
            if (work->count > 1) {
                func_ov093_021eda04(&scu->gauges[work->pos[1]]);
            }
            // The third gauge is tested against 1 too, where func_ov093_021ec5d0 tests 2; func_ov093_021eda04 skips
            // a fainted mon either way
            if (work->count > 1) {
                func_ov093_021eda04(&scu->gauges[work->pos[2]]);
            }
            (*seq)++;
        }
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}

// The message of a send-out by the count of mons and the battle type
u16 func_ov093_021ece08(BtlvScu *scu, u8 count) {
    switch (BtlSetup_GetBattleType(scu->mainModule)) {
    case 0:
    default:
        return count == 1 ? 1 : 2;
    case 1:
    case 2:
        if (count == 1) {
            return 14;
        }
        if (count == 2) {
            return 15;
        }
        return 16;
    case 3:
        if (count == 1) {
            return 17;
        }
        if (count == 2) {
            return 18;
        }
        return 19;
    }
}

// Prints a message at once
void func_ov093_021ece60(BtlvScu *scu, const StrBuf *strbuf) {
    GFL_BitmapFill(scu->msgBitmap, 0xc);
    GFL_TextRndUpdateColorIndexLUT(1, 9, 0xc);
    GFL_TextRendererDrawToBitmap(scu->msgBitmap, 0, 0, strbuf, scu->font);
    BmpWin_FlushChar(scu->msgWin);
    scu->msgSeq = 0;
    scu->waitTimer = 0;
    scu->pageWait = 0;
    scu->endWait = 0;
    scu->printedAtOnce = TRUE;
    scu->msgFinished = FALSE;
}

// Starts streaming a message, which func_ov093_021ecf68 runs
void func_ov093_021eceb8(BtlvScu *scu, const StrBuf *strbuf, u16 wait, BtlvMsgCallback callback) {
    GFL_BitmapFill(scu->msgBitmap, 0xc);
    GFL_TextRndUpdateColorIndexLUT(1, 9, 0xc);
    scu->printStream = func_0201cd10(scu->msgWin, 0, 0, strbuf, scu->font, func_ov093_021b85dc(scu->mainModule),
                                     scu->tcbManager, 0, scu->heapId, 0xc, callback);
    func_0201ce0c(scu->printStream);
    scu->msgSeq = 0;
    scu->printedAtOnce = FALSE;
    if (wait == 0xff) {
        scu->waitTimer = 80;
        scu->endWait = 0;
    } else {
        scu->waitTimer = wait;
        scu->endWait = wait;
    }
    scu->pageWait = scu->waitTimer;
    scu->msgFinished = FALSE;
}

BOOL func_ov093_021ecf50(BtlvScu *scu) {
    if (scu->msgFinished) {
        scu->msgFinished = FALSE;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021ecf68(BtlvScu *scu) {
    switch (scu->msgSeq) {
    case 0:
        func_ov093_021ed7c8(&scu->msgFade, scu->printedAtOnce);
        scu->msgSeq++;
        /* fallthrough */
    case 1:
        if (!func_ov093_021ed814(&scu->msgFade)) {
            break;
        }
        if (scu->printedAtOnce) {
            scu->msgSeq = 6;
            break;
        }
        if (scu->printStream != NULL) {
            func_0201ce20(scu->printStream);
        }
        scu->msgSeq++;
    case 2:
        if (scu->printStream != NULL) {
            if (func_ov093_021b86c0(scu->mainModule) == 0) {
                if ((GCTX_HIDGetHeldKeys() & 3) || func_02035c38()) {
                    if (func_0201ced4(scu->printStream) == 0) {
                        func_0201ce8c(scu->printStream, func_02013080(2));
                    }
                } else if (func_0201ced4(scu->printStream)) {
                    func_0201ceb8(scu->printStream);
                }
            }
            scu->streamState = func_0201ce30(scu->printStream);
            if (scu->streamState != 0) {
                if (func_ov093_021b86c0(scu->mainModule) == 0) {
                    scu->msgSeq = 3;
                } else {
                    scu->msgSeq = 4;
                }
                if (scu->streamState == 2) {
                    scu->waitTimer = scu->endWait;
                    scu->msgFinished = 1;
                }
            }
        } else {
            scu->msgSeq = 6;
        }
        break;
    case 3:
        if ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60()) {
            scu->waitTimer = 0;
        }
        if (scu->waitTimer != 0) {
            scu->waitTimer--;
            break;
        }
        scu->msgSeq = 5;
        break;
    case 4:
        if (scu->waitTimer != 0) {
            scu->waitTimer--;
            break;
        } else {
            scu->msgSeq = 5;
        }
        break;
    case 5:
        if (scu->streamState == 2) {
            func_0201dd68();
            func_0201ce48(scu->printStream);
            scu->printStream = NULL;
            scu->msgSeq = 6;
            break;
        } else {
            func_0201ce38(scu->printStream);
            GFL_SndSEPlay(0x547);
            scu->msgSeq = 2;
            scu->waitTimer = scu->pageWait;
        }
        break;
    case 6:
        if (!GFL_SndIsPlaying(0x547)) {
            scu->msgSeq = 7;
            return TRUE;
        }
        break;
    case 7:
    default:
        return TRUE;
    }
    return FALSE;
}


// Starts the effect of a move
void func_ov093_021ed118(BtlvScu *scu, u8 attackerPos, u8 targetPos, u16 move, s32 arg4, u32 arg5, u8 arg6) {
    BtlvMoveEffectParam *param = (BtlvMoveEffectParam *)scu->work;

    param->move = move;
    param->attackerPos = attackerPos;
    param->targetPos = targetPos;
    param->unk0C = arg5;
    param->unk0D = arg6;
    param->unk10 = arg4;
    BtlvScu_SetSubProc(scu, func_ov093_021ed158);
}

BOOL func_ov093_021ed158(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    switch (*seq) {
    case 0:
        func_ov093_021ed798(&scu->msgFade);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed814(&scu->msgFade)) {
            func_ov094_021f7b2c((const BtlvMoveEffectParam *)scu->work);
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            GFL_SndStop();
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}


// Runs the procedure: its init until it returns TRUE, then its main
BOOL func_ov093_021ed1b0(BtlvScu *scu) {
    return BtlvScu_RunSubProc(scu);
}

void func_ov093_021ed208(BtlvScu *scu, u8 pos, u16 move, BOOL arg3) {
    u32 hp = GetBattleMonStat(func_ov093_021b98ac(scu->pokeCon, pos), BATTLEMON_HP);
    u8 viewPos = func_ov093_021b8e70(scu->mainModule, pos);

    if (!arg3) {
        func_ov094_021f80fc(viewPos, hp);
        func_ov094_021f7c2c(viewPos, move);
    } else {
        func_ov094_021f8118(viewPos, hp);
    }
}

BOOL func_ov093_021ed250(BtlvScu *scu) {
    if ((func_ov094_021f7eb4() | func_ov094_021f8180()) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed268(BtlvScu *scu, u8 pos, u16 move) {
    func_ov094_021f7c2c(func_ov093_021b8e70(scu->mainModule, pos), move);
}

BOOL func_ov093_021ed280(BtlvScu *scu) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed294(BtlvScu *scu, u8 pos, BOOL arg2) {
    u8 viewPos = func_ov093_021b8e70(scu->mainModule, pos);

    func_ov094_021f80e4(viewPos);
    if (!arg2) {
        func_ov094_021f7ce4(viewPos);
    } else {
        func_ov094_021f7f04(viewPos);
    }
}

BOOL func_ov093_021ed2bc(BtlvScu *scu) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

// Shows a mon and its gauge
void func_ov093_021ed2d0(BtlvScu *scu, u8 pos) {
    u8 viewPos = func_ov093_021b8e70(scu->mainModule, pos);

    func_ov094_021f7ee8(func_ov093_021d5864(func_ov093_021b98ac(scu->pokeCon, pos)), viewPos);
    func_ov093_021eda04(&scu->gauges[pos]);
}

BOOL func_ov093_021ed304(BtlvScu *scu) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed318(BtlvScu *scu, u8 pos, u16 effect, BOOL immediate) {
    BtlvScuGaugeHideTask *work;

    if (immediate || effect == 0) {
        func_ov094_021f80e4(pos);
        func_ov094_021f7f04(pos);
    } else {
        work = GFL_TCBExGetData(
            GFL_TCBExMgrAddTask(scu->tcbManager, func_ov093_021ed384, sizeof(BtlvScuGaugeHideTask), 1));
        work->pos = pos;
        work->gauge = &scu->gauges[pos];
        work->count = &scu->gaugeHideTaskCount;
        // Never true here, since an effect of 0 hides the gauge at once
        if (effect == 0) {
            effect = 0x26C;
        }
        work->effect = effect;
        work->seq = 0;
        (*work->count)++;
    }
}

BOOL func_ov093_021ed374(BtlvScu *scu) {
    if (scu->gaugeHideTaskCount == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed384(TCBEx *task, void *data) {
    BtlvScuGaugeHideTask *work = data;

    switch (work->seq) {
    case 0:
        func_ov094_021f7a28(work->pos, work->effect);
        work->seq++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f80e4(work->pos);
            func_ov094_021f7f04(work->pos);
            work->seq++;
        }
        break;
    case 2:
        (*work->count)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}

void func_ov093_021ed3d4(BtlvScu *scu, u8 pos, u8 clientId, u8 monIndex, BOOL fastShow) {
    BtlvScuGaugeShowTask *work;
    BattleMon *mon;
    u8 viewPos;

    work = GFL_TCBExGetData(GFL_TCBExMgrAddTask(scu->tcbManager, func_ov093_021ed460, sizeof(BtlvScuGaugeShowTask), 1));
    work->gauge = &scu->gauges[pos];
    work->count = &scu->gaugeShowTaskCount;
    work->seq = 0;
    (*work->count)++;
    mon = GetClientMonData(scu->pokeCon, clientId, monIndex);
    viewPos = func_ov093_021b8e70(scu->mainModule, pos);
    func_ov094_021f7ee8(func_ov093_021d5864(mon), viewPos);
    if (!fastShow) {
        func_ov094_021f7a28(viewPos, 0x26d);
    }
}



BOOL func_ov093_021ed450(BtlvScu *scu) {
    if (scu->gaugeShowTaskCount == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed460(TCBEx *task, void *data) {
    BtlvScuGaugeShowTask *work = data;

    switch (work->seq) {
    case 0:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021eda04(work->gauge);
            work->seq++;
        }
        break;
    case 1:
        (*work->count)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}

void func_ov093_021ed494(BtlvScu *scu, u8 pos, BOOL arg2) {
    u32 hp = GetBattleMonStat(func_ov093_021b98ac(scu->pokeCon, pos), BATTLEMON_HP);
    u8 viewPos = func_ov093_021b8e70(scu->mainModule, pos);

    if (arg2) {
        func_ov094_021f8118(viewPos, hp);
    } else {
        func_ov094_021f80fc(viewPos, hp);
    }
}

BOOL func_ov093_021ed4d0(BtlvScu *scu) {
    if (!func_ov094_021f8180()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed4e4(BtlvScu *scu, u8 pos1, u8 pos2) {
    func_ov093_021ed9ec(&scu->gauges[pos1]);
    func_ov093_021ed9ec(&scu->gauges[pos2]);
    func_ov094_021f87d4(1);
}

BOOL func_ov093_021ed510(BtlvScu *scu, u8 pos1, u8 pos2) {
    func_ov093_021eda04(&scu->gauges[pos1]);
    func_ov093_021eda04(&scu->gauges[pos2]);
    func_ov094_021f87d4(0);
    return TRUE;
}

void func_ov093_021ed53c(BtlvScu *scu, u8 pos) {
    func_ov093_021ed9ec(&scu->gauges[pos]);
    func_ov094_021f87d4(1);
}

BOOL func_ov093_021ed554(BtlvScu *scu, u8 pos) {
    func_ov093_021eda04(&scu->gauges[pos]);
    func_ov094_021f87d4(0);
    return TRUE;
}

void func_ov093_021ed56c(BtlvScu *scu, u8 viewPos) {
    func_ov094_021f7a28(viewPos, 0x25f);
}


BOOL func_ov093_021ed57c(BtlvScu *scu, u8 viewPos) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed590(BtlvScu *scu, u8 pos, u32 arg2) {
    BtlvScuMonTask *work;

    // BTL_POS_MAX stands for no position here
    if (pos != BTL_POS_MAX) {
        work = GFL_TCBExGetData(GFL_TCBExMgrAddTask(scu->tcbManager, func_ov093_021ed5e8, sizeof(BtlvScuMonTask), 1));
        work->scu = scu;
        work->pos = pos;
        work->arg2 = arg2;
        work->viewPos = func_ov093_021b8e70(scu->mainModule, pos);
        work->count = &scu->taskCount;
        work->seq = 0;
        (*work->count)++;
    }
}

BOOL func_ov093_021ed5d8(BtlvScu *scu) {
    if (scu->taskCount == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed5e8(TCBEx *task, void *data) {
    BtlvScuMonTask *work = data;
    void *src;

    switch (work->seq) {
    case 0:
        src = GetSrcData(func_ov093_021b98ac(work->scu->pokeCon, work->pos));
        if (work->arg2 == 0) {
            func_ov094_021f7d80(src, work->viewPos);
        } else {
            func_ov094_021f7e38(src, work->viewPos);
        }
        func_ov094_021f80e4(work->viewPos);
        work->seq++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f803c(work->scu->mainModule, func_ov093_021b98ac(work->scu->pokeCon, work->pos), work->viewPos);
            (*work->count)--;
            GFL_TCBExRequestEnd(task);
        }
        break;
    }
}


void func_ov093_021ed660(BtlvScu *scu, u32 arg1, u32 index, BOOL flag) {
    BtlvScuPkmTask *work;

    work = GFL_TCBExGetData(GFL_TCBExMgrAddTask(scu->tcbManager, func_ov093_021ed724, sizeof(BtlvScuPkmTask), 1));
    work->pkm = GetSrcData(func_ov093_021b98ac(scu->pokeCon, func_ov093_021b8ed8(scu->mainModule, index)));
    work->scu = scu;
    work->arg1 = arg1;
    work->count = &scu->taskCount;
    work->flag = flag;
    work->seq = 0;
    (*work->count)++;
}

void func_ov093_021ed6bc(BtlvScu *scu, u32 index, BOOL flag) {
    BtlvScuPkmTask *work;

    work = GFL_TCBExGetData(GFL_TCBExMgrAddTask(scu->tcbManager, func_ov093_021ed724, sizeof(BtlvScuPkmTask), 1));
    work->pkm = GetSrcData(func_ov093_021b98ac(scu->pokeCon, func_ov093_021b8ed8(scu->mainModule, index)));
    work->scu = scu;
    work->arg1 = index;
    work->count = &scu->taskCount;
    work->flag = flag;
    work->seq = 0;
    (*work->count)++;
}

BOOL func_ov093_021ed714(BtlvScu *scu) {
    if (scu->taskCount == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed724(TCBEx *task, void *data) {
    BtlvScuPkmTask *work = data;

    switch (work->seq) {
    case 0:
        if (work->flag == 0) {
            func_ov094_021f7d80(work->pkm, work->arg1);
        } else {
            func_ov094_021f7e38(work->pkm, work->arg1);
        }
        work->seq++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            (*work->count)--;
            GFL_TCBExRequestEnd(task);
        }
        break;
    }
}


#define WIN_FADE_PLANES                                                                                                \
    (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ |               \
     GX_BLEND_PLANEMASK_BD)

void func_ov093_021ed76c(BtlvScuWinFade *fade, BmpWin *window) {
    fade->window = window;
    fade->state = 0;
    fade->ev1 = FX32_CONST(31);
    fade->ev2 = FX32_CONST(7);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, WIN_FADE_PLANES, 31, 7);
}

static void func_ov093_021ed798(BtlvScuWinFade *fade) {
    if (fade->state == 0 || fade->state == 3) {
        fade->ev1 = FX32_CONST(31);
        fade->ev2 = FX32_CONST(7);
        fade->ev1Step = -FX32_CONST(31) / 6;
        fade->ev2Step = FX32_CONST(9) / 6;
        fade->frames = 6;
        fade->state = 1;
    }
}

void func_ov093_021ed7c8(BtlvScuWinFade *fade, BOOL keepWindow) {
    if (fade->state == 1 || fade->state == 2) {
        if (!keepWindow) {
            GFL_BitmapFill(BmpWin_GetBitmap(fade->window), 12);
            BmpWin_FlushChar(fade->window);
        }
        fade->ev1 = 0;
        fade->ev2 = FX32_CONST(16);
        fade->ev1Step = FX32_CONST(31) / 6;
        fade->ev2Step = -FX32_CONST(9) / 6;
        fade->frames = 6;
        fade->state = 4;
    }
}

static BOOL func_ov093_021ed814(BtlvScuWinFade *fade) {
    switch (fade->state) {
    case 2:
        return TRUE;
    case 0:
        return TRUE;
    case 4:
        fade->state = 3;
        return FALSE;
    case 3:
        if (fade->frames != 0) {
            fade->ev1 += fade->ev1Step;
            fade->ev2 += fade->ev2Step;
            fade->frames--;
        } else {
            fade->ev1 = FX32_CONST(31);
            fade->ev2 = FX32_CONST(7);
            fade->state = 0;
        }
        break;
    case 1:
        if (fade->frames != 0) {
            fade->ev1 += fade->ev1Step;
            fade->ev2 += fade->ev2Step;
            fade->frames--;
        } else {
            fade->ev1 = 0;
            fade->ev2 = FX32_CONST(16);
            fade->state = 2;
        }
        break;
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, WIN_FADE_PLANES, (u8)FX_Whole(fade->ev1), (u8)FX_Whole(fade->ev2));
    return FALSE;
}

void func_ov093_021ed8ac(BtlvScu *scu) {
    BtlvScuMsgHideTask *work;

    work = GFL_TCBExGetData(GFL_TCBExMgrAddTask(scu->tcbManager, func_ov093_021ed8e8, sizeof(BtlvScuMsgHideTask), 1));
    work->scu = scu;
    work->count = &scu->taskCount;
    work->seq = 0;
    (*work->count)++;
}

BOOL func_ov093_021ed8d8(BtlvScu *scu) {
    if (scu->taskCount == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ed8e8(TCBEx *task, void *data) {
    BtlvScuMsgHideTask *work = data;

    switch (work->seq) {
    case 0:
        func_ov093_021ed798(&work->scu->msgFade);
        work->seq++;
        break;
    case 1:
        if (func_ov093_021ed814(&work->scu->msgFade)) {
            (*work->count)--;
            GFL_TCBExRequestEnd(task);
        }
        break;
    }
}

void func_ov093_021ed930(BtlvScu *scu) {
    u32 i;
    u32 max;

#ifdef BUGFIX
    for (i = 0; i < BTL_POS_MAX; i++) {
#else
    // BUG: clears one gauge too many, writing over the start of abilityWins[0]
    for (i = 0; i <= BTL_POS_MAX; i++) {
#endif
        scu->gauges[i].active = FALSE;
        scu->gauges[i].mon = NULL;
    }
    if (BtlSetup_GetBattleStyle(scu->mainModule) != BTL_STYLE_ROTATION) {
        max = GetValidPosMax(scu->mainModule);
    } else {
        max = 5;
    }
    for (i = 0; i <= max; i++) {
        func_ov093_021ed9a4(&scu->gauges[i], scu, i);
    }
}

static void func_ov093_021ed988(BtlvScu *scu) {
    u32 i;

    for (i = 0; i < BTL_POS_MAX; i++) {
        func_ov093_021ed9c8(&scu->gauges[i]);
    }
}

void func_ov093_021ed9a4(BtlvScuGauge *gauge, BtlvScu *scu, u8 pos) {
    gauge->pos = pos;
    gauge->viewPos = func_ov093_021b8e70(scu->mainModule, pos);
    gauge->scu = scu;
    gauge->shown = FALSE;
    gauge->active = TRUE;
    gauge->mon = NULL;
}

static void func_ov093_021ed9c8(BtlvScuGauge *gauge) {
    if (gauge->active) {
        if (gauge->shown) {
            func_ov094_021f80e4(gauge->viewPos);
            gauge->shown = FALSE;
        }
        gauge->mon = NULL;
        gauge->active = FALSE;
    }
}

static void func_ov093_021ed9ec(BtlvScuGauge *gauge) {
    if (gauge->shown) {
        func_ov094_021f80e4(gauge->viewPos);
        gauge->shown = FALSE;
    }
}

static void func_ov093_021eda04(BtlvScuGauge *gauge) {
    if (gauge->active) {
        gauge->mon = func_ov093_021b98ac(gauge->scu->pokeCon, gauge->pos);
        if (!IsFainted(gauge->mon)) {
            func_ov094_021f803c(gauge->scu->mainModule, gauge->mon, gauge->viewPos);
            gauge->shown = TRUE;
        }
    }
}

static BtlvScuSide func_ov093_021eda3c(BtlMainModule *mainModule, u8 pos) {
    if (func_ov093_021b8bd0(mainModule, func_ov093_021b9ae0(pos))) {
        return BTLV_SCU_SIDE_PLAYER;
    }
    return BTLV_SCU_SIDE_ENEMY;
}

void func_ov093_021eda5c(BtlvScu *scu, u8 pos, BOOL flash) {
    BtlvScuSide side = func_ov093_021eda3c(scu->mainModule, pos);

    if (side == BTLV_SCU_SIDE_PLAYER) {
        func_ov094_021f8504(0, 0, 0);
    }
    func_ov094_021f8b28();
    func_ov093_021edc28(&scu->abilityWins[side], pos, flash);
}

BOOL func_ov093_021eda94(BtlvScu *scu, u8 pos) {
    return func_ov093_021edca8(&scu->abilityWins[func_ov093_021eda3c(scu->mainModule, pos)]);
}

void func_ov093_021edab0(BtlvScu *scu, u8 pos) {
    u8 side = func_ov093_021eda3c(scu->mainModule, pos);

    func_ov093_021edf4c(&scu->abilityWins[side]);
    if (side == BTLV_SCU_SIDE_PLAYER) {
        func_ov094_021f8504(0, 1, 0);
    }
}

BOOL func_ov093_021edae0(BtlvScu *scu, u8 pos) {
    u8 side = func_ov093_021eda3c(scu->mainModule, pos);

    return func_ov093_021edf78(&scu->abilityWins[side]);
}

void func_ov093_021edb00(BtlvScu *scu, u8 pos) {
    func_ov093_021edf7c(&scu->abilityWins[func_ov093_021eda3c(scu->mainModule, pos)], pos);
}

BOOL func_ov093_021edb20(BtlvScu *scu, u8 pos) {
    return func_ov093_021edfc4(&scu->abilityWins[func_ov093_021eda3c(scu->mainModule, pos)]);
}

void func_ov093_021edb3c(BtlvScu *scu, u32 charOffset) {
    u32 i;

    scu->abilityCharFile = GFL_G2DIOReadBGNCGR(11, 0xD5, FALSE, &scu->abilityChars, scu->heapId);
    for (i = 0; i < 2; i++) {
        scu->abilityWins[i].scu = scu;
        scu->abilityWins[i].side = i;
        scu->abilityWins[i].bg = i == BTLV_SCU_SIDE_PLAYER ? 2 : 3;
        scu->abilityWins[i].chars = (u8 *)scu->abilityChars->pRawData + i * 0x900;
        scu->abilityWins[i].bitmap = GFL_BitmapCreate(18, 4, 0x20, scu->heapId);
        scu->abilityWins[i].ability = 0;
        scu->abilityWins[i].monId = 0x1F;
        scu->abilityWins[i].charOffset = charOffset + i * 0x48;
        scu->abilityWins[i].shown = FALSE;
    }
}

static void func_ov093_021edbe8(BtlvScu *scu) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (scu->abilityWins[i].bitmap != NULL) {
            GFL_BitmapFree(scu->abilityWins[i].bitmap);
            scu->abilityWins[i].bitmap = NULL;
        }
    }
    if (scu->abilityCharFile != NULL) {
        GFL_HeapFree(scu->abilityCharFile);
        scu->abilityCharFile = NULL;
    }
}

static void func_ov093_021edc28(BtlvScuAbilityWin *win, u8 pos, BOOL flash) {
    BattleMon *mon = func_ov093_021b98ac(win->scu->pokeCon, pos);
    u16 ability = GetBattleMonStat(mon, BATTLEMON_ABILITY);
    u8 monId = GetMonID(mon);

    if (win->shown) {
        if (monId == win->monId) {
            return;
        }
        func_ov093_021edf4c(win);
    }
    if (ability != win->ability || monId != win->monId) {
        win->ability = ability;
        win->monId = monId;
        func_ov093_021ede54(win);
    }
    win->flash = flash;
    win->seq = 0;
    func_ov094_021f8ae8(pos, ability);
}

BOOL func_ov093_021edca8(BtlvScuAbilityWin *win) {
    PaletteFade *fade;
    TCBManager *tcbManager;
    u16 mask;
    u8 *src;
    u32 charPos;
    u32 i;

    switch (win->seq) {
    case 0:
        GFL_BGSysLoadChar(win->bg, win->chars, 0x900, win->charOffset);
        win->seq++;
        break;
    case 1:
        win->timer = 8;
        if (win->side == 0) {
            win->scrollX = 0x90000;
            win->scrollStep = 0xfffee000;
        } else {
            win->scrollX = 0xfff70000;
            win->scrollStep = 0x12000;
        }
        GFL_BGSysMoveBG(win->bg, BG_MOVE_SET_X, win->scrollX >> 12);
        GFL_BGSysMoveBG(win->bg, BG_MOVE_SET_Y, 0);
        GFL_SndSEPlay(0x573);
        win->seq++;
        break;
    case 2:
        if (win->timer != 0) {
            win->scrollX += win->scrollStep;
            GFL_BGSysMoveBG(win->bg, BG_MOVE_SET_X, win->scrollX >> 12);
            win->timer--;
        } else {
            GFL_BGSysMoveBG(win->bg, BG_MOVE_SET_X, 0);
            win->count = 0;
            win->seq++;
        }
        break;
    case 3:
        if (win->count < 0x11) {
            src = GFL_BitmapGetPixelData(win->bitmap);
            charPos = win->charOffset + win->count;
            src += win->count * 0x20;
            for (i = 0; i < 4; i++) {
                GFL_BGSysLoadChar(win->bg, src + i * 0x240, 0x20, charPos + i * 0x12);
            }
            win->count++;
        } else {
            win->shown = 1;
            if (!win->flash) {
                return TRUE;
            }
            win->seq++;
        }
        break;
    case 4:
        fade = func_ov094_021f86e0();
        tcbManager = func_ov094_021f86d4();
        mask = 1 << (win->side + 1);
        GFL_SndSEPlay(0x54d);
        func_0202173c(fade, 1, mask, 0, 0x10, 0, 0x7fff, tcbManager);
        win->seq++;
        break;
    case 5:
        if (func_02021ed8(func_ov094_021f86e0()) == 0) {
            win->seq++;
        }
        break;
    case 6:
        if (GFL_SndIsPlaying(0x54d)) {
            break;
        }
        if (GFL_SndIsPlaying(0x573)) {
            break;
        }
        return TRUE;
    }
    return FALSE;
}


static void func_ov093_021ede54(BtlvScuAbilityWin *win) {
    BtlvScu *scu = win->scu;
    u32 size;
    u32 widths[2];
    int end;
    int x1;
    int x2;
    int overlap;
    int shift;
    int half;

    size = GFL_BitmapCalcPixelDataSize(win->bitmap);
    MIi_CpuCopy32((u8 *)win->scu->abilityChars->pRawData + win->side * 0x900, GFL_BitmapGetPixelData(win->bitmap),
                 size);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    func_ov093_021eee24(scu->abilityStrbuf, 0x67, 2, win->monId, win->ability);
    if ((u8)func_0201d33c(scu->abilityStrbuf, scu->smallFont, 0, widths, 2) > 1) {
        // Two lines, the first from the left and the second to the right, at least 12 pixels apart
        end = widths[0] + 8;
        x2 = 0x84 - widths[1];
        overlap = end - x2;
        if (overlap < 12) {
            shift = 12 - overlap;
            half = shift / 2;
            end += half;
            x2 -= shift - half;
        }
        x1 = end - widths[0];
        if (x1 < 0) {
            x1 = 0;
        }
        if (x2 < 0) {
            x2 = 0;
        }
        func_0201d37c(scu->abilityStrbuf, scu->lineStrbuf, 0);
        GFL_TextRendererDrawToBitmap(win->bitmap, x1, 7, scu->lineStrbuf, scu->smallFont);
        func_0201d37c(scu->abilityStrbuf, scu->lineStrbuf, 1);
        GFL_TextRendererDrawToBitmap(win->bitmap, x2, 0x12, scu->lineStrbuf, scu->smallFont);
    } else {
        x1 = (0x88 - GFL_FontGetBlockWidth(scu->abilityStrbuf, scu->smallFont, 0)) / 2;
        if (x1 < 0) {
            x1 = 0;
        }
        GFL_TextRendererDrawToBitmap(win->bitmap, x1, 0x12, scu->abilityStrbuf, scu->smallFont);
    }
}

static void func_ov093_021edf4c(BtlvScuAbilityWin *win) {
    if (win->side == BTLV_SCU_SIDE_PLAYER) {
        GFL_BGSysMoveBG(win->bg, BG_MOVE_SET_X, 0x90);
    } else {
        GFL_BGSysMoveBG(win->bg, BG_MOVE_SET_X, -0x90);
    }
    win->shown = FALSE;
}

static BOOL func_ov093_021edf78(BtlvScuAbilityWin *win) {
    return TRUE;
}

static void func_ov093_021edf7c(BtlvScuAbilityWin *win, u8 pos) {
    BattleMon *mon = func_ov093_021b98ac(win->scu->pokeCon, pos);
    u16 ability = GetBattleMonStat(mon, BATTLEMON_ABILITY);
    u8 monId = GetMonID(mon);

    if (ability != win->ability || monId != win->monId) {
        win->monId = monId;
        win->ability = ability;
        func_ov093_021ede54(win);
    }
    win->seq = 0;
    func_ov094_021f8ae8(pos, ability);
}

static BOOL func_ov093_021edfc4(BtlvScuAbilityWin *win) {
    switch (win->seq) {
    case 0:
        G2_SetBGMosaicSize(0, 0);
        if (win->side == BTLV_SCU_SIDE_PLAYER) {
            G2_BG2Mosaic(TRUE);
        } else {
            G2_BG3Mosaic(TRUE);
        }
        win->timer = 0;
        win->count = 0;
        GFL_SndSEPlay(SEQ_SE_DECIDE3);
        win->seq++;
        break;
    case 1:
        win->timer++;
        if (win->timer > 1) {
            win->timer = 0;
            win->count++;
            G2_SetBGMosaicSize(win->count, win->count);
            if (win->count >= 15) {
                win->seq++;
            }
        }
        break;
    case 2:
        GFL_BGSysLoadChar(win->bg, GFL_BitmapGetPixelData(win->bitmap), 0x900, win->charOffset);
        win->seq++;
        break;
    case 3:
        win->timer++;
        if (win->timer > 1) {
            win->timer = 0;
            win->count--;
            G2_SetBGMosaicSize(win->count, win->count);
            if (win->count == 0) {
                win->seq++;
            }
        }
        break;
    case 4:
        if (win->side == BTLV_SCU_SIDE_PLAYER) {
            G2_BG2Mosaic(FALSE);
        } else {
            G2_BG3Mosaic(FALSE);
        }
        win->seq++;
        break;
    case 5:
        if (!GFL_SndIsPlaying(SEQ_SE_DECIDE3)) {
            win->seq++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ee104(BtlvScu *scu, BtlvBGaugeParam *param, u8 clientId, u8 otherClientId, u32 side) {
    BattleParty *parties[2] = { NULL, NULL };
    int counts[2] = { 0, 0 };
    BattleMon *mon;
    int i;
    int which;
    s8 member;

    MI_CpuFill8(param, 0, sizeof(BtlvBGaugeParam));
    parties[0] = func_ov093_021b9af8(scu->pokeCon, clientId);
    counts[0] = GetNumMonsInParty(parties[0]);
    if (otherClientId < 4) {
        parties[1] = func_ov093_021b9af8(scu->pokeCon, otherClientId);
        counts[1] = GetNumMonsInParty(parties[1]);
        if (counts[0] > 3) {
            counts[0] = 3;
        }
        if (counts[1] > 3) {
            counts[1] = 3;
        }
    }
    param->side = side;
    for (i = 0; i < 6; i++) {
        if (i < counts[0] + counts[1]) {
            if (i < counts[0]) {
                which = 0;
                member = i;
            } else {
                which = 1;
                member = i - counts[0];
            }
            mon = GetBattleMonFromParty(parties[which], member);
            if (IsFainted(mon)) {
                param->balls[which * 3 + member] = 2;
            } else if (GetBattleMonStatus(mon)) {
                param->balls[which * 3 + member] = 3;
            } else if (!CanPokemonBattle(mon)) {
                param->balls[which * 3 + member] = 0;
            } else {
                param->balls[which * 3 + member] = 1;
            }
        }
    }
}


void func_ov093_021ee200(BtlvScu *scu) {
    func_ov093_021ef694(scu->strbuf, 0xF);
    func_ov093_021eceb8(scu, scu->strbuf, 0, NULL);
}

BOOL func_ov093_021ee21c(BtlvScu *scu) {
    return func_ov093_021ecf68(scu);
}

void func_ov093_021ee224(BtlvScu *scu) {
}

void func_ov093_021ee228(BtlvScu *scu) {
    GFL_FadeSet(FADE_ENGINE_A_BLACK, 0, 16, -3);
}

void func_ov093_021ee23c(BtlvScu *scu) {
    GFL_BitmapFill(scu->msgBitmap, 12);
    BmpWin_FlushChar(scu->msgWin);
    GFL_FadeSet(FADE_ENGINE_A_BLACK, 16, 0, -3);
}

BOOL func_ov093_021ee25c(BtlvScu *scu) {
    return func_ov093_021eb318(scu);
}

void func_ov093_021ee264(BtlvScu *scu, BattleMon *mon, const BattleMonLevelUp *levelUp) {
    scu->levelUpMon = mon;
    scu->levelUp = *levelUp;
    BtlvScu_SetSubProc(scu, func_ov093_021ee3e8);
}

BOOL func_ov093_021ee2a0(BtlvScu *scu) {
    return BtlvScu_RunSubProc(scu);
}

void func_ov093_021ee2f8(BtlvScu *scu) {
    BtlvScu_SetSubProc(scu, func_ov093_021ee4b4);
}

BOOL func_ov093_021ee318(BtlvScu *scu) {
    return BtlvScu_RunSubProc(scu);
}

void func_ov093_021ee370(BtlvScu *scu) {
    BtlvScu_SetSubProc(scu, func_ov093_021ee544);
}

BOOL func_ov093_021ee390(BtlvScu *scu) {
    return BtlvScu_RunSubProc(scu);
}

// Opens the stat window with the gains of the level-up
static BOOL func_ov093_021ee3e8(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    switch (*seq) {
    case 0:
        GFL_TextRndUpdateColorIndexLUT(1, 9, 12);
        BmpWin_FlushMap(scu->statWin);
        func_020454e0(scu->statWin, scu->statFrameCharOffset, 3);
        GFL_BitmapFill(scu->statBitmap, 0xC);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(scu->statWin));
        (*seq)++;
        break;
    case 1:
        func_ov093_021ef71c(scu->strbuf, scu->levelUp.hp, scu->levelUp.attack, scu->levelUp.defense,
                            scu->levelUp.spAttack, scu->levelUp.spDefense, scu->levelUp.speed);
        GFL_TextRendererDrawToBitmap(scu->statBitmap, 0, 0, scu->strbuf, scu->font);
        (*seq)++;
        break;
    case 2:
        BmpWin_FlushChar(scu->statWin);
        (*seq)++;
        break;
    case 3:
        GFL_BGSysMoveBG(3, BG_MOVE_SET_X, 0);
        GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, 0x100);
        GFL_SndSEPlay(SEQ_SE_MESSAGE);
        (*seq)++;
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

// Shows the new stats in the stat window
static BOOL func_ov093_021ee4b4(s32 *seq, void *arg) {
    BtlvScu *scu = arg;

    switch (*seq) {
    case 0:
        func_ov093_021d590c(scu->levelUpMon, &scu->levelUp);
        func_ov093_021ef7ac(scu->strbuf, scu->levelUp.hp, scu->levelUp.attack, scu->levelUp.defense,
                            scu->levelUp.spAttack, scu->levelUp.spDefense, scu->levelUp.speed);
        GFL_BitmapFill(scu->statBitmap, 0xC);
        GFL_TextRendererDrawToBitmap(scu->statBitmap, 0, 0, scu->strbuf, scu->font);
        (*seq)++;
        break;
    case 1:
        BmpWin_FlushChar(scu->statWin);
        GFL_SndSEPlay(SEQ_SE_MESSAGE);
        (*seq)++;
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

// Closes the stat window
static BOOL func_ov093_021ee544(s32 *seq, void *arg) {
    BtlvScu *scu = arg;
    int x;
    int y;
    int width;
    int height;
    u8 bg;

    switch (*seq) {
    case 0:
        x = BmpWin_GetPosX(scu->statWin) - 1;
        y = BmpWin_GetPosY(scu->statWin) - 1;
        width = func_02045738(scu->statWin) + 2;
        height = func_02045740(scu->statWin) + 2;
        bg = BmpWin_GetBGIndex(scu->statWin);
        GFL_BGSysFillScrArea(bg, 0, x, y, width, height, 0);
        GFL_BGSysLoadScr(bg);
        GFL_SndSEPlay(SEQ_SE_MESSAGE);
        (*seq)++;
        break;
    case 1:
        GFL_BGSysMoveBG(3, BG_MOVE_SET_X, -0x90);
        GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, 0);
        (*seq)++;
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}
