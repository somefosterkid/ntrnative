#ifndef DWC_UTIL_WIFIIDTOOL_H_
#define DWC_UTIL_WIFIIDTOOL_H_

#include "dwc/dwc.h"

void func_02058fb4(DWCAuthWiFiId *param_1);
// A new Wi-Fi info (a not attested ID from the MAC address and a random history); the Wi-Fi ID's making (into the backup through the work memory), its update after an account creation and its
// remaking from a new random history
BOOL DWCi_AUTH_GetNewWiFiInfo(DWCWiFiInfo *wifiinfo);
BOOL DWCi_AUTH_MakeWiFiID(void *work);
BOOL DWCi_AUTH_UpDateWiFiID(DWCWiFiInfo *wifiinfo, void *work);
BOOL DWCi_AUTH_RemakeWiFiID(DWCWiFiInfo *wifiinfo);

#endif
