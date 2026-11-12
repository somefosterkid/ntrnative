// TwlSDK's OS library, os_ownerInfo.c: Black's main 0x02087c6c-0x02087d28. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's.

#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"

void OS_GetMacAddress(u8 *macAddress) {
    MI_CpuCopy8((void *)0x2fffcf4, macAddress, 6);
}

void OS_GetOwnerInfo(OSOwnerInfo *info) {
    OSSystemWork *sys = (OSSystemWork *)0x2fffc80;

    info->language = sys->language;
    info->favoriteColor = sys->favoriteColor;
    info->birthday.month = sys->birthday.month;
    info->birthday.day = sys->birthday.day;
    info->nickNameLength = sys->nickNameLength;
    info->commentLength = sys->commentLength;
    MIi_CpuCopy16(sys->nickName, info->nickName, sizeof(sys->nickName));
    MIi_CpuCopy16(sys->comment, info->comment, sizeof(sys->comment));
    info->nickName[OS_OWNERINFO_NICKNAME_MAX] = 0;
    info->comment[OS_OWNERINFO_COMMENT_MAX] = 0;
}

s64 OS_GetOwnerRtcOffset(void) {
    OSSystemWork *sys = (OSSystemWork *)0x2fffc80;

    return sys->rtcOffset;
}

// The colors of the owner's favorite color setting, one by setting
const u16 data_020a5ce8[16] = {
    0x4e0c, 0x0137, 0x0c1f, 0x7e3f, 0x025f, 0x039e, 0x03f5, 0x03e0,
    0x1e80, 0x4769, 0x7ae6, 0x7960, 0x4800, 0x6811, 0x741a, 0x481f,
};

const u16 *OS_GetFavoriteColorTable(void) {
    return (u16 *)&data_020a5ce8;
}
