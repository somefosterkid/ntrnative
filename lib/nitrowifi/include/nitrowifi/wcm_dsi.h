// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/include/nitrowifi/wcm_dsi.h, with Black's names.
#ifndef BW_NITROWIFI_WCM_DSI_H
#define BW_NITROWIFI_WCM_DSI_H

#include "types.h"

// WCM's DSi backend (wcm_dsi.c), in the LTD autoload's .ltdmain: the steps that wcm_system.c takes through TwlSDK's NWM
// in mode 2. They share WCM's work, globals and notifications. The ROM names none of them, so the file has a
// descriptive name and the functions keep their default names.
s32 func_0273199c_unk(void);
s32 func_02731a84_unk(void);
s32 func_02731af0_unk(const u8 *bssid, const u8 *essid, u32 option);
s32 func_02731bf8_unk(void);
s32 func_02731cfc_unk(void);
s32 func_02731d6c_unk(void);
// dst is a six-byte address. The caller supplies a narrowed byte length; completion may be NULL for a blocking send.
s32 func_02731e50_unk(const u8 *dst, const void *buffer, u16 byteLength, void (*completion)(s32 result));
s32 func_02731f00_unk(void);

#endif // BW_NITROWIFI_WCM_DSI_H
