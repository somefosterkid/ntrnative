// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov028/union_comm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "field/union_comm.h"
#include "types.h"
#include "constants/language.h"
#include "constants/version.h"
#include "field/colosseum.h"
#include "field/comm_player.h"
#include "field/union_app.h"
#include "field/union_eff.h"
#include "field/union_main.h"
#include "field/unity_tower.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_whpipe.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "save/wifi_list.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "battle/regulation.h"
#include "field/colosseum_tool.h"
#include "nitro/os.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
void func_ov012_0217505c(UnionColosseum *colosseum, GameSystem *gsys);
void func_ov022_02183be0(UnionApp *app);
void *func_ov012_02173984(int netId, void *work, int size);
void func_ov012_02173b78(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173bfc(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173c64(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173c8c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173ccc(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173d7c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173db0(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173de4(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173e40(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173e90(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173eec(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173f54(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_02173fe0(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_0217403c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov012_021740d4(int netId, int size, const void *data, void *work, NetHandle *handle);

// The Union Room's communication: the system work that GameCommSys boots for GAME_COMM_NO_UNION, which restarts the
// network on request, receives the beacons of the machines around, keeps the group this machine is in and builds
// the beacon it sends. Names are not swan's

#define UNION_BEACON_TIMEOUT 450
#define HEAPID_UNION 0x41 // the heap the Union Room allocates in; its name in the ROM isn't known

static void func_ov012_02172c1c(void *param, UnionSystem *unisys);
void func_ov012_02172d14(void *work);
BOOL func_ov012_02172d7c(UnionSystem *unisys);
void func_ov012_02172eec(UnionSystem *unisys);
static void func_ov012_02172ff0(UnionSystem *unisys);
static void func_ov012_0217303c(UnionSystem *unisys);
static void func_ov012_021730c8(UnionSystem *unisys);
static BOOL func_ov012_021730f8(UnionSystem *unisys, UnionBeacon *beacon, const u8 *mac);
static BOOL func_ov012_021731e0(UnionSystem *unisys, UnionBeacon *beacon, const u8 *mac);
static void *func_ov012_02173248(void *work);
static void func_ov012_0217325c(UnionSystem *unisys, UnionBeacon *beacon);
static int func_ov012_021733c4(void *work);
static BOOL func_ov012_021733c8(u8 gameCommandBase, u8 beaconCommandBase, void *work);
static void func_ov012_02173400(NetHandle *handle, int a1, void *work);
static void func_ov012_02173404(void *work);
void func_ov012_021734e0(UnionSystem *unisys);
void func_ov012_0217365c(UnionVisitorList *list);

static const NetCommand data_ov012_021753f0[21];

static const GFLNetInitData data_ov012_02175380 = {
    data_ov012_021753f0,
    21,
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov012_02173248,
    func_ov012_021733c4,
    func_ov012_021733c8,
    func_ov012_02173400,
    NULL,
    func_ov012_02173404,
    NULL,
    {0},
    NULL,
    NULL,
    0,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
    HEAPID_USER,
    HEAPID_TAIL(0xd),
    HEAPID_TAIL(0xf),
    HEAPID_TAIL(0xd),
    0xf0,
    0,
    5,
    0x5a,
    10,
    1,
    0,
    0,
    1,
    0x14,
    {0x2c, 1, 0, 0},
    0,
    0,
};

static const NetCommand data_ov012_021753f0[21] = {
    { func_ov012_021739a8, NULL },
    { func_ov012_021739e0, NULL },
    { func_ov012_02173a24, NULL },
    { func_ov012_02173a68, NULL },
    { func_ov012_02173ac4, NULL },
    { func_ov012_02173b1c, NULL },
    { func_ov012_02173b78, func_ov012_02173984 },
    { func_ov012_02173bfc, func_ov012_02173984 },
    { func_ov012_02173c64, func_ov012_02173984 },
    { func_ov012_02173c8c, NULL },
    { func_ov012_02173ccc, NULL },
    { func_ov012_02173d7c, NULL },
    { func_ov012_02173db0, NULL },
    { func_ov012_02173de4, NULL },
    { func_ov012_02173e40, NULL },
    { func_ov012_02173e90, NULL },
    { func_ov012_02173eec, NULL },
    { func_ov012_02173f54, NULL },
    { func_ov012_02173fe0, NULL },
    { func_ov012_0217403c, NULL },
    { func_ov012_021740d4, NULL },
};

// pokebw2's UnionComm_Init


UnionSystem *func_ov012_02172bc0(UnionBootParam *param) {
    UnionSystem *unisys;

    GFL_HeapCreateChild(HEAPID_GAMEEVENT, HEAPID_UNION, 0x8a00);
    unisys = GFL_HeapAllocate(HEAPID_UNION, sizeof(UnionSystem), TRUE, "union_comm.c", 137);
    unisys->param = param;
    func_ov012_021734e0(unisys);
    unisys->situ.changed = TRUE;
    func_ov012_0217365c(&unisys->visitorList);
    unisys->alloc.regulation = func_0200adf0(HEAPID_UNION);
    unisys->fieldActive = TRUE;
    return unisys;
}

static void func_ov012_02172c1c(void *param, UnionSystem *unisys) {
    if (unisys->alloc.regulation != NULL) {
        GFL_HeapFree(unisys->alloc.regulation);
    }
    func_ov012_02173420(unisys);
    GFL_HeapFree(unisys);
    GFL_HeapDelete(HEAPID_UNION);
    GFL_HeapFree(param);
}

void *func_ov012_02172c4c(u32 *seq, void *param) {
    UnionSystem *unisys = func_ov012_02172bc0(param);

    unisys->commState = 1;
    func_0203cbac(&data_ov012_02175380, func_ov012_02172c98, unisys);
    return unisys;
}

BOOL func_ov012_02172c78(u32 *seq, void *param, void *work) {
    UnionSystem *unisys = work;

    if (unisys->commState >= 2) {
        unisys->commState = 3;
        func_0203d0fc(0);
        return TRUE;
    }
    return FALSE;
}


// pokebw2's UnionComm_OnNetReady
void func_ov012_02172c98(void *work) {
    UnionSystem *unisys = work;

    unisys->commState = 2;
    func_0203d8c4(FALSE);
}

BOOL func_ov012_02172cac(u32 *seq, void *param, void *work) {
    UnionSystem *unisys = work;

    unisys->unk356C = 1;
    if (unisys->commState < 3) {
        return FALSE;
    }
    unisys->commState = 4;
    if (!func_0203ceac(func_ov012_02172d14)) {
        func_ov012_02172d14(unisys);
    }
    return TRUE;
}


// pokebw2's UnionComm_ExitWait
BOOL func_ov012_02172ce8(u32 *seq, void *param, void *work) {
    UnionSystem *unisys = work;

    if (unisys->commState == 5) {
        func_ov012_02172c1c(param, unisys);
        if (func_0200fb24()) {
            func_0200fba0();
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionComm_SetEnded
void func_ov012_02172d14(void *work) {
    UnionSystem *unisys = work;

    unisys->commState = 5;
}

// pokebw2's UnionComm_RequestLink0
void func_ov012_02172d20(UnionSystem *unisys) {
    if (unisys->linkState == 0) {
        unisys->linkState = 1;
        unisys->linkMode = 0;
    }
}

// pokebw2's UnionComm_RequestLink1
void func_ov012_02172d38(UnionSystem *unisys) {
    if (unisys->linkState == 0) {
        unisys->linkState = 1;
        unisys->linkMode = 1;
    }
}

// pokebw2's UnionComm_RequestLink2
void func_ov012_02172d50(UnionSystem *unisys) {
    if (unisys->linkState == 0) {
        unisys->linkState = 1;
        unisys->linkMode = 2;
    }
}

// pokebw2's UnionComm_IsLinkRequested
BOOL func_ov012_02172d68(UnionSystem *unisys) {
    if (unisys->linkState != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov012_02172d7c(UnionSystem *unisys) {
    switch (unisys->linkState) {
    case 0:
        if (unisys->linkMode == 1) {
            return TRUE;
        }
        return FALSE;
    case 1:
        switch (unisys->linkMode) {
        case 0:
        case 1:
            unisys->linkState = 2;
            break;
        case 2:
            unisys->linkState = 4;
            break;
        default:
            unisys->linkState = 0;
            break;
        }
        break;
    case 2:
        unisys->commState = 4;
        if (!func_0203ceac(func_ov012_02172d14)) {
            func_ov012_02172d14(unisys);
        }
        unisys->linkState++;
        break;
    case 3:
        if (unisys->commState == 5) {
            if (unisys->linkMode == 1) {
                unisys->linkState = 0;
            } else {
                unisys->linkState++;
            }
        }
        break;
    case 4:
        unisys->commState = 1;
        func_0203cbac(&data_ov012_02175380, func_ov012_02172c98, unisys);
        unisys->linkState++;
        break;
    case 5:
        if (unisys->commState >= 2) {
            unisys->commState = 3;
            func_0203d0fc(0);
            unisys->linkState = 0;
        }
        break;
    }
    return TRUE;
}


// pokebw2's UnionComm_Main
void func_ov012_02172e48(u32 *seq, void *param, void *work) {
    UnionSystem *unisys = work;

    if (func_ov012_02172d7c(unisys) == TRUE) {
        return;
    }
    if (func_0203cd98()) {
        if (unisys->beaconReady == 1) {
            func_ov014_0217778c();
            unisys->beaconReady = 0;
        }
        func_ov012_021730c8(unisys);
        func_ov012_0217303c(unisys);
        func_ov012_02172ff0(unisys);
        func_ov012_02172eec(unisys);
        if (func_0200fb24() && func_0203d378() == TRUE) {
            UnionColosseum *colosseum = unisys->colosseum_sys;

            if (colosseum != NULL && colosseum->unkC8_0 == 1 && unisys->situ.step == 0x2a && !colosseum->unkC8_1) {
                func_ov012_02172d38(unisys);
                unisys->colosseum_sys->unkC8_1 = TRUE;
            }
        }
    }
}

// pokebw2's UnionComm_ActivateGroup
void func_ov012_02172edc(UnionSystem *unisys) {
    unisys->situ.mycomm.active = TRUE;
}

// pokebw2's UnionComm_UpdateGroup
void func_ov012_02172eec(UnionSystem *unisys) {
    UnionSelf *self = &unisys->situ;

    if (!self->mycomm.active) {
        return;
    }
    if (func_0200fb24()) {
        return;
    }
    switch (self->mycomm.step) {
    case 0:
        if (func_ov012_021739cc() == TRUE) {
            self->mycomm.step++;
        }
        break;
    case 1:
        if (MATH_CountPopulation(self->mycomm.doneMask) >= func_0203d1b4()) {
            func_0203d7bc(FALSE);
            self->mycomm.step++;
        }
        break;
    case 2:
        if (func_0203d378() == TRUE) {
            if (func_0203d1b4() > 1) {
                break;
            }
            func_ov012_02172d38(unisys);
            self->mycomm.step++;
        } else {
            func_ov012_02172d38(unisys);
            self->mycomm.step++;
        }
        break;
    }
}

// pokebw2's UnionComm_UpdateEntries
static void func_ov012_02172ff0(UnionSystem *unisys) {
    if (func_0203d1b4() > 1) {
        if (unisys->entryRepliedTo != 0) {
            if (func_ov012_02173d88(unisys->entryRepliedTo) == 1) {
                unisys->entryRepliedTo = 0;
            }
        }
        if (unisys->entryAcceptedTo != 0) {
            if (func_ov012_02173dbc(unisys->entryAcceptedTo) == 1) {
                unisys->entryAcceptedTo = 0;
            }
        }
    } else {
        unisys->entryRepliedTo = 0;
        unisys->entryAcceptedTo = 0;
    }
}

static void func_ov012_0217303c(UnionSystem *unisys) {
    if (unisys->colosseum_sys == NULL || !unisys->colosseum_sys->unkC8_0) {
        return;
    }
    if (func_ov012_02173840(unisys) == TRUE) {
        if (!func_ov012_02174268(unisys)) {
            func_ov010_02167590(unisys->param->gsys, unisys->colosseum_sys->commPlayer);
            if (unisys->colosseum_sys->unk16F == 0) {
                if (func_ov010_0216786c(unisys->colosseum_sys->commPlayer, (CommPlayerStatus *)unisys->colosseum_sys->unk04) == TRUE) {
                    unisys->colosseum_sys->unk16F = 1;
                }
            }
            if (unisys->colosseum_sys->unk16F == 1) {
                if (func_ov012_02174c90((ColosseumChoice *)unisys->colosseum_sys->unk04) == TRUE) {
                    unisys->colosseum_sys->unk16F = 0;
                }
            }
        }
        func_ov019_0217ff78(unisys->colosseum_sys);
    }
    func_ov012_02175110(unisys->colosseum_sys);
    func_ov012_02175144(unisys->colosseum_sys);
}


// pokebw2's UnionComm_ReceiveBeacons
static void func_ov012_021730c8(UnionSystem *unisys) {
    int i;

    for (i = 0; i < 10; i++) {
        UnionBeacon *beacon = func_0203cf10(i);

        if (beacon != NULL) {
            u8 *mac = func_0203cf3c(i);

            if (mac != NULL) {
                func_ov012_021730f8(unisys, beacon, mac);
            }
            func_ov014_02177bf4(i);
        }
    }
}

// pokebw2's UnionComm_AddBeacon
static BOOL func_ov012_021730f8(UnionSystem *unisys, UnionBeacon *beacon, const u8 *mac) {
    int i;
    UnionBeaconEntry *found = unisys->found;

    if (beacon->valid != 1) {
        return FALSE;
    }
    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++) {
        UnionBeaconEntry *entry = &found[i];

        if (entry->beacon.valid == 1 && GFL_STD_MemCmp(mac, entry->mac, 6) == 0) {
            MI_CpuCopy8(beacon, entry, sizeof(UnionBeacon));
            if (found[i].status != 1) {
                found[i].status = 2;
            }
            found[i].timeout = UNION_BEACON_TIMEOUT;
            return TRUE;
        }
    }
    if (!func_ov012_021731e0(unisys, beacon, mac)) {
        return FALSE;
    }
    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++) {
        UnionBeaconEntry *entry = &found[i];

        if (entry->beacon.valid != 1) {
            MI_CpuCopy8(beacon, entry, sizeof(UnionBeacon));
            MI_CpuCopy8(mac, entry->mac, 6);
            found[i].index = i;
            found[i].status = 1;
            found[i].timeout = UNION_BEACON_TIMEOUT;
            return TRUE;
        }
    }
    return FALSE;
}

// Whether the machine belongs to a group whose parent is one of the beacons found, which keeps it from
// being added (and gives that beacon the shortest timeout)
// pokebw2's UnionComm_IsMemberOfBeacons
static BOOL func_ov012_021731e0(UnionSystem *unisys, UnionBeacon *beacon, const u8 *mac) {
    UnionBeaconEntry *entry = unisys->found;
    int i;
    int j;

    for (i = 0; i < UNION_BEACON_ENTRY_MAX; i++, entry++) {
        if (entry->beacon.valid == 1) {
            for (j = 0; j < UNION_MEMBER_MAX; j++) {
                if (entry->beacon.members[j].used == 1 && GFL_STD_MemCmp(mac, entry->beacon.members[j].mac, 6) == 0) {
                    if (entry->timeout != 0) {
                        entry->timeout = 1;
                    }
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

// pokebw2's UnionComm_GetBeacon
static void *func_ov012_02173248(void *work) {
    UnionSystem *unisys = work;

    func_ov012_0217325c(unisys, &unisys->beacon);
    return &unisys->beacon;
}

// pokebw2's UnionComm_BuildBeacon
static void func_ov012_0217325c(UnionSystem *unisys, UnionBeacon *beacon) {
    UnionSelf *self = &unisys->situ;

    MI_CpuFill8(beacon, 0, sizeof(UnionBeacon));
    if (self->mycomm.parent != NULL) {
        if (func_0203d378() == TRUE) {
            OS_GetMacAddress(beacon->mac);
        } else {
            MI_CpuCopy8(self->mycomm.parent->mac, beacon->mac, 6);
        }
    } else if (self->mycomm.unk4 != NULL) {
        MI_CpuCopy8(self->mycomm.unk4->mac, beacon->mac, 6);
    }
    beacon->memberCount = func_0203d1b4();
    beacon->version = GAME_VERSION;
    beacon->language = GAME_LANGUAGE;
    beacon->unk0A = self->step;
    beacon->unk07_5 = self->unk02;
    beacon->activity = self->activity;
    copyName(unisys->param->playerInfo->name, beacon->name, 8);
    beacon->look = func_02008554(unisys->param->playerInfo);
    beacon->gender = getTrainerGender(unisys->param->playerInfo);
    beacon->trainerId = getIDAsUInt(unisys->param->playerInfo);
    beacon->state = self->state;
    beacon->look2[0] = self->look[0];
    beacon->look2[1] = self->look[1];
    beacon->look2[2] = self->look[2];
    beacon->look2[3] = self->look[3];
    beacon->counter = self->counter;
    beacon->country = UnityTowerVisitor_GetCountry(unisys->param->playerInfo);
    beacon->province = UnityTowerVisitor_GetProvince(unisys->param->playerInfo);
    beacon->memberList = self->mycomm.memberList;
    func_02009cb0(func_0201283c(GSYS_GetGameData(unisys->param->gsys)), &beacon->unk50);
    if (unisys->alloc.uniapp != NULL) {
        beacon->entryClosed = func_ov022_021840c8(unisys->alloc.uniapp);
    } else {
        beacon->entryClosed = FALSE;
    }
    beacon->valid = TRUE;
    unisys->beaconReady = TRUE;
}

// pokebw2's UnionComm_GetBeaconSize
static int func_ov012_021733c4(void *work) {
    return sizeof(UnionBeacon);
}

// pokebw2's UnionComm_IsBeaconJoinable
static BOOL func_ov012_021733c8(u8 gameCommandBase, u8 beaconCommandBase, void *work) {
    UnionSystem *unisys = work;

    if (unisys->situ.mycomm.unk46_0 == 1) {
        if (gameCommandBase == 0x14 || beaconCommandBase == 0x14) {
            return FALSE;
        }
        if (gameCommandBase == beaconCommandBase) {
            return TRUE;
        }
        return FALSE;
    }
    if (beaconCommandBase >= 0x14 && beaconCommandBase <= 0x1b) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionComm_NetCallback24
static void func_ov012_02173400(NetHandle *handle, int a1, void *work) {
}

// pokebw2's UnionComm_NetCallback2C
static void func_ov012_02173404(void *work) {
}

// pokebw2's UnionComm_ClearBeacons
void func_ov012_02173408(UnionSystem *unisys) {
    MI_CpuFill8(unisys->found, 0, sizeof(unisys->found));
}

// pokebw2's UnionComm_FreeResources
void func_ov012_02173420(UnionSystem *unisys) {
    unisys->alloc.cardParam = NULL;
    if (unisys->alloc.my_card != NULL) {
        GFL_HeapFree(unisys->alloc.my_card);
        unisys->alloc.my_card = NULL;
    }
    if (unisys->alloc.target_card != NULL) {
        GFL_HeapFree(unisys->alloc.target_card);
        unisys->alloc.target_card = NULL;
    }
    if (unisys->alloc.boxParty != NULL) {
        GFL_HeapFree(unisys->alloc.boxParty);
        unisys->alloc.boxParty = NULL;
    }
    if (unisys->alloc.partySelect != NULL) {
        GFL_HeapFree(unisys->alloc.partySelect);
        unisys->alloc.partySelect = NULL;
    }
    if (unisys->alloc.uniapp != NULL) {
        func_ov022_02183be0(unisys->alloc.uniapp);
        unisys->alloc.uniapp = NULL;
    }
    if (unisys->colosseum_sys != NULL) {
        func_ov012_0217505c(unisys->colosseum_sys, unisys->param->gsys);
        unisys->colosseum_sys = NULL;
    }
}

// pokebw2's UnionComm_SetSelf
void func_ov012_021734a8(UnionSystem *unisys, u32 which, uintptr_t value) {
    UnionSelf *self = &unisys->situ;

    switch (which) {
    case 0:
        self->mycomm.target = (UnionBeaconEntry *)value;
        break;
    case 1:
        self->mycomm.unk4 = (UnionBeaconEntry *)value;
        break;
    case 2:
        self->mycomm.answer_pc = (UnionBeaconEntry *)value;
        break;
    case 3:
        self->mycomm.parent = (UnionBeaconEntry *)value;
        break;
    case 4:
        self->activity = value;
        break;
    }
}

// pokebw2's UnionComm_InitSelf
void func_ov012_021734e0(UnionSystem *unisys) {
    UnionSelf *self = &unisys->situ;

    MI_CpuFill8(self, 0, sizeof(UnionSelf));
    self->counter = GFL_RandomLC(0xffff);
    self->state = 1;
    self->step = 2;
    func_ov012_02173524(unisys, &self->mycomm);
}

// pokebw2's UnionGroup_ResetTargets
void func_ov012_02173518(UnionGroup *group) {
    group->unk19 = 0xff;
    group->unk1B = 0xff;
    group->unk1A = 0xff;
}

// pokebw2's UnionGroup_Init
void func_ov012_02173524(UnionSystem *unisys, UnionGroup *group) {
    UnionMember *self;

    MI_CpuFill8(group, 0, sizeof(UnionGroup));
    func_ov012_02173518(group);
    self = &group->members[0];
    OS_GetMacAddress(self->mac);
    self->look = func_02008554(unisys->param->playerInfo);
    self->gender = getTrainerGender(unisys->param->playerInfo);
    self->used = TRUE;
    group->unk44 = 0xffff;
}

int func_ov012_0217357c(UnionGroup *group, const u8 *mac, u8 look, u8 gender) {
    int i;
    int j;

    for (i = 0; i < UNION_MEMBER_MAX; i++) {
        UnionMember *member = &group->members[i];

        if (member->used == 1 && GFL_STD_MemCmp(mac, member, 6) == 0) {
            return i;
        }
    }
    for (j = 0; j < UNION_MEMBER_MAX; j++) {
        UnionMember *member = &group->members[j];

        if (!member->used) {
            MI_CpuCopy8(mac, member, 6);
            member->look = look;
            member->gender = gender;
            member->used = TRUE;
            return j;
        }
    }
    return 0;
}


// pokebw2's UnionGroup_AddBeaconMember
void func_ov012_021735fc(UnionGroup *group, const UnionBeaconEntry *entry) {
    func_ov012_0217357c(group, entry->mac, entry->beacon.look, entry->beacon.gender);
}

// pokebw2's UnionGroup_RemoveMember
void func_ov012_02173614(UnionGroup *group, const u8 *mac) {
    int i;

    for (i = 0; i < UNION_MEMBER_MAX; i++) {
        UnionMember *member = &group->members[i];

        if (member->used == 1 && GFL_STD_MemCmp(mac, member->mac, 6) == 0) {
            MI_CpuFill8(member, 0, sizeof(UnionMember));
            return;
        }
    }
}

// pokebw2's UnionGroup_RemoveBeaconMember
void func_ov012_02173650(UnionGroup *group, const UnionBeaconEntry *entry) {
    func_ov012_02173614(group, entry->mac);
}

// pokebw2's UnionComm_InitVisitors
void func_ov012_0217365c(UnionVisitorList *list) {
    int i;

    MI_CpuFill8(list, 0, sizeof(UnionVisitorList));
    for (i = 0; i < UNION_VISITOR_MAX; i++) {
        MI_CpuFill8(list->visitors[i].mac, 0xff, 6);
    }
    list->count = -1;
    list->viewEnd = -1;
    list->drawnEnd = -1;
}

// pokebw2's UnionComm_SetLook
void func_ov012_021736a0(UnionSystem *unisys, const u16 *look) {
    UnionSelf *self = &unisys->situ;

    self->look[0] = look[0];
    self->look[1] = look[1];
    self->look[2] = look[2];
    self->look[3] = look[3];
    self->state = 0;
    self->counter++;
    self->changed = TRUE;
}

// pokebw2's UnionComm_SetState
void func_ov012_021736c8(UnionSystem *unisys, u8 state) {
    UnionSelf *self = &unisys->situ;

    self->state = state;
    self->counter++;
    self->changed = TRUE;
}

// pokebw2's UnionComm_SetCommandBase
void func_ov012_021736e0(u32 activity) {
    switch (activity) {
    case 0x14:
        func_0203d5ac(0x17, 5);
        break;
    case 0x15:
        func_0203d5ac(0x18, 5);
        break;
    case 0x16:
        func_0203d5ac(0x19, 5);
        break;
    case 0x17:
        func_0203d5ac(0x1a, 5);
        break;
    case 3:
        func_0203d5ac(0x16, 5);
        break;
    case 0x19:
        func_0203d5ac(0x1b, 5);
        break;
    }
}
