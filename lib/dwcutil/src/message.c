// libdwcutil's message.c (Black's ov016 0x021c0f60-0x021c1074): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct MsgWork {
    u8 unk_0[0x60];
    u16 *queue;
} MsgWork;

typedef struct MSGENTRY {
    u32 mesgOffset;
} MSGENTRY;

typedef struct tagMSGPRM {
    MSGENTRY *entry;
    u8 *data;
    void *file;
} MSGPRM;

typedef struct tagBLOCKHEAD {
    u32 kind;
    u32 size;
} BLOCKHEAD;

typedef struct tagMSGINFO {
    BLOCKHEAD header;
    u16 num;
    u16 size;
    u16 groupID;
    u8 color;
    u8 rsv;
} MSGINFO;

static MsgWork *data_ov016_021de58c;

void DWCi_MSGlInit(void);
u16 *DWCi_MSGlGetEx(s32 *param_1, u32 param_2, s32 param_3, s32 param_4);

void DWCi_MSGlInit(void) {
    data_ov016_021de58c = DWCi_HEAPlAllocEx(sizeof(MsgWork), 4);
    data_ov016_021de58c->queue = (u16 *)DWCi_QUElFormEx(8, (s32)data_ov016_021de58c, sizeof(MSGPRM));
}

void DWCi_MSGlEnd(void) {
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de58c);
}

void *DWCi_MSGlRead(u8 *file) {
    MSGPRM *msg;
    MSGINFO *info;
    u32 size;

    msg = (MSGPRM *)DWCi_QUElPopBack(data_ov016_021de58c->queue);
    msg->file = DWCi_ARClReadEx((s8 *)file, &size, 4);
    info = (MSGINFO *)((u8 *)msg->file + 0x20);
    msg->entry = (MSGENTRY *)(info + 1);
    msg->data = (u8 *)info + info->header.size + 8;
    return msg;
}

void DWCi_MSGlRelease(void *msg) {
    DWCi_ARClRelease((u32)((MSGPRM *)msg)->file);
    DWCi_QUElPushBack(data_ov016_021de58c->queue, (u32)msg);
}

s32 DWCi_MSGlGet(s32 *msg, u32 id) {
    return msg[1] + *(s32 *)(*msg + (id & 0xffff) * 4);
}

u16 *DWCi_MSGlGetEx(s32 *msg, u32 id, s32 n, s32 num) {
    u16 *text = (u16 *)(msg[1] + *(s32 *)(*msg + (u16)id * 4));

    if (n >= 0) {
        text[n] = num + 0x30;
    }
    return text;
}
