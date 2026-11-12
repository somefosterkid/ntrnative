// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov264/cdemo_script.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/cdemo.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "nitro/gx.h"

// Descriptive file name: the command table separates this interpreter from the demo controller.
typedef u32 (*CDemoCommandFunc)(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6710(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6730(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6744(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6758(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b676c(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6780(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6794(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b67dc(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b67f4(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b680c(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6830(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6854(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6878(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b687c(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6888(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6894(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b6898(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b689c(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b68a0(CDemoWork *work, const u32 *command);
static u32 func_ov179_021b68d4(CDemoWork *work, const u32 *command);
// pokebw2's CDEMO_COMMANDS
// pokebw2's CDemoCmd_WaitFrames
static const CDemoCommandFunc data_ov179_021b6a40[] = { func_ov179_021b6710,
                                                   func_ov179_021b6730,
                                                   func_ov179_021b6744,
                                                   func_ov179_021b6758,
                                                   func_ov179_021b676c,
                                                   func_ov179_021b6780,
                                                   func_ov179_021b6794,
                                                   func_ov179_021b67dc,
                                                   func_ov179_021b67f4,
                                                   func_ov179_021b680c,
                                                   func_ov179_021b6830,
                                                   func_ov179_021b6854,
                                                   func_ov179_021b6878,
                                                   func_ov179_021b687c,
                                                   func_ov179_021b6888,
                                                   func_ov179_021b6894,
                                                   func_ov179_021b6898,
                                                   func_ov179_021b689c,
                                                   func_ov179_021b68a0,
                                                   func_ov179_021b68d4 };
// pokebw2's CDEMO_COMMAND_LENGTHS
static const u32 data_ov179_021b6a90[] = {
#include "asset/cdemo_script_data_ov179_021b6a90.inc"
};
// pokebw2's CDemo_RunCommand
u32 func_ov179_021b66dc(CDemoWork *work) {
    u32 result = data_ov179_021b6a40[*work->scriptCursor](work, work->scriptCursor);
    if (result != 0) {
        work->scriptCursor += data_ov179_021b6a90[*work->scriptCursor];
    }
    return result;
}
static u32 func_ov179_021b6710(CDemoWork *work, const u32 *command) {
    if (work->waitCounter == command[1]) {
        work->waitCounter = 0;
        return 2;
    }
    work->waitCounter++;
    return 0;
}
// pokebw2's CDemoCmd_FadeInBlack
static u32 func_ov179_021b6730(CDemoWork *work, const u32 *command) {
    func_ov179_021b6290(work, command[1], command[2], 5);
    return 1;
}
// pokebw2's CDemoCmd_FadeOutBlack
static u32 func_ov179_021b6744(CDemoWork *work, const u32 *command) {
    func_ov179_021b62b8(work, command[1], command[2], 5);
    return 1;
}
// pokebw2's CDemoCmd_FadeInWhite
static u32 func_ov179_021b6758(CDemoWork *work, const u32 *command) {
    func_ov179_021b62e0(work, command[1], command[2], 5);
    return 1;
}
// pokebw2's CDemoCmd_FadeOutWhite
static u32 func_ov179_021b676c(CDemoWork *work, const u32 *command) {
    func_ov179_021b630c(work, command[1], command[2], 5);
    return 1;
}
// pokebw2's CDemoCmd_WaitFade
static u32 func_ov179_021b6780(CDemoWork *work, const u32 *command) {
    work->state = 2;
    work->resumeState = 5;
    return 2;
}
// pokebw2's CDemoCmd_LoadBackground
static u32 func_ov179_021b6794(CDemoWork *work, const u32 *command) {
    GFL_BGSysLoadArcNCGRStatic(work->arc, command[2], command[1], 0, 0, TRUE, HEAPID_CDEMO);
    if (command[2] == 0) {
        loadBGScrToVramByFileNoReserveNegAlign(work->arc, command[2], command[1], 0, 0, FALSE, HEAPID_CDEMO);
    }
    func_ov179_021b6338(work, command[1], command[2]);
    return 1;
}
// pokebw2's CDemoCmd_EnableBackground
static u32 func_ov179_021b67dc(CDemoWork *work, const u32 *command) {
    GFL_BGSysSetBGEnabled((u8)command[1], (u8)command[2]);
    return 1;
}
// pokebw2's CDemoCmd_SetBackgroundPriority
static u32 func_ov179_021b67f4(CDemoWork *work, const u32 *command) {
    GFL_BGSysSetBGPriority((u8)command[1], (u8)command[2]);
    return 1;
}
// pokebw2's CDemoCmd_SetBackgroundSettings
static u32 func_ov179_021b680c(CDemoWork *work, const u32 *command) {
    if (command[1] == 0) {
        work->unk6024[0] = command[2];
        work->unk6024[1] = command[3];
    } else {
        work->unk6024[2] = command[2];
        work->unk6024[3] = command[3];
    }
    return 1;
}
// pokebw2's CDemoCmd_SetTransitionSettings
static u32 func_ov179_021b6830(CDemoWork *work, const u32 *command) {
    work->unk6034[0] = command[1];
    work->unk6034[1] = command[2];
    work->state = 3;
    work->resumeState = 5;
    return 2;
}
// pokebw2's CDemoCmd_ResetBackgroundSettings
static u32 func_ov179_021b6854(CDemoWork *work, const u32 *command) {
    reg_G2_BLDCNT = 0;
    work->unk6024[0] = 0;
    work->unk6024[1] = 0;
    work->unk6024[2] = 0;
    work->unk6024[3] = 0;
    return 1;
}
// pokebw2's CDemoCmd_Unused12
static u32 func_ov179_021b6878(CDemoWork *work, const u32 *command) {
    return 1;
}
// pokebw2's CDemoCmd_ResetSound
static u32 func_ov179_021b687c(CDemoWork *work, const u32 *command) {
    func_02005d28();
    return 1;
}
// pokebw2's CDemoCmd_StopBGM
static u32 func_ov179_021b6888(CDemoWork *work, const u32 *command) {
    GFL_SndBGMStop(command[1]);
    return 1;
}
// pokebw2's CDemoCmd_Unused15
static u32 func_ov179_021b6894(CDemoWork *work, const u32 *command) {
    return 1;
}
// pokebw2's CDemoCmd_Unused16
static u32 func_ov179_021b6898(CDemoWork *work, const u32 *command) {
    return 1;
}
// pokebw2's CDemoCmd_Unused17
static u32 func_ov179_021b689c(CDemoWork *work, const u32 *command) {
    return 1;
}
// pokebw2's CDemoCmd_StartMovie
static u32 func_ov179_021b68a0(CDemoWork *work, const u32 *command) {
    work->nextFrame = 0;
    work->currentBuffer = 0;
    work->frameState = 0;
    work->lastTick = 0;
    work->elapsedFrameTime = 0;
    work->state = 4;
    return 2;
}
// pokebw2's CDemoCmd_Unused19
static u32 func_ov179_021b68d4(CDemoWork *work, const u32 *command) {
    return 1;
}
