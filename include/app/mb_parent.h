// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mb_parent.h: the lines in use so far.
#ifndef BWPORT_APP_MB_PARENT_H
#define BWPORT_APP_MB_PARENT_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/mb.h"

struct MBParentParam {
    // TRUE from the start menu, FALSE from the Poké Transfer Lab
    u8 startMenu;
    // Set by the Poké Transfer Lab
    GameData *gameData;
};

void MB_CommSetParentStateCallback(MBCommPStateCallback callback);
const MBUserInfo *MB_CommGetChildUser(u16 childAid);

#endif // BWPORT_APP_MB_PARENT_H
