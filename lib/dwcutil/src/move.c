// libdwcutil's move.c (Black's ov016 0x021c1074-0x021c1488): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void taskEnd_move(u32 param_1);
void DWCi_MOVFramework(void);
void DWCi_MOVGetStatus(u8 *stt, u8 *change_f);
BOOL DWCi_MOVFinalize(void);

typedef struct {
    u8 *pfile_name;
    u16 *pgame_name;
    u16 *pgame_inst;
    u8 *picon_charname;
    u8 *picon_plttname;
    u32 ggid;
    u8 language;
    u8 dummy[3];
} DWC_MOV_DATA;

typedef struct {
    u8 unk_0[0x1e280];
    u8 child[0x18];
    void (*callback)(s32);
    s32 *task;
    u8 unk_2a0;
    u8 unk_2a1;
} DWCMoveWork;

static DWCMoveWork *data_ov016_021de590;
static DWC_MOV_DATA data_ov016_021db0b0 = {  // the prebuilt's MOVDATA
    "dwc:/move/child.srl", NULL, NULL, "dwc:/move/banner.char", "dwc:/move/banner.plt", 0x159, 0,
};
void taskFramework(u32 task);

void DWCi_MOVlInit(u32 callback) {
    data_ov016_021de590 = DWCi_HEAPlAllocEx(0x1E2A4, 0x20);
    data_ov016_021de590->callback = (void (*)(s32))callback;
    data_ov016_021de590->unk_2a0 = 0;
    data_ov016_021de590->unk_2a1 = 0;
    {
        DWC_MOV_DATA dat = data_ov016_021db0b0;

        dat.pgame_name = (u16 *)DWCi_MSGlGet(data_ov016_021de598, 0);
        dat.pgame_inst = (u16 *)DWCi_MSGlGet(data_ov016_021de598, 1);
        dat.language = kDWCi_Language();
        func_ov016_021d3b18((s32)data_ov016_021de590, (u32 *)&dat);
    }
    if (DWCi_MOVStartAdmitChild() == 0) {
        OS_Terminate();
    }
    data_ov016_021de590->task = DWCi_TSKlForm(0, (s32)taskFramework, 0, 0x78);
}

void DWCi_MOVlEnd(void) {
    DWCi_TSKlForm(0, (s32)taskEnd_move, 0, 0x78);
}

void taskEnd_move(u32 task) {
    if (!DWCi_MOVFinalize()) {
        return;
    }
    data_ov016_021de590->unk_2a1 = 1;
    DWCi_TSKlDelete(0, task);
}

BOOL DWCi_MOVlIsEnd(void) {
    return (u32)(data_ov016_021de590 == NULL);
}

void DWCi_MOVlChangeCallback(u32 callback) {
    data_ov016_021de590->callback = (void (*)(s32))callback;
}

u32 DWCi_MOVlAdmitChild(void) {
    return DWCi_MOVStartDownloadandBootChild();
}

s32 DWCi_MOVlGetChild(void) {
    return (s32)data_ov016_021de590->child;
}

void taskFramework(u32 task) {
    u8 state;
    u8 change;

    DWCi_MOVFramework();
    if (data_ov016_021de590->unk_2a0 != 0 && data_ov016_021de590->unk_2a1 == 0) {
        if (data_ov016_021de590->callback != NULL) {
            data_ov016_021de590->callback(0);
        }
        return;
    }
    DWCi_MOVGetStatus(&state, &change);
    switch ((s32)state) {
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (change == 0) {
            return;
        }
        MI_CpuCopy16(DWCi_GetChildInfo(), data_ov016_021de590->child, 0x16);
        if (data_ov016_021de590->callback != NULL) {
            data_ov016_021de590->callback(0);
        } else {
            data_ov016_021de590->unk_2a0 = 1;
        }
        break;
    case 13:
        if (change == 0) {
            return;
        }
        if (data_ov016_021de590->callback != NULL) {
            data_ov016_021de590->callback(1);
        } else {
            data_ov016_021de590->unk_2a0 = 1;
        }
        break;
    case 20:
    case 23:
        if (change == 0) {
            return;
        }
        if (data_ov016_021de590->callback != NULL) {
            data_ov016_021de590->callback(3);
        } else {
            data_ov016_021de590->unk_2a0 = 1;
        }
        break;
    case 26:
    case 29:
        if (change == 0) {
            return;
        }
        if (data_ov016_021de590->callback != NULL) {
            data_ov016_021de590->callback(4);
        } else {
            data_ov016_021de590->unk_2a0 = 1;
        }
        break;
    case 12:
        if (change == 0) {
            return;
        }
        if (data_ov016_021de590->callback != NULL) {
            data_ov016_021de590->callback(2);
        } else {
            data_ov016_021de590->unk_2a0 = 1;
        }
        break;
    case 34:
        DWCi_TSKlDelete(0, task);
        DWCi_HEAPlFree_((u32 *)&data_ov016_021de590);
        break;
    }
}

