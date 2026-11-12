// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_effvm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle effect VM, which runs the move and effect scripts: their commands (swan's data_ov094_02209e88) move the camera
// and the Pokémon, add cell actors and particles, change the background and play the sounds of each move's
// animation. The name is the ROM's string, from GFL_HeapAllocate's asserts. BtlvEffect_QueueCommands and data_ov094_02209e88
// are swan's names; the others are ours

#include "battle/btlv_effvm.h"
#include "field/field_camera.h"
#include "types.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv.h"
#include "battle/btlv_bg.h"
#include "battle/btlv_camera.h"
#include "battle/btlv_clact.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_gauge.h"
#include "battle/btlv_mcss.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nnsys/g3d.h"
#include "pml/item.h"
#include "system/palanm.h"
#include "system/vm.h"
#include "work_area.h"

#define BTLV_EFFECT_FIRST 0x231
struct BtlvEffvm {
    u32 flipSides : 1;         // bit 0   swaps the sides of the positions; nothing in the file sets it
    u32 cameraReset : 1;       // bit 1   the camera returns to its default with the effect's end
    u32 seDelayed : 1;         // bit 2   a delayed sound effect waits (func_ov094_021f9858)
    u32 seSliding : 1;         // bit 3   a sound player's parameter moves (func_ov094_021f9948)
    u32 bgMoved : 1;           // bit 4   BG 3 was loaded; the end restores its priority and offset
    u32 alphaBlend : 1;        // bit 5   the blend was changed; the end restores it
    u32 isEffect : 1;          // bit 6   the script is an effect's, from archive 66, rather than a move's, from 65
    u32 paused : 1;            // bit 7   until func_ov094_021f9814
    u32 windowTask : 1;        // bit 8   BtlvEffect_QueueCommands' window task runs
    u32 bgmLowered : 1;        // bit 9
    u32 bgmLowerRequested : 1; // bit 10
    u32 cryDelayed : 1;        // bit 11
    u32 sePlayed : 1;          // bit 12  the end stops the sounds
    u32 mcssScaled : 1;        // bit 13
    u32 unk0_14 : 1;           // bit 14  never used
    u32 cameraLocked : 1;      // bit 15  the camera commands do nothing
    u32 particleLoading : 1;   // bit 16  a particle resource uploads in the V-blank
    u32 substituteRestore : 1; // bit 17
    u32 unk0_18 : 1;           // bit 18  set for effect 0x27e, cleared for 0x291; while set the end keeps the Pokémon
    u32 paletteAnim : 1;       // bit 19
    u32 unk0_20 : 12;
    s32 value;                     // 0x004  the scripts' register, variable 17
    TCBManager *tcbManager;        // 0x008
    ParticleSystem *particles[16]; // 0x00c
    s32 particleIds[16];           // 0x04c  each particle system's file, -1 for none
    u32 emitterCounts[16];         // 0x08c  each particle resource's count of emitters
    s32 clactIndexes[4];           // 0x0cc  -1 for none
    int attacker;                  // 0x0dc  a position, 0xff for none
    int defender;                  // 0x0e0
    u32 callAttackers[16];         // 0x0e4  the call stack, by callDepth
    u32 callDefenders[16];         // 0x124
    void *callScripts[16];         // 0x164
    s32 callValues[16];            // 0x1a4
    s32 waitTimer;                 // 0x1e4
    void *script;                  // 0x1e8
    void *savedScript;             // 0x1ec  the move's script while the substitute's runs first
    u32 savedOffset;               // 0x1f0
    void *allocs[16];              // 0x1f4  the emitters' works, freed with the effect's end
    int allocCount;                // 0x234
    HeapID heapId;                 // 0x238
    BOOL ret;                      // 0x23c  what the commands return, TRUE to make VM_Run return
    u32 waitKind;                  // 0x240  what func_ov094_021fc744 waits for
    u32 unk244;                    // 0x244  1: func_ov094_021fdb20 resets the Pokémon with 6f10, else 6ecc
    BtlvEffvmParam param;          // 0x248
    u16 move;                      // 0x258  a move, or an effect from BTLV_EFFECT_FIRST
    u32 ballSource;                // 0x25c  whose ball the ball particles show: 8 for param's item, 9 the attacker's
    u32 callDepth;                 // 0x260
    int bgX;                       // 0x264  BG 3's offset before the effect
    int bgY;                       // 0x268
    s32 voices[6];                 // 0x26c  PokeVoice handles, -1 for none
    BOOL camSaved;                 // 0x284
    VecFx32 camPos;                // 0x288
    VecFx32 camTarget;             // 0x294
    u32 unk2A0;                    // 0x2a0  variable 0x37
    u32 lastMove;                  // 0x2a4  effects 0x261 and 0x262 don't repeat for the same attacker
    u32 lastAttacker;              // 0x2a8
};


// The emitter parameters the particle commands pass func_ov094_021fd1cc, 0x5c bytes
typedef struct {
    VM *vm;             // 0x00
    u32 from;           // 0x04  where the emitter starts: a position code, 8 for fromPos
    u32 to;             // 0x08  where it aims: a position code, 8 for toPos
    VecFx32 offset;     // 0x0c  added to both, turned toward the side
    u32 unk18;          // 0x18  written only
    fx32 motionHeight;  // 0x1c  of the arc of motion types other than 1 and 4
    u32 motionType;     // 0x20  0 for none, else the emitter moves from to to (func_ov094_021fd8cc)
    fx32 motionFrames;  // 0x24
    VecFx32 fromPos;    // 0x28
    VecFx32 toPos;      // 0x34
    u32 offsetMode;     // 0x40  1, 2: offset both positions, 3: the start only
    fx32 radiusScale;   // 0x44  scales the emitter's radius and length, 0 to keep them
    fx32 lifeTimeScale; // 0x48
    fx32 scaleScale;    // 0x4c
    fx32 cameraScale;   // 0x50  scales the camera position and the amplifiers
    fx32 turns;         // 0x54  of motion types 5 and 6
    BOOL flip;          // 0x58  mirrors the spin axis
} BtlvEffvmEmitParam;

// func_ov094_021fd1cc's motion of an emitter from its start to its target, moved by func_ov094_021fd8cc, 0x60
// bytes
typedef struct {
    MtxFx43 mtx;    // 0x00  turns the motion toward the target, from the start
    fx32 radius;    // 0x30  half the distance
    fx32 height;    // 0x34
    s32 angle;      // 0x38
    s32 angle2;     // 0x3c
    s32 angleStep;  // 0x40
    s32 angle2Step; // 0x44
    int frames;     // 0x48
    u32 type;       // 0x4c
    int wait;       // 0x50  type 3 waits longer each step
    fx32 waitAccum; // 0x54
    fx32 waitStep;  // 0x58
    u32 offsetMode; // 0x5c
} BtlvEffvmEmitterMotionWork;

// func_ov094_021f9858's delayed sound effect, 0x24 bytes
typedef struct {
    BtlvEffvm *wk; // 0x00
    u32 se;        // 0x04
    u32 player;    // 0x08
    int pan;       // 0x0c
    int wait;      // 0x10
    int arg4;      // 0x14
    int arg5;      // 0x18
    int vol;       // 0x1c
    int pitch;     // 0x20
} BtlvEffvmSeWork;

// func_ov094_021f9948's move of a sound player's parameter, 0x38 bytes
typedef struct {
    BtlvEffvm *wk;     // 0x00
    u32 player;        // 0x04
    u32 type;          // 0x08  0: once, 1: back and forth
    u32 param;         // 0x0c  0, 1 or 2: which parameter
    int start;         // 0x10
    int end;           // 0x14
    fx32 value;        // 0x18
    fx32 step;         // 0x1c
    int delay;         // 0x20
    int frames;        // 0x24
    int framesReset;   // 0x28
    int stepWait;      // 0x2c
    int stepWaitReset; // 0x30
    int count;         // 0x34
} BtlvEffvmSeMoveWork;

// func_ov094_021fa12c's work for loading a particle resource in the V-blank, 0xc bytes
typedef struct {
    BtlvEffvm *wk;            // 0x0
    ParticleSystem *particle; // 0x4
    void *resource;           // 0x8
} BtlvEffvmParticleVBlankWork;

// func_ov094_021fdfa0's emitter that circles a point, moved by func_ov094_021fda28, 0x40 bytes (line
// 7544)
typedef struct {
    u32 type;        // 0x00  0, 1: around the attacker, 2, 3: the defender, 4, 5: the origin; bit 0: clockwise
    VecFx32 center;  // 0x04
    fx32 radiusX;    // 0x10
    fx32 radiusZ;    // 0x14
    u32 angle;       // 0x18
    u32 angleStep;   // 0x1c
    int frames;      // 0x20
    int framesReset; // 0x24
    int wait;        // 0x28
    int waitReset;   // 0x2c
    u32 unk30;       // 0x30
    int delay;       // 0x34
    int delayReset;  // 0x38
    BOOL ortho;      // 0x3c
} BtlvEffvmEmitterCircleWork;

// BtlvEffect_QueueCommands' window work, stepped by func_ov094_021fe79c; 0x28 bytes, allocated at line 3659
typedef struct {
    BtlvEffvm *wk;  // 0x00
    u32 state;      // 0x04
    u32 dir;        // 0x08
    int winH;       // 0x0c
    int winV;       // 0x10
    int winInOut;   // 0x14
    u32 count;      // 0x18
    u32 wait;       // 0x1c
    u32 waitReload; // 0x20
    u32 keepWindow; // 0x24
} BtlvEffvmWindowWork;

// func_ov094_021fbfc0's delayed cry, played by func_ov094_021fe6a4; 0x20 bytes, allocated at line 4551
typedef struct {
    BtlvEffvm *wk; // 0x00
    int pos;       // 0x04
    int arg2;      // 0x08  the five arguments of func_ov094_02200218
    int arg3;      // 0x0c
    int arg4;      // 0x10
    int arg5;      // 0x14
    int arg6;      // 0x18
    int delay;     // 0x1c
} BtlvEffvmCryWork;

// The work of func_ov094_021fe8d8, which waits for the cries; 4 bytes, allocated at lines 4540 and 8044
typedef struct {
    BtlvEffvm *wk; // 0x0
} BtlvEffvmCryWaitWork;

// func_ov094_021fc3d8's work, stepped by func_ov094_021fe930; 0xc bytes, allocated at line 4883
typedef struct {
    BtlvEffvm *wk; // 0x0
    u32 side;      // 0x4
    u32 doneMask;  // 0x8
} BtlvEffvmSideWork;

// func_ov094_021fe410's fade of the BGM's volume, stepped by func_ov094_021fe768; 0x10 bytes, allocated at line
// 7799
typedef struct {
    BtlvEffvm *wk; // 0x0
    fx32 volume;   // 0x4
    fx32 target;   // 0x8
    fx32 step;     // 0xc
} BtlvEffvmBgmFadeWork;

static BOOL func_ov094_021f9ab8(VM *vm, void *env);
static BOOL func_ov094_021f9efc(VM *vm, void *env);
static BOOL func_ov094_021f9fc0(VM *vm, void *env);
static BOOL func_ov094_021fa048(VM *vm, void *env);
static BOOL func_ov094_021fa09c(VM *vm, void *env);
static BOOL func_ov094_021fa108(VM *vm, void *env);
static BOOL func_ov094_021fa12c(VM *vm, void *env);
static BOOL func_ov094_021fa288(VM *vm, void *env);
static BOOL func_ov094_021fa358(VM *vm, void *env);
static BOOL func_ov094_021fa458(VM *vm, void *env);
static BOOL func_ov094_021fa5d0(VM *vm, void *env);
static BOOL func_ov094_021fa758(VM *vm, void *env);
static BOOL func_ov094_021fa794(VM *vm, void *env);
static BOOL func_ov094_021fa884(VM *vm, void *env);
static BOOL func_ov094_021fa988(VM *vm, void *env);
static BOOL func_ov094_021fab0c(VM *vm, void *env);
static BOOL func_ov094_021fac94(VM *vm, void *env);
static BOOL func_ov094_021faca0(VM *vm, void *env);
static BOOL func_ov094_021facac(VM *vm, void *env);
static BOOL func_ov094_021fad2c(VM *vm, void *env);
static BOOL func_ov094_021fadc0(VM *vm, void *env);
static BOOL func_ov094_021fae48(VM *vm, void *env);
static BOOL func_ov094_021faecc(VM *vm, void *env);
static BOOL func_ov094_021faf44(VM *vm, void *env);
static BOOL func_ov094_021fafbc(VM *vm, void *env);
static BOOL func_ov094_021fb034(VM *vm, void *env);
static BOOL func_ov094_021fb09c(VM *vm, void *env);
static BOOL func_ov094_021fb0e8(VM *vm, void *env);
static BOOL func_ov094_021fb168(VM *vm, void *env);
static BOOL func_ov094_021fb1c4(VM *vm, void *env);
static BOOL func_ov094_021fb214(VM *vm, void *env);
static BOOL func_ov094_021fb298(VM *vm, void *env);
static BOOL func_ov094_021fb2d0(VM *vm, void *env);
static BOOL func_ov094_021fb328(VM *vm, void *env);
BOOL func_ov094_021fb390(VM *vm, void *env);
static BOOL func_ov094_021fb3c8(VM *vm, void *env);
static BOOL func_ov094_021fb3dc(VM *vm, void *env);
static BOOL func_ov094_021fb49c(VM *vm, void *env);
static BOOL func_ov094_021fb500(VM *vm, void *env);
static BOOL func_ov094_021fb524(VM *vm, void *env);
static BOOL func_ov094_021fb550(VM *vm, void *env);
static BOOL func_ov094_021fb574(VM *vm, void *env);
static BOOL func_ov094_021fb5d4(VM *vm, void *env);
static BOOL func_ov094_021fb624(VM *vm, void *env);
static BOOL func_ov094_021fb6f4(VM *vm, void *env);
static BOOL func_ov094_021fb790(VM *vm, void *env);
static BOOL func_ov094_021fb7f8(VM *vm, void *env);
static BOOL func_ov094_021fb864(VM *vm, void *env);
static BOOL func_ov094_021fb894(VM *vm, void *env);
static BOOL func_ov094_021fb8f4(VM *vm, void *env);
static BOOL func_ov094_021fb91c(VM *vm, void *env);
static BOOL func_ov094_021fba64(VM *vm, void *env);
static BOOL func_ov094_021fbad8(VM *vm, void *env);
static BOOL func_ov094_021fbaf8(VM *vm, void *env);
static BOOL func_ov094_021fbc78(VM *vm, void *env);
static BOOL func_ov094_021fbcec(VM *vm, void *env);
static BOOL func_ov094_021fbd10(VM *vm, void *env);
static BOOL func_ov094_021fbd34(VM *vm, void *env);
static BOOL func_ov094_021fbd44(VM *vm, void *env);
static BOOL func_ov094_021fbd88(VM *vm, void *env);
static void func_ov094_021fbdd4(VM *vm, int value, int target, int cond, int offset);
static BOOL func_ov094_021fbe28(VM *vm, void *env);
static BOOL func_ov094_021fbe60(VM *vm, void *env);
static BOOL func_ov094_021fbe74(VM *vm, void *env);
static BOOL func_ov094_021fbe90(VM *vm, void *env);
static BOOL func_ov094_021fbf48(VM *vm, void *env);
static BOOL func_ov094_021fbfa0(VM *vm, void *env);
static BOOL func_ov094_021fbfc0(VM *vm, void *env);
BOOL func_ov094_021fc134(VM *vm, void *env);
static BOOL func_ov094_021fc148(VM *vm, void *env);
static BOOL func_ov094_021fc20c(VM *vm, void *env);
static BOOL func_ov094_021fc2dc(VM *vm, void *env);
static BOOL func_ov094_021fc334(VM *vm, void *env);
static BOOL func_ov094_021fc354(VM *vm, void *env);
static BOOL func_ov094_021fc374(VM *vm, void *env);
static BOOL func_ov094_021fc3d8(VM *vm, void *env);
static BOOL func_ov094_021fc434(VM *vm, void *env);
static BOOL func_ov094_021fc448(VM *vm, void *env);
static BOOL func_ov094_021fc744(VM *vm, void *env);
static BOOL func_ov094_021fc9d0(VM *vm, void *env);
static BOOL func_ov094_021fc9e4(VM *vm, void *env);
static BOOL func_ov094_021fc9f4(VM *vm, void *env);
static int func_ov094_021fca04(BtlvEffvm *wk, int code, int *positions);
static int func_ov094_021fcd14(BtlvEffvm *wk, int code, int *positions);
static int func_ov094_021fcf2c(VM *vm, int code);
static int func_ov094_021fd04c(BtlvEffvm *wk, int pos);
static int func_ov094_021fd070(VM *vm, int pos);
static int func_ov094_021fd088(VM *vm, int *pos);
static BOOL func_ov094_021fd15c(BtlvEffvm *wk, u32 id, int *index);
static void func_ov094_021fd19c(BtlvEffvm *wk, int index, void *resource);
static int func_ov094_021fd1a8(BtlvEffvm *wk, u32 id);
static u32 func_ov094_021fd1c0(BtlvEffvm *wk, int index);
static void func_ov094_021fd1cc(SPLEmitter *emitter);
static void func_ov094_021fd8cc(SPLEmitter *emitter, u32 type);
static void func_ov094_021fd9f0(SPLEmitter *emitter);
static void func_ov094_021fda28(SPLEmitter *emitter, u32 type);
static void func_ov094_021fdb08(ParticleSystem *particle);
static void func_ov094_021fdb20(BtlvEffvm *wk);
static void func_ov094_021fdb40(u32 se, u32 player, int pan, int arg3, int arg4, int vol, int pitch);
static u32 func_ov094_021fdbb0(BtlvEffvm *wk, u32 kind);
static void func_ov094_021fdd60(BtlvEffvm *wk, BtlvEffvmEmitParam *param, int index, int resourceId);
static BOOL func_ov094_021fdfa0(VM *vm, BtlvEffvm *wk, BOOL ortho);
void func_ov094_021fe1b8(VecFx32 *pos, const VecFx32 *offset);
static void func_ov094_021fe254(const VecFx32 *src, const MtxFx44 *mtx, VecFx32 *dest, fx32 *w);
static u32 func_ov094_021fe38c(BtlvEffvm *wk, u32 id);
static void func_ov094_021fe410(BtlvEffvm *wk, fx32 start, fx32 end, int frames);
static int func_ov094_021fe4d0(BtlvEffvm *wk);
static void func_ov094_021fe4f8(BtlvEffvm *wk);
static void func_ov094_021fe520(BtlvEffvm *wk);
static void func_ov094_021fe54c(TCB *tcb, void *data);
static void func_ov094_021fe580(TCB *tcb);
static void func_ov094_021fe594(TCB *tcb, void *data);
static void func_ov094_021fe690(TCB *tcb);
static void func_ov094_021fe6a4(TCB *tcb, void *data);
static void func_ov094_021fe750(TCB *tcb);
static void func_ov094_021fe768(TCB *tcb, void *data);
static void func_ov094_021fe79c(TCB *tcb, void *data);
static void func_ov094_021fe8ac(TCB *tcb);
static void func_ov094_021fe8d8(TCB *tcb, void *data);
static void func_ov094_021fe930(TCB *tcb, void *data);
static void func_ov094_021fe9bc(TCB *tcb, void *data);
static void func_ov094_021fe9f0(fx32 scaleX, fx32 scaleY, int frames, int wait, int count);
static BOOL func_ov094_021feaa0(u32 move);

// The script commands, the VM setup, the camera positions and targets (.data) and the particles' camera vectors (the
// rest of them, and two tables, are defined before the functions that read them). This order lays them out as in the
// ROM
static s32 data_ov094_0220ad18[8] = { 0, 0, 17, 16, 0, 0, 15, 14 };
static VecFx32 data_ov094_0220add8[6] = { { FX32_CONST(1.7), FX32_CONST(6.2), FX32_CONST(20.3) },
                                          { 0xb994, FX32_CONST(6.95), 0x6e79 },
                                          { 0x5ca6, FX32_CONST(5.95), 0x13cc3 },
                                          { 0x6994, FX32_CONST(6.95), 0x6e79 },
                                          { FX32_CONST(6.7), FX32_CONST(6.2), FX32_CONST(20.3) },
                                          { 0x2994, FX32_CONST(6.95), 0x6e79 } };
static VecFx32 data_ov094_0220ace8[2] = { { 0x5ca6, FX32_CONST(5.95), 0x13cc3 }, { 0x6994, FX32_CONST(6.95), 0x6e79 } };
// pokebw2's MOVE_SCRCMD
static const VMCommand data_ov094_02209e88[] = {
    func_ov094_021f9ab8,
    func_ov094_021f9efc,
    func_ov094_021f9fc0,
    func_ov094_021fa048,
    func_ov094_021fa09c,
    func_ov094_021fa108,
    func_ov094_021fa12c,
    func_ov094_021fa288,
    func_ov094_021fa358,
    func_ov094_021fa458,
    func_ov094_021fa5d0,
    func_ov094_021fa758,
    func_ov094_021fa794,
    func_ov094_021fa884,
    func_ov094_021fa988,
    func_ov094_021fab0c,
    func_ov094_021fac94,
    func_ov094_021faca0,
    func_ov094_021facac,
    func_ov094_021fad2c,
    func_ov094_021fadc0,
    func_ov094_021fae48,
    func_ov094_021faecc,
    func_ov094_021faf44,
    func_ov094_021fafbc,
    func_ov094_021fb034,
    func_ov094_021fb09c,
    func_ov094_021fb0e8,
    func_ov094_021fb168,
    func_ov094_021fb1c4,
    func_ov094_021fb214,
    func_ov094_021fb298,
    func_ov094_021fb2d0,
    func_ov094_021fb328,
    func_ov094_021fb390,
    func_ov094_021fb3c8,
    func_ov094_021fb3dc,
    func_ov094_021fb49c,
    func_ov094_021fb500,
    func_ov094_021fb524,
    func_ov094_021fb550,
    func_ov094_021fb574,
    func_ov094_021fb5d4,
    func_ov094_021fb624,
    BtlvEffect_QueueCommands,
    func_ov094_021fb6f4,
    func_ov094_021fb790,
    func_ov094_021fb7f8,
    func_ov094_021fb864,
    func_ov094_021fb894,
    func_ov094_021fb8f4,
    func_ov094_021fb91c,
    func_ov094_021fba64,
    func_ov094_021fbad8,
    func_ov094_021fbaf8,
    func_ov094_021fbc78,
    func_ov094_021fbcec,
    func_ov094_021fbd10,
    func_ov094_021fbd34,
    func_ov094_021fbd44,
    func_ov094_021fbd88,
    func_ov094_021fbe28,
    func_ov094_021fbe60,
    func_ov094_021fbe74,
    func_ov094_021fbe90,
    func_ov094_021fbf48,
    func_ov094_021fbfa0,
    func_ov094_021fbfc0,
    func_ov094_021fc134,
    func_ov094_021fc148,
    func_ov094_021fc20c,
    func_ov094_021fc2dc,
    func_ov094_021fc334,
    func_ov094_021fc354,
    func_ov094_021fc374,
    func_ov094_021fc3d8,
    func_ov094_021fc434,
    func_ov094_021fc448,
};
static VecFx32 data_ov094_0220acc4 = { FX32_CONST(6.7), FX32_CONST(7.7), FX32_CONST(30.8) };
static VecFx32 data_ov094_0220acdc = { FX32_CONST(9.7), FX32_CONST(6.7), FX32_CONST(17.3) };
static VecFx32 data_ov094_0220ad00[2] = { { -0xe8d, FX32_CONST(1.85), 0x27f6 }, { -0x19f, FX32_CONST(2.85), -0xa654 } };
static VecFx32 data_ov094_0220aca0 = { FX32_CONST(8.7), FX32_CONST(7.7), FX32_CONST(23.8) };
static VecFx32 data_ov094_0220ae20[6] = {
#include "asset/btlv_effvm_data_ov094_0220ae20.inc"
};
static VecFx32 data_ov094_0220acac = { 0, FX32_CONST(3.6), FX32_CONST(13.5) };
static VecFx32 data_ov094_0220ad78[4] = { { FX32_CONST(2.7), FX32_CONST(5.2), FX32_CONST(17.3) },
                                          { 0x8994, FX32_CONST(6.95), 0x6e79 },
                                          { FX32_CONST(6.7), FX32_CONST(5.2), FX32_CONST(17.3) },
                                          { 0x4994, FX32_CONST(6.95), 0x6e79 } };
static VecFx32 data_ov094_0220ada8[4] = { { FX32_CONST(-4), FX32_CONST(1.1), 0 },
                                          { 0x219f, FX32_CONST(2.85), -0xa654 },
                                          { 0, FX32_CONST(1.1), 0 },
                                          { -0x219f, FX32_CONST(2.85), -0xa654 } };
static const VMInitParam data_ov094_02209dcc = { 16, 8, data_ov094_02209e88, NELEMS(data_ov094_02209e88) };
static s32 data_ov094_0220ad58[8] = { 0, 0, 5, 4, 0, 0, 3, 2 };
static const VecFx32 data_ov094_02209db4 = { 0, 0, -FX32_ONE };
static VecFx32 data_ov094_0220acb8 = { FX32_CONST(2), FX32_CONST(3.6), FX32_CONST(6.5) };
static VecFx32 data_ov094_0220acd0 = { FX32_CONST(3), FX32_CONST(2.6), 0 };
static const VecFx32 data_ov094_02209d90 = { 0, 0, -FX32_ONE };

// pokebw2's BtlvEffvm_Create
VM *func_ov094_021f93f4(TCBManager *tcbManager, HeapID heapId) {
    BtlvEffvm *wk = GFL_HeapAllocate(heapId, sizeof(BtlvEffvm), TRUE, "btlv_effvm.c", 637);
    VM *vm;
    int i;

    wk->heapId = heapId;
    wk->tcbManager = tcbManager;
    wk->ret = TRUE;
    wk->unk244 = 1;
    for (i = 0; i < 16; i++) {
        wk->particleIds[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        wk->clactIndexes[i] = -1;
    }
    for (i = 0; i < 6; i++) {
        wk->voices[i] = -1;
    }
    vm = VM_Create(heapId, &data_ov094_02209dcc);
    VM_ChangeEnv(vm, wk);
    return vm;
}

// pokebw2's BtlvEffvm_Main
BOOL func_ov094_021f9478(VM *vm) {
    BOOL running = VM_Run(vm);
    BtlvEffvm *wk = VM_GetEnv(vm);

    if (wk->paused) {
        return FALSE;
    }
    if (!running && wk->script != NULL) {
        func_ov094_021f8198(1, 2);
        if (!wk->isEffect) {
            func_ov094_021fe410(wk, 0x6b000, 0x7f000, 20);
        }
        GFL_HeapFree(wk->script);
        wk->script = NULL;
    }
    return running;
}

// pokebw2's BtlvEffvm_Delete
void func_ov094_021f94d8(VM *vm) {
    BtlvEffvm *wk = VM_GetEnv(vm);

    func_ov094_021f97e4(vm);
    GFL_HeapFree(wk);
    VM_Free(vm);
}

// pokebw2's BtlvEffvm_Start
void func_ov094_021f94f8(VM *vm, u32 attacker, u32 defender, u16 move, BtlvEffvmParam *param) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    u32 offset;
    BOOL visible;
    void *script;

    wk->script = NULL;
    func_ov094_021f8a10(1);
    if (param != NULL) {
        wk->param = *param;
    } else {
        MIi_CpuClear16(0, &wk->param, sizeof(BtlvEffvmParam));
    }
    wk->attacker = attacker;
    wk->defender = defender;
    wk->unk244 = 1;
    wk->move = move;
    if (wk->move >= BTLV_EFFECT_FIRST && (wk->move == 0x261 || wk->move == 0x262) && wk->lastMove == wk->move &&
        wk->lastAttacker == attacker) {
        return;
    }
    wk->lastMove = wk->move;
    wk->lastAttacker = attacker;
    if (func_ov094_021feaa0(wk->move) == FALSE) {
        func_ov094_021f8934(0);
    }
    if (wk->move < BTLV_EFFECT_FIRST) {
        wk->script = GFL_ArcSysReadHeapNew(66, move, HEAPID_TAIL(wk->heapId));
        if (wk->move != 144 && wk->move != 119 && wk->move != 267) {
            func_ov094_021f8198(0, 2);
        }
        GFL_BGSysSetBGEnabled(1, FALSE);
        GFL_BGSysSetBGEnabled(2, FALSE);
        GFL_BGSysSetBGEnabled(3, FALSE);
        wk->isEffect = FALSE;
        func_ov094_021fe410(wk, 0x7f000, 0x6b000, 20);
    } else {
        wk->script = GFL_ArcSysReadHeapNew(67, move - BTLV_EFFECT_FIRST, HEAPID_TAIL(wk->heapId));
        wk->isEffect = TRUE;
        if (move >= 0x256 && !(reg_G2_BLDALPHA & 0x1f)) {
            GFL_BGSysSetBGEnabled(1, FALSE);
        }
    }
    offset = 4;
    if (param != NULL) {
        if (*(u8 *)wk->script <= param->variant) {
            param->variant = 0;
        }
        offset += param->variant * 0x38;
    }
    visible = FALSE;
    if (func_ov094_021ffd9c(func_ov094_021f8694(), wk->attacker)) {
        if (((wk->move == 248 || wk->move == 353) && param->variant == 1) || wk->move == 144 || wk->move == 119 ||
            wk->move == 267) {
        } else {
            visible = func_ov094_021fff38(func_ov094_021f8694(), wk->attacker) & 1;
        }
    }
    if (wk->move == 0x27e) {
        wk->unk0_18 = TRUE;
    }
    if (wk->move == 0x291) {
        wk->unk0_18 = FALSE;
    }
    if (wk->move == 0x27e && visible) {
        wk->savedScript = wk->script;
        wk->savedOffset = offset;
        wk->script = GFL_ArcSysReadHeapNew(67, 51, HEAPID_TAIL(wk->heapId));
        wk->isEffect = TRUE;
        wk->substituteRestore = TRUE;
        offset = 4;
    } else if (!wk->isEffect && wk->move != 144 && !wk->substituteRestore && visible) {
        wk->savedScript = wk->script;
        wk->savedOffset = offset;
        wk->script = GFL_ArcSysReadHeapNew(67, 51, HEAPID_TAIL(wk->heapId));
        wk->isEffect = TRUE;
        offset = 4;
    }
    script = wk->script;
    wk->value = 0;
    wk->sePlayed = FALSE;
    wk->ballSource = 8;
    VM_LoadScript(vm, (u8 *)wk->script + *(u32 *)((u8 *)script + offset));
}

// pokebw2's BtlvEffvm_Stop
void func_ov094_021f97e4(VM *vm) {
    BtlvEffvm *wk = VM_GetEnv(vm);

    if (wk->script != NULL) {
        func_ov094_021fc448(vm, VM_GetEnv(vm));
        GFL_HeapFree(wk->script);
        wk->script = NULL;
    }
}

// pokebw2's BtlvEffvm_Resume
void func_ov094_021f9814(VM *vm) {
    BtlvEffvm *wk = VM_GetEnv(vm);

    wk->paused = FALSE;
}

// pokebw2's BtlvEffvm_GetScriptKind
s32 func_ov094_021f9824(VM *vm) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    s32 ret = -1;

    if (wk->script != NULL) {
        ret = wk->isEffect;
    }
    return ret;
}

// pokebw2's BtlvEffvmParam_Clear
void func_ov094_021f9844(BtlvEffvmParam *param) {
    param->unk0 = 0;
    param->variant = 0;
    param->unk2 = 0;
    param->unk3 = 0;
    param->unk4 = 0;
    param->unk8 = 0;
    param->itemNo = 0;
}

// pokebw2's BtlvEffvm_PlaySEAt
void func_ov094_021f9858(VM *vm, u32 se, u32 player, u32 pan, int arg4, int arg5, int vol, int pitch, int wait) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    BtlvEffvmSeWork *work;

    if (func_ov094_021f88e0() == TRUE) {
        return;
    }
    wk->sePlayed = TRUE;
    if (pan == 2) {
        pan = 0;
    } else if (pan <= 1) {
        pan = (pan & 1) ? 127 : -128;
    } else {
        pan = (func_ov094_021fcf2c(vm, pan) & 1) ? 127 : -128;
    }
    if (vol > 255) {
        vol = 255;
    }
    if (pitch > 255) {
        pitch = 255;
    }
    if (wait == 0) {
        func_ov094_021fdb40(se, player, pan, arg4, arg5, vol, pitch);
        return;
    }
    work = GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmSeWork), FALSE, "btlv_effvm.c", 1073);
    work->wk = wk;
    work->se = se;
    work->player = player;
    work->pan = pan;
    work->wait = wait;
    work->arg4 = arg4;
    work->arg5 = arg5;
    work->vol = vol;
    work->pitch = pitch;
    wk->seDelayed = TRUE;
    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe54c, work, 0), func_ov094_021fe580, 1);
}

void func_ov094_021f9948(VM *vm, u32 player, u32 type, u32 param, int start, int end, int delay, int frames, int stepWait,
                       int count) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    BtlvEffvmSeMoveWork *work =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmSeMoveWork), FALSE, "btlv_effvm.c", 1109);

    work->wk = wk;
    work->player = player;
    work->type = type;
    work->param = param;
    work->start = start;
    work->end = end;
    work->delay = delay;
    work->frames = frames;
    work->framesReset = frames;
    work->stepWait = 0;
    work->stepWaitReset = stepWait;
    work->count = count * 2;
    if (type == 1 && work->count == 0) {
        work->count = 2;
    }
    work->value = FX32_CONST(work->start);
    work->step = FX_Div(FX32_CONST(work->end - work->start), FX32_CONST(work->frames));
    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe594, work, 0), func_ov094_021fe690, 1);
    wk->seSliding = TRUE;
}

// pokebw2's BtlvEffvm_ReleaseVoices
void func_ov094_021f9a7c(VM *vm) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    int i;

    for (i = 0; i < 6; i++) {
        if (wk->voices[i] != -1) {
            PokeVoice_Release(wk->voices[i]);
        }
    }
}

// pokebw2's BtlvEffvm_ClearLastEffect
void func_ov094_021f9aa4(VM *vm) {
    BtlvEffvm *wk = VM_GetEnv(vm);

    wk->lastMove = 0;
    wk->lastAttacker = 0;
}

// pokebw2's BtlvEffvm_CmdCameraMovePreset
static BOOL func_ov094_021f9ab8(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    VecFx32 pos;
    VecFx32 target;
    int moveType = VM_Read32(vm);
    int posType = VM_Read32(vm);
    int frames = VM_Read32(vm);
    int wait = VM_Read32(vm);
    int brakeFrames = VM_Read32(vm);
    int rule;
    int battler;

    if (wk->cameraLocked) {
        return wk->ret;
    }
    if (posType == 13 && wk->camSaved == FALSE) {
        posType = 8;
    }
    if (posType != 8 && posType != 15 && posType != 16 && posType != 14 && posType != 17) {
        func_ov094_021fdb20(wk);
    } else {
        wk->cameraReset = TRUE;
    }
    switch (posType) {
    case 15:
    case 16:
        rule = func_ov094_021f8710();
        battler = posType == 15 ? wk->attacker : wk->defender;
        switch (rule) {
        case 0:
            return wk->ret;
        case 1:
        case 2:
            switch (battler) {
            case 2:
                posType = 2;
                break;
            case 3:
                posType = 8;
                if (rule != 1) {
                    posType = 18;
                }
                break;
            case 4:
                posType = 4;
                if (rule != 1) {
                    posType = 8;
                }
                break;
            case 5:
                posType = 8;
                break;
            case 6:
                posType = 8;
                if (rule != 1) {
                    posType = 6;
                }
                break;
            case 0:
            case 1:
            case 7:
            default:
                return wk->ret;
            }
            break;
        case 3:
            if (battler == 4 || battler == 6) {
                posType = 19;
            }
            break;
        }
        break;
    case 14:
        rule = func_ov094_021f8710();
        if (rule == 0 || rule == 3) {
            return wk->ret;
        }
        break;
    case 20:
        rule = func_ov094_021f8710();
        if (rule == 0 || rule == 3) {
            func_ov094_021ff16c(func_ov094_021f8694());
            return wk->ret;
        } else {
            posType = 14;
        }
        break;
    case 21:
        rule = func_ov094_021f8710();
        if (rule == 0 || rule == 3) {
            posType = wk->attacker;
        } else if (wk->attacker & 1) {
            posType = 1;
        } else {
            posType = 14;
        }
        break;
    case 17:
        posType = 8;
        break;
    case 9:
    case 10:
        rule = func_ov094_021f8710();
        posType = wk->attacker;
        if (rule == 3 && (posType == 4 || posType == 6)) {
            posType = 19;
        }
        break;
    case 11:
    case 12:
        switch (wk->param.unk0) {
        case 0:
        case 1:
        case 2:
        case 3:
            posType = wk->defender;
            if (posType == 0xff) {
                posType = (wk->attacker & 1) ^ 1;
            }
            break;
        case 5:
        case 7:
        case 9:
        case 13:
            posType = wk->defender;
            if (posType != 0xff) {
                break;
            }
            // fallthrough
        case 11:
            posType = (wk->attacker & 1) ^ 1;
            break;
        case 6:
        case 12:
            posType = wk->attacker & 1;
            break;
        case 4:
        case 8:
        case 10:
            rule = func_ov094_021f8710();
            if (rule == 0 || rule == 3) {
                posType = (wk->attacker & 1) ^ 1;
            } else {
                posType = 14;
            }
            break;
        }
        break;
    }
    switch (posType) {
    case 0:
    case 1:
        pos.x = data_ov094_0220ace8[posType].x;
        pos.y = data_ov094_0220ace8[posType].y;
        pos.z = data_ov094_0220ace8[posType].z;
        target.x = data_ov094_0220ad00[posType].x;
        target.y = data_ov094_0220ad00[posType].y;
        target.z = data_ov094_0220ad00[posType].z;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        rule = func_ov094_021f8710();
        if (rule == 3) {
            pos.x = data_ov094_0220ace8[posType - 2].x;
            pos.y = data_ov094_0220ace8[posType - 2].y;
            pos.z = data_ov094_0220ace8[posType - 2].z;
            target.x = data_ov094_0220ad00[posType - 2].x;
            target.y = data_ov094_0220ad00[posType - 2].y;
            target.z = data_ov094_0220ad00[posType - 2].z;
            break;
        }
        if (rule == 1) {
            pos.x = data_ov094_0220ad78[posType - 2].x;
            pos.y = data_ov094_0220ad78[posType - 2].y;
            pos.z = data_ov094_0220ad78[posType - 2].z;
            target.x = data_ov094_0220ada8[posType - 2].x;
            target.y = data_ov094_0220ada8[posType - 2].y;
            target.z = data_ov094_0220ada8[posType - 2].z;
            break;
        }
        pos.x = data_ov094_0220add8[posType - 2].x;
        pos.y = data_ov094_0220add8[posType - 2].y;
        pos.z = data_ov094_0220add8[posType - 2].z;
        target.x = data_ov094_0220ae20[posType - 2].x;
        target.y = data_ov094_0220ae20[posType - 2].y;
        target.z = data_ov094_0220ae20[posType - 2].z;
        break;
    case 6:
    case 7:
        pos.x = data_ov094_0220add8[posType - 2].x;
        pos.y = data_ov094_0220add8[posType - 2].y;
        pos.z = data_ov094_0220add8[posType - 2].z;
        target.x = data_ov094_0220ae20[posType - 2].x;
        target.y = data_ov094_0220ae20[posType - 2].y;
        target.z = data_ov094_0220ae20[posType - 2].z;
        break;
    case 13:
        pos.x = wk->camPos.x;
        pos.y = wk->camPos.y;
        pos.z = wk->camPos.z;
        target.x = wk->camTarget.x;
        target.y = wk->camTarget.y;
        target.z = wk->camTarget.z;
        break;
    case 14:
        pos.x = data_ov094_0220aca0.x;
        pos.y = data_ov094_0220aca0.y;
        pos.z = data_ov094_0220aca0.z;
        target.x = data_ov094_0220acb8.x;
        target.y = data_ov094_0220acb8.y;
        target.z = data_ov094_0220acb8.z;
        if (wk->cameraReset) {
            func_ov094_021fe9f0(0xa66, 0xb9a, frames, wait, 0);
            wk->mcssScaled = TRUE;
        }
        break;
    case 19:
        pos.x = data_ov094_0220acc4.x;
        pos.y = data_ov094_0220acc4.y;
        pos.z = data_ov094_0220acc4.z;
        target.x = data_ov094_0220acac.x;
        target.y = data_ov094_0220acac.y;
        target.z = data_ov094_0220acac.z;
        if (wk->cameraReset) {
            func_ov094_021fe9f0(0xa66, 0xb9a, frames, wait, 0);
            wk->mcssScaled = TRUE;
        }
        break;
    case 18:
        pos.x = data_ov094_0220acdc.x;
        pos.y = data_ov094_0220acdc.y;
        pos.z = data_ov094_0220acdc.z;
        target.x = data_ov094_0220acd0.x;
        target.y = data_ov094_0220acd0.y;
        target.z = data_ov094_0220acd0.z;
        break;
    default:
        func_ov094_021f7208(&pos, &target);
        if (wk->mcssScaled) {
            func_ov094_021fe9f0(FX32_ONE, FX32_ONE, frames, wait, 0);
            wk->mcssScaled = FALSE;
        }
        break;
    }
    switch (moveType) {
    case 0:
        func_ov094_021f6edc(func_ov094_021f8684(), &pos, &target);
        break;
    case 1:
        func_ov094_021f6f48(func_ov094_021f8684(), &pos, &target, frames, wait, brakeFrames);
        break;
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCameraMove
static BOOL func_ov094_021f9efc(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int moveType = VM_Read32(vm);
    VecFx32 pos;
    VecFx32 target;
    VecFx32 curPos;
    VecFx32 curTarget;
    int frames;
    int wait;
    int brakeFrames;

    pos.x = VM_Read32(vm);
    pos.y = VM_Read32(vm);
    pos.z = VM_Read32(vm);
    target.x = VM_Read32(vm);
    target.y = VM_Read32(vm);
    target.z = VM_Read32(vm);
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    brakeFrames = VM_Read32(vm);
    func_ov094_021fdb20(wk);
    switch (moveType) {
    case 0:
        func_ov094_021f6edc(func_ov094_021f8684(), &pos, &target);
        break;
    case 1:
        func_ov094_021f6f48(func_ov094_021f8684(), &pos, &target, frames, wait, brakeFrames);
        break;
    case 2:
        func_ov094_021f70f0(func_ov094_021f8684(), &curPos, &curTarget);
        VEC_Add(&curPos, &pos, &pos);
        VEC_Add(&curTarget, &target, &target);
        func_ov094_021f6f48(func_ov094_021f8684(), &pos, &target, frames, wait, brakeFrames);
        break;
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCameraRotate
static BOOL func_ov094_021f9fc0(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int moveType = VM_Read32(vm);
    s32 pitch = VM_Read32(vm);
    s32 yaw = VM_Read32(vm);
    int frames = VM_Read32(vm);
    int wait = VM_Read32(vm);
    int brakeFrames = VM_Read32(vm);
    VecFx32 pos;
    VecFx32 target;

    func_ov094_021fdb20(wk);
    switch (moveType) {
    case 0:
        func_ov094_021f6f20(func_ov094_021f8684(), pitch, yaw);
        break;
    case 1:
        func_ov094_021f7108(func_ov094_021f8684(), pitch, yaw, &pos, &target);
        func_ov094_021f6f48(func_ov094_021f8684(), &pos, &target, frames, wait, brakeFrames);
        break;
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCameraShake
static BOOL func_ov094_021fa048(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int dir = VM_Read32(vm);
    fx32 amplitude = VM_Read32(vm);
    int unused = VM_Read32(vm);
    int frames = VM_Read32(vm);
    int wait = VM_Read32(vm);
    int count = VM_Read32(vm);

    func_ov094_021f7000(func_ov094_021f8684(), dir, amplitude, unused, frames, wait, count);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssCameraMode
static BOOL func_ov094_021fa09c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int type = VM_Read32(vm);
    int count;
    int i;
    int positions[8];

    if (VM_Read32(vm) == 0) {
        wk->unk244 = type;
        func_ov094_021fdb20(wk);
    } else {
        count = func_ov094_021fca04(wk, 14, positions);
        if (count != 0) {
            for (i = 0; i < count; i++) {
                if (type == 1) {
                    func_ov094_021ff228(func_ov094_021f8694(), positions[i]);
                } else {
                    func_ov094_021ff1f4(func_ov094_021f8694(), positions[i]);
                }
            }
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCameraSave
static BOOL func_ov094_021fa108(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->camSaved = TRUE;
    func_ov094_021f70f0(func_ov094_021f8684(), &wk->camPos, &wk->camTarget);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdParticleLoad
static BOOL func_ov094_021fa12c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 fileId = func_ov094_021fe38c(wk, VM_Read32(vm));
    int index;
    void *work;
    void *resource;
    BtlvEffvmParticleVBlankWork *vblank;

    if (func_ov094_021fd15c(wk, fileId, &index) == TRUE) {
        // the SPL manager's work, whose emitters and particles hold pointers
        work = GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), WORK_AREA_SIZE(0x4800), FALSE, "btlv_effvm.c", 1813);
        resource = func_0205121c(6, fileId, HEAPID_TAIL(wk->heapId));
        if (work != NULL && resource != NULL) {
            wk->particles[index] =
                func_02050c30(work, WORK_AREA_SIZE(0x4800), FALSE, 5, 6, 54, HEAPID_TAIL(wk->heapId));
            if (wk->particles[index] != NULL) {
                func_02051380(wk->particles[index], resource);
                vblank = GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmParticleVBlankWork), FALSE,
                                          "btlv_effvm.c", 1823);
                vblank->wk = wk;
                vblank->particle = wk->particles[index];
                vblank->resource = resource;
                wk->particleLoading = TRUE;
                func_ov094_021f896c(GFL_VBlankTCBAdd(func_ov094_021fe9bc, vblank, 0), NULL, 1);
                VM_SetNativeCallback(vm, func_ov094_021fc9f4);
                func_ov094_021fd19c(wk, index, resource);
            }
        }
        if (wk->particles[index] == NULL) {
            if (work != NULL) {
                GFL_HeapFree(work);
            }
            if (resource != NULL) {
                GFL_HeapFree(resource);
            }
            wk->particleIds[index] = -1;
        }
    }
    return TRUE;
}

// pokebw2's BtlvEffvm_CmdEmitterAdd
static BOOL func_ov094_021fa288(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 1866);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->from = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->unk18 = VM_Read32(vm);
    VM_Read32(vm);
    param->radiusScale = VM_Read32(vm);
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = VM_Read32(vm);
    param->cameraScale = VM_Read32(vm);
    if (param->to == 8) {
        param->to = param->from;
    }
    if (index != 16) {
        func_ov094_021fdd60(wk, param, index, resourceId);
    }
    GFL_HeapFree(param);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdEmitterAddAt
static BOOL func_ov094_021fa358(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 1917);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->from = 8;
    param->to = 8;
    param->fromPos.x = VM_Read32(vm);
    param->fromPos.y = VM_Read32(vm);
    param->fromPos.z = VM_Read32(vm);
    param->toPos.x = VM_Read32(vm);
    param->toPos.y = VM_Read32(vm);
    param->toPos.z = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->unk18 = VM_Read32(vm);
    VM_Read32(vm);
    param->radiusScale = VM_Read32(vm);
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = VM_Read32(vm);
    param->cameraScale = VM_Read32(vm);
    if (index != 16) {
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, param) == (SPLEmitter *)-1) {
            GFL_HeapFree(param);
        }
    } else {
        GFL_HeapFree(param);
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdEmitterAddOrtho
static BOOL func_ov094_021fa458(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 1974);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->from = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = VM_Read32(vm);
    param->offset.y = VM_Read32(vm);
    param->offset.z = VM_Read32(vm);
    param->radiusScale = VM_Read32(vm);
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = VM_Read32(vm);
    param->cameraScale = VM_Read32(vm);
    param->offsetMode = 1;
    if (param->from == 13) {
        param->from = 9;
        param->offsetMode = 2;
    }
    if (param->to == 8) {
        param->to = param->from;
    }
    if (index != 16) {
        G3DCameraProjection projection;
        VecFx32 pos = { 0, 0, 0 };
        VecFx32 up = { 0, FX32_ONE, 0 };
        VecFx32 target = data_ov094_02209d90;

        projection.type = G3DCAM_PROJECTION_ORTHO;
        projection.param1 = FX32_CONST(3);
        projection.param2 = FX32_CONST(-3);
        projection.param3 = FX32_CONST(-4);
        projection.param4 = FX32_CONST(4);
        projection.near = FX32_ONE;
        projection.far = FX32_CONST(512);
        projection.ndcRangeOverride = FX32_ONE;

        if (func_02051788(wk->particles[index]) == NULL) {
            func_02051648(wk->particles[index], &projection, 0x2000, &pos, &up, &target, HEAPID_TAIL(wk->heapId));
        }
        func_ov094_021fdd60(wk, param, index, resourceId);
    }
    GFL_HeapFree(param);
    return wk->ret;
}

static const VecFx32 data_ov094_02209d78 = { 0, FX32_ONE, 0 };

// pokebw2's BtlvEffvm_CmdEmitterAddAll
static BOOL func_ov094_021fa5d0(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 2047);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int count = func_ov094_021fd1c0(wk, index);
    BOOL ortho;
    int i;

    param->vm = vm;
    param->from = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->unk18 = VM_Read32(vm);
    ortho = VM_Read32(vm);
    param->radiusScale = VM_Read32(vm);
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = VM_Read32(vm);
    param->cameraScale = VM_Read32(vm);
    if (index != 16) {
        if (ortho) {
            G3DCameraProjection projection;
            VecFx32 pos = { 0, 0, 0 };
            VecFx32 up = data_ov094_02209d78;
            VecFx32 target = { 0, 0, -FX32_ONE };

            projection.type = G3DCAM_PROJECTION_ORTHO;
            projection.param1 = FX32_CONST(3);
            projection.param2 = FX32_CONST(-3);
            projection.param3 = FX32_CONST(-4);
            projection.param4 = FX32_CONST(4);
            projection.near = FX32_ONE;
            projection.far = FX32_CONST(512);
            projection.ndcRangeOverride = FX32_ONE;

            if (func_02051788(wk->particles[index]) == NULL) {
                func_02051648(wk->particles[index], &projection, 0x2000, &pos, &up, &target, HEAPID_TAIL(wk->heapId));
            }
            param->offsetMode = 2;
        }
        if (param->to == 8) {
            param->to = param->from;
        }
        for (i = 0; i < count; i++) {
            func_ov094_021fdd60(wk, param, index, i);
        }
    }
    GFL_HeapFree(param);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdParticleDelete
static BOOL func_ov094_021fa758(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));

    if (index != 16) {
        func_ov094_021fdb08(wk->particles[index]);
        wk->particles[index] = NULL;
        wk->particleIds[index] = -1;
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdEmitterMove
static BOOL func_ov094_021fa794(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 2152);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->motionType = VM_Read32(vm);
    param->from = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->motionFrames = VM_Read32(vm);
    param->motionHeight = VM_Read32(vm);
    param->radiusScale = FX32_ONE;
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = FX32_ONE;
    param->cameraScale = VM_Read32(vm);
    param->turns = VM_Read32(vm);
    if (param->turns == 0) {
        param->turns = 1;
    }
    if (index != 16) {
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, param) == (SPLEmitter *)-1) {
            GFL_HeapFree(param);
        }
    } else {
        GFL_HeapFree(param);
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdEmitterMoveFrom
static BOOL func_ov094_021fa884(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 2210);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->motionType = VM_Read32(vm);
    param->from = 8;
    param->fromPos.x = VM_Read32(vm);
    param->fromPos.y = VM_Read32(vm);
    param->fromPos.z = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->motionFrames = VM_Read32(vm);
    param->motionHeight = VM_Read32(vm);
    param->radiusScale = FX32_ONE;
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = FX32_ONE;
    param->cameraScale = VM_Read32(vm);
    param->turns = VM_Read32(vm);
    if (param->turns == 0) {
        param->turns = 1;
    }
    if (index != 16) {
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, param) == (SPLEmitter *)-1) {
            GFL_HeapFree(param);
        }
    } else {
        GFL_HeapFree(param);
    }
    return wk->ret;
}

static const VecFx32 data_ov094_02209d84 = { 0, 0, -FX32_ONE };
static const VecFx32 data_ov094_02209dc0 = { 0, FX32_ONE, 0 };

// pokebw2's BtlvEffvm_CmdEmitterMoveOrtho
static BOOL func_ov094_021fa988(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 2272);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->motionType = VM_Read32(vm);
    param->from = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->motionFrames = VM_Read32(vm);
    param->motionHeight = VM_Read32(vm);
    param->radiusScale = FX32_CONST(0.75);
    param->lifeTimeScale = VM_Read32(vm);
    param->scaleScale = FX32_CONST(0.75);
    param->cameraScale = VM_Read32(vm);
    param->turns = VM_Read32(vm);
    param->offsetMode = 3;
    if (param->turns == 0) {
        param->turns = 1;
    }
    if (index != 16) {
        G3DCameraProjection projection;
        VecFx32 pos = { 0, 0, 0 };
        VecFx32 up = data_ov094_02209dc0;
        VecFx32 target = data_ov094_02209d84;

        projection.type = G3DCAM_PROJECTION_ORTHO;
        projection.param1 = FX32_CONST(3);
        projection.param2 = FX32_CONST(-3);
        projection.param3 = FX32_CONST(-4);
        projection.param4 = FX32_CONST(4);
        projection.near = FX32_ONE;
        projection.far = FX32_CONST(512);
        projection.ndcRangeOverride = FX32_ONE;

        if (func_02051788(wk->particles[index]) == NULL) {
            func_02051648(wk->particles[index], &projection, 0x2000, &pos, &up, &target, HEAPID_TAIL(wk->heapId));
        }
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, param) == (SPLEmitter *)-1) {
            GFL_HeapFree(param);
        }
    } else {
        GFL_HeapFree(param);
    }
    return wk->ret;
}

static const VecFx32 data_ov094_02209d9c = { 0, 0, -FX32_ONE };
static const VecFx32 data_ov094_02209da8 = { 0, FX32_ONE, 0 };

// pokebw2's BtlvEffvm_CmdEmitterMoveFromOrtho
static BOOL func_ov094_021fab0c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmEmitParam *param =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 2349);
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);

    param->vm = vm;
    param->motionType = VM_Read32(vm);
    param->from = 8;
    param->fromPos.x = VM_Read32(vm);
    param->fromPos.y = VM_Read32(vm);
    param->fromPos.z = VM_Read32(vm);
    param->to = VM_Read32(vm);
    param->offset.x = 0;
    param->offset.y = VM_Read32(vm);
    param->offset.z = 0;
    param->motionFrames = VM_Read32(vm);
    param->motionHeight = VM_Read32(vm);
    param->radiusScale = FX32_ONE;
    param->lifeTimeScale = VM_Read32(vm);
    param->cameraScale = VM_Read32(vm);
    param->scaleScale = VM_Read32(vm);
    param->offsetMode = 3;
    if (index != 16) {
        G3DCameraProjection projection;
        VecFx32 pos = { 0, 0, 0 };
        VecFx32 up = data_ov094_02209da8;
        VecFx32 target = data_ov094_02209d9c;

        projection.type = G3DCAM_PROJECTION_ORTHO;
        projection.param1 = FX32_CONST(3);
        projection.param2 = FX32_CONST(-3);
        projection.param3 = FX32_CONST(-4);
        projection.param4 = FX32_CONST(4);
        projection.near = FX32_ONE;
        projection.far = FX32_CONST(512);
        projection.ndcRangeOverride = FX32_ONE;

        if (func_02051788(wk->particles[index]) == NULL) {
            func_02051648(wk->particles[index], &projection, 0x2000, &pos, &up, &target, HEAPID_TAIL(wk->heapId));
        }
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, param) == (SPLEmitter *)-1) {
            GFL_HeapFree(param);
        }
    } else {
        GFL_HeapFree(param);
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdOrbitEmitters
static BOOL func_ov094_021fac94(VM *vm, void *env) {
    return func_ov094_021fdfa0(vm, env, 0);
}

// pokebw2's BtlvEffvm_CmdOrbitEmittersAlt
static BOOL func_ov094_021faca0(VM *vm, void *env) {
    return func_ov094_021fdfa0(vm, env, 1);
}

// pokebw2's BtlvEffvm_CmdMcssMoveXY
static BOOL func_ov094_021facac(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    VecFx32 dest;
    s32 frames;
    s32 wait;
    s32 times;
    int i;

    dest.x = VM_Read32(vm);
    dest.y = VM_Read32(vm);
    dest.z = 0;
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    times = VM_Read32(vm);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff4c0(func_ov094_021f8694(), positions[i], type, &dest, frames, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssCircle
static BOOL func_ov094_021fad2c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvMcssCircle circle;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int i;

    circle.mode = VM_Read32(vm);
    circle.start = VM_Read32(vm);
    circle.radius1 = VM_Read32(vm);
    circle.radius2 = VM_Read32(vm);
    circle.frames = (s32)VM_Read32(vm) >> FX32_SHIFT;
    circle.wait = (s32)VM_Read32(vm) >> FX32_SHIFT;
    circle.count = (s32)VM_Read32(vm) >> FX32_SHIFT;
    circle.turnWait = VM_Read32(vm);
    if (count != 0 && circle.count != 0) {
        for (i = 0; i < count; i++) {
            circle.pos = positions[i];
            func_ov094_021ffa18(func_ov094_021f8694(), &circle);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssShake
static BOOL func_ov094_021fadc0(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvMcssShake shake;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    s32 axis = VM_Read32(vm);
    fx32 startAngle = VM_Read32(vm);
    fx32 endAngle = VM_Read32(vm);
    fx32 amplitude = VM_Read32(vm);
    s32 frames = VM_Read32(vm);
    int i;

    shake.frames = frames;
    shake.axis = axis;
    shake.angle = startAngle;
    shake.amplitude = amplitude;
    shake.speed = FX_Div(endAngle - startAngle, frames << FX32_SHIFT);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            shake.pos = positions[i];
            func_ov094_021ffb04(func_ov094_021f8694(), &shake);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssScale
static BOOL func_ov094_021fae48(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    VecFx32 dest;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    s32 frames;
    s32 wait;
    s32 times;
    int i;

    dest.x = VM_Read32(vm);
    dest.y = VM_Read32(vm);
    dest.z = FX32_ONE;
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    times = VM_Read32(vm);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff5b0(func_ov094_021f8694(), positions[i], type, &dest, frames, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssRotate
static BOOL func_ov094_021faecc(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    VecFx32 dest;
    s32 frames;
    s32 wait;
    s32 times;
    int i;

    dest.x = 0;
    dest.y = 0;
    dest.z = VM_Read32(vm);
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    times = VM_Read32(vm);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff730(func_ov094_021f8694(), positions[i], type, &dest, frames, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssAlpha
static BOOL func_ov094_021faf44(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    int alpha = VM_Read32(vm);
    s32 frames = VM_Read32(vm);
    s32 wait = VM_Read32(vm);
    s32 times = VM_Read32(vm);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff924(func_ov094_021f8694(), positions[i], type, alpha, frames, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssMosaic
static BOOL func_ov094_021fafbc(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    int level = VM_Read32(vm);
    s32 frames = VM_Read32(vm);
    s32 wait = VM_Read32(vm);
    s32 times = VM_Read32(vm);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ffbbc(func_ov094_021f8694(), positions[i], type, level, frames, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssBlink
static BOOL func_ov094_021fb034(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    s32 wait = VM_Read32(vm);
    s32 times = VM_Read32(vm);
    int i;

    if (count != 0) {
        if (times == 0) {
            times = 1;
        }
        for (i = 0; i < count; i++) {
            func_ov094_021ff870(func_ov094_021f8694(), positions[i], type, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssSetVanish
static BOOL func_ov094_021fb09c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int mode = VM_Read32(vm);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff304(func_ov094_021f8694(), positions[i], mode);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssPalFade
static BOOL func_ov094_021fb0e8(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int startEvy = VM_Read32(vm);
    int endEvy = VM_Read32(vm);
    int wait = VM_Read32(vm);
    u32 color = VM_Read32(vm);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ffdb4(func_ov094_021f8694(), positions[i], startEvy, endEvy, wait, color);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssSetMode
static BOOL func_ov094_021fb168(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int mode = VM_Read32(vm);
    int i;

    if (mode == 5) {
        func_ov094_02200654(func_ov094_021f8694());
    } else if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff374(func_ov094_021f8694(), positions[i], mode);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssSetValue
static BOOL func_ov094_021fb1c4(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    u32 value = VM_Read32(vm);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff470(func_ov094_021f8694(), positions[i], (u8)value);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssScaleTo
static BOOL func_ov094_021fb214(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    VecFx32 dest;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int type = VM_Read32(vm);
    s32 frames;
    s32 wait;
    s32 times;
    int i;

    dest.x = VM_Read32(vm);
    dest.y = VM_Read32(vm);
    dest.z = FX32_ONE;
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    times = VM_Read32(vm);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021ff6b0(func_ov094_021f8694(), positions[i], type, &dest, frames, wait, times);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdGaugeHide
static BOOL func_ov094_021fb298(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021f7f04(positions[i]);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCallDisplay
static BOOL func_ov094_021fb2d0(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 arg0 = VM_Read32(vm);
    u32 pos = func_ov094_021fcf2c(vm, VM_Read32(vm));
    u32 arg2 = VM_Read32(vm);
    u32 arg3 = VM_Read32(vm);
    u32 arg4 = VM_Read32(vm);

    if (arg0 == -1) {
        arg0 = wk->value;
    }
    func_ov094_021f7f58(arg0, pos, arg2, arg3, arg4);
    return wk->ret;
}

static BOOL func_ov094_021fb328(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    s32 pos;
    VecFx32 dest;
    u32 type;
    u32 frames;
    u32 wait;
    u32 count;

    type = VM_Read32(vm);
    pos = func_ov094_021f84d0(type);
    type = VM_Read32(vm);
    dest.x = VM_Read32(vm);
    dest.y = VM_Read32(vm);
    dest.z = VM_Read32(vm);
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    count = VM_Read32(vm);

    func_ov094_021ff4c0(func_ov094_021f8694(), pos, type, &dest, frames, wait, count);
    return wk->ret;
}

BOOL func_ov094_021fb390(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 pos;
    u32 anim;

    pos = VM_Read32(vm);
    anim = VM_Read32(vm);
    if (func_ov094_0220041c(func_ov094_021f8694(), pos, anim) == TRUE) {
        func_ov094_02200468(func_ov094_021f8694(), pos);
    }
    return wk->ret;
}



// pokebw2's BtlvEffvm_CmdDeleteMcss
static BOOL func_ov094_021fb3c8(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    func_ov094_021f8010(VM_Read32(vm));
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdLoadBg
static BOOL func_ov094_021fb3dc(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 fileId = VM_Read32(vm);

    GFL_BGSysSetBGPriority(3, 1);
    GFL_BGSysLoadNCGRStatic(95, fileId + 1, 3, 0, 0, FALSE, HEAPID_TAIL(wk->heapId));
    loadBGScrToVramByNarcNoReserveNegAlign(95, fileId, 3, 0, 0, FALSE, HEAPID_TAIL(wk->heapId));
    func_020215ec(func_ov094_021f86e0(), 95, fileId + 2, HEAPID_TAIL(wk->heapId), 0, 0, 0x80, 0);
    wk->bgX = GFL_BGSysGetBGOffsetX(3);
    wk->bgY = GFL_BGSysGetBGOffsetY(3);
    func_ov094_02209784(func_ov094_021f8700(), 0, 0);
    wk->bgMoved = TRUE;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdBgMove
static BOOL func_ov094_021fb49c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 mode = VM_Read32(vm);
    s32 x = VM_Read32(vm);
    s32 y = VM_Read32(vm);
    s32 frames = VM_Read32(vm);
    s32 wait = VM_Read32(vm);
    s32 count = VM_Read32(vm);
    u32 pos = func_ov094_021fcf2c(vm, 14);

    func_ov094_022097cc(func_ov094_021f8700(), pos, mode, x, y, frames, wait, count);
    return wk->ret;
}

static BOOL func_ov094_021fb500(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    return wk->ret;
}

static BOOL func_ov094_021fb524(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdBgSetPriority
static BOOL func_ov094_021fb550(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    GFL_BGSysSetBGPriority(3, (u8)VM_Read32(vm));
    wk->bgMoved = TRUE;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdSetAlphaBlend
static BOOL func_ov094_021fb574(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int alpha;

    VM_Read32(vm);
    VM_Read32(vm);
    alpha = VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    VM_Read32(vm);
    if (alpha == 31) {
        G2_SetBlendAlpha(2, 0x3d, 31, 7);
    } else {
        G2_SetBlendAlpha(10, 0x35, 31, alpha);
    }
    wk->alphaBlend = TRUE;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCallDfc70
static BOOL func_ov094_021fb5d4(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 arg0 = VM_Read32(vm);
    u32 arg1 = VM_Read32(vm);
    u32 arg2 = VM_Read32(vm);
    u32 arg3 = VM_Read32(vm);
    u32 arg4 = VM_Read32(vm);

    func_ov094_021f8298(arg0, arg1, arg2, arg3, arg4);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCallDfd74
static BOOL func_ov094_021fb624(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 arg0 = VM_Read32(vm);
    u32 arg1 = VM_Read32(vm);

    func_ov094_021f839c(arg0, arg1);
    return wk->ret;
}

BOOL BtlvEffect_QueueCommands(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmWindowWork *work =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmWindowWork), TRUE, "btlv_effvm.c", 3464);

    work->wk = wk;
    work->dir = VM_Read32(vm);
    work->winH = VM_Read32(vm);
    work->winV = VM_Read32(vm);
    work->winInOut = VM_Read32(vm);
    work->count = VM_Read32(vm);
    work->waitReload = VM_Read32(vm);
    work->keepWindow = VM_Read32(vm);
    GFL_BGSysSetBGEnabled(1, FALSE);
    GFL_BGSysSetBGEnabled(2, FALSE);
    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe79c, work, 0), func_ov094_021fe8ac, 1);
    wk->windowTask = TRUE;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdClactAdd
static BOOL func_ov094_021fb6f4(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);
    u32 fileId = VM_Read32(vm);
    int pos = func_ov094_021fcf2c(vm, VM_Read32(vm));
    fx32 offsetX = VM_Read32(vm);
    fx32 offsetY = VM_Read32(vm);
    fx32 scaleX = VM_Read32(vm);
    fx32 scaleY = VM_Read32(vm);
    VecFx32 world;
    int x;
    int y;

    func_ov094_021ff3f4(func_ov094_021f8694(), &world, pos);
    world.x += offsetX;
    world.y += offsetY;
    NNS_G3dWorldPosToScrPos(&world, &x, &y);
    wk->clactIndexes[index] = func_ov094_02201af8(func_ov094_021f86f0(), 95, fileId, x, y, scaleX, scaleY);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdClactMove
static BOOL func_ov094_021fb790(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);
    int type = VM_Read32(vm);
    ClActorPos pos;
    int frames;
    int wait;
    int count;

    pos.x = VM_Read32(vm);
    pos.y = VM_Read32(vm);
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    count = VM_Read32(vm);
    func_ov094_02201cd0(func_ov094_021f86f0(), wk->clactIndexes[index], type, &pos, frames, wait, count);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdClactScale
static BOOL func_ov094_021fb7f8(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);
    int type = VM_Read32(vm);
    VecFx32 scale;
    int frames;
    int wait;
    int count;

    scale.x = VM_Read32(vm);
    scale.y = VM_Read32(vm);
    scale.z = FX32_ONE;
    frames = VM_Read32(vm);
    wait = VM_Read32(vm);
    count = VM_Read32(vm);
    func_ov094_02201d5c(func_ov094_021f86f0(), wk->clactIndexes[index], type, &scale, frames, wait, count);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdClactSetAnim
static BOOL func_ov094_021fb864(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);
    int sequence = VM_Read32(vm);

    func_ov094_02201da0(func_ov094_021f86f0(), wk->clactIndexes[index], sequence);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdClactPalFade
static BOOL func_ov094_021fb894(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);
    int start = VM_Read32(vm);
    int end = VM_Read32(vm);
    int delay = VM_Read32(vm);
    int color = VM_Read32(vm);

    func_ov094_02201db4(func_ov094_021f86f0(), wk->clactIndexes[index], start, end, delay, color);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdClactDelete
static BOOL func_ov094_021fb8f4(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);

    func_ov094_02201ca4(func_ov094_021f86f0(), wk->clactIndexes[index]);
    wk->clactIndexes[index] = -1;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdGauge
static BOOL func_ov094_021fb91c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 mode = VM_Read32(vm);
    int target = VM_Read32(vm);
    int targets[8];
    int positions[8];
    int count;
    int i;

    if (mode <= 1) {
        switch (target) {
        case 3:
            func_ov094_021f81b4(mode, wk->attacker);
            break;
        case 4:
            count = func_ov094_021fca04(wk, 16, targets);
            if (count != 0) {
                for (i = 0; i < count; i++) {
                    // BUG: every call passes the entry past the last target, which was never written
#ifdef BUGFIX
                    func_ov094_021f81b4(mode, targets[i]);
#else
                    func_ov094_021f81b4(mode, targets[count]);
#endif
                }
            }
            break;
        default:
            func_ov094_021f8198(mode, target);
            break;
        }
    } else {
        count = 0;
        switch (target) {
        case 0:
            for (i = 0; i < 8; i += 2) {
                if (func_ov094_021f7f1c(i)) {
                    positions[count] = i;
                    count++;
                }
            }
            break;
        case 1:
            for (i = 1; i < 8; i += 2) {
                if (func_ov094_021f7f1c(i)) {
                    positions[count] = i;
                    count++;
                }
            }
            break;
        case 2:
            for (i = 0; i < 8; i++) {
                if (func_ov094_021f7f1c(i)) {
                    positions[count] = i;
                    count++;
                }
            }
            break;
        case 3:
            count = 1;
            positions[0] = wk->attacker;
            break;
        case 4:
            count = func_ov094_021fca04(wk, 16, positions);
            break;
        }
        if (count != 0) {
            for (i = 0; i < count; i++) {
                ClActorPos offset = { 0, 0 };
                int *pos;

                offset.x = mode == 2 ? 128 : -16;
                pos = &positions[i];
                if (*pos & 1) {
                    offset.x *= -1;
                }
                func_ov094_022079c0(func_ov094_021f86b4(), *pos, &offset);
            }
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdPlaySE
static BOOL func_ov094_021fba64(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 se = VM_Read32(vm);
    u32 player = VM_Read32(vm);
    int pan = VM_Read32(vm);
    int wait = VM_Read32(vm);
    int arg4 = VM_Read32(vm);
    int arg5 = VM_Read32(vm);
    int vol = VM_Read32(vm);
    int pitch = VM_Read32(vm);

    VM_Read32(vm);
    func_ov094_021f9858(vm, se, player, pan, arg4, arg5, vol, pitch, wait);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdStopSE
static BOOL func_ov094_021fbad8(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    s32 player = VM_Read32(vm);

    if (player == 5) {
        GFL_SndStop();
    } else {
        GFL_SndPlayerStop(player);
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdPanSE
static BOOL func_ov094_021fbaf8(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    BtlvEffvmSeMoveWork *work =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmSeMoveWork), FALSE, "btlv_effvm.c", 3863);
    int from;
    int to;

    work->wk = wk;
    work->player = VM_Read32(vm);
    work->type = VM_Read32(vm);
    work->param = 2;
    from = func_ov094_021fcf2c(vm, VM_Read32(vm));
    to = func_ov094_021fcf2c(vm, VM_Read32(vm));
    work->start = (from & 1) ? 127 : -128;
    work->end = (to & 1) ? 127 : -128;
    work->delay = VM_Read32(vm);
    work->framesReset = work->frames = VM_Read32(vm);
    work->stepWait = 0;
    work->stepWaitReset = VM_Read32(vm);
    work->count = VM_Read32(vm) * 2;
    if (work->type == 1 && work->count == 0) {
        work->count = 2;
    }
    work->value = FX32_CONST(work->start);
    work->step = FX_Div(FX32_CONST(work->end - work->start), FX32_CONST(work->frames));
    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe594, work, 0), func_ov094_021fe690, 1);
    wk->seSliding = TRUE;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMoveSE
static BOOL func_ov094_021fbc78(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 player = VM_Read32(vm);
    u32 type = VM_Read32(vm);
    u32 param = VM_Read32(vm);
    int start = VM_Read32(vm);
    int end = VM_Read32(vm);
    int delay = VM_Read32(vm);
    int frames = VM_Read32(vm);
    int stepWait = VM_Read32(vm);
    int count = VM_Read32(vm);

    func_ov094_021f9948(vm, player, type, param, start, end, delay, frames, stepWait, count);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdWait
static BOOL func_ov094_021fbcec(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->waitKind = VM_Read32(vm);
    VM_SetNativeCallback(vm, func_ov094_021fc744);
    wk->ret = TRUE;
    return TRUE;
}

// pokebw2's BtlvEffvm_CmdWaitFrames
static BOOL func_ov094_021fbd10(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->waitTimer = VM_Read32(vm);
    VM_SetNativeCallback(vm, func_ov094_021fc9d0);
    wk->ret = TRUE;
    return TRUE;
}

// pokebw2's BtlvEffvm_CmdSetYield
static BOOL func_ov094_021fbd34(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->ret = VM_Read32(vm);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdIfVarConst
static BOOL func_ov094_021fbd44(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int value = func_ov094_021fdbb0(wk, VM_Read32(vm));
    int cond = VM_Read32(vm);
    int target = VM_Read32(vm);
    int offset = VM_Read32(vm);

    func_ov094_021fbdd4(vm, value, target, cond, offset);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdIfVarVar
static BOOL func_ov094_021fbd88(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int value = func_ov094_021fdbb0(wk, VM_Read32(vm));
    int cond = VM_Read32(vm);
    int target = func_ov094_021fdbb0(wk, VM_Read32(vm));
    int offset = VM_Read32(vm);

    func_ov094_021fbdd4(vm, value, target, cond, offset);
    return wk->ret;
}

// pokebw2's BtlvEffvm_JumpIf
static void func_ov094_021fbdd4(VM *vm, int value, int target, int cond, int offset) {
    BOOL jump = FALSE;

    switch (cond) {
    case 0:
        if (value == target) {
            jump = TRUE;
        }
        break;
    case 1:
        if (value != target) {
            jump = TRUE;
        }
        break;
    case 2:
        if (value < target) {
            jump = TRUE;
        }
        break;
    case 3:
        if (value > target) {
            jump = TRUE;
        }
        break;
    case 4:
        if (value <= target) {
            jump = TRUE;
        }
        break;
    case 5:
        if (value >= target) {
            jump = TRUE;
        }
        break;
    }
    if (jump == TRUE) {
        VM_Jump(vm, vm->pc + offset);
    }
}

// pokebw2's BtlvEffvm_CmdIfPokeExists
static BOOL func_ov094_021fbe28(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int pos = VM_Read32(vm);
    int value = VM_Read32(vm);
    int offset = VM_Read32(vm);

    if (value == func_ov094_021f7f1c(pos)) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdSetValue
static BOOL func_ov094_021fbe60(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->value = VM_Read32(vm);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdLoadVar
static BOOL func_ov094_021fbe74(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->value = func_ov094_021fdbb0(wk, VM_Read32(vm));
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdSetVar
static BOOL func_ov094_021fbe90(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int id = VM_Read32(vm);
    int value = VM_Read32(vm);

    switch (id) {
    case 9:
        wk->param.unk0 = value;
        break;
    case 10:
        wk->param.variant = value;
        break;
    case 11:
        wk->param.unk2 = value;
        break;
    case 12:
        wk->param.unk3 = value;
        break;
    case 13:
        wk->param.unk4 = value;
        break;
    case 14:
        wk->param.unk8 = value;
        break;
    case 15:
        wk->param.itemNo = value;
        break;
    case 53:
        wk->mcssScaled = value;
        break;
    case 54:
        wk->camSaved = value;
        break;
    case 55:
        wk->unk2A0 = value;
        break;
    case 56:
        wk->cameraLocked = value;
        break;
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssEffect
static BOOL func_ov094_021fbf48(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int type = VM_Read32(vm);
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int arg = VM_Read32(vm);
    int i;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            func_ov094_021fffd8(func_ov094_021f8694(), positions[i], type, arg);
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssSwap
static BOOL func_ov094_021fbfa0(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    func_ov094_02200140(func_ov094_021f8694(), wk->defender, wk->attacker);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdPlayCry
static BOOL func_ov094_021fbfc0(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int positions[8];
    int count = func_ov094_021fca04(wk, VM_Read32(vm), positions);
    int arg2 = VM_Read32(vm);
    int arg3 = VM_Read32(vm);
    int arg4 = VM_Read32(vm);
    int arg5 = VM_Read32(vm);
    int arg6 = VM_Read32(vm);
    int delay = VM_Read32(vm);
    int i;

    if (count != 0) {
        if (delay == 0) {
            for (i = 0; i < count; i++) {
                int slot = func_ov094_021fe4d0(wk);

                wk->voices[slot] = func_ov094_02200218(func_ov094_021f8694(), positions[i], arg2, arg3, arg4, arg5, arg6);
            }
            func_ov094_021fe410(wk, 0x7f000, 0x6b000, 10);
            if (!wk->bgmLowerRequested) {
                BtlvEffvmCryWaitWork *work = GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmCryWaitWork),
                                                              FALSE, "btlv_effvm.c", 4345);

                work->wk = wk;
                func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe8d8, work, 0), NULL, 0);
            }
        } else {
            for (i = 0; i < count; i++) {
                BtlvEffvmCryWork *work =
                    GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmCryWork), FALSE, "btlv_effvm.c", 4356);

                work->wk = wk;
                work->pos = positions[i];
                work->arg2 = arg2;
                work->arg3 = arg3;
                work->arg4 = arg4;
                work->arg5 = arg5;
                work->arg6 = arg6;
                work->delay = delay;
                wk->cryDelayed = TRUE;
                func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe6a4, work, 0),
                                   func_ov094_021fe750, 1);
            }
        }
    }
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdSetBallSource
BOOL func_ov094_021fc134(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->ballSource = VM_Read32(vm);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdBallAdd
static BOOL func_ov094_021fc148(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int index = VM_Read32(vm);
    int pos = func_ov094_021fcf2c(vm, VM_Read32(vm));
    fx32 offsetX = VM_Read32(vm);
    fx32 offsetY = VM_Read32(vm);
    fx32 scaleX = VM_Read32(vm);
    fx32 scaleY = VM_Read32(vm);
    VecFx32 world;
    int x;
    int y;
    int ball;

    func_ov094_021ff3f4(func_ov094_021f8694(), &world, pos);
    world.x += offsetX;
    world.y += offsetY;
    NNS_G3dWorldPosToScrPos(&world, &x, &y);
    ball = PML_ItemGetMonsBallID(wk->param.itemNo);
    if (ball == 0 || ball > 25) {
        ball = 4;
    }
    if (ball == 25) {
        ball = 17;
    }
    wk->clactIndexes[index] =
        func_ov094_02201b20(func_ov094_021f86f0(), 95, ball * 2 + 129, ball * 2 + 130, 129, 130, x, y, scaleX, scaleY);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdCall
static BOOL func_ov094_021fc20c(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 effect = VM_Read32(vm);
    u32 attacker = VM_Read32(vm);
    u32 defender = VM_Read32(vm);

    wk->callAttackers[wk->callDepth] = wk->attacker;
    wk->callDefenders[wk->callDepth] = wk->defender;
    wk->callScripts[wk->callDepth] = wk->script;
    wk->callValues[wk->callDepth] = wk->value;
    if (attacker != 14) {
        wk->attacker = attacker;
    }
    if (defender != 16) {
        wk->defender = defender;
    }
    wk->value = 0;
    wk->script = GFL_ArcSysReadHeapNew(67, effect - BTLV_EFFECT_FIRST, HEAPID_TAIL(wk->heapId));
    VM_Call(vm, (u8 *)wk->script + ((u32 *)wk->script)[1]);
    wk->callDepth++;
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdReturn
static BOOL func_ov094_021fc2dc(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->callDepth--;
    GFL_HeapFree(wk->script);
    wk->attacker = wk->callAttackers[wk->callDepth];
    wk->defender = wk->callDefenders[wk->callDepth];
    wk->script = wk->callScripts[wk->callDepth];
    wk->value = wk->callValues[wk->callDepth];
    VM_Return(vm);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdJump
static BOOL func_ov094_021fc334(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int offset = VM_Read32(vm);

    VM_Jump(vm, vm->pc + offset);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdWaitExternal
static BOOL func_ov094_021fc354(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->paused = TRUE;
    VM_SetNativeCallback(vm, func_ov094_021fc9e4);
    wk->ret = TRUE;
    return TRUE;
}

// pokebw2's BtlvEffvm_CmdChangeScript
static BOOL func_ov094_021fc374(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int effect = VM_Read32(vm);
    void *script;

    GFL_HeapFree(wk->script);
    if (effect < BTLV_EFFECT_FIRST) {
        wk->script = GFL_ArcSysReadHeapNew(66, effect, HEAPID_TAIL(wk->heapId));
    } else {
        wk->script = GFL_ArcSysReadHeapNew(67, effect - BTLV_EFFECT_FIRST, HEAPID_TAIL(wk->heapId));
    }
    script = wk->script;
    wk->value = 0;
    VM_Jump(vm, (u8 *)script + ((u32 *)script)[1]);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdWaitSide
static BOOL func_ov094_021fc3d8(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    u32 side = VM_Read32(vm);
    BtlvEffvmSideWork *work =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmSideWork), FALSE, "btlv_effvm.c", 4688);

    work->wk = wk;
    work->side = side;
    work->doneMask = 0;
    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe930, work, 0), NULL, 1);
    return wk->ret;
}

// pokebw2's BtlvEffvm_CmdMcssReset
static BOOL func_ov094_021fc434(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    func_ov094_021f8934(VM_Read32(vm));
    return wk->ret;
}

static BOOL func_ov094_021fc448(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int i;
    BOOL visible;

    for (i = 0; i < 16; i++) {
        if (wk->particles[i] != NULL) {
            func_ov094_021fdb08(wk->particles[i]);
            wk->particles[i] = NULL;
        }
        wk->particleIds[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        if (wk->clactIndexes[i] != -1) {
            func_ov094_02201ca4(func_ov094_021f86f0(), wk->clactIndexes[i]);
            wk->clactIndexes[i] = -1;
        }
    }
    for (i = 0; i < wk->allocCount; i++) {
        GFL_HeapFree(wk->allocs[i]);
    }
    wk->allocCount = 0;
    func_ov094_021f8a10(1);
    func_ov094_021fe4f8(wk);
    func_ov094_021fe520(wk);
    if (!wk->unk0_18) {
        func_ov094_021f8934(1);
    }
    VM_Halt(vm);
    wk->ret = TRUE;
    if (wk->bgMoved) {
        BtlvScu *scu = func_ov094_021f8764();

        if (scu != NULL) {
            func_ov093_021eb2a8(scu);
        }
        GFL_BGSysSetBGPriority(3, 0);
        wk->bgMoved = FALSE;
        GFL_BGSysMoveBG(3, 0, wk->bgX);
        GFL_BGSysMoveBG(3, 3, wk->bgY);
    }
    if (wk->alphaBlend) {
        G2_SetBlendAlpha(2, 0x3d, 0x1f, 7);
        wk->alphaBlend = FALSE;
    }
    if (wk->sePlayed) {
        GFL_SndStop();
    }
    visible = FALSE;
    if (func_ov094_021ffd9c(func_ov094_021f8694(), wk->attacker)) {
        if (((wk->move == 248 || wk->move == 353) && wk->param.variant == 1) || wk->move == 144 || wk->move == 119 ||
            wk->move == 267) {
        } else {
            visible = func_ov094_021fff38(func_ov094_021f8694(), wk->attacker) & 1;
        }
    }
    if (wk->savedScript != NULL) {
        u32 offset;
        void *script;

        GFL_HeapFree(wk->script);
        script = wk->savedScript;
        wk->script = script;
        wk->savedScript = NULL;
        wk->isEffect = FALSE;
        offset = wk->savedOffset;
        GFL_BGSysSetBGEnabled(1, FALSE);
        GFL_BGSysSetBGEnabled(2, FALSE);
        GFL_BGSysSetBGEnabled(3, FALSE);
        wk->value = 0;
        VM_LoadScript(vm, (u8 *)wk->script + *(u32 *)((u8 *)script + offset));
        return TRUE;
    }
    if (wk->substituteRestore == TRUE && wk->move == 0x291) {
        GFL_HeapFree(wk->script);
        wk->move = 52;
        wk->script = GFL_ArcSysReadHeapNew(67, wk->move, HEAPID_TAIL(wk->heapId));
        wk->isEffect = TRUE;
        wk->value = 0;
        wk->substituteRestore = FALSE;
        VM_LoadScript(vm, (u8 *)wk->script + ((u32 *)wk->script)[1]);
        return TRUE;
    }
    if ((wk->move == 0x286 || !wk->isEffect) && wk->move != 144 && wk->move != 226 && !wk->substituteRestore &&
        visible) {
        if (wk->move == 0x1f6) {
            func_ov094_021ff374(func_ov094_021f8694(), wk->attacker, 0);
        }
        GFL_HeapFree(wk->script);
        wk->move = 52;
        wk->script = GFL_ArcSysReadHeapNew(67, wk->move, HEAPID_TAIL(wk->heapId));
        wk->isEffect = TRUE;
        wk->value = 0;
        wk->substituteRestore = FALSE;
        VM_LoadScript(vm, (u8 *)wk->script + ((u32 *)wk->script)[1]);
        return TRUE;
    }
    if (wk->move != 0x27e) {
        GFL_BGSysSetBGEnabled(1, TRUE);
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysSetBGEnabled(3, TRUE);
    }
    return TRUE;
}

static BOOL func_ov094_021fc744(VM *vm, void *env) {
    BtlvEffvm *wk = env;
    int i;
    BtlvMcssPos pos;

    if (wk->waitKind <= 1) {
        if (func_ov094_021f722c(func_ov094_021f8684()) == 1) {
            return FALSE;
        }
        if (wk->cameraReset) {
            wk->cameraReset = 0;
            wk->unk244 = 1;
            func_ov094_021ff16c(func_ov094_021f8694());
        }
    }
    if (wk->waitKind == 0 || wk->waitKind == 3) {
        for (pos = BTLV_MCSS_POS_FIRST; pos < BTLV_MCSS_POS_MAX; pos++) {
            if (func_ov094_021ffcb0(func_ov094_021f8694(), pos) == TRUE) {
                return FALSE;
            }
        }
    }
    if (wk->waitKind == 4) {
        for (pos = BTLV_MCSS_POS_TRAINER; pos < BTLV_MCSS_POS_MAX; pos++) {
            if (func_ov094_021ffd9c(func_ov094_021f8694(), pos) != 0) {
                if (func_ov094_02200494(func_ov094_021f8694(), pos) == TRUE) {
                    return FALSE;
                }
            }
        }
    }
    if (wk->waitKind == 0 || wk->waitKind == 5) {
        if (func_ov094_0220985c(func_ov094_021f8700()) == TRUE) {
            return FALSE;
        }
    }
    if (wk->waitKind == 0 || wk->waitKind == 2) {
        for (i = 0; i < 16; i++) {
            if (wk->particles[i] != NULL && func_02051614(wk->particles[i]) != 0) {
                return FALSE;
            }
        }
    }
    if (wk->waitKind == 0 || wk->waitKind == 6 || wk->waitKind == 8) {
        if (func_ov094_021f8324(0) != 0) {
            return FALSE;
        }
    }
    if (wk->waitKind == 0 || wk->waitKind == 7 || wk->waitKind == 8) {
        if (func_ov094_021f8324(1) != 0) {
            return FALSE;
        }
    }
    if (wk->waitKind == 0 || wk->waitKind == 9) {
        if (func_ov094_021f8324(3) != 0) {
            return FALSE;
        }
    }
    if (wk->waitKind == 10) {
        if (GFL_SndPlayerIsActiveAny() || wk->seDelayed || wk->seSliding) {
            return FALSE;
        }
    }
    if (wk->waitKind == 11) {
        if (GFL_SndPlayerIsActive(1) || wk->seDelayed || wk->seSliding) {
            return FALSE;
        }
    }
    if (wk->waitKind == 12) {
        if (GFL_SndPlayerIsActive(2) || wk->seDelayed || wk->seSliding) {
            return FALSE;
        }
    }
    if (wk->waitKind == 13) {
        if (GFL_SndPlayerIsActive(4) || wk->seDelayed || wk->seSliding) {
            return FALSE;
        }
    }
    if (wk->waitKind == 14) {
        if (GFL_SndPlayerIsActive(3) || wk->seDelayed || wk->seSliding) {
            return FALSE;
        }
    }
    if (wk->waitKind == 15) {
        if (GFL_SndPlayerIsActive(0) || wk->seDelayed || wk->seSliding) {
            return FALSE;
        }
    }
    if (wk->waitKind == 16) {
        if (wk->cryDelayed) {
            return FALSE;
        }
        for (i = 0; i < 6; i++) {
            if (wk->voices[i] != -1) {
                if (PokeVoice_IsPlaying(wk->voices[i])) {
                    return FALSE;
                }
                wk->voices[i] = -1;
            }
        }
    }
    if ((wk->waitKind == 0 || wk->waitKind == 17) && wk->windowTask) {
        return FALSE;
    }
    return TRUE;
}



// pokebw2's BtlvEffvm_WaitTimerNative
static BOOL func_ov094_021fc9d0(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    wk->waitTimer--;
    return wk->waitTimer <= 0;
}

// pokebw2's BtlvEffvm_WaitResumeNative
static BOOL func_ov094_021fc9e4(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    if (wk->paused) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's BtlvEffvm_WaitParticleLoadNative
static BOOL func_ov094_021fc9f4(VM *vm, void *env) {
    BtlvEffvm *wk = env;

    if (wk->particleLoading) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's BtlvEffvm_GetTargetPositions
static int func_ov094_021fca04(BtlvEffvm *wk, int code, int *positions) {
    int count = 1;
    int rule;
    int i;
    BtlvMcssPos pos;
    int start;
    int end;

    switch (code) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        positions[0] = code;
        break;
    case 14:
        positions[0] = wk->attacker;
        if (positions[0] == 0xff) {
            if (func_ov094_021f7f1c(0) == TRUE) {
                positions[0] = 0;
            } else if (func_ov094_021f7f1c(2) == TRUE) {
                positions[0] = 2;
            } else if (func_ov094_021f7f1c(4) == TRUE) {
                positions[0] = 4;
                // BUG: tests position 4 again where position 6 is meant
#ifdef BUGFIX
            } else if (func_ov094_021f7f1c(6) == TRUE) {
#else
            } else if (func_ov094_021f7f1c(4) == TRUE) {
#endif
                positions[0] = 6;
            }
        }
        break;
    case 15:
        if (wk->attacker > 1) {
            positions[0] = wk->attacker ^ 2;
        } else {
            positions[0] = 0xff;
        }
        break;
    case 16:
        positions[0] = wk->defender;
        if (positions[0] != 0xff) {
            break;
        }
        rule = func_ov094_021f8710();
        count = 0;
        if (rule == 0 || rule == 3) {
            if (wk->param.unk0 == 8 || wk->param.unk0 == 10) {
                count = 1;
                positions[0] = wk->attacker;
                if (func_ov094_021f7f1c(wk->attacker ^ 1)) {
                    positions[1] = wk->attacker ^ 1;
                    count++;
                }
            } else {
                count = 0;
                if (func_ov094_021f7f1c(wk->attacker ^ 1)) {
                    positions[0] = wk->attacker ^ 1;
                    count++;
                }
            }
            break;
        }
        switch (wk->param.unk0) {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            if (rule == 2 && wk->attacker != 4 && wk->attacker != 5) {
                int targets[4][3] = {
#include "asset/btlv_effvm_targets_021fca04.inc"
                };
                int row = (wk->attacker & 1) + (wk->attacker > 3 ? 2 : 0);

                for (i = 0; i < 3; i++) {
                    if (func_ov094_021f7f1c(targets[row][i])) {
                        positions[count] = targets[row][i];
                        count++;
                    }
                }
            } else {
                for (pos = 2; pos <= 7; pos++) {
                    if (wk->attacker != pos && func_ov094_021f7f1c(pos)) {
                        positions[count] = pos;
                        count++;
                    }
                }
            }
            break;
        case 5:
        case 7:
        case 9:
        case 13:
            start = (wk->attacker & 1) ? 2 : 3;
            end = rule == 2 ? 7 : 5;
            if (rule == 2) {
                if (wk->attacker <= 3) {
                    start += 2;
                }
                if (wk->attacker >= 6) {
                    end -= 2;
                }
            }
            for (pos = start; pos <= end; pos += 2) {
                if (func_ov094_021f7f1c(pos)) {
                    positions[count] = pos;
                    count++;
                }
            }
            break;
        case 11:
            for (pos = (wk->attacker & 1) ? 2 : 3; pos <= 7; pos += 2) {
                if (func_ov094_021f7f1c(pos)) {
                    positions[count] = pos;
                    count++;
                }
            }
            break;
        case 6:
        case 12:
            for (pos = (wk->attacker & 1) ? 3 : 2; pos <= 7; pos += 2) {
                if (func_ov094_021f7f1c(pos)) {
                    positions[count] = pos;
                    count++;
                }
            }
            break;
        case 8:
        case 10:
            for (pos = 2; pos <= 7; pos++) {
                if (func_ov094_021f7f1c(pos)) {
                    positions[count] = pos;
                    count++;
                }
            }
            break;
        }
        break;
    case 17:
        if (wk->defender > 1) {
            positions[0] = wk->defender ^ 2;
        } else {
            positions[0] = 0xff;
        }
        break;
    case 18:
        count = 0;
        for (pos = 0; pos <= 7; pos++) {
            if (func_ov094_021f7f1c(pos)) {
                positions[count] = pos;
                count++;
            }
        }
        break;
    case 19:
    case 20:
        start = 0;
        if (code != 19) {
            start = 1;
        }
        count = 0;
        for (pos = start; pos <= 7; pos += 2) {
            if (func_ov094_021f7f1c(pos)) {
                positions[count] = pos;
                count++;
            }
        }
        break;
    }
    if (positions[0] != 0xff && positions[0] < 8) {
        if (func_ov094_021f7f1c(positions[0]) == TRUE) {
            positions[0] = func_ov094_021fd04c(wk, positions[0]);
        } else {
            positions[0] = 0xff;
            count = 0;
        }
    }
    return count;
}

static s32 data_ov094_0220ad38[8] = { 0, 0, 17, 16, 0, 0, 15, 14 };

// pokebw2's BtlvEffvm_GetDefenderPositions
static int func_ov094_021fcd14(BtlvEffvm *wk, int code, int *positions) {
    int count = 1;
    int rule;
    int i;
    int side;
    BtlvMcssPos pos;

    positions[0] = wk->defender;
    if (positions[0] == 0xff) {
        rule = func_ov094_021f8710();
        count = 0;
        if (rule == 0 || rule == 3) {
            if (wk->param.unk0 == 8 || wk->param.unk0 == 10) {
                count = 1;
                positions[0] = wk->attacker;
                if (func_ov094_021f7f1c(wk->attacker ^ 1)) {
                    positions[1] = wk->attacker ^ 1;
                    count++;
                }
            } else {
                count = 0;
                if (func_ov094_021f7f1c(wk->attacker ^ 1)) {
                    positions[0] = wk->attacker ^ 1;
                    count++;
                }
            }
        } else {
            switch (wk->param.unk0) {
            case 0:
            case 1:
            case 2:
            case 3:
                break;
            case 4:
                if (rule == 2 && wk->attacker != 4 && wk->attacker != 5) {
                    int targets[4][3] = {
#include "asset/btlv_effvm_targets_021fcd14.inc"
                    };
                    int row = (wk->attacker & 1) + (wk->attacker > 3 ? 2 : 0);

                    for (i = 0; i < 3; i++) {
                        if (func_ov094_021f7f1c(targets[row][i]) &&
                            !func_ov094_021ff348(func_ov094_021f8694(), targets[row][i])) {
                            positions[count] = targets[row][i];
                            count++;
                        }
                    }
                } else {
                    for (pos = 2; pos <= 7; pos++) {
                        if (wk->attacker != pos && func_ov094_021f7f1c(pos) &&
                            !func_ov094_021ff348(func_ov094_021f8694(), pos)) {
                            positions[count] = pos;
                            count++;
                        }
                    }
                }
                break;
            case 5:
            case 7:
            case 9:
            case 13:
                if (rule == 1) {
                    side = 0;
                    if (!(wk->attacker & 1)) {
                        side = 1;
                    }
                    positions[0] = side;
                    count = 1;
                } else if (wk->attacker != 4 && wk->attacker != 5) {
                    positions[0] = data_ov094_0220ad18[wk->attacker];
                    count = 1;
                } else {
                    count = 1;
                    positions[0] = wk->attacker ^ 1;
                }
                break;
            case 11:
                if (rule == 1) {
                    side = 0;
                    if (!(wk->attacker & 1)) {
                        side = 1;
                    }
                    positions[0] = side;
                    count = 1;
                } else if (wk->attacker != 4 && wk->attacker != 5) {
                    positions[0] = data_ov094_0220ad38[wk->attacker];
                    count = 1;
                } else {
                    count = 1;
                    positions[0] = wk->attacker ^ 1;
                }
                break;
            case 6:
            case 12:
                for (pos = (wk->attacker & 1) ? 3 : 2; pos <= 7; pos += 2) {
                    if (func_ov094_021f7f1c(pos) && !func_ov094_021ff348(func_ov094_021f8694(), pos)) {
                        positions[count] = pos;
                        count++;
                    }
                }
                break;
            case 8:
            case 10:
                for (pos = 2; pos <= 7; pos++) {
                    if (func_ov094_021f7f1c(pos) && !func_ov094_021ff348(func_ov094_021f8694(), pos)) {
                        positions[count] = pos;
                        count++;
                    }
                }
                break;
            }
        }
    }
    return count;
}

// pokebw2's BtlvEffvm_GetTargetPosition
static int func_ov094_021fcf2c(VM *vm, int code) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    int pos;

    switch (code) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        pos = code;
        break;
    case 14:
        pos = wk->attacker;
        if (pos == 0xff) {
            if (func_ov094_021f7f1c(2) == TRUE) {
                return 2;
            }
            if (func_ov094_021f7f1c(4) == TRUE) {
                return 4;
            }
            if (func_ov094_021f7f1c(6) == TRUE) {
                return 6;
            }
            return 0;
        }
        break;
    case 15:
        if (wk->attacker > 1) {
            pos = wk->attacker ^ 2;
        } else {
            pos = 0xff;
        }
        break;
    case 16:
        pos = wk->defender;
        if (pos == 0xff) {
            if (wk->attacker & 1) {
                if (func_ov094_021f7f1c(2) == TRUE) {
                    return 2;
                }
                if (func_ov094_021f7f1c(4) == TRUE) {
                    return 4;
                }
                if (func_ov094_021f7f1c(6) == TRUE) {
                    return 6;
                }
                return 0;
            }
            if (func_ov094_021f7f1c(3) == TRUE) {
                return 3;
            }
            if (func_ov094_021f7f1c(5) == TRUE) {
                return 5;
            }
            if (func_ov094_021f7f1c(7) == TRUE) {
                return 7;
            }
            return 1;
        }
        break;
    case 17:
        if (wk->defender > 1) {
            pos = wk->defender ^ 2;
        } else {
            pos = 0xff;
        }
        break;
    }
    return func_ov094_021fd070(vm, pos);
}

// pokebw2's BtlvEffvm_CheckPosition
static int func_ov094_021fd04c(BtlvEffvm *wk, int pos) {
    if (func_ov094_021f7f1c(pos) == TRUE) {
        if (wk->flipSides) {
            pos ^= 1;
        }
    } else {
        pos = 0xff;
    }
    return pos;
}

// pokebw2's BtlvEffvm_SwapSide
static int func_ov094_021fd070(VM *vm, int pos) {
    BtlvEffvm *wk = VM_GetEnv(vm);

    if (wk->flipSides) {
        pos ^= 1;
    }
    return pos;
}

// pokebw2's BtlvEffvm_GetDefenderForEmitter
static int func_ov094_021fd088(VM *vm, int *pos) {
    BtlvEffvm *wk = VM_GetEnv(vm);
    int ret = func_ov094_021f8710();

    *pos = wk->defender;
    if (*pos == 0xff) {
        if (ret == 0 || ret == 3 || wk->isEffect == TRUE) {
            *pos = wk->attacker ^ 1;
        } else {
            switch (wk->param.unk0) {
            case 4:
            case 5:
            case 8:
            case 10:
                if (ret == 1) {
                    ret = 0;
                    *pos = (wk->attacker & 1) ? ret : 1;
                } else if (wk->attacker != 4 && wk->attacker != 5) {
                    ret = 1;
                    *pos = data_ov094_0220ad58[wk->attacker];
                } else {
                    *pos = wk->attacker ^ 1;
                }
                break;
            case 7:
            case 9:
            case 13:
                *pos = wk->attacker ^ 1;
                break;
            case 11:
                ret = 0;
                *pos = (wk->attacker & 1) ? ret : 1;
                break;
            case 6:
            case 12:
                ret = 0;
                *pos = (wk->attacker & 1) ? 1 : ret;
                break;
            }
        }
    }
    return ret;
}

// pokebw2's BtlvEffvm_FindOrAddParticleId
static BOOL func_ov094_021fd15c(BtlvEffvm *wk, u32 id, int *index) {
    int i;
    BOOL isNew = TRUE;

    for (i = 0; i < 16; i++) {
        if (id == wk->particleIds[i]) {
            isNew = FALSE;
            break;
        }
    }
    if (i == 16) {
        for (i = 0; i < 16; i++) {
            if (wk->particleIds[i] == -1) {
                wk->particleIds[i] = id;
                break;
            }
        }
    }
    *index = i;
    return isNew;
}

// pokebw2's BtlvEffvm_SetEmitterCount
static void func_ov094_021fd19c(BtlvEffvm *wk, int index, void *resource) {
    wk->emitterCounts[index] = ((u16 *)resource)[4];
}

// pokebw2's BtlvEffvm_FindParticleId
static int func_ov094_021fd1a8(BtlvEffvm *wk, u32 id) {
    int i;

    for (i = 0; i < 16; i++) {
        if (id == wk->particleIds[i]) {
            break;
        }
    }
    return i;
}

// pokebw2's BtlvEffvm_GetEmitterCount
static u32 func_ov094_021fd1c0(BtlvEffvm *wk, int index) {
    return wk->emitterCounts[index];
}

// pokebw2's BtlvEffvm_InitEmitter
static void func_ov094_021fd1cc(SPLEmitter *emitter) {
    BtlvEffvmEmitParam *param = func_02051774();
    BOOL flip = param->flip;
    VecFx32 from;
    VecFx32 to;
    VecFx32 start;
    VecFx32 axis;
    VecFx32 dir;
    VecFx32 base;
    MtxFx43 mtx;
    VecFx32 vec;
    VecFx32 flat;
    VecFx32 rotAxis;
    VecFx32 cameraPos;
    u16 spinAxis;
    VecFx16 emitterAxis;
    u16 angle;
    u32 rule;

    switch (param->from) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        param->from = func_ov094_021fd070(param->vm, param->from);
        break;
    case 9:
    case 11:
        param->from = func_ov094_021fcf2c(param->vm, param->from + 5);
        break;
    case 8:
        from.x = param->fromPos.x;
        from.y = param->fromPos.y;
        from.z = param->fromPos.z;
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        func_ov094_021ff3fc(func_ov094_021f8694(), &from, param->from - 12, 1);
        param->from = -1;
        break;
    case 18:
    case 19:
    case 20:
    case 21:
        func_ov094_021ff3fc(func_ov094_021f8694(), &from, param->from - 16, 1);
        switch (param->from) {
        case 18:
            from.z += 0x1a00;
            break;
        case 19:
            from.z += 0x2600;
            break;
        case 20:
            from.z += 0x600;
            break;
        case 21:
            from.z += 0x3900;
            break;
        }
        param->from = -1;
        break;
    }
    if (param->from != -1 && param->from != 8) {
        func_ov094_021ff3f4(func_ov094_021f8694(), &from, param->from);
    }
    switch (param->to) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        param->to = func_ov094_021fd070(param->vm, param->to);
        break;
    case 9:
        param->to = func_ov094_021fcf2c(param->vm, param->to + 5);
        break;
    case 10:
        flip = TRUE;
        param->to = func_ov094_021fcf2c(param->vm, param->to + 4);
        break;
    case 12:
        flip = TRUE;
    case 11:
        rule = func_ov094_021fd088(param->vm, &param->to);
        func_ov094_021ff3fc(func_ov094_021f8694(), &to, param->to, rule);
        param->to = -1;
        break;
    case 8:
        to.x = param->toPos.x;
        to.y = param->toPos.y;
        to.z = param->toPos.z;
        break;
    case 14:
    case 15:
    case 16:
    case 17:
        func_ov094_021ff3fc(func_ov094_021f8694(), &to, param->to - 12, 1);
        param->to = -1;
        break;
    case 18:
    case 19:
    case 20:
    case 21:
        func_ov094_021ff3fc(func_ov094_021f8694(), &to, param->to - 16, 1);
        // The start's code, not the target's, picks the height, and the start's is no longer 18-21 by now
        switch (param->from) {
        case 18:
            to.z += 0x1a00;
            break;
        case 19:
            to.z += 0x2600;
            break;
        case 20:
            to.z += 0x600;
            break;
        case 21:
            to.z += 0x3900;
            break;
        }
        param->to = -1;
        break;
    }
    if (param->to != -1 && param->to != 8) {
        func_ov094_021ff3f4(func_ov094_021f8694(), &to, param->to);
    }
    func_ov094_021f8710();
    from.z += 0x500;
    to.z += 0x500;
    if (param->offsetMode == 1 || param->offsetMode == 2) {
        func_ov094_021fe1b8(&from, &param->offset);
        func_ov094_021fe1b8(&to, &param->offset);
    } else if (param->offsetMode == 3) {
        start.x = from.x;
        start.y = from.y;
        start.z = from.z;
        from.y += param->offset.y;
        func_ov094_021fe1b8(&start, &param->offset);
    } else {
        from.y += param->offset.y;
    }
    to.y += param->offset.y;
    if (param->motionType != 0) {
        BtlvEffvm *wk = VM_GetEnv(param->vm);
        BtlvEffvmEmitterMotionWork *work;

        wk->allocs[wk->allocCount] =
            GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitterMotionWork), FALSE, "btlv_effvm.c", 6474);
        work = wk->allocs[wk->allocCount];
        func_02051a2c(emitter, work);
        func_02051a24(emitter, func_ov094_021fd8cc);
        wk->allocCount++;
        work->angle = 0;
        work->wait = 0;
        work->waitAccum = 0;
        work->waitStep = 0;
        work->type = param->motionType;
        work->frames = param->motionFrames >> FX32_SHIFT;
        work->offsetMode = param->offsetMode;
        if (param->motionType == 3) {
            work->angleStep = FX_Div(0x6000 * FX32_ONE, param->motionFrames);
        } else if (param->motionType == 5 || param->motionType == 6) {
            work->angleStep = FX_Div(0x8000 * FX32_ONE, param->motionFrames);
            work->angle2Step = FX_Div(0x10000 * FX32_ONE, param->motionFrames);
            work->angle2Step = FX_Mul(work->angle2Step, param->turns);
        } else {
            work->angleStep = FX_Div(0x8000 * FX32_ONE, param->motionFrames);
        }
        if (work->angleStep == 0) {
            work->angleStep = 1;
        }
        work->radius = VEC_Distance(&from, &to) / 2;
        if (param->motionType == 1 || param->motionType == 4) {
            work->height = 0;
        } else {
            work->height = param->motionHeight;
        }
        if (param->motionType == 4) {
            fx32 x;
            fx32 y;
            fx32 z;

            x = from.x;
            from.x = to.x;
            y = from.y;
            from.y = to.y;
            z = from.z;
            from.z = to.z;
            to.x = x + from.x;
            to.y = y + from.y;
            to.z = z + from.z;
        }
        dir.x = to.x - from.x;
        dir.y = to.y - from.y;
        dir.z = to.z - from.z;
        VEC_Normalize(&dir, &dir);
        base.x = FX32_ONE;
        base.y = 0;
        base.z = 0;
        angle = FX_AcosIdx(VEC_DotProduct(&base, &dir));
        VEC_CrossProduct(&base, &dir, &axis);
        VEC_Normalize(&axis, &axis);
        MTX_RotAxis43(&work->mtx, &axis, FX_SinIdx(angle), FX_CosIdx(angle));
        work->mtx._30 = from.x;
        work->mtx._31 = from.y;
        work->mtx._32 = from.z;
    }
    if (param->from != 8 && param->to != 8 && param->from != param->to) {
        spinAxis = 2;
        to.x -= from.x;
        to.y -= from.y;
        to.z -= from.z;
        func_020519c4(emitter, &to);
        func_02051a04(emitter, &to);
        flat.x = to.x;
        flat.y = 0;
        flat.z = to.z;
        func_020517b4(emitter, &emitterAxis);
        vec.x = emitterAxis.x;
        vec.y = 0;
        vec.z = emitterAxis.z;
        VEC_Normalize(&vec, &vec);
        VEC_Normalize(&flat, &flat);
        angle = FX_AcosIdx(VEC_DotProduct(&vec, &flat));
        VEC_CrossProduct(&vec, &flat, &rotAxis);
        VEC_Normalize(&rotAxis, &rotAxis);
        MTX_RotAxis43(&mtx, &rotAxis, FX_SinIdx(angle), FX_CosIdx(angle));
        vec.x = emitterAxis.x;
        vec.y = emitterAxis.y;
        vec.z = emitterAxis.z;
        MTX_MultVec43(&vec, &mtx, &vec);
        VEC_Normalize(&vec, &vec);
        if (flip == TRUE) {
            vec.x *= -1;
            vec.z *= -1;
        }
        emitterAxis.x = vec.x;
        emitterAxis.y = vec.y;
        emitterAxis.z = vec.z;
        if (angle < 0x2000 || (angle > 0x6000 && angle < 0xa000) || angle > 0xe000) {
            spinAxis = 0;
        }
        func_020519e4(emitter, &spinAxis);
        func_02051880(emitter, &emitterAxis);
    }
    if ((param->offsetMode != 0 && (param->from & 1)) || param->offsetMode == 0 || param->offsetMode == 2 ||
        param->offsetMode == 3) {
        fx32 radius = func_020517d0(emitter);
        fx32 lifeTime = func_02051830(emitter) << FX32_SHIFT;
        fx16 scale = func_02051820(emitter);
        fx32 length = func_020517e0(emitter);
        fx16 amplifier;

        if (param->radiusScale != 0) {
            func_020517d8(emitter, FX_Mul(radius, param->radiusScale));
            func_020517e8(emitter, FX_Mul(length, param->radiusScale));
        }
        if (param->lifeTimeScale != 0) {
            func_020518a4(emitter, FX_Mul(lifeTime, param->lifeTimeScale) >> FX32_SHIFT);
        }
        if (param->scaleScale != 0) {
            func_0205189c(emitter, (fx16)FX_Mul(scale, param->scaleScale));
        }
        if (param->cameraScale != 0) {
            // func_02051790 and func_020517a4 take the emitter here, though gfl/particle.h types them for a system
            func_02051790((ParticleSystem *)emitter, &cameraPos);
            cameraPos.x = FX_MUL(cameraPos.x, param->cameraScale);
            cameraPos.y = FX_MUL(cameraPos.y, param->cameraScale);
            cameraPos.z = FX_MUL(cameraPos.z, param->cameraScale);
            func_020517a4((ParticleSystem *)emitter, &cameraPos);
            amplifier = func_020517f0(emitter);
            func_02051800(emitter, (fx16)FX_MUL(amplifier, param->cameraScale));
            amplifier = func_02051808(emitter);
            func_02051818(emitter, (fx16)FX_MUL(amplifier, param->cameraScale));
        }
    }
    if (param->offsetMode == 3) {
        func_02051838(emitter, &start);
    } else {
        func_02051838(emitter, &from);
    }
    GFL_HeapFree(param);
}

// pokebw2's BtlvEffvm_MoveEmitterArc
static void func_ov094_021fd8cc(SPLEmitter *emitter, u32 type) {
    BtlvEffvmEmitterMotionWork *work = func_02051a34(emitter);
    VecFx32 pos;
    u16 angle;
    u16 angle2;

    if (work->frames != 0 && type != 0) {
        if (work->type == 3) {
            if (work->wait != 0) {
                work->wait--;
                return;
            }
            work->waitStep += 12;
            work->waitAccum += work->waitStep;
            work->wait = work->waitAccum >> FX32_SHIFT;
        }
        work->frames--;
        work->angle += work->angleStep;
        work->angle2 += work->angle2Step;
        angle = (work->angle >> FX32_SHIFT) + 0xc000;
        angle2 = (work->angle2 >> FX32_SHIFT) + 0xc000;
        pos.x = FX_SinIdx(angle);
        pos.x = FX_Mul(pos.x, work->radius);
        pos.x += work->radius;
        if (work->type == 6) {
            pos.z = FX_CosIdx(angle2);
            pos.z = FX_Mul(pos.z, work->height);
            pos.y = 0;
        } else if (work->type == 5) {
            pos.y = FX_CosIdx(angle2);
            pos.y = FX_Mul(pos.y, work->height);
            pos.z = 0;
        } else {
            pos.y = FX_CosIdx(angle);
            pos.y = FX_Mul(pos.y, work->height);
            pos.z = 0;
        }
        MTX_MultVec43(&pos, &work->mtx, &pos);
        if (work->offsetMode != 0) {
            func_ov094_021fe1b8(&pos, NULL);
        }
        func_02051838(emitter, &pos);
    }
}

// pokebw2's BtlvEffvm_InitCircleEmitter
static void func_ov094_021fd9f0(SPLEmitter *emitter) {
    BtlvEffvmEmitterCircleWork *work = func_02051774();

    if (work->ortho && work->center.z < 0) {
        func_0205189c(emitter, FX32_HALF);
    }
    func_02051a2c(emitter, work);
    func_02051a24(emitter, func_ov094_021fda28);
}

// pokebw2's BtlvEffvm_MoveCircleEmitter
static void func_ov094_021fda28(SPLEmitter *emitter, u32 type) {
    BtlvEffvmEmitterCircleWork *work = func_02051a34(emitter);
    VecFx32 pos;

    if (type != 0) {
        if (work->delay == 0) {
            if (work->wait == 0) {
                work->wait = work->waitReset;
                if (work->type & 1) {
                    work->angle = work->angle - work->angleStep;
                } else {
                    work->angle = work->angle + work->angleStep;
                }
                work->angle = (u16)work->angle;
                pos.x = FX_SinIdx(work->angle);
                pos.x = FX_Mul(pos.x, work->radiusX);
                pos.x += work->center.x;
                pos.y = work->center.y;
                pos.z = FX_CosIdx(work->angle);
                pos.z = FX_Mul(pos.z, work->radiusZ);
                pos.z += work->center.z;
                if (work->ortho) {
                    func_ov094_021fe1b8(&pos, NULL);
                }
                func_02051838(emitter, &pos);
                if (work->frames != 0) {
                    work->frames--;
                } else {
                    work->frames = work->framesReset;
                    work->delay = work->delayReset;
                }
            } else {
                work->wait--;
            }
        } else {
            work->delay--;
        }
    }
}

// pokebw2's BtlvEffvm_DeleteParticle
static void func_ov094_021fdb08(ParticleSystem *particle) {
    void *work = func_02051640(particle);

    func_02050db8(particle);
    GFL_HeapFree(work);
}

// pokebw2's BtlvEffvm_ResetMcss
static void func_ov094_021fdb20(BtlvEffvm *wk) {
    if (wk->unk244 == 1) {
        func_ov094_021ff1b0(func_ov094_021f8694());
    } else {
        func_ov094_021ff16c(func_ov094_021f8694());
    }
}

// pokebw2's BtlvEffvm_PlaySENow
static void func_ov094_021fdb40(u32 se, u32 player, int pan, int arg3, int arg4, int vol, int pitch) {
    if (player == 5) {
        GFL_SndSEPlay(se);
        player = GFL_SndSeqGetPlayerIndex(se);
    } else {
        GFL_SEPlayKeepVol(se, player);
    }
    NNS_SndPlayerSetVolume(func_02006138(player), arg4);
    GFL_SndPlayerSetParams(player, -1, -1, pan);
    GFL_SndPlayerSetParams(player, -1, arg3, -1);
    if (vol != 0) {
        NNS_SndPlayerSetTrackModDepth(func_02006138(player), 0xffff, vol);
        NNS_SndPlayerSetTrackModSpeed(func_02006138(player), 0xffff, pitch);
    }
}

static u32 func_ov094_021fdbb0(BtlvEffvm *wk, u32 kind) {
    BtlvMcss *mcss;

    switch (kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        mcss = func_ov094_021f8694();
        return func_ov094_021fff0c(mcss, kind);
    case 8:
        mcss = func_ov094_021f8694();
        return func_ov094_021fff0c(mcss, wk->attacker);
    case 9:
        return wk->param.unk0;
    case 10:
        return wk->param.variant;
    case 11:
        return wk->param.unk2;
    case 12:
        return wk->param.unk3;
    case 13:
        return wk->param.unk4;
    case 14:
        return wk->param.unk8;
    case 15:
        return wk->param.itemNo;
    case 16:
        return wk->value;
    case 17:
        return wk->attacker;
    case 18:
        return func_ov094_021ff348(func_ov094_021f8694(), wk->attacker);
    case 19:
        return wk->attacker & 1;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        mcss = func_ov094_021f8694();
        return (func_ov094_021fff38(mcss, kind - 0x14) & 2) != 0;
    case 28:
        mcss = func_ov094_021f8694();
        return (func_ov094_021fff38(mcss, wk->attacker) & 2) != 0;
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
        mcss = func_ov094_021f8694();
        return func_ov094_021fff68(mcss, kind - 0x1d);
    case 37:
        mcss = func_ov094_021f8694();
        return func_ov094_021fff68(mcss, wk->attacker);
    case 38:
        return func_ov094_021f8730();
    case 39:
        return func_ov094_021f8710();
    case 40:
    case 41:
    case 42:
    case 43:
        return func_ov094_021f8740(kind - 0x28);
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
        mcss = func_ov094_021f8694();
        return func_ov094_021fffac(mcss, kind - 0x2c);
    case 52:
        mcss = func_ov094_021f8694();
        return func_ov094_021fffac(mcss, wk->attacker);
    case 53:
        return wk->mcssScaled;
    case 54:
        return wk->camSaved;
    case 55:
        return wk->unk2A0;
    case 56:
        return wk->cameraLocked;
    case 57:
        return wk->defender;
    }
}


// pokebw2's BtlvEffvm_AddEmitters
static void func_ov094_021fdd60(BtlvEffvm *wk, BtlvEffvmEmitParam *param, int index, int resourceId) {
    int i;
    BtlvEffvmEmitParam *emit;

    if (param->to == param->from && (param->to == 11 || param->to == 12)) {
        int positions[8];
        int count;

        if (param->to == 12) {
            param->flip = TRUE;
        }
        count = func_ov094_021fcd14(wk, 16, positions);
        if (count != 0) {
            for (i = 0; i < count; i++) {
                emit =
                    GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 7182);
                *emit = *param;
                emit->from = positions[i];
                emit->to = positions[i];
                if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, emit) == (SPLEmitter *)-1) {
                    GFL_HeapFree(emit);
                }
            }
        }
    } else if (param->to == param->from && wk->param.unk0 == 12 && !wk->isEffect && wk->move != 366 &&
               (param->to == 9 || param->to == 10)) {
        int targets[8];
        int count;

        if (param->to == 10) {
            param->flip = TRUE;
        }
        switch (func_ov094_021f8710()) {
        case 0:
        case 3:
        default:
            targets[0] = wk->attacker;
            count = 1;
            break;
        case 1:
            targets[0] = (wk->attacker & 1) ? 1 : 0;
            count = 1;
            break;
        case 2:
            targets[0] = (wk->attacker & 1) ? 19 : 18;
            targets[1] = (wk->attacker & 1) ? 21 : 20;
            count = 2;
            break;
        }
        if (count != 0) {
            for (i = 0; i < count; i++) {
                emit =
                    GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 7237);
                *emit = *param;
                emit->from = targets[i];
                emit->to = targets[i];
                if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, emit) == (SPLEmitter *)-1) {
                    GFL_HeapFree(emit);
                }
            }
        }
    } else {
        emit = GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitParam), TRUE, "btlv_effvm.c", 7254);
        *emit = *param;
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd1cc, emit) == (SPLEmitter *)-1) {
            GFL_HeapFree(emit);
        }
    }
}

// pokebw2's BtlvEffvm_AddCircleEmitter
static BOOL func_ov094_021fdfa0(VM *vm, BtlvEffvm *wk, BOOL ortho) {
    int index = func_ov094_021fd1a8(wk, func_ov094_021fe38c(wk, VM_Read32(vm)));
    int resourceId = VM_Read32(vm);
    int pos = 0;
    BtlvEffvmEmitterCircleWork *work;
    fx32 offsetY;

    wk->allocs[wk->allocCount] =
        GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmEmitterCircleWork), FALSE, "btlv_effvm.c", 7289);
    work = wk->allocs[wk->allocCount];
    wk->allocCount++;
    work->type = VM_Read32(vm);
    work->radiusX = VM_Read32(vm);
    work->radiusZ = VM_Read32(vm);
    offsetY = VM_Read32(vm);
    work->angle = 0;
    work->frames = VM_Read32(vm);
    work->framesReset = work->frames;
    work->wait = 0;
    work->waitReset = VM_Read32(vm);
    work->unk30 = VM_Read32(vm);
    work->delay = 0;
    work->delayReset = VM_Read32(vm);
    work->angleStep = 0x10000 / work->frames;
    work->ortho = ortho;
    if (index != 16) {
        if (ortho == TRUE && func_02051788(wk->particles[index]) == NULL) {
            G3DCameraProjection projection;
            VecFx32 camPos = { 0, 0, 0 };
            VecFx32 up = { 0, FX32_ONE, 0 };
            VecFx32 target = data_ov094_02209db4;

            projection.type = G3DCAM_PROJECTION_ORTHO;
            projection.param1 = FX32_CONST(3);
            projection.param2 = FX32_CONST(-3);
            projection.param3 = FX32_CONST(-4);
            projection.param4 = FX32_CONST(4);
            projection.near = FX32_ONE;
            projection.far = FX32_CONST(512);
            projection.ndcRangeOverride = FX32_ONE;
            func_02051648(wk->particles[index], &projection, 0x2000, &camPos, &up, &target, HEAPID_TAIL(wk->heapId));
        }
        switch (work->type) {
        case 0:
        case 1:
            pos = func_ov094_021fcf2c(vm, 14);
            func_ov094_021ff3f4(func_ov094_021f8694(), &work->center, pos);
            break;
        case 2:
        case 3:
            pos = func_ov094_021fcf2c(vm, 16);
            func_ov094_021ff3f4(func_ov094_021f8694(), &work->center, pos);
            break;
        case 4:
        case 5:
            work->center.x = 0;
            work->center.y = 0;
            work->center.z = 0;
            break;
        }
        if (pos & 1) {
            work->angle = 0x8000;
        }
        work->center.y += offsetY;
        if (func_020515cc(wk->particles[index], resourceId, func_ov094_021fd9f0, work) == (SPLEmitter *)-1) {
            GFL_HeapFree(work);
        }
    }
    return wk->ret;
}

void func_ov094_021fe1b8(VecFx32 *pos, const VecFx32 *offset) {
    MtxFx44 mtx;
    fx32 w;

    MTX_Copy43To44_(NNS_G3dGlbGetCameraMtx(), &mtx);
    MTX_Concat44(&mtx, NNS_G3dGlbGetProjectionMtx(), &mtx);
    if (offset != NULL) {
        pos->x += offset->x;
        pos->y += offset->y;
        pos->z += offset->z;
    }
    func_ov094_021fe254(pos, &mtx, pos, &w);
    pos->x = (((s64)FX_Div(pos->x, w) << 14) + 0x800) >> 12;
    pos->y = (((s64)FX_Div(pos->y, w) * 0x3000) + 0x800) >> 12;
    pos->z = -pos->z;
}


// pokebw2's BtlvEffvm_MulVec44
static void func_ov094_021fe254(const VecFx32 *src, const MtxFx44 *mtx, VecFx32 *dest, fx32 *w) {
    fx32 x = src->x;
    fx32 y = src->y;
    fx32 z = src->z;
    fx64 sum;

    sum = ((fx64)x * mtx->m[0][0] + (fx64)y * mtx->m[1][0] + (fx64)z * mtx->m[2][0]);
    dest->x = (fx32)(sum >> FX32_SHIFT) + mtx->m[3][0];
    sum = ((fx64)x * mtx->m[0][1] + (fx64)y * mtx->m[1][1] + (fx64)z * mtx->m[2][1]);
    dest->y = (fx32)(sum >> FX32_SHIFT) + mtx->m[3][1];
    sum = ((fx64)x * mtx->m[0][2] + (fx64)y * mtx->m[1][2] + (fx64)z * mtx->m[2][2]);
    dest->z = (fx32)(sum >> FX32_SHIFT) + mtx->m[3][2];
    sum = ((fx64)x * mtx->m[0][3] + (fx64)y * mtx->m[1][3] + (fx64)z * mtx->m[2][3]);
    *w = (fx32)(sum >> FX32_SHIFT) + mtx->m[3][3];
}

// pokebw2's BtlvEffvm_GetBallParticleId
static u32 func_ov094_021fe38c(BtlvEffvm *wk, u32 id) {
    u32 ball;
    u32 source = wk->ballSource;

    if (source == 9) {
        ball = func_ov094_022003c4(func_ov094_021f8694(), wk->attacker);
    } else if (source != 8) {
        ball = func_ov094_022003c4(func_ov094_021f8694(), source);
    } else {
        ball = PML_ItemGetMonsBallID(wk->param.itemNo);
    }
    if (ball == 0 || ball > 25) {
        ball = 4;
    }
    ball--;
    switch (id) {
    case 3:
        id += ball;
        break;
    case 29:
        if (ball + 1 == 25) {
            ball = 16;
        } else if (ball + 1 >= 16) {
            ball = 3;
        }
        id += ball;
        break;
    case 46:
    case 47:
    case 48:
    case 49:
        id += ball * 4;
        break;
    }
    return id;
}

// pokebw2's BtlvEffvm_StartBgmFade
static void func_ov094_021fe410(BtlvEffvm *wk, fx32 start, fx32 end, int frames) {
    BtlvEffvmBgmFadeWork *work;

    if (start > end) {
        if (wk->bgmLowered) {
            wk->bgmLowerRequested = TRUE;
            return;
        }
        wk->bgmLowered = TRUE;
    } else {
        wk->bgmLowered = FALSE;
    }
    work = GFL_HeapAllocate(HEAPID_TAIL(wk->heapId), sizeof(BtlvEffvmBgmFadeWork), FALSE, "btlv_effvm.c", 7544);
    work->volume = start;
    work->target = end;
    work->wk = wk;
    func_ov094_021f9154(work->volume, work->target, &work->step, FX32_CONST(frames));
    func_ov094_021f896c(GFL_TCBMgrAddTask(wk->tcbManager, func_ov094_021fe768, work, 0), NULL, 1);
}

// pokebw2's BtlvEffvm_GetFreeVoiceSlot
static int func_ov094_021fe4d0(BtlvEffvm *wk) {
    int i;

    for (i = 0; i < 6; i++) {
        if (wk->voices[i] == -1) {
            break;
        }
    }
    if (i == 6) {
        i = 0;
    }
    return i;
}

// pokebw2's BtlvEffvm_ResetMcssAll
static void func_ov094_021fe4f8(BtlvEffvm *wk) {
    BtlvMcssPos i;

    for (i = BTLV_MCSS_POS_FIRST; i < BTLV_MCSS_POS_TRAINER; i++) {
        if (func_ov094_021ffd9c(func_ov094_021f8694(), i) == TRUE) {
            func_ov094_022004bc(func_ov094_021f8694(), i);
        }
    }
}

// pokebw2's BtlvEffvm_ClearMcssFlagAll
static void func_ov094_021fe520(BtlvEffvm *wk) {
    BtlvMcssPos i;

    for (i = BTLV_MCSS_POS_FIRST; i < BTLV_MCSS_POS_TRAINER; i++) {
        if (func_ov094_021ffd9c(func_ov094_021f8694(), i) == TRUE) {
            func_ov094_021ff470(func_ov094_021f8694(), i, 0);
        }
    }
}

// pokebw2's BtlvEffvm_SeWaitTask
static void func_ov094_021fe54c(TCB *tcb, void *data) {
    BtlvEffvmSeWork *work = data;

    if (--work->wait == 0) {
        func_ov094_021fdb40(work->se, work->player, work->pan, work->arg4, work->arg5, work->vol, work->pitch);
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffvm_SeWaitTaskRemove
static void func_ov094_021fe580(TCB *tcb) {
    BtlvEffvmSeWork *work = GFL_TCBGetData(tcb);

    work->wk->seDelayed = FALSE;
}

// pokebw2's BtlvEffvm_SeMoveTask
static void func_ov094_021fe594(TCB *tcb, void *data) {
    BtlvEffvmSeMoveWork *work = data;
    BOOL done = FALSE;
    int value;

    if (work->delay != 0) {
        work->delay--;
        return;
    }
    if (work->stepWait == 0) {
        work->stepWait = work->stepWaitReset;
        work->value += work->step;
        value = work->value >> FX32_SHIFT;
        if (work->count & 1) {
            if (work->step < 0) {
                if (value < work->start) {
                    value = work->start;
                }
            } else if (value > work->start) {
                value = work->start;
            }
        } else {
            if (work->step < 0) {
                if (value < work->end) {
                    value = work->end;
                }
            } else if (value > work->end) {
                value = work->end;
            }
        }
        work->value = FX32_CONST(value);
        switch (work->param) {
        case 0:
            GFL_SndPlayerSetParams(work->player, -1, value, -1);
            break;
        case 1:
            NNS_SndPlayerSetVolume(func_02006138(work->player), value);
            break;
        case 2:
            GFL_SndPlayerSetParams(work->player, -1, -1, value);
            break;
        }
        if (work->frames == 0) {
            work->frames = work->framesReset;
            switch (work->type) {
            case 0:
                done = TRUE;
                break;
            case 1:
                if (--work->count != 0) {
                    work->step *= -1;
                } else {
                    done = TRUE;
                }
                break;
            }
        } else {
            work->frames--;
        }
    } else {
        work->stepWait--;
    }
    if (done == TRUE) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffvm_SeMoveTaskRemove
static void func_ov094_021fe690(TCB *tcb) {
    BtlvEffvmSeMoveWork *work = GFL_TCBGetData(tcb);

    work->wk->seSliding = FALSE;
}

// pokebw2's BtlvEffvm_CryDelayTask
static void func_ov094_021fe6a4(TCB *tcb, void *data) {
    BtlvEffvmCryWork *work = data;
    int slot;
    BtlvEffvmCryWaitWork *wait;

    if (--work->delay == 0) {
        slot = func_ov094_021fe4d0(work->wk);
        work->wk->voices[slot] = func_ov094_02200218(func_ov094_021f8694(), work->pos, work->arg2, work->arg3, work->arg4,
                                                  work->arg5, work->arg6);
        func_ov094_021fe410(work->wk, 0x7f000, 0x6b000, 10);
        if (!work->wk->bgmLowerRequested) {
            wait = GFL_HeapAllocate(HEAPID_TAIL(work->wk->heapId), sizeof(BtlvEffvmCryWaitWork), FALSE, "btlv_effvm.c",
                                    7789);
            wait->wk = work->wk;
            func_ov094_021f896c(GFL_TCBMgrAddTask(work->wk->tcbManager, func_ov094_021fe8d8, wait, 0), NULL, 0);
        }
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffvm_CryDelayTaskRemove
static void func_ov094_021fe750(TCB *tcb) {
    BtlvEffvmCryWork *work = GFL_TCBGetData(tcb);

    work->wk->cryDelayed = FALSE;
}

// pokebw2's BtlvEffvm_BgmFadeTask
static void func_ov094_021fe768(TCB *tcb, void *data) {
    BtlvEffvmBgmFadeWork *work = data;
    BOOL done = TRUE;

    func_ov094_021f9220(&work->volume, &work->step, &work->target, &done);
    func_0200f698(0xffff, work->volume >> FX32_SHIFT);
    if (done) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffvm_WindowTask
static void func_ov094_021fe79c(TCB *tcb, void *data) {
    BtlvEffvmWindowWork *work = data;

    switch (work->state) {
    case 0:
        G2_SetWnd0InsidePlane((u8)work->winInOut, FALSE);
        G2_SetWnd1InsidePlane(work->winInOut & 0xff, FALSE);
        G2_SetWndOutsidePlane((work->winInOut & 0xff00) >> 8, FALSE);
        GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
        G2_SetWnd1Position(0, 0, 0, 0);
        work->state++;
    case 1:
        if (work->wait == 0) {
            work->wait = work->waitReload;
            switch (work->dir) {
            case 0:
                work->winV -= 0x100;
                work->winV++;
                break;
            case 1:
                work->winV += 0x100;
                work->winV--;
                break;
            case 2:
                work->winH -= 0x100;
                work->winH++;
                break;
            case 3:
                work->winH += 0x100;
                work->winH--;
                break;
            }
            G2_SetWnd0Position(work->winH >> 8, work->winV >> 8, work->winH & 0xff, work->winV & 0xff);
            if ((u8)work->winH == 0xff) {
                G2_SetWnd1Position(1, work->winV >> 8, 256, work->winV & 0xff);
            } else {
                G2_SetWnd1Position(0, 0, 0, 0);
            }
            if (--work->count == 0) {
                func_ov094_021f89bc(tcb);
            }
        } else {
            work->wait--;
        }
        break;
    }
}

// pokebw2's BtlvEffvm_WindowTaskRemove
static void func_ov094_021fe8ac(TCB *tcb) {
    BtlvEffvmWindowWork *work = GFL_TCBGetData(tcb);

    if (!work->keepWindow) {
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
    }
    work->wk->windowTask = FALSE;
}

// pokebw2's BtlvEffvm_CryWaitTask
static void func_ov094_021fe8d8(TCB *tcb, void *data) {
    BtlvEffvmCryWaitWork *work = data;
    int i;

    for (i = 0; i < 6; i++) {
        if (work->wk->voices[i] != -1) {
            if (PokeVoice_IsPlaying(work->wk->voices[i])) {
                return;
            }
            work->wk->voices[i] = -1;
        }
    }
    func_ov094_021fe410(work->wk, 0x6b000, 0x7f000, 10);
    work->wk->bgmLowerRequested = FALSE;
    func_ov094_021f89bc(tcb);
}

// pokebw2's BtlvEffvm_SideResetTask
static void func_ov094_021fe930(TCB *tcb, void *data) {
    BtlvEffvmSideWork *work = data;
    int pos = work->side != 0 ? 1 : 0;
    u32 allMask = work->side != 0 ? 0xaa : 0x55;

    for (; pos <= 7; pos += 2) {
        if (func_ov094_021f7f1c(pos)) {
            if (func_ov094_021ffcb0(func_ov094_021f8694(), pos)) {
                continue;
            }
            if (func_ov094_021f935c(pos) & work->doneMask) {
                continue;
            }
            func_ov094_021ff304(func_ov094_021f8694(), pos, 0);
            func_ov094_021ff470(func_ov094_021f8694(), pos, 0);
            work->doneMask |= func_ov094_021f935c(pos);
        } else {
            work->doneMask |= func_ov094_021f935c(pos);
        }
    }
    if (allMask == work->doneMask) {
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffvm_ParticleUploadTask
static void func_ov094_021fe9bc(TCB *tcb, void *data) {
    BtlvEffvmParticleVBlankWork *work = data;
    u16 vcount = reg_GX_VCOUNT;

    if (vcount >= 192 && vcount <= 200) {
        func_02051394(work->particle);
        work->wk->particleLoading = FALSE;
        func_ov094_021f89bc(tcb);
    }
}

// pokebw2's BtlvEffvm_ScaleAllMcss
static void func_ov094_021fe9f0(fx32 scaleX, fx32 scaleY, int frames, int wait, int count) {
    BtlvMcssPos i;
    fx32 scale;
    VecFx32 dest;

    func_ov094_021ff134(func_ov094_021f8694(), scaleX, scaleY);
    for (i = BTLV_MCSS_POS_FIRST; i < BTLV_MCSS_POS_TRAINER; i++) {
        if (func_ov094_021ffd9c(func_ov094_021f8694(), i)) {
            scale = func_ov094_021ff468(func_ov094_021f8694(), i, 1);
            if (i & 1) {
                scale = FX_Mul(scale, scaleY);
            } else {
                scale = FX_Mul(scale, scaleX);
            }
            dest.x = scale;
            dest.y = scale;
            dest.z = FX32_ONE;
            func_ov094_021ff630(func_ov094_021f8694(), i, 1, &dest, frames, wait, count);
        }
    }
}

static const u32 data_ov094_02209dd8[] = {
    0x235, 0x236, 0x24a, 0x24b, 0x24c, 0x24d, 0x24e, 0x24f, 0x250, 0x251, 0x252
};

// pokebw2's BtlvEffvm_IsGaugeKeptEffect
static BOOL func_ov094_021feaa0(u32 move) {
    int i;

    for (i = 0; i < NELEMS(data_ov094_02209dd8); i++) {
        if (move == data_ov094_02209dd8[i]) {
            return TRUE;
        }
    }
    return FALSE;
}
