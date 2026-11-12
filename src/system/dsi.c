// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/dsi.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/dsi.h"
#include "types.h"
#include "nitro/os.h"


// The DSi's settings, read through TwlSDK, with what a DS answers in their place. The file's name is a guess. Names
// from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except func_0202eb0c and func_0202eb44

// pokebw2's isRunningOnDSi
BOOL func_0202eb0c(void) {
    return OS_IsRunOnTwl();
}

BOOL canPlayerExchangePhotos(void) {
    if (func_0202eb0c()) {
        if (OS_GetParentalControlInfoPtr()->flags.isSetParentalControl &&
            OS_GetParentalControlInfoPtr()->flags.photoExchange) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// pokebw2's isWirelessEnabled
BOOL func_0202eb44(void) {
    return OS_IsAvailableWireless();
}

BOOL hasLicenseBeenAccepted(void) {
    if (func_0202eb0c() == TRUE) {
        return OS_IsAgreeEULA();
    }
    return TRUE;
}

void getBirthdayMonthDay(u8 *month, u8 *day) {
    OSOwnerInfo info;

    OS_GetOwnerInfo(&info);
    *month = info.birthday.month;
    *day = info.birthday.day;
}
