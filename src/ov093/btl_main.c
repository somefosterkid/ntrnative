// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "battle/btl_adapter.h"
#include "battle/btl_calc.h"
#include "battle/btl_client.h"
#include "battle/btl_field.h"
#include "battle/btl_main.h"
#include "battle/btl_net.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server.h"
#include "battle/btl_server_flow.h"
#include "battle/btl_setup.h"
#include "battle/btlv.h"
#include "constants/abilities.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "gfl/arc.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "system/comm_player_support.h"
#include "system/printsys.h"
#include "system/time_icon.h"
#include "system/text_speed.h"
#include "system/app_keycursor.h"
#include "system/gf_font.h"
#include "system/ir_check.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/ioreg.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/chatter.h"
#include "save/config.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/ringtone_sys.h"
#include "text/system/ability_handlers_btl_main.h"
#include "save/timesig_savedata.h"

// The first mon ID of each client
static const u8 data_ov093_021efcf0[4] = { 0, 12, 6, 18 };

static const u8 data_ov093_021efcfa[2][3] = {
    { 3, 5, 7 },
    { 2, 4, 6 },
};

static const u8 data_ov093_021efcf4[2][3] = {
    { 3, 5, 0 },
    { 2, 4, 0 },
};

// The steps of setting up a link battle, after the first, for each battle style
static BOOL (*const data_ov093_021efd00[7])(BtlMainModule *mainModule, s32 *state) = {
    NULL,
    func_ov093_021b75a8,
    func_ov093_021b7758,
    func_ov093_021b7968,
    func_ov093_021b7bb0,
    func_ov093_021b7e18,
    func_ov093_021b835c,
};

static BOOL (*const data_ov093_021efd1c[7])(BtlMainModule *mainModule, s32 *state) = {
    NULL,
    func_ov093_021b75a8,
    func_ov093_021b7758,
    func_ov093_021b7968,
    func_ov093_021b7bb0,
    func_ov093_021b7cb4,
    func_ov093_021b835c,
};

static BOOL (*const data_ov093_021efd38[7])(BtlMainModule *mainModule, s32 *state) = {
    NULL,
    func_ov093_021b75a8,
    func_ov093_021b7758,
    func_ov093_021b7968,
    func_ov093_021b7bb0,
    func_ov093_021b81dc,
    func_ov093_021b835c,
};

static const AdjacentOpponentData data_ov093_021efd70[BTL_POS_MAX] = {
    { 2, 2, { 5, 3, 6 }, { 0, 2, 6 } },
    { 2, 2, { 4, 2, 6 }, { 1, 3, 6 } },
    { 3, 3, { 1, 3, 5 }, { 0, 2, 4 } },
    { 3, 3, { 0, 2, 4 }, { 1, 3, 5 } },
    { 2, 2, { 1, 3, 6 }, { 4, 2, 6 } },
    { 2, 2, { 0, 2, 6 }, { 5, 3, 6 } },
};

static BOOL (*const data_ov093_021efd54[7])(BtlMainModule *mainModule, s32 *state) = {
    NULL,
    func_ov093_021b75a8,
    func_ov093_021b7758,
    func_ov093_021b7968,
    func_ov093_021b7bb0,
    func_ov093_021b8070,
    func_ov093_021b835c,
};

// The battle system's proc
const GameProcFunctions data_ov093_021efdac = { func_ov093_021b6100, func_ov093_021b6434, func_ov093_021b6500 };

void func_ov093_021ba878(BtlMainModule *mainModule);
void func_ov093_021e88cc(void);
void func_ov093_021ba82c(BtlSetup *setup);
void func_ov093_021cd6ec(void);
void func_ov093_021d804c(u16 terminator);
void func_ov093_021d8058(void);
void func_ov093_021e88f8(void);

// Starts a step of the module
static inline void BtlMainSeq_Set(BtlMainSeq *seq, BtlMainSeqFunc func, BtlMainModule *mainModule) {
    seq->func = func;
    seq->nextFunc = NULL;
    seq->mainModule = mainModule;
    seq->state = 0;
}

// Runs the module's current step, then its next one, until both are done
static inline BOOL BtlMainSeq_Run(BtlMainSeq *seq) {
    BOOL done;

    if (seq->func != NULL) {
        if (seq->func(&seq->state, seq->mainModule)) {
            seq->func = NULL;
            seq->state = 0;
        }
        done = FALSE;
    } else if (seq->nextFunc != NULL) {
        if (seq->nextFunc(&seq->state, seq->mainModule)) {
            seq->nextFunc = NULL;
            done = TRUE;
        } else {
            done = FALSE;
        }
    } else {
        done = TRUE;
    }
    return done;
}



BOOL func_ov093_021b6100(GameProc *proc, u32 *state, void *param, void *work) {
    BtlMainModule *mainModule = work;
    BtlSetup *setup = param;
    BOOL notInitialized;

    switch (*state) {
    case 0:
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_BATTLE, 0x20000);
        GFL_HeapCreateChild(HEAPID_USER, 0x14, 0x8000);
        GFL_HeapCreateChild(HEAPID_USER, 0x13, 0xab100);
        mainModule = GFL_ProcInitSubsystem(proc, sizeof(BtlMainModule), HEAPID_BATTLE);
        MIi_CpuClear32(0, mainModule, sizeof(BtlMainModule));
        func_ov093_021e8dc4(0x13);
        func_ov093_021e88cc();
        if (!GFL_SndIsVolumeControlCallbackSet()) {
            GFL_SndSetVolumeControlCallbacks();
            mainModule->unk473_7 = 1;
        }
        mainModule->heapId = HEAPID_BATTLE;
        mainModule->setup = setup;
        setup->unk90 = 6;
        mainModule->unkC0 = mainModule->setup->unk34[0];
        mainModule->unk440 = mainModule->setup->unk74;
        mainModule->unk442 = mainModule->setup->unk76;
        mainModule->unk473_3 = 0;
        mainModule->unk473_4 = 0;
        mainModule->unk473_0 = 0;
        mainModule->unk473_1 = 0;
        mainModule->unk473_5 = 0;
        mainModule->unk473_6 = 0;
        mainModule->unk471 = 0;
        mainModule->mainFunc = NULL;
        notInitialized = FALSE;
        mainModule->viewCore = NULL;
        mainModule->result = 7;
        mainModule->unk43C = func_02008374(mainModule->setup->config);
        if (!func_020083ac(setup->config)) {
            notInitialized = TRUE;
        }
        mainModule->unk473_2 = notInitialized;
        mainModule->field = func_ov093_021ef958(mainModule->heapId);
        func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
        func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
        func_ov093_021ba82c(mainModule->setup);
        mainModule->unk2C0 = NULL;
        mainModule->unk2C4 = NULL;
        if (mainModule->setup->fieldSituation.unk1b == 0) {
            buildSeed(&mainModule->rand);
        } else {
            mainModule->rand = setup->rand;
        }
        func_ov093_021d7840(&mainModule->rand, mainModule->heapId);
        func_ov093_021cd6ec();
        mainModule->prizeMoney = func_ov093_021d7f88(setup);
        mainModule->money = 0;
        mainModule->unk438 = 0;
        if (mainModule->setup->fieldSituation.unk1b == 0) {
            u32 i;

            for (i = 0; i < 4; i++) {
                mainModule->setup->unk44[i] = 0;
            }
        }
        func_ov093_021d8230(&mainModule->unk448);
        {
            BOOL enabled = func_020083c8(setup->config);
            u8 flag = 0;

            if (enabled) {
                flag = 1;
            }
            mainModule->unk46F = flag;
        }
        func_ov093_021d4150(setup->fieldSituation.netHandle, setup->unk84, 0x14);
        func_ov093_021d804c(HEAPID_BATTLE);
        (*state)++;
        break;
    case 1:
        if (mainModule->setup->fieldSituation.netHandle != NULL && func_ov093_021d4270()) {
            mainModule->unk473_0 = 1;
            mainModule->unk473_1 = 1;
            return TRUE;
        }
        if (func_ov093_021d4230()) {
            func_ov093_021eea30(mainModule->setup->fieldSituation.unk18);
            func_ov093_021b66d4(&mainModule->seq, mainModule, mainModule->setup);
            mainModule->unk460 = 0;
            func_ov093_021b9db8(mainModule);
            func_ov093_021ba094(mainModule);
            (*state)++;
        }
        break;
    case 2:
        if (BtlMainSeq_Run(&mainModule->seq)) {
            if (mainModule->unk473_0) {
                mainModule->unk473_1 = 1;
                return TRUE;
            }
            if (mainModule->setup->battleType != 4) {
                func_ov093_021ba1b4(mainModule, 4);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021b6434(GameProc *proc, u32 *state, void *param, void *work) {
    BtlMainModule *mainModule = work;
    u32 result;

    if (mainModule->mainFunc == NULL) {
        return TRUE;
    }
    if (mainModule->mainFunc(mainModule) != FALSE) {
        if (func_ov093_021b9114(mainModule) == 0) {
            result = func_ov093_021ba54c(mainModule);
            func_ov093_021ba6c0(mainModule);
            func_ov093_021ba878(mainModule);
            func_ov093_021ba2e4(mainModule);
            switch (result) {
            case 1:
                mainModule->setup->unk88 = mainModule->prizeMoney + mainModule->money;
                break;
            case 0:
            case 2:
                mainModule->setup->unk88 = -mainModule->unk438;
                break;
            }
        } else {
            mainModule->setup->unkC1_1 = func_ov093_021b64cc(mainModule);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021b64cc(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (DoesClientExist(mainModule, i) && !func_ov093_021ee894(&mainModule->recReader, i)) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_ov093_021b6500(GameProc *proc, u32 *state, void *param, void *work) {
    BtlMainModule *mainModule = work;
    BOOL done;
    s32 brightness;

    switch (*state) {
    case 0:
        if (!mainModule->unk473_5) {
            GFL_SndBGMFadeOut(30);
        }
        mainModule->unk460 = 0;
        if (GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR)) {
            mainModule->unk460 = -16;
            GX_SetMasterBrightness(-16);
            GXS_SetMasterBrightness(-16);
            *state += 2;
            break;
        }
        (*state)++;
        break;
    case 1:
        if (mainModule->unk460 > -16) {
            mainModule->unk460--;
            brightness = GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR);
            if (brightness > mainModule->unk460) {
                GX_SetMasterBrightness(mainModule->unk460);
            }
            brightness = GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR);
            if (brightness > mainModule->unk460) {
                GXS_SetMasterBrightness(mainModule->unk460);
            }
            break;
        }
        (*state)++;
        break;
    case 2:
        if (!mainModule->unk473_5) {
            if (!GFL_SndBGMIsFading()) {
                func_02005d28();
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case 3:
        if (mainModule->field != NULL) {
            func_ov093_021ef980(mainModule->field);
            mainModule->field = NULL;
        }
        if (mainModule->unk2C0 != NULL) {
            GFL_HeapFree(mainModule->unk2C0);
            mainModule->unk2C0 = NULL;
        }
        if (mainModule->unk2C4 != NULL) {
            func_ov093_021d4d5c(mainModule->unk2C4);
            mainModule->unk2C4 = NULL;
        }
        func_ov093_021d7894();
        func_ov093_021ba0cc(mainModule);
        func_ov093_021b9de8(mainModule);
        func_ov093_021b6804(&mainModule->seq, mainModule, mainModule->setup);
        (*state)++;
        break;
    case 4:
        if (mainModule->seq.func != NULL) {
            if (mainModule->seq.func(&mainModule->seq.state, mainModule->seq.mainModule)) {
                mainModule->seq.func = NULL;
                mainModule->seq.state = 0;
            }
            done = FALSE;
        } else if (mainModule->seq.nextFunc != NULL) {
            if (mainModule->seq.nextFunc(&mainModule->seq.state, mainModule->seq.mainModule)) {
                mainModule->seq.nextFunc = NULL;
                done = TRUE;
            } else {
                done = FALSE;
            }
        } else {
            done = TRUE;
        }
        if (done) {
            func_ov093_021eea54();
            func_ov093_021d4254();
            func_ov093_021d8058();
            (*state)++;
        }
        break;
    case 5:
        func_ov093_021e8df8();
        GFL_SndPlayerSetMuteStateEx(1, 0x3e);
        PokeVoice_ResetMasterVolume();
        if (mainModule->unk473_7) {
            func_02029f6c();
            mainModule->unk473_7 = 0;
        }
        GFL_ProcReleaseSubsystem(proc);
        GFL_HeapDelete(0x13);
        GFL_HeapDelete(0x14);
        GFL_HeapDelete(0x12);
        func_ov093_021e88f8();
        return TRUE;
    }
    return FALSE;
}


void func_ov093_021b66d4(BtlMainSeq *seq, BtlMainModule *mainModule, BtlSetup *setup) {
    u32 i;

    for (i = 0; i < BTL_POS_MAX; i++) {
        mainModule->posClientIds[i] = 4;
    }
    if (setup->fieldSituation.unk18 == 0) {
        switch (setup->battleStyle) {
        case 0:
            BtlMainSeq_Set(seq, func_ov093_021b6aac, mainModule);
            return;
        case 1:
            if (setup->fieldSituation.unk1a == 0) {
                BtlMainSeq_Set(seq, func_ov093_021b6c5c, mainModule);
                return;
            }
            BtlMainSeq_Set(seq, func_ov093_021b6dd0, mainModule);
            return;
        case 2:
            BtlMainSeq_Set(seq, func_ov093_021b7050, mainModule);
            return;
        case 3:
            BtlMainSeq_Set(seq, func_ov093_021b71d4, mainModule);
            return;
        default:
            BtlMainSeq_Set(seq, func_ov093_021b6aac, mainModule);
            return;
        }
    }
    switch (setup->battleStyle) {
    case 0:
        BtlMainSeq_Set(seq, func_ov093_021b734c, mainModule);
        return;
    case 1:
        BtlMainSeq_Set(seq, func_ov093_021b73c4, mainModule);
        return;
    case 2:
        BtlMainSeq_Set(seq, func_ov093_021b7518, mainModule);
        return;
    case 3:
        BtlMainSeq_Set(seq, func_ov093_021b7488, mainModule);
        return;
    default:
        BtlMainSeq_Set(seq, func_ov093_021b6aac, mainModule);
        return;
    }
}

void func_ov093_021b6804(BtlMainSeq *seq, BtlMainModule *mainModule, BtlSetup *setup) {
    BtlMainSeq_Set(seq, func_ov093_021b6c08, mainModule);
}

u8 func_ov093_021b6818(BtlSetup *setup) {
    switch (setup->battleType) {
    case 0:
        return FALSE;
    case 1:
        return FALSE;
    case 2:
        return FALSE;
    case 3:
        return TRUE;
    default:
        return FALSE;
    }
}

void func_ov093_021b6848(BtlMainModule *mainModule, BtlSetup *setup) {
    u32 i;
    void *chatter;
    u8 chatterFlags[4];

    mainModule->playerClientId = setup->fieldSituation.unk19;
    i = 0;
    mainModule->unk46D = func_ov093_021b8bec(mainModule, mainModule->playerClientId, 0);
    func_ov093_021b68d8(mainModule, setup);
    if (!func_ov093_021b9114(mainModule)) {
        chatter = getChatterBlockAddress(GameData_GetSaveControl(setup->gameData));
        mainModule->unk3E0[0] = allocChatotChatterBlk(mainModule->heapId);
        moveChatter(mainModule->unk3E0[0], chatter);
        mainModule->setup->unk44[0] = func_ov093_021b997c(mainModule, i);
        return;
    }
    for (; i < 4; i++) {
        chatterFlags[func_ov093_021ba710(mainModule->playerClientId, i)] = mainModule->setup->unk44[i];
    }
    for (i = 0; i < 4; i++) {
        mainModule->setup->unk44[i] = chatterFlags[i];
    }
}

void func_ov093_021b68d8(BtlMainModule *mainModule, BtlSetup *setup) {
    u8 index0;
    u8 index1;
    u8 index2;
    u8 index3;

    index0 = func_ov093_021ba710(mainModule->playerClientId, 0);
    index1 = func_ov093_021ba710(mainModule->playerClientId, 1);
    func_ov093_021ba128(mainModule, 0, setup->party[index0]);
    func_ov093_021ba128(mainModule, 1, setup->party[index1]);
    index2 = func_ov093_021ba710(mainModule->playerClientId, 2);
    index3 = func_ov093_021ba710(mainModule->playerClientId, 3);
    if (setup->party[index2] != NULL) {
        func_ov093_021ba128(mainModule, 2, setup->party[index2]);
    }
    if (setup->party[index3] != NULL) {
        func_ov093_021ba128(mainModule, 3, setup->party[index3]);
    }
}

void func_ov093_021b6954(BtlMainModule *mainModule, u8 clientId) {
    BtlSetup *setup = mainModule->setup;
    u8 index = func_ov093_021ba710(mainModule->playerClientId, clientId);
    BtlTrainerData *trainer = &mainModule->trainers[clientId];

    if (setup->trainers[index]->trainerId != 0) {
        func_ov093_021b9e7c(trainer, setup->trainers[index]);
    } else {
        func_ov093_021b9e38(trainer, mainModule->heapId, setup->unk34[index]);
    }
}


u8 func_ov093_021b6994(BtlMainModule *mainModule, u8 clientId) {
    BtlSetup *setup = mainModule->setup;

    switch (setup->battleStyle) {
    case 0:
        return 1;
    case 3:
        return 1;
    case 2:
        return 3;
    case 1:
        if (setup->fieldSituation.unk1a == 0) {
            return 2;
        }
        if (setup->battleType == 3) {
            return 1;
        }
        if (setup->party[(u8)((clientId + 2) & 3)] != NULL &&
            PokeParty_GetPkmCount(setup->party[(u8)((clientId + 2) & 3)]) > 0) {
            return 1;
        }
        return 2;
    default:
        return 1;
    }
}

void func_ov093_021b69fc(BtlMainModule *mainModule, BtlSetup *setup) {
    if (setup->fieldSituation.unk1b == 0) {
        func_ov093_021b9e38(&mainModule->trainers[0], mainModule->heapId, mainModule->unkC0);
        func_ov093_021b9e7c(&mainModule->trainers[1], setup->trainers[1]);
    } else {
        func_ov093_021b6954(mainModule, 0);
        func_ov093_021b6954(mainModule, 1);
    }
}

void func_ov093_021b6a3c(BtlMainModule *mainModule, BtlSetup *setup) {
    if (setup->fieldSituation.unk1b != 0) {
        func_ov093_021ee644(&mainModule->recReader, setup->unk94, setup->unk98);
        func_ov093_021cd938(mainModule->clients[0], &mainModule->recReader);
        func_ov093_021cd938(mainModule->clients[1], &mainModule->recReader);
    }
}


void func_ov093_021b6a70(BtlMainModule *mainModule, BtlSetup *setup, u32 arg2) {
    BtlClient *client;

    if (setup->fieldSituation.unk1b != 0) {
        client = mainModule->clients[setup->fieldSituation.unk19];
    } else {
        client = mainModule->clients[0];
    }
    mainModule->viewCore = BtlvCore_Create(mainModule, client, &mainModule->pokeCons[0], arg2, 0x13);
    func_ov093_021cd95c(client, mainModule->viewCore);
}

BOOL func_ov093_021b6aac(u32 *state, BtlMainModule *mainModule) {
    BtlSetup *setup = mainModule->setup;
    u32 unk = func_ov093_021b6818(setup);

    mainModule->clientCount = 2;
    mainModule->posClientIds[0] = 0;
    mainModule->posClientIds[1] = 1;
    func_ov093_021b6848(mainModule, setup);
    func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 1);
    func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 1);
    mainModule->server =
        func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
    mainModule->unk46E = 1;
    func_ov093_021b69fc(mainModule, setup);
    mainModule->clients[0] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 0, 1,
                                                 0, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    mainModule->clients[1] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 1, 1,
                                                 1, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    func_ov093_021b6a3c(mainModule, setup);
    func_ov093_021b6a70(mainModule, setup, unk);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[0]), 0, 1);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[1]), 1, 1);
    func_ov093_021bab40(mainModule->server);
    mainModule->mainFunc = func_ov093_021b83f4;
    return TRUE;
}

BOOL func_ov093_021b6c08(u32 *state, BtlMainModule *mainModule) {
    u32 i;

    if (mainModule->viewCore != NULL) {
        func_ov093_021e9034(mainModule->viewCore);
        mainModule->viewCore = NULL;
    }
    for (i = 0; i < 4; i++) {
        if (mainModule->clients[i] != NULL) {
            func_ov093_021cd8e0(mainModule->clients[i]);
        }
    }
    func_ov093_021b9674(&mainModule->pokeCons[0]);
    func_ov093_021b9674(&mainModule->pokeCons[1]);
    if (mainModule->server != NULL) {
        func_ov093_021bab5c(mainModule->server);
    }
    if (mainModule->unk0C != NULL) {
        func_ov093_021bab5c(mainModule->unk0C);
    }
    return TRUE;
}

BOOL func_ov093_021b6c5c(u32 *state, BtlMainModule *mainModule) {
    BtlSetup *setup = mainModule->setup;
    u32 unk = func_ov093_021b6818(setup);

    mainModule->playerClientId = 0;
    mainModule->clientCount = 2;
    mainModule->posClientIds[0] = 0;
    mainModule->posClientIds[1] = 1;
    mainModule->posClientIds[2] = 0;
    mainModule->posClientIds[3] = 1;
    func_ov093_021b6848(mainModule, setup);
    func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 1);
    func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 1);
    func_ov093_021b69fc(mainModule, setup);
    mainModule->server =
        func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
    mainModule->unk46E = 1;
    mainModule->clients[0] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 0, 2,
                                                 0, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    mainModule->clients[1] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 1, 2,
                                                 1, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    func_ov093_021b6a3c(mainModule, setup);
    func_ov093_021b6a70(mainModule, setup, unk);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[0]), 0, 2);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[1]), 1, 2);
    func_ov093_021bab40(mainModule->server);
    mainModule->mainFunc = func_ov093_021b83f4;
    return TRUE;
}

BOOL func_ov093_021b6dd0(u32 *state, BtlMainModule *mainModule) {
    BtlSetup *setup = mainModule->setup;
    u32 unk = func_ov093_021b6818(setup);
    s32 i;

    mainModule->playerClientId = 0;
    mainModule->clientCount = 2;
    mainModule->posClientIds[0] = 0;
    mainModule->posClientIds[2] = 0;
    mainModule->posClientIds[1] = 1;
    mainModule->posClientIds[3] = 1;
    switch (setup->fieldSituation.unk1a) {
    case 1:
    case 2:
    case 3:
        mainModule->posClientIds[2] = 2;
        mainModule->posClientIds[3] = 3;
        mainModule->clientCount = 4;
        break;
    case 4:
        mainModule->posClientIds[3] = 3;
        mainModule->clientCount = 3;
        break;
    }
    func_ov093_021b6848(mainModule, setup);
    func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
    func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(mainModule, i)) {
            func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, i);
            func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, i);
        }
    }
    if (setup->fieldSituation.unk1b == 0) {
        func_ov093_021b9e38(&mainModule->trainers[0], mainModule->heapId, mainModule->unkC0);
        func_ov093_021b9e7c(&mainModule->trainers[1], setup->trainers[1]);
        if (DoesClientExist(mainModule, 2)) {
            func_ov093_021b9e7c(&mainModule->trainers[2], setup->trainers[2]);
        }
        if (DoesClientExist(mainModule, 3)) {
            func_ov093_021b9e7c(&mainModule->trainers[3], setup->trainers[3]);
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021b6954(mainModule, i);
            }
        }
    }
    mainModule->server =
        func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
    mainModule->unk46E = 1;
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(mainModule, i)) {
            u8 count = func_ov093_021b8b78(mainModule, i);
            BOOL isAI = FALSE;

            if (i != 0) {
                isAI = TRUE;
            }
            mainModule->clients[i] =
                func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, i, count,
                                    isAI, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
        }
    }
    if (setup->fieldSituation.unk1b != 0) {
        func_ov093_021ee644(&mainModule->recReader, setup->unk94, setup->unk98);
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021cd938(mainModule->clients[i], &mainModule->recReader);
            }
        }
    }
    func_ov093_021b6a70(mainModule, setup, unk);
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(mainModule, i)) {
            u8 count = func_ov093_021b8b78(mainModule, i);

            func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[i]), i, count);
        }
    }
    func_ov093_021bab40(mainModule->server);
    mainModule->mainFunc = func_ov093_021b83f4;
    return TRUE;
}


BOOL func_ov093_021b7050(u32 *state, BtlMainModule *mainModule) {
    BtlSetup *setup = mainModule->setup;
    u32 unk = func_ov093_021b6818(setup);

    mainModule->clientCount = 2;
    mainModule->unk46E = 1;
    mainModule->posClientIds[0] = 0;
    mainModule->posClientIds[1] = 1;
    mainModule->posClientIds[2] = 0;
    mainModule->posClientIds[3] = 1;
    mainModule->posClientIds[4] = 0;
    mainModule->posClientIds[5] = 1;
    func_ov093_021b6848(mainModule, setup);
    func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 1);
    func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 1);
    func_ov093_021b69fc(mainModule, setup);
    mainModule->server =
        func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
    mainModule->unk46E = 1;
    mainModule->clients[0] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 0, 3,
                                                 0, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    mainModule->clients[1] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 1, 3,
                                                 1, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    func_ov093_021b6a3c(mainModule, setup);
    func_ov093_021b6a70(mainModule, setup, unk);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[0]), 0, 3);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[1]), 1, 3);
    func_ov093_021bab40(mainModule->server);
    mainModule->mainFunc = func_ov093_021b83f4;
    return TRUE;
}

BOOL func_ov093_021b71d4(u32 *state, BtlMainModule *mainModule) {
    BtlSetup *setup = mainModule->setup;
    u32 unk = func_ov093_021b6818(setup);

    mainModule->clientCount = 2;
    mainModule->posClientIds[0] = 0;
    mainModule->posClientIds[1] = 1;
    mainModule->posClientIds[2] = 0;
    mainModule->posClientIds[3] = 1;
    mainModule->posClientIds[4] = 0;
    mainModule->posClientIds[5] = 1;
    func_ov093_021b6848(mainModule, setup);
    func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, 1);
    func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 0);
    func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, 1);
    mainModule->server =
        func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
    mainModule->unk46E = 1;
    func_ov093_021b69fc(mainModule, setup);
    mainModule->clients[0] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 0, 1,
                                                 0, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    mainModule->clients[1] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], 0, setup->fieldSituation.netHandle, 1, 1,
                                                 1, unk, setup->fieldSituation.unk1b, &mainModule->rand, mainModule->heapId);
    func_ov093_021b6a3c(mainModule, setup);
    func_ov093_021b6a70(mainModule, setup, unk);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[0]), 0, 1);
    func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[1]), 1, 1);
    func_ov093_021bab40(mainModule->server);
    mainModule->mainFunc = func_ov093_021b83f4;
    return TRUE;
}

BOOL func_ov093_021b734c(u32 *state, BtlMainModule *mainModule) {
    if (*state == 0) {
        mainModule->clientCount = 2;
        mainModule->posClientIds[0] = 0;
        mainModule->posClientIds[1] = 1;
        mainModule->playerClientId = mainModule->setup->fieldSituation.unk19;
        mainModule->unk46D = func_ov093_021b8bec(mainModule, mainModule->playerClientId, 0);
        (*state)++;
        return FALSE;
    }
    if (*state < 7) {
        if (data_ov093_021efd1c[*state](mainModule, &mainModule->unk460)) {
            mainModule->unk460 = 0;
            (*state)++;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021b73c4(u32 *state, BtlMainModule *mainModule) {
    if (*state == 0) {
        switch (mainModule->setup->fieldSituation.unk1a) {
        case 0:
        default:
            mainModule->clientCount = 2;
            mainModule->posClientIds[0] = 0;
            mainModule->posClientIds[1] = 1;
            mainModule->posClientIds[2] = 0;
            mainModule->posClientIds[3] = 1;
            break;
        case 2:
            mainModule->unk471 = 2;
        case 1:
            mainModule->clientCount = 4;
            mainModule->posClientIds[0] = 0;
            mainModule->posClientIds[1] = 1;
            mainModule->posClientIds[2] = 2;
            mainModule->posClientIds[3] = 3;
            break;
        }
        mainModule->playerClientId = mainModule->setup->fieldSituation.unk19;
        mainModule->unk46D = func_ov093_021b8bec(mainModule, mainModule->playerClientId, 0);
        (*state)++;
        return FALSE;
    }
    if (*state < 7) {
        if (data_ov093_021efd00[*state](mainModule, &mainModule->unk460)) {
            mainModule->unk460 = 0;
            (*state)++;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021b7488(u32 *state, BtlMainModule *mainModule) {
    if (*state == 0) {
        mainModule->clientCount = 2;
        mainModule->posClientIds[0] = 0;
        mainModule->posClientIds[1] = 1;
        mainModule->posClientIds[2] = 0;
        mainModule->posClientIds[3] = 1;
        mainModule->posClientIds[4] = 0;
        mainModule->posClientIds[5] = 1;
        mainModule->playerClientId = mainModule->setup->fieldSituation.unk19;
        mainModule->unk46D = func_ov093_021b8bec(mainModule, mainModule->playerClientId, 0);
        (*state)++;
        return FALSE;
    }
    if (*state < 7) {
        if (data_ov093_021efd38[*state](mainModule, &mainModule->unk460)) {
            mainModule->unk460 = 0;
            (*state)++;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021b7518(u32 *state, BtlMainModule *mainModule) {
    if (*state == 0) {
        mainModule->clientCount = 2;
        mainModule->posClientIds[0] = 0;
        mainModule->posClientIds[1] = 1;
        mainModule->posClientIds[2] = 0;
        mainModule->posClientIds[3] = 1;
        mainModule->posClientIds[4] = 0;
        mainModule->posClientIds[5] = 1;
        mainModule->playerClientId = mainModule->setup->fieldSituation.unk19;
        mainModule->unk46D = func_ov093_021b8bec(mainModule, mainModule->playerClientId, 0);
        (*state)++;
        return FALSE;
    }
    if (*state < 7) {
        if (data_ov093_021efd54[*state](mainModule, &mainModule->unk460)) {
            mainModule->unk460 = 0;
            (*state)++;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov093_021d4270(void);
BOOL func_ov093_021d4294(u8 clientId);
BOOL func_ov093_021d4348(void);
u8 func_ov093_021d4378(void);
BOOL func_ov093_021d43b8(BtlMainSyncData *syncData);
BOOL func_ov093_021d440c(BtlMainSyncData *syncData);




BOOL func_ov093_021b75a8(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    switch (*state) {
    case 0:
        if (func_ov093_021d4294(mainModule->playerClientId)) {
            (*state)++;
        }
        break;
    case 1:
        if (func_ov093_021d4348()) {
            mainModule->unk46A = 0;
            mainModule->unk46E = func_ov093_021d4360();
            mainModule->setup->unk91 = func_ov093_021d4378();
            if (func_ov093_021d4360()) {
                *state = 2;
            } else {
                *state = 3;
            }
        }
        break;
    case 2:
        mainModule->syncData.rand = mainModule->rand;
        setup = mainModule->setup;
        mainModule->syncData.unk18 = setup->unk86;
        mainModule->syncData.unk1A = setup->unk74;
        mainModule->syncData.unk1C = setup->unk76;
        if (BtlSetup_IsBattleType(mainModule, 0x1000)) {
            mainModule->syncData.unk1E = mainModule->unk43C;
            mainModule->syncData.unk1F_0 = mainModule->unk473_2;
        } else {
            mainModule->syncData.unk1E = 2;
            mainModule->syncData.unk1F_0 = 1;
        }
        if (func_ov093_021d43b8(&mainModule->syncData)) {
            (*state)++;
        }
        break;
    case 3:
        if (func_ov093_021d440c(&mainModule->syncData)) {
            setup = mainModule->setup;
            setup->unk86 = mainModule->syncData.unk18;
            setup->rand = mainModule->syncData.rand;
            mainModule->rand = mainModule->syncData.rand;
            mainModule->unk473_2 = mainModule->syncData.unk1F_0;
            mainModule->unk43C = mainModule->syncData.unk1E;
            mainModule->unk440 = mainModule->syncData.unk1A;
            mainModule->unk442 = mainModule->syncData.unk1C;
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL func_ov093_021b7758(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u32 i;
    u8 index;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    switch (*state) {
    case 0:
        func_ov093_021d4af4(2);
        (*state)++;
        break;
    case 1:
        if (func_ov093_021d4b18(2)) {
            (*state)++;
        }
        break;
    case 2:
        if (func_ov093_021d4438(setup->party[0])) {
            (*state)++;
        }
        break;
    case 3:
        if (func_ov093_021d44cc()) {
            if (mainModule->unk471 == 0) {
                *state = 10;
                break;
            }
            if (mainModule->unk46E) {
                mainModule->unk2C4 =
                    func_ov093_021d4d24(PokeParty_GetSaveDataSize(), HEAPID_TAIL(mainModule->heapId));
            }
            mainModule->unk470 = 0;
            (*state)++;
        }
        break;
    case 4:
        index = 1;
        if (mainModule->unk470 != 0) {
            index = 3;
        }
        mainModule->unk472 = index;
        if (mainModule->unk46E) {
            func_ov093_021d4d64(mainModule->unk2C4, setup->party[mainModule->unk472], mainModule->unk472);
        }
        func_ov093_021d4af4(mainModule->unk470 + 6);
        (*state)++;
        break;
    case 5:
        if (func_ov093_021d4b18(mainModule->unk470 + 6)) {
            (*state)++;
        }
        break;
    case 6:
        if (!mainModule->unk46E || func_ov093_021d4680(mainModule->unk2C4)) {
            (*state)++;
        }
        break;
    case 7:
        if (func_ov093_021d4784(mainModule->unk472)) {
            mainModule->unk470++;
            if (mainModule->unk470 >= mainModule->unk471) {
                if (mainModule->unk46E) {
                    func_ov093_021d4d5c(mainModule->unk2C4);
                    mainModule->unk2C4 = NULL;
                }
                *state = 10;
            } else {
                *state = 4;
            }
        }
        break;
    case 10:
        func_ov093_021b93e0(&mainModule->pokeCons[0], mainModule, FALSE);
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021ba128(mainModule, i, func_ov093_021d47a4(i));
                func_ov093_021ba648(mainModule, i, func_ov093_021d47a4(i));
                func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, i);
            }
        }
        (*state)++;
        break;
    case 11:
        func_ov093_021b93e0(&mainModule->pokeCons[1], mainModule, TRUE);
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, i);
            }
        }
        (*state)++;
        break;
    case 12:
        func_ov093_021d4800();
        (*state)++;
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021b7968(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u32 i;
    PlayerInfo *info;
    MsgData *msgData;
    StrBuf *strBuf;
    const u16 *str;
    s32 j;
    u16 name[16];
    u8 index;
    StrBuf *newName;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    switch (*state) {
    case 0:
        func_ov093_021d4af4(4);
        (*state)++;
        break;
    case 1:
        if (func_ov093_021d4b18(4)) {
            (*state)++;
        }
        break;
    case 2:
        if (func_ov093_021d4808(mainModule->unkC0)) {
            (*state)++;
        }
        break;
    case 3:
        if (func_ov093_021d4898()) {
            for (i = 0; i < mainModule->clientCount; i++) {
                info = func_ov093_021d48cc(i);
                if (info != NULL) {
                    func_ov093_021b9e38(&mainModule->trainers[i], mainModule->heapId, info);
                    if (i != mainModule->playerClientId && setup->unkC1_2) {
                        msgData = GFL_MsgSysLoadData(0, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, mainModule->heapId);
                        strBuf = GFL_MsgDataLoadStrbufNew(msgData, AbilityHandlersBtlMain_Text_VersionName);
                        GFL_MsgDataFree(msgData);
                        str = GFL_StrBufGetStringPtr(strBuf);
                        for (j = 0; j < 15 && j < GFL_StrBufGetCharCount(strBuf); j++) {
                            name[j] = str[j];
                        }
                        name[j] = GFL_StrBufGetTerminator();
                        GFL_StrBufFree(strBuf);
                        copyTrainerName(mainModule->trainers[i].playerInfo, name);
                        GFL_StrBufFree(mainModule->trainers[i].name);
                        msgData = GFL_MsgSysLoadData(0, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, mainModule->heapId);
                        newName = GFL_MsgDataLoadStrbufNew(msgData, AbilityHandlersBtlMain_Text_VersionName);
                        GFL_MsgDataFree(msgData);
                        mainModule->trainers[i].name = newName;
                    }
                    func_ov093_021ba670(mainModule, i, mainModule->trainers[i].playerInfo);
                }
            }
            (*state)++;
        }
        break;
    case 4:
        if (mainModule->unk471 == 0) {
            *state = 10;
            break;
        }
        mainModule->unk470 = 0;
        (*state)++;
        break;
    case 5:
        func_ov093_021d4af4(mainModule->unk470 + 8);
        (*state)++;
        break;
    case 6:
        if (func_ov093_021d4b18(mainModule->unk470 + 8)) {
            (*state)++;
        }
        break;
    case 7:
        index = 1;
        if (mainModule->unk470 != 0) {
            index = 3;
        }
        mainModule->unk472 = index;
        if (mainModule->unk46E && !func_ov093_021d4908(setup->trainers[mainModule->unk472])) {
            break;
        }
        (*state)++;
    case 8:
        if (func_ov093_021d49b0()) {
            func_ov093_021b9ef0(&mainModule->trainers[mainModule->unk472], func_ov093_021d49cc());
            func_ov093_021d49e0();
            mainModule->unk470++;
            if (mainModule->unk470 >= mainModule->unk471) {
                *state = 10;
            } else {
                *state = 5;
            }
        }
        break;
    case 10:
        func_ov093_021d4a04();
        (*state)++;
        break;
    default:
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov093_021b7bb0(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u32 i;
    void *chatter;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    switch (*state) {
    case 0:
        func_ov093_021d4500();
        func_ov093_021d4af4(3);
        (*state)++;
        break;
    case 1:
        if (func_ov093_021d4b18(3)) {
            (*state)++;
        }
        break;
    case 2:
        if (func_ov093_021d450c(getChatterBlockAddress(GameData_GetSaveControl(setup->gameData)))) {
            (*state)++;
        }
        break;
    case 3:
        if (func_ov093_021d45fc()) {
            for (i = 0; i < 4; i++) {
                if (DoesClientExist(mainModule, i) && !func_ov093_021b9f70(mainModule, i)) {
                    chatter = func_ov093_021d4648(i);
                    if (chatter != NULL) {
                        mainModule->unk3E0[i] = allocChatotChatterBlk(mainModule->heapId);
                        MI_CpuCopy8(chatter, mainModule->unk3E0[i], func_02007b00());
                    }
                }
                func_ov093_021ba698(mainModule, i);
            }
            func_ov093_021d4674();
            (*state)++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021b7cb4(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u8 clientId = mainModule->playerClientId;
    u32 unk = func_ov093_021b6818(setup);
    u32 opponent;
    u32 i;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    if (mainModule->unk46E) {
        mainModule->server =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        mainModule->clients[clientId] =
            func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], setup->fieldSituation.unk18,
                                setup->fieldSituation.netHandle, clientId, 1, 0, unk, 0, &mainModule->rand, mainModule->heapId);
        opponent = 1;
        func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[clientId]), clientId, 1);
        if (clientId != 0) {
            opponent = 0;
        }
        func_ov093_021baacc(mainModule->server, setup->fieldSituation.unk18, setup->fieldSituation.netHandle, opponent, 1);
    } else {
        mainModule->clients[clientId] =
            func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], setup->fieldSituation.unk18,
                                setup->fieldSituation.netHandle, clientId, 1, 0, unk, 0, &mainModule->rand, mainModule->heapId);
        mainModule->unk0C =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021bab10(mainModule->unk0C, i, 1);
            }
        }
        func_ov093_021cd960(mainModule->clients[clientId], mainModule->unk0C);
    }
    return TRUE;
}

BOOL func_ov093_021b7e18(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u8 clientId = mainModule->playerClientId;
    u8 unk = func_ov093_021b6818(setup);
    u8 posCount = setup->fieldSituation.unk1a == 0 ? 2 : 1;
    u8 ally;
    u32 i;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    if (mainModule->unk46E) {
        mainModule->server =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        mainModule->clients[clientId] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0],
                                                            setup->fieldSituation.unk18, setup->fieldSituation.netHandle,
                                                            clientId, posCount, 0, unk, 0, &mainModule->rand,
                                                            mainModule->heapId);
        func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[clientId]), clientId, posCount);
        if (setup->fieldSituation.unk1a == 2) {
            ally = func_ov093_021b9010(mainModule, clientId);
            mainModule->clients[1] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0],
                                                         setup->fieldSituation.unk18, setup->fieldSituation.netHandle, 1,
                                                         posCount, 1, unk, 0, &mainModule->rand, mainModule->heapId);
            func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[1]), 1, posCount);
            mainModule->clients[3] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0],
                                                         setup->fieldSituation.unk18, setup->fieldSituation.netHandle, 3,
                                                         posCount, 1, unk, 0, &mainModule->rand, mainModule->heapId);
            func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[3]), 3, posCount);
            func_ov093_021baacc(mainModule->server, setup->fieldSituation.unk18, setup->fieldSituation.netHandle, ally,
                                posCount);
        } else {
            for (i = 0; i < mainModule->clientCount; i++) {
                if (i != clientId) {
                    func_ov093_021baacc(mainModule->server, setup->fieldSituation.unk18, setup->fieldSituation.netHandle,
                                        i, posCount);
                }
            }
        }
    } else {
        mainModule->clients[clientId] = func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0],
                                                            setup->fieldSituation.unk18, setup->fieldSituation.netHandle,
                                                            clientId, posCount, 0, unk, 0, &mainModule->rand,
                                                            mainModule->heapId);
        mainModule->unk0C =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021bab10(mainModule->unk0C, i, func_ov093_021b6994(mainModule, i));
            }
        }
        func_ov093_021cd960(mainModule->clients[clientId], mainModule->unk0C);
    }
    return TRUE;
}

BOOL func_ov093_021b8070(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u8 clientId = mainModule->playerClientId;
    u32 unk = func_ov093_021b6818(setup);
    u32 opponent;
    u32 i;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    if (mainModule->unk46E) {
        mainModule->server =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        mainModule->clients[clientId] =
            func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], setup->fieldSituation.unk18,
                                setup->fieldSituation.netHandle, clientId, 3, 0, unk, 0, &mainModule->rand, mainModule->heapId);
        opponent = 0;
        func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[clientId]), clientId, 3);
        if (clientId == 0) {
            opponent = 1;
        }
        func_ov093_021baacc(mainModule->server, setup->fieldSituation.unk18, setup->fieldSituation.netHandle, opponent, 3);
    } else {
        mainModule->clients[clientId] =
            func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], setup->fieldSituation.unk18,
                                setup->fieldSituation.netHandle, clientId, 3, 0, unk, 0, &mainModule->rand, mainModule->heapId);
        mainModule->unk0C =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        for (i = 0; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021bab10(mainModule->unk0C, i, func_ov093_021b6994(mainModule, i));
            }
        }
        func_ov093_021cd960(mainModule->clients[clientId], mainModule->unk0C);
    }
    return TRUE;
}

BOOL func_ov093_021b81dc(BtlMainModule *mainModule, s32 *state) {
    BtlSetup *setup = mainModule->setup;
    u8 clientId = mainModule->playerClientId;
    u32 unk = func_ov093_021b6818(setup);
    u32 i;

    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    if (mainModule->unk46E) {
        mainModule->server =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        i = 0;
        mainModule->clients[clientId] =
            func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], setup->fieldSituation.unk18,
                                setup->fieldSituation.netHandle, clientId, 1, i, unk, i, &mainModule->rand, mainModule->heapId);
        func_ov093_021baa94(mainModule->server, func_ov093_021cd978(mainModule->clients[clientId]), clientId, 1);
        for (; i < mainModule->clientCount; i++) {
            if (i != clientId) {
                func_ov093_021baacc(mainModule->server, setup->fieldSituation.unk18, setup->fieldSituation.netHandle, i,
                                    1);
            }
        }
    } else {
        i = 0;
        mainModule->clients[clientId] =
            func_ov093_021cd6f0(mainModule, &mainModule->pokeCons[0], setup->fieldSituation.unk18,
                                setup->fieldSituation.netHandle, clientId, 1, i, unk, i, &mainModule->rand, mainModule->heapId);
        mainModule->unk0C =
            func_ov093_021ba9c8(mainModule, &mainModule->rand, &mainModule->pokeCons[1], unk, mainModule->heapId);
        for (; i < 4; i++) {
            if (DoesClientExist(mainModule, i)) {
                func_ov093_021bab10(mainModule->unk0C, i, func_ov093_021b6994(mainModule, i));
            }
        }
        func_ov093_021cd960(mainModule->clients[clientId], mainModule->unk0C);
    }
    return TRUE;
}

BOOL func_ov093_021b835c(BtlMainModule *mainModule, s32 *state) {
    if (func_ov093_021d4270()) {
        mainModule->unk473_0 = 1;
        return TRUE;
    }
    switch (*state) {
    case 0:
        mainModule->viewCore = BtlvCore_Create(mainModule, mainModule->clients[mainModule->playerClientId],
                                               &mainModule->pokeCons[0], func_ov093_021b6818(mainModule->setup), 0x13);
        func_ov093_021cd95c(mainModule->clients[mainModule->playerClientId], mainModule->viewCore);
        if (mainModule->unk46E) {
            mainModule->mainFunc = func_ov093_021b8434;
            func_ov093_021bab40(mainModule->server);
        } else {
            mainModule->mainFunc = func_ov093_021b84d8;
        }
        (*state)++;
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021b83f4(BtlMainModule *mainModule) {
    BOOL done = FALSE;
    s32 i;

    func_ov093_021bab9c(mainModule->server);
    for (i = 0; i < 4; i++) {
        if (mainModule->clients[i] != NULL && func_ov093_021cd97c(mainModule->clients[i])) {
            func_ov093_021cdd94(mainModule->clients[i], &mainModule->unk448);
            done = TRUE;
        }
    }
    func_ov093_021e908c(mainModule->viewCore);
    return done;
}

BOOL func_ov093_021b8434(BtlMainModule *mainModule) {
    BOOL done;
    s32 i;

    if (!mainModule->unk473_0) {
        if (func_ov093_021d4270()) {
            mainModule->unk473_0 = 1;
            func_ov093_021b8548(mainModule);
        }
    } else if (mainModule->unk473_1) {
        return TRUE;
    }
    done = func_ov093_021bab9c(mainModule->server);
    if (done) {
        func_ov093_021babdc(mainModule->server, &mainModule->unk448);
    }
    for (i = 0; i < 4; i++) {
        if (mainModule->clients[i] != NULL && mainModule->unkC4[i] == 0) {
            mainModule->unkC4[i] = func_ov093_021cd97c(mainModule->clients[i]);
        }
    }
    if (func_ov093_021b8564(mainModule)) {
        func_ov093_021cdd94(mainModule->clients[mainModule->playerClientId], &mainModule->unk448);
        done = TRUE;
    }
    func_ov093_021e908c(mainModule->viewCore);
    return done;
}

BOOL func_ov093_021b84d8(BtlMainModule *mainModule) {
    BOOL done;
    s32 i;

    if (!mainModule->unk473_0) {
        if (func_ov093_021d4270()) {
            mainModule->unk473_0 = 1;
            func_ov093_021b8548(mainModule);
        }
    } else if (mainModule->unk473_1) {
        return TRUE;
    }
    done = FALSE;
    for (i = 0; i < 4; i++) {
        if (mainModule->clients[i] != NULL && func_ov093_021cd97c(mainModule->clients[i])) {
            func_ov093_021cdd94(mainModule->clients[i], &mainModule->unk448);
            done = TRUE;
        }
    }
    func_ov093_021e908c(mainModule->viewCore);
    return done;
}

void func_ov093_021b8548(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (mainModule->clients[i] != NULL) {
            func_ov093_021cd914(mainModule->clients[i]);
        }
    }
}

BOOL func_ov093_021b8564(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (mainModule->clients[i] != NULL && mainModule->unkC4[i] == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// Layout reconstructed from the opponent-position lookup in the game code.
u32 BtlSetup_GetBattleStyle(BtlMainModule *mainModule) {
    return mainModule->setup->battleStyle;
}

u32 func_ov093_021b8590(BtlMainModule *mainModule) {
    return mainModule->unk473_2;
}

u8 func_ov093_021b85a0(BtlMainModule *mainModule) {
    return mainModule->setup->unk80;
}

BOOL IsSwitchMode(BtlMainModule *mainModule) {
    if (BtlSetup_GetBattleType(mainModule) == 1
        && BtlSetup_GetBattleStyle(mainModule) == 0
        && func_ov093_021b86c0(mainModule) == 0
        && func_020083c8(mainModule->setup->config) == 0) {
        return TRUE;
    }
    return FALSE;
}


s32 func_ov093_021b85dc(BtlMainModule *mainModule) {
    return func_02013080(mainModule->unk43C);
}


BOOL func_ov231_021809d8(void);

u32 func_ov093_021b85ec(BtlMainModule *mainModule) {
    BOOL result;
    if (mainModule->setup->battleType <= 1) {
        GFL_OvlLoad(OVERLAY_ID(231));
        result = func_ov231_021809d8();
        GFL_OvlUnload(OVERLAY_ID(231));
        if (result == FALSE) {
            return 0;
        }
        if (BtlSetup_IsBattleType(mainModule, 0x200) == FALSE) {
            return 1;
        }
    }
    return 0;
}


void *func_ov093_021b8630(BtlMainModule *mainModule) {
    if (mainModule->setup->battleType <= 1) {
        return mainModule->setup->unk70;
    }
    return NULL;
}


u32 GetValidPosMax(BtlMainModule *mainModule) {
    switch (mainModule->setup->battleStyle) {
    case 0:
        return 1;
    case 1:
        return 3;
    case 2:
        return 5;
    case 3:
        return 5;
    default:
        return 5;
    }
}

u32 func_ov093_021b8670(BtlMainModule *mainModule) {
    switch (mainModule->setup->battleStyle) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 3;
    case 3:
        return 1;
    default:
        return 1;
    }
}

BOOL func_ov093_021b86a0(BtlMainModule *mainModule, u32 pos) {
    return pos < func_ov093_021b8670(mainModule) * 2;
}

u32 BtlSetup_GetBattleType(BtlMainModule *mainModule) {
    return mainModule->setup->battleType;
}

u8 func_ov093_021b86c0(BtlMainModule *mainModule) {
    return mainModule->setup->fieldSituation.unk18;
}

BOOL func_ov093_021b86c8(BtlMainModule *mainModule) {
    return mainModule->setup->fieldSituation.unk1a != 0;
}

u16 func_ov093_021b86dc(BtlMainModule *mainModule) {
    return mainModule->setup->fieldSituation.unk12;
}

u32 GetRunMode(BtlMainModule *mainModule) {
    switch (mainModule->setup->battleType) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 2;
    default:
        return 1;
    }
}


BtlFieldSituation *GetFieldEffectData(BtlMainModule *mainModule) {
    return &mainModule->setup->fieldSituation;
}

PlayerInfo *func_ov093_021b871c(BtlMainModule *mainModule) {
    return mainModule->unkC0;
}

BOOL func_ov093_021b8724(BtlMainModule *mainModule, BattleMon *mon) {
    return PokeDex_IsCaught(mainModule->setup->pokedex, GetBattleMonSpecies(mon));
}

u32 func_ov093_021b873c(BtlMainModule *mainModule) {
    return PokeDex_GetCaughtNoNational(mainModule->setup->pokedex);
}

GameData *func_ov093_021b8748(BtlMainModule *mainModule) {
    return mainModule->setup->gameData;
}

void func_ov093_021b8750(BtlMainModule *mainModule, u8 clientId, BattleMon *mon) {
    if (BtlSetup_GetBattleType(mainModule) <= 1 && clientId != 0) {
        PokeDex_RegistPkm(mainModule->setup->pokedex, GetSrcData(mon));
    }
}


u8 func_ov093_021b8778(BtlMainModule *mainModule, u16 posFlags, u8 *out) {
    u8 type = posFlags >> 8;
    u8 pos = posFlags;

    if (type == 0) {
        *out = pos;
        return *out != 6 ? 1 : 0;
    }
    if (pos != 6) {
        switch (mainModule->setup->battleStyle) {
        case 0:
        default:
            return func_ov093_021b87e8(mainModule, type, pos, out);
        case 1:
            return func_ov093_021b8834(mainModule, type, pos, out);
        case 2:
            return func_ov093_021b88ec(mainModule, type, pos, out);
        case 3:
            return func_ov093_021b87e8(mainModule, type, pos, out);
        }
    }
    *out = 6;
    return 0;
}

u8 func_ov093_021b87e8(BtlMainModule *mainModule, u8 type, u8 pos, u8 *out) {
    switch (type) {
    default:
        return 0;
    case 1:
    case 2:
    case 6:
        *out = func_ov093_021b8c50(mainModule, pos, 0);
        break;
    case 3:
        *out = pos;
        break;
    case 5:
    case 8:
        out[0] = 0;
        out[1] = 1;
        return 2;
    }
    return 1;
}

u8 func_ov093_021b8834(BtlMainModule *mainModule, u8 type, u8 pos, u8 *out) {
    switch (type) {
    default:
        return 0;
    case 1:
    case 6:
        out[0] = func_ov093_021b8c50(mainModule, pos, 0);
        out[1] = func_ov093_021b8c50(mainModule, pos, 1);
        return 2;
    case 2:
        out[0] = func_ov093_021b8c50(mainModule, pos, 0);
        out[1] = func_ov093_021b8c50(mainModule, pos, 1);
        out[2] = func_ov093_021b8cb0(mainModule, pos);
        return 3;
    case 3:
        out[0] = pos;
        out[1] = func_ov093_021b8cb0(mainModule, pos);
        if (out[0] > out[1]) {
            u8 tmp = out[0];

            out[0] = out[1];
            out[1] = tmp;
        }
        return 2;
    case 4:
    case 9:
        out[0] = func_ov093_021b8cb0(mainModule, pos);
        return 1;
    case 7:
        out[0] = pos;
        out[1] = func_ov093_021b8cb0(mainModule, pos);
        return 2;
    case 5:
    case 8:
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        return 4;
    }
}

u8 func_ov093_021b88ec(BtlMainModule *mainModule, u8 type, u8 pos, u8 *out) {
    u8 isCenter = func_ov093_021b99f0(pos);
    u8 index;

    switch (type) {
    default:
    case 0:
    case 1:
        if (isCenter) {
            out[0] = func_ov093_021b8c50(mainModule, pos, 0);
            out[1] = func_ov093_021b8c50(mainModule, pos, 1);
            out[2] = func_ov093_021b8c50(mainModule, pos, 2);
            return 3;
        }
        out[0] = func_ov093_021b8c50(mainModule, pos, 1);
        index = pos >> 1;
        if (index == 0) {
            index = 2;
        } else if (index == 2) {
            index = 0;
        }
        out[1] = func_ov093_021b8c20(2, pos, index);
        return 2;
    case 6:
        out[0] = func_ov093_021b8c50(mainModule, pos, 0);
        out[1] = func_ov093_021b8c50(mainModule, pos, 1);
        out[2] = func_ov093_021b8c50(mainModule, pos, 2);
        return 3;
    case 7:
        out[0] = func_ov093_021b8c38(pos, 0);
        out[1] = func_ov093_021b8c38(pos, 1);
        out[2] = func_ov093_021b8c38(pos, 2);
        return 3;
    case 8:
        out[0] = func_ov093_021b8c50(mainModule, pos, 0);
        out[1] = func_ov093_021b8c50(mainModule, pos, 1);
        out[2] = func_ov093_021b8c50(mainModule, pos, 2);
        out[3] = func_ov093_021b8c38(pos, 0);
        out[4] = func_ov093_021b8c38(pos, 1);
        out[5] = func_ov093_021b8c38(pos, 2);
        return 6;
    case 2:
        if (isCenter) {
            out[0] = func_ov093_021b8c50(mainModule, pos, 0);
            out[1] = func_ov093_021b8c50(mainModule, pos, 1);
            out[2] = func_ov093_021b8c50(mainModule, pos, 2);
            out[3] = func_ov093_021b8c38(pos, 0);
            out[4] = func_ov093_021b8c38(pos, 2);
            return 5;
        }
        out[0] = func_ov093_021b8c50(mainModule, pos, 1);
        index = pos >> 1;
        if (index == 0) {
            index = 2;
        } else if (index == 2) {
            index = 0;
        }
        out[1] = func_ov093_021b8c20(2, pos, index);
        out[2] = func_ov093_021b8c38(pos, 1);
        return 3;
    case 3:
        if (isCenter) {
            out[0] = pos;
            out[1] = func_ov093_021b8c38(pos, 0);
            out[2] = func_ov093_021b8c38(pos, 2);
            return 3;
        }
        out[0] = pos;
        out[1] = func_ov093_021b8c38(pos, 1);
        return 2;
    case 9:
        if (isCenter) {
            out[0] = func_ov093_021b8c38(pos, 0);
            out[1] = func_ov093_021b8c38(pos, 2);
            return 2;
        }
        out[0] = func_ov093_021b8c38(pos, 1);
        return 1;
    case 4:
        if (isCenter) {
            out[0] = func_ov093_021b8c38(pos, 0);
            out[1] = func_ov093_021b8c38(pos, 2);
            return 2;
        }
        out[0] = func_ov093_021b8c38(pos, 1);
        return 1;
    case 5:
        if (isCenter) {
            out[0] = func_ov093_021b8c50(mainModule, pos, 0);
            out[1] = func_ov093_021b8c50(mainModule, pos, 1);
            out[2] = func_ov093_021b8c50(mainModule, pos, 2);
            out[3] = func_ov093_021b8c38(pos, 0);
            out[4] = func_ov093_021b8c38(pos, 2);
            out[5] = pos;
            return 6;
        }
        out[0] = func_ov093_021b8c50(mainModule, pos, 1);
        index = pos >> 1;
        if (index == 0) {
            index = 2;
        } else if (index == 2) {
            index = 0;
        }
        out[1] = func_ov093_021b8c20(2, pos, index);
        out[2] = func_ov093_021b8c38(pos, 1);
        out[3] = pos;
        return 4;
    }
}

u32 func_ov093_021b8b78(BtlMainModule *mainModule, u8 clientId) {
    u32 count;
    u32 i;

    for (i = 0, count = 0; i < BTL_POS_MAX; i++) {
        if (clientId == mainModule->posClientIds[i]) {
            count++;
        }
    }
    return count;
}

BOOL DoesClientExist(BtlMainModule *mainModule, u8 clientId) {
    u32 i;

    if (clientId < 4) {
        for (i = 0; i < 6; i++) {
            if (mainModule->posClientIds[i] == clientId) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

u8 GetClientSide(BtlMainModule *mainModule, u8 clientId) {
    return clientId & 1;
}

BOOL func_ov093_021b8bd0(BtlMainModule *mainModule, u8 side) {
    if (side == GetClientSide(mainModule, mainModule->playerClientId)) {
        return TRUE;
    }
    return FALSE;
}

u8 func_ov093_021b8bec(BtlMainModule *mainModule, u8 clientId, u8 slot) {
    u8 pos;

    for (pos = 0; pos < BTL_POS_MAX; pos++) {
        if (clientId == mainModule->posClientIds[pos]) {
            if (slot == 0) {
                return pos;
            }
            slot--;
        }
    }
    return BTL_POS_MAX;
}

u8 func_ov093_021b8c20(u32 battleStyle, u8 pos, u8 index) {
    if ((pos & 1) == 0) {
        return index * 2 + 1;
    }
    return index * 2;
}

// pokebw2's GetPosOnSameSide
u8 func_ov093_021b8c38(u8 pos, u8 index) {
    if ((pos & 1) == 0) {
        return index * 2;
    }
    return index * 2 + 1;
}

u8 func_ov093_021b8c50(BtlMainModule *mainModule, u8 pos, u8 index) {
    return func_ov093_021b8c20(mainModule->setup->battleStyle, pos, index);
}

u8 func_ov093_021b8c5c(u32 battleStyle, u8 pos) {
    u8 index;

    switch (battleStyle) {
    default:
    case 0:
        return func_ov093_021b8c20(battleStyle, pos, 0);
    case 1:
        return func_ov093_021b8c20(battleStyle, pos, (u8)(pos >> 1) ^ 1);
    case 2:
        index = pos >> 1;
        if (index == 0) {
            index = 2;
        } else if (index == 2) {
            index = 0;
        }
        return func_ov093_021b8c20(battleStyle, pos, index);
    case 3:
        return func_ov093_021b8c20(battleStyle, pos, 0);
    }
}

u8 func_ov093_021b8cb0(BtlMainModule *mainModule, u8 pos) {
    switch (mainModule->setup->battleStyle) {
    case 0:
        return pos;
    case 3:
        return pos;
    case 1:
        return (pos + 2) & 3;
    case 2:
        return pos;
    default:
        return pos;
    }
}

// pokebw2's AreClientsOnOppositeSides
BOOL func_ov093_021b8ce8(BtlMainModule *mainModule, u8 clientId1, u8 clientId2) {
    return (clientId1 & 1) != (clientId2 & 1);
}

u8 BattlePosToClientID(BtlMainModule *mainModule, u8 pos) {
    return mainModule->posClientIds[pos];
}

u8 MonIDToClientID(u8 monId) {
    u8 i;
    u8 first;
    u8 last;

    for (i = 0; i < 4; i++) {
        first = data_ov093_021efcf0[i];
        last = first + 5;
        if (monId >= first && monId <= last) {
            return i;
        }
    }
    return 0;
}

u8 func_ov093_021b8d38(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u16 posFlags, u8 *monIds) {
    u8 positions[8];
    u8 count;
    u8 found;
    u8 i;
    BattleMon *mon;

    count = func_ov093_021b8778(mainModule, posFlags, positions);
    for (i = 0, found = 0; i < count; i++) {
        mon = func_ov093_021b98a4(pokeCon, positions[i]);
        if (mon != NULL && !IsFainted(mon)) {
            monIds[found++] = GetMonID(mon);
        }
    }
    return found;
}

u8 MonIDToBattlePos(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 monId) {
    u8 clientId;
    s32 slot;
    u8 pos;

    clientId = MonIDToClientID(monId);
    slot = func_ov093_021b9864(pokeCon, clientId, monId);
    if (slot >= 0) {
        pos = func_ov093_021b8bec(mainModule, clientId, slot);
        if (pos != 6) {
            return pos;
        }
    }
    return 6;
}

u8 func_ov093_021b8dc0(BtlMainModule *mainModule, BtlPokeCon *pokeCon, u8 monId) {
    u8 pos = MonIDToBattlePos(mainModule, pokeCon, monId);

    if (pos != BTL_POS_MAX) {
        return func_ov093_021b8e70(mainModule, pos);
    }
    return 0xff;
}

u8 func_ov093_021b8ddc(u8 monId) {
    return MonIDToClientID(monId);
}

u8 func_ov093_021b8de4(BtlMainModule *mainModule, u8 pos) {
    return BattlePosToClientID(mainModule, pos);
}

// How many positions before pos belong to the same client
static inline u8 CountClientPosBefore(BtlMainModule *mainModule, u8 pos) {
    u8 clientId = BattlePosToClientID(mainModule, pos);
    u8 count = 0;

    while (pos-- != 0) {
        if (clientId == mainModule->posClientIds[pos]) {
            count++;
        }
    }
    return count;
}

u8 func_ov093_021b8dec(BtlMainModule *mainModule, u8 pos) {
    return CountClientPosBefore(mainModule, pos);
}

void func_ov093_021b8e28(BtlMainModule *mainModule, u8 pos, u8 *clientId, u8 *index) {
    u8 i = pos;
    u8 count;

    *clientId = BattlePosToClientID(mainModule, pos);
    count = 0;
    while (i-- != 0) {
        if (*clientId == mainModule->posClientIds[i]) {
            count++;
        }
    }
    *index = count;
}

u8 func_ov093_021b8e70(BtlMainModule *mainModule, u8 pos) {
    u32 sameSide = TRUE;
    u8 side;
    u32 battleStyle;

    if ((mainModule->unk46D & 1) != (pos & 1)) {
        sameSide = FALSE;
    }
    side = sameSide;
    battleStyle = mainModule->setup->battleStyle;
    if (battleStyle == 0) {
        u32 result = 0;

        if (side == 0) {
            result = 1;
        }
        return result;
    }
    if (battleStyle == 1) {
        return data_ov093_021efcf4[side][(u8)(pos >> 1)];
    }
    return data_ov093_021efcfa[side][(u8)(pos >> 1)];
}

u8 func_ov093_021b8ed8(BtlMainModule *mainModule, u8 index) {
    u8 isEnemy = index & 1;
    u8 playerSide;

    if (mainModule->setup->battleStyle == 0) {
        if (isEnemy) {
            return mainModule->unk46D == 0 ? 1 : 0;
        }
        return mainModule->unk46D;
    }
    playerSide = mainModule->unk46D & 1;
    if (isEnemy) {
        return (playerSide ^ 1) + (index - 3);
    }
    return playerSide + (index - 2);
}

BOOL func_ov093_021b8f38(BtlMainModule *mainModule) {
    if (func_ov093_021b86c8(mainModule)) {
        u32 battleType = BtlSetup_GetBattleType(mainModule);

        if (battleType != 1 && battleType != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

PokeParty *func_ov093_021b8f5c(BtlMainModule *mainModule, u8 clientId) {
    func_ov093_021b969c(&mainModule->pokeCons[0], mainModule, clientId, FALSE);
    return func_ov093_021b985c(&mainModule->pokeCons[0], clientId);
}

PokeParty *func_ov093_021b8f7c(BtlMainModule *mainModule, u8 clientId) {
    if (func_ov093_021b86c8(mainModule)) {
        clientId = (clientId + 2) & 3;
        if (DoesClientExist(mainModule, clientId)) {
            func_ov093_021b969c(&mainModule->pokeCons[0], mainModule, clientId, FALSE);
            return func_ov093_021b985c(&mainModule->pokeCons[0], clientId);
        }
    }
    return NULL;
}

u8 func_ov093_021b8fc0(BtlMainModule *mainModule, u8 clientId) {
    if (func_ov093_021b86c8(mainModule)) {
        return (clientId & 2) ? 1 : 0;
    }
    return 0;
}

u8 func_ov093_021b8fe4(BtlMainModule *mainModule) {
    return func_ov093_021b8fc0(mainModule, mainModule->playerClientId);
}

u8 GetPlayerClientID(BtlMainModule *mainModule) {
    return mainModule->playerClientId;
}

u8 func_ov093_021b9000(BtlMainModule *mainModule) {
    return func_ov093_021b9010(mainModule, mainModule->playerClientId);
}

u8 func_ov093_021b9010(BtlMainModule *mainModule, u8 clientId) {
    u8 allyId = (clientId + 2) & 3;

    if (!DoesClientExist(mainModule, allyId)) {
        allyId = 4;
    }
    return allyId;
}

BOOL IsAllyClientID(u8 clientId1, u8 clientId2) {
    if (clientId1 == clientId2) {
        return TRUE;
    }
    if (clientId1 == (u8)((clientId2 + 2) & 3)) {
        return TRUE;
    }
    return FALSE;
}

u8 func_ov093_021b904c(BtlMainModule *mainModule, u8 other) {
    return func_ov093_021b9064(mainModule, mainModule->playerClientId, other);
}

u32 func_ov093_021b9064(BtlMainModule *mainModule, u8 clientId, u8 other) {
    u8 oppositeSide = clientId;
    oppositeSide &= 1;
    oppositeSide ^= 1;

    if (other != 0) {
        u8 candidate = oppositeSide + ((other & 1) << 1);
        if (DoesClientExist(mainModule, candidate)) {
            oppositeSide = candidate;
        }
    }
    return oppositeSide;
}

void BattleClient_SubItem(BtlMainModule *mainModule, u8 clientId, u16 item) {
    BtlSetup *setup;

    setup = mainModule->setup;
    if (setup->fieldSituation.unk1b == 0 && clientId == mainModule->playerClientId) {
        BagSave_SubItem(setup->bag, item, 1, mainModule->heapId);
    }
}

void BattleClient_AddItem(BtlMainModule *mainModule, u8 clientId, u16 item) {
    BtlSetup *setup;

    setup = mainModule->setup;
    if (setup->fieldSituation.unk1b == 0 && clientId == mainModule->playerClientId) {
        BagSave_AddItem(setup->bag, item, 1, mainModule->heapId);
    }
}

BagSave *func_ov093_021b90e8(BtlMainModule *mainModule) {
    return mainModule->setup->bag;
}

void *func_ov093_021b90f0(BtlMainModule *mainModule) {
    return mainModule->setup->unk64;
}

BOOL func_ov093_021b90f8(BtlMainModule *mainModule) {
    BtlSetup *setup = mainModule->setup;
    if (setup->unk94 != NULL && setup->fieldSituation.unk1b == 0) {
        return TRUE;
    }
    return FALSE;
}



u8 func_ov093_021b9114(BtlMainModule *mainModule) {
    return mainModule->setup->fieldSituation.unk1b;
}

u32 ReturnZero(BtlMainModule *mainModule, u32 flag) {
    return 0;
}

void func_ov093_021b9120(BtlMainModule *mainModule, u8 battlePos) {
    u8 i;
    u8 clientId;
    u8 count;
    if (mainModule->setup->unk90 == 6) {
        i = battlePos;
        clientId = BattlePosToClientID(mainModule, battlePos);
        count = 0;
        while (i-- != 0) {
            if (clientId == mainModule->posClientIds[i]) {
                count++;
            }
        }
        mainModule->setup->unk90 = count;
    }
}


void func_ov093_021b916c(BtlMainModule *mainModule, u32 result) {
    if (mainModule->result == 7) {
        mainModule->result = result;
    }
}

u32 func_ov093_021b917c(BtlMainModule *mainModule) {
    return func_ov093_021ba54c(mainModule);
}

void func_ov093_021b9184(BtlMainModule *mainModule) {
    mainModule->setup->unkC1_0 = 1;
}


u32 func_ov093_021b919c(BtlMainModule *mainModule) {
    if (!mainModule->unk473_3) {
        if (mainModule->unk473_6) {
            mainModule->prizeMoney *= 2;
        }
        mainModule->prizeMoney = PassPower_ApplyPrizeMoney(mainModule->prizeMoney);
        mainModule->unk473_3 = 1;
    }
    return mainModule->prizeMoney;
}

void func_ov093_021b91e4(BtlMainModule *mainModule, u32 money) {
    if (!mainModule->unk473_3) {
        mainModule->money += money;
        if (mainModule->money > 99999) {
            mainModule->money = 99999;
        }
    }
}

u32 func_ov093_021b9210(BtlMainModule *mainModule) {
    u32 money = mainModule->money;

    if (mainModule->unk473_6) {
        money *= 2;
        if (money > 99999) {
            money = 99999;
        }
    }
    return money;
}

void func_ov093_021b9234(BtlMainModule *mainModule) {
    mainModule->unk473_6 = 1;
}

u32 func_ov093_021b9244(BtlMainModule *mainModule) {
    u32 loss = 0;
    u32 cash;
    if (!mainModule->unk473_4) {
        if (mainModule->setup->battleType <= 1) {
            cash = getCash(getTrainerCardDataBlkAddress(mainModule->setup->gameData));
            loss = func_ov093_021d7ff8(mainModule->setup->unk80,
                func_ov093_021b9af8(&mainModule->pokeCons[1], mainModule->playerClientId));
            if (loss > cash) {
                loss = cash;
            }
            mainModule->unk438 = loss;
        }
        mainModule->unk473_4 = 1;
    }
    return loss;
}


void func_ov093_021b92a0(BtlMainModule *mainModule) {
    mainModule->unk473_5 = 1;
}

void ChangeFriendshipWhenFainted(BtlMainModule *mainModule, BattleMon *mon, BOOL reason) {
    if (mainModule->setup->battleType <= 1 && mainModule->setup->fieldSituation.unk1b == 0) {
        ChangeFriendship(mainModule, mon, reason ? 5 : 4);
    }
}

void ChangeFriendship(BtlMainModule *mainModule, BattleMon *mon, u32 reason) {
    u8 monId;
    const BattleMon *param1;
    const BattleMon *param2;
    PartyPkm *src1;
    PartyPkm *src2;
    const BtlFieldSituation *field;

    monId = GetMonID(mon);
    param1 = GetPokeParamConst(&mainModule->pokeCons[1], monId);
    param2 = GetPokeParamConst(&mainModule->pokeCons[0], monId);
    src1 = GetSrcData(param1);
    src2 = GetSrcData(param2);
    field = GetFieldEffectData(mainModule);
    FriendshipManagerCalc(src1, reason, field->env.zoneId, HEAPID_TAIL(mainModule->heapId));
    FriendshipManagerCalc(src2, reason, field->env.zoneId, HEAPID_TAIL(mainModule->heapId));
}

void func_ov093_021b9358(BtlMainModule *mainModule, u8 monId) {
    BattleMon *mon = GetPokeParam(&mainModule->pokeCons[1], monId);

    func_ov093_021d54f4(GetPokeParam(&mainModule->pokeCons[0], monId), mon);
}


BattleMon *GetIllusionDisguise(BtlMainModule *mainModule, BtlPokeCon *pokeCon, BattleMon *mon) {
    u8 clientId;
    PartyPkm *disguise;
    BattleParty *party;
    u32 count;
    u32 i;
    BattleMon *member;

    if (IsIllusionEnabled(mon)) {
        clientId = func_ov093_021b8ddc(GetMonID(mon));
        disguise = func_ov093_021d5864(mon);
        party = func_ov093_021b9af8(pokeCon, clientId);
        count = GetNumMonsInParty(party);
        for (i = 0; i < count; i++) {
            member = GetBattleMonFromParty(party, i);
            if (GetSrcData(member) == disguise) {
                return member;
            }
        }
    }
    return mon;
}


void func_ov093_021b93e0(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u32 unkE4) {
    u32 i;

    pokeCon->mainModule = mainModule;
    pokeCon->unkE4 = unkE4;
    for (i = 0; i < 4; i++) {
        func_ov093_021b9b40(&pokeCon->parties[i]);
    }
    for (i = 0; i < 24; i++) {
        pokeCon->mons[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        pokeCon->srcParties[i] = NULL;
    }
}

void func_ov093_021b9424(BtlPokeCon *pokeCon) {
    u32 i;

    for (i = 0; i < 24; i++) {
        if (pokeCon->mons[i] != NULL) {
            func_ov093_021d53b8(pokeCon->mons[i]);
            pokeCon->mons[i] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        func_ov093_021b9b40(&pokeCon->parties[i]);
    }
}

void func_ov093_021b9460(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u8 clientId) {
    PokeParty *srcParty;
    BattleParty *battleParty;
    u32 count;
    u8 monId;
    s32 i;
    s32 eligible;
    BattleMon *mon;
    PokeParty *party;
    u8 partyCount;
    u8 k;
    u16 species;
    PartyPkm *pkm;
    u8 firstMonId;

    srcParty = func_ov093_021ba178(mainModule, clientId, pokeCon->unkE4);
    battleParty = &pokeCon->parties[clientId];
    count = PokeParty_GetPkmCount(srcParty);
    firstMonId = data_ov093_021efcf0[clientId];
    pokeCon->srcParties[clientId] = srcParty;
    for (monId = firstMonId, i = 0; i < count; i++) {
        pkm = PokeParty_GetPkm(srcParty, i);
        pokeCon->mons[monId] = BattleMon_Create(pkm, monId, HEAPID_BATTLE);
        AddBattleMonToParty(battleParty, pokeCon->mons[monId]);
        monId++;
    }
    eligible = GetPartyPkmnEligibleForBattle(srcParty);
    if (eligible > 0) {
        for (monId = firstMonId, i = 0; i < eligible; monId++) {
            if (GetBattleMonStat(pokeCon->mons[monId], 0x11) == ABILITY_ILLUSION) {
                SetIllusionDisguise(pokeCon->mons[monId], PokeParty_GetPkm(srcParty, eligible));
            }
            i++;
        }
    }
    if (BtlSetup_GetBattleType(mainModule) == 0 && clientId == 1) {
        for (i = 0; i < count; firstMonId++) {
            mon = pokeCon->mons[firstMonId];
            if (GetBattleMonSpecies(mon) == SPECIES_ZOROARK && GetBattleMonStat(mon, 0x11) == ABILITY_ILLUSION) {
                party = func_ov093_021ba178(mainModule, 0, pokeCon->unkE4);
                species = 0;
                partyCount = PokeParty_GetPkmCount(party);
                for (k = 0; k < partyCount; k++) {
                    u16 partySpecies = PokeParty_GetParam(PokeParty_GetPkm(party, k), PKM_PARAM_SPECIES, NULL);

                    if (partySpecies == SPECIES_RAIKOU) {
                        species = SPECIES_ENTEI;
                        break;
                    }
                    if (partySpecies == SPECIES_ENTEI) {
                        species = SPECIES_SUICUNE;
                        break;
                    }
                    if (partySpecies == SPECIES_SUICUNE) {
                        species = SPECIES_RAIKOU;
                        break;
                    }
                }
                if (species != 0) {
                    if (mainModule->unk2C0 == NULL) {
                        mainModule->unk2C0 =
                            PokeParty_NewTempPkm(species, GetBattleMonStat(mon, 0xf), 0, mainModule->heapId);
                    }
                    SetIllusionDisguise(mon, mainModule->unk2C0);
                }
            }
            i++;
        }
    }
    func_ov093_021b9b60(battleParty);
}


s32 GetPartyPkmnEligibleForBattle(PokeParty *party) {
    s32 index;
    PartyPkm *pkm;

    for (index = PokeParty_GetPkmCount(party) - 1; index >= 0; index--) {
        pkm = PokeParty_GetPkm(party, index);
        if (PokeParty_GetParam(pkm, (PkmField)PKM_PARAM_IS_EGG, NULL) == 0 &&
            PokeParty_GetParam(pkm, (PkmField)PKM_PARAM_HP, NULL) != 0) {
            return index;
        }
    }
    return -1;
}

void func_ov093_021b9674(BtlPokeCon *pokeCon) {
    u32 i;

    for (i = 0; i < 24; i++) {
        if (pokeCon->mons[i] != NULL) {
            func_ov093_021d53b8(pokeCon->mons[i]);
            pokeCon->mons[i] = NULL;
        }
    }
}

void func_ov093_021b969c(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u8 clientId, BOOL keepBaseForm) {
    PokeParty *srcParty;
    BattleParty *party;
    u32 count;
    u8 disguiseSlots[6];
    u32 i;
    u32 j;
    BattleMon *mon;
    PartyPkm *disguise;
    PartyPkm *pkm;

    srcParty = func_ov093_021b985c(pokeCon, clientId);
    party = GetPartyData(pokeCon, clientId);
    count = PokeParty_GetPkmCount(srcParty);
    PokeParty_Init(mainModule->unk2B8);
    for (i = 0; i < 6; i++) {
        disguiseSlots[i] = 6;
    }
    for (i = 0; i < count; i++) {
        mon = func_ov093_021b9bf0(party, i);
        func_ov093_021d6b84(mon, keepBaseForm);
        PokeParty_AddPkm(mainModule->unk2B8, GetSrcData(mon));
        if (IsIllusionEnabled(mon)) {
            disguise = func_ov093_021d5864(mon);
            for (j = 0; j < count; j++) {
                if (disguise == GetSrcData(func_ov093_021b9bf0(party, j))) {
                    disguiseSlots[i] = j;
                    break;
                }
            }
        }
    }
    PokeParty_Copy(mainModule->unk2B8, srcParty);
    for (i = 0; i < count; i++) {
        pkm = PokeParty_GetPkm(srcParty, i);
        mon = func_ov093_021b9bf0(party, i);
        func_ov093_021d6c3c(mon, pkm);
        if (disguiseSlots[i] != 6) {
            SetIllusionDisguise(mon, PokeParty_GetPkm(srcParty, disguiseSlots[i]));
        }
    }
}

void func_ov093_021b97a0(BtlPokeCon *pokeCon, BtlMainModule *mainModule, u8 clientId) {
    PokeParty *srcParty;
    BattleParty *party;
    u32 count;
    u32 firstMonId;
    u32 i;
    u32 j;
    BattleMon *mon;

    srcParty = func_ov093_021b985c(pokeCon, clientId);
    party = GetPartyData(pokeCon, clientId);
    count = PokeParty_GetPkmCount(srcParty);
    PokeParty_Init(mainModule->unk2B8);
    firstMonId = data_ov093_021efcf0[clientId];
    for (i = 0; i < count; i++) {
        for (j = 0; j < count; j++) {
            mon = func_ov093_021b9bf0(party, j);
            if (firstMonId + i == GetMonID(mon)) {
                func_ov093_021d6b84(mon, TRUE);
                PokeParty_AddPkm(mainModule->unk2B8, GetSrcData(mon));
                if (func_ov093_021d67d0(mon)) {
                    mainModule->setup->unkC2[i] = 1;
                }
                break;
            }
        }
    }
    PokeParty_Copy(mainModule->unk2B8, srcParty);
}

PokeParty *func_ov093_021b985c(BtlPokeCon *pokeCon, u8 clientId) {
    return pokeCon->srcParties[clientId];
}

s32 func_ov093_021b9864(BtlPokeCon *pokeCon, u8 clientId, u8 monId) {
    BattleParty *parties = pokeCon->parties;
    u32 count = GetNumMonsInParty(&parties[clientId]);
    u32 i;

    for (i = 0; i < count; i++) {
        if (monId == GetMonID(GetBattleMonFromParty(&parties[clientId], i))) {
            return i;
        }
    }
    return -1;
}

BattleMon *func_ov093_021b98a4(BtlPokeCon *pokeCon, u8 pos) {
    return func_ov093_021b98ac(pokeCon, pos);
}

BattleMon *func_ov093_021b98ac(const BtlPokeCon *pokeCon, u8 pos) {
    u8 i = pos;
    u8 clientId;
    u8 index;
    BattleParty *parties;
    BtlMainModule *mainModule = pokeCon->mainModule;

    clientId = BattlePosToClientID(mainModule, pos);
    index = 0;
    while (i-- != 0) {
        if (clientId == mainModule->posClientIds[i]) {
            index++;
        }
    }
    parties = pokeCon->parties;
    if (index < GetNumMonsInParty(&parties[clientId])) {
        return GetBattleMonFromParty(&parties[clientId], index);
    }
    return NULL;
}

BattleMon *func_ov093_021b990c(BtlPokeCon *pokeCon, u8 clientId, u8 index) {
    return func_ov093_021b9bf0(&pokeCon->parties[clientId], index);
}

BattleMon *GetClientMonData(BtlPokeCon *pokeCon, u8 clientId, u8 monId) {
    return GetBattleMonFromParty(&pokeCon->parties[clientId], monId);
}

BattleMon *GetPokeParam(BtlPokeCon *pokeCon, u8 monId) {
    return pokeCon->mons[monId];
}

const BattleMon *GetPokeParamConst(const BtlPokeCon *pokeCon, u8 monId) {
    return pokeCon->mons[monId];
}

BOOL func_ov093_021b994c(BtlMainModule *mainModule, u32 index, void **out) {
    u8 clientId = func_ov093_021b8de4(mainModule, func_ov093_021b8ed8(mainModule, index));

    if (mainModule->unk3E0[clientId] != NULL) {
        *out = mainModule->unk3E0[clientId];
        return TRUE;
    }
    return FALSE;
}

u8 func_ov093_021b997c(BtlMainModule *mainModule, u8 clientId) {
    if (!func_ov093_021b9114(mainModule)) {
        if (mainModule->unk3E0[clientId] != NULL && doesChatotExist(mainModule->unk3E0[clientId])) {
            return func_02007c70(mainModule->unk3E0[clientId]);
        }
        return 0;
    }
    return mainModule->setup->unk44[clientId];
}

// pokebw2's GetClientBattlerCount
u8 func_ov093_021b99b8(BtlMainModule *mainModule, u8 clientId) {
    return func_ov093_021b6994(mainModule, clientId);
}

u8 func_ov093_021b99c0(BtlMainModule *mainModule, u8 clientId) {
    u8 count = func_ov093_021b6994(mainModule, clientId);

    if (BtlSetup_GetBattleStyle(mainModule) == 3) {
        count += 2;
    }
    return count;
}

const AdjacentOpponentData *func_ov093_021b99e0(u8 pos) {
    return &data_ov093_021efd70[pos];
}

BOOL func_ov093_021b99f0(u8 pos) {
    switch (pos) {
    case 2:
    case 3:
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL func_ov093_021b9a00(u8 pos, u8 *out) {
    if (!func_ov093_021b99f0(pos)) {
        *out = func_ov093_021b8c20(2, pos, pos >> 1);
        return TRUE;
    }
    return FALSE;
}

BOOL IsAllyMonID(u8 monId1, u8 monId2) {
    return GetSideFromMonID(monId1) == GetSideFromMonID(monId2);
}

u8 GetSideFromMonID(u8 monId) {
    BtlSide side = monId < 12 ? BTL_SIDE_1ST : BTL_SIDE_2ND;
    return side;
}

// Public function name from swan.
u8 GetSideFromOpposingMonID(u8 monId) {
    return func_ov093_021b9a5c(GetSideFromMonID(monId));
}

u8 func_ov093_021b9a5c(u8 side) {
    return side == 0;
}

// pokebw2's IsAdjacentOpponent
BOOL func_ov093_021b9a6c(u8 pos1, u8 pos2) {
    const AdjacentOpponentData *data;
    u32 i;

    data = func_ov093_021b99e0(pos1);
    for (i = 0; i < data->count1; i++) {
        if (pos2 == data->list1[i]) {
            return TRUE;
        }
    }
    for (i = 0; i < data->count2; i++) {
        if (pos2 == data->list2[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 func_ov093_021b9ab0(u32 pos) {
    switch (pos) {
    case 2:
        return 1;
    case 3:
        return 2;
    default:
        return 0;
    }
}

u32 func_ov093_021b9ac8(u32 pos) {
    switch (pos) {
    case 2:
        return 2;
    case 3:
        return 1;
    default:
        return 0;
    }
}

u8 func_ov093_021b9ae0(u8 pos) {
    return pos & 1;
}

BattleParty *GetPartyData(BtlPokeCon *pokeCon, u32 clientId) {
    return &pokeCon->parties[clientId];
}

BattleParty *func_ov093_021b9af8(BtlPokeCon *pokeCon, u32 clientId) {
    return &pokeCon->parties[clientId];
}

u8 func_ov093_021b9b04(BtlPokeCon *pokeCon, u32 clientId) {
    return GetAlivePartyCount(func_ov093_021b9af8(pokeCon, clientId));
}



void func_ov093_021b9b10(BtlMainModule *mainModule, u8 clientId, u8 value) {
    if (mainModule->unk46E != 0 && DoesClientExist(mainModule, clientId) && mainModule->clients[clientId] != NULL) {
        func_ov093_021cd9f4(mainModule->clients[clientId], value);
    }
}

void func_ov093_021b9b40(BattleParty *party) {
    s32 i;

    party->count = 0;
    for (i = 0; i < 6; i++) {
        party->mons[i] = NULL;
    }
}

void AddBattleMonToParty(BattleParty *party, BattleMon *mon) {
    party->mons[party->count++] = mon;
}

void func_ov093_021b9b60(BattleParty *party) {
    u32 i;
    u32 processed;

    processed = 0;
    i = 0;
    for (; processed < party->count; processed++) {
        if (CanPokemonBattle(party->mons[i])) {
            i++;
        } else {
            func_ov093_021b9c24(party, i);
        }
    }
}

u8 GetNumMonsInParty(BattleParty *party) {
    return party->count;
}

u8 GetAlivePartyCount(BattleParty *party) {
    s32 i;
    s32 count;

    i = 0;
    count = 0;
    for (; i < party->count; i++) {
        if (CanPokemonBattle(party->mons[i])) {
            count++;
        }
    }
    return count;
}

u8 func_ov093_021b9bc4(BattleParty *party, s32 index) {
    s32 i;
    u32 count;

    count = 0;
    for (i = index; i < party->count; i++) {
        if (CanPokemonBattle(party->mons[i])) {
            count++;
        }
    }
    return count;
}

BattleMon *func_ov093_021b9bf0(BattleParty *party, u8 index) {
    if (index < party->count) {
        return party->mons[index];
    }
    return NULL;
}

BattleMon *GetBattleMonFromParty(BattleParty *party, u8 index) {
    if (index < party->count) {
        return party->mons[index];
    }
    return 0;
}

void func_ov093_021b9c10(BattleParty *party, u32 first, u32 second) {
    BattleMon *mon;

    mon = party->mons[first];
    party->mons[first] = party->mons[second];
    party->mons[second] = mon;
}

void func_ov093_021b9c24(BattleParty *party, u8 index) {
    BattleMon *mon;

    mon = party->mons[index];
    for (; index < party->count - 1; index++) {
        party->mons[index] = party->mons[index + 1];
    }
    party->mons[index] = mon;
}

void func_ov093_021b9c50(BattleParty *party, u32 pos, BattleMon **oldOut, BattleMon **newOut) {
    BattleMon *first;
    BattleMon *next;
    BattleMon *other;
    u32 index1;
    u32 index2;

    if (pos != 0 && pos != 1) {
        first = party->mons[0];
        index1 = func_ov093_021b9ab0(pos);
        index2 = func_ov093_021b9ac8(pos);
        next = party->mons[index1];
        party->mons[0] = next;
        other = party->mons[index2];
        party->mons[index1] = other;
        party->mons[index2] = first;
        if (oldOut != NULL) {
            *oldOut = first;
        }
        if (newOut != NULL) {
            *newOut = party->mons[0];
        }
    }
}

s32 FindPartyMon(const BattleParty *party, BattleMon *mon) {
    s32 i;

    for (i = 0; i < party->count; i++) {
        if (party->mons[i] == mon) {
            return i;
        }
    }
    return -1;
}

s32 func_ov093_021b9cbc(const BattleParty *party, u32 monId) {
    s32 i;

    for (i = 0; i < party->count; i++) {
        if (GetMonID(party->mons[i]) == monId) {
            return i;
        }
    }
    return -1;
}

BattleMon *func_ov093_021b9ce8(const BattleParty *party) {
    s32 i;

    for (i = 0; i < party->count; i++) {
        if (CanPokemonBattle(party->mons[i])) {
            return party->mons[i];
        }
    }
    return NULL;
}

void func_ov093_021b9d10(BtlMainModule *mainModule, BattleParty *party, u8 clientId) {
    s32 last;
    s32 i;
    BattleMon *mon;

    for (last = party->count - 1; last > 0; last--) {
        if (CanPokemonBattle(party->mons[last])) {
            break;
        }
    }
    if (BtlSetup_GetBattleStyle(mainModule) != 3) {
        i = func_ov093_021b8b78(mainModule, clientId);
    } else {
        i = 3;
    }
    for (; i < party->count; i++) {
        mon = party->mons[i];
        if (GetBattleMonStat(mon, 0x11) == ABILITY_ILLUSION) {
            if (i < last) {
                SetIllusionDisguise(mon, GetSrcData(party->mons[last]));
            } else if (func_ov093_021d5864(mon) != mainModule->unk2C0) {
                func_ov093_021d5854(mon);
            }
        }
    }
    for (i = 0; i < party->count; i++) {
        IsIllusionEnabled(party->mons[i]);
    }
}

void func_ov093_021b9db8(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        mainModule->trainers[i].playerInfo = NULL;
        mainModule->trainers[i].unk0A = 0;
        mainModule->trainers[i].unk0C = 0;
    }
    for (i = 0; i < 4; i++) {
        mainModule->unk3E0[i] = NULL;
    }
}

void func_ov093_021b9de8(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (mainModule->trainers[i].playerInfo != NULL) {
            GFL_HeapFree(mainModule->trainers[i].playerInfo);
            mainModule->trainers[i].playerInfo = NULL;
        }
        if (mainModule->trainers[i].name != NULL) {
            GFL_HeapFree(mainModule->trainers[i].name);
            mainModule->trainers[i].name = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        if (mainModule->unk3E0[i] != NULL) {
            GFL_HeapFree(mainModule->unk3E0[i]);
            mainModule->unk3E0[i] = NULL;
        }
    }
}

void func_ov093_021b9e38(BtlTrainerData *trainer, HeapID heapId, PlayerInfo *src) {
    u16 trainerClass;
    trainer->playerInfo = func_0200846c(heapId);
    func_02008494(src, trainer->playerInfo);
    trainerClass = 0;
    trainer->unk0A = 0;
    if (getTrainerGender(trainer->playerInfo) != 0) {
        trainerClass = 1;
    }
    trainer->unk08 = trainerClass;
    trainer->name = copyTrainerNameToNewStrbuf(trainer->playerInfo->name, 0x12);
    func_0202428c(&trainer->unk18);
    func_0202428c(&trainer->unk20);
}

void func_ov093_021b9e7c(BtlTrainerData *trainer, const BtlSetupTrainer *src) {
    u32 i;
    trainer->playerInfo = NULL;
    if (src != NULL) {
        trainer->unk0A = src->trainerId;
        trainer->unk08 = src->trainerClass;
        trainer->name = GFL_StrBufClone(src->name, 0x12);
        trainer->unk0C = src->aiFlags;
        MI_CpuCopy8(src->items, trainer->unk10, 8);
        func_02024570(&trainer->unk18, &src->unk18);
        func_02024570(&trainer->unk20, &src->unk20);
    } else {
        trainer->unk0A = 0;
        trainer->unk08 = 0;
        trainer->name = NULL;
        for (i = 0; i < 4; i++) {
            trainer->unk10[i] = 0;
        }
        func_0202428c(&trainer->unk18);
        func_0202428c(&trainer->unk20);
    }
}

void func_ov093_021b9ef0(BtlTrainerData *trainer, const BtlCommTrainerData *src) {
    u32 i;

    trainer->playerInfo = NULL;
    if (src != NULL) {
        trainer->unk0A = src->trainerId;
        trainer->unk08 = src->trainerClass;
        trainer->name = GFL_StrBufCreate(0x20, HEAPID_BATTLE);
        GFL_StrBufCopyString(trainer->name, src->name, src->nameLength + 1);
        trainer->unk0C = src->aiFlags;
        MI_CpuCopy8(src->items, trainer->unk10, sizeof(trainer->unk10));
        func_02024570(&trainer->unk18, &src->unk18);
        func_02024570(&trainer->unk20, &src->unk20);
    } else {
        trainer->unk0A = 0;
        trainer->unk08 = 0;
        trainer->name = NULL;
        for (i = 0; i < 4; i++) {
            trainer->unk10[i] = 0;
        }
        func_0202428c(&trainer->unk18);
        func_0202428c(&trainer->unk20);
    }
}




BOOL func_ov093_021b9f70(BtlMainModule *mainModule, u8 clientId) {
    if (mainModule->trainers[clientId].playerInfo == NULL) {
        return TRUE;
    }
    return FALSE;
}

u16 func_ov093_021b9f84(BtlMainModule *mainModule, u8 clientId, u8 index) {
    if (func_ov093_021b9f70(mainModule, clientId)) {
        BtlTrainerData *trainer = &mainModule->trainers[clientId];

        if (index < 4) {
            return trainer->unk10[index];
        }
    }
    return 0;
}

StrBuf *func_ov093_021b9fac(BtlMainModule *mainModule, u8 clientId, u32 *trainerClass) {
    *trainerClass = mainModule->trainers[clientId].unk08;
    return mainModule->trainers[clientId].name;
}

u32 func_ov093_021b9fbc(BtlMainModule *mainModule, u8 clientId) {
    if (BtlSetup_GetBattleType(mainModule) == 0) {
        if (BtlSetup_IsBattleType(mainModule, 8)) {
            return 0x800;
        }
        if (BtlSetup_GetBattleStyle(mainModule) == 1) {
            return 0x80;
        }
    } else if (func_ov093_021b9f70(mainModule, clientId)) {
        return mainModule->trainers[clientId].unk0C;
    }
    return 0;
}

u16 func_ov093_021ba004(BtlMainModule *mainModule, u8 clientId) {
    if (func_ov093_021b9f70(mainModule, clientId)) {
        return mainModule->trainers[clientId].unk0A;
    }
    return 0;
}

u32 func_ov093_021ba020(BtlMainModule *mainModule, u8 clientId) {
    return mainModule->trainers[clientId].unk08;
}

PMSData *func_ov093_021ba02c(BtlMainModule *mainModule, u8 clientId, u32 which) {
    if (BtlSetup_GetBattleType(mainModule) == 2 && func_ov093_021b9f70(mainModule, clientId)) {
        if (which == 1) {
            return &mainModule->trainers[clientId].unk20;
        }
        return &mainModule->trainers[clientId].unk18;
    }
    return NULL;
}

PlayerInfo *func_ov093_021ba064(BtlMainModule *mainModule, u8 clientId) {
    if (func_ov093_021b9f70(mainModule, clientId)) {
        return NULL;
    }
    return mainModule->trainers[clientId].playerInfo;
}

PlayerInfo *func_0202e7dc(CommPlayerSupport *support);
PlayerInfo *func_ov093_021ba080(BtlMainModule *mainModule) {
    return func_0202e7dc(mainModule->setup->unk70);
}


BtlField *func_ov093_021ba08c(BtlMainModule *mainModule) {
    return mainModule->field;
}

PokeParty *PokeParty_Create(HeapID heapId);

void func_ov093_021ba094(BtlMainModule *mainModule) {
    u32 i;
    for (i = 0; i < 4; i++) {
        mainModule->unk298[i] = PokeParty_Create(0x12);
        mainModule->unk2A8[i] = PokeParty_Create(0x12);
    }
    mainModule->unk2B8 = PokeParty_Create(0x12);
}


void func_ov093_021ba0cc(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (mainModule->unk298[i] != NULL) {
            GFL_HeapFree(mainModule->unk298[i]);
            mainModule->unk298[i] = NULL;
        }
        if (mainModule->unk2A8[i] != NULL) {
            GFL_HeapFree(mainModule->unk2A8[i]);
            mainModule->unk2A8[i] = NULL;
        }
    }
    if (mainModule->unk2B8 != NULL) {
        GFL_HeapFree(mainModule->unk2B8);
        mainModule->unk2B8 = NULL;
    }
}

void func_ov093_021ba128(BtlMainModule *mainModule, u8 clientId, const PokeParty *party) {
    if (mainModule->unk298[clientId] != NULL) {
        PokeParty_Init(mainModule->unk298[clientId]);
        PokeParty_Init(mainModule->unk2A8[clientId]);
        PokeParty_Copy(party, mainModule->unk298[clientId]);
        PokeParty_Copy(party, mainModule->unk2A8[clientId]);
        if (PokeParty_GetPkmCount(mainModule->unk2A8[clientId]) != 0) {
            PokeParty_GetPkm(mainModule->unk2A8[clientId], 0);
        }
    }
}

PokeParty *func_ov093_021ba178(BtlMainModule *mainModule, u8 clientId, u32 useSecond) {
    if (useSecond) {
        if (PokeParty_GetPkmCount(mainModule->unk2A8[clientId]) != 0) {
            PokeParty_GetPkm(mainModule->unk2A8[clientId], 0);
        }
        return mainModule->unk2A8[clientId];
    }
    return mainModule->unk298[clientId];
}

u32 BtlSetup_IsBattleType(BtlMainModule *mainModule, u32 flag) {
    return BtlSetup_CheckFlag(mainModule->setup, flag);
}

void func_ov093_021ba1b4(BtlMainModule *mainModule, u32 record) {
    BtlSetup *setup = mainModule->setup;

    if (setup->fieldSituation.unk1b == 0) {
        RecordAddOne(setup->records, record);
    }
}

void func_ov093_021ba1cc(BtlMainModule *mainModule, u32 record, u32 value) {
    BtlSetup *setup = mainModule->setup;

    if (setup->fieldSituation.unk1b == 0) {
        RecordAdd(setup->records, record, value);
    }
}

void *func_ov093_021ba1e4(BtlMainModule *mainModule) {
    return mainModule->setup->unk78;
}

u8 func_ov093_021b6818(BtlSetup *setup);


BOOL func_ov093_021ba1ec(BtlMainModule *mainModule) {
    BOOL result = FALSE;
    u32 unk = func_ov093_021b6818(mainModule->setup);
    if (unk == 1) {
        if (mainModule->setup->unk7F != 0) {
            result = TRUE;
        }
    }
    return result;
}

BOOL func_ov093_021ba1ec(BtlMainModule *mainModule);

BOOL func_ov093_021ba20c(BtlMainModule *mainModule) {
    if (func_ov093_021b6818(mainModule->setup) == 1) {
        return func_ov093_021ba1ec(mainModule);
    }
    switch (BtlSetup_GetBattleType(mainModule)) {
    case 0:
    case 1:
        return TRUE;
    default:
        return FALSE;
    }
}



void func_ov093_021ba238(BtlMainModule *mainModule, BattleMon *mon) {
    ChangeFriendship(mainModule, mon, 0);
    func_ov093_021bb8bc(mainModule->server, mon);
}

void func_ov093_021ba250(BtlMainModule *mainModule, BattleMon *mon, u16 item) {
    u8 monId;
    const BattleMon *param1;
    const BattleMon *param2;
    PartyPkm *src1;
    PartyPkm *src2;
    const BtlFieldSituation *field;

    monId = GetMonID(mon);
    param1 = GetPokeParamConst(&mainModule->pokeCons[1], monId);
    param2 = GetPokeParamConst(&mainModule->pokeCons[0], monId);
    src1 = GetSrcData(param1);
    src2 = GetSrcData(param2);
    field = GetFieldEffectData(mainModule);
    func_0201b7b4(src1, item, field->env.zoneId, HEAPID_TAIL(mainModule->heapId));
    func_0201b7b4(src2, item, field->env.zoneId, HEAPID_TAIL(mainModule->heapId));
}

void func_ov093_021ba2d4(BtlMainModule *mainModule, BattleMon *mon) {
    if (mainModule->server != NULL) {
        func_ov093_021bb8dc(mainModule->server, mon);
    }
}

void func_ov093_021ba2e4(BtlMainModule *mainModule) {
    PokeParty *party;
    u32 count;
    u32 i;
    PartyPkm *original;
    PartyPkm *pkm;
    u16 item;
    u8 clientId;
    u32 j;
    unsigned long index;
    u32 k;
    u32 maxHP;
    u8 firstMonId;
    u8 slot;
    BattleMon *mon;

    if (mainModule->setup->fieldSituation.unk1b != 0) {
        return;
    }
    if (mainModule->setup->battleType <= 1) {
        func_ov093_021b97a0(&mainModule->pokeCons[1], mainModule, mainModule->playerClientId);
        party = func_ov093_021b985c(&mainModule->pokeCons[1], mainModule->playerClientId);
        if (mainModule->setup->battleType == 1 && func_ov093_021d7f60(func_ov093_021ba020(mainModule, 1))) {
            func_0201b818(party, GetFieldEffectData(mainModule)->env.zoneId, HEAPID_TAIL(mainModule->heapId));
        }
        if (mainModule->setup->battleType == 1) {
            count = PokeParty_GetPkmCount(party);
            for (i = 0; i < count; i++) {
                original = PokeParty_GetPkm(mainModule->setup->party[0], i);
                pkm = PokeParty_GetPkm(party, i);
                item = PokeParty_GetParam(original, PKM_PARAM_ITEM, NULL);
                if (ItemGetParam(item, 0x10) == 0) {
                    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, item);
                } else if (item != PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL)) {
                    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, 0);
                }
            }
        }
        PokeParty_Copy(party, mainModule->setup->party[0]);
        for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
            PokeParty_GetPkm(party, i);
        }
    }
    if (mainModule->setup->battleType == 0) {
        clientId = func_ov093_021b9064(mainModule, mainModule->playerClientId, 0);
        func_ov093_021b969c(&mainModule->pokeCons[1], mainModule, clientId, TRUE);
        PokeParty_Copy(func_ov093_021b985c(&mainModule->pokeCons[1], clientId), mainModule->setup->party[1]);
    }
    if (mainModule->setup->battleType == 4) {
        return;
    }
    for (j = 0; j < 4; j++) {
        BattleParty *battleParty;
        u32 monCount;
        u32 hp;
        u8 slot;
        index = func_ov093_021ba710(mainModule->playerClientId, j);
        for (k = 0; k < 6; k++) {
            mainModule->setup->unkC8[index][k] = 0;
        }
        if (DoesClientExist(mainModule, index)) {
            battleParty = GetPartyData(&mainModule->pokeCons[0], index);
            monCount = GetNumMonsInParty(battleParty);
            maxHP = 0;
            hp = 0;
            firstMonId = data_ov093_021efcf0[index];
            for (i = 0; i < monCount; i++) {
                mon = func_ov093_021b9bf0(battleParty, i);
                slot = GetMonID(mon) - firstMonId;
                hp += GetBattleMonStat(mon, 13);
                maxHP += GetBattleMonStat(mon, 14);
                if (IsFainted(mon)) {
                    mainModule->setup->unkC8[index][slot] = 2;
                } else if (GetBattleMonStatus(mon) != 0) {
                    mainModule->setup->unkC8[index][slot] = 1;
                }
            }
            mainModule->setup->unkE0[index] = hp * 100 / maxHP;
        }
    }
}




BOOL func_ov093_021ba534(BtlMainModule *mainModule) {
    switch (BtlSetup_GetBattleType(mainModule)) {
    case 2:
    case 3:
        return TRUE;
    default:
        return FALSE;
    }
}

u32 func_ov093_021ba54c(BtlMainModule *mainModule) {
    u32 result;
    if (BtlSetup_GetBattleType(mainModule) == 4) {
        result = 1;
    } else if (mainModule->unk473_0) {
        result = 6;
    } else if (mainModule->setup->unk90 != 6) {
        result = 5;
    } else if (func_ov093_021d826c(&mainModule->unk448)) {
        result = func_ov093_021d8270(&mainModule->unk448, mainModule->playerClientId, BtlSetup_GetBattleType(mainModule));
    } else {
        if (mainModule->result == 7) {
            mainModule->result = 1;
        }
        result = mainModule->result;
    }
    mainModule->setup->unk8C = result;
    return result;
}


u16 func_ov093_021ba5c4(BtlMainModule *mainModule) {
    return mainModule->unk442;
}

u16 func_ov093_021ba5d0(BtlMainModule *mainModule) {
    return mainModule->unk440;
}

BOOL func_ov093_021ba5d8(BtlMainModule *mainModule) {
    if (mainModule->setup->fieldSituation.unk1b == 0) {
        if (mainModule->unk440 != 0) {
            return func_ov093_021cdda0(mainModule->clients[mainModule->playerClientId]);
        }
        return FALSE;
    }
    return func_ov093_021cdda0(mainModule->clients[mainModule->playerClientId]);
}

BOOL func_ov093_021ba618(BtlMainModule *mainModule) {
    BOOL result;

    if (mainModule->setup->fieldSituation.unk1b != 0) {
        result = func_ov093_021cddcc(mainModule->clients[mainModule->playerClientId]);
        if (result) {
            mainModule->result = 2;
        }
        return result;
    }
    return FALSE;
}

void func_ov093_021ba648(BtlMainModule *mainModule, u8 clientId, const PokeParty *party) {
    u8 index = func_ov093_021ba710(mainModule->playerClientId, clientId);

    if (mainModule->setup->party[index] != NULL) {
        PokeParty_Copy(party, mainModule->setup->party[index]);
    }
}

void func_ov093_021ba670(BtlMainModule *mainModule, u8 clientId, const PlayerInfo *src) {
    u8 index = func_ov093_021ba710(mainModule->playerClientId, clientId);

    if (mainModule->setup->unk34[index] != NULL) {
        func_02008494(src, mainModule->setup->unk34[index]);
    }
}

void func_ov093_021ba698(BtlMainModule *mainModule, u8 clientId) {
    u8 index = func_ov093_021ba710(mainModule->playerClientId, clientId);

    mainModule->setup->unk44[index] = func_ov093_021b997c(mainModule, clientId);
}

void *func_ov093_021cd924(BtlClient *client, u32 *size);


void func_ov093_021ba6c0(BtlMainModule *mainModule) {
    u32 size;
    void *buf;
    if (func_ov093_021b90f8(mainModule)) {
        buf = func_ov093_021cd924(mainModule->clients[mainModule->playerClientId], &size);
        if (buf != NULL) {
            MI_CpuCopy8(buf, mainModule->setup->unk94, size);
            mainModule->setup->unk98 = size;
            mainModule->setup->rand = mainModule->rand;
        }
    }
}


u8 func_ov093_021ba710(u8 playerClientId, u8 clientId) {
    u32 result;

    if (playerClientId == clientId) {
        return 0;
    }
    result = 1;
    if ((u8)(playerClientId & result) == (u8)(clientId & result)) {
        return 2;
    }
    if (clientId > 1) {
        result = 3;
    }
    return result;
}

void func_ov093_021ba73c(BtlMainModule *mainModule, u32 arg1) {
    u32 unk;
    u32 i;

    unk = func_ov093_021b6818(mainModule->setup);
    func_ov093_021d787c(&mainModule->rand);
    func_ov093_021b68d8(mainModule, mainModule->setup);
    func_ov093_021b9424(&mainModule->pokeCons[0]);
    func_ov093_021b9424(&mainModule->pokeCons[1]);
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(mainModule, i)) {
            func_ov093_021b9460(&mainModule->pokeCons[1], mainModule, i);
            func_ov093_021b9460(&mainModule->pokeCons[0], mainModule, i);
            func_ov093_021cd984(mainModule->clients[i], arg1);
        }
    }
    func_ov093_021e9034(mainModule->viewCore);
    func_ov093_021e8e28(0x13);
    mainModule->viewCore = BtlvCore_Create(mainModule, mainModule->clients[mainModule->setup->fieldSituation.unk19],
                                           &mainModule->pokeCons[0], unk, 0x13);
    func_ov093_021cd95c(mainModule->clients[mainModule->setup->fieldSituation.unk19], mainModule->viewCore);
    func_ov093_021bab40(mainModule->server);
}

void func_ov093_021ba7f8(BtlMainModule *mainModule) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (DoesClientExist(mainModule, i)) {
            func_ov093_021cd9b0(mainModule->clients[i]);
        }
    }
}

BtlServerFlow *func_ov093_021bb964(BtlServer *server);

BtlServerFlow *func_ov093_021ba820(BtlMainModule *mainModule) {
    return func_ov093_021bb964(mainModule->server);
}

void func_ov093_021ba82c(BtlSetup *setup) {
    setup->unkB6 = 0;
    setup->unkB7 = 0;
    setup->unkB8 = 0;
    setup->unkB9 = 0;
    setup->unkBA = 0;
    setup->unkBB = 0;
    setup->unkBC = 0;
    setup->unkBD = 0;
    setup->unkBE = 0;
    setup->unkBF = 0;
    setup->unkC0 = 0;
    setup->unkB4 = 0;
}

u16 func_ov093_021c7cbc(BtlServerFlow *flow);
u16 func_ov093_021c7cc8(BtlServerFlow *flow);
u16 GetTurnCounter(BtlServerFlow *flow);
u8 func_ov093_021c8140(BtlServerFlow *flow, u32 arg1);
u16 func_ov093_021c7cd4(BtlServerFlow *flow);
u16 func_ov093_021c7cdc(BtlServerFlow *flow);
u16 func_ov093_021c7ce8(BtlServerFlow *flow);
u8 func_ov093_021d54b4(BattleMon *mon);

void func_ov093_021ba878(BtlMainModule *mainModule) {
    BtlServerFlow *flow;
    BtlSetup *setup;
    BattleParty *party;
    BattleMon *mon;
    u32 count;
    u32 alive;
    u32 i;

    if (mainModule->server != NULL) {
        flow = func_ov093_021bb964(mainModule->server);
        setup = mainModule->setup;
        setup->unkB6 = GetTurnCounter(flow);
        setup->unkB7 = func_ov093_021c8140(flow, 0);
        setup->unkB8 = func_ov093_021c7cbc(flow);
        setup->unkB9 = func_ov093_021c7cc8(flow);
        setup->unkBA = func_ov093_021c7cd4(flow);
        setup->unkBB = func_ov093_021c7cdc(flow);
        setup->unkBC = func_ov093_021c7ce8(flow);
        if (setup->unk8C == 1) {
            setup->unkBD = 1;
        }
        party = GetPartyData(&mainModule->pokeCons[1], 0);
        count = GetNumMonsInParty(party);
        alive = GetAlivePartyCount(party);
        for (i = 0; i < count; i++) {
            mon = func_ov093_021b9bf0(party, i);
            setup->unkC0 += func_ov093_021d54b4(mon);
            setup->unkB4 += GetBattleMonStat(mon, 0xd);
        }
        setup->unkBF = count - alive;
        party = GetPartyData(&mainModule->pokeCons[1], 1);
        count = GetNumMonsInParty(party);
        alive = GetAlivePartyCount(party);
        setup->unkBE = count - alive;
        if (DoesClientExist(mainModule, 3)) {
            party = GetPartyData(&mainModule->pokeCons[1], 3);
            if (party != NULL) {
                count = GetNumMonsInParty(party);
                alive = GetAlivePartyCount(party);
                setup->unkBE += count - alive;
            }
        }
    }
}




