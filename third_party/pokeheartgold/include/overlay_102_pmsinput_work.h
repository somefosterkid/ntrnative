#ifndef POKEHEARTGOLD_OVERLAY_102_PMSINPUT_WORK_H
#define POKEHEARTGOLD_OVERLAY_102_PMSINPUT_WORK_H

// PMSInputWork: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_102_021E77F8_021E8294_021E848C_021E85E8.c) and 39 accesses; fields at every accessed offset, pads elsewhere.

struct ButtonMan;
struct PMSIParam;
struct PMSISearch;
struct PMSInputData;
struct PMSInputView;
struct TCBManager;

typedef struct PMSInputWork {
    struct PMSIParam *param;
    u32 inputMode;
    struct { u16 type; u16 id; u16 words[2]; } sentence;
    u16 words[2];
    struct PMSInputView *vwk;
    struct PMSInputData *dwk;
    struct PMSISearch *search;
    int mainSeq;
    void *mainProc;
    void *nextProc;
    int subSeq;
    void *subProc;
    u16 keyTrg;
    u16 keyCont;
    u16 keyRepeat;
    u8 pad_3A[0x2];
    u32 modeButton;
    u32 unk40;
    struct ButtonMan *bmn;
    u16 buttonPos;
    u16 editPos;
    u16 categoryPos;
    u16 categoryPosPrev;
    u16 categoryPosSaved;
    u8 pad_52[0x2];
    struct { u16 top; u16 scrollMax; u16 wordMax; u8 x; u8 y; int scrollVector; u16 onBack; u16 selected; } wordWin;
    struct { s16 type; s8 id; s8 count; } sentenceWk;
    u8 sentenceEditPosMax;
    u8 pad_69[2];
    u8 categoryMode;
    u8 scrollBarHeld;
    struct { u8 pos; u8 max; } menu;
    u8 pad_6F[0x1];
    int keyMode;
    void *ktChangeFunc;
    void *tcbMem;
    struct TCBManager *tcbMgr;
} PMSInputWork;

// sizeof(PMSInputWork) == 0x80

#endif // POKEHEARTGOLD_OVERLAY_102_PMSINPUT_WORK_H
