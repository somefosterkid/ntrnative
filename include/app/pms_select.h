// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pms_select.h: the lines in use so far.
#ifndef BWPORT_APP_PMS_SELECT_H
#define BWPORT_APP_PMS_SELECT_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of cancel and result in Black
struct PMSSelectParam {
    SaveControl *save;
    // Set when the player backs out, and result is NULL
    BOOL cancel;
    // The phrase chosen
    PMSData *result;
};

#endif // BWPORT_APP_PMS_SELECT_H
