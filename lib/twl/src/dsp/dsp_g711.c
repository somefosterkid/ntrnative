// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/dsp/dsp_g711.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "twl/dsp.h"

// TwlSDK's DSP library, the G.711 component (guessed file name, after the SDK's dsp_g711.c): a DSP process, as a COFF
// image in the LTD autoload's .rodata. The opener hands the image to the file system as a file of memory, the loader
// starts the process under the name "g711" and the unloader quits it. ARM code in the DSi-only LTD autoload. The ROM
// names none of it; the functions have the SDK's names where twl/dsp.h knows them, and keep their default names
// otherwise.

// The state of the component. Nothing in the game's code touches its first two words
// Black 2's, not checked against Black
typedef struct DSPG711State {
    u32 unk_00;
    u32 unk_04;
    BOOL loaded;
} DSPG711State;

// pokebw2's sG711State
static DSPG711State data_02735940;
// pokebw2's sG711Context
static DSPProcessContext data_0273594c;

// Opens the component's image, as a file of memory, in file
// pokebw2's DSP_OpenStaticComponentG711Core
void func_0270723c(FSFile *file) {
    if (!FS_IsAvailable()) {
        FS_Init(FS_DMA_NOT_USE);
    }
    FS_InitFile(file);
    FS_CreateFileFromMemory(file, data_02707cb4, DSP_COMPONENT_IMAGE_SIZE);
}

// Starts the process from the image in file, into the banks of WRAM B and C that slotB and slotC allow
// pokebw2's DSP_LoadG711Core
BOOL func_02707278(FSFile *file, int slotB, int slotC) {
    if (!data_02735940.loaded) {
        DSP_InitProcessContext(&data_0273594c, "g711");
        if (DSP_ExecuteProcess(&data_0273594c, file, slotB, slotC)) {
            data_02735940.loaded = TRUE;
        }
    }
    return data_02735940.loaded;
}

// pokebw2's DSP_UnloadG711Core
void func_027072e0(void) {
    if (data_02735940.loaded) {
        DSP_QuitProcess(&data_0273594c);
        data_02735940.loaded = FALSE;
    }
}
