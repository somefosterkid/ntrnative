#ifndef GUARD_OV174_021B6100_H
#define GUARD_OV174_021B6100_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/tcbl.h"
#include "gfl/clact.h"
#include "struct_decls.h"
#include "system/app_printsys_common.h"

typedef struct Ov174Work Ov174Work;
typedef struct LocalTvtFace LocalTvtFace;

// The start of the call's script file: the person of each face (0xff: none, two faces if the last two are) and its
// background; Black's are a byte each, pokebw2's zones u16s
typedef struct {
    u8 persons[4];
    u8 zones[4];
} LocalTvtScriptHeader;

// A line of the call's script (the file the work holds, after its LocalTvtScriptHeader): a state of data_ov174_021b7398, 0 to end
typedef struct {
    u8 state;
    // The face that speaks (state 1)
    u8 face;
    // The message (state 1) or the frames to wait (state 2)
    u16 param;
} Ov174ScriptCmd;

struct Ov174Init {
    u16 unk_00;
    u8 unk_02[2];
    GameData *gameData;
};

struct Ov174Work {
    HeapID heapId;      // 0x00
    u8 unk_02[2];
    TCB *unk_04;
    s32 unk_08;
    u8 unk_0c;
    u8 unk_0d;
    u8 unk_0e;
    u8 unk_0f;
    s32 unk_10;
    s32 unk_14;
    ArcTool *arc;       // 0x18
    // The faces (unk_08 of them), then at [4] the one that speaks
    LocalTvtFace *unk_1c[5];
    struct Ov174Init *unk_30;
    s32 unk_34;
    void *file;
    LocalTvtScriptHeader *header;
    Ov174ScriptCmd *unk_40;
    u32 unk_44;
    u32 unk_48;
    u32 unk_4c;
    ClActUnit *unk_50;
    ClActor *unk_54;
    TCBExManager *unk_58;
    BmpWin *unk_5c;
    Font *unk_60;
    PrintStream *unk_64;
    MsgData *unk_68;
    MsgData *unk_6c;
    StrBuf *unk_70;
    WordSet *unk_74;
    PrintQueue *unk_78;
    KeyCursor *unk_7c;
    AppPrintsysCommon unk_80;
    u8 unk_88;
    u8 unk_89;
    u16 unk_8a;
}; // 0x8c

typedef struct Ov174StateFuncs {
    void (*unk_0)(Ov174Work *work);
    u32 (*unk_4)(Ov174Work *work);
} Ov174StateFuncs;

extern const Ov174StateFuncs data_ov174_021b7398[];

void func_ov174_021b6af8(Ov174Work *work);
u32 func_ov174_021b6b5c(Ov174Work *work);
void func_ov174_021b6b8c(Ov174Work *work);
u32 func_ov174_021b6b94(Ov174Work *work);

u32 func_ov174_021b622c(Ov174Work *param_1);
void func_ov174_021b6704(Ov174Work *work, u32 idx);
void func_ov174_021b68f8(Ov174Work *work, u32 param_2);
void func_ov174_021b6100(Ov174Work *work);
void func_ov174_021b61c4(Ov174Work *work);
void func_ov174_021b6754(Ov174Work *work);
u32 func_ov174_021b68e8(Ov174Work *work);
void func_ov174_021b6a90(Ov174Work *work);
void func_ov174_021b6994(Ov174Work *work);
void func_ov174_021b6a04(Ov174Work *work, u8 msgId);

#endif // GUARD_OV174_021B6100_H
