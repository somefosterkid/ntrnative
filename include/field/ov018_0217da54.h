#ifndef FIELD_OV018_0217DA54_H
#define FIELD_OV018_0217DA54_H

#include "types.h"
#include "struct_decls.h"
#include "field/intrude_comm.h"
#include "field/intrude_field.h"
#include "gfl/clact.h"
#include "nnsys/g2d.h"
#include "system/printsys.h"

// The message windows of the Entralink's events (ov018_0217f2b4.c), at 0x10 of an event's work: the word set, the
// formatted and the loaded string, three message data, the talk window, two system windows, the message BG and the
// yes/no menu
typedef struct Ov018MsgWork {
    WordSet *wordSet;
    StrBuf *strBuf;
    StrBuf *tmpBuf;
    MsgData *msgData;
    MsgData *unk_10;
    MsgData *unk_14;
    MsgBGTalkWin *talkWin;
    MsgBGSysWin *unk_1c;
    MsgBGSysWin *unk_20;
    FieldMsgBG *msgBGSys;
    // 1 while a message printed in talkWin is shown
    u8 unk_28;
    ListMenuUI *menu;
} Ov018MsgWork;

struct Ov018Sub {
    u8 netId;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u32 unk_04;
    s32 unk_08;
    IntrudeMissionEntry *entry;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
};

// The three the work keeps: the entry of a zone and the zone it was looked up for
struct Ov018Slot {
    IntrudeFieldUnkEntry *entry;
    u16 zone;
    u8 unk_06[2];
};

// The work of the Entralink's touch screen, allocated by func_ov018_0217cfe4
typedef struct Ov018Work {
    GameSystem *gsys;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    Font *unk_1c;
    TCBExManager *unk_20;
    WordSet *unk_24;
    PrintQueue *unk_28;
    MsgData *unk_2c;
    MsgData *unk_30;
    TCB *unk_34;
    StrBuf *unk_38;
    StrBuf *unk_3c;
    StrBuf *unk_40;
    PrintWindow win44;
    PrintWindow win4c;
    ClActUnit *unk_54;
    ClActRenderer *unk_58;
    ClActor *actors[21];
    BmpOamSys *unk_b0;
    GFLBitmap *unk_b4[2];
    u8 unk_bc[0xc0 - 0xbc];
    BmpOamActor *unk_c0[2];
    void *unk_c8;
    NNSG2dCharacterData *unk_cc;
    void *unk_d0;
    u32 unk_d4;
    struct Ov018Sub sub;
    struct Ov018Slot slots[3];
    u8 unk_104;
    u8 unk_105;
    u8 unk_106;
    u8 unk_107;
    s16 unk_108;
    s16 unk_10a;
    u8 unk_10c[0x10f - 0x10c];
    u8 unk_10f;
    u16 unk_110;
    u8 unk_112;
    u8 unk_113;
    u8 unk_114;
    u8 unk_115;
    u8 unk_116;
    u8 unk_117;
    s16 unk_118;
    s8 unk_11a;
    u8 unk_11b;
    u8 unk_11c;
    u8 unk_11d;
    u8 unk_11e;
    u8 unk_11f;
    u16 unk_120[4];
    u16 unk_128[4];
    u16 unk_130[4];
    u8 unk_138[0x168 - 0x138];
    u8 unk_168;
    u8 unk_169;
    u8 unk_16a;
    u8 unk_16b;
    u8 unk_16c;
    u8 unk_16d[0x170 - 0x16d];
} Ov018Work;

Ov018Work *func_ov018_0217cfe4(GameSystem *gsys);
void func_ov018_0217d160(Ov018Work *work);
void func_ov018_0217d1c0(Ov018Work *work, s32 param_2);
void func_ov018_0217d3ac(Ov018Work *work);
GameEvent *func_ov018_0217d4dc(Ov018Work *work, s32 param_2, FieldSubscreen *subscreen);
void func_ov018_0217d5ac(TCB *tcb, void *data);
void func_ov018_0217d624(Ov018Work *work);
void func_ov018_0217d6c8(Ov018Work *work);
void func_ov018_0217d720(Ov018Work *work, ArcTool *arc);
void func_ov018_0217d870(Ov018Work *work);
void func_ov018_0217d890(Ov018Work *work);
void func_ov018_0217d904(Ov018Work *work);
void func_ov018_0217d918(Ov018Work *work, ArcTool *arc);
void func_ov018_0217da18(Ov018Work *work);
void func_ov018_0217da54(Ov018Work *work, ArcTool *arc);
void func_ov018_0217da90(Ov018Work *work);
void func_ov018_0217de44(Ov018Work *work);
void func_ov018_0217de68(Ov018Work *work, PassPowerLevel *level);
void func_ov018_0217e014(Ov018Work *work, PassPowerLevel *level);
void func_ov018_0217e0f4(Ov018Work *work, IntrudeComm *comm, PassPowerLevel *level);
void func_ov018_0217e3d0(Ov018Work *work, PassPowerLevel *level, u16 zoneId);
void func_ov018_0217e4d0(Ov018Work *work, PassPowerLevel *level);
void func_ov018_0217e524(Ov018Work *work, PassPowerLevel *level);
void func_ov018_0217e638(Ov018Work *work, u32 zoneId);
void func_ov018_0217e714(Ov018Work *work, IntrudeComm *comm, u16 zoneId);
PassPowerLevel *func_ov018_0217ebd8(Ov018Work *work);
void func_ov018_0217ec7c(IntrudeComm *comm, Ov018Work *work, u16 zoneId);
void func_ov018_0217eeb4(Ov018Work *work);
void func_ov018_0217ef44(Ov018Work *work, IntrudeComm *comm);
void func_ov018_0217ef70(Ov018Work *work, u8 netId);
void func_ov018_0217efcc(Ov018Work *work, IntrudeComm *comm);
void func_ov018_0217f008(Ov018Work *work, IntrudeComm *comm);
void func_ov018_0217f0c4(Ov018Work *work);
u32 func_ov018_0217f0fc(Ov018Work *work);
void func_ov018_0217f174(Ov018Work *work);
void func_ov018_0217f194(Ov018Work *work);
u32 func_ov018_0217f1e8(Ov018Work *work);
void func_ov018_0217f240(Ov018Work *work, s32 param_2);

void func_ov018_0217f3c4(Ov018MsgWork *work, u16 msgId);
BOOL func_ov018_0217f40c(Ov018MsgWork *work);
u32 func_ov018_0217f420(void);
void func_ov018_0217f318(Ov018MsgWork *work);
u32 func_ov018_0217f5f0(IntrudeComm *comm, u32 netId);
void func_ov018_0217f4c4(Ov018MsgWork *work);
void func_ov018_0217f4d8(Ov018MsgWork *work, GameSystem *gsys);
u32 func_ov018_0217f508(Ov018MsgWork *work);
void func_ov018_0217f2b4(Ov018MsgWork *work, GameSystem *gsys);
void func_ov018_0217f574(Ov018MsgWork *work, u32 msgId, u16 x, u16 y);
void func_ov018_0217f58c(Ov018MsgWork *work);

void func_ov018_0217f528(Ov018MsgWork *work, GameSystem *gsys, u32 x, u32 y, u32 width, u32 height);

extern const ClActorSetup data_ov018_021836aa;
void func_ov018_0217f4a0(Ov018MsgWork *work, u32 msgId, u16 x, u16 y);
#endif // FIELD_OV018_0217DA54_H
