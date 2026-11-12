// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/config_panel.h: the lines in use so far.
#ifndef BWPORT_APP_CONFIG_PANEL_H
#define BWPORT_APP_CONFIG_PANEL_H

#include "types.h"
#include "struct_decls.h"

// What the screen is started with. result is set when the screen was left with the touch bar's close icon, which
// closes the menu as well
typedef struct {
    BOOL result;
    GameSystem *gsys;
} ConfigPanelParam;

// The options screen's proc, from the field menu
extern const GameProcFunctions data_ov068_021bb5c8;

#endif // BWPORT_APP_CONFIG_PANEL_H
