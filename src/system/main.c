// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/main.c, ported by tools/port_file.py.
#include "types.h"
#include "gfl/g3d.h"
#include "gfl/net.h"
#include "gfl/net_system.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "save/config.h"
#include "save/high_link.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/brightness.h"
#include "system/game_beacon.h"
#include "system/gfl_use.h"
#include "system/machine_use.h"
#include "system/main.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wih_dwc.h"
#include "system/wipe.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
extern const GameProcFunctions data_020a0480;
extern OSThread g_GFLSndThread;
void func_02005610(void);
void func_0201f344(HeapID heapId);
void func_020082dc(void);
void func_02010db4(void);
void GFLAppInit(void);

// The game's main file: the main loop, the game's setup and its per-frame updates. The file name is a guess, as the
// ROM does not embed it. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except those of the
// statics

void GFLAppIRQHBlank(void);
void GFLAppIRQVBlank(void);
void GameUpdateVBlank(void);
void GameInit(void);
void GameUpdate(void);
void setMonoSnd(const Config *config);

// Not static: it is the first object of .bss, and the boot code's parameter block (0x02004fb8), which is not
// decompiled, points at the start of .bss through this symbol
BOOL g_MainMemoryPriorityBlocked;

BOOL func_020051d8(void);

void main(void) {
    SystemInit();
    GFLAppInit();
    OS_SetIrqFunction(OS_IE_H_BLANK, GFLAppIRQHBlank);
    OS_SetIrqFunction(OS_IE_V_BLANK, GFLAppIRQVBlank);
    OS_EnableIrqMask(OS_IE_H_BLANK | OS_IE_V_BLANK);
    OS_EnableIrq();
    GX_HBlankIntr(TRUE);
    GX_VBlankIntr(TRUE);
    OS_EnableInterrupts();
    GameInit();

    while (TRUE) {
        BOOL priority;

        func_020053ec();
        GFLAppUpdate();
        GameUpdate();
        func_0200560c();
        GFL_G3DSysCheckSwapBuffers();

        priority = func_020051d8();
        if (priority) {
            MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);
        }
        if (!OS_IsThreadTerminated(&g_GFLSndThread)) {
            OS_SleepThreadDirect(&g_GFLSndThread, NULL);
        }
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        GameUpdateVBlank();
        if (!OS_IsThreadTerminated(&g_GFLSndThread)) {
            OS_WakeupThreadDirect(&g_GFLSndThread);
        }
        if (priority) {
            MI_SetMainMemoryPriority(MI_PROCESSOR_ARM7);
        }
    }
}

BOOL func_020051d8(void) {
    if (func_020392b8() > 1) {
        return FALSE;
    }
    if (func_0203d26c() != FALSE) {
        return FALSE;
    }
    if (func_0203d29c() == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void GFLAppIRQHBlank(void) {
    func_02005610();
}

void GFLAppIRQVBlank(void) {
    GFL_VBlankExecCallback();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    MI_WaitDma(3);
}

void GameUpdateVBlank(void) {
    SystemUpdateVBlank();
    GFL_VBlankUpdate();
}

void GameInit(void) {
    GFL_OvlLoad(OVERLAY_ID(10));
    GFL_OvlLoad(OVERLAY_ID(11));
    GFL_TextRndInit(0);
    func_0203cb34(1, 0, 1, 12);
    setupDownloadPlayConfig(12);
    downloadPlayConfigForWifi();
    GCTX_ProcMgrQueueProc(OVERLAY_NONE, &data_020a0480, NULL);
    func_02006f20(7);
    func_02008414(&getTrainerDataBlkAddress(SaveControl_GetInstance())->config);
    saveMainSaveAreaAddress(SaveControl_GetInstance());
    GFL_NetHeapInit();
    GFL_NetErrInit(8);
    GFL_DebugSetAssertFailCallback(AssertFailErrorDisp);
    func_0201f344(0);
    PML_PersonalInit(0);
    GFL_SndInit();
    PokeVoice_Init(1, getResIDFunc, getWaveInfoFunc);
    setMonoSnd(&getTrainerDataBlkAddress(SaveControl_GetInstance())->config);
    func_020082dc();
    func_020266e0(1);
    func_02010db4();
}

void GameUpdate(void) {
    func_02023fd8();
    func_02022174();
    func_0200f734();
    GFL_SndUpdate();
    PokeVoice_Update();
    func_02026728();
    PassPower_Update();
}

// Sets the sound's output from the options
void setMonoSnd(const Config *config) {
    if (getSoundFlags(config) == 0) {
        func_02005c28(TRUE);
    } else {
        func_02005c28(FALSE);
    }
}
