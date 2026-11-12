// The PC build's DSi-only features: not available. The PC runs Black as an original DS (OS_IsRunOnTwl is FALSE), so
// the game skips its DSi paths; these are what the link needs once port/exclude.txt leaves the DSi files out
// (port/exclude.txt's dsi lines), each answering as a DS does:
//   - src/system/dsi.c (parental control, the DSi's wireless switch and EULA): a DS has none of them;
//   - TwlSDK's SNDEX (the DSi's sound extension: src/system/snd_sys.c and snd_mic.c call it under hw_isDSi): libntr
//     has SNDEXi_* but nothing on its ARM7 side answers (SNDEXi_Init would wait forever), so it is never started;
//   - the C-Gear Xtransceiver (overlay 172: the DSi camera, SSP JPEG, DSP shutter sound and G711, TWL microphone):
//     its process (comm_tvt_sys.c's data_ov172_021c7240, which src/ov010/event_comm_tvt.c and src/ov006/
//     event_wificlub.c start) ends at once, back to the field. port/exclude.txt should leave all of src/ov172 out:
//     its other files call into the DSi ones.
// Plain definitions (mingw-w64's linker mishandles weak ones): the files they stand in for have to be left out.
#include "types.h"
#include "gfl/proc.h"
#include "system/dsi.h"

// ---- src/system/dsi.c ----

// pokebw2's isRunningOnDSi
BOOL func_0202eb0c(void) {
    return FALSE;
}

// The DSi's parental controls: a DS has none, and exchanges no photos
BOOL canPlayerExchangePhotos(void) {
    return FALSE;
}

// The DSi's wireless switch: a DS's wireless is always available (libntr's OS_IsAvailableWireless reads the TWL
// settings, which are zero on PC)
BOOL func_0202eb44(void) {
    return TRUE;
}

// The DSi's EULA: TRUE on a DS
BOOL hasLicenseBeenAccepted(void) {
    return TRUE;
}

void getBirthdayMonthDay(u8 *month, u8 *day) {
    OSOwnerInfo info;

    OS_GetOwnerInfo(&info);
    *month = info.birthday.month;
    *day = info.birthday.day;
}

// ---- TwlSDK's SNDEX (lib/twl/src/snd/sndex.c) ----

void func_02704364(void);
SNDEXResult func_027047c0(int target, const u16 *coefficients);

// SNDEX_Init
void func_02704364(void) {
}

// SNDEX_SetIirFilter: SNDEX was never started
SNDEXResult func_027047c0(int target, const u16 *coefficients) {
    (void)target, (void)coefficients;
    return SNDEX_RESULT_BEFORE_INIT;
}

// ---- the Xtransceiver's process (src/ov172/comm_tvt_sys.c) ----

// Each step of the process is done at once
static BOOL StubCommTvt_Done(GameProc *proc, u32 *state, void *param, void *work) {
    (void)proc, (void)state, (void)param, (void)work;
    return TRUE;
}

GameProcFunctions data_ov172_021c7240 = {
    StubCommTvt_Done,
    StubCommTvt_Done,
    StubCommTvt_Done,
};
