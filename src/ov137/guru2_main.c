// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov216/guru2_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/guru2.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/union_app.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/calctool.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/math_util.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "pml/met_data.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/aeabi.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/net_save.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/time_icon.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/ability_handlers.h"
#include "work_area.h"

// guru2_main.c (the ROM's name, from its allocations): the Spin Trade's screen with the disc. The Eggs drop onto the
// disc, which the parent spins while every player's touches of the button make the Eggs jump; when the time is up
// each player takes the Egg in front of them, and those whose Egg stopped on a bonus area get a Berry. Names are ours

// The Spin Trade's archives, which constants/arc.h doesn't have yet: its models and BGs, and the countdown's actor
#define ARCID_GURU2 183
#define ARCID_GURU2_OBJ 184

// The class of the timing syncs
#define GURU2_SYNC_CLASS 0x28
enum {
    GURU2_SYNC_START = 0xca,
    GURU2_SYNC_COUNTDOWN,
    GURU2_SYNC_RESULT,
    GURU2_SYNC_SAVE,
    GURU2_SYNC_END,
};

// The bits of GURU2_NET_CMD_5's signals
#define GURU2_MAIN_SIGNAL_JOIN 1
#define GURU2_MAIN_SIGNAL_COUNTDOWN 4
#define GURU2_MAIN_SIGNAL_START 8

#define FULL_TURN FX32_CONST(360)
// How far from the disc's center the Eggs are
#define EGG_RADIUS 22
// The game's length in frames, and when the disc starts to slow down
#define GAME_FRAMES 600
#define GAME_SLOW_FRAMES 300

#define COLOR_NAME PRINT_COLOR(1, 2, 0)
#define COLOR_NAME_MINE PRINT_COLOR(3, 4, 0)

// The archive's files
enum {
    NARC_GURU2_EGG_NSBMD = 0,
    NARC_GURU2_DISC_2_NSBMD,
    NARC_GURU2_DISC_3_NSBMD,
    NARC_GURU2_DISC_4_NSBMD,
    NARC_GURU2_DISC_5_NSBMD,
    NARC_GURU2_SHADOW_NSBMD,
    NARC_GURU2_CURSOR_0_NSBMD,
    NARC_GURU2_CURSOR_1_NSBMD,
    NARC_GURU2_CURSOR_2_NSBMD,
    NARC_GURU2_CURSOR_3_NSBMD,
    NARC_GURU2_CURSOR_4_NSBMD,
    NARC_GURU2_CURSOR_0_ANM,
    NARC_GURU2_CURSOR_1_ANM,
    NARC_GURU2_CURSOR_2_ANM,
    NARC_GURU2_CURSOR_3_ANM,
    NARC_GURU2_CURSOR_4_ANM,
    NARC_GURU2_BG_NCGR,
    NARC_GURU2_BG_NCLR,
    NARC_GURU2_BG3_NSCR,
    NARC_GURU2_BG2_NSCR,
    NARC_GURU2_SUB_NCLR,
    NARC_GURU2_SUB_BG3_NCGR,
    NARC_GURU2_SUB_BG2_NCGR,
    NARC_GURU2_SUB_BG3_NSCR,
    NARC_GURU2_BUTTON_NSCR,
    NARC_GURU2_BUTTON_PUSHED_NSCR,
    NARC_GURU2_BUTTON_HALF_NSCR,
    NARC_GURU2_BG_NSBMD,
};

enum {
    SEQ_INIT,
    SEQ_FADE_IN_WAIT,
    SEQ_PARENT_WAIT_CHILDREN,
    SEQ_PARENT_SEND_JOIN_SIGNAL,
    SEQ_PARENT_WAIT_JOIN,
    SEQ_PARENT_SEND_PLAYER_COUNT,
    SEQ_PARENT_SEND_POSITIONS,
    SEQ_PARENT_SEND_START_SIGNAL,
    SEQ_CHILD_SEND_JOIN,
    SEQ_CHILD_WAIT_START_SIGNAL,
    SEQ_SYNC_START,
    SEQ_SYNC_START_WAIT,
    SEQ_SEND_PARTY_INDEX,
    SEQ_SEND_PARTY,
    SEQ_WAIT_PARTIES,
    SEQ_WAIT_EGG_CHECK,
    SEQ_GAME_INIT,
    SEQ_EGG_FALL,
    SEQ_EGG_FALL_WAIT,
    SEQ_COUNTDOWN_BEFORE,
    SEQ_PARENT_SEND_COUNTDOWN_SIGNAL,
    SEQ_CHILD_WAIT_COUNTDOWN_SIGNAL,
    SEQ_SYNC_COUNTDOWN,
    SEQ_SYNC_COUNTDOWN_WAIT,
    SEQ_COUNTDOWN_START,
    SEQ_COUNTDOWN_WAIT,
    SEQ_GAME_START,
    SEQ_GAME,
    SEQ_GAME_END,
    SEQ_PARENT_ALIGN_OWN,
    SEQ_PARENT_ALIGN,
    SEQ_SYNC_RESULT,
    SEQ_SYNC_RESULT_WAIT,
    SEQ_PARENT_SEND_RESULT,
    SEQ_CHILD_WAIT_RESULT,
    SEQ_RESULT_NAMES,
    SEQ_RESULT_NAMES_WAIT,
    SEQ_RESULT_MESSAGE_WAIT,
    SEQ_BONUS,
    SEQ_BONUS_WAIT,
    SEQ_BAG_FULL_BEFORE,
    SEQ_BAG_FULL_WAIT,
    SEQ_SYNC_SAVE,
    SEQ_SYNC_SAVE_WAIT,
    SEQ_SAVE,
    SEQ_BAD_EGG,
    SEQ_BAD_EGG_WAIT,
    SEQ_SYNC_END,
    SEQ_SYNC_END_WAIT,
    SEQ_END_WAIT,
    SEQ_FADE_OUT,
    SEQ_FADE_OUT_WAIT,
    SEQ_END,
};

// What a sequence function returns: to wait a frame, to run the next one at once, or that the screen has ended
enum {
    SEQ_RESULT_WAIT,
    SEQ_RESULT_CONTINUE,
    SEQ_RESULT_END,
};

// GURU2_NET_CMD_8: what the parent sends each frame of the game
typedef struct {
    fx32 discAngle;
    // How high the Eggs have jumped
    fx32 jumpY;
    // In degrees
    u16 eggAngles[GURU2_MEMBER_MAX];
    u16 timeUp;
} Guru2GameData;

// GURU2_NET_CMD_12: where everything stopped, in degrees
typedef struct {
    u16 discAngle;
    // The net IDs whose Egg stopped on a bonus area
    u16 bonusBits;
    // By position
    u16 eggAngles[6];
} Guru2ResultData;

// GURU2_NET_CMD_9: a member's position around the disc
typedef struct {
    u16 position;
    u16 netId;
} Guru2PositionData;

typedef struct {
    BOOL active;
    BOOL visible;
    BOOL landed;
    int netId;
    int position;
    // The order in which it dropped, which is the name window it has
    int index;
    int startPosition;
    fx32 angle;
    fx32 angleOffset;
    u16 rotX;
    u16 rotY;
    u16 rotZ;
    VecFx32 rotation;
    VecFx32 scale;
    VecFx32 pos;
    VecFx32 offset;
    fx32 jumpY;
    u8 unk60[0x54];
} Guru2Egg;

typedef struct {
    BOOL active;
    u16 rotX;
    u16 rotY;
    u16 rotZ;
    VecFx32 scale;
    VecFx32 pos;
    u8 unk24[0x54];
    Guru2Egg *egg;
} Guru2EggShadow;

// The arrow over a player's Egg
typedef struct {
    BOOL active;
    VecFx32 pos;
    u8 unk10[0x54];
    Guru2Egg *egg;
} Guru2EggCursor;

typedef struct {
    u8 unk0[0x140];
    Guru2Egg eggs[GURU2_MEMBER_MAX];
    Guru2EggShadow shadows[GURU2_MEMBER_MAX];
    Guru2EggCursor cursors[GURU2_MEMBER_MAX];
    u32 eggScene;
    u32 shadowScene;
    u32 cursorScenes[GURU2_MEMBER_MAX];
} Guru2Objects;

typedef struct {
    VecFx32 pos;
    VecFx32 drawPos;
    VecFx32 scale;
    // The shake after the button is pressed, and the push down before it
    VecFx32 shake;
    VecFx32 offset;
    fx32 angle;
    // What turns the player's own position to the front
    fx32 baseAngle;
    fx32 angleOffset;
    fx32 speed;
    u16 rotX;
    s16 rotY;
    u16 rotZ;
    u8 unk52[0x6a];
    u32 scene;
} Guru2Disc;

// The size of Guru2MainWork's seqWork, the running sequence's (func_ov137_021e16ac): the Egg drop's, which holds no
// pointers, so it fits on a PC target too
#define GURU2_SEQ_WORK_SIZE 0x20

// The work of the sequence that drops the Eggs
typedef struct {
    int count;
    int angle;
    u8 unk8[8];
    int position;
    int playerCount;
} Guru2EggFallSeqWork;
WORK_AREA_CHECK(Guru2EggFallSeqWork, GURU2_SEQ_WORK_SIZE);

typedef struct {
    int seq;
    u32 unk4;
    BOOL sePlayed;
    fx32 speed;
    Guru2Egg *egg;
    Guru2MainWork *g2m;
} Guru2EggFallTask;

typedef struct {
    BOOL active;
    int seq;
    int frame;
    int count;
    fx32 speed;
    fx32 accel;
    u32 unk18;
    Guru2Egg *egg;
} Guru2EggJumpTask;

typedef struct {
    BOOL active;
    int seq;
    u32 unk8;
    fx32 speed;
    Guru2Egg *egg;
} Guru2EggSpinTask;

typedef struct {
    BOOL active;
    int seq;
    fx32 amplitude;
    fx32 speed;
    fx32 angle;
    Guru2Egg *egg;
} Guru2EggShakeTask;

// The disc's bump when the button is pressed
typedef struct {
    int seq;
    // Whether the Eggs are in the air
    BOOL jumping;
    BOOL active;
    int count;
    fx32 shake;
} Guru2DiscBumpTask;

typedef struct {
    u16 seq;
    u16 active;
    int frame;
} Guru2ButtonTask;

// An Egg's hops on a bonus area
typedef struct {
    int seq;
    int frame;
    int wait;
    Guru2Egg *egg;
} Guru2BonusTask;

typedef struct {
    int seq;
    int wait;
    u32 unk8;
    int count;
    Guru2MainWork *g2m;
    ClActor *actor;
} Guru2CountdownTask;

typedef struct {
    Guru2MainWork *g2m;
    int out;
    int evy;
} Guru2FadeTask;

typedef struct {
    int nameWindowCount;
    Font *font;
    MsgData *msgData;
    WordSet *wordSet;
    BmpWin *msgWindow;
    BmpWin *nameWindows[GURU2_MEMBER_MAX];
    PrintWindow msgPrint;
    PrintWindow namePrints[GURU2_MEMBER_MAX];
    PrintQueue *queue;
    StrBuf *strbuf;
} Guru2MsgWork;

typedef struct {
    u8 unk0[0x14];
    G3DCamera *camera;
} Guru2CameraWork;

struct Guru2MainWork {
    int seq;
    u32 unk04;
    int gameFrame;
    int wait;
    u32 unk10;
    int sendCount;
    BOOL error;
    // The net IDs whose Egg stopped on a bonus area
    u32 bonusBits;
    BOOL timeUp;
    PokeParty *party;
    // The Egg that stopped in front of the player
    Guru2Egg *frontEgg;
    int position;
    int playerCount;
    // A press of the button that arrived this frame, and this machine's to send
    u32 buttonReceived;
    u32 buttonSend;
    BOOL gameDataReceived;
    BOOL resultReceived;
    // The party slot each member trades, by net ID
    u32 partyIndices[GURU2_MEMBER_MAX];
    Guru2GameData gameData;
    Guru2ResultData result;
    Guru2PositionData positions[GURU2_MEMBER_MAX];
    UnionAppMember *members[GURU2_MEMBER_MAX];
    StrBuf *names[GURU2_MEMBER_MAX];
    u8 seqWork[GURU2_SEQ_WORK_SIZE];
    Guru2ProcWork *g2p;
    Guru2CommWork *g2c;
    Guru2Disc disc;
    Guru2Objects objects;
    G3DManager *g3dManager;
    Guru2EggJumpTask jumpTasks[GURU2_MEMBER_MAX];
    Guru2EggSpinTask spinTasks[GURU2_MEMBER_MAX];
    Guru2EggShakeTask shakeTasks[GURU2_MEMBER_MAX];
    Guru2DiscBumpTask bumpTask;
    Guru2ButtonTask buttonTask;
    Guru2BonusTask bonusTasks[GURU2_MEMBER_MAX];
    Guru2MsgWork msg;
    Guru2CameraWork camera;
    ArcTool *arc;
    NNSG2dScreenData *screenData;
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *paletteData;
    ClActUnit *clactUnit;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    PaletteFade *fade;
    TCB *jumpTCB;
    TCB *spinTCB;
    TCB *shakeTCB;
    TCB *bumpTCB;
    TCB *buttonTCB;
    TCB *bonusTCB;
    void *tcbBuffer;
    TCBManager *tcbManager;
    TCB *vblankTCB;
    NetSave *netSave;
    u32 bgScene;
    WaitIcon *waitIcon;
};

typedef int (*Guru2MainSeqFunc)(Guru2MainWork *g2m);
typedef int (*Guru2EggFallFunc)(Guru2EggFallTask *task);

// A name window, in the layout of Gen 4's window templates
typedef struct {
    u32 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u16 palette;
    u16 charOffset;
} Guru2NameWindowData;

// A bonus area of the disc, in degrees
typedef struct {
    u16 start;
    u16 end;
} Guru2BonusArea;

typedef struct {
    int count;
    const Guru2BonusArea *areas;
} Guru2BonusAreaList;

static void func_ov137_021dea68(Guru2MainWork *g2m);
static int func_ov137_021dea70(Guru2MainWork *g2m);
static int func_ov137_021dea84(Guru2MainWork *g2m);
static int func_ov137_021deac8(Guru2MainWork *g2m);
static int func_ov137_021deae0(Guru2MainWork *g2m);
static UnionApp *func_ov137_021deb10(Guru2MainWork *g2m);
static int func_ov137_021deb1c(Guru2MainWork *g2m);
static int func_ov137_021deb40(Guru2MainWork *g2m);
static int func_ov137_021deb74(Guru2MainWork *g2m);
static int func_ov137_021dec08(Guru2MainWork *g2m);
static int func_ov137_021dec38(Guru2MainWork *g2m);
static int func_ov137_021dec64(Guru2MainWork *g2m);
static int func_ov137_021dec84(Guru2MainWork *g2m);
static int func_ov137_021decb0(Guru2MainWork *g2m);
static int func_ov137_021deccc(Guru2MainWork *g2m);
static int func_ov137_021decf4(Guru2MainWork *g2m);
static int func_ov137_021ded3c(Guru2MainWork *g2m);
static int func_ov137_021ded88(Guru2MainWork *g2m);
static int func_ov137_021dedac(Guru2MainWork *g2m);
static int func_ov137_021dee4c(Guru2MainWork *g2m);
static int func_ov137_021deed8(Guru2MainWork *g2m);
static int func_ov137_021def14(Guru2MainWork *g2m);
static int func_ov137_021def3c(Guru2MainWork *g2m);
static void func_ov137_021def6c(Guru2MainWork *g2m);
static int func_ov137_021defb4(Guru2MainWork *g2m);
static int func_ov137_021defd0(Guru2MainWork *g2m);
static int func_ov137_021defe8(Guru2MainWork *g2m);
static int func_ov137_021df004(Guru2MainWork *g2m);
static int func_ov137_021df01c(Guru2MainWork *g2m);
static int func_ov137_021df050(Guru2MainWork *g2m);
static int func_ov137_021df060(Guru2MainWork *g2m);
static int func_ov137_021df1c4(Guru2MainWork *g2m);
static int func_ov137_021df204(Guru2MainWork *g2m);
static int func_ov137_021df22c(Guru2MainWork *g2m);
static int func_ov137_021df2a0(Guru2MainWork *g2m);
static int func_ov137_021df2b8(Guru2MainWork *g2m);
static int func_ov137_021df2e0(Guru2MainWork *g2m);
static int func_ov137_021df370(Guru2MainWork *g2m);
static int func_ov137_021df3e8(Guru2MainWork *g2m);
static int func_ov137_021df434(Guru2MainWork *g2m);
static int func_ov137_021df488(Guru2MainWork *g2m);
static int func_ov137_021df4ac(Guru2MainWork *g2m);
static int func_ov137_021df548(Guru2MainWork *g2m);
static int func_ov137_021df56c(Guru2MainWork *g2m);
static int func_ov137_021df598(Guru2MainWork *g2m);
static int func_ov137_021df5b0(Guru2MainWork *g2m);
static int func_ov137_021df5ec(Guru2MainWork *g2m);
static int func_ov137_021df648(Guru2MainWork *g2m);
static int func_ov137_021df6a4(Guru2MainWork *g2m);
static int func_ov137_021df6b8(Guru2MainWork *g2m);
static int func_ov137_021df6d4(Guru2MainWork *g2m);
static int func_ov137_021df6fc(Guru2MainWork *g2m);
static int func_ov137_021df778(Guru2MainWork *g2m);
static int func_ov137_021df7a4(Guru2MainWork *g2m);
static int func_ov137_021df7cc(Guru2MainWork *g2m);
static int func_ov137_021df7e4(Guru2MainWork *g2m);
static void func_ov137_021df7e8(TCB *tcb, void *data);
static void func_ov137_021df800(Guru2MainWork *g2m);
static void func_ov137_021df83c(Guru2MainWork *g2m);
static void func_ov137_021df864(Guru2MainWork *g2m);
static void func_ov137_021df984(Guru2MainWork *g2m);
static void func_ov137_021df998(Guru2MainWork *g2m);
static void func_ov137_021dfa14(Guru2MainWork *g2m);
static void func_ov137_021dfa68(void);
static void func_ov137_021dfad8(Guru2MainWork *g2m);
static void func_ov137_021dfb60(Guru2MainWork *g2m);
static void func_ov137_021dfc74(Guru2MainWork *g2m);
static void func_ov137_021dfe40(Guru2MainWork *g2m);
static void func_ov137_021dfe6c(Guru2MainWork *g2m, u32 fileId);
static void func_ov137_021dfea0(Guru2MainWork *g2m);
void func_ov137_021dfeac(Guru2MainWork *g2m);
static void func_ov137_021dfeb8(Guru2MainWork *g2m);
static void func_ov137_021dfec4(Guru2MainWork *g2m);
static void func_ov137_021dfefc(Guru2MainWork *g2m);
static void func_ov137_021dff20(Guru2MainWork *g2m, int evy);
static void func_ov137_021dff44(Guru2MainWork *g2m);
static void func_ov137_021dff70(Guru2MainWork *g2m);
static void func_ov137_021dffc8(Guru2MainWork *g2m);
static void func_ov137_021dfff8(Guru2MainWork *g2m);
void func_ov137_021e00d4(Guru2MainWork *g2m);
void func_ov137_021e0130(Guru2MainWork *g2m, u32 messageId);
void func_ov137_021e01b0(Guru2MainWork *g2m, u32 messageId, UnionAppMember *member);
void func_ov137_021e026c(Guru2MainWork *g2m, u32 messageId, u32 item);
static void func_ov137_021e0304(Guru2MainWork *g2m);
static void func_ov137_021e0338(Guru2MainWork *g2m, int count);
void func_ov137_021e03a0(Guru2MainWork *g2m);
void func_ov137_021e03d8(Guru2MainWork *g2m, StrBuf *name, int index, u32 color);
static void func_ov137_021e0450(Guru2MainWork *g2m, StrBuf *name, int index, int netId);
static void func_ov137_021e0478(Guru2MainWork *g2m, int index);
static void func_ov137_021e04b4(Guru2MainWork *g2m);
static BOOL func_ov137_021e04c4(Guru2MainWork *g2m);
static void func_ov137_021e04e0(Guru2MainWork *g2m);
static void func_ov137_021e0554(Guru2MainWork *g2m);
static void func_ov137_021e0570(Guru2MainWork *g2m);
static void func_ov137_021e05d4(Guru2MainWork *g2m);
static void func_ov137_021e0634(Guru2Disc *disc, fx32 angle);
static void func_ov137_021e0640(Guru2MainWork *g2m);
static void func_ov137_021e0678(Guru2MainWork *g2m);
static void func_ov137_021e0698(Guru2MainWork *g2m);
static void func_ov137_021e06e0(Guru2MainWork *g2m);
static void func_ov137_021e073c(Guru2MainWork *g2m);
static void func_ov137_021e078c(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e07a4(Guru2MainWork *g2m);
static void func_ov137_021e0848(Guru2MainWork *g2m);
static void func_ov137_021e0980(Guru2MainWork *g2m, fx32 angle);
static void func_ov137_021e09c0(Guru2Egg *egg, const VecFx32 *offset, fx32 angle, fx32 jumpY);
static Guru2Egg *func_ov137_021e0a44(Guru2MainWork *g2m);
static void func_ov137_021e0b14(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e0b38(Guru2MainWork *g2m, Guru2EggShadow *shadow, fx32 jumpY);
static void func_ov137_021e0ba4(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e0bc0(Guru2MainWork *g2m, Guru2EggCursor *cursor, int index);
static void func_ov137_021e0c08(Guru2MainWork *g2m, int netId, int position, int index, int angle, int angleOffset,
                             Guru2Egg *egg);
static void func_ov137_021e0c70(TCB *tcb, void *data);
static int func_ov137_021e0ca4(Guru2EggFallTask *task);
static int func_ov137_021e0ccc(Guru2EggFallTask *task);
static int func_ov137_021e0d0c(Guru2EggFallTask *task);
static void func_ov137_021e0d50(Guru2MainWork *g2m, Guru2EggJumpTask *task);
static void func_ov137_021e0dc0(TCB *tcb, void *data);
static void func_ov137_021e0de8(Guru2MainWork *g2m);
static void func_ov137_021e0e1c(Guru2MainWork *g2m);
static void func_ov137_021e0e2c(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e0e50(Guru2MainWork *g2m);
static BOOL func_ov137_021e0e8c(Guru2MainWork *g2m);
static void func_ov137_021e0eac(Guru2EggSpinTask *task);
static void func_ov137_021e0efc(TCB *tcb, void *data);
static void func_ov137_021e0f20(Guru2MainWork *g2m);
static void func_ov137_021e0f54(Guru2MainWork *g2m);
static void func_ov137_021e0f64(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e0fac(Guru2EggShakeTask *task);
static void func_ov137_021e1028(TCB *tcb, void *data);
static void func_ov137_021e104c(Guru2MainWork *g2m);
static void func_ov137_021e1080(Guru2MainWork *g2m);
static void func_ov137_021e1090(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e10d8(Guru2MainWork *g2m, Guru2DiscBumpTask *task);
static void func_ov137_021e11a0(TCB *tcb, void *data);
static void func_ov137_021e11b4(Guru2MainWork *g2m);
static void func_ov137_021e11e4(Guru2MainWork *g2m);
static void func_ov137_021e11f4(Guru2MainWork *g2m);
static BOOL func_ov137_021e1204(Guru2MainWork *g2m);
static BOOL func_ov137_021e1210(Guru2MainWork *g2m);
static void func_ov137_021e121c(TCB *tcb, void *data);
static void func_ov137_021e129c(Guru2MainWork *g2m);
static void func_ov137_021e1320(Guru2MainWork *g2m, Guru2ButtonTask *task);
static void func_ov137_021e13a8(TCB *tcb, void *data);
static void func_ov137_021e13bc(Guru2MainWork *g2m);
static void func_ov137_021e13f0(Guru2MainWork *g2m);
static u32 func_ov137_021e1400(Guru2MainWork *g2m);
static void func_ov137_021e140c(Guru2MainWork *g2m);
static void func_ov137_021e141c(Guru2MainWork *g2m, Guru2BonusTask *task);
static void func_ov137_021e1468(TCB *tcb, void *data);
static void func_ov137_021e1488(Guru2MainWork *g2m);
static void func_ov137_021e14bc(Guru2MainWork *g2m);
static void func_ov137_021e14cc(Guru2MainWork *g2m, Guru2Egg *egg);
static void func_ov137_021e14e4(Guru2MainWork *g2m);
static void func_ov137_021e1540(TCB *tcb, void *data);
static void func_ov137_021e1584(Guru2MainWork *g2m, int out);
static void func_ov137_021e15dc(fx32 *angle, fx32 addend);
static void func_ov137_021e1604(Guru2MainWork *g2m, fx32 angle);
static BOOL func_ov137_021e161c(Guru2MainWork *g2m);
static void *func_ov137_021e1634(Guru2MainWork *g2m, u32 fileId, BOOL head);
static void *func_ov137_021e16ac(Guru2MainWork *g2m, u32 size);
static void *func_ov137_021e16c0(Guru2MainWork *g2m);
static BOOL func_ov137_021e16c4(Guru2MainWork *g2m, u32 signal);
static BOOL func_ov137_021e16d8(Guru2MainWork *g2m);
static int func_ov137_021e170c(Guru2MainWork *g2m);
static BOOL func_ov137_021e1768(Guru2MainWork *g2m, int netId);
static void func_ov137_021e1784(Guru2MainWork *g2m, int netId);
static int func_ov137_021e17ec(Guru2MainWork *g2m);
static BOOL func_ov137_021e1830(Guru2MainWork *g2m, fx32 angle, int playerCount);
static BOOL func_ov137_021e18cc(PokeParty *party);
static BOOL func_ov137_021e1910(Guru2MainWork *g2m);
static int func_ov137_021e1954(Guru2MainWork *g2m);
static BOOL func_ov137_021e1974(Guru2MainWork *g2m);
static void func_ov137_021e1988(u32 me);
static BOOL func_ov137_021e19a4(void);
static void func_ov137_021e19c0(Guru2MainWork *g2m, fx32 angle);

// pokebw2's sGuru2VRAMConfig
static const BGSysVRAMConfig data_ov137_021e1ea4 = {
    GX_VRAM_BG_128_B,      GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_E | GX_VRAM_F, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A,       GX_VRAM_TEXPLTT_0_G,     GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// How much each of the Eggs' hops on a bonus area lifts it
// pokebw2's sBonusHopHeights
static int data_ov137_021e2278[] = {
#include "asset/guru2_main_data_ov137_021e2278.inc"
};

// pokebw2's sNameWindows5
static const Guru2NameWindowData data_ov137_021e1f64[] = {
#include "asset/guru2_main_data_ov137_021e1f64.inc"
};

// pokebw2's sGuru2ButtonRects
static const TouchRect data_ov137_021e1cb0[] = {
    { TOUCH_RECT_CIRCLE, 128, 96, 64 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The angle that turns each position to the front, by player count
// pokebw2's sPositionAngles
static const u16 data_ov137_021e1fa0[GURU2_MEMBER_MAX + 1][GURU2_MEMBER_MAX] = {
#include "asset/guru2_main_data_ov137_021e1fa0.inc"
};

// What turns each disc's model to match its positions, by player count
// pokebw2's sDiscAngleOffsets
static const fx32 data_ov137_021e1de8[GURU2_MEMBER_MAX + 1] = {
    0, 0, FX32_CONST(-72), 0, FX32_CONST(-181), 0,
};

// pokebw2's sEggResources
static const G3DSceneResourceSetup data_ov137_021e1cd0[] = {
    { ARCID_GURU2, NARC_GURU2_EGG_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sDisc2Resources
static const G3DSceneResourceSetup data_ov137_021e1cdc[] = {
    { ARCID_GURU2, NARC_GURU2_DISC_2_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sEdgeColors
static const GXRgb data_ov137_021e1d90[8] = {
    GX_RGB(4, 4, 4), GX_RGB(4, 4, 4), GX_RGB(4, 4, 4), GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4), GX_RGB(4, 4, 4), GX_RGB(4, 4, 4), GX_RGB(4, 4, 4),
};

// pokebw2's sNameWindows4
static const Guru2NameWindowData data_ov137_021e1ed4[] = {
#include "asset/guru2_main_data_ov137_021e1ed4.inc"
};

// pokebw2's sGuru2CameraTarget
static const VecFx32 data_ov137_021e1d0c = { 0, 0, 0 };

// pokebw2's sDiscActors
static const G3DSceneActorSetup data_ov137_021e1d60[] = {
    { 0, 0, 0, 0, NULL, 0 },
};

// The first and last of the Berries a bonus area gives, by player count
// pokebw2's sBonusItems
static const u32 data_ov137_021e1f34[GURU2_MEMBER_MAX + 1][2] = {
    { ITEM_NONE, ITEM_NONE },
    { ITEM_NONE, ITEM_NONE },
    { ITEM_FIGY_BERRY, ITEM_IAPAPA_BERRY },
    { ITEM_FIGY_BERRY, ITEM_IAPAPA_BERRY },
    { ITEM_CHERI_BERRY, ITEM_ASPEAR_BERRY },
    { ITEM_POMEG_BERRY, ITEM_TAMATO_BERRY },
};

// pokebw2's sBGActors
static const G3DSceneActorSetup data_ov137_021e1d70[] = {
    { 0, 0, 0, 0, NULL, 0 },
};

// Where on the disc each position's Egg is, by player count
// pokebw2's sEggAngles
static const u16 data_ov137_021e1fdc[GURU2_MEMBER_MAX + 1][GURU2_MEMBER_MAX] = {
#include "asset/guru2_main_data_ov137_021e1fdc.inc"
};

// pokebw2's sDisc4Resources
static const G3DSceneResourceSetup data_ov137_021e1d24[] = {
    { ARCID_GURU2, NARC_GURU2_DISC_4_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sCursor3Resources
static const G3DSceneResourceSetup data_ov137_021e1e18[] = {
    { ARCID_GURU2, NARC_GURU2_CURSOR_3_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_GURU2, NARC_GURU2_CURSOR_3_ANM, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sDisc5Resources
static const G3DSceneResourceSetup data_ov137_021e1d18[] = {
    { ARCID_GURU2, NARC_GURU2_DISC_5_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sNameWindows3
static const Guru2NameWindowData data_ov137_021e1e80[] = {
#include "asset/guru2_main_data_ov137_021e1e80.inc"
};

// pokebw2's sShadowResources
static const G3DSceneResourceSetup data_ov137_021e1cf4[] = {
    { ARCID_GURU2, NARC_GURU2_SHADOW_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sDisc3Resources
static const G3DSceneResourceSetup data_ov137_021e1ce8[] = {
    { ARCID_GURU2, NARC_GURU2_DISC_3_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sGuru2CameraPosition
static const VecFx32 data_ov137_021e1d00 = { 0, FX32_CONST(48), FX32_CONST(112) };

// pokebw2's sNameWindows2
static const Guru2NameWindowData data_ov137_021e1da0[] = {
#include "asset/guru2_main_data_ov137_021e1da0.inc"
};

// pokebw2's sEggActors
static const G3DSceneActorSetup data_ov137_021e1d30[] = {
    { 0, 0, 0, 0, NULL, 0 },
};

// pokebw2's sBGResources
static const G3DSceneResourceSetup data_ov137_021e1cc4[] = {
    { ARCID_GURU2, NARC_GURU2_BG_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sCursor0Resources
static const G3DSceneResourceSetup data_ov137_021e1db8[] = {
    { ARCID_GURU2, NARC_GURU2_CURSOR_0_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_GURU2, NARC_GURU2_CURSOR_0_ANM, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sCursorAnimations
static const G3DSceneAnimationSetup data_ov137_021e1c90[] = {
    { 1, 0 },
};

// pokebw2's sCursor1Resources
static const G3DSceneResourceSetup data_ov137_021e1e48[] = {
    { ARCID_GURU2, NARC_GURU2_CURSOR_1_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_GURU2, NARC_GURU2_CURSOR_1_ANM, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sShadowActors
static const G3DSceneActorSetup data_ov137_021e1d40[] = {
    { 0, 0, 0, 0, NULL, 0 },
};

// pokebw2's sCursor4Resources
static const G3DSceneResourceSetup data_ov137_021e1dd0[] = {
    { ARCID_GURU2, NARC_GURU2_CURSOR_4_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_GURU2, NARC_GURU2_CURSOR_4_ANM, 0, G3D_SCENE_RES_ARCSYS },
};

// By player count, from two
// pokebw2's sDiscScenes
static const G3DSceneSetup data_ov137_021e20ec[] = {
    { data_ov137_021e1cdc, NELEMS(data_ov137_021e1cdc), data_ov137_021e1d60, NELEMS(data_ov137_021e1d60) },
    { data_ov137_021e1ce8, NELEMS(data_ov137_021e1ce8), data_ov137_021e1d60, NELEMS(data_ov137_021e1d60) },
    { data_ov137_021e1d24, NELEMS(data_ov137_021e1d24), data_ov137_021e1d60, NELEMS(data_ov137_021e1d60) },
    { data_ov137_021e1d18, NELEMS(data_ov137_021e1d18), data_ov137_021e1d60, NELEMS(data_ov137_021e1d60) },
};

// pokebw2's sCursorActors
static const G3DSceneActorSetup data_ov137_021e1d50[] = {
    { 0, 0, 0, 0, data_ov137_021e1c90, NELEMS(data_ov137_021e1c90) },
};

// pokebw2's sCursor2Resources
static const G3DSceneResourceSetup data_ov137_021e1e00[] = {
    { ARCID_GURU2, NARC_GURU2_CURSOR_2_NSBMD, 0, G3D_SCENE_RES_ARCSYS },
    { ARCID_GURU2, NARC_GURU2_CURSOR_2_ANM, 0, G3D_SCENE_RES_ARCSYS },
};

// pokebw2's sBGScene
static const G3DSceneSetup data_ov137_021e1d80 = { data_ov137_021e1cc4, NELEMS(data_ov137_021e1cc4), data_ov137_021e1d70, NELEMS(data_ov137_021e1d70) };

// pokebw2's sEggFallFuncs
static const Guru2EggFallFunc data_ov137_021e1cb8[] = {
    func_ov137_021e0ca4,
    func_ov137_021e0ccc,
    func_ov137_021e0d0c,
};

// pokebw2's sBonusAreas2
static const Guru2BonusArea data_ov137_021e1c94[] = {
    { 0, 180 },
};

// pokebw2's sNameWindows
static const Guru2NameWindowData *const data_ov137_021e1e30[GURU2_MEMBER_MAX + 1] = {
    NULL, NULL, data_ov137_021e1da0, data_ov137_021e1e80, data_ov137_021e1ed4, data_ov137_021e1f64,
};

// pokebw2's sGuru2MainSeqFuncs
static const Guru2MainSeqFunc data_ov137_021e217c[] = {
    func_ov137_021dea70,
    func_ov137_021dea84,
    func_ov137_021deac8,
    func_ov137_021deae0,
    func_ov137_021deb1c,
    func_ov137_021deb40,
    func_ov137_021deb74,
    func_ov137_021dec08,
    func_ov137_021dec38,
    func_ov137_021dec64,
    func_ov137_021dec84,
    func_ov137_021decb0,
    func_ov137_021deccc,
    func_ov137_021decf4,
    func_ov137_021ded3c,
    func_ov137_021ded88,
    func_ov137_021dedac,
    func_ov137_021dee4c,
    func_ov137_021deed8,
    func_ov137_021def14,
    func_ov137_021def3c,
    func_ov137_021defb4,
    func_ov137_021defd0,
    func_ov137_021defe8,
    func_ov137_021df004,
    func_ov137_021df01c,
    func_ov137_021df050,
    func_ov137_021df060,
    func_ov137_021df1c4,
    func_ov137_021df204,
    func_ov137_021df22c,
    func_ov137_021df2a0,
    func_ov137_021df2b8,
    func_ov137_021df2e0,
    func_ov137_021df370,
    func_ov137_021df3e8,
    func_ov137_021df434,
    func_ov137_021df488,
    func_ov137_021df4ac,
    func_ov137_021df548,
    func_ov137_021df56c,
    func_ov137_021df598,
    func_ov137_021df5b0,
    func_ov137_021df5ec,
    func_ov137_021df648,
    func_ov137_021df6a4,
    func_ov137_021df6b8,
    func_ov137_021df6d4,
    func_ov137_021df6fc,
    func_ov137_021df778,
    func_ov137_021df7a4,
    func_ov137_021df7cc,
    func_ov137_021df7e4,
};

// pokebw2's Guru2Main_Init
BOOL func_ov137_021de7fc(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2ProcWork *g2p = param;
    int i;
    Guru2MainWork *g2m;

    func_0203d7b0(1);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GURU2, 0x80000);
    g2m = GFL_ProcInitSubsystem(proc, sizeof(Guru2MainWork), HEAPID_GURU2);
    i = 0;
    MI_CpuFill8(g2m, 0, sizeof(Guru2MainWork));
    g2p->g2m = g2m;
    g2m->g2p = g2p;
    g2m->g2c = g2p->g2c;
    g2m->party = GameData_GetParty(g2m->g2p->param->gameData);
    g2m->arc = GFL_ArcSysCreateFileHandle(ARCID_GURU2, HEAPID_GURU2);
    func_ov137_021df800(g2m);
    g2m->vblankTCB = GFL_VBlankTCBAdd(func_ov137_021df7e8, g2m, 1);
    g2m->g3dManager = GFL_G3DMgrCreate(20, 11, HEAPID_GURU2);
    func_ov137_021e0640(g2m);
    func_ov137_021e06e0(g2m);
    func_ov137_021e04e0(g2m);

    // Until the parent sends the positions, the members are in net ID order
    {
        int position = 0;
        int netId = func_02039338();
        Guru2Disc *disc = &g2m->disc;

        for (; i < GURU2_MEMBER_MAX; i++) {
            if (g2m->g2p->memberBits & (1 << i)) {
                if (i == netId) {
                    break;
                }
                position++;
            }
        }
        disc->baseAngle = data_ov137_021e1fa0[g2m->g2p->memberCount][position] << FX32_SHIFT;
    }

    g2m->tcbBuffer =
        GFL_HeapAllocate(HEAPID_TAIL(HEAPID_GURU2), GFL_TCBMgrCalcAllocSize(32), FALSE, "guru2_main.c", 1260);
    g2m->tcbManager = GFL_TCBMgrCreate(32, g2m->tcbBuffer);
    func_ov137_021e0de8(g2m);
    func_ov137_021e0f20(g2m);
    func_ov137_021e104c(g2m);
    func_ov137_021e11b4(g2m);
    func_ov137_021e13bc(g2m);
    func_ov137_021e1488(g2m);
    GFL_WipeSet(0, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 8, 1, HEAPID_GURU2);
    func_0203d7bc(TRUE);
    return TRUE;
}

// pokebw2's Guru2Main_Exit
BOOL func_ov137_021de960(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2MainWork *g2m = work;

    func_0201c6ec(g2m->msg.queue);
    func_0201c4c0(g2m->msg.queue);
    if (g2m->waitIcon != NULL) {
        func_0202f038(g2m->waitIcon);
    }
    func_ov137_021e0e1c(g2m);
    func_ov137_021e0f54(g2m);
    func_ov137_021e1080(g2m);
    func_ov137_021e11e4(g2m);
    func_ov137_021e13f0(g2m);
    func_ov137_021e14bc(g2m);
    func_02030da4(g2m->tcbManager);
    GFL_HeapFree(g2m->tcbBuffer);
    func_ov137_021e0678(g2m);
    func_ov137_021e0554(g2m);
    func_ov137_021e073c(g2m);
    GFL_G3DMgrFree(g2m->g3dManager);
    GFL_G3DSysFree();
    func_ov137_021df83c(g2m);
    GFL_TCBRemove(g2m->vblankTCB);
    GFL_ArcToolFree(g2m->arc);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GURU2);
    func_020351a0(8);
    return TRUE;
}

// pokebw2's Guru2Main_Main
BOOL func_ov137_021dea0c(GameProc *proc, u32 *state, void *param, void *work) {
    Guru2MainWork *g2m = work;
    int result;

    do {
        result = data_ov137_021e217c[g2m->seq](g2m);
    } while (result == SEQ_RESULT_CONTINUE);
    if (result == SEQ_RESULT_END) {
        return TRUE;
    }

    func_ov137_021e0570(g2m);
    func_ov137_021e07a4(g2m);
    func_ov137_021df864(g2m);
    func_ov137_021dea68(g2m);
    GFL_TCBMgrUpdate(g2m->tcbManager);
    if (GFL_WipeIsFinished() && func_0200fb24()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Guru2Main_ClearReceived
static void func_ov137_021dea68(Guru2MainWork *g2m) {
    g2m->buttonReceived = 0;
    g2m->gameDataReceived = FALSE;
}

// pokebw2's Guru2MainSeq_Init
static int func_ov137_021dea70(Guru2MainWork *g2m) {
    func_ov137_021e0130(g2m, AbilityHandlers_Text_000);
    g2m->seq = SEQ_FADE_IN_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_FadeInWait
static int func_ov137_021dea84(Guru2MainWork *g2m) {
    if (GFL_WipeIsFinished()) {
        if (func_02039338() == 0) {
            g2m->seq = SEQ_PARENT_WAIT_CHILDREN;
        } else {
            g2m->seq = SEQ_CHILD_SEND_JOIN;
        }
        g2m->waitIcon = func_0202ee30(g2m->tcbManager, g2m->msg.msgWindow, 15, 16, HEAPID_GURU2);
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ParentWaitChildren
static int func_ov137_021deac8(Guru2MainWork *g2m) {
    if (func_ov137_021e16d8(g2m)) {
        g2m->seq = SEQ_PARENT_SEND_JOIN_SIGNAL;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ParentSendJoinSignal
static int func_ov137_021deae0(Guru2MainWork *g2m) {
    u16 signal = GURU2_MAIN_SIGNAL_JOIN;

    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_5, &signal, sizeof(signal)) == TRUE) {
        g2m->seq = SEQ_PARENT_WAIT_JOIN;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2Main_GetUnionApp
static UnionApp *func_ov137_021deb10(Guru2MainWork *g2m) {
    return g2m->g2p->param->app;
}

// pokebw2's Guru2MainSeq_ParentWaitJoin
static int func_ov137_021deb1c(Guru2MainWork *g2m) {
    int count = func_ov137_021e170c(g2m) + 1;

    if (count != func_ov022_021841a4(func_ov137_021deb10(g2m))) {
        return SEQ_RESULT_WAIT;
    }
    g2m->seq = SEQ_PARENT_SEND_PLAYER_COUNT;
    return SEQ_RESULT_CONTINUE;
}

// pokebw2's Guru2MainSeq_ParentSendPlayerCount
static int func_ov137_021deb40(Guru2MainWork *g2m) {
    g2m->playerCount = func_ov137_021e170c(g2m) + 1;
    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_10, &g2m->playerCount, sizeof(g2m->playerCount)) == TRUE) {
        g2m->sendCount = 0;
        g2m->seq = SEQ_PARENT_SEND_POSITIONS;
    }
    return SEQ_RESULT_WAIT;
}

// Sends a member's position a frame, the parent's first
// pokebw2's Guru2MainSeq_ParentSendPositions
static int func_ov137_021deb74(Guru2MainWork *g2m) {
    Guru2PositionData data;
    int i;
    int count;

    if (g2m->sendCount == 0) {
        data.netId = 0;
        data.position = 0;
        if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_9, &data, sizeof(data)) == TRUE) {
            g2m->sendCount++;
        }
        return SEQ_RESULT_WAIT;
    }

    for (i = 1, count = 1; i < GURU2_MEMBER_MAX; i++) {
        if (g2m->g2c->unk42 & (1 << i)) {
            if (count >= g2m->sendCount) {
                data.netId = i;
                data.position = g2m->sendCount;
                if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_9, &data, sizeof(data)) == TRUE) {
                    g2m->sendCount++;
                }
                break;
            }
            count++;
        }
    }
    if (g2m->sendCount >= g2m->playerCount) {
        g2m->seq = SEQ_PARENT_SEND_START_SIGNAL;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ParentSendStartSignal
static int func_ov137_021dec08(Guru2MainWork *g2m) {
    u16 signal = GURU2_MAIN_SIGNAL_START;

    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_5, &signal, sizeof(signal)) == TRUE) {
        g2m->seq = SEQ_SYNC_START;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ChildSendJoin
static int func_ov137_021dec38(Guru2MainWork *g2m) {
    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_6, NULL, 0) == TRUE) {
        g2m->seq = SEQ_CHILD_WAIT_START_SIGNAL;
        g2m->unk10 = 0;
        return SEQ_RESULT_WAIT;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ChildWaitStartSignal
static int func_ov137_021dec64(Guru2MainWork *g2m) {
    if (func_ov137_021e16c4(g2m, GURU2_MAIN_SIGNAL_START) == TRUE) {
        g2m->playerCount = func_ov137_021e170c(g2m) + 1;
        g2m->seq = SEQ_SYNC_START;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncStart
static int func_ov137_021dec84(Guru2MainWork *g2m) {
    NetHandle *handle = func_020399a8();

    func_0202f038(g2m->waitIcon);
    g2m->waitIcon = NULL;
    func_02039c68(handle, GURU2_SYNC_START, GURU2_SYNC_CLASS);
    g2m->seq = SEQ_SYNC_START_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncStartWait
static int func_ov137_021decb0(Guru2MainWork *g2m) {
    if (func_02039cb4(func_020399a8(), GURU2_SYNC_START, GURU2_SYNC_CLASS)) {
        g2m->seq = SEQ_SEND_PARTY_INDEX;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SendPartyIndex
static int func_ov137_021deccc(Guru2MainWork *g2m) {
    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_11, &g2m->g2p->partyIndex, sizeof(g2m->g2p->partyIndex))) {
        g2m->seq = SEQ_SEND_PARTY;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SendParty
static int func_ov137_021decf4(Guru2MainWork *g2m) {
    NetHandle *handle = func_020399a8();
    u32 joined = func_ov022_021841a0(func_ov137_021deb10(g2m));

    if (func_0203d4d0(handle, (u8)joined, GURU2_NET_CMD_BLOCK, GURU2_COMM_BLOCK_SIZE, g2m->party, TRUE, TRUE, TRUE)) {
        g2m->seq = SEQ_WAIT_PARTIES;
    }
    return SEQ_RESULT_WAIT;
}

// Once every party has arrived, tells the others whether one of them has a bad Egg
// pokebw2's Guru2MainSeq_WaitParties
static int func_ov137_021ded3c(Guru2MainWork *g2m) {
    if (g2m->playerCount == func_ov137_021e17ec(g2m)) {
        if (func_ov137_021e1910(g2m) == TRUE) {
            func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_13, NULL, 0);
            g2m->seq = SEQ_BAD_EGG;
        } else {
            func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_14, NULL, 0);
            g2m->seq = SEQ_WAIT_EGG_CHECK;
        }
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_WaitEggCheck
static int func_ov137_021ded88(Guru2MainWork *g2m) {
    if (func_ov137_021e1974(g2m) == TRUE) {
        g2m->seq = SEQ_BAD_EGG;
    } else if (g2m->playerCount == func_ov137_021e1954(g2m)) {
        g2m->seq = SEQ_GAME_INIT;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_GameInit
static int func_ov137_021dedac(Guru2MainWork *g2m) {
    Guru2EggFallSeqWork *seqWork = func_ov137_021e16ac(g2m, sizeof(Guru2EggFallSeqWork));
    int i;

    seqWork->position = g2m->position;
    seqWork->playerCount = g2m->playerCount;
    seqWork->angle = data_ov137_021e1fa0[seqWork->playerCount][g2m->position];
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (func_ov137_021e1768(g2m, i)) {
            g2m->members[i] = func_ov022_02184180(func_ov137_021deb10(g2m), i);
            textCopy(g2m->members[i]->info.name, g2m->names[i]);
        }
    }
    func_ov137_021def6c(g2m);
    g2m->disc.baseAngle = seqWork->angle << FX32_SHIFT;
    g2m->seq = SEQ_EGG_FALL;
    GFL_SndBGMPlay(SEQ_BGM_GURUGURU, 0xffff);
    g2m->g2p->param->started = TRUE;
    func_ov137_021e0304(g2m);
    return SEQ_RESULT_CONTINUE;
}

// Drops an Egg every 15 frames, the player's own first
// pokebw2's Guru2MainSeq_EggFall
static int func_ov137_021dee4c(Guru2MainWork *g2m) {
    Guru2EggFallSeqWork *seqWork;
    int angle;
    int netId;

    g2m->wait--;
    if (g2m->wait > 0) {
        return SEQ_RESULT_WAIT;
    }

    g2m->wait = 15;
    seqWork = func_ov137_021e16c0(g2m);
    angle = data_ov137_021e1fdc[seqWork->playerCount][seqWork->position];
    netId = g2m->positions[seqWork->position].netId;
    g2m->gameData.eggAngles[seqWork->position] = angle;
    func_ov137_021e0c08(g2m, netId, seqWork->position, seqWork->count, angle, seqWork->angle,
                     &g2m->objects.eggs[seqWork->position]);
    seqWork->position++;
    seqWork->position %= seqWork->playerCount;
    seqWork->count++;
    if (seqWork->count == seqWork->playerCount) {
        g2m->wait = 0;
        g2m->seq = SEQ_EGG_FALL_WAIT;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_EggFallWait
static int func_ov137_021deed8(Guru2MainWork *g2m) {
    Guru2Objects *objects = &g2m->objects;
    int i = 0;
    int landed = 0;
    int count = g2m->playerCount;

    do {
        if (objects->eggs[i].landed == TRUE) {
            landed++;
        }
        i++;
    } while (i < count);
    if (landed == count) {
        g2m->seq = SEQ_COUNTDOWN_BEFORE;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_CountdownBefore
static int func_ov137_021def14(Guru2MainWork *g2m) {
    g2m->wait++;
    if (g2m->wait > 30) {
        g2m->wait = 0;
        if (func_02039338() == 0) {
            g2m->seq = SEQ_PARENT_SEND_COUNTDOWN_SIGNAL;
        } else {
            g2m->seq = SEQ_CHILD_WAIT_COUNTDOWN_SIGNAL;
        }
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ParentSendCountdownSignal
static int func_ov137_021def3c(Guru2MainWork *g2m) {
    u16 signal = GURU2_MAIN_SIGNAL_COUNTDOWN;

    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_5, &signal, sizeof(signal)) == TRUE) {
        g2m->seq = SEQ_SYNC_COUNTDOWN;
    }
    return SEQ_RESULT_WAIT;
}

// Records the other members as trainers the player has met
// pokebw2's Guru2Main_AddMetTrainers
static void func_ov137_021def6c(Guru2MainWork *g2m) {
    RivalDataSave *rivalData = getHollow_RivalData(GameData_GetSaveControl(g2m->g2p->param->gameData));
    int i;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (g2m->members[i] != NULL && i != func_02039338()) {
            func_0200ed54(rivalData, getIDAsUInt(&g2m->members[i]->info));
        }
    }
}

// pokebw2's Guru2MainSeq_ChildWaitCountdownSignal
static int func_ov137_021defb4(Guru2MainWork *g2m) {
    if (func_ov137_021e16c4(g2m, GURU2_MAIN_SIGNAL_COUNTDOWN) == TRUE) {
        g2m->seq = SEQ_SYNC_COUNTDOWN;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncCountdown
static int func_ov137_021defd0(Guru2MainWork *g2m) {
    func_02039c68(func_020399a8(), GURU2_SYNC_COUNTDOWN, GURU2_SYNC_CLASS);
    g2m->seq = SEQ_SYNC_COUNTDOWN_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncCountdownWait
static int func_ov137_021defe8(Guru2MainWork *g2m) {
    if (func_02039cb4(func_020399a8(), GURU2_SYNC_COUNTDOWN, GURU2_SYNC_CLASS)) {
        g2m->seq = SEQ_COUNTDOWN_START;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_CountdownStart
static int func_ov137_021df004(Guru2MainWork *g2m) {
    func_ov137_021e1584(g2m, TRUE);
    func_ov137_021e129c(g2m);
    g2m->seq = SEQ_COUNTDOWN_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_CountdownWait
static int func_ov137_021df01c(Guru2MainWork *g2m) {
    int i;

    g2m->wait++;
    if (g2m->wait < 95) {
        return SEQ_RESULT_WAIT;
    }

    for (i = 0; i < g2m->playerCount; i++) {
        func_ov137_021e0478(g2m, i);
    }
    g2m->wait = 0;
    g2m->seq = SEQ_GAME_START;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_GameStart
static int func_ov137_021df050(Guru2MainWork *g2m) {
    func_02039338();
    g2m->seq = SEQ_GAME;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_Game
static int func_ov137_021df060(Guru2MainWork *g2m) {
    Guru2GameData data;
    int i;
    BOOL timeUp = func_ov137_021e161c(g2m);
    Guru2Disc *disc;

    // The game ends once the Eggs are down after the time is up
    if (timeUp == TRUE && !func_ov137_021e1210(g2m) && g2m->gameData.jumpY == 0) {
        g2m->timeUp = TRUE;
    }
    if (g2m->gameData.timeUp == TRUE) {
        g2m->seq = SEQ_GAME_END;
        return SEQ_RESULT_CONTINUE;
    }

    if (timeUp == FALSE) {
        if (g2m->buttonReceived != 0 && !func_ov137_021e1210(g2m)) {
            func_ov137_021e11f4(g2m);
        }
        if (!func_ov137_021e1400(g2m) && func_ov137_021e04c4(g2m) == TRUE) {
            if (!func_ov137_021e1210(g2m)) {
                g2m->buttonSend = 1;
            }
            func_ov137_021e140c(g2m);
        }
        if (g2m->buttonSend != 0) {
            if (func_ov137_021dcbc4(g2m->g2p->g2c, GURU2_NET_CMD_7, &g2m->buttonSend, 1) == TRUE) {
                g2m->buttonSend = 0;
            }
        }
    }

    // The parent speeds the disc up in the first half and slows it down in the second
    disc = &g2m->disc;
    if (func_02039338() == 0) {
        if (g2m->gameFrame < GAME_SLOW_FRAMES) {
            disc->speed += 0x5f;
            if (disc->speed >= FX32_CONST(7)) {
                disc->speed = FX32_CONST(7);
            }
        } else {
            disc->speed -= 0x5f;
            if (disc->speed < FX32_CONST(4)) {
                disc->speed = FX32_CONST(4);
            }
        }
    }
    if (func_02039338() == 0) {
        data.discAngle = g2m->disc.angle;
        data.timeUp = g2m->timeUp;
        data.jumpY = g2m->objects.eggs[0].jumpY;
        for (i = 0; i < g2m->playerCount; i++) {
            data.eggAngles[i] = g2m->objects.eggs[i].angle / FX32_ONE;
        }
        func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_8, &data, sizeof(data));
    }
    if (func_02039338() == 0) {
        fx32 speed = g2m->disc.speed;

        // While the Eggs are in the air, the disc turns under them at twice the speed
        if (func_ov137_021e1204(g2m) == TRUE) {
            func_ov137_021e0634(&g2m->disc, speed * 2);
        } else {
            func_ov137_021e1604(g2m, speed);
        }
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_GameEnd
static int func_ov137_021df1c4(Guru2MainWork *g2m) {
    func_ov137_021e1584(g2m, FALSE);
    g2m->frontEgg = func_ov137_021e0a44(g2m);
    if (func_02039338() == 0) {
        if (g2m->frontEgg->netId == func_02039338()) {
            g2m->seq = SEQ_PARENT_ALIGN_OWN;
        } else {
            g2m->seq = SEQ_PARENT_ALIGN;
        }
    } else {
        g2m->seq = SEQ_SYNC_RESULT;
    }
    GFL_SndSEPlay(SEQ_SE_SYS_55);
    return SEQ_RESULT_CONTINUE;
}

// The parent never takes its own Egg: the disc turns on until another is in front
// pokebw2's Guru2MainSeq_ParentAlignOwn
static int func_ov137_021df204(Guru2MainWork *g2m) {
    func_ov137_021e19c0(g2m, FX32_CONST(4));
    g2m->frontEgg = func_ov137_021e0a44(g2m);
    if (g2m->frontEgg->netId != func_02039338()) {
        g2m->seq = SEQ_PARENT_ALIGN;
    }
    return SEQ_RESULT_WAIT;
}

// Turns the disc until the front Egg is at the parent's position
// pokebw2's Guru2MainSeq_ParentAlign
static int func_ov137_021df22c(Guru2MainWork *g2m) {
    int position = g2m->position;
    int count = g2m->playerCount;
    fx32 angle;
    fx32 target;
    fx32 speed;
    fx32 distance;

    speed = FX32_CONST(4);
    angle = g2m->frontEgg->angle;
    target = data_ov137_021e1fdc[count][position] << FX32_SHIFT;
    distance = target - angle;
    if (distance < 0) {
        distance = target + FULL_TURN - angle;
    }
    if (distance <= FX32_CONST(16)) {
        speed >>= 1;
    }
    if (distance < speed) {
        speed = distance;
    }
    func_ov137_021e19c0(g2m, speed);
    func_ov137_021e15dc(&angle, speed);
    if (angle / FX32_ONE == target / FX32_ONE) {
        g2m->seq = SEQ_SYNC_RESULT;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncResult
static int func_ov137_021df2a0(Guru2MainWork *g2m) {
    func_02039c68(func_020399a8(), GURU2_SYNC_RESULT, GURU2_SYNC_CLASS);
    g2m->seq = SEQ_SYNC_RESULT_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncResultWait
static int func_ov137_021df2b8(Guru2MainWork *g2m) {
    if (func_02039cb4(func_020399a8(), GURU2_SYNC_RESULT, GURU2_SYNC_CLASS)) {
        if (func_02039338() == 0) {
            g2m->seq = SEQ_PARENT_SEND_RESULT;
        } else {
            g2m->seq = SEQ_CHILD_WAIT_RESULT;
        }
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ParentSendResult
static int func_ov137_021df2e0(Guru2MainWork *g2m) {
    Guru2ResultData data;
    int i;

    data.discAngle = g2m->gameData.discAngle / FX32_ONE;
    g2m->bonusBits = 0;
    for (i = 0; i < g2m->playerCount; i++) {
        Guru2Egg *egg = &g2m->objects.eggs[i];

        data.eggAngles[egg->position] = g2m->gameData.eggAngles[i];
        if (func_ov137_021e1830(g2m, g2m->gameData.eggAngles[i] << FX32_SHIFT, g2m->playerCount) == TRUE) {
            g2m->bonusBits |= 1 << egg->netId;
        }
    }
    data.bonusBits = g2m->bonusBits;
    if (func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_12, &data, sizeof(data)) == TRUE) {
        g2m->seq = SEQ_RESULT_NAMES;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ChildWaitResult
static int func_ov137_021df370(Guru2MainWork *g2m) {
    if (g2m->resultReceived == TRUE) {
        int i;
        Guru2ResultData *result = &g2m->result;

        g2m->disc.angle = result->discAngle << FX32_SHIFT;
        g2m->gameData.discAngle = result->discAngle << FX32_SHIFT;
        for (i = 0; i < g2m->playerCount; i++) {
            Guru2Egg *egg = &g2m->objects.eggs[i];

            egg->angle = result->eggAngles[egg->position] << FX32_SHIFT;
            g2m->gameData.eggAngles[i] = result->eggAngles[egg->position];
        }
        g2m->bonusBits = result->bonusBits;
        func_ov137_021e0570(g2m);
        func_ov137_021e07a4(g2m);
        g2m->frontEgg = func_ov137_021e0a44(g2m);
        g2m->seq = SEQ_RESULT_NAMES;
    }
    return SEQ_RESULT_WAIT;
}

// Shows the names of the Eggs' owners where the Eggs stopped
// pokebw2's Guru2MainSeq_ResultNames
static int func_ov137_021df3e8(Guru2MainWork *g2m) {
    int i;
    int position = g2m->frontEgg->position;

    g2m->gameData.jumpY = 0;
    for (i = 0; i < g2m->playerCount; i++) {
        int netId = g2m->objects.eggs[position].netId;

        func_ov137_021e0450(g2m, g2m->names[netId], i, netId);
        position = (position + 1) % g2m->playerCount;
    }
    g2m->wait = 0;
    g2m->seq = SEQ_RESULT_NAMES_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ResultNamesWait
static int func_ov137_021df434(Guru2MainWork *g2m) {
    int i;

    g2m->wait++;
    if (g2m->wait < 120) {
        return SEQ_RESULT_WAIT;
    }

    g2m->wait = 0;
    for (i = 0; i < g2m->playerCount; i++) {
        func_ov137_021e0478(g2m, i);
    }
    func_ov137_021e01b0(g2m, AbilityHandlers_Text_002, g2m->members[g2m->frontEgg->netId]);
    func_ov137_021e1988(SEQ_ME_ITEM);
    g2m->seq = SEQ_RESULT_MESSAGE_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_ResultMessageWait
static int func_ov137_021df488(Guru2MainWork *g2m) {
    if (g2m->wait < 120) {
        g2m->wait++;
    } else if (!func_ov137_021e19a4()) {
        g2m->wait = 0;
        g2m->seq = SEQ_BONUS;
    }
    return SEQ_RESULT_WAIT;
}

// Gives the player a Berry if the Egg they got stopped on a bonus area
// pokebw2's Guru2MainSeq_Bonus
static int func_ov137_021df4ac(Guru2MainWork *g2m) {
    u32 item;
    BOOL added;

    func_ov137_021e14e4(g2m);
    if (!(g2m->bonusBits & (1 << g2m->frontEgg->netId))) {
        g2m->seq = SEQ_SYNC_SAVE;
        return SEQ_RESULT_WAIT;
    }

    item = data_ov137_021e1f34[g2m->playerCount][0];
    item += GFL_RandomLC(0xffff) % (data_ov137_021e1f34[g2m->playerCount][1] - item + 1);
    if (item < data_ov137_021e1f34[g2m->playerCount][0]) {
        item = data_ov137_021e1f34[g2m->playerCount][0];
    } else if (item > data_ov137_021e1f34[g2m->playerCount][1]) {
        item = data_ov137_021e1f34[g2m->playerCount][1];
    }
    func_ov137_021e026c(g2m, AbilityHandlers_Text_003, item);
    added = BagSave_AddItem(GameData_GetBag(g2m->g2p->param->gameData), item, 1, HEAPID_GURU2);
    func_ov137_021e1988(SEQ_ME_ITEM);
    if (added == TRUE) {
        g2m->seq = SEQ_BONUS_WAIT;
    } else {
        g2m->seq = SEQ_BAG_FULL_BEFORE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_BonusWait
static int func_ov137_021df548(Guru2MainWork *g2m) {
    if (g2m->wait < 180) {
        g2m->wait++;
    }
    if (!func_ov137_021e19a4()) {
        g2m->wait = 0;
        g2m->seq = SEQ_SYNC_SAVE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_BagFullBefore
static int func_ov137_021df56c(Guru2MainWork *g2m) {
    if (g2m->wait < 180) {
        g2m->wait++;
    } else if (!func_ov137_021e19a4()) {
        g2m->wait = 0;
        g2m->seq = SEQ_BAG_FULL_WAIT;
        func_ov137_021e0130(g2m, AbilityHandlers_Text_004);
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_BagFullWait
static int func_ov137_021df598(Guru2MainWork *g2m) {
    if (g2m->wait < 180) {
        g2m->wait++;
    } else {
        g2m->wait = 0;
        g2m->seq = SEQ_SYNC_SAVE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncSave
static int func_ov137_021df5b0(Guru2MainWork *g2m) {
    NetHandle *handle = func_020399a8();

    RecordAddOne(g2m->g2p->param->records, 90);
    func_ov137_021e0130(g2m, AbilityHandlers_Text_011);
    func_02039c68(handle, GURU2_SYNC_SAVE, GURU2_SYNC_CLASS);
    g2m->seq = SEQ_SYNC_SAVE_WAIT;
    func_0203516c(8);
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncSaveWait
static int func_ov137_021df5ec(Guru2MainWork *g2m) {
    if (func_02039cb4(func_020399a8(), GURU2_SYNC_SAVE, GURU2_SYNC_CLASS)) {
        func_ov137_021e1784(g2m, g2m->frontEgg->netId);
        g2m->netSave = func_0201088c(HEAPID_GURU2, g2m->g2p->param->gameData);
        g2m->waitIcon = func_0202ee30(g2m->tcbManager, g2m->msg.msgWindow, 15, 16, HEAPID_GURU2);
        g2m->seq = SEQ_SAVE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_Save
static int func_ov137_021df648(Guru2MainWork *g2m) {
    BOOL done = func_02010904(g2m->netSave);

    if (func_0200fb24()) {
        func_0200fbb0();
        GFL_NetErrShow(1);
        func_0202223c(0, WIPE_COLOR_BLACK);
        func_0202223c(1, WIPE_COLOR_BLACK);
        return SEQ_RESULT_WAIT;
    }
    if (done) {
        func_0202f038(g2m->waitIcon);
        g2m->waitIcon = NULL;
        func_02010934(g2m->netSave);
        g2m->seq = SEQ_SYNC_END;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_BadEgg
static int func_ov137_021df6a4(Guru2MainWork *g2m) {
    func_ov137_021e0130(g2m, AbilityHandlers_Text_012);
    g2m->seq = SEQ_BAD_EGG_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_BadEggWait
static int func_ov137_021df6b8(Guru2MainWork *g2m) {
    g2m->wait++;
    if (g2m->wait >= 180) {
        g2m->wait = 0;
        g2m->seq = SEQ_SYNC_END;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncEnd
static int func_ov137_021df6d4(Guru2MainWork *g2m) {
    NetHandle *handle = func_020399a8();

    if (!g2m->error) {
        func_02039c68(handle, GURU2_SYNC_END, GURU2_SYNC_CLASS);
    }
    func_ov137_021e0130(g2m, AbilityHandlers_Text_010);
    g2m->seq = SEQ_SYNC_END_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_SyncEndWait
static int func_ov137_021df6fc(Guru2MainWork *g2m) {
    NetHandle *handle = func_020399a8();

    if (!g2m->error) {
        if (func_02039cb4(handle, GURU2_SYNC_END, GURU2_SYNC_CLASS)) {
            g2m->waitIcon = func_0202ee30(g2m->tcbManager, g2m->msg.msgWindow, 15, 16, HEAPID_GURU2);
            func_0203d7bc(FALSE);
            func_0203a5e4(GURU2_NET_CMD_BASE);
            func_ov022_02184174(func_ov137_021deb10(g2m));
            g2m->seq = SEQ_END_WAIT;
        }
    } else {
        g2m->wait++;
        if (g2m->wait >= 180) {
            g2m->wait = 0;
            if (g2m->g2c->unk3C == 0) {
                func_0203d7b0(0);
            }
            g2m->seq = SEQ_END_WAIT;
        }
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_EndWait
static int func_ov137_021df778(Guru2MainWork *g2m) {
    if (func_ov022_0218417c(func_ov137_021deb10(g2m))) {
        func_0202f038(g2m->waitIcon);
        g2m->waitIcon = NULL;
        g2m->seq = SEQ_FADE_OUT;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_FadeOut
static int func_ov137_021df7a4(Guru2MainWork *g2m) {
    GFL_WipeSet(2, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 8, 1, HEAPID_GURU2);
    g2m->seq = SEQ_FADE_OUT_WAIT;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_FadeOutWait
static int func_ov137_021df7cc(Guru2MainWork *g2m) {
    if (GFL_WipeIsFinished()) {
        g2m->seq = SEQ_END;
        return SEQ_RESULT_CONTINUE;
    }
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2MainSeq_End
static int func_ov137_021df7e4(Guru2MainWork *g2m) {
    return SEQ_RESULT_END;
}

// pokebw2's Guru2Main_VBlank
static void func_ov137_021df7e8(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;

    func_02021d50(g2m->fade);
    func_0204a648();
    GFL_BGSysUpdate();
}

// pokebw2's Guru2Main_InitGraphics
static void func_ov137_021df800(Guru2MainWork *g2m) {
    func_ov137_021df984(g2m);
    func_ov137_021dfad8(g2m);
    func_ov137_021dfb60(g2m);
    func_ov137_021dfec4(g2m);
    func_ov137_021dff44(g2m);
    func_ov137_021dfc74(g2m);
    func_ov137_021dfff8(g2m);
    func_ov137_021dff70(g2m);
    func_ov137_021df998(g2m);
}

// pokebw2's Guru2Main_FreeGraphics
static void func_ov137_021df83c(Guru2MainWork *g2m) {
    func_ov137_021dfa14(g2m);
    func_ov137_021dffc8(g2m);
    func_ov137_021e00d4(g2m);
    func_ov137_021dfe40(g2m);
    func_ov137_021dfefc(g2m);
    func_ov137_021e04b4(g2m);
}

// pokebw2's Guru2Main_Draw
static void func_ov137_021df864(Guru2MainWork *g2m) {
    int i;
    fx32 far = FX32_CONST(4096);

    GFL_G3DCameraSetProjectionZFar(g2m->camera.camera, &far);
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g2m->camera.camera);
    GFL_G3DSysMtxViewFlush();
    NNS_G3dGlbLightVector(0, 0, -FX32_ONE, 0);
    NNS_G3dGlbLightColor(0, GX_RGB(31, 31, 31));
    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    func_ov137_021e0698(g2m);
    func_ov137_021e05d4(g2m);
    func_ov137_021e0848(g2m);
    GFL_G3DSysReqSwapBuffers();
    func_0204a600();

    func_0201c4e4(g2m->msg.queue);
    PrintWindow_Flush(&g2m->msg.msgPrint, g2m->msg.queue);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        PrintWindow_Flush(&g2m->msg.namePrints[i], g2m->msg.queue);
    }
    if (g2m->waitIcon != NULL) {
        func_0202f0b0(g2m->waitIcon);
    }
}

// pokebw2's Guru2Main_InitVRAM
static void func_ov137_021df984(Guru2MainWork *g2m) {
    GFL_BGSysSetVRAMBanks(&data_ov137_021e1ea4);
    BmpWin_InitAllocator(HEAPID_GURU2);
}

// pokebw2's Guru2Main_ShowBGs
static void func_ov137_021df998(Guru2MainWork *g2m) {
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabled(1, TRUE);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(6, TRUE);
    GFL_BGSysSetBGEnabled(7, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    G2_SetBG0Priority(1);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD, 11, 10);
}

// pokebw2's Guru2Main_HideBGs
static void func_ov137_021dfa14(Guru2MainWork *g2m) {
    GFL_BGSysSetBGEnabled(0, FALSE);
    GFL_BGSysSetBGEnabled(1, FALSE);
    GFL_BGSysSetBGEnabled(2, FALSE);
    GFL_BGSysSetBGEnabled(3, FALSE);
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(7, FALSE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
}

// pokebw2's Guru2Main_Setup3D
static void func_ov137_021dfa68(void) {
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(data_ov137_021e1d90);
    G3X_SetClearColor(GX_RGB(31, 31, 31), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

// pokebw2's Guru2Main_Init3D
static void func_ov137_021dfad8(Guru2MainWork *g2m) {
    Guru2CameraWork *camera = &g2m->camera;

    GFL_G3DSysCreate(FALSE, 1, FALSE, 1, 0, HEAPID_GURU2, func_ov137_021dfa68);
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    {
        VecFx32 up = { 0, FX32_ONE, 0 };

        camera->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)),
                                             FX_CosIdx(DEG_TO_IDX(20)), FX32_ONE * 4 / 3, 0, FX32_ONE, FX32_CONST(1024),
                                             0, &data_ov137_021e1d00, &up, &data_ov137_021e1d0c, HEAPID_GURU2);
    }
    GFL_G3DCameraFlush(camera->camera);
}

// pokebw2's Guru2Main_InitBGs
static void func_ov137_021dfb60(Guru2MainWork *g2m) {
    GFL_BGSysCreate(HEAPID_GURU2);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    {
        BGSysLCDConfig config = { 1, 0, 0, TRUE };

        GFL_BGSysSetLCDConfig(&config);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x0000,
            GX_BG_CHARBASE_0x04000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            FALSE,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(1);
        GFL_BGSysClearCharCore(1, BGSYS_TILE_SIZE_16, 0, HEAPID_GURU2);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x0800,
            GX_BG_CHARBASE_0x0c000,
            0x4000,
            GX_BG_EXTPLTT_01,
            2,
            0,
            FALSE,
        };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(2);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x1000,
            GX_BG_CHARBASE_0x0c000,
            0x4000,
            GX_BG_EXTPLTT_01,
            3,
            0,
            FALSE,
        };

        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(3);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x1000,
            GX_BG_CHARBASE_0x14000,
            0,
            GX_BG_EXTPLTT_01,
            2,
            0,
            FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(6);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x1800,
            GX_BG_CHARBASE_0x1c000,
            0,
            GX_BG_EXTPLTT_01,
            3,
            0,
            FALSE,
        };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(7);
    }
}

// pokebw2's Guru2Main_LoadBGs
static void func_ov137_021dfc74(Guru2MainWork *g2m) {
    void *file;

    file = func_ov137_021e1634(g2m, NARC_GURU2_BG_NCLR, FALSE);
    NNS_G2dGetUnpackedPaletteData(file, &g2m->paletteData);
    func_020215bc(g2m->fade, g2m->paletteData->pRawData, 0, 0, 0x40);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_SUB_NCLR, FALSE);
    NNS_G2dGetUnpackedPaletteData(file, &g2m->paletteData);
    func_020215bc(g2m->fade, g2m->paletteData->pRawData, 1, 0, 0x40);
    GFL_HeapFree(file);
    func_02022004(g2m->fade, 1, 0xffff, 8, GX_RGB(0, 0, 0));

    file = func_ov137_021e1634(g2m, NARC_GURU2_BG_NCGR, FALSE);
    NNS_G2dGetUnpackedCharacterData(file, &g2m->charData);
    GFL_BGSysLoadChar(3, g2m->charData->pRawData, g2m->charData->szByte, 0);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_SUB_BG2_NCGR, FALSE);
    NNS_G2dGetUnpackedCharacterData(file, &g2m->charData);
    GFL_BGSysLoadChar(6, g2m->charData->pRawData, g2m->charData->szByte, 0);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_SUB_BG3_NCGR, FALSE);
    NNS_G2dGetUnpackedCharacterData(file, &g2m->charData);
    GFL_BGSysLoadChar(7, g2m->charData->pRawData, g2m->charData->szByte, 0);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_BG2_NSCR, FALSE);
    NNS_G2dGetUnpackedScreenData(file, &g2m->screenData);
    GFL_BGSysBufferScrDefault(2, g2m->screenData->rawData, g2m->screenData->szByte);
    GFL_BGSysLoadScr(2);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_BG3_NSCR, FALSE);
    NNS_G2dGetUnpackedScreenData(file, &g2m->screenData);
    GFL_BGSysBufferScrDefault(3, g2m->screenData->rawData, g2m->screenData->szByte);
    GFL_BGSysLoadScr(3);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_BUTTON_NSCR, FALSE);
    NNS_G2dGetUnpackedScreenData(file, &g2m->screenData);
    GFL_BGSysBufferScrDefault(6, g2m->screenData->rawData, g2m->screenData->szByte);
    GFL_BGSysLoadScr(6);
    GFL_HeapFree(file);

    file = func_ov137_021e1634(g2m, NARC_GURU2_SUB_BG3_NSCR, FALSE);
    NNS_G2dGetUnpackedScreenData(file, &g2m->screenData);
    GFL_BGSysBufferScrDefault(7, g2m->screenData->rawData, g2m->screenData->szByte);
    GFL_BGSysLoadScr(7);
    GFL_HeapFree(file);
}

// pokebw2's Guru2Main_FreeBGs
static void func_ov137_021dfe40(Guru2MainWork *g2m) {
    BmpWin_FreeAllocator();
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysFree();
}

// pokebw2's Guru2Main_LoadButtonScreen
static void func_ov137_021dfe6c(Guru2MainWork *g2m, u32 fileId) {
    void *file = func_ov137_021e1634(g2m, fileId, FALSE);

    NNS_G2dGetUnpackedScreenData(file, &g2m->screenData);
    GFL_BGSysBufferScrDefault(6, g2m->screenData->rawData, g2m->screenData->szByte);
    GFL_BGSysLoadScr(6);
    GFL_HeapFree(file);
}

// pokebw2's Guru2Main_ShowButtonHalf
static void func_ov137_021dfea0(Guru2MainWork *g2m) {
    func_ov137_021dfe6c(g2m, NARC_GURU2_BUTTON_HALF_NSCR);
}

// pokebw2's Guru2Main_ShowButton
void func_ov137_021dfeac(Guru2MainWork *g2m) {
    func_ov137_021dfe6c(g2m, NARC_GURU2_BUTTON_NSCR);
}

// pokebw2's Guru2Main_ShowButtonPushed
static void func_ov137_021dfeb8(Guru2MainWork *g2m) {
    func_ov137_021dfe6c(g2m, NARC_GURU2_BUTTON_PUSHED_NSCR);
}

// pokebw2's Guru2Main_InitFade
static void func_ov137_021dfec4(Guru2MainWork *g2m) {
    g2m->fade = func_02021518(HEAPID_GURU2);
    func_02021ee4(g2m->fade, TRUE);
    func_0202155c(g2m->fade, 0, 0x200, HEAPID_GURU2);
    func_0202155c(g2m->fade, 1, 0x200, HEAPID_GURU2);
}

// pokebw2's Guru2Main_FreeFade
static void func_ov137_021dfefc(Guru2MainWork *g2m) {
    func_020215a0(g2m->fade, 0);
    func_020215a0(g2m->fade, 1);
    func_02021540(g2m->fade);
}

// Darkens the touch screen
// pokebw2's Guru2Main_SetSubFade
static void func_ov137_021dff20(Guru2MainWork *g2m, int evy) {
    func_02022004(g2m->fade, 1, 0xffff, evy, GX_RGB(0, 0, 0));
}

// pokebw2's Guru2Main_InitClAct
static void func_ov137_021dff44(Guru2MainWork *g2m) {
    ClActSys_Create(&data_020a1448, &data_ov137_021e1ea4, HEAPID_GURU2);
    g2m->clactUnit = func_0204b100(30, 1, HEAPID_GURU2);
}

// pokebw2's Guru2Main_LoadClActResources
static void func_ov137_021dff70(Guru2MainWork *g2m) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_GURU2_OBJ, HEAPID_GURU2);

    g2m->chars = func_0204a6c8(arc, 6, FALSE, 0, HEAPID_GURU2);
    g2m->palette = func_0204abf0(arc, 0, 0, 0, HEAPID_GURU2);
    g2m->cellAnims = func_0204af28(arc, 10, 14, HEAPID_GURU2);
    GFL_ArcToolFree(arc);
    func_0203d34c(TRUE, HEAPID_GURU2);
}

// pokebw2's Guru2Main_FreeClAct
static void func_ov137_021dffc8(Guru2MainWork *g2m) {
    func_0204a8d4(g2m->chars);
    func_0204ada4(g2m->palette);
    func_0204afd8(g2m->cellAnims);
    func_0204b1cc(g2m->clactUnit);
    func_0204a5ac();
}

// pokebw2's Guru2Main_InitMsg
static void func_ov137_021dfff8(Guru2MainWork *g2m) {
    Guru2MsgWork *msg = &g2m->msg;
    int i;

    LoadSysMsgBox(1, 1, 15, 0, HEAPID_GURU2);
    func_02021640(g2m->fade, ARCID_WINFRAME, 3, HEAPID_GURU2, 0, 0x20, 14 * 16);
    func_02021640(g2m->fade, ARCID_FONT, 5, HEAPID_GURU2, 0, 0x20, 15 * 16);
    msg->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_GURU2);
    msg->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS, HEAPID_GURU2);
    msg->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_GURU2);
    msg->msgWindow = BmpWin_CreateDynamic(1, 2, 19, 26, 4, 15, TRUE);
    msg->strbuf = GFL_StrBufCreate(0x100, HEAPID_GURU2);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        g2m->names[i] = GFL_StrBufCreate(8, HEAPID_GURU2);
    }
    PrintWindow_Init(&msg->msgPrint, msg->msgWindow);
    msg->queue = func_0201c440(HEAPID_GURU2);
    func_ov137_021e0338(g2m, g2m->g2p->memberCount);
}

void func_ov137_021e00d4(Guru2MainWork *g2m) {
    int i;
    BmpWin *window;
    Guru2MsgWork *msg = &g2m->msg;

    window = msg->msgWindow;
    func_020456f8(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    BmpWin_Free(msg->msgWindow);
    func_ov137_021e03a0(g2m);
    GFL_MsgDataFree(msg->msgData);
    GFL_WordSetSystemFree(msg->wordSet);
    GFL_FontFree(msg->font);
    GFL_StrBufFree(msg->strbuf);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        GFL_StrBufFree(g2m->names[i]);
    }
    BmpWin_FreeAllocator();
}

void func_ov137_021e0130(Guru2MainWork *g2m, u32 messageId) {
    Guru2MsgWork *msg = &g2m->msg;
    BmpWin *window = msg->msgWindow;

    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    BmpWin_DrawFrame(window, 2, 1, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_MsgDataLoadStrbuf(msg->msgData, messageId, msg->strbuf);
    PrintWindow_Print(&msg->msgPrint, msg->queue, 0, 0, msg->strbuf, msg->font, 0x440);
    BmpWin_Transfer(window);
}

void func_ov137_021e01b0(Guru2MainWork *g2m, u32 messageId, UnionAppMember *member) {
    Guru2MsgWork *msg = &g2m->msg;
    BmpWin *window = msg->msgWindow;
    UnionApp *app = func_ov137_021deb10(g2m);
    StrBuf *strbuf;

    copyVarForText(msg->wordSet, 1, &func_ov022_02184180(app, (u8)func_02039338())->info);
    copyVarForText(msg->wordSet, 2, &member->info);
    strbuf = GFL_StrBufCreate(0x100, HEAPID_GURU2);
    GFL_MsgDataLoadStrbuf(msg->msgData, messageId, strbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, strbuf);
    GFL_StrBufFree(strbuf);
    BmpWin_DrawFrame(window, 2, 1, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    PrintWindow_Print(&msg->msgPrint, msg->queue, 0, 0, msg->strbuf, msg->font, 0x440);
    BmpWin_Transfer(window);
}

void func_ov137_021e026c(Guru2MainWork *g2m, u32 messageId, u32 item) {
    Guru2MsgWork *msg = &g2m->msg;
    BmpWin *window = msg->msgWindow;
    StrBuf *strbuf;

    loadItemNameToStrbuf(msg->wordSet, 0, item);
    strbuf = GFL_StrBufCreate(0x100, HEAPID_GURU2);
    GFL_MsgDataLoadStrbuf(msg->msgData, messageId, strbuf);
    GFL_WordSetFormatStrbuf(msg->wordSet, msg->strbuf, strbuf);
    GFL_StrBufFree(strbuf);
    BmpWin_DrawFrame(window, 2, 1, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    PrintWindow_Print(&msg->msgPrint, msg->queue, 0, 0, msg->strbuf, msg->font, 0x440);
    BmpWin_Transfer(window);
}





// pokebw2's Guru2Main_ClearMsg
static void func_ov137_021e0304(Guru2MainWork *g2m) {
    BmpWin *window = g2m->msg.msgWindow;

    func_0201f7a8(window, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    BmpWin_Transfer(window);
}

// pokebw2's Guru2Main_CreateNameWindows
static void func_ov137_021e0338(Guru2MainWork *g2m, int count) {
    int i;
    Guru2MsgWork *msg = &g2m->msg;
    const Guru2NameWindowData *data = data_ov137_021e1e30[count];

    msg->nameWindowCount = count;
    for (i = 0; i < count; i++) {
        msg->nameWindows[i] = BmpWin_CreateDynamic(data[i].bg, data[i].x, data[i].y, data[i].width, data[i].height,
                                                   data[i].palette, TRUE);
        PrintWindow_Init(&msg->namePrints[i], msg->nameWindows[i]);
    }
}

void func_ov137_021e03a0(Guru2MainWork *g2m) {
    int i;
    BmpWin *window;
    Guru2MsgWork *msg = &g2m->msg;

    for (i = 0; i < msg->nameWindowCount; i++) {
        window = msg->nameWindows[i];
        func_020456f8(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        BmpWin_Free(msg->nameWindows[i]);
    }
}

void func_ov137_021e03d8(Guru2MainWork *g2m, StrBuf *name, int index, u32 color) {
    Guru2MsgWork *msg = &g2m->msg;
    BmpWin *window = msg->nameWindows[index];

    BmpWin_DrawFrame(window, 2, 1, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    PrintWindow_Print(&msg->namePrints[index], msg->queue, 4, 1, name, msg->font, color);
    BmpWin_Transfer(window);
}



// Prints a member's name in a name window, the player's own in another color
// pokebw2's Guru2Main_PrintName
static void func_ov137_021e0450(Guru2MainWork *g2m, StrBuf *name, int index, int netId) {
    u32 color = COLOR_NAME;

    if (netId == func_02039338()) {
        color = COLOR_NAME_MINE;
    }
    func_ov137_021e03d8(g2m, name, index, color);
}

// pokebw2's Guru2Main_ClearName
static void func_ov137_021e0478(Guru2MainWork *g2m, int index) {
    BmpWin *window = g2m->msg.nameWindows[index];

    func_0201f7a8(window, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    BmpWin_Transfer(window);
}

// pokebw2's Guru2Main_FreeCamera
static void func_ov137_021e04b4(Guru2MainWork *g2m) {
    GFL_G3DCameraFree(g2m->camera.camera);
}

// pokebw2's Guru2Main_IsButtonTouched
static BOOL func_ov137_021e04c4(Guru2MainWork *g2m) {
    if (func_02035c08(data_ov137_021e1cb0) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Guru2Main_InitDisc
static void func_ov137_021e04e0(Guru2MainWork *g2m) {
    int count = g2m->g2p->memberCount;
    Guru2Disc *disc = &g2m->disc;

    disc->scene = GFL_G3DMgrNewScene(g2m->g3dManager, &data_ov137_021e20ec[count - 2]);
    disc->angle = 0;
    disc->angleOffset = data_ov137_021e1de8[count];
    VEC_Set(&disc->scale, FX32_ONE, FX32_ONE, FX32_ONE);
    disc->rotX = 0;
    disc->rotY = 0;
    disc->rotZ = 0;
    VEC_Set(&disc->pos, 0, FX32_CONST(-36), 0);
    disc->drawPos = disc->pos;
}

// pokebw2's Guru2Main_FreeDisc
static void func_ov137_021e0554(Guru2MainWork *g2m) {
    GFL_G3DMgrDeleteScene(g2m->g3dManager, g2m->disc.scene);
}

// pokebw2's Guru2Main_UpdateDisc
static void func_ov137_021e0570(Guru2MainWork *g2m) {
    Guru2Disc *disc = &g2m->disc;
    fx32 angle = g2m->gameData.discAngle;

    func_ov137_021e15dc(&angle, disc->baseAngle);
    func_ov137_021e15dc(&angle, disc->angleOffset);
    disc->rotY = (360 - angle / FX32_ONE) % 360;
    disc->drawPos.x = disc->pos.x + disc->offset.x + disc->shake.x;
    disc->drawPos.y = disc->pos.y + disc->offset.y + disc->shake.y;
    disc->drawPos.z = disc->pos.z + disc->offset.z + disc->shake.z;
}

// pokebw2's Guru2Main_DrawDisc
static void func_ov137_021e05d4(Guru2MainWork *g2m) {
    Guru2Disc *disc = &g2m->disc;
    G3DActor *actor = GFL_G3DMgrGetActor(g2m->g3dManager, disc->scene);
    SRTMatrix srt;

    VEC_Set(&srt.translation, disc->drawPos.x, disc->drawPos.y, disc->drawPos.z);
    VEC_Set(&srt.scale, disc->scale.x, disc->scale.y, disc->scale.z);
    MAT3_RotationEulerZYX(disc->rotX, DEG_TO_IDX(1) * disc->rotY, disc->rotZ, &srt.rotation);
    GFL_G3DSysDrawObj(actor, &srt);
}

// pokebw2's Guru2Disc_Turn
static void func_ov137_021e0634(Guru2Disc *disc, fx32 angle) {
    func_ov137_021e15dc(&disc->angle, angle);
}

// The model behind the disc
// pokebw2's Guru2Main_InitBG3D
static void func_ov137_021e0640(Guru2MainWork *g2m) {
    g2m->bgScene = GFL_G3DMgrNewScene(g2m->g3dManager, &data_ov137_021e1d80);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 0, FALSE);
}

// pokebw2's Guru2Main_FreeBG3D
static void func_ov137_021e0678(Guru2MainWork *g2m) {
    GFL_G3DMgrDeleteScene(g2m->g3dManager, g2m->bgScene);
}

// pokebw2's Guru2Main_DrawBG3D
static void func_ov137_021e0698(Guru2MainWork *g2m) {
    G3DActor *actor = GFL_G3DMgrGetActor(g2m->g3dManager, g2m->bgScene);
    SRTMatrix srt;

    VEC_Set(&srt.translation, 0, 0, 0);
    VEC_Set(&srt.scale, FX32_ONE, FX32_ONE, FX32_ONE);
    MAT3_RotationEulerZYX(0, 0, 0, &srt.rotation);
    GFL_G3DSysDrawObj(actor, &srt);
}

// pokebw2's sBonusAreas4
static const Guru2BonusArea data_ov137_021e1ca8[] = {
    { 37, 142 },
    { 215, 323 },
};

// pokebw2's sBonusAreas5
static const Guru2BonusArea data_ov137_021e1c98[] = {
    { 0, 215 },
    { 323, 359 },
};

// pokebw2's sBonusAreas3
static const Guru2BonusArea data_ov137_021e1ca0[] = {
    { 0, 198 },
    { 342, 359 },
};

// pokebw2's sBonusAreas
static const Guru2BonusAreaList data_ov137_021e1f04[GURU2_MEMBER_MAX + 1] = {
    { 0, NULL },
    { 0, NULL },
    { NELEMS(data_ov137_021e1c94), data_ov137_021e1c94 },
    { NELEMS(data_ov137_021e1ca0), data_ov137_021e1ca0 },
    { NELEMS(data_ov137_021e1ca8), data_ov137_021e1ca8 },
    { NELEMS(data_ov137_021e1c98), data_ov137_021e1c98 },
};

// pokebw2's sCursorScenes
static const G3DSceneSetup data_ov137_021e212c[GURU2_MEMBER_MAX] = {
    { data_ov137_021e1db8, NELEMS(data_ov137_021e1db8), data_ov137_021e1d50, NELEMS(data_ov137_021e1d50) },
    { data_ov137_021e1e48, NELEMS(data_ov137_021e1e48), data_ov137_021e1d50, NELEMS(data_ov137_021e1d50) },
    { data_ov137_021e1e00, NELEMS(data_ov137_021e1e00), data_ov137_021e1d50, NELEMS(data_ov137_021e1d50) },
    { data_ov137_021e1e18, NELEMS(data_ov137_021e1e18), data_ov137_021e1d50, NELEMS(data_ov137_021e1d50) },
    { data_ov137_021e1dd0, NELEMS(data_ov137_021e1dd0), data_ov137_021e1d50, NELEMS(data_ov137_021e1d50) },
};

// The Eggs' scene and their shadows'
// pokebw2's sEggScenes
static const G3DSceneSetup data_ov137_021e1e60[] = {
    { data_ov137_021e1cd0, NELEMS(data_ov137_021e1cd0), data_ov137_021e1d30, NELEMS(data_ov137_021e1d30) },
    { data_ov137_021e1cf4, NELEMS(data_ov137_021e1cf4), data_ov137_021e1d40, NELEMS(data_ov137_021e1d40) },
};

// pokebw2's Guru2Main_InitObjects
static void func_ov137_021e06e0(Guru2MainWork *g2m) {
    Guru2Objects *objects = &g2m->objects;
    int i;

    objects->eggScene = GFL_G3DMgrNewScene(g2m->g3dManager, &data_ov137_021e1e60[0]);
    objects->shadowScene = GFL_G3DMgrNewScene(g2m->g3dManager, &data_ov137_021e1e60[1]);
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        objects->cursorScenes[i] = GFL_G3DMgrNewScene(g2m->g3dManager, &data_ov137_021e212c[i]);
    }
}

// pokebw2's Guru2Main_FreeObjects
static void func_ov137_021e073c(Guru2MainWork *g2m) {
    int i;
    Guru2Objects *objects = &g2m->objects;

    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        GFL_G3DMgrDeleteScene(g2m->g3dManager, objects->cursorScenes[i]);
    }
    GFL_G3DMgrDeleteScene(g2m->g3dManager, objects->eggScene);
    GFL_G3DMgrDeleteScene(g2m->g3dManager, objects->shadowScene);
}

// pokebw2's Guru2Main_ShowEgg
static void func_ov137_021e078c(Guru2MainWork *g2m, Guru2Egg *egg) {
    egg->visible = TRUE;
    func_ov137_021e0b14(g2m, egg);
    func_ov137_021e0ba4(g2m, egg);
}

// pokebw2's Guru2Main_UpdateObjects
static void func_ov137_021e07a4(Guru2MainWork *g2m) {
    int i = 0;
    int count = g2m->playerCount;
    Guru2Objects *objects = &g2m->objects;

    for (; i < count; i++) {
        if (objects->eggs[i].active) {
            func_ov137_021e09c0(&objects->eggs[i], &g2m->disc.offset, g2m->gameData.eggAngles[i] << FX32_SHIFT,
                            g2m->gameData.jumpY);
        }
        if (objects->shadows[i].active) {
            func_ov137_021e0b38(g2m, &objects->shadows[i], g2m->gameData.jumpY);
        }
        if (objects->cursors[i].active) {
            func_ov137_021e0bc0(g2m, &objects->cursors[i], i);
        }
    }
}

// pokebw2's Guru2Main_DrawObjects
static void func_ov137_021e0848(Guru2MainWork *g2m) {
    int i = 0;
    int count = g2m->playerCount;
    Guru2Objects *objects = &g2m->objects;
    Guru2Egg *egg;
    Guru2EggShadow *shadow = objects->shadows;
    Guru2EggCursor *cursor = objects->cursors;

    egg = objects->eggs;
    for (; i < count; egg++, shadow++, cursor++, i++) {
        if (egg->visible) {
            G3DActor *actor = GFL_G3DMgrGetActor(g2m->g3dManager, objects->eggScene);
            SRTMatrix srt;

            VEC_Set(&srt.translation, egg->pos.x, egg->pos.y, egg->pos.z);
            VEC_Set(&srt.scale, egg->scale.x, egg->scale.y, egg->scale.z);
            MAT3_RotationEulerZYX(egg->rotX, egg->rotY, egg->rotZ, &srt.rotation);
            GFL_G3DSysDrawObj(actor, &srt);
        }
        if (shadow->active) {
            G3DActor *actor = GFL_G3DMgrGetActor(g2m->g3dManager, objects->shadowScene);
            SRTMatrix srt;

            VEC_Set(&srt.translation, shadow->pos.x, shadow->pos.y, shadow->pos.z);
            VEC_Set(&srt.scale, shadow->scale.x, shadow->scale.y, shadow->scale.z);
            MAT3_RotationEulerZYX(shadow->rotX, shadow->rotY, shadow->rotZ, &srt.rotation);
            GFL_G3DSysDrawObj(actor, &srt);
        }
        if (cursor->active) {
            G3DActor *actor = GFL_G3DMgrGetActor(g2m->g3dManager, objects->cursorScenes[i]);
            SRTMatrix srt;

            VEC_Set(&srt.translation, cursor->pos.x, cursor->pos.y, cursor->pos.z);
            VEC_Set(&srt.scale, FX32_ONE, FX32_ONE, FX32_ONE);
            MTX_Identity33_(&srt.rotation);
            GFL_G3DSysDrawObj(actor, &srt);
        }
    }
}

// pokebw2's Guru2Main_TurnEggs
static void func_ov137_021e0980(Guru2MainWork *g2m, fx32 angle) {
    int i = 0;
    int count = g2m->playerCount;
    Guru2Objects *objects = &g2m->objects;

    for (; i < count; i++) {
        if (objects->eggs[i].active) {
            func_ov137_021e15dc(&objects->eggs[i].angle, angle);
        }
    }
}

// Places an Egg on the disc at an angle
// pokebw2's Guru2Egg_Update
static void func_ov137_021e09c0(Guru2Egg *egg, const VecFx32 *offset, fx32 angle, fx32 jumpY) {
    u16 degrees;

    func_ov137_021e15dc(&angle, egg->angleOffset);
    degrees = angle / FX32_ONE;
    egg->pos.x = egg->offset.x + offset->x + EGG_RADIUS * func_0203f63c(degrees);
    egg->pos.y = egg->offset.y - FX32_CONST(6) + offset->y + jumpY;
    egg->pos.z = egg->offset.z + offset->z + EGG_RADIUS * func_0203f5fc(degrees);
    egg->rotX = egg->rotation.x / FX32_ONE;
    egg->rotY = egg->rotation.y / FX32_ONE;
    egg->rotZ = egg->rotation.z / FX32_ONE;
}

// The Egg nearest the player's position, going back against the disc's turn
// pokebw2's Guru2Main_FindFrontEgg
static Guru2Egg *func_ov137_021e0a44(Guru2MainWork *g2m) {
    int distances[GURU2_MEMBER_MAX];
    int order[GURU2_MEMBER_MAX];
    int index;
    int count = g2m->playerCount;
    Guru2Objects *objects = &g2m->objects;
    int i = 0;
    int j;
    int nearest;
    fx32 target = data_ov137_021e1fdc[count][g2m->position] << FX32_SHIFT;

    do {
        order[i] = i;
        distances[i] = 0xffff;
        i++;
    } while (i < count);

    for (i = 0; i < count; i++) {
        fx32 distance = target - objects->eggs[i].angle;

        if (distance < 0) {
            distance = target + FULL_TURN - objects->eggs[i].angle;
        }
        distances[i] = distance / FX32_ONE;
    }

    for (i = 1; i < count; i++) {
        index = order[i];
        nearest = distances[index];

        for (j = i - 1; j >= 0 && distances[order[j]] > nearest; j--) {
            order[j + 1] = order[j];
        }
        order[j + 1] = index;
    }
    return &objects->eggs[order[0]];
}

// pokebw2's Guru2Main_AddEggShadow
static void func_ov137_021e0b14(Guru2MainWork *g2m, Guru2Egg *egg) {
    Guru2EggShadow *shadow = &g2m->objects.shadows[egg->position];

    shadow->active = TRUE;
    shadow->egg = egg;
    func_ov137_021e0b38(g2m, shadow, 0);
}

// The shadow stays on the disc under its Egg, smaller the higher the Egg is
// pokebw2's Guru2Main_UpdateEggShadow
static void func_ov137_021e0b38(Guru2MainWork *g2m, Guru2EggShadow *shadow, fx32 jumpY) {
    fx32 height;
    fx32 shrink;

    shadow->rotX = 0;
    shadow->rotY = 0;
    shadow->rotZ = 0;
    shadow->pos = shadow->egg->pos;
    shadow->pos.x -= shadow->egg->offset.x;
    shadow->pos.y -= shadow->egg->offset.y + jumpY;
    shadow->pos.z -= shadow->egg->offset.z;
    shadow->pos.y += FX32_CONST(2);

    height = shadow->egg->offset.y + jumpY;
    if (height < 0) {
        height = -height;
    }
    shrink = height / 128;
    if (shrink > FX32_CONST(0.5)) {
        shrink = FX32_CONST(0.5);
    }
    shadow->scale.x = FX32_ONE - shrink;
    shadow->scale.y = FX32_ONE - shrink;
    shadow->scale.z = FX32_ONE - shrink;
}

// pokebw2's Guru2Main_AddEggCursor
static void func_ov137_021e0ba4(Guru2MainWork *g2m, Guru2Egg *egg) {
    Guru2EggCursor *cursor = &g2m->objects.cursors[egg->position];

    cursor->active = TRUE;
    cursor->egg = egg;
}

// pokebw2's Guru2Main_UpdateEggCursor
static void func_ov137_021e0bc0(Guru2MainWork *g2m, Guru2EggCursor *cursor, int index) {
    G3DActor *actor = GFL_G3DMgrGetActor(g2m->g3dManager, g2m->objects.cursorScenes[index]);

    cursor->pos = cursor->egg->pos;
    cursor->pos.y += FX32_CONST(20);
    GFL_G3DActorStepAnmFrameLoop(actor, 0, FX32_ONE);
}

// Sets up a member's Egg and starts its drop onto the disc
static void func_ov137_021e0c08(Guru2MainWork *g2m, int netId, int position, int index, int angle, int angleOffset,
                             Guru2Egg *egg) {
    Guru2EggFallTask *task;

    egg->netId = netId;
    egg->position = position;
    egg->index = index;
    egg->startPosition = position;
    egg->angle = angle << FX32_SHIFT;
    egg->angleOffset = angleOffset << FX32_SHIFT;
    egg->active = TRUE;
    egg->landed = FALSE;

    task = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_GURU2), sizeof(Guru2EggFallTask), FALSE, "guru2_main.c", 4878);
    MI_CpuFill8(task, 0, sizeof(Guru2EggFallTask));
    task->g2m = g2m;
    task->egg = egg;
    GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e0c70, task, 0);
}

// pokebw2's Guru2EggFallTask_Main
static void func_ov137_021e0c70(TCB *tcb, void *data) {
    Guru2EggFallTask *task = data;
    int result;

    do {
        result = data_ov137_021e1cb8[task->seq](task);
    } while (result == SEQ_RESULT_CONTINUE);
    if (result == SEQ_RESULT_END) {
        task->egg->landed = TRUE;
        GFL_HeapFree(task);
        GFL_TCBRemove(tcb);
    }
}

// pokebw2's Guru2EggFallTask_Init
static int func_ov137_021e0ca4(Guru2EggFallTask *task) {
    Guru2Egg *egg = task->egg;

    egg->scale.x = FX32_ONE;
    egg->scale.y = FX32_ONE;
    egg->scale.z = FX32_ONE;
    egg->offset.y = FX32_CONST(192);
    func_ov137_021e078c(task->g2m, egg);
    task->speed = FX32_CONST(8);
    task->seq = 1;
    return SEQ_RESULT_WAIT;
}

// pokebw2's Guru2EggFallTask_Fall
static int func_ov137_021e0ccc(Guru2EggFallTask *task) {
    Guru2Egg *egg = task->egg;

    egg->offset.y -= task->speed;
    if (!task->sePlayed && egg->offset.y <= FX32_CONST(104)) {
        task->sePlayed = TRUE;
        GFL_SndSEPlay(SEQ_SE_SYS_51);
    }
    if (egg->offset.y <= FX32_CONST(32)) {
        egg->offset.y = FX32_CONST(32);
        task->seq = 2;
    }
    return SEQ_RESULT_WAIT;
}

// Slows the Egg down onto the disc, and shows its maintainer's name
// pokebw2's Guru2EggFallTask_Land
static int func_ov137_021e0d0c(Guru2EggFallTask *task) {
    Guru2Egg *egg = task->egg;

    if (task->speed > FX32_ONE) {
        task->speed -= FX32_ONE;
        if (task->speed < FX32_ONE) {
            task->speed = FX32_ONE;
        }
    }
    egg->offset.y -= task->speed;
    if (egg->offset.y <= 0) {
        egg->offset.y = 0;
        func_ov137_021e0450(task->g2m, task->g2m->names[egg->netId], egg->index, egg->netId);
        return SEQ_RESULT_END;
    }
    return SEQ_RESULT_WAIT;
}

// An Egg's jump, up and down over 15 frames each
// pokebw2's Guru2EggJumpTask_Update
static void func_ov137_021e0d50(Guru2MainWork *g2m, Guru2EggJumpTask *task) {
    Guru2Egg *egg = task->egg;

    switch (task->seq) {
    case 0:
        func_ov137_021e0f64(g2m, egg);
        task->speed = FX32_CONST(4);
        task->accel = -0x444;
        task->seq++;
    case 1:
        egg->jumpY += task->speed;
        task->speed += task->accel;
        task->frame++;
        if (task->frame >= 15) {
            task->frame = 0;
            task->speed = 0;
            if (task->count == 0) {
                task->count = 1;
            } else {
                egg->jumpY = 0;
                func_ov137_021e1090(g2m, egg);
                MI_CpuFill8(task, 0, sizeof(Guru2EggJumpTask));
            }
        }
        break;
    }
}

// pokebw2's Guru2EggJumpTask_Main
static void func_ov137_021e0dc0(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;
    int i;
    Guru2EggJumpTask *task = g2m->jumpTasks;

    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (task->active) {
            func_ov137_021e0d50(g2m, task);
        }
    }
}

// pokebw2's Guru2Main_InitEggJump
static void func_ov137_021e0de8(Guru2MainWork *g2m) {
    MI_CpuFill8(g2m->jumpTasks, 0, sizeof(g2m->jumpTasks));
    g2m->jumpTCB = GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e0dc0, g2m, 0x100);
}

// pokebw2's Guru2Main_FreeEggJump
static void func_ov137_021e0e1c(Guru2MainWork *g2m) {
    GFL_TCBRemove(g2m->jumpTCB);
}

// pokebw2's Guru2Main_StartEggJump
static void func_ov137_021e0e2c(Guru2MainWork *g2m, Guru2Egg *egg) {
    int i;
    Guru2EggJumpTask *task = g2m->jumpTasks;

    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (!task->active) {
            task->active = TRUE;
            task->egg = egg;
            return;
        }
    }
}

// pokebw2's Guru2Main_StartEggJumps
static void func_ov137_021e0e50(Guru2MainWork *g2m) {
    Guru2Objects *objects = &g2m->objects;
    int count = g2m->playerCount;
    int i = 0;

    do {
        if (objects->eggs[i].active) {
            func_ov137_021e0e2c(g2m, &objects->eggs[i]);
        }
        i++;
    } while (i < count);
}

// pokebw2's Guru2Main_IsEggJumping
static BOOL func_ov137_021e0e8c(Guru2MainWork *g2m) {
    int i;
    Guru2EggJumpTask *task = g2m->jumpTasks;

    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (task->active) {
            return TRUE;
        }
    }
    return FALSE;
}

// An Egg's spin as it jumps, which slows down each turn
// pokebw2's Guru2EggSpinTask_Update
static void func_ov137_021e0eac(Guru2EggSpinTask *task) {
    Guru2Egg *egg = task->egg;

    switch (task->seq) {
    case 0:
        task->speed = FX32_CONST(24);
        egg->rotation.y = 0;
        task->seq++;
    case 1:
        func_ov137_021e15dc(&egg->rotation.y, task->speed);
        if (egg->rotation.y < FX32_CONST(4)) {
            task->speed = (u32)task->speed >> 1;
            if (task->speed < FX32_CONST(4)) {
                egg->rotation.y = 0;
                MI_CpuFill8(task, 0, sizeof(Guru2EggSpinTask));
            }
        }
        break;
    }
}

// pokebw2's Guru2EggSpinTask_Main
static void func_ov137_021e0efc(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;
    int i;
    Guru2EggSpinTask *task = g2m->spinTasks;

    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (task->active) {
            func_ov137_021e0eac(task);
        }
    }
}

// pokebw2's Guru2Main_InitEggSpin
static void func_ov137_021e0f20(Guru2MainWork *g2m) {
    MI_CpuFill8(g2m->spinTasks, 0, sizeof(g2m->spinTasks));
    g2m->spinTCB = GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e0efc, g2m, 0x101);
}

// pokebw2's Guru2Main_FreeEggSpin
static void func_ov137_021e0f54(Guru2MainWork *g2m) {
    GFL_TCBRemove(g2m->spinTCB);
}

// Starts an Egg's spin, or starts it over
// pokebw2's Guru2Main_StartEggSpin
static void func_ov137_021e0f64(Guru2MainWork *g2m, Guru2Egg *egg) {
    Guru2EggSpinTask *tasks = g2m->spinTasks;
    int i;
    Guru2EggSpinTask *task;

    task = tasks;
    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (task->active == TRUE && task->egg == egg) {
            task->seq = 0;
            return;
        }
    }

    task = tasks;
    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (!task->active) {
            task->active = TRUE;
            task->egg = egg;
            return;
        }
    }
}

// An Egg's wobble after it lands, which halves each swing
// pokebw2's Guru2EggShakeTask_Update
static void func_ov137_021e0fac(Guru2EggShakeTask *task) {
    Guru2Egg *egg = task->egg;
    fx32 angle;
    fx32 rotation;

    switch (task->seq) {
    case 0:
        egg->rotation.z = 0;
        task->angle = 0;
        task->amplitude = FX32_CONST(30);
        task->speed = FX32_CONST(20);
        task->seq++;
    case 1:
        rotation = task->amplitude / FX32_ONE * func_0203f5fc(task->angle / FX32_ONE);
        egg->rotation.z = 0;
        func_ov137_021e15dc(&egg->rotation.z, rotation);
        angle = task->angle;
        func_ov137_021e15dc(&task->angle, task->speed);
        if (task->angle < angle) {
            task->amplitude = (u32)task->amplitude >> 1;
            if (task->amplitude == 0) {
                egg->rotation.z = 0;
                MI_CpuFill8(task, 0, sizeof(Guru2EggShakeTask));
            }
        }
        break;
    }
}

// pokebw2's Guru2EggShakeTask_Main
static void func_ov137_021e1028(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;
    int i;
    Guru2EggShakeTask *task = g2m->shakeTasks;

    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (task->active) {
            func_ov137_021e0fac(task);
        }
    }
}

// pokebw2's Guru2Main_InitEggShake
static void func_ov137_021e104c(Guru2MainWork *g2m) {
    MI_CpuFill8(g2m->shakeTasks, 0, sizeof(g2m->shakeTasks));
    g2m->shakeTCB = GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e1028, g2m, 0x102);
}

// pokebw2's Guru2Main_FreeEggShake
static void func_ov137_021e1080(Guru2MainWork *g2m) {
    GFL_TCBRemove(g2m->shakeTCB);
}

// pokebw2's Guru2Main_StartEggShake
static void func_ov137_021e1090(Guru2MainWork *g2m, Guru2Egg *egg) {
    Guru2EggShakeTask *tasks = g2m->shakeTasks;
    int i;
    Guru2EggShakeTask *task;

    task = tasks;
    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (task->active == TRUE && task->egg == egg) {
            task->seq = 0;
            return;
        }
    }

    task = tasks;
    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        if (!task->active) {
            task->active = TRUE;
            task->egg = egg;
            return;
        }
    }
}

// The disc sinks, springs back up and throws the Eggs, then shakes until they have landed
// pokebw2's Guru2DiscBumpTask_Update
static void func_ov137_021e10d8(Guru2MainWork *g2m, Guru2DiscBumpTask *task) {
    Guru2Disc *disc = &g2m->disc;

    switch (task->seq) {
    case 0:
        break;
    case 1:
        task->shake = FX32_CONST(4);
        task->count = 0;
        task->seq++;
    case 2:
        disc->offset.y -= FX32_ONE;
        if (disc->offset.y <= FX32_CONST(-8)) {
            task->seq++;
        }
        break;
    case 3:
        disc->offset.y += FX32_CONST(2);
        if (disc->offset.y >= 0) {
            disc->offset.y = 0;
            func_ov137_021e0e50(g2m);
            task->jumping = TRUE;
            task->seq++;
            GFL_SndSEPlay(SEQ_SE_DANSA);
        }
        break;
    case 4:
        disc->shake.y = task->shake;
        task->shake = -task->shake;
        task->count++;
        if ((task->count & 3) == 0) {
            if (task->shake <= FX32_ONE) {
                disc->shake.y = 0;
                task->seq++;
            } else {
                task->shake >>= 1;
            }
        }
        break;
    case 5:
        if (!func_ov137_021e0e8c(g2m)) {
            task->active = FALSE;
            task->jumping = FALSE;
            task->seq = 0;
            GFL_SndSEPlay(SEQ_SE_SYS_83);
        }
        break;
    }
}

// pokebw2's Guru2DiscBumpTask_Main
static void func_ov137_021e11a0(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;

    func_ov137_021e10d8(g2m, &g2m->bumpTask);
}

// pokebw2's Guru2Main_InitDiscBump
static void func_ov137_021e11b4(Guru2MainWork *g2m) {
    MI_CpuFill8(&g2m->bumpTask, 0, sizeof(Guru2DiscBumpTask));
    g2m->bumpTCB = GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e11a0, g2m, 0xff);
}

// pokebw2's Guru2Main_FreeDiscBump
static void func_ov137_021e11e4(Guru2MainWork *g2m) {
    GFL_TCBRemove(g2m->bumpTCB);
}

// pokebw2's Guru2Main_StartDiscBump
static void func_ov137_021e11f4(Guru2MainWork *g2m) {
    Guru2DiscBumpTask *task = &g2m->bumpTask;

    task->active = TRUE;
    task->seq = 1;
}

// pokebw2's Guru2Main_AreEggsJumping
static BOOL func_ov137_021e1204(Guru2MainWork *g2m) {
    return g2m->bumpTask.jumping;
}

// pokebw2's Guru2Main_IsDiscBumping
static BOOL func_ov137_021e1210(Guru2MainWork *g2m) {
    return g2m->bumpTask.active;
}

// Plays the countdown's sounds while its actor's animation runs
// pokebw2's Guru2CountdownTask_Main
static void func_ov137_021e121c(TCB *tcb, void *data) {
    Guru2CountdownTask *task = data;

    switch (task->seq) {
    case 0:
        if (task->count < 4) {
            u32 se[] = { SEQ_SE_SYS_52, SEQ_SE_SYS_52, SEQ_SE_SYS_52, SEQ_SE_SYS_53 };

            task->wait--;
            if (task->wait <= 0) {
                task->wait = 30;
                GFL_SndSEPlay(se[task->count]);
                task->count++;
            }
        }
        func_0204b8d4(task->actor, FX32_CONST(1.5));
        if (!func_0204b98c(task->actor)) {
            task->seq++;
        }
        break;
    case 1:
        func_0204b3b4(task->actor);
        GFL_HeapFree(task);
        GFL_TCBRemove(tcb);
        break;
    }
}

// pokebw2's Guru2Main_StartCountdown
static void func_ov137_021e129c(Guru2MainWork *g2m) {
    Guru2CountdownTask *task;
    ClActorSetup setup;

    task = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_GURU2), sizeof(Guru2CountdownTask), FALSE, "guru2_main.c", 5638);
    MI_CpuFill8(task, 0, sizeof(Guru2CountdownTask));
    setup.x = 128;
    setup.y = 100;
    setup.sequence = 0;
    setup.bgPriority = 0;
    setup.priority = 0;
    task->actor = func_0204b294(g2m->clactUnit, g2m->chars, g2m->palette, g2m->cellAnims, &setup, 0, HEAPID_GURU2);
    func_0204b8d4(task->actor, FX32_ONE);
    task->g2m = g2m;
    GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e121c, task, 0);
}

// The button's push on the touch screen
// pokebw2's Guru2ButtonTask_Update
static void func_ov137_021e1320(Guru2MainWork *g2m, Guru2ButtonTask *task) {
    switch (task->seq) {
    case 0:
        break;
    case 1:
        func_ov137_021dfea0(g2m);
        GFL_SndSEPlay(SEQ_SE_SYS_54);
        task->frame = 0;
        task->seq++;
    case 2:
        task->frame++;
        if (task->frame >= 2) {
            task->frame = 0;
            task->seq++;
            func_ov137_021dfeb8(g2m);
        }
        break;
    case 3:
        task->frame++;
        if (task->frame >= 4) {
            func_ov137_021dfea0(g2m);
            task->frame = 0;
            task->seq++;
        }
        break;
    case 4:
        task->frame++;
        if (task->frame >= 2) {
            func_ov137_021dfeac(g2m);
            task->active = FALSE;
            task->seq = 0;
        }
        break;
    }
}

// pokebw2's Guru2ButtonTask_Main
static void func_ov137_021e13a8(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;

    func_ov137_021e1320(g2m, &g2m->buttonTask);
}

// pokebw2's Guru2Main_InitButton
static void func_ov137_021e13bc(Guru2MainWork *g2m) {
    MI_CpuFill8(&g2m->buttonTask, 0, sizeof(Guru2ButtonTask));
    g2m->buttonTCB = GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e13a8, g2m, 0x103);
}

// pokebw2's Guru2Main_FreeButton
static void func_ov137_021e13f0(Guru2MainWork *g2m) {
    GFL_TCBRemove(g2m->buttonTCB);
}

// pokebw2's Guru2Main_IsButtonPushed
static u32 func_ov137_021e1400(Guru2MainWork *g2m) {
    return g2m->buttonTask.active;
}

// pokebw2's Guru2Main_PushButton
static void func_ov137_021e140c(Guru2MainWork *g2m) {
    Guru2ButtonTask *task = &g2m->buttonTask;

    task->active = TRUE;
    task->seq = 1;
}

// An Egg hops on its bonus area, with a rest between the hops
// pokebw2's Guru2BonusTask_Update
static void func_ov137_021e141c(Guru2MainWork *g2m, Guru2BonusTask *task) {
    switch (task->seq) {
    case 0:
        break;
    case 1:
        task->egg->offset.y = data_ov137_021e2278[task->frame] << FX32_SHIFT;
        task->frame++;
        if (task->frame >= (int)NELEMS(data_ov137_021e2278)) {
            task->frame = 0;
            task->seq++;
        }
        break;
    case 2:
        task->wait++;
        if (task->wait >= 15) {
            task->wait = 0;
            task->seq = 1;
        }
        break;
    }
}

// pokebw2's Guru2BonusTask_Main
static void func_ov137_021e1468(TCB *tcb, void *data) {
    Guru2MainWork *g2m = data;
    int i;
    Guru2BonusTask *task = g2m->bonusTasks;

    for (i = 0; i < GURU2_MEMBER_MAX; i++, task++) {
        func_ov137_021e141c(g2m, task);
    }
}

// pokebw2's Guru2Main_InitBonus
static void func_ov137_021e1488(Guru2MainWork *g2m) {
    MI_CpuFill8(g2m->bonusTasks, 0, sizeof(g2m->bonusTasks));
    g2m->bonusTCB = GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e1468, g2m, 0x104);
}

// pokebw2's Guru2Main_FreeBonus
static void func_ov137_021e14bc(Guru2MainWork *g2m) {
    GFL_TCBRemove(g2m->bonusTCB);
}

// pokebw2's Guru2Main_StartBonus
static void func_ov137_021e14cc(Guru2MainWork *g2m, Guru2Egg *egg) {
    Guru2BonusTask *task = &g2m->bonusTasks[egg->netId];

    task->seq = 1;
    task->egg = egg;
}

// pokebw2's Guru2Main_StartBonuses
static void func_ov137_021e14e4(Guru2MainWork *g2m) {
    u32 bonusBits = g2m->bonusBits;
    int i = 0;
    int count = g2m->playerCount;
    Guru2Objects *objects = &g2m->objects;

    for (; i < count; i++) {
        if (objects->eggs[i].active && (bonusBits & (1 << objects->eggs[i].netId))) {
            func_ov137_021e14cc(g2m, &objects->eggs[i]);
        }
    }
}

// pokebw2's Guru2FadeTask_Main
static void func_ov137_021e1540(TCB *tcb, void *data) {
    Guru2FadeTask *task = data;
    BOOL done = FALSE;

    if (task->out == TRUE) {
        task->evy--;
        if (task->evy <= 0) {
            task->evy = 0;
            done = TRUE;
        }
    } else {
        task->evy++;
        if (task->evy >= 8) {
            task->evy = 8;
            done = TRUE;
        }
    }
    func_ov137_021dff20(task->g2m, task->evy);
    if (done == TRUE) {
        GFL_HeapFree(task);
        GFL_TCBRemove(tcb);
    }
}

// Lights the touch screen up for the game, or darkens it again
// pokebw2's Guru2Main_StartSubFade
static void func_ov137_021e1584(Guru2MainWork *g2m, int out) {
    int start[] = { 0, 8 };
    Guru2FadeTask *task =
        GFL_HeapAllocate(HEAPID_TAIL(HEAPID_GURU2), sizeof(Guru2FadeTask), FALSE, "guru2_main.c", 5992);

    task->g2m = g2m;
    task->out = out;
    task->evy = start[out];
    GFL_TCBMgrAddTask(g2m->tcbManager, func_ov137_021e1540, task, 0);
}

// Adds to an angle, which stays within a turn
// pokebw2's Guru2Main_AddAngle
static void func_ov137_021e15dc(fx32 *angle, fx32 addend) {
    *angle += addend;
    while (*angle < 0) {
        *angle += FULL_TURN;
    }
    *angle %= FULL_TURN;
}

// pokebw2's Guru2Main_Turn
static void func_ov137_021e1604(Guru2MainWork *g2m, fx32 angle) {
    func_ov137_021e0634(&g2m->disc, angle);
    func_ov137_021e0980(g2m, angle);
}

// Returns whether the game's time is up
// pokebw2's Guru2Main_CountGameFrame
static BOOL func_ov137_021e161c(Guru2MainWork *g2m) {
    g2m->gameFrame++;
    if (g2m->gameFrame < GAME_FRAMES) {
        return FALSE;
    }
    g2m->gameFrame = GAME_FRAMES;
    return TRUE;
}

// pokebw2's Guru2Main_LoadFile
static void *func_ov137_021e1634(Guru2MainWork *g2m, u32 fileId, BOOL head) {
    u32 size = GFL_ArcToolGetDataLength(g2m->arc, fileId);
    void *file;

    if (head == TRUE) {
        file = GFL_HeapAllocate(HEAPID_GURU2, size, FALSE, "guru2_main.c", 6090);
    } else {
        file = GFL_HeapAllocate(HEAPID_TAIL(HEAPID_GURU2), size, FALSE, "guru2_main.c", 6092);
    }
    GFL_ArcToolRead(g2m->arc, fileId, file);
    return file;
}

// GURU2_NET_CMD_7: someone pressed the button
// pokebw2's Guru2Main_OnButton
void func_ov137_021e168c(Guru2MainWork *g2m, u32 value) {
    if (value != 0) {
        g2m->buttonReceived = value;
    }
}

// GURU2_NET_CMD_8: the parent's Guru2GameData
// pokebw2's Guru2Main_OnGameData
void func_ov137_021e1694(Guru2MainWork *g2m, const void *data) {
    g2m->gameDataReceived = TRUE;
    g2m->gameData = *(const Guru2GameData *)data;
}

// pokebw2's Guru2Main_InitSeqWork
static void *func_ov137_021e16ac(Guru2MainWork *g2m, u32 size) {
    MI_CpuFill8(g2m->seqWork, 0, sizeof(g2m->seqWork));
    return g2m->seqWork;
}

// pokebw2's Guru2Main_GetSeqWork
static void *func_ov137_021e16c0(Guru2MainWork *g2m) {
    return g2m->seqWork;
}

// pokebw2's Guru2Main_HasSignal
static BOOL func_ov137_021e16c4(Guru2MainWork *g2m, u32 signal) {
    if (g2m->g2c->unk40 & signal) {
        return TRUE;
    }
    return FALSE;
}

// Whether every child of the receipt screen has joined this one
// pokebw2's Guru2Main_HaveChildrenJoined
static BOOL func_ov137_021e16d8(Guru2MainWork *g2m) {
    int count = 0;
    int max = g2m->g2p->memberCount - 1;
    u32 bits = g2m->g2c->unk42;

    while (bits != 0) {
        count += bits & 1;
        bits >>= 1;
    }
    return count >= max;
}

// pokebw2's Guru2Main_CountChildren
static int func_ov137_021e170c(Guru2MainWork *g2m) {
    int count = 0;
    u32 bits = g2m->g2c->unk42;

    while (bits != 0) {
        count += bits & 1;
        bits >>= 1;
    }
    return count;
}

// GURU2_NET_CMD_9: a member's position
// pokebw2's Guru2Main_OnPosition
void func_ov137_021e172c(Guru2MainWork *g2m, const void *data) {
    const Guru2PositionData *position = data;

    g2m->positions[position->position] = *position;
    if (position->netId == func_02039338()) {
        g2m->position = position->position;
    }
}

// GURU2_NET_CMD_10: the parent's player count, which this machine counts itself
// pokebw2's Guru2Main_OnPlayerCount
void func_ov137_021e1758(Guru2MainWork *g2m, u32 value) {
    g2m->playerCount = func_ov022_021841a4(func_ov137_021deb10(g2m));
}

// pokebw2's Guru2Main_IsMember
static BOOL func_ov137_021e1768(Guru2MainWork *g2m, int netId) {
    if (netId != 0 && !(g2m->g2c->unk42 & (1 << netId))) {
        return FALSE;
    }
    return TRUE;
}

// Replaces the Egg the player put in with the one of the given member
// pokebw2's Guru2Main_TradeEgg
static void func_ov137_021e1784(Guru2MainWork *g2m, int netId) {
    PokeParty *party = g2m->party;
    PokeParty *otherParty = func_ov137_021dce00(g2m->g2c, netId);
    u32 otherIndex = g2m->partyIndices[netId];
    PartyPkm *pkm = PokeParty_GetPkm(party, g2m->g2p->partyIndex);
    PartyPkm *otherPkm = PokeParty_GetPkm(otherParty, otherIndex);
    UnionApp *app = func_ov137_021deb10(g2m);

    PokeParty_SetupMetData(otherPkm, 6, &func_ov022_02184180(app, func_02039338())->info, 30002, HEAPID_GURU2);
    copyPartyPkm(otherPkm, pkm);
}

// How many members' parties have arrived
// pokebw2's Guru2Main_CountParties
static int func_ov137_021e17ec(Guru2MainWork *g2m) {
    int count = 0;
    u32 bits = g2m->g2c->blockBits;

    while (bits != 0) {
        count += bits & 1;
        bits >>= 1;
    }
    return count;
}

// GURU2_NET_CMD_11: the party slot a member trades
// pokebw2's Guru2Main_OnPartyIndex
void func_ov137_021e180c(Guru2MainWork *g2m, int netId, u32 value) {
    g2m->partyIndices[netId] = value;
}

// GURU2_NET_CMD_12: the parent's Guru2ResultData
// pokebw2's Guru2Main_OnResult
void func_ov137_021e1814(Guru2MainWork *g2m, const void *data) {
    g2m->result = *(const Guru2ResultData *)data;
    g2m->resultReceived = TRUE;
}

// Whether an Egg at this angle of the disc is over a bonus area
// pokebw2's Guru2Main_IsBonusAngle
static BOOL func_ov137_021e1830(Guru2MainWork *g2m, fx32 angle, int playerCount) {
    fx32 edge;
    u32 start;
    const Guru2BonusAreaList *list;
    int count;
    u32 end;
    const Guru2BonusArea *area;

    func_ov137_021e15dc(&angle, -g2m->disc.angle);
    func_ov137_021e15dc(&angle, -g2m->disc.angleOffset);
    edge = angle;
    func_ov137_021e15dc(&edge, FX32_CONST(-4));
    start = edge / FX32_ONE;
    edge = angle;
    func_ov137_021e15dc(&edge, FX32_CONST(5));
    end = edge / FX32_ONE;

    list = &data_ov137_021e1f04[playerCount];
    count = list->count;
    area = list->areas;
    for (; count != 0; area++, count--) {
        if ((start >= area->start && start <= area->end) || (end >= area->start && end <= area->end)) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's Guru2Main_PartyHasBadEgg
static BOOL func_ov137_021e18cc(PokeParty *party) {
    int count = PokeParty_GetPkmCount(party);
    int i;

    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);

        if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) && PokeParty_GetParam(pkm, PKM_PARAM_BAD_EGG, NULL)) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's Guru2Main_AnyPartyHasBadEgg
static BOOL func_ov137_021e1910(Guru2MainWork *g2m) {
    int i;

    if (func_ov137_021e18cc(g2m->party) == TRUE) {
        return TRUE;
    }
    for (i = 0; i < GURU2_MEMBER_MAX; i++) {
        if (func_ov137_021e1768(g2m, i) && func_ov137_021e18cc(func_ov137_021dce00(g2m->g2c, i)) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

// How many members have found no bad Egg
// pokebw2's Guru2Main_CountEggChecks
static int func_ov137_021e1954(Guru2MainWork *g2m) {
    int count = 0;
    u32 bits = g2m->g2c->unk4A;

    while (bits != 0) {
        count += bits & 1;
        bits >>= 1;
    }
    return count;
}

// pokebw2's Guru2Main_IsBadEggFound
static BOOL func_ov137_021e1974(Guru2MainWork *g2m) {
    if (g2m->g2c->unk48 != 0) {
        return TRUE;
    }
    return FALSE;
}

// Plays a jingle over the music
// pokebw2's Guru2Main_PlayME
static void func_ov137_021e1988(u32 me) {
    GFL_SndBGMSetPaused(TRUE);
    GFL_SndBGMPush();
    GFL_SndBGMPlay(me, 0xffff);
}

// Returns whether the jingle still plays, and brings the music back once it doesn't
// pokebw2's Guru2Main_IsMEPlaying
static BOOL func_ov137_021e19a4(void) {
    if (!GFL_SndBGMIsPlaying()) {
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
        return FALSE;
    }
    return TRUE;
}

// The parent turns the disc and the Eggs, and sends the others where they are
// pokebw2's Guru2Main_ParentTurn
static void func_ov137_021e19c0(Guru2MainWork *g2m, fx32 angle) {
    if (func_02039338() == 0) {
        Guru2GameData data;
        int i;

        func_ov137_021e1604(g2m, angle);
        data.discAngle = g2m->disc.angle;
        data.jumpY = g2m->objects.eggs[0].jumpY;
        for (i = 0; i < g2m->playerCount; i++) {
            data.eggAngles[i] = g2m->objects.eggs[i].angle / FX32_ONE;
        }
        func_ov137_021dcbc4(g2m->g2c, GURU2_NET_CMD_8, &data, sizeof(data));
    }
}
