// libdwcutil's dwc_moveid.c (Black's ov016 0x021d3b18-0x021d486c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "system/dwc_backup.h"
#include "gfl/dwc_mov_wh.h"

static s32 data_ov016_021de668;
void DWCi_MOV_MBP_StartDownloadAll(void);
const MBUserInfo * MB_CommGetChildUser (u16 child_aid);
void DWCi_MOV_MBP_Init(u32 param_1, u32 param_2);
void Parent_InitDataShare(void);
u32 JudgeConnectableChild(s32 param_1);
u16 DWCi_MOV_MBP_GetChildBmp(s32 param_1);
void DWCi_MOV_MBP_StartRebootAll(void);
void Parent_StepDataShare(u16 param_1, u16 param_2, void *param_3);
void Parent_data_receive_progress(void);
u16 Parent_get_child_data_kind(void);
void func_ov016_021d3cd0(void);
void DWCi_MOVGetStatus(u8 *arg0, s8 *arg1);
void _DWCi_MOVSearchChannel(void);
void _DWCi_MOVStartWaitChild(void);
void _DWCi_MOV_WaitandDownloadandBoot_Child(void);
void _DWCi_MOV_ReadyMovUserID(void);
void _DWCi_MOV_MovUserID(void);
void _DWCi_MOV_EraseFlashData(void);
u32 _DWCi_MOVEnd(void);

void func_ov016_021d3b18(s32 param_1, u32 *param_2) {
    data_ov016_021de668 = param_1;
    DWCi_MOV_MBP_MemInit(param_1 + 0xB00);
    *(s16 *)((u8 *)data_ov016_021de668 + 0x648) = 0;
    *(s16 *)((u8 *)data_ov016_021de668 + 0x64A) = 0;
    *(s8 *)((u8 *)data_ov016_021de668 + 0xA90) = 1;
    *(s8 *)((u8 *)data_ov016_021de668 + 0xA91) = 1;
    *(s32 *)((u8 *)data_ov016_021de668 + 0xA9C) = 0;
    Parent_InitDataShare();
    *(u32 *)((u8 *)data_ov016_021de668 + 0xAB4) = (u32) *(u32 *)param_2;
    *(s32 *)((u8 *)data_ov016_021de668 + 0xAB8) = (s32) *(s32 *)((u8 *)param_2 + 4);
    *(s32 *)((u8 *)data_ov016_021de668 + 0xABC) = (s32) *(s32 *)((u8 *)param_2 + 8);
    *(s32 *)((u8 *)data_ov016_021de668 + 0xAC0) = (s32) *(s32 *)((u8 *)param_2 + 0xC);
    *(s32 *)((u8 *)data_ov016_021de668 + 0xAC4) = (s32) *(s32 *)((u8 *)param_2 + 0x10);
    *(s32 *)((u8 *)data_ov016_021de668 + 0xAC8) = (s32) *(s32 *)((u8 *)param_2 + 0x14);
    *(u8 *)((u8 *)data_ov016_021de668 + 0xA92) = (u8) *(u8 *)((u8 *)param_2 + 0x18);
    *(s8 *)((u8 *)data_ov016_021de668 + 0xACC) = 2;
    OS_GetTick();
    func_02058770((DWCMemPage *)(data_ov016_021de668 + 0x64C));
    OS_GetTick();
    *(u32 *)((u8 *)data_ov016_021de668 + 0xAA4) = DWCi_SETTINGlGetMemMap();
}

BOOL DWCi_MOVFinalize(void) {
    u8 state = *(u8 *)(data_ov016_021de668 + 0xa90);

    if (state == 1 || state == 0x14 || state == 0x17 || state == 0x1a || state == 0x1d) {
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x22;
        *(u8 *)(data_ov016_021de668 + 0xaac) = 0;
        return TRUE;
    }
    if (state == 4 || state == 5 || state == 6 || state == 0xd) {
        if (state == 4 && *(u32 *)(data_ov016_021de668 + 0xa98) < 6) {
            return FALSE;
        }
        MB_EndToIdle();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x10;
        *(u8 *)(data_ov016_021de668 + 0xaac) = 2;
        return TRUE;
    }
    if ((u8)(state + 0xf7) <= 1) {
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x20;
        return TRUE;
    }
    if (state == 0xc) {
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x22;
        return TRUE;
    }
    return FALSE;
}


void func_ov016_021d3cd0(void) {
    DWCi_MOV_WH_SetGgid(*(u32 *)(data_ov016_021de668 + 0xac8));
    *(u8 *)(data_ov016_021de668 + 0xa90) = 1;
    *(u16 *)(data_ov016_021de668 + 0x648) = WM_GetNextTgid();
    MI_CpuCopy8(*(void **)(data_ov016_021de668 + 0xaa4), (void *)(data_ov016_021de668 + 0xa50), 0x40);
    *(u8 *)(data_ov016_021de668 + 0xa93) = 0;
    *(u32 *)(data_ov016_021de668 + 0x204) = 0;
    *(u16 *)(data_ov016_021de668 + 0x648) = *(u16 *)(data_ov016_021de668 + 0x648) + 1;
}

u32 DWCi_MOVStartAdmitChild(void) {
    u8 val;

    val = *(u8 *)(data_ov016_021de668 + 0xa90);
    if ((val == 1 || val == 0x1a) || val == 0x1d) {
        func_ov016_021d3cd0();
        DWCi_MOV_WH_Initialize();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 2;
        return 1;
    }
    return 0;
}

u32 DWCi_MOVStartDownloadandBootChild(void) {
    if (*(u8 *)(data_ov016_021de668 + 0xa90) == 5) {
        *(u8 *)(data_ov016_021de668 + 0xa90) = 6;
        DWCi_MOV_MBP_StartDownloadAll();
        return 1;
    }
    return 0;
}

void DWCi_MOVFramework(void) {
    u8 flag;

    switch (*(u8 *)(data_ov016_021de668 + 0xa90)) {
    case 1:
        flag = *(u8 *)((u8 *)data_ov016_021de668 + 0xaac);
        if (flag == 1) {
            *(u8 *)((u8 *)data_ov016_021de668 + 0xaac) = 0;
            DWCi_MOVStartAdmitChild();
            return;
        } else if (flag == 2) {
            *(u8 *)((u8 *)data_ov016_021de668 + 0xaac) = 0;
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x22;
        }
        return;
    case 2:
        _DWCi_MOVSearchChannel();
        return;
    case 3:
        _DWCi_MOVStartWaitChild();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 4;
        return;
    case 4:
        *(s32 *)(data_ov016_021de668 + 0xa98) = *(s32 *)(data_ov016_021de668 + 0xa98) + 1;
        _DWCi_MOV_WaitandDownloadandBoot_Child();
        return;
    case 5:
    case 6:
        _DWCi_MOV_WaitandDownloadandBoot_Child();
        return;
    case 7:
        _DWCi_MOV_ReadyMovUserID();
        return;
    case 8:
    case 9:
    case 10:
        _DWCi_MOV_MovUserID();
        return;
    case 11:
        _DWCi_MOV_EraseFlashData();
        return;
    case 0x10:
        *(u32 *)(data_ov016_021de668 + 0xa9c) = 0;
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x11;
        return;
    case 0x11:
        if ((*(u32 *)(data_ov016_021de668 + 0xa9c))++ <= 0x1e) {
            return;
        }
        _DWCi_MOVEnd();
        return;
    case 0x12:
        MB_EndToIdle();
        *(u32 *)(data_ov016_021de668 + 0xa9c) = 0;
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x16;
        return;
    case 0x13:
        if ((*(u32 *)(data_ov016_021de668 + 0xa9c))++ <= 0x1e) {
            return;
        }
        if (DWCi_MOV_WH_GetSystemState() != 1) {
            return;
        }
        DWCi_MOV_WH_End();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x14;
        return;
    case 0x15:
        DWCi_MOV_WH_Finalize();
        *(u32 *)(data_ov016_021de668 + 0xa9c) = 0;
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x16;
        return;
    case 0x16:
        if ((*(u32 *)(data_ov016_021de668 + 0xa9c))++ <= 0x1e) {
            return;
        }
        if (DWCi_MOV_WH_GetSystemState() != 1) {
            return;
        }
        DWCi_MOV_WH_End();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x17;
        return;
    case 0x18:
        DWCi_MOV_WH_Finalize();
        *(u32 *)(data_ov016_021de668 + 0xa9c) = 0;
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x19;
        return;
    case 0x19:
        if ((*(u32 *)(data_ov016_021de668 + 0xa9c))++ <= 0x1e) {
            return;
        }
        if (DWCi_MOV_WH_GetSystemState() != 1) {
            return;
        }
        DWCi_MOV_WH_End();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1a;
        return;
    case 0x1b:
        DWCi_MOV_WH_Finalize();
        *(u32 *)(data_ov016_021de668 + 0xa9c) = 0;
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1c;
        return;
    case 0x1c:
        if ((*(u32 *)(data_ov016_021de668 + 0xa9c))++ <= 0x1e) {
            return;
        }
        if (DWCi_MOV_WH_GetSystemState() == 1) {
            DWCi_MOV_WH_End();
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1d;
            return;
        }
        if (*(u32 *)(data_ov016_021de668 + 0xa9c) % 0x1e != 1) {
            return;
        }
        if (*(u32 *)(data_ov016_021de668 + 0xa9c) <= 0x37) {
            return;
        }
        DWCi_MOV_WH_Finalize();
        return;
    case 0x20:
        DWCi_MOV_WH_Finalize();
        *(u32 *)(data_ov016_021de668 + 0xa9c) = 0;
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x21;
        return;
    case 0x21:
        if ((*(u32 *)(data_ov016_021de668 + 0xa9c))++ <= 0x1e) {
            return;
        }
        if (DWCi_MOV_WH_GetSystemState() != 1) {
            return;
        }
        DWCi_MOV_WH_End();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 0x22;
        return;
    case 0x1e:
        DWCi_MOV_WH_Finalize();
        return;
    case 0x22:
        return;
    }
}


void DWCi_MOVGetStatus(u8 *stt, s8 *change_f) {
    *stt = *(u8 *)(data_ov016_021de668 + 0xA90);
    if (*(u8 *)(data_ov016_021de668 + 0xA90) != *(u8 *)(data_ov016_021de668 + 0xA91)) {
        *change_f = 1;
    } else {
        *change_f = 0;
    }
    *(u8 *)(data_ov016_021de668 + 0xA91) = *(u8 *)(data_ov016_021de668 + 0xA90);
}

const MBUserInfo *DWCi_GetChildInfo(void) {
    return MB_CommGetChildUser(1);
}

void _DWCi_MOVSearchChannel(void) {
    switch (DWCi_MOV_WH_GetSystemState()) {
    case 1:
        DWCi_MOV_WH_StartMeasureChannel();
        break;
    case 7:
        *(u16 *)(data_ov016_021de668 + 0x64A) = DWCi_MOV_WH_GetMeasureChannel();
        *(s32 *)(data_ov016_021de668 + 0xA98) = 0;
        *(s8 *)(data_ov016_021de668 + 0xA90) = 3;
        break;
    case 0:
        *(s32 *)(data_ov016_021de668 + 0xA98) = 0;
        *(s8 *)(data_ov016_021de668 + 0xA90) = 3;
        break;
    case 9:
        DWCi_MOV_WH_Reset();
        break;
    case 3:
        break;
    default:
        OS_Terminate();
        break;
    }
}

void _DWCi_MOVStartWaitChild(void) {
    DWCi_MOV_MBP_Init(*(u32 *)(data_ov016_021de668 + 0xac8),
                                            (u32)*(u16 *)(data_ov016_021de668 + 0x648));
}

void _DWCi_MOV_WaitandDownloadandBoot_Child(void) {
    u32 systemState;

    switch ((u32) DWCi_MOV_MBP_GetState()) {
    default:
        break;
    case 1:
        DWCi_MOV_MBP_Start((MBGameRegistry *)(data_ov016_021de668 + 0xAB4), (s32) *(u16 *)((u8 *)data_ov016_021de668 + 0x64A));
        return;
    case 2:
        if (DWCi_MOV_MBP_GetChildBmp(2) != 0) {
            *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 5U;
            return;
        }
        if ((DWCi_MOV_MBP_GetChildBmp(3) != 0) || (DWCi_MOV_MBP_GetChildBmp(4) != 0)) {
            *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 6U;
            return;
        }
        if (*(u8 *)((u8 *)data_ov016_021de668 + 0xA90) != 5) {
            return;
        }
        if (DWCi_MOV_MBP_GetChildBmp(2) == 0) {
            *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 0xDU;
        }
        return;
    case 3:
        if (DWCi_MOV_MBP_IsBootableAll() != 0) {
            DWCi_MOV_MBP_StartRebootAll();
            return;
        }
        if ((u32) (u8) (*(u8 *)((u8 *)data_ov016_021de668 + 0xA90) + 0xFA) > 1U) {
            return;
        }
        if (DWCi_MOV_MBP_GetChildBmp(3) == 0) {
            *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 0x12U;
        }
        return;
    case 5:
        *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 7U;
        return;
    case 7:
        DWCi_MOV_MBP_Cancel();
        *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 1U;
        return;
    case 0:
        systemState = DWCi_MOV_WH_GetSystemState();
        switch (systemState) {
        case 1:
            DWCi_MOV_WH_End();
            return;
        case 3:
            return;
        case 0:
            *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 0x1FU;
            return;
        default:
            *(u8 *)((u8 *)data_ov016_021de668 + 0xA90) = 0x1FU;
            return;
        }
        return;
    }
}

void _DWCi_MOV_ReadyMovUserID(void) {
    Parent_InitDataShare();
    DWCi_MOV_WH_SetJudgeAcceptFunc((u32)JudgeConnectableChild);
    *(u8 *)(data_ov016_021de668 + 0xa90) = 8;
}

void _DWCi_MOV_MovUserID(void) {
    switch (DWCi_MOV_WH_GetSystemState()) {
    case 1: {
        u16 tgid = *(u16 *)(data_ov016_021de668 + 0x648);
        func_ov016_021d376c(4, tgid + 1, *(u16 *)(data_ov016_021de668 + 0x64a), tgid);
        break;
    }
    case 4:
    case 5:
    case 6:
        Parent_StepDataShare(0, *(u16 *)(data_ov016_021de668 + 0x200),
                (void *)(*(s32 *)(data_ov016_021de668 + 0xaa4) + (*(u16 *)(data_ov016_021de668 + 0x200) % 16) * 0x40));
        Parent_data_receive_progress();
        if (*(u8 *)(data_ov016_021de668 + 0xa90) == 0x1b) {
            return;
        }
        if (*(u32 *)(data_ov016_021de668 + 0x204) > 0x1e0) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1b;
            return;
        }
        if (Parent_get_child_data_kind() == 0x10 || Parent_get_child_data_kind() == 0x20) {
            Parent_get_child_data_kind();
            *(u8 *)(data_ov016_021de668 + 0xa90) = 10;
            return;
        }
        if (Parent_get_child_data_kind() == 0x40) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0xb;
            return;
        }
        if (Parent_get_child_data_kind() == 0xff) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1b;
            return;
        }
        if (Parent_get_child_data_kind() == 0x50) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x15;
            return;
        }
        if (Parent_get_child_data_kind() == 0x60) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x18;
            return;
        }
        if (Parent_get_child_data_kind() == 0x70) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1b;
            return;
        }
        if (Parent_get_child_data_kind() == 0) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 8;
            return;
        }
        if (Parent_get_child_data_kind() == 0xbd) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 9;
        } else {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1f;
        }
        break;
    }
}

void _DWCi_MOV_EraseFlashData(void) {
    *(u8 *)(data_ov016_021de668 + 0xa90) = 0xc;
}

u32 _DWCi_MOVEnd(void) {
    if (DWCi_MOV_WH_GetSystemState() == 1) {
        DWCi_MOV_WH_End();
        *(u8 *)(data_ov016_021de668 + 0xa90) = 1;
        return 1;
    }
    return 0;
}

u32 JudgeConnectableChild(s32 cb) {
    u32 playerNo;

    playerNo = (u32)DWCi_MOV_MBP_GetPlayerNo((u8 *)(cb + 10));
    if (playerNo != 0) {
        *(s32 *)(data_ov016_021de668 + (playerNo - 1) * 4 + 0xaa0) = DWCi_MOV_MBP_GetChildInfo(playerNo);
        return 1;
    }
    return 0;
}

void Parent_InitDataShare(void) {
    MI_CpuFill8((void *)((s32)(u8 *)data_ov016_021de668 + 0x100), 0, 0x100);
    MI_CpuFill8((void *)data_ov016_021de668, 0, 0x100);
    *(void **)((s32)(u8 *)data_ov016_021de668 + 0xab0) = (void *)data_ov016_021de668;
}

void Parent_StepDataShare(u16 kind, u16 counter, void *data) {
    u16 i;

    if (*(u8 *)(data_ov016_021de668 + 0xa93) == 1) {
        **(u16 **)(data_ov016_021de668 + 0xab0) = kind;
        *(u16 *)(*(s32 *)(data_ov016_021de668 + 0xab0) + 2) = counter;
        MI_CpuCopy8(data, (void *)(*(s32 *)(data_ov016_021de668 + 0xab0) + 4), 0x40);
    } else {
        *(s32 *)(data_ov016_021de668 + 0x204) = *(s32 *)(data_ov016_021de668 + 0x204) + 1;
        **(u16 **)(data_ov016_021de668 + 0xab0) = 0xbc;
        *(u8 *)(*(s32 *)(data_ov016_021de668 + 0xab0) + 4) = *(u8 *)(data_ov016_021de668 + 0xa92);
    }
    if (DWCi_MOV_WH_GetSystemState() != 5) {
        return;
    }
    if (func_ov016_021d3904((u16 *)data_ov016_021de668) == 0) {
        *(s32 *)(data_ov016_021de668 + 0x204) = *(s32 *)(data_ov016_021de668 + 0x204) + 4;
        return;
    }
    if (*(u8 *)(data_ov016_021de668 + 0xa93) == 0) {
        *(s32 *)(data_ov016_021de668 + 0x204) = *(s32 *)(data_ov016_021de668 + 0x204) + 1;
    } else {
        *(s32 *)(data_ov016_021de668 + 0x204) = 0;
        if (DWCi_MOV_WH_GetBitmap() != 3) {
            *(u8 *)(data_ov016_021de668 + 0xa90) = 0x1b;
            return;
        }
    }
    for (i = 0; i < 2; i++) {
        u16 *src = DWCi_MOV_WH_GetSharedDataAdr(i);
        if (src != NULL) {
            MI_CpuCopy8(src, (u8 *)(data_ov016_021de668 + 0x100) + i * 0x44, 0x44);
            *(s32 *)(data_ov016_021de668 + i * 4 + 0x208) = 1;
        } else {
            *(s32 *)(data_ov016_021de668 + i * 4 + 0x208) = 0;
        }
    }
}


typedef struct ParentShareEntry {
    u16 kind;
    u16 counter;
    u8 data[64];
} ParentShareEntry;
void Parent_data_receive_progress(void) {
    s32 offset;
    s32 entries;
    s32 i;

    /* FAKEMATCH(goto-loop): a for loop over the 16 entries gives other registers (48 lines). The prebuilt twin
       (libdwcutil.a dwc_moveid.o, the same code) has `s32 i` in r12 and `GShareData *sd` at line 1116; its line
       table puts i = 0 on line 1112 and the increment and compare on line 1157. Round 37: for, do/while (++i < 16),
       while (TRUE) with a break or a return, with sd = &share[i] at either level: all 48 lines, i in r3 */
    i = 0;
loop_1:
    if (*(s32 *)((u8 *)((data_ov016_021de668 + (i * 4))) + 0x208) != 0) {
        entries = data_ov016_021de668 + 0x100;
        offset = i * 0x44;
        if (i == 1) {
            if (*(u8 *)((u8 *)data_ov016_021de668 + 0xA93) == 1) {
                if (*(u16 *)(entries + offset) != 0x10) {
                    return;
                }
                *(s32 *)((u8 *)data_ov016_021de668 + 0xA94) = (s32) (*(s32 *)((u8 *)data_ov016_021de668 + 0xA94) + 1);
                if (!(*(s32 *)((u8 *)data_ov016_021de668 + 0xA94) & 1)) {
                    *(u16 *)((u8 *)data_ov016_021de668 + 0x200) = (u16) (*(u16 *)((u8 *)data_ov016_021de668 + 0x200) + 1);
                    if ((u32) *(u16 *)((u8 *)data_ov016_021de668 + 0x200) >= 0x24U) {
                        *(u16 *)((u8 *)data_ov016_021de668 + 0x200) = 0U;
                    }
                }
                goto block_12;
            }
            *(s16 *)((u8 *)data_ov016_021de668 + 0x202) = 0xBC;
            if (*(u16 *)(entries + offset) == 0xBD) {
                *(u8 *)((u8 *)data_ov016_021de668 + 0xA93) = 1U;
                *(u16 *)((u8 *)data_ov016_021de668 + 0x200) = 0U;
                *(s32 *)((u8 *)data_ov016_021de668 + 0xA94) = 0;
            }
            goto block_12;
        }
    }
block_12:
    i += 1;
    if (i >= 0x10) {
        return;
    }
    goto loop_1;
}

u16 Parent_get_child_data_kind(void) {
    return *(u16 *)(data_ov016_021de668 + 0x144);
}
