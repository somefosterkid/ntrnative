// libdwcutil's dwc_mov_mbp.c (Black's ov016 0x021d1d04-0x021d298c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "app/mb_parent/mb_fileinfo.h"
#include "app/mb_parent/mb_wm_base.h"
#include "app/mb_parent.h"
#include "nitro/wm.h"

typedef struct DWCiMovMbpState {
    u16 state;
    u16 bmp[6];
} DWCiMovMbpState;

typedef struct DWCiMovMbpChildInfo {
    MBUserInfo user;
    u8 macAddress[6];
    u16 playerNo;
} DWCiMovMbpChildInfo;

typedef struct DWCiMovMbpWork {
    DWCiMovMbpState st;
    DWCiMovMbpChildInfo child[1];
    u8 file[0x10000];
    u8 unk_1002c[0x14];
    u8 mbWork[0xb100];
    void *mbWorkp;
    void *filep;
} DWCiMovMbpWork;

// pokebw2's wb
static DWCiMovMbpWork *data_ov016_021de65c;
void DWCi_MOV_MBP_ChangeState(u16 param_1);
void ParentStateCallback(u16 childAid, u32 status, void *arg);
s32 MB_StartParentFromIdle(s32 channel);
u32 DWCi_MOV_MBP_RegistFile(MBGameRegistry *param_1);
void DWCi_MOV_MBP_AcceptChild(u16 arg0);
void MBPi_CheckAllReboot(void);
u32 DWCi_MOV_MBP_GetChildState(u16 aid);

void DWCi_MOV_MBP_MemInit(s32 mw) {
    data_ov016_021de65c = (DWCiMovMbpWork *)mw;
    DWCi_MOV_WH_MemInit(mw + 0x1b160);
    data_ov016_021de65c->mbWorkp = NULL;
    data_ov016_021de65c->filep = NULL;
}

static inline void DWCi_MOV_MBP_ClearState(DWCiMovMbpState *s) {
    s->state = 0;
    s->bmp[0] = 0;
    s->bmp[1] = 0;
    s->bmp[2] = 0;
    s->bmp[3] = 0;
    s->bmp[4] = 0;
    s->bmp[5] = 0;
}
void DWCi_MOV_MBP_Init(u32 ggid, u32 tgid) {
    MBUserInfo user;
    NVRAMConfig *config = (NVRAMConfig *)HW_NVRAM_USER_INFO;

    user.favoriteColor = 1;
    user.nameLength = config->owner.nickNameLength;
    MI_CpuCopy8(config->owner.nickName, user.name, 0x14);
    user.playerNo = 0;
    data_ov016_021de65c->st = (DWCiMovMbpState){0};
    data_ov016_021de65c->mbWorkp = data_ov016_021de65c->mbWork;
    if (MB_Init(data_ov016_021de65c->mbWorkp, &user, ggid, tgid, 2) != 0) {
        OS_Terminate();
    }
    MB_SetParentCommParam(0x100, 1);
    MB_CommSetParentStateCallback(ParentStateCallback);
    DWCi_MOV_MBP_ChangeState(1);
}


void DWCi_MOV_MBP_Start(MBGameRegistry *gameInfo, s32 channel) {
    DWCi_MOV_MBP_ChangeState(2);
    if (MB_StartParentFromIdle(channel) != 0) {
        DWCi_MOV_MBP_ChangeState(7);
        return;
    }
    if (DWCi_MOV_MBP_RegistFile(gameInfo) != 0) {
        return;
    }
    OS_Terminate();
}

u32 DWCi_MOV_MBP_RegistFile(MBGameRegistry *gameInfo) {
    FSFile file;
    FSFile *fp;
    u32 result;

    result = 0;
    if (gameInfo->romFilePathp == NULL) {
        fp = NULL;
    } else {
        fp = &file;
        FS_InitFile(fp);
        if (!FS_OpenFile(fp, gameInfo->romFilePathp)) {
            return 0;
        }
    }
    if (MB_GetSegmentLength(fp) != 0) {
        data_ov016_021de65c->filep = data_ov016_021de65c->file;
        if (data_ov016_021de65c->filep != NULL
            && MB_ReadSegment(fp, data_ov016_021de65c->filep, 0x10000) != 0
            && MB_RegisterFile(gameInfo, data_ov016_021de65c->filep) != 0) {
            result = 1;
        }
    }
    if (fp == &file) {
        FS_CloseFile(&file);
    }
    return result;
}


void DWCi_MOV_MBP_AcceptChild(u16 child_aid) {
    u16 val;
    u32 state;

    state = 1 << child_aid;
    if (MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_ACCEPT) != 0) {
        return;
    }
    val = ~state;
    state = OS_DisableInterrupts();
    data_ov016_021de65c->st.bmp[0] = (u16)(data_ov016_021de65c->st.bmp[0] & val);
    data_ov016_021de65c->st.bmp[1] = (u16)(data_ov016_021de65c->st.bmp[1] & val);
    data_ov016_021de65c->st.bmp[2] = (u16)(data_ov016_021de65c->st.bmp[2] & val);
    data_ov016_021de65c->st.bmp[3] = (u16)(data_ov016_021de65c->st.bmp[3] & val);
    data_ov016_021de65c->st.bmp[4] = (u16)(data_ov016_021de65c->st.bmp[4] & val);
    data_ov016_021de65c->st.bmp[5] = (u16)(data_ov016_021de65c->st.bmp[5] & val);
    OS_RestoreInterrupts(state);
    MB_DisconnectChild(child_aid);
}

void DWCi_MOV_MBP_KickChild(u16 aid) {
    u16 mask;
    OSIntrMode mode;

    if (MB_CommResponseRequest(aid, MB_COMM_RESPONSE_REQUEST_KICK) == 0) {
        mask = ~(1 << aid);
        mode = OS_DisableInterrupts();
        data_ov016_021de65c->st.bmp[0] &= mask;
        data_ov016_021de65c->st.bmp[1] &= mask;
        data_ov016_021de65c->st.bmp[2] &= mask;
        data_ov016_021de65c->st.bmp[3] &= mask;
        data_ov016_021de65c->st.bmp[4] &= mask;
        data_ov016_021de65c->st.bmp[5] &= mask;
        OS_RestoreInterrupts(mode);
        MB_DisconnectChild(aid);
        return;
    }
    mode = OS_DisableInterrupts();
    mask = ~(1 << aid);
    data_ov016_021de65c->st.bmp[1] &= mask;
    data_ov016_021de65c->st.bmp[0] &= mask;
    OS_RestoreInterrupts(mode);
}

void DWCi_MOV_MBP_StartDownload(u16 aid) {
    u16 mask;
    OSIntrMode mode;

    if (MB_CommResponseRequest(aid, MB_COMM_RESPONSE_REQUEST_DOWNLOAD) == 0) {
        mask = ~(1 << aid);
        mode = OS_DisableInterrupts();
        data_ov016_021de65c->st.bmp[0] &= mask;
        data_ov016_021de65c->st.bmp[1] &= mask;
        data_ov016_021de65c->st.bmp[2] &= mask;
        data_ov016_021de65c->st.bmp[3] &= mask;
        data_ov016_021de65c->st.bmp[4] &= mask;
        data_ov016_021de65c->st.bmp[5] &= mask;
        OS_RestoreInterrupts(mode);
        MB_DisconnectChild(aid);
        return;
    }
    mode = OS_DisableInterrupts();
    data_ov016_021de65c->st.bmp[2] &= ~(1 << aid);
    data_ov016_021de65c->st.bmp[3] |= (1 << aid);
    OS_RestoreInterrupts(mode);
}

void DWCi_MOV_MBP_StartDownloadAll(void) {
    u16 i;

    DWCi_MOV_MBP_ChangeState(3);
    for (i = 1; i < 0x10; i++) {
        if ((data_ov016_021de65c->st.bmp[0] & (1 << i)) && !(data_ov016_021de65c->st.bmp[1] & (1 << i))) {
            if (!(data_ov016_021de65c->st.bmp[2] & (1 << i))) {
                u16 mask = ~(1 << i);
                OSIntrMode mode = OS_DisableInterrupts();
                data_ov016_021de65c->st.bmp[0] &= mask;
                data_ov016_021de65c->st.bmp[1] &= mask;
                data_ov016_021de65c->st.bmp[2] &= mask;
                data_ov016_021de65c->st.bmp[3] &= mask;
                data_ov016_021de65c->st.bmp[4] &= mask;
                data_ov016_021de65c->st.bmp[5] &= mask;
                OS_RestoreInterrupts(mode);
                MB_DisconnectChild(i);
            } else {
                DWCi_MOV_MBP_StartDownload(i);
            }
        }
    }
}

u32 DWCi_MOV_MBP_IsBootableAll(void) {
    u16 childAid;

    if (data_ov016_021de65c->st.bmp[0] == 0) {
        return 0U;
    }
    childAid = 1;
loop_3:
    if ((data_ov016_021de65c->st.bmp[0] & (1 << childAid)) && (MB_CommIsBootable(childAid) == 0)) {
        return 0U;
    }
    childAid += 1;
    if ((u32) childAid >= 0x10U) {
        return 1U;
    }
    goto loop_3;
}

void DWCi_MOV_MBP_StartRebootAll(void) {
    u16 i;
    u32 bootBmp;

    bootBmp = 0;
    for (i = 1; i < 0x10; i++) {
        u32 bit = 1 << i;
        if (data_ov016_021de65c->st.bmp[4] & (1 << i)) {
            if (MB_CommResponseRequest(i, MB_COMM_RESPONSE_REQUEST_BOOT) != 0) {
                bootBmp |= (u16)bit;
            } else {
                u16 mask = ~bit;
                OSIntrMode mode = OS_DisableInterrupts();
                data_ov016_021de65c->st.bmp[0] &= mask;
                data_ov016_021de65c->st.bmp[1] &= mask;
                data_ov016_021de65c->st.bmp[2] &= mask;
                data_ov016_021de65c->st.bmp[3] &= mask;
                data_ov016_021de65c->st.bmp[4] &= mask;
                data_ov016_021de65c->st.bmp[5] &= mask;
                OS_RestoreInterrupts(mode);
                MB_DisconnectChild(i);
            }
        }
    }
    if (bootBmp == 0) {
        DWCi_MOV_MBP_ChangeState(7);
    } else {
        DWCi_MOV_MBP_ChangeState(4);
    }
}

void DWCi_MOV_MBP_Cancel(void) {
    DWCi_MOV_MBP_ChangeState(6);
    MB_EndToIdle();
}

void MBPi_CheckAllReboot(void) {
    if ((data_ov016_021de65c->st.state == 4) && (data_ov016_021de65c->st.bmp[0] == data_ov016_021de65c->st.bmp[5])) {
        MB_EndToIdle();
    }
}

void ParentStateCallback(u16 childAid, u32 status, void *arg) {
    static char *MB_CALLBACK_TABLE[15] = {
        "MB_COMM_PSTATE_NONE",
        "MB_COMM_PSTATE_INIT_COMPLETE",
        "MB_COMM_PSTATE_CONNECTED",
        "MB_COMM_PSTATE_DISCONNECTED",
        "MB_COMM_PSTATE_KICKED",
        "MB_COMM_PSTATE_REQ_ACCEPTED",
        "MB_COMM_PSTATE_SEND_PROCEED",
        "MB_COMM_PSTATE_SEND_COMPLETE",
        "MB_COMM_PSTATE_BOOT_REQUEST",
        "MB_COMM_PSTATE_BOOT_STARTABLE",
        "MB_COMM_PSTATE_REQUESTED",
        "MB_COMM_PSTATE_MEMBER_FULL",
        "MB_COMM_PSTATE_END",
        "MB_COMM_PSTATE_ERROR",
        "MB_COMM_PSTATE_WAIT_TO_SEND",
    };
    switch (status) {
    case MB_COMM_PSTATE_CONNECTED:
        if (DWCi_MOV_MBP_GetState() == 2) {
            DWCiMovMbpWork *work = data_ov016_021de65c;
            OSIntrMode mode = OS_DisableInterrupts();
            work->st.bmp[0] = work->st.bmp[0] | (u16)(1 << childAid);
            OS_RestoreInterrupts(mode);
            {
                u8 *mac = data_ov016_021de65c->child[childAid - 1].macAddress;
                WMStartParentCallback *cb = arg;
                mac[0] = cb->macAddress[0];
                mac[1] = cb->macAddress[1];
                mac[2] = cb->macAddress[2];
                mac[3] = cb->macAddress[3];
                mac[4] = cb->macAddress[4];
                mac[5] = cb->macAddress[5];
            }
            data_ov016_021de65c->child[childAid - 1].playerNo = childAid;
        }
        break;
    case MB_COMM_PSTATE_DISCONNECTED:
        if (DWCi_MOV_MBP_GetChildState(childAid) != 6) {
            u16 mask = ~(1 << childAid);
            OSIntrMode mode = OS_DisableInterrupts();
            data_ov016_021de65c->st.bmp[0] &= mask;
            data_ov016_021de65c->st.bmp[1] &= mask;
            data_ov016_021de65c->st.bmp[2] &= mask;
            data_ov016_021de65c->st.bmp[3] &= mask;
            data_ov016_021de65c->st.bmp[4] &= mask;
            data_ov016_021de65c->st.bmp[5] &= mask;
            OS_RestoreInterrupts(mode);
            MBPi_CheckAllReboot();
        }
        break;
    case MB_COMM_PSTATE_REQUESTED:
        if (DWCi_MOV_MBP_GetState() != 2) {
            DWCi_MOV_MBP_KickChild(childAid);
        } else {
            const MBUserInfo *userInfo;
            data_ov016_021de65c->st.bmp[1] |= (1 << childAid);
            DWCi_MOV_MBP_AcceptChild(childAid);
            userInfo = MB_CommGetChildUser(childAid);
            if (userInfo != NULL) {
                MI_CpuCopy8(userInfo, &data_ov016_021de65c->child[childAid - 1], sizeof(MBUserInfo));
            }
        }
        break;
    case MB_COMM_PSTATE_WAIT_TO_SEND:
        data_ov016_021de65c->st.bmp[1] &= ~(1 << childAid);
        data_ov016_021de65c->st.bmp[2] |= (1 << childAid);
        if (DWCi_MOV_MBP_GetState() == 3) {
            DWCi_MOV_MBP_StartDownload(childAid);
        }
        break;
    case MB_COMM_PSTATE_SEND_COMPLETE:
        data_ov016_021de65c->st.bmp[3] &= ~(1 << childAid);
        data_ov016_021de65c->st.bmp[4] |= (1 << childAid);
        break;
    case MB_COMM_PSTATE_BOOT_STARTABLE:
        data_ov016_021de65c->st.bmp[4] &= ~(1 << childAid);
        data_ov016_021de65c->st.bmp[5] |= (1 << childAid);
        MBPi_CheckAllReboot();
        break;
    case MB_COMM_PSTATE_END:
        if (DWCi_MOV_MBP_GetState() == 4) {
            DWCi_MOV_MBP_ChangeState(5);
        } else {
            DWCi_MOV_MBP_ChangeState(0);
        }
        if (data_ov016_021de65c->filep != NULL) {
            data_ov016_021de65c->filep = NULL;
        }
        if (data_ov016_021de65c->mbWorkp != NULL) {
            data_ov016_021de65c->mbWorkp = NULL;
        }
        break;
    case MB_COMM_PSTATE_ERROR:
        switch (((MBErrorStatus *)arg)->errcode) {
        case MB_ERRCODE_INVALID_PARAM:
        case MB_ERRCODE_INVALID_STATE:
        case MB_ERRCODE_FATAL:
            DWCi_MOV_MBP_ChangeState(7);
            break;
        case MB_ERRCODE_WM_FAILURE:
            break;
        }
        break;
    case MB_COMM_PSTATE_INIT_COMPLETE:
    case MB_COMM_PSTATE_KICKED:
    case MB_COMM_PSTATE_REQ_ACCEPTED:
    case MB_COMM_PSTATE_SEND_PROCEED:
    case MB_COMM_PSTATE_BOOT_REQUEST:
    case MB_COMM_PSTATE_MEMBER_FULL:
        break;
    default:
        OS_Terminate();
        break;
    }
}

void DWCi_MOV_MBP_ChangeState(u16 state) {
    static char *STATE_NAME[8] = {
        "DWCi_MOV_MBP_STATE_STOP",
        "DWCi_MOV_MBP_STATE_IDLE",
        "DWCi_MOV_MBP_STATE_ENTRY",
        "DWCi_MOV_MBP_STATE_DATASENDING",
        "DWCi_MOV_MBP_STATE_REBOOTING",
        "DWCi_MOV_MBP_STATE_COMPLETE",
        "DWCi_MOV_MBP_STATE_CANCEL",
        "DWCi_MOV_MBP_STATE_ERROR",
    };
    data_ov016_021de65c->st.state = state;
}

u16 DWCi_MOV_MBP_GetState(void) {
    return data_ov016_021de65c->st.state;
}

u16 DWCi_MOV_MBP_GetChildBmp(s32 aid) {
    // Black has this table in .data (an explicit zero), beside the two debug-print tables
#pragma explicit_zero_data on
    static u16 *BITMAP_TABLE[6] = {0};
#pragma explicit_zero_data reset
    BITMAP_TABLE[0] = &data_ov016_021de65c->st.bmp[0];
    BITMAP_TABLE[1] = &data_ov016_021de65c->st.bmp[1];
    BITMAP_TABLE[2] = &data_ov016_021de65c->st.bmp[2];
    BITMAP_TABLE[3] = &data_ov016_021de65c->st.bmp[3];
    BITMAP_TABLE[4] = &data_ov016_021de65c->st.bmp[4];
    BITMAP_TABLE[5] = &data_ov016_021de65c->st.bmp[5];
    return *BITMAP_TABLE[aid];
}


u32 DWCi_MOV_MBP_GetChildState(u16 aid) {
    DWCiMovMbpState tmpState;
    u32 enabled;
    u32 ret = 1;
    u16 mask = ret << aid;

    enabled = OS_DisableInterrupts();
    if (!(data_ov016_021de65c->st.bmp[0] & mask)) {
        OS_RestoreInterrupts(enabled);
        return 0;
    }
    MI_CpuCopy8(data_ov016_021de65c, &tmpState, sizeof(DWCiMovMbpState));
    OS_RestoreInterrupts(enabled);
    if (tmpState.bmp[1] & mask) {
        return 2;
    }
    if (tmpState.bmp[2] & mask) {
        return 3;
    }
    if (tmpState.bmp[3] & mask) {
        return 4;
    }
    if (tmpState.bmp[4] & mask) {
        return 5;
    }
    ret = (tmpState.bmp[5] & mask) ? 6 : ret;
    return ret;
}

s32 DWCi_MOV_MBP_GetChildInfo(u32 child_aid) {
    if (data_ov016_021de65c->st.bmp[0] & (1 << child_aid)) {
        return (s32)&data_ov016_021de65c->child[child_aid - 1];
    }
    return 0;
}

u16 DWCi_MOV_MBP_GetPlayerNo(u8 *macAddress) {
    DWCiMovMbpWork *work = data_ov016_021de65c;
    u16 i;

    for (i = 1; i < 2; i++) {
        if (work->st.bmp[0] & (1 << i)) {
            u8 *mac = work->child[i - 1].macAddress;
            if (macAddress[0] == mac[0]
                && macAddress[1] == mac[1]
                && macAddress[2] == mac[2]
                && macAddress[3] == mac[3]
                && macAddress[4] == mac[4]
                && macAddress[5] == mac[5]) {
                return work->child[i - 1].playerNo;
            }
        }
    }
    return 0;
}
