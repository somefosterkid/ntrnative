#ifndef POKEHEARTGOLD_OVERLAY_14_BOX2_SYS_WORK_H
#define POKEHEARTGOLD_OVERLAY_14_BOX2_SYS_WORK_H

// Box2SysWork: merged by tools/hg/promote.py from 4 m2c local copies (2 variants, base
// src/overlay_14_021EB290_021EB290.c) and 18 accesses; fields at every accessed offset, pads elsewhere.

struct Box2AppWork;
struct GameProcManager;

typedef struct Box2SysWork {
    void *param;
    struct GameProcManager *procManager;
    BOOL procMgrResult;
    void *subProcWork;
    u16 subRet;
    u8 pad_12[12];
    u8 subProcType;
    u8 unk13;
    u8 tray;
    u8 trayMax;
    u8 pos;
    u8 getTray;
    u8 unk18;
    u8 trayScroll;
    u8 unk1A;
    u8 unk1B;
    u8 moveMode : 4;
    u8 unk1C_4 : 4;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u32 curRcvPos;
    int nextSeq;
    struct Box2AppWork *app;
} Box2SysWork;

// sizeof(Box2SysWork) == 0x3c

#endif // POKEHEARTGOLD_OVERLAY_14_BOX2_SYS_WORK_H
