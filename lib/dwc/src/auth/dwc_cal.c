// Black's code at ov009 0x02178f4c-0x0217956c. libdwcauth.a's dwc_cal.o: the code reads the console's calibration data
// (OS_GetMacAddress, OS_GetOwnerInfo, RTC) into WiFiInfo and formats it into the HTTP request fields (sdkver, userid,
// macadr, birth, devtime, ssid), as the Auth_GetCalInfo and Auth_SetCalInfoToHttp functions of the object do.

#include "types.h"
#include "dwc/dwc.h"
#include "dwc/http.h"
#include "string.h"
#include "wstring.h"
#include "nitro/mi.h"
#include "nitrowifi/wcm.h"
#include "dwc/ac.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
typedef struct NasAuthInfo {
    char userId[0xe];
    char password[7];
    char gameCode[5];
    char makerCode[3];
    char unitCode[2];
    char macAddress[0xd];
    char language[3];
    char birthday[5];
    char deviceTime[0xd];
    char bssid[0xd];
    char ssid[0x20];
    char unk_6e;
    char apInfo[0xe];
    char unk_7d;
    u16 deviceName[10];
    u8 unk_92[2];
} NasAuthInfo;


void DWCi_BM_GetWiFiInfo(DWCWiFiInfo *param_1);
BOOL DWC_Auth_GetCalInfoFromWiFiInfo(NasAuthInfo *info, DWCWiFiInfo *wifi);

#pragma thumb off
u32 DWC_Auth_GetCalInfoFromNVRAM(u8 *param_1) {
    s32 aiStack_20 [5];

    DWCi_BM_GetWiFiInfo((DWCWiFiInfo *)aiStack_20);
    return DWC_Auth_GetCalInfoFromWiFiInfo((NasAuthInfo *)param_1, (DWCWiFiInfo *)aiStack_20);
}

BOOL DWC_Auth_GetCalInfoFromWiFiInfo(NasAuthInfo *info, DWCWiFiInfo *wifi) {
    u8 mac[6];
    OSOwnerInfo owner;
    RTCDate date;
    RTCTime time;
    OSIntrMode enabled;
    const u8 *bssid;
    const u8 *ssid;
    u16 ssidLength;
    u8 apType;
    u64 userId;
    int i;

    MI_CpuFill8(info, 0, sizeof(NasAuthInfo));

    if (wifi->attestedUserId != 0) {
        userId = wifi->attestedUserId;
        OS_SNPrintf(info->userId, sizeof(info->userId), "%013llu", userId);
    } else {
        userId = wifi->notAttestedId;
        OS_SNPrintf(info->userId, sizeof(info->userId), "%013llu", userId);
    }
    OS_SNPrintf(info->password, sizeof(info->password), "%03u", wifi->pass);

    if (*(u8 *)(HW_ROM_HEADER_BUF + 0xc) == 0) {
        OS_Terminate();
    }
    MI_CpuCopy8((void *)(HW_ROM_HEADER_BUF + 0xc), info->gameCode, 4);
    if (*(u8 *)(HW_ROM_HEADER_BUF + 0x10) == 0) {
        OS_Terminate();
    }
    MI_CpuCopy8((void *)(HW_ROM_HEADER_BUF + 0x10), info->makerCode, 2);
    info->unitCode[0] = '0';

    OS_GetMacAddress(mac);
    for (i = 0; i < 6; i++) {
        OS_SPrintf(&info->macAddress[i * 2], "%02x", mac[i]);
    }

    OS_GetOwnerInfo(&owner);
    if (owner.language >= 8) {
        owner.language = 1;
    }
    OS_SNPrintf(info->language, 3, "%02x", owner.language);
    MI_CpuCopy8(owner.nickName, info->deviceName, 0x14);
    OS_SNPrintf(info->birthday, 5, "%02x%02x", owner.birthday.month, owner.birthday.day);

    if (RTC_GetDateTime(&date, &time) != RTC_RESULT_SUCCESS) {
        return FALSE;
    }
    OS_SNPrintf(info->deviceTime, 0xd, "%02d%02d%02d%02d%02d%02d", date.year, date.month, date.day, time.hour, time.minute, time.second);

    enabled = OS_DisableInterrupts();
    bssid = func_ov009_02166384();
    DC_InvalidateRange((void *)bssid, 6);
    if (bssid == NULL) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    for (i = 0; i < 6; i++) {
        OS_SPrintf(&info->bssid[i * 2], "%02x", bssid[i]);
    }

    apType = DWC_AC_GetApType();
    if (apType == 0xff || apType > 99) {
        apType = 99;
    }
    OS_SNPrintf(info->apInfo, 0xe, "%02d:0000000-00", apType);

    ssid = func_ov009_021663c8(&ssidLength);
    DC_InvalidateRange((void *)ssid, 0x20);
    if (ssid == NULL) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    MI_CpuCopy8(ssid, info->ssid, 0x20);
    DWC_AC_GetApSpotInfo((u8 *)&info->apInfo[3]);
    OS_RestoreInterrupts(enabled);
    return TRUE;
}


BOOL DWC_Auth_SetCalInfoToHttp(DWCHttp *http, const NasAuthInfo *info, int sendSsid) {
    char text[0x21];

    OS_SNPrintf(text, 0x21, "%03d%03d", 5, 3);
    if (DWC_Http_Add_PostBase64Item(http, "sdkver", text, strlen(text)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "userid", info->userId, strlen(info->userId)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "passwd", info->password, strlen(info->password)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "bssid", info->bssid, strlen(info->bssid)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "apinfo", info->apInfo, strlen(info->apInfo)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "gamecd", info->gameCode, strlen(info->gameCode)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "makercd", info->makerCode, strlen(info->makerCode)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "unitcd", info->unitCode, strlen(info->unitCode)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "macadr", info->macAddress, strlen(info->macAddress)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "lang", info->language, strlen(info->language)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "birth", info->birthday, strlen(info->birthday)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "devtime", info->deviceTime, strlen(info->deviceTime)) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(http, "devname", (const char *)info->deviceName, wcslen(info->deviceName) * 2) != 0) {
        return FALSE;
    }
    if (sendSsid == 1) {
        if (DWC_Http_Add_PostBase64Item(http, "ssid", info->ssid, strlen(info->ssid)) != 0) {
            return FALSE;
        }
    }
    OS_SNPrintf(text, 0x21, "Nitro WiFi SDK/%d.%d", 5, 3);
    if (DWC_Http_Add_HeaderItem(http, "User-Agent", text) != 0) {
        return FALSE;
    }
    if (DWC_Http_Add_HeaderItem(http, "HTTP_X_GAMECD", info->gameCode) != 0) {
        return FALSE;
    }
    return TRUE;
}

#pragma thumb on
