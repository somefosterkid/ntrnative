// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_scd.h: the lines in use so far.
#ifndef BWPORT_BATTLE_BTLV_SCD_H
#define BWPORT_BATTLE_BTLV_SCD_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "battle/btlv.h"

// Black 2's, not checked against Black
#define NUM_PROCS 4

// Black 2's, not checked against Black
// The positions a move can aim at, which the target screen shows. When a choice is to be made the count is in
// selectCount, and the positions are what the choice is among; otherwise count is the number the move hits
typedef struct BtlvScdTargetList {
    u8 selectCount;
    u8 count;
    u8 kind; // TARGETS_GROUP or TARGETS_SIDE, when selectCount is 0
    u8 pos[6];
} BtlvScdTargetList;

// unk18, procs, numProcs, unk64, moveSelectMain, mon, action, unk78, targetParam, targets, targetChosen, moveRange,
// slot, typeCount, typeMask carved out of the unk_ arrays, from pokebw2's layout, not checked against Black
struct BtlvScd {
    u8 unk_00[0x8];
    Font *font;                 // 0x08
    StrBuf *strBuf;             // 0x0c
    BtlClient *client;          // 0x10
    PrintQueue *printQueue;     // 0x14
    u8 unk18[8];                  // 0x18
    BtlvSubProc procs[NUM_PROCS]; // 0x20 a stack of the procedures that are running; the last one runs
    u32 numProcs;                 // 0x60
    u8 unk64[4];
    BtlvSubProcFn moveSelectMain; // 0x68 what the move screens run, to go back to them
    BattleMon *mon;               // 0x6C
    BattleAction *action;         // 0x70 the choice of the player
    u32 unk_74;
    u8 unk78[4];
    BtlvSelectTargetParam *targetParam; // 0x7C the mons of a rotation battle
    BtlvScdTargetList targets;          // 0x80
    u8 targetChosen;                    // 0x89
    u8 moveRange;                       // 0x8A the move's target type
    u8 slot;                            // 0x8B the number of the mon choosing this turn
    s8 unk_8c;
    s8 unk_8d;
    s8 unk_8e;
    u8 unk_8f;
    u8 unk_90;
    u8 unk_91;
    u8 unk_92_low : 7;          // bits 0-6 of 0x92
    u8 unk_92_high : 1;         // bit 7 of 0x92
    u8 unk_93;
    u32 typeCount;                      // 0x94
    u32 typeMask;                       // 0x98
    BtlvCore *core;             // 0x9c
    BtlMainModule *mainModule;  // 0xa0
    BtlPokeCon *pokeCon;        // 0xa4
    BattleParty *party;         // 0xa8
    HeapID heapId;              // 0xac
    BtlvInput *input;           // 0xb0
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvScd_Delete
void data_ov095_06899dd9(BtlvScd *scd);
// pokebw2's BtlvScd_ShowRecorderScreen
// The recorder screen of a BtlvInputRecorderParam and its buttons
void data_ov095_0689b781(BtlvScd *scd, void *param);
// pokebw2's BtlvScd_GetKeyCursorFlag
// Whether the player uses the keys, which the input sets; and ending the input at once (a forced quit)
u8 *data_ov095_0689b66d(BtlvScd *scd);
// pokebw2's BtlvScd_GetCommandResult
u32 data_ov095_06899f29(BtlvScd *scd);
// pokebw2's BtlvScd_StartTargetSelect
// The target screen of the move in action
void data_ov095_0689a009(BtlvScd *scd, BattleMon *mon, BattleAction *action);
// pokebw2's BtlvScd_IsTargetSelectDone
BOOL data_ov095_0689a021(BtlvScd *scd);
// pokebw2's BtlvScd_StartMoveSelect
// The move screen, and the same with the finger demo of the tutorial. They are done when BtlvScd_IsDone says so
void data_ov095_06899f41(BtlvScd *scd, BattleMon *mon, BattleAction *action);
// pokebw2's BtlvScd_StartRotationSelect
// The move screen of a rotation battle, with the moves of its three mons
void data_ov095_06899f65(BtlvScd *scd, BtlvSelectTargetParam *param, BattleAction *action);
// pokebw2's BtlvScd_ResumeMoveSelect
// Goes back to the move screen that was started last, after the target screen was cancelled
void data_ov095_06899fdd(BtlvScd *scd);
// pokebw2's BtlvScd_IsDone
BOOL data_ov095_06899fd5(BtlvScd *scd);
// pokebw2's BtlvScd_StartFadeOut
// Fades the lower screen out and in; mode is passed to BtlvInput_StartPaletteFade
void data_ov095_06899ca5(BtlvScd *scd, u32 mode);
// pokebw2's BtlvScd_IsFadeDone
BOOL data_ov095_06899d05(BtlvScd *scd);
// pokebw2's BtlvScd_OnFadeOutDone
// Empty, called when a fade out is done and right after a fade in starts
void data_ov095_06899d2d(BtlvScd *scd);
// pokebw2's BtlvScd_StartFadeIn
void data_ov095_06899cdd(BtlvScd *scd);
// pokebw2's BtlvScd_OnFadeInStart
void data_ov095_06899d31(BtlvScd *scd);
// pokebw2's BtlvScd_ShowRotationScreen
// The rotation screen, and its input: the button pressed, if any
void data_ov095_0689b515(BtlvScd *scd, void *param);
// pokebw2's BtlvScd_PollRotationInput
BOOL data_ov095_0689b525(BtlvScd *scd, u8 *button);
// pokebw2's BtlvScd_HideRotationScreen
void data_ov095_0689b559(BtlvScd *scd);
// pokebw2's BtlvScd_ShowYesNo
// The yes/no screen of a BtlvInputYesNoParam, and its answer once it is given
void data_ov095_0689b5a5(BtlvScd *scd, void *param);
// pokebw2's BtlvScd_PollYesNo
BOOL data_ov095_0689b5cd(BtlvScd *scd, u32 *answer);
// pokebw2's BtlvScd_ForceQuitInput
void data_ov095_0689b671(BtlvScd *scd);
// pokebw2's BtlvScd_IsForceQuitInputDone
BOOL data_ov095_0689b69d(BtlvScd *scd);
// pokebw2's BtlvScd_CheckRecorderInput
u32 data_ov095_0689b791(BtlvScd *scd);

// Black's, no twin in pokebw2's C
BtlvScd *data_ov095_06899af1(BtlvCore *core, BtlMainModule *mainModule, BtlPokeCon *pokeCon, TCBExManager *tcbManager,
                             Font *font, BtlClient *client, u32 scdState, HeapID heapId);
void data_ov095_06899ed1(BtlvScd *scd, BattleMon *mon, u8 slot, u8 shooterEnergy, BattleAction *action);
void data_ov095_06899be1(BtlvScd *scd);
void data_ov095_06899fe9(BtlvScd *scd, BattleMon *mon, BattleAction *action);
BOOL data_ov095_06899db1(BtlvScd *scd);
void data_ov095_06899f01(BtlvScd *scd, BattleMon *mon, u8 slot, BattleAction *action);
void data_ov095_06899d65(BtlvScd *scd);
void data_ov095_06899d3d(BtlvScd *scd);
BOOL data_ov095_0689b0cd(BtlvScd *scd);
void data_ov095_0689a029(BtlvScd *scd);
u8 data_ov095_0689cb2d(BtlvPokeSelectParam *select, u8 index);

#endif // BWPORT_BATTLE_BTLV_SCD_H
