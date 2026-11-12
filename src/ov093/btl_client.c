// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_client.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_action.h"
#include "battle/btl_adapter.h"
#include "battle/btl_calc.h"
#include "battle/btl_client.h"
#include "battle/btl_field.h"
#include "battle/btl_main.h"
#include "battle/btl_poke_select.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_rec.h"
#include "battle/btl_server.h"
#include "battle/btl_server_cmd.h"
#include "battle/btl_setup.h"
#include "battle/btl_tables.h"
#include "battle/btlv.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_mcss.h"
#include "battle/tr_ai.h"
#include "battle/trainer_data.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/types.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/math.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "save/player_info.h"
#include "system/pms_data.h"

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0) where it has them

// The playback of a recorded battle
typedef struct {
    u8 seq;
    // 1 or 2 when the playback stopped
    u8 result;
    u8 skipping : 1;
    u8 quitStarted : 1;
    u8 quitDone : 1;
    u8 chapterReached : 1;
    u8 unk2_4 : 1;
    u8 dataEnd : 1;
    u16 quitTimer;
    u16 chapter;
    u16 skipTarget;
    u16 maxChapter;
    u16 skipCount;
} BtlClientRecPlayer;

struct BtlClient {
    BtlMainModule *mainModule;
    BtlPokeCon *pokeCon;
    BattleMon *procMon;
    BattleAction *procAction;
    void *recorder;
    void *recReader;
    BtlClientRecPlayer recPlayer;
    BOOL (*mainProc)(BtlClient *client);
    BtlClientIDList clientIdList;
    BtlField *field;
    MATHRandContext32 targetRand;
    BtlAdapter *adapter;
    BtlvCore *viewCore;
    BtlvStringParam strParam;
    BtlvStringParam strParam2;
    BtlServer *cmdCheckServer;
    BtlvSelectTargetParam rotationParam;
    u8 unkCC;
    BOOL (*cmdProc)(BtlClient *client, s32 *seq);
    s32 cmdSeq;
    BOOL (*selActProc)(BtlClient *client, s32 *seq);
    s32 selActSeq;
    const void *returnData;
    u32 returnDataSize;
    u32 dummyReturnData;
    u16 cmdLimitTime;
    u16 gameLimitTime;
    u16 aiItems[4];
    VM *aiVM;
    MATHRandContext32 aiRand;
    s8 aiSwitchReserved[6];
    u8 hintShown[4];
    BattleParty *party;
    u8 numCoverPos;
    u8 procActionIdx;
    s8 prevActionIdx;
    u8 firstActionIdx;
    u8 forceActionMsg;
    u8 unk129;
    BattleAction actions[3];
    u8 shooterCost[3];
    BtlServerCmdQueue *cmdQueue;
    u32 cmdArgs[16];
    u32 serverCmd;
    BOOL (*serverCmdProc)(BtlClient *client, s32 *seq, const u32 *args);
    s32 serverCmdSeq;
    BtlvPokeListCmd pokeListCmd;
    BtlvPokeSelectParam pokeSelect;
    u16 heapId;
    u16 savedHp;
    u16 hintMsgId;
    u16 reservedItems[3];
    u8 clientId;
    // 0 for the player, 1 for the AI, 2 for a recorded battle's playback
    u8 clientType;
    u8 mainSeq;
    u8 waitMsgShown;
    u8 bagMode;
    u8 shooterEnergy;
    u8 yesNoResult;
    u8 cmdLimitOver;
    u8 cmdCheckReq;
    u8 extraActionCount;
    u8 moveInfoPos;
    u8 moveInfoIdx;
    u8 unk1BA_0 : 1;
    u8 forceQuit : 1;
    u8 escapeSelected : 1;
    u8 cmdCheckEnable : 1;
    u8 unk1BA_4 : 1;
    u8 unk1BA_5 : 1;
    u8 changePokeCount;
    u8 unk1B4;
    u8 changePokePos[3];
};

// A step of what a client does for a command, run until it returns TRUE
typedef BOOL (*BtlClientCmdProc)(BtlClient *client, s32 *seq);

// The move the experience command's level-up is learning
static u16 sLearnMove;
// The sequence of the move learning, and its timer
static s32 sExpSeq;
// The frames of the ability swap's animation
// pokebw2's sSwapTimer
static u32 data_ov093_021f3b64;
// Where the search for the moves learned at the new level goes on
static u32 sLearnIdx;
// The experience left to add
static u32 sExpLeft;
static BattleMonLevelUp sLevelUp;

BtlClientCmdProc func_ov093_021cdd50(BtlClient *client, u32 cmd, u32 *unk01);
BOOL func_ov093_021cda18(BtlClient *client);
BOOL func_ov093_021cdc0c(BtlClient *client);
void func_ov093_021cdd34(BtlClient *client);
BOOL func_ov093_021cdddc(BtlClient *client, s32 *seq);
BOOL func_ov093_021cde8c(BtlClient *client, s32 *seq);
void func_ov093_021cdecc(BtlClient *client);
u32 func_ov093_021cdee8(BtlClient *client);
BattleMon *func_ov093_021cdf1c(BtlClient *client);
u8 func_ov093_021cdfe4(u8 dir);
BOOL func_ov093_021ce040(BtlClient *client, s32 *seq);
BOOL func_ov093_021ce044(BtlClient *client, s32 *seq);
void func_ov093_021ce068(BtlClient *client);
BOOL CheckIfOverCmdLimit(BtlClient *client);
BOOL CheckActionSelectForceQuit(BtlClient *client, BOOL (*proc)(BtlClient *client, s32 *seq));
void func_ov093_021ce0e0(BtlClient *client);
void func_ov093_021ce0f8(BtlClient *client, BattleMon *mon, BtlvStringParam *param);
void func_ov093_021ce150(BtlClient *client, BOOL (*proc)(BtlClient *client, s32 *seq));
BOOL func_ov093_021ce160(BtlClient *client);
BOOL func_ov093_021ce1b0(BtlClient *client, s32 *seq);
BOOL func_ov093_021cdf58(BtlClient *client, s32 *seq);
BOOL func_ov093_021ce170(BtlClient *client, s32 *seq);
void func_ov093_021ce2e8(BtlClient *client);
BOOL func_ov093_021ce308(BtlClient *client, s32 *seq);
void func_ov093_021ce3b4(BtlClient *client, const BtlvStringParam *param);
BOOL func_ov093_021ce4d0(BtlClient *client, s32 *seq);
BOOL func_ov093_021ce3c8(BtlClient *client, s32 *seq);
BOOL func_ov093_021ce7d4(BtlClient *client, s32 *seq);
BOOL func_ov093_021ce8f0(BtlClient *client, u16 *msgId);
BOOL func_ov093_021cea94(BtlClient *client, s32 *seq);
BOOL func_ov093_021cedc0(BtlClient *client, s32 *seq);
BOOL func_ov093_021ceeb8(BtlClient *client, s32 *seq);
void func_ov093_021cecb4(BtlClient *client, BtlvSelectTargetParam *param);
BOOL func_ov093_021ced4c(BtlClient *client, s32 *seq);
BOOL IsPokeBallTargetHiding(BtlClient *client);
BOOL BattleClient_ActionSelectEscape(BtlClient *client, s32 *seq);
BOOL func_ov093_021cf220(BtlClient *client, s32 *seq);
BOOL BattleClient_ActionSelectFinish(BtlClient *client, s32 *seq);
void selItemWork_Init(BtlClient *client);
void selItemWork_Reserve(BtlClient *client, u8 index, u16 item);
void selItemWork_Restore(BtlClient *client, u8 index);
void selItemWork_Quit(BtlClient *client);
void func_ov093_021cf41c(BtlClient *client);
void func_ov093_021cf430(BtlClient *client, u8 index, u8 cost);
u8 func_ov093_021cf43c(BtlClient *client, u8 index);
u8 func_ov093_021cf448(BtlClient *client);
BOOL AutoSelectAction(BtlClient *client, BattleMon *mon, BattleAction *action);
BOOL CheckForSelectableMove(BtlClient *client, BattleMon *mon, BattleAction *action);
void SetStruggleAction(BattleAction *action, BtlClient *client, BattleMon *mon);
BOOL IsUnselectableMove(BtlClient *client, BattleMon *mon, u16 move, BtlvStringParam *param);
u8 StoreSelectableMoveFlag(BtlClient *client, BattleMon *mon, u8 *selectable);
BOOL CanMonUseHeldItem(BtlClient *client, BattleMon *mon);
u32 CanMonSwitch(BtlClient *client, BattleMon *mon, u8 *trapMonId, u16 *trapAbility);
u32 func_ov093_021cf918(BtlClient *client, u8 *trapMonId, u16 *trapAbility);
u32 IsMonTrapped(BtlClient *client, BattleMon *mon, u8 *trapMonId, u16 *trapAbility);
BOOL DoesMonHaveShadowTag(BtlClient *client, BattleMon *mon);
BOOL IsMonTrappedByArenaTrap(BtlClient *client, BattleMon *mon);
BOOL IsMonSteelType(BtlClient *client, BattleMon *mon);
void func_ov093_021cfba4(BtlClient *client);
BOOL func_ov093_021cfbbc(BtlClient *client, u8 slot);
void func_ov093_021cfbe0(BtlClient *client, u8 index, u8 slot);
BOOL AISwitchChecks(BtlClient *client, BattleMon *mon, u8 index, u8 *slot);
BattleMon *func_ov093_021cfd50(BtlClient *client, u8 pos);
BOOL ShouldSwitchIfPerishSongLastTurn(BtlClient *client, BattleMon *mon);
BOOL ShouldSwitchIfWonderGuard(BtlClient *client, BattleMon *mon, BattleMon *target);
BOOL ShouldSwitchIfNoEffectiveMoves(BtlClient *client, BattleMon *mon, BattleMon *target);
BOOL ShouldSwitchIfChoicedIntoIneffectiveMove(BtlClient *client, BattleMon *mon, BattleMon *target);
BOOL ShouldSwitchIfTypeAbsorbingAbility(BtlClient *client, BattleMon *mon, BattleMon *target, u8 *slot);
BOOL ShouldSwitchIfAsleepWithNaturalCure(BtlClient *client, BattleMon *mon, u8 *slot);
BOOL FinalSwitchChecks(BtlClient *client, BattleMon *mon, BattleMon *target, u8 *slot);
BOOL CheckMonsForTypeAbsorbingAbility(BtlClient *client, u16 ability, u8 *slot);
BOOL CheckIfMonToSwitchToWithSEMove(BtlClient *client, BattleMon *target, s32 minEffectiveness);
BOOL func_ov093_021d05c4(BtlClient *client, u8 moveType, s32 maxEffect, u8 *partyIdx);
BOOL DoesMonHaveSuperEffectiveMove(BtlClient *client, BattleMon *attacker, BattleMon *defender, s32 minEffect);
u8 func_ov093_021d06c8(BtlClient *client);
BOOL AISelectAction(BtlClient *client, s32 *seq);
u8 GetNumBattleReadyPartyMons(BtlClient *client, u8 *list);
void PickBestMonToSwitchInto(BtlClient *client, u8 *list, u8 count, BattleMon *defender);
void func_ov093_021d0b10(BtlClient *client, u8 mode, u8 count, BtlvPokeListCmd *cmd,
                                BtlvPokeSelectParam *select);
void func_ov093_021d0b6c(BtlClient *client, const BtlvPokeSelectParam *select);
void func_ov093_021d0bb0(BtlClient *client);
u8 func_ov093_021d0c78(BtlClient *client, u8 *positions);
BOOL func_ov093_021d0cc4(BtlClient *client, s32 *seq);
BOOL func_ov093_021d0d38(BtlClient *client, s32 *seq);
BOOL func_ov093_021d0d44(BtlClient *client, s32 *seq);
BOOL func_ov093_021d0d50(BtlClient *client, s32 *seq);
BOOL func_ov093_021d0e74(BtlClient *client, s32 *seq);
BOOL func_ov093_021d0e7c(BtlClient *client, s32 *seq, u8 mode);
BOOL func_ov093_021d102c(BtlClient *client);
u32 func_ov093_021d1058(BtlClient *client, BattleAction *action);
BOOL func_ov093_021d10b8(BtlClient *client, s32 *seq);
BOOL func_ov093_021d123c(BtlClient *client, s32 *seq);
BOOL func_ov093_021d127c(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1290(BtlClient *client, s32 *seq);
u32 func_ov093_021d13a4(BtlClient *client);
BOOL func_ov093_021d13fc(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1410(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1708(BtlClient *client, s32 *seq);
BOOL func_ov093_021d171c(BtlClient *client, s32 *seq);
void func_ov093_021d185c(BtlClient *client, u32 result, u8 which);
BOOL func_ov093_021d18ec(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1960(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1a18(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1a60(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1a74(BtlClient *client, s32 *seq);
BOOL func_ov093_021d1bd0(BtlClient *client, s32 *seq, const u32 *args);
u16 func_ov093_021d1c40(BtlClient *client, u8 clientId, BOOL *withName);
BOOL func_ov093_021d1cac(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d1cf4(BtlClient *client, s32 *seq, const u32 *args);
u16 func_ov093_021d1df4(BtlClient *client);
BOOL func_ov093_021d1e6c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d1eb4(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d1f10(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d1f58(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d1fb4(BtlClient *client, s32 *seq, const u32 *args);
s32 func_ov093_021d2000(BtlClient *client, u16 move, u8 pos);
BOOL func_ov093_021d2030(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d20bc(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d210c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d215c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d21d4(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2234(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2278(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2320(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2374(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d23b8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2424(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2490(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2510(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2580(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d25c4(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2628(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d266c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2700(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2814(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d284c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d28ac(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2930(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2990(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d299c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d29cc(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2a20(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d2c7c(u32 arg0);
BOOL func_ov093_021d2ca8(BtlClient *client, s32 *seq, BattleMon *mon);
BOOL func_ov093_021d2fd8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3138(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d31a0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d324c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d32e0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d344c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d34b0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d34e8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3568(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d35c8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3630(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3688(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d36cc(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d36ec(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d370c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3734(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d375c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d377c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3794(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d37b8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d37dc(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d37fc(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d381c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3894(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d38ac(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d38c4(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d38e0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3900(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3918(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3934(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3988(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d39a8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3a20(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3a40(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3a7c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3a98(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3ab4(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3ad0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3aec(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3b08(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3b28(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3b5c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3b74(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3b88(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3b9c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3bb0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3bc0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3be0(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3c0c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3c2c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3c44(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3c70(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3c90(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3ca8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3cc8(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3cfc(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3d1c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3d2c(BtlClient *client, s32 *seq, const u32 *args);
BOOL func_ov093_021d3d50(BtlClient *client, s32 *seq, const u32 *args);
void func_ov093_021d3d9c(BtlClient *client);
u16 func_ov093_021d3dc4(BtlClient *client, BattleMon *mon, BattleParty *party);
BOOL IsXItem(u16 item, BattleMon *mon);
BOOL IsStatusCureItem(u16 item, BattleMon *mon);
void func_ov093_021d3f04(BtlClientRecPlayer *recPlayer);
void func_ov093_021d3f48(BtlClientRecPlayer *recPlayer, u32 maxChapter);
BOOL func_ov093_021d3f4c(BtlClientRecPlayer *recPlayer);
void func_ov093_021d3f54(BtlClientRecPlayer *recPlayer);
u32 func_ov093_021d3f60(BtlClientRecPlayer *recPlayer);
void func_ov093_021d3f64(BtlClientRecPlayer *recPlayer, u32 target);
void func_ov093_021d3f88(BtlClientRecPlayer *recPlayer);
BOOL func_ov093_021d3f98(BtlClientRecPlayer *recPlayer);
u32 func_ov093_021d3fa8(BtlClientRecPlayer *recPlayer);
BOOL func_ov093_021d3fac(BtlClientRecPlayer *recPlayer);
void func_ov093_021d3fc4(BtlClientRecPlayer *recPlayer);
void func_ov093_021d3fd0(BtlClientRecPlayer *recPlayer);
void func_ov093_021d3fdc(BtlClient *client, BtlClientRecPlayer *recPlayer);

// A server command a client handles, with its handler for each client type
typedef struct {
    u8 cmd;
    u8 unk01;
    BOOL (*procs[3])(BtlClient *client, s32 *seq);
} BtlClientCmdEntry;

// 0x021d7070: the commands that the clients handle (also used by the other ranges' handlers)
static const BtlClientCmdEntry sCmdProcTable[15] = {
    { 1, 1, { func_ov093_021cdddc, NULL, func_ov093_021cde8c } },
    { 2, 0, { func_ov093_021cdf58, func_ov093_021ce044, func_ov093_021ce040 } },
    { 3, 0, { func_ov093_021ce170, AISelectAction, func_ov093_021ce1b0 } },
    { 7, 0, { func_ov093_021d0cc4, NULL, NULL } },
    { 5, 0, { func_ov093_021d0d38, func_ov093_021d0d50, func_ov093_021d0e74 } },
    { 6, 0, { func_ov093_021d0d44, func_ov093_021d0d50, func_ov093_021d0e74 } },
    { 4, 0, { func_ov093_021d10b8, NULL, func_ov093_021d0e74 } },
    { 8, 0, { func_ov093_021d1a74, NULL, func_ov093_021d1a60 } },
    { 10, 0, { func_ov093_021d123c, NULL, NULL } },
    { 11, 1, { func_ov093_021d1410, NULL, func_ov093_021d13fc } },
    { 12, 1, { func_ov093_021d171c, NULL, func_ov093_021d1708 } },
    { 14, 1, { func_ov093_021d18ec, NULL, NULL } },
    { 15, 1, { func_ov093_021d1960, NULL, NULL } },
    { 13, 1, { func_ov093_021d1290, NULL, func_ov093_021d127c } },
    { 16, 1, { func_ov093_021d1a18, NULL, NULL } },
};

// The trainer messages the AI trainer may say as hints, in the order they are checked
static const u16 sTrainerHintMsgs[4] = { 0x12, 0x11, 0x13, 0x14 };

// A type and the abilities that absorb moves of it, for the AI's switch checks
typedef struct {
    u8 type;
    u16 abilities[4];
} TypeAbsorbingAbilities;

static const TypeAbsorbingAbilities sTypeAbsorbingAbilities[4] = {
    { TYPE_WATER, { ABILITY_WATER_ABSORB, ABILITY_STORM_DRAIN, ABILITY_DRY_SKIN, 0 } },
    { TYPE_ELECTRIC, { ABILITY_VOLT_ABSORB, ABILITY_MOTOR_DRIVE, ABILITY_LIGHTNINGROD, 0 } },
// BUG: Overgrow doesn't absorb Grass moves; Sap Sipper does
#ifdef BUGFIX
    { TYPE_GRASS, { ABILITY_SAP_SIPPER, 0, 0, 0 } },
#else
    { TYPE_GRASS, { ABILITY_OVERGROW, 0, 0, 0 } },
#endif
    { TYPE_FIRE, { ABILITY_FLASH_FIRE, 0, 0, 0 } },
};

// The item that restores a stat stage, and the stage, of each X item
typedef struct {
    u8 param;
    u8 stat;
} BtlClientItemStat;

// The item that cures a condition, and the condition
typedef struct {
    u8 param;
    u8 condition;
} BtlClientItemCondition;

// The most mons the AI may have left to use the item in each of its four slots
static const u8 sAIItemMaxMons[4] = { 6, 4, 2, 1 };
// pokebw2's BattleClient_Delete

BtlClient *func_ov093_021cd6f0(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 commMode, void *netHandle, u16 clientId,
                               u16 numCoverPos, u8 isAI, u32 bagMode, BOOL recPlay, MATHRandContext32 *rand, HeapID heapId) {
    BtlClient *client;
    BOOL flag;
    u32 i;

    client = GFL_HeapAllocate(heapId, sizeof(BtlClient), TRUE, "btl_client.c", 0x209);
    flag = TRUE;
    if (commMode == 0 || isAI != 0) {
        flag = FALSE;
    }
    client->clientId = clientId;
    client->heapId = heapId;
    client->clientType = isAI;
    client->adapter = func_ov093_021eea58(netHandle, clientId, flag, heapId);
    client->party = func_ov093_021b9af8(pokeCon, clientId);
    client->mainModule = mainModule;
    client->pokeCon = pokeCon;
    client->numCoverPos = numCoverPos;
    client->procActionIdx = 0;
    client->unk129 = clientId;
    client->unkCC = 0;
    client->viewCore = NULL;
    client->savedHp = 0;
    client->cmdQueue = GFL_HeapAllocate(heapId, sizeof(BtlServerCmdQueue), TRUE, "btl_client.c", 0x21a);
    client->mainProc = func_ov093_021cda18;
    client->mainSeq = 0;
    client->cmdCheckServer = NULL;
    client->cmdCheckEnable = FALSE;
    client->cmdCheckReq = 0;
    client->waitMsgShown = 0;
    client->shooterEnergy = 0;
    client->cmdLimitTime = 0;
    client->gameLimitTime = 0;
    client->cmdLimitOver = 0;
    client->unk1BA_0 = FALSE;
    client->forceQuit = FALSE;
    client->unk1BA_4 = FALSE;
    client->unk1BA_5 = FALSE;
    client->field = func_ov093_021ba08c(mainModule);
    client->bagMode = bagMode;
    func_ov093_021d8230(&client->clientIdList);
    func_ov093_021d3f04(&client->recPlayer);
    func_ov093_021d3d9c(client);
    client->aiRand = *rand;
    if (client->clientType == 1 && !recPlay) {
        u32 trainerClass = func_ov093_021b9fbc(client->mainModule, client->clientId);
        BtlServerFlow *serverFlow = func_ov093_021ba820(client->mainModule);

        client->aiVM = TrAI_CreateVM(client->mainModule, serverFlow, client->pokeCon, trainerClass, client->heapId);
    } else {
        client->aiVM = NULL;
    }
    for (i = 0; i < 4; i++) {
        client->hintShown[i] = 0;
    }
    if (client->clientType == 0 && func_ov093_021b90f8(client->mainModule)) {
        client->recorder = func_ov093_021ee5e4(heapId);
    } else {
        client->recorder = NULL;
    }
    return client;
}

void func_ov093_021cd8e0(BtlClient *client) {
    if (client->recorder != NULL) {
        func_ov093_021ee600(client->recorder);
    }
    if (client->aiVM != NULL) {
        func_ov096_02188080(client->aiVM);
    }
    GFL_HeapFree(client->cmdQueue);
    func_ov093_021eeae0(client->adapter);
    GFL_HeapFree(client);
}

void func_ov093_021cd914(BtlClient *client) {
    client->forceQuit = TRUE;
}

void *func_ov093_021cd924(BtlClient *client, u32 *size) {
    if (client->recorder != NULL) {
        return func_ov093_021ee63c(client->recorder, size);
    }
    return NULL;
}

void func_ov093_021cd938(BtlClient *client, void *data) {
    client->clientType = 2;
    client->recReader = data;
    func_ov093_021d3f48(&client->recPlayer, func_ov093_021ee830(data));
}

void func_ov093_021cd95c(BtlClient *client, BtlvCore *viewCore) {
    client->viewCore = viewCore;
}

void func_ov093_021cd960(BtlClient *client, BtlServer *server) {
    client->cmdCheckServer = server;
    client->cmdCheckEnable = TRUE;
}

BtlAdapter *func_ov093_021cd978(BtlClient *client) {
    return client->adapter;
}

BOOL func_ov093_021cd97c(BtlClient *client) {
    return client->mainProc(client);
}

void func_ov093_021cd984(BtlClient *client, u32 arg1) {
    func_ov093_021ee674(client->recReader);
    func_ov093_021eebdc(client->adapter);
    func_ov093_021d3f64(&client->recPlayer, arg1);
    func_ov093_021cda0c(client, func_ov093_021cdc0c);
}

void func_ov093_021cd9b0(BtlClient *client) {
    func_ov093_021d3f88(&client->recPlayer);
    func_ov093_021cda0c(client, func_ov093_021cda18);
}

BOOL func_ov093_021cd9c8(BtlClient *client) {
    if (client->clientType == 2) {
        return TRUE;
    }
    return FALSE;
}

u16 func_ov093_021cd9dc(BtlClient *client) {
    return client->recPlayer.maxChapter;
}

BOOL func_ov093_021cd9e0(BtlClient *client) {
    if (client->mainProc == func_ov093_021cdc0c) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021cd9f4(BtlClient *client, u8 value) {
    client->shooterEnergy += value;
    if (client->shooterEnergy > 14) {
        client->shooterEnergy = 14;
    }
}

void func_ov093_021cda0c(BtlClient *client, BOOL (*proc)(BtlClient *client)) {
    client->mainProc = proc;
    client->mainSeq = 0;
}

BOOL func_ov093_021cda18(BtlClient *client) {
    func_ov093_021d3fdc(client, &client->recPlayer);
    switch (client->mainSeq) {
    case 0: {
        u32 cmd = func_ov093_021eebe8(client->adapter);

        if (client->clientType == 2 && (client->unk1BA_4 || client->unk1BA_5) && cmd == 0) {
            return FALSE;
        }
        func_ov093_021cdd34(client);
        if (cmd == 9) {
            const BtlClientIDList *list;

            func_ov093_021eec20(client->adapter, (const void **)&list);
            client->clientIdList = *list;
            client->cmdSeq = 0;
            client->mainSeq = 3;
        } else if (cmd != 0) {
            u32 unk01;

            client->cmdProc = func_ov093_021cdd50(client, cmd, &unk01);
            if (unk01) {
                if (!func_ov093_021d3fac(&client->recPlayer)) {
                    client->mainSeq = 6;
                    break;
                }
            } else {
                func_ov093_021d3fc4(&client->recPlayer);
            }
            if (client->cmdProc != NULL) {
                client->mainSeq = 1;
                client->cmdSeq = 0;
            } else {
                client->cmdSeq = 0;
                client->mainSeq = 2;
            }
        } else if (client->forceQuit) {
            return TRUE;
        }
        break;
    }
    case 1:
        if (client->cmdProc(client, &client->cmdSeq)) {
            if (client->forceQuit) {
                return TRUE;
            }
            if (func_ov093_021d3f4c(&client->recPlayer)) {
                client->mainSeq = 4;
            } else {
                client->mainSeq = 2;
            }
        }
        break;
    case 2:
        if (func_ov093_021eec4c(client->adapter, client->returnData, client->returnDataSize)) {
            client->mainSeq = 0;
        }
        if (client->forceQuit) {
            return TRUE;
        }
        break;
    case 3:
        if (func_ov093_021eec4c(client->adapter, client->returnData, client->returnDataSize)) {
            client->mainSeq = 7;
        }
        if (client->forceQuit) {
            return TRUE;
        }
        break;
    case 6:
        if (func_ov093_021d3f4c(&client->recPlayer)) {
            client->mainSeq = 4;
        }
        break;
    case 4:
        if (func_ov093_021d3f60(&client->recPlayer) == 1) {
            client->mainSeq = 7;
            break;
        }
        if (client->viewCore != NULL) {
            GFL_SndPlayerSetMuteStateEx(1, 0x3e);
            PokeVoice_ResetMasterVolume();
        }
        client->mainSeq = 5;
        break;
    case 5:
        if (!GFL_SndBGMIsFading()) {
            u32 chapter = func_ov093_021d3fa8(&client->recPlayer);

            GFL_SndPlayerSetMuteStateEx(0, 1);
            func_ov093_021ba73c(client->mainModule, chapter);
            PokeVoice_SetMasterVolume(0);
        }
        break;
    case 7:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021cdc0c(BtlClient *client) {
    func_ov093_021d3fdc(client, &client->recPlayer);
    switch (client->mainSeq) {
    case 0:
        client->mainSeq = 1;
        // fall through
    case 1:
        if (!func_ov093_021d3f98(&client->recPlayer)) {
            u32 cmd = func_ov093_021eebe8(client->adapter);

            if (cmd != 0) {
                client->cmdProc = func_ov093_021cdd50(client, cmd, NULL);
                if (client->cmdProc != NULL) {
                    client->mainSeq = 2;
                    client->cmdSeq = 0;
                } else {
                    func_ov093_021cdd34(client);
                    client->mainSeq = 3;
                    client->cmdSeq = 0;
                }
            }
        } else {
            if (client->viewCore != NULL) {
                GFL_SndPlayerSetMuteStateEx(1, 1);
                func_ov093_021eae24(client->viewCore);
                GFL_SndBGMFadeIn(0x1e);
                func_ov094_021f88bc(0);
                PokeVoice_ResetMasterVolume();
            }
            client->mainSeq = 4;
        }
        break;
    case 2:
        if (client->cmdProc(client, &client->cmdSeq)) {
            client->mainSeq = 3;
            client->cmdSeq = 0;
            if (func_ov094_021f8678()) {
                func_ov094_021f88bc(1);
            }
        }
        break;
    case 3:
        if (func_ov093_021eec4c(client->adapter, client->returnData, client->returnDataSize)) {
            client->mainSeq = 1;
        }
        break;
    case 4:
        if (client->viewCore != NULL && func_ov093_021eae34(client->viewCore) && !GFL_SndBGMIsFading()) {
            u16 chapter = func_ov093_021d3fa8(&client->recPlayer);

            func_ov093_021eae74(client->viewCore, chapter, chapter);
            func_ov093_021ba7f8(client->mainModule);
        }
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}


void func_ov093_021cdd34(BtlClient *client) {
    client->dummyReturnData = 0;
    client->returnData = &client->dummyReturnData;
    client->returnDataSize = sizeof(client->dummyReturnData);
}

BtlClientCmdProc func_ov093_021cdd50(BtlClient *client, u32 cmd, u32 *unk01) {
    u32 i;

    for (i = 0; i < NELEMS(sCmdProcTable); i++) {
        if (cmd == sCmdProcTable[i].cmd) {
            if (unk01 != NULL) {
                *unk01 = sCmdProcTable[i].unk01;
            }
            return sCmdProcTable[i].procs[client->clientType];
        }
    }
    return NULL;
}

void func_ov093_021cdd94(BtlClient *client, BtlClientIDList *list) {
    *list = client->clientIdList;
}

BOOL func_ov093_021cdda0(BtlClient *client) {
    if (client->clientType != 2) {
        if (client->gameLimitTime != 0) {
            return func_ov094_021f8524(0);
        }
        return FALSE;
    }
    return client->unk1BA_4;
}

BOOL func_ov093_021cddcc(BtlClient *client) {
    return client->unk1BA_5;
}

BOOL func_ov093_021cdddc(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        if (BtlSetup_GetBattleType(client->mainModule) != 4) {
            func_ov093_021e90a0(client->viewCore, 0);
        } else {
            func_ov093_021e90a0(client->viewCore, 1);
        }
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021e90d0(client->viewCore)) {
            client->cmdLimitTime = func_ov093_021ba5c4(client->mainModule);
            client->gameLimitTime = func_ov093_021ba5d0(client->mainModule);
            if (client->cmdLimitTime != 0 || client->gameLimitTime != 0) {
                func_ov094_021f84e8(client->gameLimitTime, client->cmdLimitTime);
                if (client->gameLimitTime != 0) {
                    func_ov094_021f8504(0, 1, 1);
                }
            }
            func_ov093_021cdecc(client);
            if (func_ov093_021cd9e0(client)) {
                func_ov093_021eaea8(client->viewCore, func_ov093_021d3fa8(&client->recPlayer));
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL func_ov093_021cde8c(BtlClient *client, s32 *seq) {
    if (client->viewCore != NULL) {
        BOOL done = func_ov093_021cdddc(client, seq);

        if (done && func_ov093_021ee688(client->recReader, client->clientId)) {
            func_ov093_021d3f54(&client->recPlayer);
        }
        return done;
    }
    func_ov093_021ee688(client->recReader, client->clientId);
    return TRUE;
}

void func_ov093_021cdecc(BtlClient *client) {
    BattleMon *mon = func_ov093_021cdf1c(client);

    if (mon != NULL) {
        client->savedHp = GetBattleMonStat(mon, 13);
    }
}

u32 func_ov093_021cdee8(BtlClient *client) {
    BattleMon *mon = func_ov093_021cdf1c(client);

    if (mon != NULL) {
        u32 hp = GetBattleMonStat(mon, 13);

        if (hp >= client->savedHp) {
            return 0;
        }
        return (client->savedHp - hp) * 100 / hp;
    }
    return 0;
}

BattleMon *func_ov093_021cdf1c(BtlClient *client) {
    if (BtlSetup_GetBattleStyle(client->mainModule) == 0 && !func_ov093_021b86c0(client->mainModule)) {
        u8 pos = func_ov093_021b9064(client->mainModule, client->clientId, 0);

        return GetClientMonData(client->pokeCon, pos, 0);
    }
    return NULL;
}

BOOL func_ov093_021cdf58(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021eaa00(client->viewCore, client->unkCC);
        func_ov093_021ce068(client);
        (*seq)++;
        break;
    case 1:
        if (CheckIfOverCmdLimit(client)) {
            func_ov093_021eaa5c(client->viewCore);
            *seq = 2;
        } else {
            u8 result;

            if (func_ov093_021eaa34(client->viewCore, &result)) {
                client->unkCC = result;
                *seq = 3;
            }
        }
        break;
    case 2:
        client->unkCC = func_ov093_021cdfe4(client->unkCC);
        (*seq)++;
        break;
    case 3:
        func_ov093_021ce0e0(client);
        (*seq)++;
        break;
    case 4:
        client->returnData = &client->unkCC;
        client->returnDataSize = 1;
        return TRUE;
    }
    return FALSE;
}


// pokebw2's BattleClient_PickRotation
u8 func_ov093_021cdfe4(u8 dir) {
    u32 rand = GFL_RandomMTRange(100);

    if (dir == 0) {
        if (rand < 30) {
            dir = 3;
        } else if (rand < 60) {
            dir = 2;
        } else {
            dir = 1;
        }
    } else {
        switch (dir) {
        case 1:
        default:
            dir = rand < 50 ? 2 : 3;
            break;
        case 3:
            dir = rand < 40 ? 2 : 1;
            break;
        case 2:
            dir = rand < 40 ? 3 : 1;
            break;
        }
    }
    return dir;
}

BOOL func_ov093_021ce040(BtlClient *client, s32 *seq) {
    return TRUE;
}

BOOL func_ov093_021ce044(BtlClient *client, s32 *seq) {
    client->unkCC = func_ov093_021cdfe4(client->unkCC);
    client->returnData = &client->unkCC;
    client->returnDataSize = 1;
    return TRUE;
}

void func_ov093_021ce068(BtlClient *client) {
    if (client->cmdLimitTime != 0) {
        func_ov094_021f8504(1, 1, 1);
        client->cmdLimitOver = FALSE;
    }
}

BOOL CheckIfOverCmdLimit(BtlClient *client) {
    if (client->cmdLimitTime != 0) {
        if (!client->cmdLimitOver && func_ov094_021f8524(1)) {
            client->cmdLimitOver = TRUE;
        }
        return client->cmdLimitOver;
    }
    return FALSE;
}

BOOL CheckActionSelectForceQuit(BtlClient *client, BOOL (*proc)(BtlClient *client, s32 *seq)) {
    if (CheckIfOverCmdLimit(client)) {
        if (proc != NULL) {
            func_ov093_021ce150(client, proc);
        }
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ce0e0(BtlClient *client) {
    if (client->cmdLimitTime != 0) {
        func_ov094_021f8504(1, 0, 0);
    }
}

void func_ov093_021ce0f8(BtlClient *client, BattleMon *mon, BtlvStringParam *param) {
    if (BtlSetup_GetBattleStyle(client->mainModule) != 3) {
        Btlv_StringParam_Setup(param, 1, 0x45);
        Btlv_StringParam_AddArg(param, GetMonID(mon));
        func_ov093_021eaf78(param, 0);
    } else {
        Btlv_StringParam_Setup(param, 1, 0x46);
        Btlv_StringParam_AddArg(param, client->clientId);
        func_ov093_021eaf78(param, 0);
    }
}

void func_ov093_021ce150(BtlClient *client, BOOL (*proc)(BtlClient *client, s32 *seq)) {
    client->selActProc = proc;
    client->selActSeq = 0;
}

BOOL func_ov093_021ce160(BtlClient *client) {
    return client->selActProc(client, &client->selActSeq);
}

BOOL func_ov093_021ce170(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021ce150(client, func_ov093_021ce308);
        func_ov093_021ce068(client);
        (*seq)++;
        break;
    case 1:
        CheckIfOverCmdLimit(client);
        if (func_ov093_021ce160(client)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021ce1b0(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0: {
        BattleAction *action;
        u8 count;
        u8 flag;

        if (client->unk1BA_5) {
            func_ov093_021ce2e8(client);
            return TRUE;
        }
        action = func_ov093_021ee6b8(client->recReader, client->clientId, &count, &flag);
        if (flag) {
            func_ov093_021d3f54(&client->recPlayer);
        }
        client->returnData = action;
        client->returnDataSize = count * sizeof(BattleAction);
        if (action->bits.action == 8) {
            func_ov093_021d3fd0(&client->recPlayer);
            if (client->viewCore != NULL) {
                Btlv_StringParam_Setup(&client->strParam, 1, 0xb7);
                func_ov093_021ea614(client->viewCore, &client->strParam);
                (*seq)++;
            } else {
                func_ov093_021ce2e8(client);
                client->unk1BA_4 = TRUE;
                return TRUE;
            }
        } else if (action->bits.action == 9) {
            client->unk1BA_5 = TRUE;
            func_ov093_021d3fd0(&client->recPlayer);
            if (client->viewCore != NULL) {
                Btlv_StringParam_Setup(&client->strParam, 1, 0xb8);
                func_ov093_021ea614(client->viewCore, &client->strParam);
                *seq += 2;
            } else {
                func_ov093_021ce2e8(client);
                return TRUE;
            }
        } else {
            return TRUE;
        }
        break;
    }
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021ce2e8(client);
            client->unk1BA_4 = TRUE;
            return TRUE;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021ce2e8(client);
            return TRUE;
        }
        break;
    }
    return FALSE;
}



// The step the action selection is at
static s32 sSelectSeq;

void func_ov093_021ce2e8(BtlClient *client) {
    client->procAction = &client->actions[0];
    BattleAction_SetNull(client->procAction);
    client->returnData = client->procAction;
    client->returnDataSize = 4;
}

BOOL func_ov093_021ce308(BtlClient *client, s32 *seq) {
    u32 i;

    client->procActionIdx = 0;
    client->prevActionIdx = -1;
    client->firstActionIdx = 0;
    client->escapeSelected = FALSE;
    func_ov093_021d0b10(client, 0, client->numCoverPos, &client->pokeListCmd, &client->pokeSelect);
    func_ov093_021cf41c(client);
    selItemWork_Init(client);
    for (i = 0; i < 3; i++) {
        BattleAction_SetNull(&client->actions[i]);
    }
    for (i = 0; i < client->numCoverPos; i++) {
        if (!AutoSelectAction(client, func_ov093_021b990c(client->pokeCon, client->clientId, i), NULL)) {
            client->firstActionIdx = i;
            break;
        }
    }
    func_ov093_021ce150(client, func_ov093_021ce4d0);
    return FALSE;
}

void func_ov093_021ce3b4(BtlClient *client, const BtlvStringParam *param) {
    func_ov093_021ea614(client->viewCore, param);
    client->forceActionMsg = TRUE;
}

BOOL func_ov093_021ce3c8(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        BattleClientCmd_ForceQuitInputNotify(client->viewCore);
        (*seq)++;
        break;
    case 1:
        if (BattleClientCmd_ForceQuitInputWait(client->viewCore)) {
            u8 selectable[4];

            while (client->procActionIdx < client->numCoverPos) {
                client->procMon = func_ov093_021b990c(client->pokeCon, client->clientId, client->procActionIdx);
                client->procAction = &client->actions[client->procActionIdx];
                BattleAction_SetNull(client->procAction);
                if (!AutoSelectAction(client, client->procMon, client->procAction)) {
                    u8 count = StoreSelectableMoveFlag(client, client->procMon, selectable);

                    if (count < 4) {
                        u16 move = MoveGetID(client->procMon, count);

                        BattleAction_SetFightParam(
                            client->procAction, move,
                            DecideMoveTargetAutoForClient(client->mainModule, client->pokeCon, client->procMon, move,
                                                          &client->targetRand));
                    } else {
                        SetStruggleAction(client->procAction, client, client->procMon);
                    }
                }
                client->procActionIdx++;
            }
            (*seq)++;
        }
        break;
    case 2:
        client->returnData = client->actions;
        client->returnDataSize = client->numCoverPos * sizeof(BattleAction);
        func_ov093_021ce150(client, BattleClient_ActionSelectFinish);
        break;
    }
    return FALSE;
}

BOOL func_ov093_021ce4d0(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        client->procMon = func_ov093_021b990c(client->pokeCon, client->clientId, client->procActionIdx);
        client->procAction = &client->actions[client->procActionIdx];
        client->extraActionCount = 0;
        BattleAction_SetNull(client->procAction);
        if (AutoSelectAction(client, client->procMon, client->procAction)) {
            func_ov093_021ce150(client, func_ov093_021cf220);
        } else if (func_ov093_021ce8f0(client, &client->hintMsgId) == 1) {
            func_ov093_021ce150(client, func_ov093_021ce7d4);
        } else {
            (*seq)++;
        }
        break;
    case 1:
        if (client->prevActionIdx != client->procActionIdx || client->forceActionMsg != 0) {
            func_ov093_021ce0f8(client, client->procMon, &client->strParam);
            func_ov093_021ea65c(client->viewCore, &client->strParam);
            client->forceActionMsg = 0;
            client->prevActionIdx = client->procActionIdx;
            (*seq)++;
        } else {
            *seq += 2;
        }
        break;
    case 2:
        if (!func_ov093_021ea710(client->viewCore)) {
            return FALSE;
        }
        (*seq)++;
        // fall through
    case 3:
        func_ov093_021e9760(client->viewCore, client->procMon, client->procActionIdx > client->firstActionIdx,
                            client->procAction);
        (*seq)++;
        break;
    case 4:
        if (CheckActionSelectForceQuit(client, func_ov093_021ce3c8)) {
            func_ov093_021e979c(client->viewCore);
            return FALSE;
        }
        switch (func_ov093_021e97a4(client->viewCore)) {
        case 3:
            if (CheckCondition(client->procMon, 0x21)) {
                func_ov093_021e9964(client->viewCore);
                *seq = 6;
            } else {
                func_ov093_021cf430(client, client->procActionIdx, 0);
                func_ov093_021ce150(client, func_ov093_021cedc0);
            }
            break;
        case 1:
            func_ov093_021cf430(client, client->procActionIdx, 0);
            func_ov093_021ce150(client, func_ov093_021cea94);
            break;
        case 2:
            if (CheckCondition(client->procMon, 0x21)) {
                *seq = 6;
            } else if (client->bagMode == 1 && !func_ov093_021ba1ec(client->mainModule)) {
                func_ov093_021e9964(client->viewCore);
                *seq = 5;
            } else {
                func_ov093_021ce150(client, func_ov093_021ceeb8);
            }
            break;
        case 4:
            if (client->procActionIdx == client->firstActionIdx) {
                if (CheckCondition(client->procMon, 0x21) && GetRunMode(client->mainModule) != 2) {
                    *seq = 6;
                } else {
                    func_ov093_021cf430(client, client->procActionIdx, 0);
                    func_ov093_021ce150(client, BattleClient_ActionSelectEscape);
                }
            } else if (client->procActionIdx != 0) {
                do {
                    client->procActionIdx--;
                    if (!AutoSelectAction(client,
                                          func_ov093_021b990c(client->pokeCon, client->clientId, client->procActionIdx),
                                          NULL)) {
                        client->shooterEnergy += func_ov093_021cf43c(client, client->procActionIdx);
                        if (BattleAction_GetAction(&client->actions[client->procActionIdx]) == 3) {
                            data_ov095_0689cb09(&client->pokeSelect);
                        }
                        if (BattleAction_GetAction(&client->actions[client->procActionIdx]) == 2) {
                            selItemWork_Restore(client, client->procActionIdx);
                        }
                        func_ov093_021ce150(client, func_ov093_021ce4d0);
                        return FALSE;
                    }
                } while (client->procActionIdx != 0);
            }
            break;
        }
        break;
    case 5:
        if (func_ov093_021e9974(client->viewCore)) {
            *seq = 3;
        }
        break;
    case 6:
        func_ov093_021e9954(client->viewCore);
        (*seq)++;
        break;
    case 7:
        if (func_ov093_021e9974(client->viewCore)) {
            Btlv_StringParam_Setup(&client->strParam, 1, 0xc5);
            Btlv_StringParam_AddArg(&client->strParam, GetMonID(client->procMon));
            func_ov093_021ce3b4(client, &client->strParam);
            (*seq)++;
        }
        break;
    case 8:
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 1;
        }
        break;
    }
    return FALSE;
}




BOOL func_ov093_021ce7d4(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0: {
        u8 clientId = func_ov093_021b904c(client->mainModule, 0);
        u16 trainer = func_ov093_021ba004(client->mainModule, clientId);

        func_ov094_021f7f58(func_ov093_021ba020(client->mainModule, clientId), 9, 0, 0, 0);
        func_ov094_021f7998(0x270);
        func_ov093_021ea6b0(client->viewCore, trainer, client->hintMsgId);
        if ((client->hintMsgId == 0x13 || client->hintMsgId == 0x14) &&
            func_ov093_021d7f38(func_ov093_021ba020(client->mainModule, clientId)) && !client->unk1BA_0) {
            func_ov094_021f87ec(0x47b);
            if (func_ov094_021f8774() == 0) {
                GFL_SndBGMFadeOut(8);
                *seq = 1;
                break;
            }
        }
        *seq = 2;
        break;
    }
    case 1:
        if (!GFL_SndBGMIsFading()) {
            GFL_SndBGMPlay(0x47b, 0xffff);
            client->unk1BA_0 = TRUE;
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4() && func_ov093_021ea710(client->viewCore)) {
            func_ov094_021f7998(0x271);
            (*seq)++;
        }
        break;
    case 3:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ce150(client, func_ov093_021ce4d0);
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021ce8f0(BtlClient *client, u16 *msgId) {
    BattleParty *party;
    s32 selected;
    u32 count;
    u32 trainer;
    int msg;
    BattleMon *mon;
    u32 i;
    u8 enemy;

    if (BtlSetup_GetBattleType(client->mainModule) == 1) {
        i = 0;
        enemy = func_ov093_021b904c(client->mainModule, 0);
        trainer = func_ov093_021ba004(client->mainModule, enemy);
        if (trainer != 0) {
            party = GetPartyData(client->pokeCon, enemy);
            mon = func_ov093_021b990c(client->pokeCon, enemy, i);
            if (IsFainted(mon)) {
                count = func_ov093_021b99b8(client->mainModule, enemy);
                for (i = 1; i < count; i++) {
                    mon = func_ov093_021b990c(client->pokeCon, enemy, i);
                    if (!IsFainted(mon)) {
                        break;
                    }
                }
            }
            selected = -1;
            for (i = 0; i < 4; i++) {
                if (client->hintShown[i] == 0) {
                    msg = sTrainerHintMsgs[i];
                    if (func_0202a298(trainer, msg, client->heapId)) {
                        switch (msg) {
                        case 0x12:
                            if (GetBattleMonStat(mon, 13) <= DivideMaxHp(mon, 2)) {
                                selected = i;
                                client->hintShown[i] = TRUE;
                            }
                            break;
                        case 0x11:
                            if (!IsMonFullHP(mon)) {
                                selected = i;
                                client->hintShown[i] = TRUE;
                            }
                            break;
                        case 0x13:
                            if (GetNumMonsInParty(party) > 1 && GetAlivePartyCount(party) == 1) {
                                selected = i;
                                client->hintShown[i] = TRUE;
                            }
                            break;
                        case 0x14:
                            if (GetNumMonsInParty(party) > 1 && GetAlivePartyCount(party) == 1) {
                                if (GetBattleMonStat(mon, 13) <= DivideMaxHp(mon, 2)) {
                                    selected = i;
                                    client->hintShown[i] = TRUE;
                                }
                            }
                            break;
                        }
                    } else {
                        client->hintShown[i] = TRUE;
                    }
                }
            }
            if (selected >= 0) {
                *msgId = sTrainerHintMsgs[selected];
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL func_ov093_021cea94(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        if (BtlSetup_GetBattleStyle(client->mainModule) != 3) {
            if (CheckForSelectableMove(client, client->procMon, client->procAction)) {
                func_ov093_021ce150(client, func_ov093_021cf220);
                break;
            }
            *seq = 1;
        } else {
            func_ov093_021cecb4(client, &client->rotationParam);
            *seq = 2;
        }
        break;
    case 1:
        func_ov093_021e97bc(client->viewCore, client->procMon, client->procAction);
        *seq = 3;
        break;
    case 2:
        func_ov093_021e9808(client->viewCore, &client->rotationParam, client->procAction);
        *seq = 3;
        break;
    case 3:
        if (CheckActionSelectForceQuit(client, func_ov093_021ce3c8)) {
            func_ov093_021e98ac(client->viewCore);
            return FALSE;
        }
        if (func_ov093_021e98b4(client->viewCore)) {
            u32 actionType = BattleAction_GetAction(client->procAction);

            if (actionType == 0) {
                func_ov093_021ce150(client, func_ov093_021ce4d0);
            } else if (actionType == 5) {
                func_ov093_021ce150(client, func_ov093_021cf220);
            } else {
                BattleMon *mon = client->procMon;
                BattleAction *action = client->procAction;
                u32 pos = client->procActionIdx;
                u8 rotation = 0;

                if (BtlSetup_GetBattleStyle(client->mainModule) == 3) {
                    rotation = client->rotationParam.rotateAction.bits.target;
                    pos = func_ov093_021b9ab0(rotation);
                    mon = client->rotationParam.mons[pos].mon;
                }
                if (func_ov093_021d832c(action)) {
                    client->moveInfoPos = pos;
                    client->moveInfoIdx = func_ov093_021d5778(mon, action->bits.move);
                    if (client->moveInfoIdx != 4) {
                        func_ov093_021ce150(client, func_ov093_021ced4c);
                        return FALSE;
                    }
                }
                if (IsUnselectableMove(client, mon, action->bits.move, &client->strParam)) {
                    func_ov093_021eaf78(&client->strParam, 0xff);
                    func_ov093_021ce3b4(client, &client->strParam);
                    *seq = 7;
                    break;
                }
                if (BtlSetup_GetBattleStyle(client->mainModule) == 3 && rotation != 0 && rotation != 1 &&
                    !IsFainted(mon)) {
                    u16 move = func_ov093_021d8340(&client->rotationParam.action);

                    func_ov093_021d83fc(client->procAction++, rotation);
                    BattleAction_SetFightParam(client->procAction, move, 6);
                    client->extraActionCount++;
                }
                *seq = 4;
            }
        }
        break;
    case 4:
        if (func_ov093_021d7ef0(BtlSetup_GetBattleStyle(client->mainModule))) {
            *seq = 5;
        } else {
            func_ov093_021ce150(client, func_ov093_021cf220);
        }
        break;
    case 5:
        func_ov093_021e98bc(client->viewCore, client->procMon, client->procAction);
        *seq = 6;
        break;
    case 6:
        if (CheckActionSelectForceQuit(client, func_ov093_021ce3c8)) {
            func_ov093_021e9920(client->viewCore);
            return FALSE;
        } else {
            u32 result = func_ov093_021e98e8(client->viewCore);

            if (result == 1) {
                func_ov093_021ce150(client, func_ov093_021cf220);
            } else if (result == 2) {
                *seq = 1;
            }
        }
        break;
    case 7:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021e9858(client->viewCore);
            *seq = 3;
        }
        break;
    }
    return FALSE;
}




void func_ov093_021cecb4(BtlClient *client, BtlvSelectTargetParam *param) {
    u32 i;
    u32 j;

    for (i = 0; i < 3; i++) {
        BattleMon *mon = GetBattleMonFromParty(client->party, i);
        param->mons[i].mon = mon;
        for (j = 0; j < 4; j++) {
            param->mons[i].selectable[j] = FALSE;
        }
        if (!IsFainted(mon)) {
            u32 count = GetBattleMonMoveCount(mon);
            for (j = 0; j < count; j++) {
                if (GetMovePP(mon, j) == 0 || IsUnselectableMove(client, mon, MoveGetID(mon, j), NULL)) {
                    param->mons[i].selectable[j] = FALSE;
                } else {
                    param->mons[i].selectable[j] = TRUE;
                }
            }
        }
    }
}

BOOL func_ov093_021ced4c(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        BattleClientCmd_StartMoveInfoView(client->viewCore, client->moveInfoPos, client->moveInfoIdx);
        (*seq)++;
        break;
    case 1:
        if (CheckActionSelectForceQuit(client, NULL)) {
            BattleClientCmd_QuitPokeSelect(client->viewCore);
            *seq = 2;
            return FALSE;
        }
        if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            func_ov093_021ce150(client, func_ov093_021cea94);
        }
        break;
    case 2:
        if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            func_ov093_021ce150(client, func_ov093_021ce3c8);
        }
        break;
    }
    return FALSE;
}


BOOL func_ov093_021cedc0(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0: {
        u16 arg3;
        u8 monId;
        u8 canCancel = FALSE;
        u8 result = CanMonSwitch(client, client->procMon, &monId, &arg3);
        if (result != 4 || monId == GetMonID(client->procMon)) {
            canCancel = TRUE;
        }
        BattleClientCmd_StartPokeList(client->viewCore, &client->pokeListCmd, client->procActionIdx, canCancel,
                                      &client->pokeSelect);
        (*seq)++;
        break;
    }
    case 1:
        if (CheckActionSelectForceQuit(client, NULL)) {
            BattleClientCmd_QuitPokeSelect(client->viewCore);
            *seq = 2;
            return FALSE;
        }
        if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            if (!data_ov095_0689cb15(&client->pokeSelect)) {
                u8 slot = data_ov095_0689cb1d(&client->pokeSelect);
                if (slot < 6) {
                    func_ov093_021d8394(client->procAction, client->procActionIdx, slot);
                    func_ov093_021ce150(client, func_ov093_021cf220);
                    break;
                }
            }
            func_ov093_021ce150(client, func_ov093_021ce4d0);
        }
        break;
    case 2:
        if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            func_ov093_021ce150(client, func_ov093_021ce3c8);
        }
        break;
    }
    return FALSE;
}



BOOL func_ov093_021ceeb8(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0: {
        u8 shooterCost = func_ov093_021cf448(client);
        u8 isFirst = client->procActionIdx == client->firstActionIdx;
        u8 hiding = IsPokeBallTargetHiding(client);

        BattleClientCmd_StartItemSelect(client->viewCore, client->bagMode, client->shooterEnergy, shooterCost, isFirst, hiding);
        (*seq)++;
        break;
    }
    case 1:
        if (CheckActionSelectForceQuit(client, NULL)) {
            func_ov093_021e9e70(client->viewCore);
            *seq = 2;
            return FALSE;
        }
        if (func_ov093_021e9e80(client->viewCore)) {
            u16 item = func_ov093_021ea01c(client->viewCore);
            u16 target = func_ov093_021ea044(client->viewCore);

            if (item != 0 && target != 6) {
                u8 cost = func_ov093_021ea030(client->viewCore);
                u32 param = func_ov093_021ea04c(client->viewCore);

                if (client->shooterEnergy >= cost) {
                    client->shooterEnergy -= cost;
                } else {
                    client->shooterEnergy = 0;
                    cost = client->shooterEnergy;
                }
                func_ov093_021cf430(client, client->procActionIdx, cost);
                func_ov093_021d8358(client->procAction, item, target, param);
                func_ov093_021ea058(client->viewCore);
                if (ItemGetParam(item, 0xf) == 4) {
                    client->escapeSelected = TRUE;
                }
                selItemWork_Reserve(client, client->procActionIdx, item);
                func_ov093_021ce150(client, func_ov093_021cf220);
            } else {
                func_ov093_021ce150(client, func_ov093_021ce4d0);
            }
        }
        break;
    case 2:
        if (func_ov093_021e9e80(client->viewCore)) {
            func_ov093_021ce150(client, func_ov093_021ce3c8);
        }
        break;
    }
    return FALSE;
}



BOOL IsPokeBallTargetHiding(BtlClient *client) {
    if (BtlSetup_GetBattleType(client->mainModule) == 0) {
        BattleParty *party = func_ov093_021b9af8(client->pokeCon, 1);
        u32 numMons = GetNumMonsInParty(party);
        u32 numFront = func_ov093_021b8670(client->mainModule);
        BOOL hiding;
        u32 i;
        u32 alive;

        if (numFront > numMons) {
            numFront = numMons;
        }
        hiding = FALSE;
        alive = 0;
        for (i = 0; i < numFront; i++) {
            BattleMon *mon = GetBattleMonFromParty(party, i);
            if (!IsFainted(mon)) {
                alive++;
                if (IsSemiInvulnMove(mon)) {
                    hiding = TRUE;
                }
            }
        }
        if (alive == 1 && hiding) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL BattleClient_ActionSelectEscape(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0: {
        u16 trapAbility;
        u8 trapMonId;

        switch (GetRunMode(client->mainModule)) {
        case 0:
        default:
            if (func_ov093_021cf918(client, &trapMonId, &trapAbility) == 4) {
                *seq = 5;
            } else {
                if (trapAbility != 0) {
                    Btlv_StringParam_Setup(&client->strParam, 2, 0x365);
                    Btlv_StringParam_AddArg(&client->strParam, trapMonId);
                    Btlv_StringParam_AddArg(&client->strParam, trapAbility);
                } else {
                    Btlv_StringParam_Setup(&client->strParam, 1, 0x4a);
                }
                func_ov093_021ce3b4(client, &client->strParam);
                *seq = 1;
            }
            break;
        case 1:
            Btlv_StringParam_Setup(&client->strParam, 1, 0x4c);
            func_ov093_021ce3b4(client, &client->strParam);
            *seq = 1;
            break;
        case 2:
            Btlv_StringParam_Setup(&client->strParam, 1, 0x4d);
            func_ov093_021eaf78(&client->strParam, 0xff);
            func_ov093_021ce3b4(client, &client->strParam);
            *seq = 3;
            break;
        }
        break;
    }
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021ce0f8(client, client->procMon, &client->strParam);
            func_ov093_021ea65c(client->viewCore, &client->strParam);
            *seq = 2;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021ce150(client, func_ov093_021ce4d0);
        }
        break;
    case 3:
        func_ov093_021ea710(client->viewCore);
        if (func_ov093_021ea720(client->viewCore)) {
            Btlv_StringParam_Setup(&client->strParam, 3, 8);
            Btlv_StringParam_Setup(&client->strParam2, 3, 9);
            func_ov093_021eabc4(client->viewCore, &client->strParam, &client->strParam2, 1);
            *seq = 4;
        }
        break;
    case 4:
        if (func_ov093_021ea710(client->viewCore)) {
            u32 result;

            if (func_ov093_021eac54(client->viewCore, &result)) {
                if (result == 0) {
                    *seq = 5;
                } else {
                    func_ov093_021ce150(client, func_ov093_021ce4d0);
                }
            } else if (CheckIfOverCmdLimit(client)) {
                func_ov093_021ce150(client, func_ov093_021ce3c8);
            }
        }
        break;
    case 5:
        func_ov093_021d8414(client->procAction);
        client->escapeSelected = TRUE;
        func_ov093_021ce150(client, func_ov093_021cf220);
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_ActionSelectAutoRest
BOOL func_ov093_021cf220(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021e9964(client->viewCore);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021e9974(client->viewCore)) {
            client->procActionIdx++;
            if (client->procActionIdx < client->numCoverPos) {
                if (!client->escapeSelected) {
                    func_ov093_021ce150(client, func_ov093_021ce4d0);
                    break;
                }
                while (client->procActionIdx < client->numCoverPos) {
                    BattleMon *mon = func_ov093_021b990c(client->pokeCon, client->clientId, client->procActionIdx);
                    if (!AutoSelectAction(client, mon, &client->actions[client->procActionIdx])) {
                        BattleAction_SetNull(&client->actions[client->procActionIdx]);
                    }
                    client->procActionIdx++;
                }
            }
            if (client->procActionIdx >= client->numCoverPos) {
                u8 count = client->numCoverPos + client->extraActionCount;
                client->extraActionCount = 0;
                client->returnData = client->actions;
                client->returnDataSize = count * sizeof(BattleAction);
                func_ov093_021ce150(client, BattleClient_ActionSelectFinish);
            }
        }
        break;
    }
    return FALSE;
}

BOOL BattleClient_ActionSelectFinish(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        selItemWork_Quit(client);
        if (func_ov093_021b86c0(client->mainModule)) {
            client->waitMsgShown = TRUE;
            func_ov093_021ea7fc(client->viewCore);
            (*seq)++;
        } else {
            *seq += 2;
        }
        break;
    case 1:
        if (func_ov093_021ea814(client->viewCore)) {
            (*seq)++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ce0e0(client);
            (*seq)++;
            return TRUE;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

void selItemWork_Init(BtlClient *client) {
    u32 i;
    for (i = 0; i < 3; i++) {
        client->reservedItems[i] = 0;
    }
}

void selItemWork_Reserve(BtlClient *client, u8 index, u16 item) {
    if (client->bagMode == 0 && client->clientId == GetPlayerClientID(client->mainModule)) {
        client->reservedItems[index] = item;
        BattleClient_SubItem(client->mainModule, client->clientId, item);
    }
}

void selItemWork_Restore(BtlClient *client, u8 index) {
    if (client->bagMode == 0 && client->clientId == GetPlayerClientID(client->mainModule)) {
        u16 item = client->reservedItems[index];
        if (item != 0) {
            client->reservedItems[index] = 0;
            BattleClient_AddItem(client->mainModule, client->clientId, item);
        }
    }
}

void selItemWork_Quit(BtlClient *client) {
    u32 i;
    if (client->bagMode == 0) {
        for (i = 0; i < 3; i++) {
            selItemWork_Restore(client, i);
        }
    }
}

// pokebw2's shooterWork_Init
void func_ov093_021cf41c(BtlClient *client) {
    MI_CpuFill8(client->shooterCost, 0, 3);
}

// pokebw2's shooterWork_SetCost
void func_ov093_021cf430(BtlClient *client, u8 index, u8 cost) {
    client->shooterCost[index] = cost;
}

// pokebw2's shooterWork_GetCost
u8 func_ov093_021cf43c(BtlClient *client, u8 index) {
    return client->shooterCost[index];
}

// pokebw2's shooterWork_GetTotalCost
u8 func_ov093_021cf448(BtlClient *client) {
    u32 i;
    u32 total = 0;
    for (i = 0; i < client->procActionIdx; i++) {
        total += client->shooterCost[i];
    }
    return total;
}

BOOL AutoSelectAction(BtlClient *client, BattleMon *mon, BattleAction *action) {
    if (IsFainted(mon)) {
        if (action != NULL) {
            BattleAction_SetNull(action);
        }
        return TRUE;
    }
    if (GetAdditionalConditionFlag(mon, 12)) {
        if (action != NULL) {
            BattleAction_SetSkip(action);
        }
        return TRUE;
    }
    if (CheckCondition(mon, 25)) {
        BattleConditionCont cont = GetConditionContinuationParam(mon, 25);
        u16 move = Condition_GetParam(cont);
        u8 target = GetPrevTargetPos(mon);
        if (action != NULL) {
            BattleAction_SetFightParam(action, move, target);
        }
        return TRUE;
    }
    if (CheckCondition(mon, 26)) {
        u16 move = GetPreviousMoveID(mon);
        u8 target = GetPrevTargetPos(mon);
        func_ov093_021d5778(mon, move);
        if (action != NULL) {
            BattleAction_SetFightParam(action, move, target);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL CheckForSelectableMove(BtlClient *client, BattleMon *mon, BattleAction *action) {
    u8 moveCount = GetBattleMonMoveCount(mon);
    u32 selectable = 0;
    u32 i;
    for (i = 0; i < moveCount; i++) {
        if (!IsUnselectableMove(client, mon, MoveGetID(mon, i), NULL) && GetMovePP(mon, i) != 0) {
            selectable++;
        }
    }
    if (selectable == 0) {
        SetStruggleAction(action, client, mon);
        return TRUE;
    }
    return FALSE;
}

void SetStruggleAction(BattleAction *action, BtlClient *client, BattleMon *mon) {
    u8 target =
        DecideMoveTargetAutoForClient(client->mainModule, client->pokeCon, mon, MOVE_STRUGGLE, &client->targetRand);
    BattleAction_SetFightParam(action, MOVE_STRUGGLE, target);
}

BOOL IsUnselectableMove(BtlClient *client, BattleMon *mon, u16 move, BtlvStringParam *param) {
    if (move == MOVE_STRUGGLE) {
        return FALSE;
    }
    if (CanMonUseHeldItem(client, mon) && CheckCondition(mon, 27)) {
        u16 lockedMove = Condition_GetParam(GetConditionContinuationParam(mon, 27));
        if (MoveIsUsable(mon, lockedMove) && lockedMove != move) {
            if (param != NULL) {
                Btlv_StringParam_Setup(param, 1, 0x63);
                Btlv_StringParam_AddArg(param, GetBattleMonHeldItem(mon));
                Btlv_StringParam_AddArg(param, lockedMove);
            }
            return TRUE;
        }
    }
    if (CheckCondition(mon, 23)) {
        BattleConditionCont cont = GetConditionContinuationParam(mon, 23);
        u16 encoreMove = Condition_GetParam(cont);
        if (move != encoreMove) {
            if (param != NULL) {
                Btlv_StringParam_Setup(param, 1, 0x64);
                Btlv_StringParam_AddArg(param, GetMonID(mon));
                Btlv_StringParam_AddArg(param, encoreMove);
            }
            return TRUE;
        }
    }
    if (CheckCondition(mon, CONDITION_TAUNT) && !PML_MoveIsDamaging(move)) {
        if (param != NULL) {
            Btlv_StringParam_Setup(param, 2, 0x23b);
            Btlv_StringParam_AddArg(param, GetMonID(mon));
            Btlv_StringParam_AddArg(param, move);
        }
        return TRUE;
    }
    if (CheckCondition(mon, CONDITION_TORMENT) && move == GetPreviousMoveUsed(mon)) {
        if (param != NULL) {
            Btlv_StringParam_Setup(param, 2, 0x244);
            Btlv_StringParam_AddArg(param, GetMonID(mon));
            Btlv_StringParam_AddArg(param, move);
        }
        return TRUE;
    }
    if (CheckCondition(mon, CONDITION_DISABLE) && move == GetDisabledMove(mon, CONDITION_DISABLE) &&
        move != MOVE_STRUGGLE) {
        if (param != NULL) {
            Btlv_StringParam_Setup(param, 2, 0x253);
            Btlv_StringParam_AddArg(param, GetMonID(mon));
            Btlv_StringParam_AddArg(param, move);
        }
        return TRUE;
    }
    if (CheckCondition(mon, CONDITION_HEAL_BLOCK) && getMoveFlag(move, 12)) {
        if (param != NULL) {
            Btlv_StringParam_Setup(param, 2, 0x37a);
            Btlv_StringParam_AddArg(param, GetMonID(mon));
            Btlv_StringParam_AddArg(param, move);
        }
        return TRUE;
    }
    if (CheckFieldEffect(client->field, 3) && CheckImprison(client->field, client->pokeCon, mon, move)) {
        if (param != NULL) {
            Btlv_StringParam_Setup(param, 2, 0x24d);
            Btlv_StringParam_AddArg(param, GetMonID(mon));
            Btlv_StringParam_AddArg(param, move);
        }
        return TRUE;
    }
    if (CheckFieldEffect(client->field, FIELD_CONDITION_GRAVITY) && getMoveFlag(move, 9)) {
        if (param != NULL) {
            Btlv_StringParam_Setup(param, 2, 0x43e);
            Btlv_StringParam_AddArg(param, GetMonID(mon));
            Btlv_StringParam_AddArg(param, move);
        }
        return TRUE;
    }
    return FALSE;
}

u8 StoreSelectableMoveFlag(BtlClient *client, BattleMon *mon, u8 *selectable) {
    u8 moveCount = GetBattleMonMoveCount(mon);
    u8 first = 4;
    u8 i;
    for (i = 0; i < moveCount; i++) {
        if (GetMovePP(mon, i) != 0) {
            selectable[i] = !IsUnselectableMove(client, mon, MoveGetID(mon, i), NULL);
            if (first == 4 && selectable[i]) {
                first = i;
            }
        } else {
            selectable[i] = FALSE;
        }
    }
    for (; i < 4; i++) {
        selectable[i] = FALSE;
    }
    return first;
}

BOOL CanMonUseHeldItem(BtlClient *client, BattleMon *mon) {
    if (CheckFieldEffect(client->field, 7)) {
        return FALSE;
    }
    if (CheckCondition(mon, CONDITION_EMBARGO)) {
        return FALSE;
    }
    if (GetBattleMonStat(mon, 0x11) != ABILITY_KLUTZ) {
        return TRUE;
    }
    return FALSE;
}

u32 CanMonSwitch(BtlClient *client, BattleMon *mon, u8 *trapMonId, u16 *trapAbility) {
    u32 result;
    if (CanMonUseHeldItem(client, mon) && GetBattleMonHeldItem(mon) == ITEM_SHED_SHELL) {
        return 4;
    }
    result = IsMonTrapped(client, mon, trapMonId, trapAbility);
    if (result == 4) {
        *trapMonId = 31;
        *trapAbility = 0;
        return 4;
    }
    return result;
}

// pokebw2's CheckEscapeBlocked
u32 func_ov093_021cf918(BtlClient *client, u8 *trapMonId, u16 *trapAbility) {
    u32 i;
    for (i = 0; i < client->numCoverPos; i++) {
        BattleMon *mon = GetBattleMonFromParty(client->party, i);
        if (!IsFainted(mon)) {
            if (CanMonUseHeldItem(client, mon) && GetBattleMonHeldItem(mon) == ITEM_SMOKE_BALL) {
                return 4;
            }
            if (GetBattleMonStat(mon, 0x11) == ABILITY_RUN_AWAY) {
                return 4;
            }
        }
    }
    for (i = 0; i < client->numCoverPos; i++) {
        u32 result = IsMonTrapped(client, GetBattleMonFromParty(client->party, i), trapMonId, trapAbility);
        if (result != 4) {
            return result;
        }
    }
    return 4;
}

u32 IsMonTrapped(BtlClient *client, BattleMon *mon, u8 *trapMonId, u16 *trapAbility) {
    u8 monIds[4];
    u8 count =
        func_ov093_021b8d38(client->mainModule, client->pokeCon,
                            0x100 | MonIDToBattlePos(client->mainModule, client->pokeCon, GetMonID(mon)), monIds);
    u8 i;
    for (i = 0; i < count; i++) {
        BattleMon *opponent = GetPokeParam(client->pokeCon, monIds[i]);
        u16 ability = GetBattleMonStat(opponent, 0x11);
        // The ID is read and dropped here, likely left from a removed debug print
        GetMonID(opponent);
        if (ability == ABILITY_SHADOW_TAG && DoesMonHaveShadowTag(client, mon)) {
            *trapMonId = GetMonID(opponent);
            *trapAbility = ability;
            return 0;
        }
        if (ability == ABILITY_ARENA_TRAP && IsMonTrappedByArenaTrap(client, mon)) {
            *trapMonId = GetMonID(opponent);
            *trapAbility = ability;
            return 0;
        }
        if (ability == ABILITY_MAGNET_PULL && IsMonSteelType(client, mon)) {
            *trapMonId = GetMonID(opponent);
            *trapAbility = ability;
            return 0;
        }
    }
    if (CheckCondition(mon, CONDITION_MEAN_LOOK) || CheckCondition(mon, CONDITION_BIND) ||
        CheckCondition(mon, CONDITION_INGRAIN)) {
        *trapMonId = GetMonID(mon);
        *trapAbility = 0;
        return 3;
    }
    return 4;
}

BOOL DoesMonHaveShadowTag(BtlClient *client, BattleMon *mon) {
    if (GetBattleMonStat(mon, 0x11) != ABILITY_SHADOW_TAG) {
        return TRUE;
    }
    return FALSE;
}

BOOL IsMonTrappedByArenaTrap(BtlClient *client, BattleMon *mon) {
    BOOL canUseItem = CanMonUseHeldItem(client, mon);
    if (CheckFieldEffect(client->field, FIELD_CONDITION_GRAVITY)) {
        return TRUE;
    }
    if (CheckCondition(mon, 31)) {
        return TRUE;
    }
    if (CheckCondition(mon, CONDITION_INGRAIN)) {
        return TRUE;
    }
    if (canUseItem && GetBattleMonHeldItem(mon) == ITEM_IRON_BALL) {
        return TRUE;
    }
    if (GetBattleMonStat(mon, 0x11) == ABILITY_LEVITATE) {
        return FALSE;
    }
    if (DoesMonHaveType(mon, TYPE_FLYING)) {
        return FALSE;
    }
    if (CheckCondition(mon, 30)) {
        return FALSE;
    }
    if (CheckCondition(mon, 32)) {
        return FALSE;
    }
    if (canUseItem && GetBattleMonHeldItem(mon) == ITEM_AIR_BALLOON) {
        return FALSE;
    }
    return TRUE;
}

BOOL IsMonSteelType(BtlClient *client, BattleMon *mon) {
    if (DoesMonHaveType(mon, TYPE_STEEL)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ClearAISwitchReserved
void func_ov093_021cfba4(BtlClient *client) {
    u32 i;
    for (i = 0; i < 6; i++) {
        client->aiSwitchReserved[i] = -1;
    }
}

// pokebw2's IsAISwitchReserved
BOOL func_ov093_021cfbbc(BtlClient *client, u8 slot) {
    u32 i;
    for (i = 0; i < 6; i++) {
        if (slot == client->aiSwitchReserved[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's ReserveAISwitch
void func_ov093_021cfbe0(BtlClient *client, u8 index, u8 slot) {
    if (index < 6) {
        client->aiSwitchReserved[index] = slot;
    }
}

BOOL AISwitchChecks(BtlClient *client, BattleMon *mon, u8 index, u8 *slot) {
    u8 switchTo = 6;
    u8 trapMonId;
    u16 trapAbility;
    u8 slots[6];
    BattleMon *target;
    BOOL shouldSwitch;

    if (CanMonSwitch(client, mon, &trapMonId, &trapAbility) != 4) {
        return FALSE;
    }
    if (GetAlivePartyCount(client->party) <= client->numCoverPos) {
        return FALSE;
    }
    target = func_ov093_021cfd50(client, MonIDToBattlePos(client->mainModule, client->pokeCon, GetMonID(mon)));
    if (target == NULL) {
        return FALSE;
    }
    if (IsIllusionEnabled(target)) {
        target = GetIllusionDisguise(client->mainModule, client->pokeCon, target);
    }
    if (ShouldSwitchIfPerishSongLastTurn(client, mon)) {
        shouldSwitch = TRUE;
    } else if (ShouldSwitchIfWonderGuard(client, mon, target)) {
        shouldSwitch = TRUE;
    } else if (ShouldSwitchIfNoEffectiveMoves(client, mon, target)) {
        shouldSwitch = TRUE;
    } else if (ShouldSwitchIfChoicedIntoIneffectiveMove(client, mon, target)) {
        shouldSwitch = TRUE;
    } else if (ShouldSwitchIfTypeAbsorbingAbility(client, mon, target, &switchTo)) {
        shouldSwitch = TRUE;
    } else if (ShouldSwitchIfAsleepWithNaturalCure(client, mon, &switchTo)) {
        shouldSwitch = TRUE;
    } else if (FinalSwitchChecks(client, mon, target, &switchTo)) {
        shouldSwitch = TRUE;
    } else {
        return FALSE;
    }
    if (!shouldSwitch) {
        return FALSE;
    }
    if (switchTo == 6) {
        u32 count = GetNumBattleReadyPartyMons(client, slots);
        u32 i;
        PickBestMonToSwitchInto(client, slots, count, target);
        for (i = 0; i < count; i++) {
            if (!func_ov093_021cfbbc(client, slots[i])) {
                switchTo = slots[i];
                break;
            }
        }
    }
    if (switchTo == 6) {
        return FALSE;
    }
    func_ov093_021cfbe0(client, index, switchTo);
    *slot = switchTo;
    return TRUE;
}

// pokebw2's PickRandomOpponent
BattleMon *func_ov093_021cfd50(BtlClient *client, u8 pos) {
    u8 monIds[4];
    u8 count = func_ov093_021b8d38(client->mainModule, client->pokeCon, 0x100 | pos, monIds);
    if (count != 0) {
        u8 index = MATH_Rand32(&client->aiRand, count);
        return GetPokeParam(client->pokeCon, monIds[index]);
    }
    return NULL;
}

BOOL ShouldSwitchIfPerishSongLastTurn(BtlClient *client, BattleMon *mon) {
    if (CheckCondition(mon, CONDITION_PERISH_SONG)) {
        BattleConditionCont cont = GetConditionContinuationParam(mon, CONDITION_PERISH_SONG);
        u8 turns = func_ov093_021e8afc(cont);
        u8 count = func_ov093_021d631c(mon, CONDITION_PERISH_SONG);
        if (count + 1 == turns) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ShouldSwitchIfWonderGuard(BtlClient *client, BattleMon *mon, BattleMon *target) {
    if (BtlSetup_GetBattleStyle(client->mainModule) != BTL_STYLE_SINGLE) {
        return FALSE;
    }
    if (GetBattleMonStat(target, 0x11) == ABILITY_WONDER_GUARD &&
        !DoesMonHaveSuperEffectiveMove(client, mon, target, TYPE_EFFECTIVENESS_DOUBLE) &&
        CheckIfMonToSwitchToWithSEMove(client, target, TYPE_EFFECTIVENESS_DOUBLE) &&
        MATH_Rand32(&client->aiRand, 3) < 2) {
        return TRUE;
    }
    return FALSE;
}

BOOL ShouldSwitchIfNoEffectiveMoves(BtlClient *client, BattleMon *mon, BattleMon *target) {
    PokeTypePair targetType = GetPokeType(target);
    u8 moveCount = GetBattleMonMoveCount(mon);
    u32 i;
    u32 damaging = 0;
    for (i = 0; i < moveCount; i++) {
        u16 move = MoveGetID(mon, i);
        if (PML_MoveIsDamaging(move)) {
            if (func_ov093_021d799c(PML_MoveGetType(move), targetType) != TYPE_EFFECTIVENESS_IMMUNE) {
                return FALSE;
            }
            damaging++;
        }
    }
    if (damaging >= 2) {
        if (CheckIfMonToSwitchToWithSEMove(client, target, TYPE_EFFECTIVENESS_DOUBLE)) {
            return MATH_Rand32(&client->aiRand, 3) < 2;
        }
        if (CheckIfMonToSwitchToWithSEMove(client, target, TYPE_EFFECTIVENESS_NORMAL)) {
            if (MATH_Rand32(&client->aiRand, 2) < 1) {
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}

BOOL ShouldSwitchIfChoicedIntoIneffectiveMove(BtlClient *client, BattleMon *mon, BattleMon *target) {
    if (CheckCondition(mon, 27)) {
        BattleConditionCont cont = GetConditionContinuationParam(mon, 27);
        u16 move = Condition_GetParam(cont);
        if (move != 0) {
            BOOL isDamaging = PML_MoveIsDamaging(move);
            u8 type = PML_MoveGetType(move);
            if (func_ov093_021d799c(type, GetPokeType(target)) == TYPE_EFFECTIVENESS_IMMUNE && isDamaging) {
                if (CheckIfMonToSwitchToWithSEMove(client, target, TYPE_EFFECTIVENESS_DOUBLE)) {
                    return MATH_Rand32(&client->aiRand, 3) < 2;
                }
                if (CheckIfMonToSwitchToWithSEMove(client, target, TYPE_EFFECTIVENESS_NORMAL)) {
                    if (MATH_Rand32(&client->aiRand, 2) < 1) {
                        return TRUE;
                    }
                    return FALSE;
                }
            }
            if (!isDamaging) {
                if (MATH_Rand32(&client->aiRand, 2) < 1) {
                    return TRUE;
                }
                return FALSE;
            }
        }
    }
    return FALSE;
}

BOOL ShouldSwitchIfTypeAbsorbingAbility(BtlClient *client, BattleMon *mon, BattleMon *target, u8 *slot) {
    BattleMonDamageRecord record;
    u8 found;
    u32 i;
    u8 turnsAgo = 0;
    u32 j;

    if (DoesMonHaveSuperEffectiveMove(client, mon, target, TYPE_EFFECTIVENESS_DOUBLE) &&
        MATH_Rand32(&client->aiRand, 3) == 0) {
        return FALSE;
    }
    while (GetDamageReceived(mon, 1, turnsAgo++, &record)) {
        for (i = 0; i < 4; i++) {
            const TypeAbsorbingAbilities *entry = &sTypeAbsorbingAbilities[i];
            if (record.type == entry->type) {
                for (j = 0; j < 4; j++) {
                    if (entry->abilities[j] == 0) {
                        break;
                    }
                    if (CheckMonsForTypeAbsorbingAbility(client, entry->abilities[j], &found) &&
                        MATH_Rand32(&client->aiRand, 2) == 0) {
                        *slot = found;
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

BOOL ShouldSwitchIfAsleepWithNaturalCure(BtlClient *client, BattleMon *mon, u8 *slot) {
    BattleMonDamageRecord record;
    u8 found;

    if (GetBattleMonStat(mon, 0x11) == ABILITY_NATURAL_CURE &&
        (CheckCondition(mon, CONDITION_SLEEP) || CheckCondition(mon, CONDITION_FREEZE)) &&
        GetHPRatio(mon) >= FX32_CONST(50)) {
        if (GetDamageReceived(mon, 1, 0, &record)) {
            if (func_ov093_021d05c4(client, record.type, TYPE_EFFECTIVENESS_HALF, &found)) {
                *slot = found;
                return TRUE;
            }
        } else {
            if (MATH_Rand32(&client->aiRand, 2) == 0) {
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}

BOOL FinalSwitchChecks(BtlClient *client, BattleMon *mon, BattleMon *target, u8 *slot) {
    BattleMonDamageRecord record;
    u32 stages;
    u32 stat;
    u32 numMons;
    u32 i;

    if (DoesMonHaveSuperEffectiveMove(client, mon, target, TYPE_EFFECTIVENESS_DOUBLE) &&
        MATH_Rand32(&client->aiRand, 10) != 0) {
        return FALSE;
    }
    stages = 0;
    for (stat = BATTLEMON_ATTACK_STAGE; stat <= BATTLEMON_EVASION_STAGE; stat++) {
        stages += GetBattleMonStat(mon, stat);
    }
    if (stages >= 4) {
        return FALSE;
    }
    if (GetDamageReceived(mon, 1, 0, &record)) {
        numMons = GetNumMonsInParty(client->party);
        for (i = GetNumMonsOnField(BtlSetup_GetBattleStyle(client->mainModule), client->numCoverPos); i < numMons;
             i++) {
            if (!func_ov093_021cfbbc(client, i)) {
                BattleMon *candidate = GetBattleMonFromParty(client->party, i);
                if (DoesMonHaveSuperEffectiveMove(client, candidate, target, TYPE_EFFECTIVENESS_DOUBLE)) {
                    s32 effectiveness = func_ov093_021d799c(record.type, GetPokeType(candidate));
                    if (effectiveness == TYPE_EFFECTIVENESS_IMMUNE) {
                        if (MATH_Rand32(&client->aiRand, 2) == 0) {
                            *slot = i;
                            return TRUE;
                        }
                    } else if (effectiveness < TYPE_EFFECTIVENESS_NORMAL) {
                        if (MATH_Rand32(&client->aiRand, 3) == 0) {
                            *slot = i;
                            return TRUE;
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}

BOOL CheckMonsForTypeAbsorbingAbility(BtlClient *client, u16 ability, u8 *slot) {
    u32 numMons = GetNumMonsInParty(client->party);
    u32 i;
    for (i = GetNumMonsOnField(BtlSetup_GetBattleStyle(client->mainModule), client->numCoverPos); i < numMons; i++) {
        if (!func_ov093_021cfbbc(client, i)) {
            BattleMon *candidate = GetBattleMonFromParty(client->party, i);
            if (!IsFainted(candidate) && ability == GetBattleMonStat(candidate, 0x11)) {
                *slot = i;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL CheckIfMonToSwitchToWithSEMove(BtlClient *client, BattleMon *target, s32 minEffectiveness) {
    u32 numMons = GetNumMonsInParty(client->party);
    u32 i;
    for (i = GetNumMonsOnField(BtlSetup_GetBattleStyle(client->mainModule), client->numCoverPos); i < numMons; i++) {
        if (!func_ov093_021cfbbc(client, i) &&
            DoesMonHaveSuperEffectiveMove(client, GetBattleMonFromParty(client->party, i), target, minEffectiveness)) {
            return TRUE;
        }
    }
    return FALSE;
}

static inline void SetReturnData(BtlClient *client, const void *data, u32 size) {
    client->returnData = data;
    client->returnDataSize = size;
}

// The party mon at an index; the index is wider than GetBattleMonFromParty's, which narrows it at each call
static inline BattleMon *GetPartyMonAt(BtlClient *client, u32 idx) {
    return GetBattleMonFromParty(client->party, idx);
}

// Finds a party mon in the back that takes at most maxEffect from moveType
// pokebw2's FindPartyMonResistingType
BOOL func_ov093_021d05c4(BtlClient *client, u8 moveType, s32 maxEffect, u8 *partyIdx) {
    u8 count = GetNumMonsInParty(client->party);
    u32 i;
    BattleMon *mon;

    for (i = GetNumMonsOnField(BtlSetup_GetBattleStyle(client->mainModule), client->numCoverPos); i < count; i++) {
        if (!func_ov093_021cfbbc(client, i)) {
            mon = GetBattleMonFromParty(client->party, i);
            if (CanPokemonBattle(mon) && func_ov093_021d799c(moveType, GetPokeType(mon)) <= maxEffect) {
                *partyIdx = i;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL DoesMonHaveSuperEffectiveMove(BtlClient *client, BattleMon *attacker, BattleMon *defender, s32 minEffect) {
    PokeTypePair types;
    u8 moveCount;
    u32 i;
    u16 move;

    if (!IsFainted(attacker)) {
        types = GetPokeType(defender);
        moveCount = GetBattleMonMoveCount(attacker);
        for (i = 0; i < moveCount; i++) {
            move = MoveGetID(attacker, i);
            if (GetMovePP(attacker, i) != 0 && PML_MoveIsDamaging(move) &&
                !IsUnselectableMove(client, attacker, move, NULL) &&
                func_ov093_021d799c(PML_MoveGetType(move), types) >= minEffect) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// Picks a rotation position to rotate to at random among the ones whose mon can fight, or 1 to stay
// pokebw2's AIPickRotationPos
u8 func_ov093_021d06c8(BtlClient *client) {
    u8 positions[3];
    u32 count = 1;

    positions[0] = 1;
    if (!IsFainted(GetPartyMonAt(client, func_ov093_021b9ab0(2)))) {
        positions[count++] = 2;
    }
    if (!IsFainted(GetPartyMonAt(client, func_ov093_021b9ab0(3)))) {
        positions[count++] = 3;
    }
    if (count > 1) {
        return positions[MATH_Rand32(&client->aiRand, count)];
    }
    return 1;
}

BOOL AISelectAction(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        func_ov093_021cfba4(client);
        client->procActionIdx = 0;
        client->extraActionCount = 0;
        *seq = 1;
        // fall through
    case 1:
        if (client->procActionIdx < client->numCoverPos) {
            u16 item;
            u8 slot;
            u8 selectable[4];

            client->procMon = GetBattleMonFromParty(client->party, client->procActionIdx);
            client->procAction = &client->actions[client->procActionIdx];
            if (IsFainted(client->procMon)) {
                BattleAction_SetNull(client->procAction);
                *seq = 3;
                break;
            }
            if (AutoSelectAction(client, client->procMon, client->procAction)) {
                *seq = 3;
                break;
            }
            item = func_ov093_021d3dc4(client, client->procMon, client->party);
            if (item != 0) {
                func_ov093_021d8358(client->procAction, item, client->procActionIdx, 0);
                *seq = 3;
                break;
            }
            if (BtlSetup_GetBattleType(client->mainModule) != 0 &&
                AISwitchChecks(client, client->procMon, client->procActionIdx, &slot)) {
                func_ov093_021d8394(client->procAction, client->procActionIdx, slot);
                *seq = 3;
                break;
            }
            if (BtlSetup_GetBattleStyle(client->mainModule) == 3) {
                u8 partner = func_ov093_021d06c8(client);

                if (partner != 1) {
                    u8 pos = func_ov093_021b9ab0(partner);

                    func_ov093_021d83fc(client->procAction, partner);
                    client->procMon = GetBattleMonFromParty(client->party, pos);
                    client->extraActionCount = 1;
                    client->procAction++;
                }
            }
            if (CheckCondition(client->procMon, 0x19)) {
                u16 move = Condition_GetParam(GetConditionContinuationParam(client->procMon, 0x19));

                BattleAction_SetFightParam(client->procAction, move,
                                           DecideMoveTargetAutoForClient(client->mainModule, client->pokeCon,
                                                                         client->procMon, move, &client->aiRand));
                *seq = 3;
                break;
            }
            if (CheckForSelectableMove(client, client->procMon, client->procAction)) {
                *seq = 3;
            }
            GetBattleMonMoveCount(client->procMon);
            if (StoreSelectableMoveFlag(client, client->procMon, selectable) != 4) {
                func_ov096_021880bc(client->aiVM, selectable,
                                    MonIDToBattlePos(client->mainModule, client->pokeCon, GetMonID(client->procMon)));
                *seq = 2;
                break;
            }
            SetStruggleAction(client->procAction, client, client->procMon);
            *seq = 3;
        }
        break;
    case 2:
        if (!func_ov096_02187fb0(client->aiVM)) {
            u8 moveIdx = func_ov096_02188134(client->aiVM);

            if (moveIdx != 4) {
                u8 target = func_ov096_02188140(client->aiVM);

                BattleAction_SetFightParam(client->procAction, MoveGetID(client->procMon, moveIdx), target);
                *seq = 3;
                break;
            }
            func_ov093_021d8414(client->procAction);
            *seq = 3;
        }
        break;
    case 3:
        client->procActionIdx++;
        if (client->procActionIdx >= client->numCoverPos) {
            SetReturnData(client, client->actions, (client->numCoverPos + client->extraActionCount) * sizeof(BattleAction));
            client->extraActionCount = 0;
            return TRUE;
        }
        *seq = 1;
        break;
    }
    return FALSE;
}


u8 GetNumBattleReadyPartyMons(BtlClient *client, u8 *list) {
    u8 num;
    u8 count = GetNumMonsInParty(client->party);
    u8 i = GetNumMonsOnField(BtlSetup_GetBattleStyle(client->mainModule), client->numCoverPos);

    num = 0;
    for (; i < count; i++) {
        if (CanPokemonBattle(GetBattleMonFromParty(client->party, i))) {
            if (list != NULL) {
                list[num] = i;
            }
            num++;
        }
    }
    return num;
}

void PickBestMonToSwitchInto(BtlClient *client, u8 *list, u8 count, BattleMon *defender) {
    u16 power[6];
    PokeTypePair types;
    u8 i;
    BattleMon *mon;
    u8 moveCount;
    u8 j;
    u16 move;
    u8 moveType;
    u16 basePower;
    u16 tmpPower;
    u8 tmpIdx;

    types = GetPokeType(defender);
    for (i = 0; i < count; i++) {
        power[i] = 0;
        mon = GetBattleMonFromParty(client->party, list[i]);
        if (!IsFainted(mon)) {
            moveCount = GetBattleMonMoveCount(mon);
            for (j = 0; j < moveCount; j++) {
                if (GetMovePP(mon, j) != 0) {
                    move = MoveGetID(mon, j);
                    if (PML_MoveIsDamaging(move)) {
                        moveType = PML_MoveGetType(move);
                        basePower = PML_MoveGetBasePower(move);
                        if (basePower < 10) {
                            basePower = 60;
                        }
                        switch (func_ov093_021d799c(moveType, types)) {
                        case 5:
                            basePower *= 4;
                            break;
                        case 4:
                            basePower *= 2;
                            break;
                        case 2:
                            basePower /= 2;
                            break;
                        case 1:
                            basePower /= 4;
                            break;
                        case 0:
                            basePower = 0;
                            break;
                        }
                        if (power[i] < basePower) {
                            power[i] = basePower;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (power[i] < power[j]) {
                tmpPower = power[i];
                power[i] = power[j];
                power[j] = tmpPower;
                tmpIdx = list[i];
                list[i] = list[j];
                list[j] = tmpIdx;
            }
        }
    }
}

void func_ov093_021d0b10(BtlClient *client, u8 mode, u8 count, BtlvPokeListCmd *cmd,
                                BtlvPokeSelectParam *select) {
    data_ov095_0689ca99(cmd, client->party, count, mode);
    data_ov095_0689cab1(cmd, client->numCoverPos);
    data_ov095_0689cae1(select, cmd);
}










void func_ov093_021d0b6c(BtlClient *client, const BtlvPokeSelectParam *select) {
    u8 i;

    for (i = 0; i < select->count; i++) {
        func_ov093_021d8394(&client->actions[i], select->position[i], select->partyIndex[i]);
    }
    client->returnData = client->actions;
    client->returnDataSize = select->count * sizeof(BattleAction);
}

void func_ov093_021d0bb0(BtlClient *client) {
    u8 start;
    u8 num = 0;
    u8 partyCount = GetNumMonsInParty(client->party);
    u32 i;
    u32 j;
    u8 clientId;
    u8 idx;

    start = func_ov093_021b99c0(client->mainModule, client->clientId);
    for (i = 0; i < client->changePokeCount; i++) {
        func_ov093_021b8e28(client->mainModule, client->changePokePos[i], &clientId, &idx);
        for (j = start; j < partyCount; j++) {
            if (!IsFainted(GetBattleMonFromParty(client->party, j))) {
                func_ov093_021d8394(&client->actions[num++], idx, j);
                start = j + 1;
                break;
            }
        }
    }
    client->returnData = client->actions;
    client->returnDataSize = num * sizeof(BattleAction);
}

// Collects the positions in the server's request that belong to this client
// pokebw2's CollectOwnChangePositions
u8 func_ov093_021d0c78(BtlClient *client, u8 *positions) {
    const u8 *data;
    u8 size = func_ov093_021eec20(client->adapter, (const void **)&data);
    u32 i;
    u32 count;

    for (i = 0, count = 0; i < size; i++) {
        if (client->clientId == func_ov093_021b8de4(client->mainModule, data[i])) {
            positions[count++] = data[i];
        }
    }
    return count;
}

BOOL func_ov093_021d0cc4(BtlClient *client, s32 *seq) {
    u32 result;
    u8 yes;

    switch (*seq) {
    case 0:
        Btlv_StringParam_Setup(&client->strParam, 3, 5);
        Btlv_StringParam_Setup(&client->strParam2, 3, 1);
        func_ov093_021eabc4(client->viewCore, &client->strParam, &client->strParam2, 0);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021eac54(client->viewCore, &result)) {
            yes = FALSE;
            if (result != 0) {
                yes = TRUE;
            }
            client->yesNoResult = yes;
            client->returnData = &client->yesNoResult;
            client->returnDataSize = 1;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d0d38(BtlClient *client, s32 *seq) {
    return func_ov093_021d0e7c(client, seq, 2);
}

BOOL func_ov093_021d0d44(BtlClient *client, s32 *seq) {
    return func_ov093_021d0e7c(client, seq, 1);
}

// The AI's choice of the mons to send out
// pokebw2's AISelectChangePoke
BOOL func_ov093_021d0d50(BtlClient *client, s32 *seq) {
    u8 list[6];
    u8 idx;
    u8 clientId;
    u8 num;
    u8 count;
    u8 i;
    BattleMon *defender;
    u32 size;

    client->changePokeCount = func_ov093_021d0c78(client, client->changePokePos);
    if (client->changePokeCount != 0) {
        num = GetNumBattleReadyPartyMons(client, list);
        if (num != 0) {
            count = client->changePokeCount;
            defender = func_ov093_021cfd50(client, client->changePokePos[0]);
            if (defender != NULL) {
                PickBestMonToSwitchInto(client, list, num, defender);
            }
            if (count > num) {
                if (BtlSetup_GetBattleStyle(client->mainModule) == BTL_STYLE_ROTATION) {
                    size = func_ov093_021d1058(client, client->actions);
                    if (size != 0) {
                        client->returnDataSize = size;
                        client->returnData = client->actions;
                        return TRUE;
                    }
                }
                count = num;
            }
            for (i = 0; i < count; i++) {
                func_ov093_021b8e28(client->mainModule, client->changePokePos[i], &clientId, &idx);
                func_ov093_021d8394(&client->actions[i], idx, list[i]);
            }
            client->returnData = client->actions;
            client->returnDataSize = count * sizeof(BattleAction);
        } else {
            if (func_ov093_021d102c(client)) {
                size = func_ov093_021d1058(client, client->actions);
                if (size != 0) {
                    client->returnDataSize = size;
                    client->returnData = client->actions;
                    return TRUE;
                }
            }
            func_ov093_021d83c4(client->actions);
            client->returnData = client->actions;
            client->returnDataSize = sizeof(BattleAction);
        }
    } else {
        BattleAction_SetNull(client->actions);
        client->returnData = client->actions;
        client->returnDataSize = sizeof(BattleAction);
    }
    return TRUE;
}

BOOL func_ov093_021d0e74(BtlClient *client, s32 *seq) {
    return func_ov093_021ce1b0(client, seq);
}

// The player's choice of the mons to send out
// pokebw2's PlayerSelectChangePoke
BOOL func_ov093_021d0e7c(BtlClient *client, s32 *seq, u8 mode) {
    u8 list[6];
    u8 clientId;
    u8 idx;
    u8 num;
    u8 count;
    BattleMon *mon;
    u32 size;

    switch (*seq) {
    case 0:
        client->changePokeCount = func_ov093_021d0c78(client, client->changePokePos);
        if (client->changePokeCount != 0) {
            num = GetNumBattleReadyPartyMons(client, list);
            if (num != 0) {
                count = client->changePokeCount;
                if (count > num) {
                    count = num;
                }
                if (func_ov093_021d102c(client)) {
                    mon = GetBattleMonFromParty(client->party, 0);
                    if (num == 1 && IsFainted(mon)) {
                        func_ov093_021d8394(&client->actions[0], 0, list[0]);
                        client->returnData = client->actions;
                        client->returnDataSize = sizeof(BattleAction);
                        *seq = 4;
                        break;
                    }
                }
                func_ov093_021b8e28(client->mainModule, client->changePokePos[0], &clientId, &idx);
                func_ov093_021d0b10(client, mode, count, &client->pokeListCmd, &client->pokeSelect);
                BattleClientCmd_StartPokeList(client->viewCore, &client->pokeListCmd, idx, 0, &client->pokeSelect);
                func_ov093_021ce068(client);
                *seq = 1;
            } else {
                if (func_ov093_021d102c(client)) {
                    size = func_ov093_021d1058(client, client->actions);
                    if (size != 0) {
                        client->returnDataSize = size;
                        client->returnData = client->actions;
                        *seq = 4;
                        break;
                    }
                }
                func_ov093_021d83c4(client->actions);
                client->returnData = client->actions;
                client->returnDataSize = sizeof(BattleAction);
                *seq = 4;
            }
        } else {
            BattleAction_SetNull(client->actions);
            client->returnData = client->actions;
            client->returnDataSize = sizeof(BattleAction);
            *seq = 4;
        }
        break;
    case 1:
        if (CheckIfOverCmdLimit(client)) {
            BattleClientCmd_QuitPokeSelect(client->viewCore);
            *seq = 2;
        } else if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            func_ov093_021d0b6c(client, &client->pokeSelect);
            *seq = 3;
        }
        break;
    case 2:
        if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            func_ov093_021d0bb0(client);
            *seq = 3;
        }
        break;
    case 3:
        func_ov093_021ce0e0(client);
        *seq = 4;
        break;
    case 4:
        if (func_ov093_021b86c0(client->mainModule)) {
            client->waitMsgShown = TRUE;
            func_ov093_021ea7fc(client->viewCore);
            *seq = 5;
        } else {
            *seq = 6;
        }
        break;
    case 5:
        if (func_ov093_021ea814(client->viewCore)) {
            *seq = 6;
        }
        break;
    case 6:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IsRotationFrontFainted
BOOL func_ov093_021d102c(BtlClient *client) {
    if (BtlSetup_GetBattleStyle(client->mainModule) == BTL_STYLE_ROTATION &&
        IsFainted(GetBattleMonFromParty(client->party, 0))) {
        return TRUE;
    }
    return FALSE;
}

// Rotates to the first position whose mon can fight, returning the action's size, or 0 when none can
// pokebw2's RotateToLivingMon
u32 func_ov093_021d1058(BtlClient *client, BattleAction *action) {
    if (!IsFainted(GetPartyMonAt(client, func_ov093_021b9ab0(2)))) {
        func_ov093_021d83fc(action, 2);
        return sizeof(BattleAction);
    }
    if (!IsFainted(GetPartyMonAt(client, func_ov093_021b9ab0(3)))) {
        func_ov093_021d83fc(action, 3);
        return sizeof(BattleAction);
    }
    return 0;
}

BOOL func_ov093_021d10b8(BtlClient *client, s32 *seq) {
    const u8 *data;
    u32 result;
    u8 partyIdx;

    switch (*seq) {
    case 0:
        func_ov093_021eec20(client->adapter, (const void **)&data);
        if (*data != 0x1f) {
            Btlv_StringParam_Setup(&client->strParam, 1, 20);
            Btlv_StringParam_AddArg(&client->strParam, 1);
            Btlv_StringParam_AddArg(&client->strParam, *data);
            func_ov093_021eaf78(&client->strParam, 0xff);
            func_ov093_021ea614(client->viewCore, &client->strParam);
            *seq = 1;
        } else {
            *seq = 4;
        }
        break;
    case 1:
        func_ov093_021ea710(client->viewCore);
        if (func_ov093_021ea720(client->viewCore)) {
            Btlv_StringParam_Setup(&client->strParam, 3, 6);
            Btlv_StringParam_Setup(&client->strParam2, 3, 7);
            func_ov093_021eabc4(client->viewCore, &client->strParam, &client->strParam2, 1);
            *seq = 2;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore) && func_ov093_021eac54(client->viewCore, &result)) {
            if (result == 0) {
                func_ov093_021d0b10(client, 0, 1, &client->pokeListCmd, &client->pokeSelect);
                BattleClientCmd_StartPokeList(client->viewCore, &client->pokeListCmd, 0, 0, &client->pokeSelect);
                *seq = 3;
            } else {
                *seq = 4;
            }
        }
        break;
    case 3:
        if (BattleClientCmd_WaitPokeSelect(client->viewCore)) {
            if (!data_ov095_0689cb15(&client->pokeSelect) &&
                (partyIdx = data_ov095_0689cb1d(&client->pokeSelect)) != 0 && partyIdx < 6) {
                func_ov093_021d8394(client->actions, 0, partyIdx);
                *seq = 5;
            } else {
                *seq = 4;
            }
        }
        break;
    case 4:
        BattleAction_SetNull(client->actions);
        *seq = 5;
        // fallthrough
    case 5:
        client->returnData = client->actions;
        client->returnDataSize = sizeof(BattleAction);
        return TRUE;
    }
    return FALSE;
}







BOOL func_ov093_021d123c(BtlClient *client, s32 *seq) {
    const void *data;
    u16 size = func_ov093_021eec20(client->adapter, &data);

    if (client->recorder != NULL) {
        func_ov093_021ee608(client->recorder, data, size);
    }
    if (client->cmdCheckServer != NULL) {
        func_ov093_021bb4e0(client->cmdCheckServer, data, size);
        client->cmdCheckReq = func_ov093_021ee638(data);
    }
    return TRUE;
}

BOOL func_ov093_021d127c(BtlClient *client, s32 *seq) {
    if (client->viewCore != NULL) {
        return func_ov093_021d1290(client, seq);
    }
    return TRUE;
}

BOOL func_ov093_021d1290(BtlClient *client, s32 *seq) {
    u32 name;
    u32 result;
    u8 isMulti;
    u16 msg;

    switch (*seq) {
    case 0:
        if (client->unk1BA_5) {
            return TRUE;
        }
        name = func_ov093_021b904c(client->mainModule, 0);
        result = func_ov093_021d13a4(client);
        isMulti = func_ov093_021b86c8(client->mainModule);
        func_ov093_021b916c(client->mainModule, result);
        switch (result) {
        case 1:
            msg = isMulti ? 0x31 : 0x30;
            break;
        case 0:
            msg = isMulti ? 0x33 : 0x32;
            break;
        case 2:
            msg = isMulti ? 0x35 : 0x34;
            break;
        default:
            return TRUE;
        }
        Btlv_StringParam_Setup(&client->strParam, 1, msg);
        Btlv_StringParam_AddArg(&client->strParam, name);
        if (isMulti) {
            Btlv_StringParam_AddArg(&client->strParam, func_ov093_021b904c(client->mainModule, 1));
        }
        func_ov093_021ea614(client->viewCore, &client->strParam);
        if (result == 1) {
            GFL_SndBGMPlay(func_ov093_021b86dc(client->mainModule), 0xffff);
            (*seq)++;
        } else {
            GFL_SndBGMFadeOut(30);
            *seq += 2;
        }
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            (*seq)++;
        }
        break;
    case 3:
        if (!GFL_SndBGMIsFading()) {
            func_02005d28();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// The battle's result for this client's side: a result for the other side is flipped
// pokebw2's GetBattleResultForClient
u32 func_ov093_021d13a4(BtlClient *client) {
    const u16 *data;

    func_ov093_021eec20(client->adapter, (const void **)&data);
    switch (data[1]) {
    case 0:
        if (!IsAllyClientID(data[0], client->clientId)) {
            return 1;
        }
        break;
    case 1:
        if (!IsAllyClientID(data[0], client->clientId)) {
            return 0;
        }
        break;
    }
    return data[1];
}

BOOL func_ov093_021d13fc(BtlClient *client, s32 *seq) {
    if (client->viewCore != NULL) {
        return func_ov093_021d1410(client, seq);
    }
    return TRUE;
}

BOOL func_ov093_021d1410(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        if (func_ov093_021d13a4(client) == 1) {
            u8 player;
            u8 opponent;

            func_ov094_021f878c(func_ov093_021b86dc(client->mainModule));
            player = func_ov093_021b904c(client->mainModule, 0);
            opponent = func_ov093_021b904c(client->mainModule, 1);
            if (player == opponent) {
                Btlv_StringParam_Setup(&client->strParam, 1, 0x2c);
                Btlv_StringParam_AddArg(&client->strParam, player);
            } else {
                Btlv_StringParam_Setup(&client->strParam, 1, 0x2d);
                Btlv_StringParam_AddArg(&client->strParam, player);
                Btlv_StringParam_AddArg(&client->strParam, opponent);
            }
            func_ov093_021ea614(client->viewCore, &client->strParam);
            *seq = 1;
        } else if (!BtlSetup_IsBattleType(client->mainModule, 0x100)) {
            u8 playerId = GetPlayerClientID(client->mainModule);

            Btlv_StringParam_Setup(&client->strParam, 1, 0x36);
            Btlv_StringParam_AddArg(&client->strParam, playerId);
            Btlv_StringParam_AddArg(&client->strParam, playerId);
            func_ov093_021ea614(client->viewCore, &client->strParam);
            *seq = 9;
        } else {
            return TRUE;
        }
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov094_021f7f58(func_ov093_021ba020(client->mainModule, func_ov093_021b904c(client->mainModule, 0)), 9, 0, 0, 0);
            func_ov094_021f7998(0x270);
            *seq = 2;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ea6b0(client->viewCore, func_ov093_021ba004(client->mainModule, func_ov093_021b904c(client->mainModule, 0)), 1);
            *seq = 3;
        }
        break;
    case 3:
        if (func_ov093_021ea710(client->viewCore)) {
            if (func_ov093_021b904c(client->mainModule, 0) != func_ov093_021b904c(client->mainModule, 1)) {
                func_ov094_021f7998(0x271);
                *seq = 4;
            } else {
                *seq = 7;
            }
        }
        break;
    case 4:
        if (!func_ov094_021f7eb4()) {
            func_ov094_021f7f58(func_ov093_021ba020(client->mainModule, func_ov093_021b904c(client->mainModule, 1)), 9, 0, 0, 0);
            func_ov094_021f7998(0x270);
            *seq = 5;
        }
        break;
    case 5:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ea6b0(client->viewCore, func_ov093_021ba004(client->mainModule, func_ov093_021b904c(client->mainModule, 1)), 1);
            *seq = 6;
        }
        break;
    case 6:
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 7;
        }
        break;
    case 7: {
        u32 value = func_ov093_021b919c(client->mainModule);

        if (value != 0) {
            u8 playerId = GetPlayerClientID(client->mainModule);

            Btlv_StringParam_Setup(&client->strParam, 1, 0x3a);
            Btlv_StringParam_AddArg(&client->strParam, playerId);
            Btlv_StringParam_AddArg(&client->strParam, value);
            func_ov093_021ea614(client->viewCore, &client->strParam);
        }
        if (func_ov093_021b9210(client->mainModule) == 0) {
            *seq = 11;
        } else {
            *seq = 8;
        }
        break;
    }
    case 8:
        if (func_ov093_021ea710(client->viewCore)) {
            u32 value = func_ov093_021b9210(client->mainModule);

            if (value != 0) {
                u8 playerId = GetPlayerClientID(client->mainModule);

                Btlv_StringParam_Setup(&client->strParam, 1, 0x3b);
                Btlv_StringParam_AddArg(&client->strParam, playerId);
                Btlv_StringParam_AddArg(&client->strParam, value);
                func_ov093_021ea614(client->viewCore, &client->strParam);
            }
            *seq = 11;
        }
        break;
    case 9:
        if (func_ov093_021ea710(client->viewCore)) {
            u32 value = func_ov093_021b9244(client->mainModule);

            if (value != 0) {
                Btlv_StringParam_Setup(&client->strParam, 1, 0x38);
                Btlv_StringParam_AddArg(&client->strParam, client->clientId);
                Btlv_StringParam_AddArg(&client->strParam, value);
                func_ov093_021ea614(client->viewCore, &client->strParam);
            }
            *seq = 10;
        }
        break;
    case 10:
        if (func_ov093_021ea710(client->viewCore)) {
            Btlv_StringParam_Setup(&client->strParam, 1, 0x39);
            Btlv_StringParam_AddArg(&client->strParam, client->clientId);
            func_ov093_021ea614(client->viewCore, &client->strParam);
            *seq = 11;
        }
        break;
    case 11:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov093_021d1708(BtlClient *client, s32 *seq) {
    if (client->viewCore != NULL) {
        return func_ov093_021d171c(client, seq);
    }
    return TRUE;
}

BOOL func_ov093_021d171c(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0: {
        u32 count;

        if (client->unk1BA_5) {
            return TRUE;
        }
        count = func_ov093_021d13a4(client);
        func_ov093_021b916c(client->mainModule, count);
        if (count == 1) {
            func_ov094_021f878c(func_ov093_021b86dc(client->mainModule));
        }
        if (count <= 1) {
            func_ov094_021f7f58(func_ov093_021ba020(client->mainModule, func_ov093_021b904c(client->mainModule, 0)), 9, 0, 0, 0);
            func_ov094_021f7998(0x270);
            func_ov093_021d185c(client, count, 0);
            *seq = 1;
            break;
        }
        return TRUE;
    }
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ea600(client->viewCore);
            *seq = 2;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            if (func_ov093_021b904c(client->mainModule, 0) == func_ov093_021b904c(client->mainModule, 1)) {
                return TRUE;
            }
            func_ov094_021f7998(0x271);
            *seq = 3;
        }
        break;
    case 3:
        if (!func_ov094_021f7eb4()) {
            u32 count = func_ov093_021d13a4(client);

            func_ov094_021f7f58(func_ov093_021ba020(client->mainModule, func_ov093_021b904c(client->mainModule, 1)), 9, 0, 0, 0);
            func_ov094_021f7998(0x270);
            func_ov093_021d185c(client, count, 1);
            *seq = 4;
        }
        break;
    case 4:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ea600(client->viewCore);
            *seq = 5;
        }
        break;
    case 5:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// Shows a trainer's win or lose message: their easy chat sentence if they have one, else their trainer data's message
// pokebw2's ShowTrainerWinLoseMsg
void func_ov093_021d185c(BtlClient *client, u32 result, u8 which) {
    u8 clientId = func_ov093_021b904c(client->mainModule, which);
    const PMSData *sentence = func_ov093_021ba02c(client->mainModule, clientId, result);
    StrBuf *str;
    u16 trainerId;

    if (sentence != NULL && func_02024450(sentence)) {
        str = func_0202435c(sentence, HEAPID_TAIL(client->heapId));
        func_ov093_021ea5e4(client->viewCore, str);
        GFL_StrBufFree(str);
    } else {
        trainerId = func_ov093_021ba004(client->mainModule, clientId);
        if (trainerId != 0) {
            Btlv_StringParam_Setup(&client->strParam, 7, 0);
            Btlv_StringParam_AddArg(&client->strParam, trainerId);
            Btlv_StringParam_AddArg(&client->strParam, result);
            func_ov093_021ea5f0(client->viewCore, &client->strParam);
        }
    }
}

BOOL func_ov093_021d18ec(BtlClient *client, s32 *seq) {
    u32 value;
    u8 playerId;

    switch (*seq) {
    case 0:
        if (!BtlSetup_IsBattleType(client->mainModule, 0x200)) {
            value = func_ov093_021b9210(client->mainModule);
            if (value != 0) {
                playerId = GetPlayerClientID(client->mainModule);
                Btlv_StringParam_Setup(&client->strParam, 1, 0x3b);
                Btlv_StringParam_AddArg(&client->strParam, playerId);
                Btlv_StringParam_AddArg(&client->strParam, value);
                func_ov093_021ea614(client->viewCore, &client->strParam);
            }
        }
        (*seq)++;
        // fallthrough
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d1960(BtlClient *client, s32 *seq) {
    u32 value;

    switch (*seq) {
    case 0:
        Btlv_StringParam_Setup(&client->strParam, 1, 0x36);
        Btlv_StringParam_AddArg(&client->strParam, client->clientId);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            value = func_ov093_021b9244(client->mainModule);
            if (value != 0) {
                Btlv_StringParam_Setup(&client->strParam, 1, 0x37);
                Btlv_StringParam_AddArg(&client->strParam, client->clientId);
                Btlv_StringParam_AddArg(&client->strParam, value);
                func_ov093_021ea614(client->viewCore, &client->strParam);
            }
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            Btlv_StringParam_Setup(&client->strParam, 1, 0x39);
            Btlv_StringParam_AddArg(&client->strParam, client->clientId);
            func_ov093_021ea614(client->viewCore, &client->strParam);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ea710(client->viewCore)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

typedef BOOL (*BtlClientServerCmdProc)(BtlClient *client, s32 *seq, const u32 *args);

// A server command and the handler that plays it
typedef struct {
    u32 cmd;
    BtlClientServerCmdProc proc;
} BtlClientServerCmdEntry;

// This part's functions

// The handlers of the server's commands
static const BtlClientServerCmdEntry sServerCmdTable[] = {
    { 0x5a, func_ov093_021d1e6c },       { 0x5c, func_ov093_021d1eb4 },   { 0x5b, func_ov093_021d1f10 },
    { 0x5d, func_ov093_021d1f58 },       { 0x59, func_ov093_021d1fb4 },   { 0x30, func_ov093_021d2030 },
    { 0x31, func_ov093_021d20bc },       { 0x32, func_ov093_021d210c },   { 0x33, func_ov093_021d215c },
    { 0x34, func_ov093_021d21d4 },       { 0x35, func_ov093_021d2234 },   { 0x36, func_ov093_021d2278 },
    { 0x39, func_ov093_021d2320 },       { 0x3a, func_ov093_021d2374 },   { 0x3b, func_ov093_021d1bd0 },
    { 0x3c, func_ov093_021d1cac },       { 0x3d, func_ov093_021d1cf4 },   { 0x37, func_ov093_021d2424 },
    { 0x38, func_ov093_021d23b8 },       { 0x3e, func_ov093_021d29cc }, { 0x3f, func_ov093_021d2490 },
    { 0x40, func_ov093_021d2510 }, { 0x41, func_ov093_021d2580 },   { 0x42, func_ov093_021d25c4 },
    { 0x57, func_ov093_021d3630 },       { 0x58, func_ov093_021d3688 },   { 0x01, func_ov093_021d36cc },
    { 0x02, func_ov093_021d36ec },       { 0x03, func_ov093_021d377c },   { 0x04, func_ov093_021d370c },
    { 0x05, func_ov093_021d3734 },       { 0x06, func_ov093_021d375c },   { 0x07, func_ov093_021d3794 },
    { 0x08, func_ov093_021d37b8 },       { 0x09, func_ov093_021d37dc },   { 0x0a, func_ov093_021d37fc },
    { 0x0b, func_ov093_021d381c },       { 0x0c, func_ov093_021d3894 },   { 0x0d, func_ov093_021d38ac },
    { 0x0e, func_ov093_021d38c4 },       { 0x0f, func_ov093_021d38e0 },   { 0x10, func_ov093_021d3900 },
    { 0x11, func_ov093_021d3918 },       { 0x12, func_ov093_021d3934 },   { 0x13, func_ov093_021d3d2c },
    { 0x14, func_ov093_021d3d50 },       { 0x16, func_ov093_021d3988 },   { 0x15, func_ov093_021d39a8 },
    { 0x17, func_ov093_021d3a20 },       { 0x18, func_ov093_021d3a40 },   { 0x19, func_ov093_021d3a7c },
    { 0x1a, func_ov093_021d3a98 },       { 0x1b, func_ov093_021d3ab4 },   { 0x1c, func_ov093_021d3ad0 },
    { 0x1d, func_ov093_021d3aec },       { 0x1e, func_ov093_021d3b08 },   { 0x1f, func_ov093_021d3b28 },
    { 0x20, func_ov093_021d3b5c },       { 0x21, func_ov093_021d3b74 },   { 0x22, func_ov093_021d3b88 },
    { 0x23, func_ov093_021d3b9c },       { 0x24, func_ov093_021d3bb0 },   { 0x25, func_ov093_021d3bc0 },
    { 0x26, func_ov093_021d3be0 },       { 0x27, func_ov093_021d3c0c },   { 0x28, func_ov093_021d3c2c },
    { 0x29, func_ov093_021d3c44 },       { 0x2a, func_ov093_021d3c70 },   { 0x2b, func_ov093_021d3c90 },
    { 0x2c, func_ov093_021d3ca8 },       { 0x2d, func_ov093_021d3cc8 },   { 0x2e, func_ov093_021d3cfc },
    { 0x2f, func_ov093_021d3d1c },       { 0x43, func_ov093_021d2628 },   { 0x44, func_ov093_021d266c },
    { 0x45, func_ov093_021d2a20 },    { 0x46, func_ov093_021d2fd8 },   { 0x47, func_ov093_021d3138 },
    { 0x48, func_ov093_021d31a0 },       { 0x49, func_ov093_021d324c },   { 0x4a, func_ov093_021d32e0 },
    { 0x4b, func_ov093_021d344c },       { 0x4c, func_ov093_021d34b0 },   { 0x4d, func_ov093_021d34e8 },
    { 0x4e, func_ov093_021d3568 },       { 0x4f, func_ov093_021d35c8 },   { 0x50, func_ov093_021d2700 },
    { 0x51, func_ov093_021d2814 },       { 0x52, func_ov093_021d284c },   { 0x53, func_ov093_021d28ac },
    { 0x54, func_ov093_021d2930 },       { 0x55, func_ov093_021d2990 },   { 0x56, func_ov093_021d299c },
};

// The message and sound effect of each weather's start
static const struct {
    u16 msg;
    u16 se;
} sWeatherStartTable[] = {
    { 0x54, 0x26b }, { 0x54, 0x26b }, { 0x55, 0x268 }, { 0x57, 0x269 }, { 0x56, 0x26a },
};

// The effort values that the effort value command raises, in its arguments' order
BOOL func_ov093_021d1a18(BtlClient *client, s32 *seq) {
    switch (*seq) {
    case 0:
        Btlv_StringParam_Setup(&client->strParam, 1, 0xb7);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            (*seq)++;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d1a60(BtlClient *client, s32 *seq) {
    if (client->viewCore != NULL) {
        return func_ov093_021d1a74(client, seq);
    }
    return TRUE;
}

// pokebw2's BattleClient_ServerCmdLoop
BOOL func_ov093_021d1a74(BtlClient *client, s32 *seq) {
    while (TRUE) {
        switch (*seq) {
        case 0: {
            const void *data;
            u32 size;

            size = func_ov093_021eec20(client->adapter, &data);
            if (client->cmdCheckServer != NULL && client->cmdCheckReq && client->cmdCheckEnable) {
                if (func_ov093_021bb570(client->cmdCheckServer, client->cmdCheckReq, data, size)) {
                    func_ov093_021b9184(client->mainModule);
                    client->cmdCheckEnable = FALSE;
                }
                client->cmdCheckReq = 0;
            }
            BtlServerCmdQueue_Setup(client->cmdQueue, data, size);
            if (client->waitMsgShown) {
                client->waitMsgShown = FALSE;
                func_ov093_021ea824(client->viewCore);
            }
            (*seq)++;
        }
        case 1:
            if (BtlServerCmdQueue_IsEmpty(client->cmdQueue)) {
                *seq = 4;
                return TRUE;
            }
            if (func_ov093_021d3f4c(&client->recPlayer)) {
                return TRUE;
            }
            (*seq)++;
        case 2: {
            u32 i;

            client->serverCmd = func_ov093_021cd5e0(client->cmdQueue, client->cmdArgs);
            if (client->serverCmd == 0x5e) {
                return TRUE;
            }
            for (i = 0; i < NELEMS(sServerCmdTable); i++) {
                if (client->serverCmd == sServerCmdTable[i].cmd) {
                    break;
                }
            }
            if (i == NELEMS(sServerCmdTable)) {
                *seq = 1;
                continue;
            }
            client->serverCmdProc = sServerCmdTable[i].proc;
            client->serverCmdSeq = 0;
            (*seq)++;
        }
        case 3:
            if (client->serverCmdProc(client, &client->serverCmdSeq, client->cmdArgs)) {
                *seq = 1;
                continue;
            }
            break;
        case 4:
            return TRUE;
        }
        return FALSE;
    }
}

BOOL func_ov093_021d1bd0(BtlClient *client, s32 *seq, const u32 *args) {
    BOOL withName;

    switch (*seq) {
    case 0:
        Btlv_StringParam_Setup(&client->strParam, 1, func_ov093_021d1c40(client, args[0], &withName));
        if (withName) {
            Btlv_StringParam_AddArg(&client->strParam, args[0]);
        }
        Btlv_StringParam_AddArg(&client->strParam, args[1]);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_GetRecallMsg
u16 func_ov093_021d1c40(BtlClient *client, u8 clientId, BOOL *withName) {
    if (clientId == client->clientId) {
        *withName = FALSE;
        if (BtlSetup_GetBattleStyle(client->mainModule) == 0 && !func_ov093_021b86c0(client->mainModule)) {
            u32 ratio = func_ov093_021cdee8(client);

            if (ratio >= 75) {
                return 0x1d;
            }
            if (ratio > 50) {
                return 0x1c;
            }
            if (ratio > 25) {
                return 0x1b;
            }
            if (ratio != 0) {
                return 0x1a;
            }
            return 0x19;
        }
        return 0x1a;
    } else {
        *withName = TRUE;
        if (func_ov093_021b9f70(client->mainModule, clientId)) {
            return 0x1e;
        }
        return 0x1f;
    }
}

BOOL func_ov093_021d1cac(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea458(client->viewCore, func_ov093_021b8e70(client->mainModule, args[0]), args[1]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea478(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d1cf4(BtlClient *client, s32 *seq, const u32 *args) {
    u8 clientId = args[0];
    u8 pos = args[1];
    u8 show = args[3];

    switch (*seq) {
    case 0:
        if (show) {
            u8 monId = GetMonID(GetClientMonData(client->pokeCon, clientId, pos));

            if (!func_ov093_021b8ce8(client->mainModule, client->clientId, clientId)) {
                if (client->clientId == clientId) {
                    Btlv_StringParam_Setup(&client->strParam, 1, func_ov093_021d1df4(client));
                    Btlv_StringParam_AddArg(&client->strParam, monId);
                } else {
                    Btlv_StringParam_Setup(&client->strParam, 1, 0x11);
                    Btlv_StringParam_AddArg(&client->strParam, clientId);
                    Btlv_StringParam_AddArg(&client->strParam, monId);
                }
            } else if (func_ov093_021b9f70(client->mainModule, clientId)) {
                Btlv_StringParam_Setup(&client->strParam, 1, 0xe);
                Btlv_StringParam_AddArg(&client->strParam, clientId);
                Btlv_StringParam_AddArg(&client->strParam, monId);
            } else {
                Btlv_StringParam_Setup(&client->strParam, 1, 0x11);
                Btlv_StringParam_AddArg(&client->strParam, clientId);
                Btlv_StringParam_AddArg(&client->strParam, monId);
            }
            func_ov093_021ea614(client->viewCore, &client->strParam);
        }
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021ea488(client->viewCore, func_ov093_021b8bec(client->mainModule, clientId, pos), clientId, pos);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea4c4(client->viewCore)) {
            func_ov093_021cdecc(client);
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// pokebw2's BattleClient_GetPlayerSendOutMsg
u16 func_ov093_021d1df4(BtlClient *client) {
    if (BtlSetup_GetBattleStyle(client->mainModule) == 0) {
        u8 clientId = func_ov093_021b9064(client->mainModule, client->clientId, 0);
        BattleMon *mon = GetClientMonData(client->pokeCon, clientId, 0);
        fx32 ratio;

        if (IsFainted(mon)) {
            return 0xb;
        }
        ratio = GetHPRatio(mon);
        if (ratio >= FX32_CONST(77.5)) {
            return 0xb;
        }
        if (ratio >= FX32_CONST(55)) {
            return 0x15;
        }
        if (ratio >= FX32_CONST(32.5)) {
            return 0x16;
        }
        if (ratio >= FX32_CONST(10)) {
            return 0x17;
        }
        return 0x18;
    }
    return 0xb;
}

BOOL func_ov093_021d1e6c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov093_021ea678(client->viewCore, args[0], &args[1]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d1eb4(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov093_021ea678(client->viewCore, args[0], &args[2]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea720(client->viewCore)) {
            GFL_SndSEPlay(args[1]);
        }
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d1f10(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov093_021ea694(client->viewCore, args[0], &args[1]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d1f58(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov093_021ea694(client->viewCore, args[0], &args[2]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea720(client->viewCore)) {
            GFL_SndSEPlay(args[1]);
        }
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d1fb4(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov093_021ea6f4(client->viewCore, args[0], args[1]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_GetMoveAnimType
s32 func_ov093_021d2000(BtlClient *client, u16 move, u8 pos) {
    if (move != 0xae) {
        return PML_MoveGetParam(move, 0x1b);
    } else {
        BattleMon *mon = func_ov093_021b98ac(client->pokeCon, pos);

        if (mon != NULL) {
            return func_ov093_021d80a8(mon);
        }
        return PML_MoveGetParam(move, 0x1b);
    }
}

BOOL func_ov093_021d2030(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        if (func_ov093_021b8590(client->mainModule)) {
            u16 move;
            u8 attackerPos;
            u8 targetPos;
            u8 arg3;
            s32 arg4;

            attackerPos = args[0];
            targetPos = args[1];
            move = args[2];
            arg3 = args[3];
            arg4 = func_ov093_021d2000(client, move, attackerPos);
            func_ov093_021ea0a0(client->viewCore, attackerPos, targetPos, move, arg4, arg3, 0);
            (*seq)++;
            break;
        }
        return TRUE;
    case 1:
        if (func_ov093_021ea0e4(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d20bc(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u8 pos = func_ov093_021b8dc0(client->mainModule, client->pokeCon, args[0]);

        func_ov093_021ea354(client->viewCore, pos, args[1] ? TRUE : FALSE);
        (*seq)++;
        break;
    }
    case 1:
        return !func_ov094_021f7eb4();
    }
    return FALSE;
}

BOOL func_ov093_021d210c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea0f4(client->viewCore, args[2], MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]),
                            args[1]);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea120(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d215c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u16 count = args[0];
        u32 type = args[1];
        u16 arg2 = args[2];
        u8 monIds[6];
        u8 i;

        for (i = 0; i < count; i++) {
            monIds[i] = func_ov093_021cd644(client->cmdQueue);
        }
        func_ov093_021ea280(client->viewCore, count, type, monIds, arg2);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021ea2bc(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d21d4(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea384(client->viewCore, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea3a0(client->viewCore)) {
            func_ov093_021ea678(client->viewCore, 0x61, args);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2234(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov094_021f81d0(
            args[1],
            func_ov093_021b8e70(client->mainModule, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0])));
        (*seq)++;
        break;
    case 1:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d2278(BtlClient *client, s32 *seq, const u32 *args) {
    u8 pos;

    switch (*seq) {
    case 0:
        if (!func_ov093_021cd9e0(client)) {
            pos = MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]);
            func_ov093_021ea0a0(client->viewCore, pos, pos, 1, PML_MoveGetParam(1, 0x1b), 0, 0);
        }
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea0e4(client->viewCore)) {
            pos = MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]);
            if (!func_ov093_021cd9e0(client)) {
                GFL_SndSEPlay(0x564);
            }
            func_ov093_021ea0f4(client->viewCore, 1, pos, 1);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea120(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2320(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea40c(client->viewCore, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea428(client->viewCore)) {
            Clear_ForFainted(GetPokeParam(client->pokeCon, args[0]));
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL func_ov093_021d2374(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea438(client->viewCore, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea448(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d23b8(BtlClient *client, s32 *seq, const u32 *args) {
    u8 viewPos =
        func_ov093_021b8e70(client->mainModule, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));

    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        if (!func_ov093_021b8590(client->mainModule)) {
            return TRUE;
        }
        func_ov093_021ea7c8(client->viewCore, viewPos);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea7e8(client->viewCore, viewPos)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2424(BtlClient *client, s32 *seq, const u32 *args) {
    u8 viewPos =
        func_ov093_021b8e70(client->mainModule, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));

    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        if (!func_ov093_021b8590(client->mainModule)) {
            return TRUE;
        }
        func_ov093_021ea7d8(client->viewCore, viewPos);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea7e8(client->viewCore, viewPos)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_ScWeatherStart
BOOL func_ov093_021d2490(BtlClient *client, s32 *seq, const u32 *args) {
    u8 weather = args[0];

    switch (*seq) {
    case 0:
        func_ov093_021ef9c0(client->field, weather, args[1]);
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        if (weather < NELEMS(sWeatherStartTable)) {
            func_ov094_021f7998(sWeatherStartTable[weather].se);
        }
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ea678(client->viewCore, sWeatherStartTable[weather].msg, NULL);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_ScWeatherEnd
BOOL func_ov093_021d2510(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u16 msg;

        switch ((u8)args[0]) {
        case 1:
            msg = 0x59;
            break;
        case 2:
            msg = 0x5a;
            break;
        case 4:
            msg = 0x5b;
            break;
        case 3:
            msg = 0x5c;
            break;
        default:
            return TRUE;
        }
        func_ov093_021ea678(client->viewCore, msg, NULL);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021ea710(client->viewCore)) {
            func_ov093_021ef9c8(client->field);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2580(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea384(client->viewCore, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea3a0(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d25c4(BtlClient *client, s32 *seq, const u32 *args) {
    u8 pos = MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]);

    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        if (!func_ov093_021b8590(client->mainModule)) {
            return TRUE;
        }
        func_ov093_021ea838(client->viewCore, pos);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea850(client->viewCore, pos)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2628(BtlClient *client, s32 *seq, const u32 *args) {
    u8 monId = args[0];

    switch (*seq) {
    case 0:
        func_ov093_021ea384(client->viewCore, MonIDToBattlePos(client->mainModule, client->pokeCon, monId));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea3a0(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d266c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u8 clientId = args[0];
        u8 pos1 = args[1];
        u8 pos2 = args[2];
        u8 slot1 = func_ov093_021b8dec(client->mainModule, pos1);
        u8 slot2 = func_ov093_021b8dec(client->mainModule, pos2);
        u8 viewPos1 = func_ov093_021b8e70(client->mainModule, pos1);
        u8 viewPos2 = func_ov093_021b8e70(client->mainModule, pos2);

        func_ov093_021b9c10(GetPartyData(client->pokeCon, clientId), slot1, slot2);
        func_ov093_021ea904(client->viewCore, clientId, viewPos1, viewPos2, slot1, slot2);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021ea950(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2700(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov094_021f7998(0x281);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            u8 clientId = args[0];
            u8 slot = args[1];
            u8 pos1 = func_ov093_021b8bec(client->mainModule, clientId, slot);
            u8 pos2 = func_ov093_021b8bec(client->mainModule, clientId, 1);
            u8 viewPos1 = func_ov093_021b8e70(client->mainModule, pos1);
            u8 viewPos2 = func_ov093_021b8e70(client->mainModule, pos2);

            func_ov093_021b9c10(GetPartyData(client->pokeCon, clientId), slot, 1);
            func_ov093_021ea904(client->viewCore, clientId, viewPos1, viewPos2, slot, 1);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea950(client->viewCore)) {
            u8 clientId = args[2];
            u8 slot = args[3];
            u8 pos1 = func_ov093_021b8bec(client->mainModule, clientId, slot);
            u8 pos2 = func_ov093_021b8bec(client->mainModule, clientId, 1);
            u8 viewPos1 = func_ov093_021b8e70(client->mainModule, pos1);
            u8 viewPos2 = func_ov093_021b8e70(client->mainModule, pos2);

            func_ov093_021b9c10(GetPartyData(client->pokeCon, clientId), slot, 1);
            func_ov093_021ea904(client->viewCore, clientId, viewPos1, viewPos2, slot, 1);
            (*seq)++;
        }
        break;
    case 3:
        if (func_ov093_021ea950(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2814(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov094_021f7e94(func_ov093_021b8e70(client->mainModule, args[0]));
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d284c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u8 viewPos = func_ov093_021b8e70(client->mainModule, args[0]);

        if (func_ov093_021cd9e0(client)) {
            func_ov094_021f88bc(TRUE);
        }
        func_ov094_021f7ea4(viewPos);
        (*seq)++;
        break;
    }
    case 1:
        if (!func_ov094_021f7eb4()) {
            if (func_ov093_021cd9e0(client)) {
                func_ov094_021f88bc(FALSE);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_ScTransform
BOOL func_ov093_021d28ac(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);
        BattleMon *target = GetPokeParam(client->pokeCon, args[1]);
        u8 pos = func_ov093_021b8dc0(client->mainModule, client->pokeCon, args[0]);
        u8 targetPos = func_ov093_021b8dc0(client->mainModule, client->pokeCon, args[1]);

        TransformSet(mon, target);
        func_ov093_021ea8b4(client->viewCore, pos, targetPos);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021ea8d4(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2930(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        if (!func_ov093_021cd9e0(client)) {
            u8 pos = MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]);

            func_ov093_021ea1b8(client->viewCore, args[2], pos, func_ov093_021d7ad4(args[1]));
            (*seq)++;
            break;
        }
        return TRUE;
    case 1:
        if (func_ov093_021ea1f0(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2990(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov094_021f878c(args[0]);
    return TRUE;
}

BOOL func_ov093_021d299c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov093_021ea8e4(client->viewCore);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea8f4(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_ScAddEVs
BOOL func_ov093_021d29cc(BtlClient *client, s32 *seq, const u32 *args) {
    static const u16 sEVParams[] = {
        PKM_PARAM_EV_HP,     PKM_PARAM_EV_HP + 1, PKM_PARAM_EV_HP + 2,
        PKM_PARAM_EV_HP + 3, PKM_PARAM_EV_HP + 4, PKM_PARAM_EV_HP + 5,
    };
    PartyPkm *pkm = GetSrcData(GetPokeParam(client->pokeCon, args[0]));
    u32 i;

    for (i = 0; i < NELEMS(sEVParams); i++) {
        // BUG: tests the next entry rather than this one; the table has 6 (ROM 0x021eff88, 0xc bytes), so the last
        // pass reads the first u16 of the table after it (0x011e) and is never skipped
        if (sEVParams[i + 1] != 0) {
            u16 param = sEVParams[i];
            u32 value = args[1 + i] + PokeParty_GetParam(pkm, param, NULL);

            if (value > 255) {
                value = 255;
            }
            PokeParty_SetParam(pkm, param, value);
        }
    }
    return TRUE;
}

BOOL func_ov093_021d2a20(BtlClient *client, s32 *seq, const u32 *args) {
    u8 monId = args[0];
    BattleMon *mon = GetPokeParam(client->pokeCon, monId);
    u32 viewPos = func_ov093_021b8dc0(client->mainModule, client->pokeCon, monId);
    BOOL showGauge = FALSE;

    if (viewPos != 0xff && func_ov094_021f8204(viewPos)) {
        showGauge = TRUE;
    }

    switch (*seq) {
    case 0:
        sExpLeft = args[1];
        sExpSeq = 0;
        *seq = 1;
        // fall through
    case 1:
        if (sExpLeft != 0) {
            u32 exp = sExpLeft;

            if (func_ov093_021d69b8(mon, &exp, &sLevelUp)) {
                *seq = 3;
            } else {
                if (showGauge) {
                    func_ov094_021f8134(viewPos, sExpLeft);
                    *seq = 2;
                } else {
                    *seq = 13;
                }
                exp = 0;
            }
            sExpLeft = exp;
        } else {
            *seq = 13;
        }
        break;
    case 2:
        if (!func_ov094_021f8180()) {
            *seq = 13;
        }
        break;
    case 3:
        func_ov093_021d6ea0(&mon->core.src);
        func_ov093_021d6bfc(mon);
        if (showGauge) {
            func_ov094_021f8150(viewPos, mon);
            *seq = 4;
        } else {
            *seq = 6;
        }
        break;
    case 4:
        if (!func_ov094_021f8180() && !GFL_SndPlayerIsActiveAny()) {
            func_ov093_021ea3b8(client->viewCore, viewPos, 0x25d);
            *seq = 5;
        }
        break;
    case 5:
        if (func_ov093_021ea3d4(client->viewCore)) {
            *seq = 6;
        }
        break;
    case 6:
        Btlv_StringParam_Setup(&client->strParam, 1, 0x3c);
        Btlv_StringParam_AddArg(&client->strParam, monId);
        Btlv_StringParam_AddArg(&client->strParam, sLevelUp.level);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        *seq = 7;
        break;
    case 7:
        if (func_ov093_021ea720(client->viewCore)) {
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            GFL_SndBGMPlay(0x515, 0xffff);
        }
        if (func_ov093_021ea710(client->viewCore) && !GFL_SndBGMIsPlaying()) {
            GFL_SndBGMPop();
            GFL_SndBGMSetPaused(FALSE);
            sExpSeq = 0;
            *seq = 8;
        }
        break;
    case 8:
        sExpSeq++;
        if ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60() || sExpSeq > 0x50) {
            sExpSeq = 0;
            func_ov093_021ba238(client->mainModule, mon);
            func_ov093_021eada4(client->viewCore, mon, &sLevelUp);
            *seq = 9;
        }
        break;
    case 9:
        if (func_ov093_021eadb4(client->viewCore) && ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60())) {
            func_ov093_021eadc4(client->viewCore);
            *seq = 10;
        }
        break;
    case 10:
        if (func_ov093_021eadd4(client->viewCore) && ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60())) {
            func_ov093_021eade4(client->viewCore);
            *seq = 11;
        }
        break;
    case 11:
        if (func_ov093_021eadf4(client->viewCore)) {
            sExpSeq = 0;
            *seq = 12;
        }
        break;
    case 12:
        if (func_ov093_021d2ca8(client, &sExpSeq, mon)) {
            *seq = 1;
        }
        break;
    case 13:
        func_ov093_021b9358(client->mainModule, monId);
        return TRUE;
    }
    return FALSE;
}


// pokebw2's BattleClient_ForgetMoveSECallback
BOOL func_ov093_021d2c7c(u32 arg0) {
    switch (arg0) {
    case 3:
        GFL_SndSEPlay(0x56b);
        break;
    case 5:
        if (GFL_SndIsPlaying(0x56b)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's BattleClient_LearnMoveSeq
BOOL func_ov093_021d2ca8(BtlClient *client, s32 *seq, BattleMon *mon) {
    u8 monId = GetMonID(mon);

    switch (*seq) {
    case 0:
        sLearnIdx = 0;
        sLearnMove = 0;
        *seq = 1;
    case 1:
        sLearnMove = func_02018328(GetSrcData(mon), &sLearnIdx, client->heapId);
        if (sLearnMove == 0) {
            return TRUE;
        }
        if (sLearnMove == 0xfffe) {
            break;
        }
        if (sLearnMove & 0x8000) {
            sLearnMove &= 0x7fff;
            *seq = 5;
        } else {
            *seq = 2;
        }
        break;
    case 2: {
        BattleMon *bpp = GetPokeParam(client->pokeCon, monId);

        GetSrcData(bpp);
        func_ov093_021d6bfc(bpp);
        Btlv_StringParam_Setup(&client->strParam, 5, 3);
        Btlv_StringParam_AddArg(&client->strParam, monId);
        Btlv_StringParam_AddArg(&client->strParam, sLearnMove);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        *seq = 3;
    }
    case 3:
        if (func_ov093_021ea720(client->viewCore)) {
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            GFL_SndBGMPlay(0x515, 0xffff);
        }
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 4;
        }
        break;
    case 4:
        if (!GFL_SndBGMIsPlaying()) {
            GFL_SndBGMPop();
            GFL_SndBGMSetPaused(FALSE);
            *seq = 1;
        }
        break;
    case 5:
        Btlv_StringParam_Setup(&client->strParam, 5, 4);
        Btlv_StringParam_AddArg(&client->strParam, monId);
        Btlv_StringParam_AddArg(&client->strParam, sLearnMove);
        func_ov093_021eaf78(&client->strParam, 0xff);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        *seq = 6;
    case 6:
        if (func_ov093_021ea720(client->viewCore)) {
            BtlvStringParam yesParam;
            BtlvStringParam noParam;

            Btlv_StringParam_Setup(&yesParam, 6, 2);
            Btlv_StringParam_Setup(&noParam, 6, 3);
            func_ov093_021eabc4(client->viewCore, &yesParam, &noParam, 1);
        }
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 7;
        }
        break;
    case 7: {
        u32 answer;

        if (func_ov093_021eac54(client->viewCore, &answer)) {
            if (answer == 0) {
                func_ov093_021eac64(client->viewCore,
                                    func_ov093_021b9cbc(func_ov093_021b9af8(client->pokeCon, client->clientId), monId),
                                    sLearnMove);
                *seq = 8;
            } else {
                *seq = 10;
            }
        }
        break;
    }
    case 8: {
        u8 slot;

        if (func_ov093_021eac80(client->viewCore, &slot)) {
            if (slot == 4) {
                *seq = 5;
            } else {
                PartyPkm *pkm = GetSrcData(GetPokeParam(client->pokeCon, monId));
                u16 oldMove = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + slot, NULL);

                PokeParty_SetMove(pkm, sLearnMove, slot);
                Btlv_StringParam_Setup(&client->strParam, 5, 5);
                Btlv_StringParam_AddArg(&client->strParam, monId);
                Btlv_StringParam_AddArg(&client->strParam, oldMove);
                func_ov093_021ea638(client->viewCore, &client->strParam, func_ov093_021d2c7c);
                *seq = 9;
            }
        }
        break;
    }
    case 9:
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 2;
        }
        break;
    case 10:
        Btlv_StringParam_Setup(&client->strParam, 5, 7);
        Btlv_StringParam_AddArg(&client->strParam, monId);
        Btlv_StringParam_AddArg(&client->strParam, sLearnMove);
        func_ov093_021eaf78(&client->strParam, 0xff);
        func_ov093_021ea614(client->viewCore, &client->strParam);
        *seq = 11;
        break;
    case 11:
        if (func_ov093_021ea720(client->viewCore)) {
            BtlvStringParam yesParam;
            BtlvStringParam noParam;

            Btlv_StringParam_Setup(&yesParam, 6, 4);
            Btlv_StringParam_AddArg(&yesParam, sLearnMove);
            Btlv_StringParam_Setup(&noParam, 6, 5);
            Btlv_StringParam_AddArg(&noParam, sLearnMove);
            func_ov093_021eabc4(client->viewCore, &yesParam, &noParam, 1);
        }
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 12;
        }
        break;
    case 12: {
        u32 answer;

        if (func_ov093_021eac54(client->viewCore, &answer)) {
            if (answer == 0) {
                Btlv_StringParam_Setup(&client->strParam, 5, 8);
                Btlv_StringParam_AddArg(&client->strParam, monId);
                Btlv_StringParam_AddArg(&client->strParam, sLearnMove);
                func_ov093_021ea614(client->viewCore, &client->strParam);
                *seq = 13;
            } else {
                *seq = 5;
            }
        }
        break;
    }
    case 13:
        if (func_ov093_021ea710(client->viewCore)) {
            *seq = 1;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d2fd8(BtlClient *client, s32 *seq, const u32 *args) {
    static const u16 sEscapeMessages[4] = { 0x3d, 0x3e, 0x3f, 0x40 };
    BattleMon *mon;
    u16 message;

    switch (*seq) {
    case 0:
        func_ov094_021f7cfc(func_ov093_021b8e70(client->mainModule, args[0]), args[5], args[1], args[2], args[4]);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            if (args[2]) {
                mon = func_ov093_021b98ac(client->pokeCon, args[0]);
                Btlv_StringParam_Setup(&client->strParam, 1, 0x41);
                Btlv_StringParam_AddArg(&client->strParam, GetMonID(mon));
                func_ov094_021f883c(0x518);
            } else {
                if (args[1] < 4) {
                    message = sEscapeMessages[args[1]];
                } else {
                    message = 0x3d;
                }
                Btlv_StringParam_Setup(&client->strParam, 1, message);
            }
            func_ov093_021ea614(client->viewCore, &client->strParam);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            if (args[2]) {
                mon = func_ov093_021b98ac(client->pokeCon, args[0]);
                func_ov093_021ba2d4(client->mainModule, mon);
                (*seq)++;
            } else {
                return TRUE;
            }
        }
        break;
    case 3:
        if (!GFL_SndBGMIsPlaying()) {
            func_ov094_021f7c0c();
            GFL_SndBGMPlay(0x47c, 0xffff);
            func_ov093_021b92a0(client->mainModule);
            if (args[3]) {
                mon = func_ov093_021b98ac(client->pokeCon, args[0]);
                Btlv_StringParam_Setup(&client->strParam, 1, 0x42);
                Btlv_StringParam_AddArg(&client->strParam, GetMonID(mon));
                func_ov093_021ea614(client->viewCore, &client->strParam);
                (*seq)++;
            } else {
                return TRUE;
            }
        }
        break;
    case 4:
        if (func_ov093_021ea710(client->viewCore)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d3138(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0:
        func_ov094_021f7d44(func_ov093_021b8e70(client->mainModule, args[0]), args[1]);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            Btlv_StringParam_Setup(&client->strParam, 1, 0x43);
            func_ov093_021ea614(client->viewCore, &client->strParam);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d31a0(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u8 clientId = args[0];
        u8 result = args[1];

        func_ov093_021b9c50(GetPartyData(client->pokeCon, clientId), result, NULL, NULL);
        func_ov093_021eaa6c(client->viewCore, clientId, result);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021eaad8(client->viewCore)) {
            u8 result = args[1];
            u8 isNpc;
            u16 message;

            if (result == 1) {
                return TRUE;
            }
            isNpc = func_ov093_021b9f70(client->mainModule, args[0]);
            if (result == 3) {
                message = isNpc ? 0x26 : 0x27;
            } else {
                message = isNpc ? 0x28 : 0x29;
            }
            func_ov093_021ea678(client->viewCore, message, args);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea710(client->viewCore)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d324c(BtlClient *client, s32 *seq, const u32 *args) {
    u8 monId = args[0];
    u8 pos = MonIDToBattlePos(client->mainModule, client->pokeCon, monId);

    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            ChangeAbility(GetPokeParam(client->pokeCon, monId), args[1]);
            return TRUE;
        }
        func_ov093_021ea768(client->viewCore, pos, 0);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea778(client->viewCore, pos)) {
            ChangeAbility(GetPokeParam(client->pokeCon, monId), args[1]);
            func_ov093_021ea7a8(client->viewCore, pos);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ea7b8(client->viewCore, pos)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d32e0(BtlClient *client, s32 *seq, const u32 *args) {
    u8 monId1 = args[0];
    u8 monId2 = args[1];
    u16 ability1 = args[2];
    u16 ability2 = args[3];
    u8 pos1 = MonIDToBattlePos(client->mainModule, client->pokeCon, monId1);
    u8 pos2 = MonIDToBattlePos(client->mainModule, client->pokeCon, monId2);
    BattleMon *mon1 = GetPokeParam(client->pokeCon, monId1);
    BattleMon *mon2 = GetPokeParam(client->pokeCon, monId2);

    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            ChangeAbility(mon1, ability1);
            ChangeAbility(mon2, ability2);
            return TRUE;
        }
        if (IsAllyMonID(monId1, monId2)) {
            ChangeAbility(mon1, ability1);
            ChangeAbility(mon2, ability2);
            return TRUE;
        }
        func_ov093_021ea768(client->viewCore, pos1, 0);
        data_ov093_021f3b64 = 8;
        (*seq)++;
        break;
    case 1:
        func_ov093_021ea778(client->viewCore, pos1);
        if (data_ov093_021f3b64 != 0) {
            data_ov093_021f3b64--;
        } else {
            func_ov093_021ea768(client->viewCore, pos2, 0);
            (*seq)++;
        }
        break;
    case 2: {
        u8 done1 = func_ov093_021ea778(client->viewCore, pos1);
        u8 done2 = func_ov093_021ea778(client->viewCore, pos2);

        if (done1 && done2) {
            (*seq)++;
        }
        break;
    }
    case 3:
        ChangeAbility(mon1, ability1);
        ChangeAbility(mon2, ability2);
        data_ov093_021f3b64 = 0;
        (*seq)++;
        break;
    case 4:
        if (++data_ov093_021f3b64 > 8) {
            func_ov093_021ea7a8(client->viewCore, pos1);
            (*seq)++;
        }
        break;
    case 5:
        if (func_ov093_021ea7b8(client->viewCore, pos1)) {
            func_ov093_021ea7a8(client->viewCore, pos2);
            (*seq)++;
        }
        break;
    case 6:
        if (func_ov093_021ea7b8(client->viewCore, pos2)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d344c(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u8 monId = args[0];
        u8 pos = MonIDToBattlePos(client->mainModule, client->pokeCon, monId);
        BattleMon *mon = GetPokeParam(client->pokeCon, monId);
        BOOL skip = func_ov093_021cd9e0(client);

        IllusionBreak(mon);
        func_ov093_021ea868(client->viewCore, pos, skip);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021ea878(client->viewCore)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d34b0(BtlClient *client, s32 *seq, const u32 *args) {
    if (*seq == 0) {
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov094_021f7998(args[0]);
        (*seq)++;
    } else if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d34e8(BtlClient *client, s32 *seq, const u32 *args) {
    u8 pos = func_ov093_021b8e70(client->mainModule, args[0]);

    if (*seq == 0) {
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        if (!func_ov093_021b8590(client->mainModule) && data_ov095_0689c9d5(args[1])) {
            return TRUE;
        }
        func_ov093_021ea3b8(client->viewCore, pos, args[1]);
        (*seq)++;
    } else if (func_ov093_021ea3d4(client->viewCore)) {
        return TRUE;
    }
    return FALSE;
}




BOOL func_ov093_021d3568(BtlClient *client, s32 *seq, const u32 *args) {
    if (*seq == 0) {
        u8 pos1;

        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        pos1 = func_ov093_021b8e70(client->mainModule, args[0]);
        func_ov093_021ea3e8(client->viewCore, pos1, func_ov093_021b8e70(client->mainModule, args[1]), args[2]);
        (*seq)++;
    } else if (func_ov093_021ea3f8(client->viewCore)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d35c8(BtlClient *client, s32 *seq, const u32 *args) {
    switch (*seq) {
    case 0: {
        u8 pos =
            func_ov093_021b8e70(client->mainModule, MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]));

        ChangeForm(GetPokeParam(client->pokeCon, args[0]), args[1]);
        func_ov093_021ea888(client->viewCore, pos);
        (*seq)++;
        break;
    }
    case 1:
        if (func_ov093_021ea8a4(client->viewCore)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d3630(BtlClient *client, s32 *seq, const u32 *args) {
    u8 pos = MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]);

    switch (*seq) {
    case 0:
        if (func_ov093_021cd9e0(client)) {
            return TRUE;
        }
        func_ov093_021ea768(client->viewCore, pos, 1);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea778(client->viewCore, pos)) {
            (*seq)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021d3688(BtlClient *client, s32 *seq, const u32 *args) {
    u8 pos = MonIDToBattlePos(client->mainModule, client->pokeCon, args[0]);

    switch (*seq) {
    case 0:
        func_ov093_021ea788(client->viewCore, pos);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ea798(client->viewCore, pos)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021d36cc(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d5f90(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d36ec(BtlClient *client, s32 *seq, const u32 *args) {
    HPAdd(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d370c(BtlClient *client, s32 *seq, const u32 *args) {
    u8 moveIdx = args[1];
    u8 amount = args[2];
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);

    func_ov093_021d5608(mon, moveIdx, amount);
    return TRUE;
}

BOOL func_ov093_021d3734(BtlClient *client, s32 *seq, const u32 *args) {
    u8 moveIdx = args[1];
    u8 amount = args[2];
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);

    func_ov093_021d5640(mon, moveIdx, amount);
    return TRUE;
}

BOOL func_ov093_021d375c(BtlClient *client, s32 *seq, const u32 *args) {
    u8 moveIdx = args[1];
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);

    func_ov093_021d56cc(mon, moveIdx);
    return TRUE;
}

BOOL func_ov093_021d377c(BtlClient *client, s32 *seq, const u32 *args) {
    HPZero(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d3794(BtlClient *client, s32 *seq, const u32 *args) {
    Move_IncrementPP(GetPokeParam(client->pokeCon, args[0]), args[1], args[2]);
    return TRUE;
}

BOOL func_ov093_021d37b8(BtlClient *client, s32 *seq, const u32 *args) {
    Move_IncrementPP_Org(GetPokeParam(client->pokeCon, args[0]), args[1], args[2]);
    return TRUE;
}

BOOL func_ov093_021d37dc(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d5dc0(GetPokeParam(client->pokeCon, args[0]), args[1], args[2]);
    return TRUE;
}

BOOL func_ov093_021d37fc(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d5e38(GetPokeParam(client->pokeCon, args[0]), args[1], args[2]);
    return TRUE;
}

BOOL func_ov093_021d381c(BtlClient *client, s32 *seq, const u32 *args) {
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);

    func_ov093_021d5ea8(mon, 1, args[1]);
    func_ov093_021d5ea8(mon, 2, args[2]);
    func_ov093_021d5ea8(mon, 3, args[3]);
    func_ov093_021d5ea8(mon, 4, args[4]);
    func_ov093_021d5ea8(mon, 5, args[5]);
    func_ov093_021d5ea8(mon, 6, args[6]);
    func_ov093_021d5ea8(mon, 7, args[7]);
    return TRUE;
}

BOOL func_ov093_021d3894(BtlClient *client, s32 *seq, const u32 *args) {
    StatStageRecover(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d38ac(BtlClient *client, s32 *seq, const u32 *args) {
    StatStageReset(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d38c4(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d5f38(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d38e0(BtlClient *client, s32 *seq, const u32 *args) {
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);
    BattleCondition value;

    value.raw = args[2];
    SetMoveCondition(mon, args[1], value);
    return TRUE;
}

BOOL func_ov093_021d3900(BtlClient *client, s32 *seq, const u32 *args) {
    CureCondition(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d3918(BtlClient *client, s32 *seq, const u32 *args) {
    CureMoveCondition(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3934(BtlClient *client, s32 *seq, const u32 *args) {
    u8 pos1 = client->cmdArgs[1];
    u8 pos2 = client->cmdArgs[2];
    BattleParty *party = GetPartyData(client->pokeCon, (u8)client->cmdArgs[0]);
    BattleMon *mon;

    if (pos1 != pos2) {
        func_ov093_021b9c10(party, pos1, pos2);
    }
    mon = func_ov093_021b9bf0(party, pos1);
    func_ov093_021d63ec(mon, args[3]);
    func_ov093_021d6580(mon);
    return TRUE;
}

BOOL func_ov093_021d3988(BtlClient *client, s32 *seq, const u32 *args) {
    ChangePokeType(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d39a8(BtlClient *client, s32 *seq, const u32 *args) {
    u8 monIds[6];
    u32 numMons = args[0];
    u32 i;
    u32 numConditions = args[1];
    u32 j;

    func_ov093_021d8090(args[2], monIds);
    for (i = 0; i < numConditions; i++) {
        u32 condition = data_ov095_0689c9f9(i);

        if (condition == 0) {
            break;
        }
        for (j = 0; j < numMons; j++) {
            BattleMon *mon = GetPokeParam(client->pokeCon, monIds[j]);

            if (!IsFainted(mon)) {
                func_ov093_021d6064(mon, condition, NULL, NULL);
            }
        }
    }
    return TRUE;
}




BOOL func_ov093_021d3a20(BtlClient *client, s32 *seq, const u32 *args) {
    ConsumeItem(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3a40(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d6744(GetPokeParam(client->pokeCon, args[0]), args[1], args[2], args[3], args[4], args[5]);
    return TRUE;
}

BOOL func_ov093_021d3a7c(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d5fe4(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3a98(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d6008(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3ab4(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d5fc0(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3ad0(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d6440(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3aec(BtlClient *client, s32 *seq, const u32 *args) {
    ChangeAbility(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3b08(BtlClient *client, s32 *seq, const u32 *args) {
    SetItem(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3b28(BtlClient *client, s32 *seq, const u32 *args) {
    Move_UpdateID(GetPokeParam(client->pokeCon, args[0]), args[1], args[4], args[2], args[3]);
    return TRUE;
}

BOOL func_ov093_021d3b5c(BtlClient *client, s32 *seq, const u32 *args) {
    Clear_ForSwitch(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d3b74(BtlClient *client, s32 *seq, const u32 *args) {
    BattleCondition value;

    value.raw = args[1];
    FieldStatusaddEffectCore(client->field, args[0], value, 0);
    return TRUE;
}

BOOL func_ov093_021d3b88(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021efad8(client->field, args[0], args[1]);
    return TRUE;
}

BOOL func_ov093_021d3b9c(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021efb34(client->field, args[0]);
    return TRUE;
}

BOOL func_ov093_021d3bb0(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021efa94(client->field, args[0]);
    return TRUE;
}

BOOL func_ov093_021d3bc0(BtlClient *client, s32 *seq, const u32 *args) {
    COUNTER_Set(GetPokeParam(client->pokeCon, args[0]), args[1], args[2]);
    return TRUE;
}

BOOL func_ov093_021d3be0(BtlClient *client, s32 *seq, const u32 *args) {
    BattleMon *source = GetPokeParam(client->pokeCon, args[0]);

    CopyBatonPassParams(GetPokeParam(client->pokeCon, args[1]), source);
    return TRUE;
}

BOOL func_ov093_021d3c0c(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d6d5c(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

BOOL func_ov093_021d3c2c(BtlClient *client, s32 *seq, const u32 *args) {
    ResetSpActPriority(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d3c44(BtlClient *client, s32 *seq, const u32 *args) {
    u8 clientId = args[0];
    u8 value = args[1];

    if (clientId == client->clientId) {
        func_ov093_021cd9f4(client, value);
    } else {
        func_ov093_021b9b10(client->mainModule, clientId, value);
    }
    return TRUE;
}

BOOL func_ov093_021d3c70(BtlClient *client, s32 *seq, const u32 *args) {
    u8 clientId = args[0];

    func_ov093_021b9d10(client->mainModule, GetPartyData(client->pokeCon, clientId), clientId);
    return TRUE;
}

BOOL func_ov093_021d3c90(BtlClient *client, s32 *seq, const u32 *args) {
    ClearConsumedItem(GetPokeParam(client->pokeCon, args[0]));
    return TRUE;
}

BOOL func_ov093_021d3ca8(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021d6264(GetPokeParam(client->pokeCon, args[0]), (u8)args[1]);
    return TRUE;
}

BOOL func_ov093_021d3cc8(BtlClient *client, s32 *seq, const u32 *args) {
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);
    BattleMonDamageRecord record;

    BattleMonDamageRecord_Init(&record, args[1], args[2], args[4], args[3], args[5]);
    func_ov093_021d6848(mon, &record);
    return TRUE;
}

BOOL func_ov093_021d3cfc(BtlClient *client, s32 *seq, const u32 *args) {
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);

    func_ov093_021d6408(mon);
    ComboMove_ClearParam(mon);
    return TRUE;
}

BOOL func_ov093_021d3d1c(BtlClient *client, s32 *seq, const u32 *args) {
    func_ov093_021efc78(client->field, NULL, NULL);
    return TRUE;
}

BOOL func_ov093_021d3d2c(BtlClient *client, s32 *seq, const u32 *args) {
    u32 stat = args[1];
    BattleMon *mon = GetPokeParam(client->pokeCon, args[0]);

    SetBaseStatus(mon, stat, args[2]);
    return TRUE;
}

BOOL func_ov093_021d3d50(BtlClient *client, s32 *seq, const u32 *args) {
    SetWeight(GetPokeParam(client->pokeCon, args[0]), args[1]);
    return TRUE;
}

// pokebw2's BattleClient_GetClientId
u8 func_ov093_021d3d70(BtlClient *client) {
    return client->clientId;
}

// pokebw2's BattleClient_GetParty
BattleParty *func_ov093_021d3d7c(BtlClient *client) {
    return client->party;
}

u8 func_ov093_021d3d84(BtlClient *client) {
    return func_ov093_021ef9a8(client->field);
}

u8 func_ov093_021d3d90(BtlClient *client) {
    return client->shooterEnergy;
}

void func_ov093_021d3d9c(BtlClient *client) {
    u32 i;

    for (i = 0; i < 4; i++) {
        client->aiItems[i] = func_ov093_021b9f84(client->mainModule, client->clientId, i);
    }
}

u16 func_ov093_021d3dc4(BtlClient *client, BattleMon *mon, BattleParty *party) {
    u8 numMons = GetNumMonsInParty(party);
    u32 i;

    if (CheckCondition(mon, 0x13)) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (sAIItemMaxMons[i] < numMons) {
            break;
        }
        if (client->aiItems[i] != 0) {
            BOOL use = FALSE;

            if (ItemGetParam(client->aiItems[i], 0x29)) {
                if (GetBattleMonStat(mon, 0xd) > DivideMaxHp(mon, 4)) {
                    continue;
                }
                use = TRUE;
            } else if (IsXItem(client->aiItems[i], mon)) {
                use = TRUE;
            } else if (IsStatusCureItem(client->aiItems[i], mon)) {
                use = TRUE;
            }
            if (use) {
                u16 *slot = &client->aiItems[i];
                u16 item = *slot;

                *slot = 0;
                return item;
            }
        }
    }
    return 0;
}

static const BtlClientItemStat sXItems[6] = {
#include "asset/btl_client_sXItems.inc"
};

BOOL IsXItem(u16 item, BattleMon *mon) {
    u32 i;

    for (i = 0; i < NELEMS(sXItems); i++) {
        if (ItemGetParam(item, sXItems[i].param) && IsStatChangeValid(mon, sXItems[i].stat, 1)) {
            return TRUE;
        }
    }
    if (ItemGetParam(item, 0x24) && func_ov093_021d5f14(mon) == 0) {
        return TRUE;
    }
    return FALSE;
}

static const BtlClientItemCondition sCureItems[7] = {
#include "asset/btl_client_sCureItems.inc"
};

BOOL IsStatusCureItem(u16 item, BattleMon *mon) {
    u32 i;

    for (i = 0; i < NELEMS(sCureItems); i++) {
        if (ItemGetParam(item, sCureItems[i].param) && CheckCondition(mon, sCureItems[i].condition)) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's RecPlayer_Init
void func_ov093_021d3f04(BtlClientRecPlayer *recPlayer) {
    recPlayer->result = 0;
    recPlayer->seq = 0;
    recPlayer->chapterReached = FALSE;
    recPlayer->quitStarted = FALSE;
    recPlayer->quitDone = FALSE;
    recPlayer->chapter = 0;
    recPlayer->skipping = FALSE;
    recPlayer->skipTarget = 0;
    recPlayer->unk2_4 = FALSE;
    recPlayer->maxChapter = 0;
    recPlayer->dataEnd = FALSE;
    recPlayer->skipCount = 0;
    recPlayer->quitTimer = 0;
}

// pokebw2's RecPlayer_SetMaxChapter
void func_ov093_021d3f48(BtlClientRecPlayer *recPlayer, u32 maxChapter) {
    recPlayer->maxChapter = maxChapter;
}

BOOL func_ov093_021d3f4c(BtlClientRecPlayer *recPlayer) {
    return recPlayer->quitDone;
}

void func_ov093_021d3f54(BtlClientRecPlayer *recPlayer) {
    recPlayer->chapterReached = TRUE;
}

u32 func_ov093_021d3f60(BtlClientRecPlayer *recPlayer) {
    return recPlayer->result;
}

// pokebw2's RecPlayer_StartSkip
void func_ov093_021d3f64(BtlClientRecPlayer *recPlayer, u32 target) {
    recPlayer->skipping = TRUE;
    recPlayer->skipCount = 0;
    recPlayer->skipTarget = target;
    recPlayer->quitStarted = FALSE;
    recPlayer->quitDone = FALSE;
}

void func_ov093_021d3f88(BtlClientRecPlayer *recPlayer) {
    recPlayer->seq = 0;
    recPlayer->skipping = FALSE;
}

BOOL func_ov093_021d3f98(BtlClientRecPlayer *recPlayer) {
    if (recPlayer->skipCount == recPlayer->skipTarget) {
        return TRUE;
    }
    return FALSE;
}

u32 func_ov093_021d3fa8(BtlClientRecPlayer *recPlayer) {
    return recPlayer->skipTarget;
}

BOOL func_ov093_021d3fac(BtlClientRecPlayer *recPlayer) {
    if (recPlayer->quitStarted) {
        return FALSE;
    }
    recPlayer->unk2_4 = TRUE;
    return TRUE;
}

void func_ov093_021d3fc4(BtlClientRecPlayer *recPlayer) {
    recPlayer->unk2_4 = FALSE;
}

void func_ov093_021d3fd0(BtlClientRecPlayer *recPlayer) {
    recPlayer->dataEnd = TRUE;
}

void func_ov093_021d3fdc(BtlClient *client, BtlClientRecPlayer *recPlayer) {
    BOOL changed;
    u16 max;

    if (client->clientType == 2 && !recPlayer->unk2_4 && !recPlayer->dataEnd && recPlayer->maxChapter != 0) {
        max = recPlayer->maxChapter;
        changed = FALSE;
        if (recPlayer->chapterReached) {
            recPlayer->chapterReached = FALSE;
            if (!recPlayer->skipping) {
                if (recPlayer->chapter < max && recPlayer->seq == 0) {
                    recPlayer->chapter++;
                    if (recPlayer->quitTimer == 0) {
                        recPlayer->skipTarget = recPlayer->chapter;
                    }
                    changed = TRUE;
                }
            } else {
                if (recPlayer->skipCount < recPlayer->skipTarget) {
                    recPlayer->skipCount++;
                    if (recPlayer->skipCount == recPlayer->skipTarget) {
                        recPlayer->chapter = recPlayer->skipTarget;
                        if (client->viewCore != NULL) {
                            func_ov093_021eae74(client->viewCore, recPlayer->chapter, recPlayer->skipTarget);
                        }
                    }
                }
                return;
            }
        }
        if (recPlayer->chapter == 0 || client->viewCore == NULL) {
            return;
        }
        switch (recPlayer->seq) {
        case 0: {
            u32 input = func_ov093_021eae64(client->viewCore);
            BOOL moved = FALSE;

            switch (input) {
            case 2:
                recPlayer->result = 1;
                func_ov093_021eae04(client->viewCore);
                recPlayer->quitStarted = TRUE;
                recPlayer->seq = 1;
                func_ov093_021eaecc(client->viewCore, recPlayer->chapter, TRUE);
                GFL_SndBGMFadeOut(30);
                GFL_SndPlayerSetMuteStateEx(0, 0x3e);
                PokeVoice_SetMasterVolume(0);
                func_ov094_021f8b10();
                func_ov093_021b92a0(client->mainModule);
                break;
            case 0:
                if (recPlayer->skipTarget > 1) {
                    recPlayer->skipTarget--;
                    moved = TRUE;
                }
                recPlayer->quitTimer = 45;
                break;
            case 1:
                if (recPlayer->skipTarget < recPlayer->maxChapter) {
                    recPlayer->skipTarget++;
                    moved = TRUE;
                }
                recPlayer->quitTimer = 45;
                break;
            }
            if (moved || changed) {
                func_ov093_021eae74(client->viewCore, recPlayer->chapter, recPlayer->skipTarget);
            }
            if (recPlayer->quitTimer != 0) {
                recPlayer->quitTimer--;
                if (recPlayer->quitTimer == 0) {
                    recPlayer->result = 2;
                    func_ov093_021eae04(client->viewCore);
                    recPlayer->quitStarted = TRUE;
                    recPlayer->seq = 1;
                    GFL_SndBGMFadeOut(30);
                    GFL_SndPlayerSetMuteStateEx(0, 0x3e);
                    PokeVoice_SetMasterVolume(0);
                    func_ov094_021f8b10();
                }
            }
            break;
        }
        case 2:
            break;
        case 1:
            if (func_ov093_021eae14(client->viewCore)) {
                recPlayer->quitDone = TRUE;
                recPlayer->seq = 2;
            }
            break;
        }
    }
}

