// The frame palette VRAM manager's init and reset, which Black's link left out (gfd_FramePlttVramMan.c) and the Gen 4
// games call, written from their behaviour through that file's state functions. Not in Black's ROM, so the DS build
// doesn't list this file
#include "nnsys/gfd.h"

// The palette VRAM the manager was set up with: a reset allocates from 0 up and from here down
static u32 sFrmPlttSize;

// The frame manager frees only by going back to a saved state: freeing a key does nothing
int NNS_GfdFreeFrmPlttVram(NNSGfdPlttKey key) {
    (void)key;
    return 0;
}

void NNS_GfdInitFrmPlttVramManager(u32 szByte, BOOL useAsDefault) {
    sFrmPlttSize = szByte;
    NNS_GfdResetFrmPlttVramState();
    if (useAsDefault) {
        NNS_GfdDefaultFuncAllocPlttVram = NNS_GfdAllocFrmPlttVram;
        NNS_GfdDefaultFuncFreePlttVram = NNS_GfdFreeFrmPlttVram;
    }
}

void NNS_GfdResetFrmPlttVramState(void) {
    NNSGfdFrmPlttVramState state;
    state.address[0] = 0;
    state.address[1] = sFrmPlttSize;
    NNS_GfdSetFrmPlttVramState(&state);
}
