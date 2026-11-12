// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/ac/dwc_ac_cleanup.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// DWC's auto-connect, its teardown (TWL-DWC 5.3, ac). The file name is a guess: the three functions form one state
// machine on a state byte, and may belong to the file before or after, with no data to tell. dwc_ac.c's destroy and
// dwc_ac_result.c call it.

#include "dwc/ac.h"

static int func_0205b264(void);
static BOOL func_0205b2f8(void);
void DWC_Netcheck_Abort(void);
void DWC_Netcheck_Destroy(void);

// Steps the teardown whose state is *state. Up to 10 is the wireless manager's, brought down to idle by
// func_0205b264; from 0xb to 0x11 the sockets are brought down first, by func_0205b2f8, and 0xe lets go
// of CPS before that. TRUE when it is over (the state is then 0), or when it failed (the state is then 0x12)
// pokebw2's DWCAc_StepTeardown
BOOL func_0205b1e4(u8 *state) {
    if (*state <= 10) {
        int result = func_0205b264();

        if (result == 1) {
            *state = 0;
            return TRUE;
        }
        if (result == -1) {
            *state = 0x12;
            return TRUE;
        }
    } else if (*state == 14) {
        DWC_Netcheck_Abort();
        DWC_Netcheck_Destroy();
        *state = 12;
    } else if (*state < 18) {
        if (func_0205b2f8() == TRUE) {
            *state = 10;
        }
    }
    return FALSE;
}

// Asks the wireless manager for the next step down from the state it is in; 1 once it is down, -1 if it cannot be
// pokebw2's DWCAc_StepWcmTeardown
static int func_0205b264(void) {
    switch (func_ov009_021656a0()) {
    case 0:
        return 1;
    case 1:
        func_ov009_02164ee0();
        break;
    case 3:
        func_ov009_02165088();
        break;
    case 6:
        func_ov009_02165204();
        break;
    case 9:
        func_ov009_02165478();
        break;
    case 12:
        func_ov009_02165560();
        break;
    case 11:
        DWCi_AC_SetError(0);
        return -1;
    }
    return 0;
}

// Brings the sockets down: TRUE once SOCL has closed them and SOC is cleaned up (or was never started)
// pokebw2's DWCAc_StepSocTeardown
static BOOL func_0205b2f8(void) {
    int result;

    if (data_ov009_021a1cd8.config == NULL) {
        return TRUE;
    }
    if (SOCL_CalmDown() != 0) {
        return FALSE;
    }
    result = func_ov009_0215fa90();
    return result == 0 || result == SOCL_ENETRESET;
}
