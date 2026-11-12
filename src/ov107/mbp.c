// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov181/mbp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mb_parent/mbp.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "nitro/fs.h"
#include "nitro/mb.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "nitro/wm.h"
#include "work_area.h"

// The DS Download Play parent of NitroSDK's demos (mbp.c), which Game Freak took over with its heap, its asserts and
// a fixed-size file buffer. It hands the child program to the library and tracks each child's state in bitmaps by AID

// How many children may connect
#define MBP_CHILD_MAX 1
// The size of the file buffer, which holds the child program's segment
#define MBP_FILE_BUF_SIZE 0x10000
#define MBP_DMA_NO 1

// The state of each child
enum {
    MBP_CHILDSTATE_NONE,
    MBP_CHILDSTATE_CONNECTING,
    MBP_CHILDSTATE_REQUEST,
    MBP_CHILDSTATE_ENTRY,
    MBP_CHILDSTATE_DOWNLOADING,
    MBP_CHILDSTATE_BOOTABLE,
    MBP_CHILDSTATE_REBOOT,
};

typedef struct {
    u16 state;
    // The children in each state, by AID
    u16 connectChildBmp;
    u16 requestChildBmp;
    u16 entryChildBmp;
    u16 downloadChildBmp;
    u16 bootableChildBmp;
    u16 rebootChildBmp;
    // Whether the library runs
    BOOL started;
} MBPState;

typedef struct {
    MBUserInfo user;
    u8 macAddress[6];
    u16 playerNo;
} MBPChildInfo;

BOOL func_ov107_021f0850(const MBGameRegistry *gameInfo);
void func_ov107_021f08e4(u16 childAid);
void func_ov107_021f0938(u16 childAid);
void func_ov107_021f09b0(u16 childAid);
void func_ov107_021f0b04(void);
static void func_ov107_021f0b2c(u16 childAid, u32 status, void *arg);
static void func_ov107_021f0d1c(u16 state);
int func_ov107_021f0d34(u16 childAid);

typedef struct {
    u8 *filebuf;
    // The library's work
    void *cWork;
    MBPState mbpState;
    MBPChildInfo childInfo[MBP_CHILD_MAX];
} MBPWork;

static MBPWork data_ov107_021f6eac;

static inline void MBP_DisconnectChildFromBmp(u16 aid) {
    u16 aidMask = ~(1 << aid);
    u32 enabled = OS_DisableInterrupts();

    data_ov107_021f6eac.mbpState.connectChildBmp &= aidMask;
    data_ov107_021f6eac.mbpState.requestChildBmp &= aidMask;
    data_ov107_021f6eac.mbpState.entryChildBmp &= aidMask;
    data_ov107_021f6eac.mbpState.downloadChildBmp &= aidMask;
    data_ov107_021f6eac.mbpState.bootableChildBmp &= aidMask;
    data_ov107_021f6eac.mbpState.rebootChildBmp &= aidMask;
    OS_RestoreInterrupts(enabled);
}

static inline void MBP_DisconnectChild(u16 aid) {
    MBP_DisconnectChildFromBmp(aid);
    MB_DisconnectChild(aid);
}

// pokebw2's MBP_AcceptChild


int MB_Init (void * work, const MBUserInfo * user, u32 ggid, u32 tgid, u32 dma);
BOOL MB_SetParentCommParam (u16 sendSize, u16 maxChildren);
void MB_CommSetParentStateCallback(void (*callback)(u16, u32, void *));
void func_ov107_021f0754(HeapID heapId, u32 ggid) {
    MBPState state;
    MBUserInfo myUser;
    OSOwnerInfoEx info;

    OS_GetOwnerInfoEx(&info);
    myUser.favoriteColor = info.favoriteColor;
    myUser.nameLength = info.nickNameLength;
    MI_CpuCopy8(info.nickName, myUser.name, info.nickNameLength * 2);
    myUser.playerNo = 0;
    state = (MBPState){ 0 };
    data_ov107_021f6eac.mbpState = state;
    // MB's work, whose structs and WM's (in it) hold pointers
    data_ov107_021f6eac.cWork = allocConfigDSSoftwareFeature(HEAPID_MB_PARENT, WORK_AREA_SIZE(0xb100), "mbp.c", 0x9e);
    GFL_ASSERT(MB_Init(data_ov107_021f6eac.cWork, &myUser, heapId, ggid, TRUE) == 0);
    MB_SetParentCommParam(0x0100, 1);
    MB_CommSetParentStateCallback(func_ov107_021f0b2c);
    func_ov107_021f0d1c(1);
}

int MB_StartParentFromIdle(int channel);
void func_ov107_021f080c(const MBGameRegistry *gameInfo, u16 channel) {
    func_ov107_021f0d1c(MBP_STATE_ENTRY);
    if (MB_StartParentFromIdle(channel) != 0) {
        func_ov107_021f0d1c(MBP_STATE_ERROR);
        return;
    }
    data_ov107_021f6eac.mbpState.started = TRUE;
    if (!func_ov107_021f0850(gameInfo)) {
        GFL_DebugAssertFail("", 0, "");
    }
}

u32 MB_GetSegmentLength(FSFile *file);
BOOL MB_ReadSegment(FSFile *file, void *buf, u32 size);
BOOL MB_RegisterFile(const MBGameRegistry *gameInfo, const void *buf);
BOOL func_ov107_021f0850(const MBGameRegistry *gameInfo) {
    FSFile file;
    FSFile *p_file;
    BOOL ret = FALSE;

    if (gameInfo->romFilePathp == NULL) {
        p_file = NULL;
    } else {
        p_file = &file;
        FS_InitFile(p_file);
        if (!FS_OpenFile(p_file, gameInfo->romFilePathp)) {
            return ret;
        }
    }
    if (MB_GetSegmentLength(p_file) != 0) {
        data_ov107_021f6eac.filebuf = GFL_HeapAllocate(0x5d, MBP_FILE_BUF_SIZE, TRUE, "mbp.c", 0x11f);
        if (data_ov107_021f6eac.filebuf != NULL) {
            if (MB_ReadSegment(p_file, data_ov107_021f6eac.filebuf, MBP_FILE_BUF_SIZE) && MB_RegisterFile(gameInfo, data_ov107_021f6eac.filebuf)) {
                ret = TRUE;
            }
            if (!ret) {
                GFL_HeapFree(data_ov107_021f6eac.filebuf);
            }
        }
    }
    if (p_file == &file) {
        FS_CloseFile(&file);
    }
    return ret;
}



void func_ov107_021f08e4(u16 childAid) {
    if (!MB_CommResponseRequest(childAid, MB_COMM_RESPONSE_REQUEST_ACCEPT)) {
        MBP_DisconnectChild(childAid);
    }
}

// pokebw2's MBP_KickChild
void func_ov107_021f0938(u16 childAid) {
    if (!MB_CommResponseRequest(childAid, MB_COMM_RESPONSE_REQUEST_KICK)) {
        MBP_DisconnectChild(childAid);
        return;
    }
    {
        u32 enabled = OS_DisableInterrupts();

        data_ov107_021f6eac.mbpState.requestChildBmp &= ~(1 << childAid);
        data_ov107_021f6eac.mbpState.connectChildBmp &= ~(1 << childAid);
        OS_RestoreInterrupts(enabled);
    }
}

// pokebw2's MBP_StartDownload
void func_ov107_021f09b0(u16 childAid) {
    if (!MB_CommResponseRequest(childAid, MB_COMM_RESPONSE_REQUEST_DOWNLOAD)) {
        MBP_DisconnectChild(childAid);
        return;
    }
    {
        u32 enabled = OS_DisableInterrupts();

        data_ov107_021f6eac.mbpState.entryChildBmp &= ~(1 << childAid);
        data_ov107_021f6eac.mbpState.downloadChildBmp |= 1 << childAid;
        OS_RestoreInterrupts(enabled);
    }
}

// pokebw2's MBP_IsBootableAll
BOOL func_ov107_021f0a28(void) {
    u16 aid;

    if (data_ov107_021f6eac.mbpState.connectChildBmp == 0) {
        return FALSE;
    }
    for (aid = 1; aid < 16; aid++) {
        if (!(data_ov107_021f6eac.mbpState.connectChildBmp & (1 << aid))) {
            continue;
        }
        if (!MB_CommIsBootable(aid)) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's MBP_StartRebootAll
void func_ov107_021f0a64(void) {
    u16 aid;
    u16 sentChild = 0;

    for (aid = 1; aid < 16; aid++) {
        if (!(data_ov107_021f6eac.mbpState.bootableChildBmp & (1 << aid))) {
            continue;
        }
        if (!MB_CommBootRequest(aid)) {
            MBP_DisconnectChild(aid);
            continue;
        }
        sentChild |= 1 << aid;
    }
    if (sentChild == 0) {
        func_ov107_021f0d1c(MBP_STATE_ERROR);
        return;
    }
    func_ov107_021f0d1c(MBP_STATE_REBOOTING);
}

// pokebw2's MBP_Cancel
void func_ov107_021f0ae4(void) {
    func_ov107_021f0d1c(MBP_STATE_CANCEL);
    if (data_ov107_021f6eac.mbpState.started == TRUE) {
        data_ov107_021f6eac.mbpState.started = FALSE;
        MB_EndToIdle();
    }
}

// Ends the library once every child has rebooted into the downloaded program
// pokebw2's MBP_CheckRebootEnd
void func_ov107_021f0b04(void) {
    if (data_ov107_021f6eac.mbpState.state == MBP_STATE_REBOOTING &&
        data_ov107_021f6eac.mbpState.connectChildBmp == data_ov107_021f6eac.mbpState.rebootChildBmp &&
        data_ov107_021f6eac.mbpState.started == TRUE) {
        data_ov107_021f6eac.mbpState.started = FALSE;
        MB_EndToIdle();
    }
}

const MBUserInfo * MB_CommGetChildUser (u16 child_aid);
u16 func_ov107_021f0d28(void);
static void func_ov107_021f0b2c(u16 childAid, u32 status, void *arg) {
    switch (status) {
    case MB_COMM_PSTATE_INIT_COMPLETE:
        break;
    case MB_COMM_PSTATE_CONNECTED: {
        WMStartParentCallback *cb = arg;
        u32 enabled;

        if (func_ov107_021f0d28() != MBP_STATE_ENTRY) {
            break;
        }
        enabled = OS_DisableInterrupts();
        data_ov107_021f6eac.mbpState.connectChildBmp |= 1 << childAid;
        OS_RestoreInterrupts(enabled);
        data_ov107_021f6eac.childInfo[childAid - 1].macAddress[0] = cb->macAddress[0];
        data_ov107_021f6eac.childInfo[childAid - 1].macAddress[1] = cb->macAddress[1];
        data_ov107_021f6eac.childInfo[childAid - 1].macAddress[2] = cb->macAddress[2];
        data_ov107_021f6eac.childInfo[childAid - 1].macAddress[3] = cb->macAddress[3];
        data_ov107_021f6eac.childInfo[childAid - 1].macAddress[4] = cb->macAddress[4];
        data_ov107_021f6eac.childInfo[childAid - 1].macAddress[5] = cb->macAddress[5];
        data_ov107_021f6eac.childInfo[childAid - 1].playerNo = childAid;
        break;
    }
    case MB_COMM_PSTATE_DISCONNECTED:
        if (func_ov107_021f0d34(childAid) != MBP_CHILDSTATE_REBOOT) {
            MBP_DisconnectChildFromBmp(childAid);
            func_ov107_021f0b04();
        }
        break;
    case MB_COMM_PSTATE_KICKED:
        break;
    case MB_COMM_PSTATE_REQ_ACCEPTED:
        break;
    case MB_COMM_PSTATE_SEND_PROCEED:
        break;
    case MB_COMM_PSTATE_BOOT_REQUEST:
        break;
    case MB_COMM_PSTATE_MEMBER_FULL:
        break;
    case MB_COMM_PSTATE_REQUESTED: {
        MBUserInfo *userInfo;

        if (func_ov107_021f0d28() != MBP_STATE_ENTRY) {
            func_ov107_021f0938(childAid);
            break;
        }
        data_ov107_021f6eac.mbpState.requestChildBmp |= 1 << childAid;
        func_ov107_021f08e4(childAid);
        userInfo = MB_CommGetChildUser(childAid);
        if (userInfo != NULL) {
            MI_CpuCopy8(userInfo, &data_ov107_021f6eac.childInfo[childAid - 1].user, sizeof(MBUserInfo));
        }
        break;
    }
    case MB_COMM_PSTATE_WAIT_TO_SEND:
        data_ov107_021f6eac.mbpState.requestChildBmp &= ~(1 << childAid);
        data_ov107_021f6eac.mbpState.entryChildBmp |= 1 << childAid;
        func_ov107_021f09b0(childAid);
        break;
    case MB_COMM_PSTATE_SEND_COMPLETE:
        data_ov107_021f6eac.mbpState.downloadChildBmp &= ~(1 << childAid);
        data_ov107_021f6eac.mbpState.bootableChildBmp |= 1 << childAid;
        break;
    case MB_COMM_PSTATE_BOOT_STARTABLE:
        data_ov107_021f6eac.mbpState.bootableChildBmp &= ~(1 << childAid);
        data_ov107_021f6eac.mbpState.rebootChildBmp |= 1 << childAid;
        func_ov107_021f0b04();
        break;
    case MB_COMM_PSTATE_END:
        if (func_ov107_021f0d28() == MBP_STATE_REBOOTING) {
            func_ov107_021f0d1c(MBP_STATE_COMPLETE);
        } else {
            func_ov107_021f0d1c(MBP_STATE_STOP);
        }
        if (data_ov107_021f6eac.filebuf != NULL) {
            GFL_HeapFree(data_ov107_021f6eac.filebuf);
            data_ov107_021f6eac.filebuf = NULL;
        }
        if (data_ov107_021f6eac.cWork != NULL) {
            func_0203d7fc(data_ov107_021f6eac.cWork);
            data_ov107_021f6eac.cWork = NULL;
        }
        break;
    case MB_COMM_PSTATE_ERROR: {
        MBErrorStatus *cb = arg;

        switch (cb->errcode) {
        case MB_ERRCODE_INVALID_PARAM:
        case MB_ERRCODE_INVALID_STATE:
        case MB_ERRCODE_FATAL:
            func_ov107_021f0d1c(MBP_STATE_ERROR);
            break;
        case MB_ERRCODE_WM_FAILURE:
            break;
        }
        break;
    }
    case MB_COMM_PSTATE_NONE:
    default:
        GFL_DebugAssertFail("", 0, "");
        break;
    }
}


// pokebw2's MBP_ChangeState
static void func_ov107_021f0d1c(u16 state) {
    data_ov107_021f6eac.mbpState.state = state;
}

// pokebw2's MBP_GetState
u16 func_ov107_021f0d28(void) {
    return data_ov107_021f6eac.mbpState.state;
}

// pokebw2's MBP_GetChildState
int func_ov107_021f0d34(u16 childAid) {
    MBPState tmpState;
    u16 bitmap = 1 << childAid;
    u32 enabled = OS_DisableInterrupts();

    if ((data_ov107_021f6eac.mbpState.connectChildBmp & bitmap) == 0) {
        OS_RestoreInterrupts(enabled);
        return MBP_CHILDSTATE_NONE;
    }
    MI_CpuCopy8(&data_ov107_021f6eac.mbpState, &tmpState, sizeof(MBPState));
    OS_RestoreInterrupts(enabled);
    if (tmpState.requestChildBmp & bitmap) {
        return MBP_CHILDSTATE_REQUEST;
    }
    if (tmpState.entryChildBmp & bitmap) {
        return MBP_CHILDSTATE_ENTRY;
    }
    if (tmpState.downloadChildBmp & bitmap) {
        return MBP_CHILDSTATE_DOWNLOADING;
    }
    if (tmpState.bootableChildBmp & bitmap) {
        return MBP_CHILDSTATE_BOOTABLE;
    }
    if (tmpState.rebootChildBmp & bitmap) {
        return MBP_CHILDSTATE_REBOOT;
    }
    return MBP_CHILDSTATE_CONNECTING;
}

// pokebw2's MBP_FreeBuffers
void func_ov107_021f0db4(void) {
    if (data_ov107_021f6eac.filebuf != 0) {
        GFL_HeapFree(data_ov107_021f6eac.filebuf);
        data_ov107_021f6eac.filebuf = 0;
    }
    if (data_ov107_021f6eac.cWork != 0) {
        func_0203d7fc(data_ov107_021f6eac.cWork);
        data_ov107_021f6eac.cWork = 0;
    }
}

// pokebw2's MBP_IsStarted
BOOL func_ov107_021f0ddc(void) {
    return data_ov107_021f6eac.mbpState.started;
}
