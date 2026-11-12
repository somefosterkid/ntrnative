// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/vct/vct_session.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "vct_internal.h"

#include "nitro/mi.h"
#include "nitro/os.h"
#include "gfl/std.h"
#include "dwc/ext.h"
#include "dwc/vct.h"

// The machines the voice chat talks to. Machines ask each other for voice with small control packets, and a table of
// what each packet does to a machine's state decides whether to carry on, to refuse, or to give the application an
// event.

// The sessions that are free and in use, the OS_GetTick and the random number generator's constants
typedef struct VCTSessionPool {
    VCTSession *freeList;
    VCTSession *usedList;
    u64 (*OS_GetTick)(void);
    u32 randomMultiplier;
    u32 randomIncrement;
    // A session to answer from for a machine that cannot be given one, and the one session of the second mode
    VCTSession pending;
    VCTSession host;
} VCTSessionPool;

typedef struct VCTSessionState {
    // How many machines are in the voice chat, and which
    int count;
    u32 mask;
    // When the last request was sent, or 0
    u64 requestTime;
} VCTSessionState;

VCTSession *func_ov013_021b24fc(u8 aid);
static int func_ov013_021b2564(VCTSession *session, int command);
static int func_ov013_021b25b0(VCTSession *session, int command);
static void func_ov013_021b25fc(const VCTSession *session, VCTControlPacket *packet);
int func_ov013_021b2620(VCTSession *session, VCTControlPacket *packet);
void func_ov013_021b26dc(int flag);
void func_ov013_021b2724(VCTSession *session, u8 aid);
BOOL func_ov013_021b275c(const VCTSession *session, VCTControlPacket *packet);
static int func_ov013_021b279c(VCTControlPacket *packet, VCTSession *session);
int func_ov013_021b2848(VCTSession *session, VCTControlPacket *packet, u8 command);
int func_ov013_021b2894(VCTSession *session, const VCTControlPacket *packet);
static BOOL func_ov013_021b2958(const VCTSession *session);
int func_ov013_021b29a4(VCTSession *session, const VCTControlPacket *packet);
VCTSession *func_ov013_021b2a20(int aid);
int func_ov013_021b2010(VCTSession *session, u32 command, VCTControlPacket *packet, u32 size);
int func_ov013_021b20d4(VCTSession *session, u32 command, VCTControlPacket *packet, u32 size);
static BOOL func_ov013_021b2aa0(const VCTControlPacket *packet, VCTEventResult *result);
static BOOL func_ov013_021b2be0(VCTControlPacket *packet, VCTEventResult *result);
static BOOL func_ov013_021b2d54(const VCTControlPacket *packet, VCTEventResult *result);
BOOL func_ov013_021b2dd0(VCTControlPacket *packet, VCTEventResult *result);

// The events that a packet gives the application, by the command of the broadcast and of the direct packet
// pokebw2's sBroadcastEvents
static const u8 data_ov013_021b3af0[5] = {1, 8, 4, 10, 0};
// pokebw2's sDirectEvents
static const u8 data_ov013_021b3af5[7] = {7, 12, 2, 3, 9, 2, 0};

// What a control packet does to a machine in each state (a row each), by the command (a column each: those of a
// broadcast packet, then those of a direct one): the next state, -1 to take no notice, or -2 to refuse. The first two
// tables are for when the packet is a reply, the others for when it is a request, one of each for the first mode.
// pokebw2's sCheckTableOther
static const s8 data_ov013_021b3b84[6][11] = {
    {-2, -2, -2, 1, -1, -2, 0, 0, 0, -2, -2}, {-2, 5, 5, -2, -1, -2, 0, -2, -2, -2, -2},
    {-2, 5, -2, -2, -1, -2, 0, -2, -2, -2, -2}, {-2, -2, -2, -2, -1, -2, 0, -2, -2, -2, -2},
    {-2, -2, -2, -2, -1, 3, 0, 0, 0, -2, 0}, {-2, -2, -2, -2, -1, 0, 0, -2, -2, 0, -2},
};
// pokebw2's sReplyTableMode1
static const s8 data_ov013_021b3bc8[6][11] = {
    {4, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1}, {-2, -2, -2, -2, -1, 2, 0, 0, 0, -1, 0},
    {-2, 5, 5, -2, -1, -1, -1, -1, -1, -1, -1}, {-2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1},
    {-2, -2, 5, -2, -1, -1, -1, -1, -1, -1, -1}, {-2, -2, -2, -2, -1, 0, 0, -1, -1, 0, -1},
};
// pokebw2's sReplyTableOther
static const s8 data_ov013_021b3afc[6][11] = {
    {-2, -2, -2, 3, -1, -1, 0, -1, -1, -1, -1}, {-2, -2, -2, -3, -1, 2, 0, 0, 0, -1, 0},
    {-2, -2, -2, -3, -1, -1, -1, -1, -1, -1, -1}, {-2, 0, -2, -3, -1, -1, -1, -1, -1, -1, -1},
    {-2, -2, -2, -3, -1, -1, -1, -1, -1, -1, -1}, {-2, -2, -2, -3, -1, 0, 0, -1, -1, 0, -1},
};
// pokebw2's sCheckTableMode1
static const s8 data_ov013_021b3b40[6][11] = {
    {1, -2, -2, -2, -2, -2, 0, 0, 0, -2, -2}, {-2, -2, 5, -2, -2, -2, 0, -2, -2, -2, -2},
    {-2, 5, -2, -2, -2, -2, 0, -2, -2, -2, -2}, {-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
    {-2, -2, -2, -2, -2, 2, 0, 0, 0, -2, 0}, {-2, -2, -2, -2, -2, 0, 0, -2, -2, 0, -2},
};

// How long to wait for an answer, in OS_GetTick ticks
// pokebw2's sRequestTimeout
static u64 data_ov013_021b3c48 = 15709687;

// pokebw2's sSessionState
static u64 data_ov013_021b4bd8;
static int data_ov013_021b4bd0;
static u32 data_ov013_021b4bd4;
// pokebw2's sSessionPool
static VCTSessionPool data_ov013_021b4be0;


VCTSession *func_ov013_021b1f2c(u8 aid) {
    if (data_ov013_021b60d8.mode == 0 || data_ov013_021b60d8.mode == 2) {
        return NULL;
    }
    return func_ov013_021b24fc(aid);
}

BOOL func_ov013_021b1f54(VCTSession *session) {
    VCTSession *previous = NULL;
    VCTSession *current = data_ov013_021b4be0.usedList;

    if (session == NULL) {
        return FALSE;
    }
    if (data_ov013_021b60d8.mode == 2) {
        if (session == &data_ov013_021b4be0.host) {
            data_ov013_021b4be0.host.active = 0;
        }
        return TRUE;
    }
    while (current != NULL) {
        if (current == session) {
            session->active = 0;
            if (previous != NULL) {
                previous->next = session->next;
            } else if (current->next != NULL) {
                data_ov013_021b4be0.usedList = current->next;
            } else {
                data_ov013_021b4be0.usedList = NULL;
            }
            session->next = data_ov013_021b4be0.freeList;
            data_ov013_021b4be0.freeList = session;
            return TRUE;
        }
        previous = current;
        current = current->next;
    }
    return FALSE;
}


int func_ov013_021b2010(VCTSession *session, u32 command, VCTControlPacket *packet, u32 size) {
    if (session == NULL) {
        return -1;
    }
    if (size < 0x10) {
        return -1;
    }
    if (data_ov013_021b60d8.unkC == 1) {
        return -3;
    }
    if (command >= 5) {
        return -3;
    }
    packet->type = 0xff;
    func_ov013_021b2848(session, packet, command);
    if (func_ov013_021b29a4(session, packet) == 0) {
        return -3;
    }
    if (data_ov013_021b60d8.mode == 1 && command == 0) {
        if (!func_ov013_021b2958(session)) {
            return -3;
        }
    }
    return 0;
}


int func_ov013_021b20b0(VCTSession *session, int command) {
    if (data_ov013_021b60d8.mode == 2) {
        return -3;
    }
    return func_ov013_021b2564(session, command);
}

int func_ov013_021b20d4(VCTSession *session, u32 command, VCTControlPacket *packet, u32 size) {
    if (session == NULL) {
        return -1;
    }
    if (size < 0x10) {
        return -1;
    }
    if (command >= 6) {
        return -3;
    }
    packet->type = 0;
    func_ov013_021b2848(session, packet, command);
    if (command == 0) {
        if (data_ov013_021b60d8.mode == 1 && session->state == 4) {
            if (!func_ov013_021b2958(session)) {
                return -3;
            }
        }
        session->mask = session->aid != 0 ? 1 << session->aid : 1;
    }
    if (func_ov013_021b29a4(session, packet) == 0) {
        return -3;
    }
    return 0;
}


int func_ov013_021b2180(VCTSession *session, int command) {
    if (data_ov013_021b60d8.mode == 2) {
        return -3;
    }
    return func_ov013_021b25b0(session, command);
}

int func_ov013_021b21a4(u8 aid) {
    VCTSession *session;

    if (data_ov013_021b60d8.mode != 3) {
        return -2;
    }
    if (data_ov013_021b4bd0 == 3) {
        return -6;
    }
    if (aid == data_ov013_021b60d8.aid) {
        return -1;
    }
    if (func_ov013_021b2a20(aid) != NULL) {
        return 0;
    }
    session = func_ov013_021b1f2c(aid);
    if (session == NULL) {
        return -1;
    }
    session->aid = aid;
    session->origin = aid;
    session->state = 2;
    session->mask = 1 << aid;
    data_ov013_021b60d8.callback(aid, 7, session, data_ov013_021b60d8.userData);
    data_ov013_021b4bd0++;
    return 0;
}


int func_ov013_021b2254(u8 aid) {
    VCTSession *session;

    if (data_ov013_021b60d8.mode != 3) {
        return -2;
    }
    session = func_ov013_021b2a20(aid);
    if (session == NULL) {
        return -1;
    }
    data_ov013_021b60d8.callback(aid, 9, session, data_ov013_021b60d8.userData);
    data_ov013_021b4bd0--;
    return 0;
}

BOOL func_ov013_021b22b4(VCTConfig *config) {
    u32 i;
    VCTSession *last;

    if (config->sessions == NULL || config->sessionCount == 0 || config->sessionCount > 8) {
        return FALSE;
    }
    data_ov013_021b4be0.freeList = config->sessions;
    MI_CpuFill8(config->sessions, 0, config->sessionCount * sizeof(VCTSession));
    MI_CpuFill8(&data_ov013_021b4be0.host, 0, sizeof(VCTSession));
    MI_CpuFill8(&data_ov013_021b4be0.pending, 0, sizeof(VCTSession));
    for (i = 0; i < config->sessionCount - 1; i++) {
        data_ov013_021b4be0.freeList[i].next = &data_ov013_021b4be0.freeList[i + 1];
    }
    last = data_ov013_021b4be0.freeList + config->sessionCount - 1;
    last->next = NULL;
    data_ov013_021b4be0.usedList = NULL;
    data_ov013_021b4bd4 = 0;
    data_ov013_021b4bd0 = 0;
    data_ov013_021b60d8.unk14 = 0;
    data_ov013_021b4be0.OS_GetTick = OS_GetTick;
    data_ov013_021b4be0.randomMultiplier = 0x5d588b65;
    data_ov013_021b4be0.randomIncrement = 0x269ec3;
    return TRUE;
}





void func_ov013_021b23c8(void)
{
    data_ov013_021b4be0.usedList = NULL;
    data_ov013_021b4be0.freeList = NULL;
}

void func_ov013_021b23e0(void) {
    u64 now = OS_GetTick();

    if (data_ov013_021b60d8.mode != 2) {
        return;
    }
    if (data_ov013_021b60d8.unkC == 0) {
        return;
    }
    if (data_ov013_021b4bd8 == 0) {
        return;
    }
    if (now - data_ov013_021b4bd8 <= data_ov013_021b3c48) {
        return;
    }
    data_ov013_021b4bd8 = 0;
    data_ov013_021b4be0.host.active = 0;
    data_ov013_021b4be0.host.state = 0;
    func_ov013_021b26dc(0);
    data_ov013_021b60d8.callback(data_ov013_021b4be0.host.aid, 9, &data_ov013_021b4be0.host, data_ov013_021b60d8.userData);
}


BOOL func_ov013_021b2488(int aid, VCTControlPacket *packet, int size, VCTEventResult *result) {
    if (size != sizeof(VCTControlPacket)) {
        return FALSE;
    }
    if (packet->from != aid || packet->to != data_ov013_021b60d8.aid) {
        return FALSE;
    }
    if (packet->length != sizeof(VCTControlPacket)) {
        return func_ov013_021b2d54(packet, result);
    }
    if (data_ov013_021b60d8.unkC == 0) {
        return func_ov013_021b2aa0(packet, result);
    }
    return func_ov013_021b2be0(packet, result);
}

VCTSession *func_ov013_021b24fc(u8 aid) {
    VCTSession *session;

    if (aid >= 0x20) {
        return NULL;
    }
    if (aid == data_ov013_021b60d8.aid) {
        return NULL;
    }
    session = data_ov013_021b4be0.freeList;
    if (session == NULL) {
        return NULL;
    }
    data_ov013_021b4be0.freeList = session->next;
    func_ov013_021b2724(session, aid);
    session->next = data_ov013_021b4be0.usedList;
    data_ov013_021b4be0.usedList = session;
    return session;
}



static int func_ov013_021b2564(VCTSession *session, int command)
{
    VCTControlPacket packet;
    int result = func_ov013_021b2010(session, command, &packet, sizeof(packet));

    if (result != 0) {
        return result;
    }
    if (!func_ov013_021b275c(session, &packet)) {
        return -4;
    }
    return 0;
}

static int func_ov013_021b25b0(VCTSession *session, int command)
{
    VCTControlPacket packet;
    int result = func_ov013_021b20d4(session, command, &packet, sizeof(packet));

    if (result != 0) {
        return result;
    }
    if (!func_ov013_021b275c(session, &packet)) {
        return -4;
    }
    return 0;
}

static void func_ov013_021b25fc(const VCTSession *session, VCTControlPacket *packet)
{
    if (packet->flag == 1) {
        packet->origin = session->aid;
        packet->argument = data_ov013_021b4bd4;
    }
}

int func_ov013_021b2620(VCTSession *session, VCTControlPacket *packet) {
    int sent = 0;
    u8 aid;

    func_ov013_021b25fc(session, packet);
    for (aid = 0; aid < 0x20; aid++) {
        if ((data_ov013_021b4bd4 & (aid != 0 ? 1 << aid : 1)) && aid != data_ov013_021b60d8.aid) {
            packet->to = aid;
            if (DWC_SendReliable(aid, packet, sizeof(VCTControlPacket))) {
                sent++;
            }
        }
    }
    if (packet->flag == 1) {
        session->mask = data_ov013_021b4bd4;
    } else if (packet->flag == 0) {
        session->mask = 0;
    }
    return sent == 0 ? -4 : 0;
}


static void func_ov013_021b26dc(int flag) {
    VCTControlPacket packet;

    packet.type = 0xff;
    func_ov013_021b2848(&data_ov013_021b4be0.host, &packet, 4);
    packet.flag = flag;
    func_ov013_021b2620(&data_ov013_021b4be0.host, &packet);
}


void func_ov013_021b2724(VCTSession *session, u8 aid) {
    memset(session, 0, sizeof(VCTSession));
    session->active = data_ov013_021b60d8.mode;
    session->state = 0;
    session->aid = aid;
}

static BOOL func_ov013_021b275c(const VCTSession *session, VCTControlPacket *packet) {
    if (packet->type == 0xff && packet->command == 4) {
        func_ov013_021b2620((VCTSession *)session, packet);
    } else {
        if (!DWC_SendReliable(session->aid, packet, sizeof(VCTControlPacket))) {
            return FALSE;
        }
    }
    return TRUE;
}



// The event that a control packet gives the application, if any
static int func_ov013_021b279c(VCTControlPacket *packet, VCTSession *session) {
    session->origin = packet->origin;
    if (packet->type == 0xff) {
        if (packet->command == 0) {
            session->mask = session->aid != 0 ? 1 << session->aid : 1;
        }
        return data_ov013_021b3af0[packet->command];
    } else if (packet->type == 0) {
        if (packet->command == 0) {
            if (session->state == 1) {
                session->mask = session->aid != 0 ? 1 << session->aid : 1;
                return 7;
            }
            return session->state == 5 ? 9 : 12;
        }
        return data_ov013_021b3af5[packet->command];
    }
    return 12;
}

int func_ov013_021b2848(VCTSession *session, VCTControlPacket *packet, u8 command) {
    packet->magic = 0x5f564354;
    packet->length = 0x10;
    packet->command = command;
    packet->from = data_ov013_021b60d8.aid;
    packet->to = session->aid;
    packet->flag = 0;
    packet->origin = data_ov013_021b60d8.aid;
    packet->argument = 0;
    return 0;
}

static int func_ov013_021b2894(VCTSession *session, const VCTControlPacket *packet)
{
    int column = packet->command;
    int next;
    if (packet->type == 0) {
        column += 5;
    }
    if ((u32)column >= 11) {
        column = -1;
    }
    if (column < 0) {
        return -1;
    }
    if (data_ov013_021b60d8.mode == 1) {
        next = data_ov013_021b3bc8[session->state][column];
    } else {
        next = data_ov013_021b3afc[session->state][column];
    }
    switch (next) {
    case -1:
        return next;
    case -2:
        func_ov013_021b25b0(session, 1);
        session->state = 0;
        session->active = 0;
        return next;
    case -3:
        func_ov013_021b25b0(session, 3);
        session->state = 0;
        session->active = 0;
        return next;
    default:
        return next;
    }
}



static BOOL func_ov013_021b2958(const VCTSession *session)
{
    VCTSession *other;

    for (other = data_ov013_021b4be0.usedList; other != NULL; other = other->next) {
        if (other->active != 0 && other->state == 2 && session != other) {
            return FALSE;
        }
    }
    return TRUE;
}

static int func_ov013_021b29a4(VCTSession *session, const VCTControlPacket *packet)
{
    int column = packet->command;
    int next;

    if (packet->type == 0) {
        column += 5;
    }
    if ((u32)column >= 11) {
        column = -1;
    }
    if (column < 0) {
        return -1;
    }
    if (data_ov013_021b60d8.mode == 1) {
        next = data_ov013_021b3b40[session->state][column];
    } else {
        next = data_ov013_021b3b84[session->state][column];
    }
    if (next == -1) {
        return TRUE;
    }
    if (next == -2) {
        return FALSE;
    }
    session->state = next;
    return TRUE;
}


VCTSession *func_ov013_021b2a20(int aid) {
    VCTSession *session = data_ov013_021b4be0.usedList;

    if (data_ov013_021b60d8.mode == 2) {
        if (data_ov013_021b4be0.host.active != 0 && data_ov013_021b4be0.host.aid == aid) {
            return &data_ov013_021b4be0.host;
        }
        return NULL;
    }
    for (; session != NULL; session = session->next) {
        if (session->active != 0 && session->aid == aid) {
            return session;
        }
    }
    return NULL;
}


static BOOL func_ov013_021b2aa0(const VCTControlPacket *packet, VCTEventResult *result)
{
    VCTSession *session;
    int next;

    if (packet->type == VCT_TYPE_BROADCAST) {
        if (packet->command == 4) {
            return func_ov013_021b2dd0(packet, result);
        }
        if (packet->command == 3) {
            return FALSE;
        }
    }
    session = func_ov013_021b2a20(packet->from);
    if (session != NULL) {
        next = func_ov013_021b2894(session, packet);
        switch (next) {
        case -1:
            return FALSE;
        case -2:
            result->event = 12;
            session->active = 0;
            result->session = session;
            return TRUE;
        }
        result->event = func_ov013_021b279c(packet, session);
        result->session = session;
        session->state = next;
        return result->event != 0;
    }
    session = func_ov013_021b24fc(packet->from);
    if (session == NULL) {
        func_ov013_021b2724(&data_ov013_021b4be0.pending, packet->from);
        func_ov013_021b25b0(&data_ov013_021b4be0.pending, 3);
        return FALSE;
    }
    next = func_ov013_021b2894(session, packet);
    if (next == -2 || next == -1) {
        func_ov013_021b1f54(session);
        return FALSE;
    }
    result->event = func_ov013_021b279c(packet, session);
    result->session = session;
    session->state = next;
    return result->event != 0;
}

static inline BOOL HandleHostNext(VCTControlPacket *packet, VCTEventResult *result, VCTSession *host, int next) {
    result->event = func_ov013_021b279c(packet, host);
    result->session = host;
    host->state = next;
    if (result->event == 10) {
        func_ov013_021b26dc(1);
        data_ov013_021b4bd8 = OS_GetTick();
    } else {
        func_ov013_021b26dc(0);
        data_ov013_021b4bd8 = 0;
    }
    return TRUE;
}

static BOOL func_ov013_021b2be0(VCTControlPacket *packet, VCTEventResult *result) {
    VCTSession *host;
    int next;

    if (data_ov013_021b4be0.host.active == 2) {
        if (data_ov013_021b4be0.host.aid == packet->from) {
            host = &data_ov013_021b4be0.host;
            next = func_ov013_021b2894(host, packet);
            switch (next) {
            case -2:
                result->event = 12;
                result->session = NULL;
                return TRUE;
            case -3:
            case -1:
                return FALSE;
            default:
                result->event = func_ov013_021b279c(packet, host);
                result->session = host;
                result->session->state = next;
                break;
            }
        } else {
            func_ov013_021b26dc(1);
            return FALSE;
        }
    } else {
        u32 bit = packet->from != 0 ? 1 << packet->from : 1;
        if (!(data_ov013_021b4bd4 & bit)) {
            return FALSE;
        }
        data_ov013_021b4be0.host.aid = packet->from;
        data_ov013_021b4be0.host.active = 2;
        host = &data_ov013_021b4be0.host;
        data_ov013_021b4be0.host.state = 0;
        next = func_ov013_021b2894(host, packet);
        switch (next) {
        case -3:
        case -2:
        case -1:
            data_ov013_021b4be0.host.active = 0;
            return FALSE;
        }
        result->event = func_ov013_021b279c(packet, host);
        result->session = host;
        result->session->state = next;
    }
    if (result->event == 10) {
        func_ov013_021b26dc(1);
        data_ov013_021b4bd8 = OS_GetTick();
    } else {
        func_ov013_021b26dc(0);
        data_ov013_021b4bd8 = 0;
    }
    return TRUE;
}

static BOOL func_ov013_021b2d54(const VCTControlPacket *packet, VCTEventResult *result)
{
    VCTSession *session;

    if (packet->type == VCT_TYPE_BROADCAST && packet->command == 4) {
        return FALSE;
    }
    session = func_ov013_021b2a20(packet->from);
    if (session != NULL) {
        result->event = 2;
        result->session = session;
        session->active = 0;
        func_ov013_021b25b0(session, 2);
        return TRUE;
    }
    func_ov013_021b2724(&data_ov013_021b4be0.pending, packet->from);
    func_ov013_021b25b0(&data_ov013_021b4be0.pending, 2);
    return FALSE;
}

BOOL func_ov013_021b2dd0(VCTControlPacket *packet, VCTEventResult *result) {
    if (data_ov013_021b60d8.mode != 2) {
        return FALSE;
    }
    if (data_ov013_021b60d8.hostAid != packet->from) {
        return FALSE;
    }
    switch (packet->flag) {
    case 0:
        if (data_ov013_021b4be0.host.active == 0) {
            return FALSE;
        }
        if (data_ov013_021b4be0.host.state == 5 || data_ov013_021b4be0.host.state == 2) {
            result->event = 9;
        } else {
            result->event = 5;
        }
        data_ov013_021b4be0.host.active = 0;
        data_ov013_021b4be0.host.state = 0;
        result->session = &data_ov013_021b4be0.host;
        break;
    case 1:
        data_ov013_021b4be0.host.active = 2;
        if (packet->origin == data_ov013_021b60d8.aid) {
            data_ov013_021b4be0.host.state = 2;
            result->event = 7;
            data_ov013_021b4be0.host.mask = packet->argument;
        } else {
            data_ov013_021b4be0.host.state = 3;
            result->event = 6;
            data_ov013_021b4be0.host.mask = packet->origin != 0 ? 1 << packet->origin : 1;
        }
        data_ov013_021b4be0.host.origin = packet->origin;
        result->session = &data_ov013_021b4be0.host;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


