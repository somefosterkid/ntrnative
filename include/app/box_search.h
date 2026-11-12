// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box_search.h: the lines in use so far.
#ifndef BWPORT_APP_BOX_SEARCH_H
#define BWPORT_APP_BOX_SEARCH_H

#include "types.h"
#include "app/box2.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
struct BoxSearchParam {
    u32 unk0;
    Box2SysWork *syswk;
    Box2Param *param;
};

#endif // BWPORT_APP_BOX_SEARCH_H
