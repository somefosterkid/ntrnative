// (Black / White) The PC build compiles src/ov093/btl_client.c through this file (tools/port_sources.py). Test only (the route
// pilot, BWPORT_PILOT; docs/port-worker.md "Routes", IN A BATTLE): the pilot only presses A, so the player's move is whatever the
// move menu's cursor is on. The stand-ins of the action menu's and the move menu's polls (port/extern/src/ov093/btlv_core.c) call
// these two with the client, so the pilot keeps the move the A press chose while it can hurt the target, replaces one that
// cannot (immune: PortRoute_BestMove, the game's own type effectiveness, func_ov093_021d799c, no PP, disabled) with the
// usable one that hurts it most, and switches to the first party member that has one when no move of the lead can hurt it
// (Fighting and Normal against a Ghost). The DS build never sees this file.
#include "battle/btl_client.h"
#include "battle/btl_adapter.h"   // (its prototype is declared before the call sites are renamed)
static BOOL (*const sPortSweepRealSend)(BtlAdapter *, const void *, u32) = func_ov093_021eec4c;   // (White's build renames it: w_ ...)
#define func_ov093_021eec4c(adapter, data, size) PortSweep_Send(client, adapter, data, size)
BOOL PortSweep_Send(BtlClient *client, BtlAdapter *adapter, const void *data, u32 size);
#include "battle/btl_server_cmd.h"
static u16 (*const sPortSweepRealReadCmd)(BtlServerCmdQueue *, u32 *) = func_ov093_021cd5e0;
#define func_ov093_021cd5e0(que, args) PortSweep_ReadCmd(client, que, args)
u16 PortSweep_ReadCmd(BtlClient *client, BtlServerCmdQueue *que, u32 *args);
#include "battle/btlv.h"
static void (*const sPortSweepRealPokeList)(BtlvCore *, const BtlvPokeListCmd *, s32, u16, BtlvPokeSelectParam *) =
    BattleClientCmd_StartPokeList;
#define BattleClientCmd_StartPokeList(core, cmd, idx, move, sel) PortSweep_PokeList(core, cmd, idx, move, sel)
void PortSweep_PokeList(BtlvCore *core, const BtlvPokeListCmd *cmd, s32 idx, u16 move, BtlvPokeSelectParam *sel);
#include "../../../../src/ov093/btl_client.c"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "port_route.h"
#include "port_bw_sweep_battle.h"

// the first opponent of mon's position that has not fainted (the game's own position expansion, never a random pick)
static BattleMon *PortTarget(BtlClient *client, BattleMon *mon) {
    u8 ids[4];
    u8 n = func_ov093_021b8d38(client->mainModule, client->pokeCon,
                               0x100 | MonIDToBattlePos(client->mainModule, client->pokeCon, GetMonID(mon)), ids);
    u8 i;

    for (i = 0; i < n; i++) {
        BattleMon *t = GetPokeParam(client->pokeCon, ids[i]);

        if (!IsFainted(t)) {
            return t;
        }
    }
    return NULL;
}

// mon's move slot against target (-1: none hurts it): slot keepMove's when that move (an id, 0: none) can hurt it, else the best
static int PortBestMove(BtlClient *client, BattleMon *mon, BattleMon *target, u16 keepMove) {
    int eff[4], power[4], usable[4];
    int n = GetBattleMonMoveCount(mon);
    int keep = -1;
    int i;

    for (i = 0; i < n; i++) {
        u16 move = MoveGetID(mon, i);

        usable[i] = GetMovePP(mon, i) != 0 && !IsUnselectableMove(client, mon, move, NULL);
        eff[i] = PML_MoveIsDamaging(move) ? func_ov093_021d799c(PML_MoveGetType(move), GetPokeType(target)) : 0;
        power[i] = PML_MoveGetBasePower(move);
        if (keepMove != 0 && move == keepMove && keep < 0) {
            keep = i;
        }
    }
    return PortRoute_BestMove(n, eff, power, usable, keep);
}

// The move menu answered (func_ov093_021e98b4 TRUE): a fight action keeps its move unless that cannot hurt the target, then gets
// mon's best (the target stays the menu's)
void PortBattle_PickMove(BtlClient *client, BattleMon *mon, BattleAction *action) {
    BattleMon *target;
    int slot;

    if (getenv("BWPORT_PILOT") == NULL || BattleAction_GetAction(action) != 1 || (target = PortTarget(client, mon)) == NULL) {
        return;
    }
    slot = PortBestMove(client, mon, target, action->bits.move);
    if (slot >= 0 && MoveGetID(mon, slot) != action->bits.move) {
        fprintf(stderr, "pilot: battle move: #%d's slot %d (move %d) against #%d, not move %d\n", GetBattleMonSpecies(mon), slot,
                MoveGetID(mon, slot), GetBattleMonSpecies(target), action->bits.move);
        action->bits.move = MoveGetID(mon, slot);
    }
}

// The action menu answered Fight (func_ov093_021e97a4 1): when no move of mon hurts the target, the switch to the first party
// member in the back that has one is set as mon's action (the party list never opens) and TRUE says the menu's answer is spent
BOOL PortBattle_Switch(BtlClient *client, BattleMon *mon) {
    BattleMon *target;
    u8 count = GetNumMonsInParty(client->party);
    u32 i;

    if (getenv("BWPORT_PILOT") == NULL || (target = PortTarget(client, mon)) == NULL || PortBestMove(client, mon, target, 0) >= 0) {
        return FALSE;
    }
    for (i = GetNumMonsOnField(BtlSetup_GetBattleStyle(client->mainModule), client->numCoverPos); i < count; i++) {
        BattleMon *m = GetBattleMonFromParty(client->party, i);

        if (!func_ov093_021cfbbc(client, i) && CanPokemonBattle(m) && PortBestMove(client, m, target, 0) >= 0) {
            fprintf(stderr, "pilot: battle switch: no move of #%d hurts #%d: party %u (#%d)\n", GetBattleMonSpecies(mon),
                    GetBattleMonSpecies(target), (unsigned)i, GetBattleMonSpecies(m));
            func_ov093_021cf430(client, client->procActionIdx, 0);
            func_ov093_021d8394(client->procAction, client->procActionIdx, i);
            func_ov093_021ce150(client, func_ov093_021cf220);
            return TRUE;
        }
    }
    fprintf(stderr, "pilot: battle switch: no move of #%d hurts #%d and nobody in the back has one\n", GetBattleMonSpecies(mon),
            GetBattleMonSpecies(target));
    return FALSE;
}
