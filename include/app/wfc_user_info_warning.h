// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wfc_user_info_warning.h: the lines in
// use so far.
#ifndef BWPORT_APP_WFC_USER_INFO_WARNING_H
#define BWPORT_APP_WFC_USER_INFO_WARNING_H

#include "types.h"
#include "gfl/overlay.h"

// The notice that the Wi-Fi connection's user data may have been lost (wfc_user_info_warning.c);
// overlay 17 in Black, the 0x11 that its code loads
#define OVERLAY_WFC_USER_INFO_WARNING OVERLAY_ID(17)

// pokebw2's WFCUserInfoWarning_Show
// Shows the warning on its own screen, and returns when A or B is pressed
void func_ov017_021b6100(void);

#endif // BWPORT_APP_WFC_USER_INFO_WARNING_H
