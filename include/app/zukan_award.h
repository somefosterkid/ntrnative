// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_award.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_AWARD_H
#define BWPORT_APP_ZUKAN_AWARD_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/overlay.h"

// Black 2's; twinned code confirms the offsets of unk04 in Black
struct ZukanAwardParam {
    GameData *gameData;
    // Whether the diploma is shown again, without its jingle and its slide in
    u32 unk04;
};

// Black 2's, not checked against Black
#define OVERLAY_CHIHOU_ZUKAN_AWARD OVERLAY_ID(314)

// Black 2's, not checked against Black
#define OVERLAY_ZENKOKU_ZUKAN_AWARD OVERLAY_ID(315)

#endif // BWPORT_APP_ZUKAN_AWARD_H
