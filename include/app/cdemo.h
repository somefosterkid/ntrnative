// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/cdemo.h: the lines in use so far.
#ifndef BWPORT_APP_CDEMO_H
#define BWPORT_APP_CDEMO_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/proc.h"

// The demo's heap, pokebw2's HEAPID_CDEMO
#define HEAPID_CDEMO 0x7f

struct CDemoParam {
    // Which demo plays
    u16 demo;
    // Whether A, B or Start skips the demo
    u8 canSkip;
    // Set when the demo was skipped
    u8 skipped;
};

struct CDemoPalette {
    u8 data[0x2000];
    u32 size;
};

struct CDemoWork {
    CDemoParam *param;
    ArcTool *arc;
    CDemoPalette palettes[3];
    TCB *vblankTask;
    const u32 *scriptStart;
    const u32 *scriptCursor;
    u32 waitCounter;
    u32 unk6024[4];
    u32 unk6034[2];
    u16 nextFrame;
    u8 currentBuffer;
    u8 frameState;
    u64 lastTick;
    u64 elapsedFrameTime;
    u32 state;
    u32 resumeState;
    BOOL initialized;
};

// pokebw2's CDEMO_PROC_FUNCTIONS
extern const GameProcFunctions data_ov179_021b68d8;

// pokebw2's CDemo_Main
BOOL func_ov179_021b6380(CDemoWork *work);
// pokebw2's CDemo_DisableGraphics
void func_ov179_021b6220(void);
// pokebw2's CDemo_InitScript
void func_ov179_021b622c(CDemoWork *work);
// pokebw2's CDemo_StartVBlank
void func_ov179_021b624c(CDemoWork *work);
// pokebw2's CDemo_StopVBlank
void func_ov179_021b6268(CDemoWork *work);
// pokebw2's CDemo_FadeInBlack
void func_ov179_021b6290(CDemoWork *work, int division, int sync, u32 resumeState);
// pokebw2's CDemo_FadeOutBlack
void func_ov179_021b62b8(CDemoWork *work, int division, int sync, u32 resumeState);
// pokebw2's CDemo_FadeInWhite
void func_ov179_021b62e0(CDemoWork *work, int division, int sync, u32 resumeState);
// pokebw2's CDemo_FadeOutWhite
void func_ov179_021b630c(CDemoWork *work, int division, int sync, u32 resumeState);
// pokebw2's CDemo_LoadPalette
void func_ov179_021b6338(CDemoWork *work, u32 index, u32 fileId);

// pokebw2's CDemo_RunCommand
u32 func_ov179_021b66dc(CDemoWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CDemo_ConfigureVRAM
void func_ov179_021b615c(void);
// pokebw2's CDemo_InitGraphics
void func_ov179_021b616c(CDemoWork *work);
// pokebw2's CDemo_FreeScript
void func_ov179_021b6248(CDemoWork *work);

#endif // BWPORT_APP_CDEMO_H
