// (Black / White) The battle sweep's battle half (tools/battle_sweep_bw.py, docs/battle-sweep.md), included once at the end of
// port/extern/src/ov093/btl_client.c's stand-in, after the game's file: BtlClient is complete there. Test only: nothing runs
// unless BWPORT_BW_TESTLIST=O:F,O:F,... is set. Each turn's action selection of a client is answered with the pair's move (the
// player's client O, the AI's F, whatever the menu or the AI picked): the server runs a fight action's move id whether or not the
// Pokemon knows it (a move it lacks has no PP slot: no PP is used). The send of the actions (func_ov093_021eec4c, renamed by the
// stand-in) is where it is rewritten, once per turn and client; the player's client refills both sides' HP first, prints
// `sweep: case K` and, past the list, `sweep: end list`. BWPORT_BW_TESTSLOT=stop: the first action selection of the player ends
// the run (`test: moves done`, the species passes). BWPORT_BW_TESTEXIT=1: exit 0 there. The player's client prints
// `sweep: exec battler B move M` for each move the server runs (its "used a move" command 0x59, read through the stand-in).
// Our lead fainting (self-KO, OHKO: the 1/8 cap is on damage only) ends the instance with `sweep: end battle` at the next send;
// so does the party list opening during the list (U-turn, Volt Switch, Baton Pass: the A pulse cannot pick a Pokemon there) and
// the experience command (0x45) of the foe fainting (its level-up / learn-move screens need a choice).
#include <stdlib.h>
#include <stdio.h>

// The game's names (Black / White: the numbers and the decomp's func_ names; Black 2 / White 2's stand-in
// port/extern_b2w2/src/ov167/btl_client.c defines these three first): the server commands "a move was used" (battler, move)
// and "experience gained", and the move's default target.
#ifndef PSW_MOVE_CMD
#define PSW_MOVE_CMD 0x59
#define PSW_EXP_CMD 0x45
#define PSW_TARGET(client, mon, move) func_ov093_021d80bc((client)->mainModule, (client)->pokeCon, (mon), (move))
#endif

extern void PORT_SweepCase(int k, const char *what);
extern void PORT_SweepEnd(const char *why, int exitNow);

BtlPokeCon *gPortSweepPokeCon;
static unsigned short sSwOur[1024], sSwFoe[1024];
static int sSwN = -1, sSwTurn[4], sSwShown = -1;

u16 PortSweep_ReadCmd(BtlClient *client, BtlServerCmdQueue *que, u32 *args) {
    u16 cmd = sPortSweepRealReadCmd(que, args);

    if (cmd == PSW_MOVE_CMD && client->clientType == 0 && getenv("BWPORT_BW_TESTLIST") != NULL) {
        fprintf(stderr, "sweep: exec battler %u move %u at V-blank %u\n", (unsigned)args[0], (unsigned)args[1], (unsigned)OS_GetVBlankCount());
    }
    if (cmd == PSW_EXP_CMD && client->clientType == 0 && sSwShown >= 0 && getenv("BWPORT_BW_TESTLIST") != NULL) {
        // the experience command (the foe fainted: Explosion, Memento): the level-up and learn-move screens wait for a
        // choice the A pulse cannot make, the case has run
        PORT_SweepEnd("battle", getenv("BWPORT_BW_TESTEXIT") != NULL);
    }
    return cmd;
}

void PortSweep_PokeList(BtlvCore *core, const BtlvPokeListCmd *cmd, s32 idx, u16 move, BtlvPokeSelectParam *sel) {
    if (sSwShown >= 0 && getenv("BWPORT_BW_TESTLIST") != NULL) {
        PORT_SweepEnd("battle", getenv("BWPORT_BW_TESTEXIT") != NULL);
    }
    sPortSweepRealPokeList(core, cmd, idx, move, sel);
}

static void PortSweep_Parse(void) {
    const char *p = getenv("BWPORT_BW_TESTLIST");
    int o, f, n;

    sSwN = 0;
    while (p != NULL && sSwN < 1024 && sscanf(p, "%d:%d%n", &o, &f, &n) == 2) {
        sSwOur[sSwN] = (unsigned short)o;
        sSwFoe[sSwN++] = (unsigned short)f;
        p += n;
        if (*p == ',') {
            p++;
        }
    }
}

static void PortSweep_Refill(BtlMainModule *mm) {
    int pc, c, i;

    for (pc = 0; pc < 2; pc++) {
        for (c = 0; c < 2; c++) {
            BattleParty *party = GetPartyData(&mm->pokeCons[pc], (u32)c);
            int n = GetNumMonsInParty(party);

            for (i = 0; i < n; i++) {
                BattleMon *m = GetBattleMonFromParty(party, (u8)i);

                m->core.hp = m->core.maxHP;
                ResetStatStages(&m->statStages);
            }
        }
    }
}

BOOL PortSweep_Send(BtlClient *client, BtlAdapter *adapter, const void *data, u32 size) {
    int id = client->clientId, human = client->clientType == 0, n = (int)(size / sizeof(BattleAction)), i;
    // a turn's action selection: the actions array with a fight action (every other send, acks and switch picks, is not a turn)
    int turn = data == (const void *)client->actions && size % sizeof(BattleAction) == 0 && n >= 1 && n <= 3 && id < 4 &&
               BattleAction_GetAction(&client->actions[0]) == 1;
    BOOL ret;

    gPortSweepPokeCon = &client->mainModule->pokeCons[1];
    if (getenv("BWPORT_BW_TESTLIST") != NULL && turn) {
        int k = sSwTurn[id];

        if (sSwN < 0) {
            PortSweep_Parse();
        }
        if (human && k >= sSwN) {
            PORT_SweepEnd("list", getenv("BWPORT_BW_TESTEXIT") != NULL);
        } else if (k < sSwN) {
            if (human && sSwShown != k) {
                char what[48];

                sSwShown = k;
                PortSweep_Refill(client->mainModule);
                snprintf(what, sizeof what, "our %u foe %u", (unsigned)sSwOur[k], (unsigned)sSwFoe[k]);
                PORT_SweepCase(k, what);
            }
            for (i = 0; i < n; i++) {
                BattleAction *a = &client->actions[i];

                if (BattleAction_GetAction(a) == 1) {
                    u16 move = human ? sSwOur[k] : sSwFoe[k];
                    BattleMon *mon = GetBattleMonFromParty(client->party, (u8)i);

                    BattleAction_SetFightParam(a, move, PSW_TARGET(client, mon, move));
                }
            }
        }
    }
    if (human && !turn && sSwShown >= 0 && getenv("BWPORT_BW_TESTLIST") != NULL &&
        GetBattleMonFromParty(client->party, 0)->core.hp == 0) {   // our lead fainted (a self-KO, an OHKO): the case ran, the
        PORT_SweepEnd("battle", getenv("BWPORT_BW_TESTEXIT") != NULL);   // harness resumes after it (the "next Pokemon" prompt is not a turn)
    }
    if (human && turn && getenv("BWPORT_BW_TESTSLOT") != NULL) {
        fprintf(stderr, "test: moves done at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        if (getenv("BWPORT_BW_TESTEXIT") != NULL) {
            fflush(NULL);
            _exit(0);
        }
    }
    ret = sPortSweepRealSend(adapter, data, size);
    if (ret && turn) {
        sSwTurn[id]++;
    }
    return ret;
}
