// TwlSDK's OS library, os_ownerInfoEx.c: Black's main 0x02087d28-0x02087ea0, the DSi's owner settings. Its functions
// are twins of libntr's (MIT, https://github.com/cybervisi0n/libntr) libraries/os/src/os_ownerInfoEx.c for SDK 5,
// whose TWL headers lib/nitro/sdk doesn't have yet. On a DSi each reads the settings, or asks the LTD main memory's
// part of the library (its twl/ltdmain_begin.h blocks, in the LTD autoload's .ltdmain 0x027312d0-0x0273131c, which
// ltd_autoload_0's delinks.txt names os_ownerInfoEx@ltd.c: tools/two_modules.py); on a DS each answers what a DS has.

#include "types.h"
#include "nitro/os.h"
#include "nitro/mi.h"
#include "twl/os.h"


void OS_GetOwnerInfoEx(OSOwnerInfoEx *info) {
    OSSystemWork *sys;
    OSNvramUserInfo *nvram;
    u16 language;

    sys = (OSSystemWork *)0x02fffc80;
    info->favoriteColor = sys->favoriteColor;
    info->birthday.month = sys->birthday.month;
    info->birthday.day = sys->birthday.day;
    info->nickNameLength = sys->nickNameLength;
    info->commentLength = sys->commentLength;
    MI_CpuCopy16(sys->nickName, info->nickName, sizeof(sys->nickName));
    MI_CpuCopy16(sys->comment, info->comment, sizeof(sys->comment));
    info->nickName[OS_OWNERINFO_NICKNAME_MAX] = 0;
    info->comment[OS_OWNERINFO_COMMENT_MAX] = 0;
    if (OS_IsRunOnTwl()) {
        nvram = sys->nvramUserInfo;
        info->country = nvram->country;
        language = nvram->language;
    } else {
        info->country = 0;
        language = sys->language;
    }
    info->language = language;
}

#include "twl/ltdmain_begin.h"
// TwlSDK's OS_IsAvailableWireless_ltdmain, in the LTD autoload's .ltdmain: the DSi's wireless setting
BOOL func_027312d0_unk(void) {
    return TWL_SETTINGS_FLAGS->isAvailableWireless == 1 ? TRUE : FALSE;
}
#include "twl/ltdmain_end.h"

s32 OS_IsAvailableWireless(void) {
    BOOL BVar1;

    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        return func_027312d0_unk();
    }
    return 1;
}

BOOL OS_IsAgreeEULA(void) {
    OSTWLSettingsData *settings;

    if (OS_IsRunOnTwl()) {
        settings = *(OSTWLSettingsData **)0x02fffdfc;
        return ((settings->unk_0 >> 24) & 1) ? TRUE : FALSE;
    }
    return FALSE;
}


u32 OS_GetAgreedEULAVersion(void) {
    BOOL BVar1;
    u32 val;
    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        val = (u32)*(u8 *)(*(s32 *)0x02fffdfc + 0x10);
    } else {
        val = 0;
    }
    return val;
}

u32 OS_GetROMHeaderEULAVersion(void) {
    BOOL BVar1;
    u32 val;
    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        val = (u32)*(u8 *)0x02ffe20e;
    } else {
        val = 0;
    }
    return val;
}

OSTWLParentalControl *OS_GetParentalControlInfoPtr(void) {
    BOOL BVar1;
    OSTWLParentalControl *twlParentalControl;
    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        twlParentalControl = (OSTWLParentalControl *)(*(s32 *)0x02fffdfc + 0x94);
    } else {
        twlParentalControl = (OSTWLParentalControl *)0x0;
    }
    return twlParentalControl;
}

#include "twl/ltdmain_begin.h"
// TwlSDK's OS_IsForceDisableWireless_ltdmain, in the LTD autoload's .ltdmain: the hardware's secure setting
BOOL OSi_IsForceDisableWirelessTwl(void) {
    OSTWLHWSecureInfo *info = (OSTWLHWSecureInfo *)HW_HW_SECURE_INFO;
    return info->flags.forceDisableWireless == 1 ? TRUE : FALSE;
}
#include "twl/ltdmain_end.h"

s32 OS_IsForceDisableWireless(void) {
    BOOL BVar1;

    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        return OSi_IsForceDisableWirelessTwl();
    }
    return 0;
}
