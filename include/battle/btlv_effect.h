// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_effect.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_EFFECT_H
#define BWPORT_BATTLE_BTLV_EFFECT_H

// Overlay 94's btlv_effect.c, the battle effect manager. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "battle/btl_setup.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// A fade of 3D textures' palettes toward a color, which BtlvEffect_UpdateTexPaletteFade (btlv_effect.c) steps; the
// stage and the field each have one
typedef struct {
    void **resources; // 0x0  the G3D resources whose palettes fade
    void **palettes;  // 0x4  a copy of each one's palette data, the fade's source
    u8 active;        // 0x8
    u8 count;         // 0x9  of resources
    u8 evy;           // 0xa  the current blend, stepped by one toward targetEvy
    u8 targetEvy;     // 0xb
    u8 wait;          // 0xc  frames left until the next step
    u8 waitFrames;    // 0xd
    u16 color;        // 0xe
} BtlvTexPaletteFade;

void BtlvEffect_UpdateTexPaletteFade(BtlvTexPaletteFade *fade);

// pokebw2's BtlvEffect_GetPaletteFade
PaletteFade *func_ov094_021f86e0(void);
void func_ov094_021f8168(void);
void func_ov094_021f8a78(void);
BOOL func_ov094_021f8914(void);
void func_ov094_021f8a40(u32 arg0);
// pokebw2's BtlvEffect_Exit
void func_ov094_021f7808(void);
// pokebw2's BtlvEffect_AddPokemon
// The Pokémon and the trainers, by position; the trainers' from 8 on
void func_ov094_021f7ee8(PartyPkm *pkm, u32 viewPos);

// A value moving toward a goal, or swinging back and forth, by a step every few frames. The effect tools' work,
// 0x3c bytes, stepped by btlv_effect.c's tools
struct BtlvEffToolMove {
    s32 type;          // 0x00  0: jump to the end, 1 and 4: move to the end, 2 and 3: swing, back to the start
    VecFx32 start;     // 0x04
    VecFx32 end;       // 0x10
    VecFx32 step;      // 0x1c
    s32 stepTime;      // 0x28  frames until the swing turns
    s32 stepTimeReset; // 0x2c
    s32 wait;          // 0x30  frames until the next step
    s32 waitReset;     // 0x34
    s32 count;         // 0x38  the swing's turns left
};

// pokebw2's BtlvEffect_AddTask
// The effect's task slots: a task with its end function and group, its end, and the end of a group's tasks
void func_ov094_021f896c(TCB *tcb, void (*endFunc)(TCB *tcb), u32 group);
// pokebw2's BtlvEffTool_CalcStepVec
void func_ov094_021f918c(const VecFx32 *start, const VecFx32 *end, VecFx32 *step, fx32 frames);
// pokebw2's BtlvEffTool_Move
BOOL func_ov094_021f9260(BtlvEffToolMove *move, VecFx32 *value);
// pokebw2's BtlvEffect_EndTask
void func_ov094_021f89bc(TCB *tcb);

// pokebw2's BtlvEffect_GetTCBManager
TCBManager *func_ov094_021f86d4(void);

// pokebw2's BtlvEffTool_Step
void func_ov094_021f9220(fx32 *value, fx32 *step, const fx32 *end, BOOL *done);

// pokebw2's BtlvEffect_IsTimeUp
BOOL func_ov094_021f8524(int which);
// pokebw2's BtlvEffect_CreateTimer
// The timer
void func_ov094_021f84e8(s32 gameLimitTime, s32 cmdLimitTime);
// pokebw2's BtlvEffect_SetTimerVisible
void func_ov094_021f8504(int which, BOOL visible, BOOL restart);
// pokebw2's BtlvEffect_GetMcss
BtlvMcss *func_ov094_021f8694(void);
// pokebw2's BtlvEffect_StartPaletteFade
// target: 0 the stage, 1 the field, 2 both, 3 the palettes, 4 all
void func_ov094_021f8298(u32 target, u8 evy, u8 targetEvy, u8 wait, u16 color);
// pokebw2's BtlvEffect_IsPaletteFading
BOOL func_ov094_021f8324(u32 target);
// pokebw2's BtlvEffect_SetVanish
// target: 0 the stage, 1 the field, 2 BG 3, 3 and 4 the stage's sides
void func_ov094_021f839c(u32 target, BOOL hide);
// pokebw2's BtlvEffect_PlayBgm
void func_ov094_021f878c(u32 bgm);
// pokebw2's BtlvEffect_StartEffect26E
void func_ov094_021f7e94(u8 pos);
// pokebw2's BtlvEffect_StartEffect26F
void func_ov094_021f7ea4(u8 pos);
// pokebw2's BtlvEffect_CalcGaugeExpLevelUp
void func_ov094_021f8150(u32 viewPos, BattleMon *mon);
// pokebw2's BtlvEffect_PlayBgmNoPinch
void func_ov094_021f883c(u32 bgm);

// pokebw2's BtlvEffect_PosBit
u32 func_ov094_021f935c(u32 bit);
// pokebw2's BtlvEffect_GetGaugeStatus
BOOL func_ov094_021f821c(u32 viewPos, u32 *color, u32 *status);
// pokebw2's BtlvEffect_GetGauge
BtlvGauge *func_ov094_021f86b4(void);
// pokebw2's BtlvEffect_GetMainModule
BtlMainModule *func_ov094_021f8754(void);
// pokebw2's BtlvEffect_GetBattleStyle
u32 func_ov094_021f8710(void); // the battle style
// pokebw2's BtlvEffect_GetEffvm
VM *func_ov094_021f86c4(void);
// pokebw2's BtlvEffect_GetStage
BtlvStage *func_ov094_021f86a4(void);

// pokebw2's BtlvEffect_GetUnk2E4
BOOL func_ov094_021f88f4(void);
// pokebw2's BtlvEffect_ZoomCamera
void func_ov094_021f8554(u32 pos, u32 mode, int frames, int wait, int brakeFrames);

// pokebw2's BtlvEffect_GetBattleStyle
u32 func_ov094_021f8710(void); // the battle style
// pokebw2's BtlvEffect_GetBattleType
u32 func_ov094_021f8720(void);  // the battle type

// pokebw2's BtlvEffect_CreateSetup
BtlvEffectSetup *func_ov094_021f745c(BtlMainModule *mainModule, BtlvScu *scu, HeapID heapId);
// pokebw2's BtlvEffect_StartEffect285
void func_ov094_021f7e74(u8 pos);
// pokebw2's BtlvEffect_StartEffect286
void func_ov094_021f7e84(u8 pos);
// pokebw2's BtlvEffect_GetClact
BtlvClact *func_ov094_021f86f0(void);

struct BtlvMoveEffectParam {
    u16 move;          // 0x00
    u32 attackerPos;   // 0x04
    u32 targetPos;     // 0x08
    u8 unk0C;          // 0x0C arg5 of func_ov167_021d3094
    u8 unk0D;          // 0x0D its arg6
    s32 unk10;         // 0x10 its arg4, PML_MoveGetParam(move, 27); BtlvEffect_StartMove keeps a byte of it
}; // size 0x14

// pokebw2's BtlvEffect_DelBallGauge
void func_ov094_021f8260(u32 side);
// pokebw2's BtlvEffect_DelGauge
void func_ov094_021f80e4(u32 viewPos);
// pokebw2's BtlvEffect_StartEffect23B
void func_ov094_021f7ce4(u8 viewPos);
// pokebw2's BtlvEffect_DelPokemon
void func_ov094_021f7f04(u32 viewPos);
// pokebw2's BtlvEffect_ChangeSprite
void func_ov094_021f7e38(PartyPkm *pkm, u32 viewPos);
// pokebw2's BtlvEffect_AddGauge
// The gauges and the ball gauges
void func_ov094_021f803c(BtlMainModule *mainModule, BattleMon *mon, u32 viewPos);

// pokebw2's BtlvEffect_SetGaugeFlag
// pokebw2's BtlvEffect_GetCamera
BtlvCamera *func_ov094_021f8684(void);
// pokebw2's BtlvEffect_GetBg
BtlvBg *func_ov094_021f8700(void);
// pokebw2's BtlvEffect_IsMulti
BOOL func_ov094_021f8730(void);
// pokebw2's BtlvEffect_GetTrainerType
u16 func_ov094_021f8740(u32 pos);
// pokebw2's BtlvEffect_GetScu
BtlvScu *func_ov094_021f8764(void);
// pokebw2's BtlvEffect_SetPokemonCheck
void func_ov094_021f8934(u32 arg0);
// pokebw2's BtlvEffect_EndTaskGroup
void func_ov094_021f8a10(u32 group);
// pokebw2's BtlvEffect_ReleaseVoices
void func_ov094_021f8b10(void);
// pokebw2's BtlvEffTool_CalcStep
void func_ov094_021f9154(fx32 start, fx32 end, fx32 *step, fx32 frames);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The setup that the init copies, and the init and end of the view
BtlvEffectSetup *BtlvEffect_Create(u32 battleStyle, u32 battleType, const BtlFieldEnv *fieldEnv, BOOL multi,
                                   const u16 *trainerTypes, BtlMainModule *mainModule, BtlvScu *scu, HeapID heapId);
// pokebw2's BtlvEffect_Init
void func_ov094_021f7544(BtlvEffectSetup *setup, Font *font, HeapID heapId);
// pokebw2's BtlvEffect_Main
void func_ov094_021f78e4(void);
// pokebw2's BtlvEffect_Start
// Effects started by number, at positions or for a move; one started while another runs waits in a task
void func_ov094_021f7998(u32 effNo);
void func_ov094_021f7f58(s32 type, u32 pos, fx32 x, fx32 y, fx32 z);
void func_ov094_021f87ec(u32 arg0);
BOOL func_ov094_021f8204(u32 viewPos);
void func_ov094_021f8134(u32 viewPos, u32 exp);
BOOL func_ov094_021f8180(void);
BOOL func_ov094_021f8774(void);
// pokebw2's BtlvEffect_IsBusy
BOOL func_ov094_021f7eb4(void);
// pokebw2's BtlvEffect_AddGaugeByPkm
void func_ov094_021f8090(PokeDexSave *pokedex, PartyPkm *pkm, u32 viewPos);
// pokebw2's BtlvEffect_StartEffect23A
void func_ov094_021f7cfc(u8 pos, u16 itemNo, u8 arg2, u32 arg3, u32 arg4);
// pokebw2's BtlvEffect_Resume
void func_ov094_021f7c0c(void);
// pokebw2's BtlvEffect_SetFlag26
void func_ov094_021f88bc(u32 arg0);
// pokebw2's BtlvEffect_SetGaugeStatus
void func_ov094_021f81d0(u32 status, u8 pos);
// pokebw2's BtlvEffect_StartEffect23D
void func_ov094_021f7d44(u8 pos, u16 itemNo);
// pokebw2's BtlvEffect_StopIdleEffect
// pokebw2's BtlvEffect_CheckExist
BOOL func_ov094_021f7f1c(int pos);
// pokebw2's BtlvEffect_CheckExistPokemon
BOOL func_ov094_021f853c(u8 pos);
// pokebw2's BtlvEffect_StartRotation
// Turns a side of a rotation battle; mode 1 does nothing
void func_ov094_021f8414(u8 mode, u32 side, u32 arg2);
// pokebw2's BtlvEffect_AddBallGauge
void func_ov094_021f823c(const BtlvBGaugeParam *param);
// pokebw2's BtlvEffect_IsBallGaugeBusy
BOOL func_ov094_021f827c(u32 side);
// pokebw2's BtlvEffect_CalcGaugeHPAtOnce
void func_ov094_021f8118(u32 viewPos, s32 hp);
// pokebw2's BtlvEffect_CheckExecuteGauge
// pokebw2's BtlvEffect_SetNoPinchBgm
void func_ov094_021f87d4(BOOL noPinchBgm);
// pokebw2's BtlvEffect_ClearVoices
void func_ov094_021f8b28(void);
// pokebw2's BtlvEffect_SetAbility
// The abilities shown, by view position
void func_ov094_021f8ae8(u32 viewPos, u32 ability);
// pokebw2's BtlvEffect_StartMove
void func_ov094_021f7b2c(const BtlvMoveEffectParam *param);
// pokebw2's BtlvEffect_StartChangeSprite
void func_ov094_021f7d80(PartyPkm *pkm, u32 viewPos);
// pokebw2's BtlvEffect_SetGaugeDrawEnableBySide
void func_ov094_021f8198(BOOL visible, u32 side);
// pokebw2's BtlvEffect_SetGaugeDrawEnable
void func_ov094_021f81b4(BOOL visible, u32 viewPos);
// pokebw2's BtlvEffect_GaugeAction
void func_ov094_021f81ec(u32 viewPos);
// pokebw2's BtlvEffect_SwapPokemon
void func_ov094_021f84b4(u8 pos1, u8 pos2);
// pokebw2's BtlvEffect_GetTrainerIndex
s32 func_ov094_021f84d0(u32 pos);
// pokebw2's BtlvEffect_IsPinchBgm
// The BGM, with the gauges' pinch music
// pokebw2's BtlvEffect_GetState
u32 func_ov094_021f88a8(void);
// pokebw2's BtlvEffect_GetFlag26
u32 func_ov094_021f88e0(void);
// pokebw2's BtlvEffect_GetUnk304
// pokebw2's BtlvEffect_SetIdleEffectMode
// The effects played while the battle waits for input
void func_ov094_021f8a40(u32 mode);
// pokebw2's BtlvEffect_GetAbility
u32 func_ov094_021f8afc(u8 viewPos);
// pokebw2's BtlvEffect_DelTrainer
void func_ov094_021f8010(u32 pos);

// pokebw2's BtlvEffect_RestartIdleEffect
void func_ov094_021f8ac0(u32 mode);
// pokebw2's BtlvEffect_GetWork
// The work and its parts
BtlvEffect *func_ov094_021f8678(void);
// pokebw2's BtlvEffect_SetState
void func_ov094_021f8884(u32 state);

// Black's, no twin in pokebw2's C
void func_ov094_021f7a28(u32 pos, int effect);
void func_ov094_021f7aac(u32 pos1, u32 pos2, int effect);

void func_ov094_021f84b4(u8 arg0, u8 arg1);

void func_ov094_021f80fc(u32 arg0, u32 arg1);
void func_ov094_021f7c2c(u32 arg0, u16 arg1);
BOOL func_ov094_021f882c(void);

#endif // BWPORT_BATTLE_BTLV_EFFECT_H
