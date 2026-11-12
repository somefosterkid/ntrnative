// Black City's or White Forest's state and residents, and the messages its communication exchanges. Its first, second
// and last functions are pokebw2's (GPL-3.0, https://github.com/fuddlesworth/pokebw2) src/ov012/city_state.c, which
// is the whole file in Black 2/White 2's overlay 12; the rest is Black's alone. The file's name is pokebw2's, and
// the ROM names none of its functions.
#include "types.h"
#include "field/city_state.h"
#include "gfl/net.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "system/game_data.h"

typedef BOOL (*CityCommMessageFunc)(CityComm *comm, CityState *state, const CityCommMessage *message,
                                    CityCommMessage *reply);

static CityResident *func_ov010_02161dc4(CityState *state, u32 id, int list);
static void func_ov010_021620b0(CityResident *residents);
static BOOL func_ov010_0216210c(void);
static void func_ov010_02162120(CityComm *comm, CityState *state, u32 mask);
static BOOL func_ov010_02162198(CityComm *comm, CityState *state, const CityCommMessage *message,
                                CityCommMessage *reply);
static BOOL func_ov010_021621c0(CityComm *comm, CityState *state, const CityCommMessage *message,
                                CityCommMessage *reply);
static BOOL func_ov010_021621e8(CityComm *comm, CityState *state, const CityCommMessage *message,
                                CityCommMessage *reply);
static void func_ov010_0216220c(CityComm *comm, const CityCommMessage *message);
static void func_ov010_02162230(CityComm *comm, u16 index, const CityCommMessage *message);
static BOOL func_ov010_02162254(CityComm *comm, u16 index, CityCommMessage *dest);
static void func_ov010_02162278(CityComm *comm, u16 index, u16 value4, u16 value6);
static void func_ov010_02162298(CityComm *comm, u16 value, u32 kind);
static BOOL func_ov010_021622ac(CityComm *comm, u16 index, CityCommMessage *dest);
static BOOL func_ov010_021622d8(CityComm *comm, CityCommMessage *dest);
static void func_ov010_021622f4(CityComm *comm);
static void func_ov010_0216230c(CityComm *comm, u16 index);
static void func_ov010_02162330(CityComm *comm, u16 index);
static void func_ov010_02162358(CityComm *comm);

static const u32 sKindValues[3] = { 1, 3, 4 };

// What a sent message asks, by its unk6
static CityCommMessageFunc sMessageFuncs[3] = {
    func_ov010_02162198,
    func_ov010_021621c0,
    func_ov010_021621e8,
};

void func_ov010_02161d64(CityState *state) {
    if (state->city == 1) {
        SetAllowVersionSpecificZone(0, 1);
        SetAllowVersionSpecificZone(1, 1);
    }
    if (state->city == 0) {
        SetAllowVersionSpecificZone(0, 0);
        SetAllowVersionSpecificZone(1, 0);
    }
}

void func_ov010_02161d98(CityState *state) {
    int i;

    state->initialized = 0;
    state->city = 2;
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        func_ov010_02161e78(&state->residents[i]);
        func_ov010_02161e78(&state->visitors[i]);
    }
}

// The resident with the ID in list 0, list 1 or, with 2, either
static CityResident *func_ov010_02161dc4(CityState *state, u32 id, int list) {
    int i;

    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        if (list == 0 || list == 2) {
            if (func_ov010_02161ecc(&state->residents[i]) && id == state->residents[i].id) {
                return &state->residents[i];
            }
        }
        if (list == 1 || list == 2) {
            if (func_ov010_02161ecc(&state->visitors[i]) && id == state->visitors[i].id) {
                return &state->visitors[i];
            }
        }
    }
    return NULL;
}

BOOL func_ov010_02161e4c(CityState *state, u32 id, int list) {
    if (func_ov010_02161dc4(state, id, list) != NULL) {
        return TRUE;
    }
    return FALSE;
}

// Closes the gaps in a list
void func_ov010_02161e60(CityState *state, int list) {
    if (list == 0) {
        func_ov010_021620b0(state->residents);
    } else {
        func_ov010_021620b0(state->visitors);
    }
}

void func_ov010_02161e78(CityResident *resident) {
    MI_CpuFill8(resident, 0, sizeof(CityResident));
}

BOOL func_ov010_02161e84(const CityResident *resident) {
    u16 eom = GFL_StrBufGetTerminator();
    int i;
    BOOL ended;

    if (resident->id >= 30) {
        return FALSE;
    }
    if (resident->unk2 > 100) {
        return FALSE;
    }
    if (resident->hasName) {
        ended = FALSE;
        for (i = 0; i < 8; i++) {
            if (eom == resident->name[i]) {
                ended = TRUE;
            }
        }
        if (!ended) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_ov010_02161ecc(const CityResident *resident) {
    if (resident->exists == 0) {
        return FALSE;
    }
    if (func_ov010_02161e84(resident)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov010_02161eec(u16 *ids) {
    int i;

    for (i = 0; i < 10; i++) {
        ids[i] = 0xffff;
    }
}

void func_ov010_02161f00(CityResidentInfo *info) {
    MI_CpuFill8(info, 0, sizeof(CityResidentInfo));
    info->unk2 = 10;
}

void func_ov010_02161f14(CityResidentInfo *info, CityState *state, HeapID heapId) {
    void *handle;
    CityResidentData *data;

    if (state->city == 1) {
        handle = func_ov020_021867c4(state->residents[0].id, HEAPID_TAIL(heapId));
        data = func_ov020_0218682c(handle);
        info->unk0 = data->unk24;
        info->unk2 = data->unk8;
        func_ov020_02186804(handle);
    } else {
        info->unk0 = 0;
        info->unk2 = 10;
    }
}

void func_ov010_02161f54(CityComm *comm) {
    MI_CpuFill8(comm, 0, sizeof(CityComm));
    comm->netId = func_02039338();
}

void func_ov010_02161f68(CityComm *comm) {
    MI_CpuFill8(comm, 0, sizeof(CityComm));
}

void func_ov010_02161f74(CityComm *comm, CityState *state, u32 mask) {
    if (!func_ov010_0216210c()) {
        func_ov010_02162120(comm, state, mask);
    }
}

// Moves the visitor with the ID from src to dest
void func_ov010_02161f90(CityComm *comm, CityState *dest, CityState *src, PlayerInfo *player, u16 id, HeapID heapId) {
    CityResident *resident = func_ov010_02161dc4(src, id, 1);
    void *handle = func_ov020_021867c4(id, heapId);
    CityResidentData *data = func_ov020_0218682c(handle);

    if (dest->city == 0) {
        resident->unk2 = data->unk6;
    } else {
        resident->unk2 = data->unk4;
    }
    resident->unk3 = 7;
    func_ov020_02186804(handle);
    func_ov020_02186520(dest, player, resident);
    func_ov010_02161e78(resident);
    func_ov010_02161e60(src, 1);
}

void func_ov010_02161fec(CityComm *comm, u8 value) {
    comm->unk4 = value;
}

void func_ov010_02161ff0(CityComm *comm, const CityCommMessage *message) {
    if (!func_ov010_0216210c()) {
        func_ov010_0216220c(comm, message);
    }
}

void func_ov010_02162008(CityComm *comm, u16 index, const CityCommMessage *message) {
    if (!func_ov010_0216210c()) {
        func_ov010_02162230(comm, index, message);
    }
}

BOOL func_ov010_02162024(CityComm *comm, u16 index, CityCommMessage *dest) {
    return func_ov010_021622ac(comm, index, dest);
}

BOOL func_ov010_0216202c(CityComm *comm, CityCommMessage *dest, u8 *value) {
    *value = comm->unk4;
    return func_ov010_021622d8(comm, dest);
}

void func_ov010_02162038(CityComm *comm, u16 index) {
    func_ov010_02162330(comm, index);
}

void func_ov010_02162040(CityComm *comm) {
    func_ov010_02162358(comm);
}

void func_ov010_02162048(CityComm *comm, u16 value, u32 kind) {
    CityCommMessage message;

    if (!func_ov010_0216210c()) {
        func_ov010_02162298(comm, value, kind);
        return;
    }
    if (sKindValues[kind] != 4) {
        message.netId = comm->netId;
        message.unk4 = value;
        message.unk6 = sKindValues[kind];
        func_ov010_0216220c(comm, &message);
    }
}

BOOL func_ov010_0216208c(CityComm *comm, u16 unused) {
    if (comm->flags & 8) {
        return TRUE;
    }
    return FALSE;
}

u16 func_ov010_0216209c(CityComm *comm) {
    return comm->received.unk6;
}

void func_ov010_021620a0(CityComm *comm) {
    func_ov010_02162358(comm);
    func_ov010_021622f4(comm);
}

static void func_ov010_021620b0(CityResident *residents) {
    int i;
    int j;

    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        if (!func_ov010_02161ecc(&residents[i])) {
            for (j = i + 1; j < CITY_RESIDENT_MAX; j++) {
                if (func_ov010_02161ecc(&residents[j])) {
                    MI_CpuCopy8(&residents[j], &residents[i], sizeof(CityResident));
                    func_ov010_02161e78(&residents[j]);
                    break;
                }
            }
        }
    }
}

static BOOL func_ov010_0216210c(void) {
    if (func_0200fb24()) {
        return TRUE;
    }
    return FALSE;
}

// Answers the sent messages of the mask, and clears the others
static void func_ov010_02162120(CityComm *comm, CityState *state, u32 mask) {
    int i;
    CityCommMessage reply;
    CityCommMessage message;
    BOOL replied;

    for (i = 0; i < 3; i++) {
        if (!(mask & (1 << i))) {
            func_ov010_0216230c(comm, i);
        } else if (func_ov010_02162254(comm, i, &message)) {
            replied = sMessageFuncs[message.unk6](comm, state, &message, &reply);
            func_ov010_0216230c(comm, i);
            if (replied) {
                func_ov010_02162278(comm, i, reply.unk4, reply.unk6);
            }
        }
    }
}

static BOOL func_ov010_02162198(CityComm *comm, CityState *state, const CityCommMessage *message,
                                CityCommMessage *reply) {
    reply->netId = message->netId;
    reply->unk4 = message->unk4;
    if (func_ov010_02161dc4(state, message->unk4, 1) != NULL) {
        reply->unk6 = 0;
    } else {
        reply->unk6 = 1;
    }
    return TRUE;
}

static BOOL func_ov010_021621c0(CityComm *comm, CityState *state, const CityCommMessage *message,
                                CityCommMessage *reply) {
    reply->netId = message->netId;
    reply->unk4 = message->unk4;
    if (func_ov010_02161dc4(state, message->unk4, 1) != NULL) {
        reply->unk6 = 2;
    } else {
        reply->unk6 = 3;
    }
    return TRUE;
}

static BOOL func_ov010_021621e8(CityComm *comm, CityState *state, const CityCommMessage *message,
                                CityCommMessage *reply) {
    CityResident *resident = func_ov010_02161dc4(state, message->unk4, 1);

    if (resident != NULL) {
        func_ov010_02161e78(resident);
        func_ov010_02161e60(state, 1);
    }
    return FALSE;
}

static void func_ov010_0216220c(CityComm *comm, const CityCommMessage *message) {
    if (message->netId == comm->netId) {
        MI_CpuCopy8(message, &comm->received, sizeof(CityCommMessage));
        comm->flags = comm->flags | 8;
    }
}

static void func_ov010_02162230(CityComm *comm, u16 index, const CityCommMessage *message) {
    MI_CpuCopy8(message, &comm->sent[index], sizeof(CityCommMessage));
    comm->flags = comm->flags | 1 << index;
}

static BOOL func_ov010_02162254(CityComm *comm, u16 index, CityCommMessage *dest) {
    BOOL result = FALSE;

    if (comm->flags & (1 << index)) {
        MI_CpuCopy8(&comm->sent[index], dest, sizeof(CityCommMessage));
        result = TRUE;
    }
    return result;
}

static void func_ov010_02162278(CityComm *comm, u16 index, u16 value4, u16 value6) {
    comm->replies[index].netId = index;
    comm->replies[index].unk4 = value4;
    comm->replies[index].unk6 = value6;
    comm->flags = comm->flags | 1 << (index + 5);
}

static void func_ov010_02162298(CityComm *comm, u16 value, u32 kind) {
    comm->unk28.netId = comm->unk4;
    comm->unk28.unk4 = value;
    comm->unk28.unk6 = kind;
    comm->flags = comm->flags | 0x10;
}

static BOOL func_ov010_021622ac(CityComm *comm, u16 index, CityCommMessage *dest) {
    BOOL result = FALSE;

    if (comm->flags & (1 << (index + 5))) {
        MI_CpuCopy8(&comm->replies[index], dest, sizeof(CityCommMessage));
        result = TRUE;
    }
    return result;
}

static BOOL func_ov010_021622d8(CityComm *comm, CityCommMessage *dest) {
    BOOL result = FALSE;

    if (comm->flags & 0x10) {
        MI_CpuCopy8(&comm->unk28, dest, sizeof(CityCommMessage));
        result = TRUE;
    }
    return result;
}

static void func_ov010_021622f4(CityComm *comm) {
    MI_CpuFill8(&comm->received, 0, sizeof(CityCommMessage));
    comm->flags = comm->flags & ~8;
}

static void func_ov010_0216230c(CityComm *comm, u16 index) {
    MI_CpuFill8(&comm->sent[index], 0, sizeof(CityCommMessage));
    comm->flags = comm->flags & ~(1 << index);
}

static void func_ov010_02162330(CityComm *comm, u16 index) {
    MI_CpuFill8(&comm->replies[index], 0, sizeof(CityCommMessage));
    comm->flags = comm->flags & ~(1 << (index + 5));
}

static void func_ov010_02162358(CityComm *comm) {
    MI_CpuFill8(&comm->unk28, 0, sizeof(CityCommMessage));
    comm->flags = comm->flags & ~0x10;
}

BOOL func_ov010_02162370(s32 value) {
    if (value >= 7) {
        return TRUE;
    }
    return FALSE;
}
