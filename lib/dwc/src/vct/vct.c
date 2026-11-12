// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/vct/vct.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "vct_internal.h"

#include "nitro/mi.h"
#include "nitro/os.h"
#include "gfl/std.h"
#include "dwc/vct.h"

// The library's entry points: starting and stopping it, running it a tick, and sorting what the network gives it.

// Black 2's, not checked against Black
typedef struct VCTLibrary {
    u32 tickCount;
    u32 receivedCount;
    BOOL initialized;
} VCTLibrary;

// pokebw2's sLibrary
static VCTLibrary data_ov013_021b60cc;
// pokebw2's gVCTSettings
VCTSettings data_ov013_021b60d8;

// A function that does nothing but be called with the signature of a library, which keeps the library in the link
void OSi_ReferSymbol(const char *signature);

static int func_ov013_021b3924(int aid, void *data, int size, VCTEventResult *result);



BOOL func_ov013_021b370c(VCTConfig *config) {
    OSi_ReferSymbol(VCTi_Signature);
    if (config == NULL) {
        return FALSE;
    }
    if (data_ov013_021b60cc.initialized) {
        return TRUE;
    }
    if (config->mode != 1 && config->mode != 2 && config->mode != 3) {
        return FALSE;
    }
    if (config->callback == NULL) {
        return FALSE;
    }
    if (config->buffer == NULL || config->bufferSize == 0) {
        return FALSE;
    }
    if ((int)config->buffer % 32 != 0) {
        return FALSE;
    }
    MI_CpuFill8(&data_ov013_021b60d8, 0, 0x18);
    data_ov013_021b60d8.callback = config->callback;
    data_ov013_021b60d8.userData = config->userData;
    data_ov013_021b60d8.mode = config->mode;
    data_ov013_021b60d8.unkC = 0;
    if (config->aid >= VCT_AID_MAX) {
        return FALSE;
    }
    data_ov013_021b60d8.aid = config->aid;
    if (!func_ov013_021b2ed8(config->buffer, config->bufferSize)) {
        func_ov013_021b2f98();
        return FALSE;
    }
    if (!func_ov013_021b22b4(config)) {
        func_ov013_021b2f98();
        return FALSE;
    }
    if (func_ov013_021b0508()) {
        data_ov013_021b60cc.initialized = TRUE;
        return TRUE;
    }
    func_ov013_021b23c8();
    func_ov013_021b2f98();
    return FALSE;
}

void func_ov013_021b3844(void) {
    func_ov013_021b0600();
    func_ov013_021b23c8();
    func_ov013_021b2f98();
    data_ov013_021b60d8.mode = 0;
    data_ov013_021b60cc.initialized = FALSE;
}

void func_ov013_021b386c(void) {
    if (data_ov013_021b60cc.initialized == 0) {
        return;
    }
    data_ov013_021b60cc.tickCount++;
    if ((data_ov013_021b60cc.tickCount & 0xf) == 0) {
        func_ov013_021b23e0();
    }
    if (VCT_SendPending()) {
        func_ov013_021b111c();
    }
}

BOOL func_ov013_021b38b0(u8 aid, void *data, int size) {
    VCTEventResult result;

    switch (func_ov013_021b3924(aid, data, size, &result)) {
    case 0:
        break;
    case 1:
        data_ov013_021b60d8.callback(aid, result.event, result.session, data_ov013_021b60d8.userData);
        return TRUE;
    case 2:
        return FALSE;
    }
    return TRUE;
}

static int func_ov013_021b3924(int aid, void *data, int size, VCTEventResult *result) {
    VCTVoicePacket *voice = data;
    VCTControlPacket *control = data;
    u64 now;

    if (data == NULL) {
        return 0;
    }
    if (voice->magic != VCT_PACKET_MAGIC) {
        return 2;
    }
    if (data_ov013_021b60d8.mode == 0 || !data_ov013_021b60cc.initialized) {
        return 0;
    }
    result->event = 0;
    result->session = NULL;
    now = OS_GetTick();
    if ((voice->type & 0xf0) == VCT_TYPE_VOICE) {
        data_ov013_021b60cc.receivedCount++;
        func_ov013_021b0f38(aid, voice, size, now);
        return 0;
    }
    if (control->type == VCT_TYPE_CONTROL || control->type == VCT_TYPE_BROADCAST) {
        return func_ov013_021b2488(aid, control, size, result) != FALSE ? 1 : 0;
    }
    return 0;
}




