// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/th_award.h: the lines in use so far.
#ifndef BWPORT_APP_TH_AWARD_H
#define BWPORT_APP_TH_AWARD_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/overlay.h"

struct ThAwardParam {
    u8 gender;
    TrialHouseSave *save;
    // Whether the result is the downloaded challenge's rather than the game's
    BOOL downloaded;
    // Whether a jingle plays for the rank
    BOOL playJingle;
};

// Black 2's, not checked against Black
#define OVERLAY_TH_AWARD OVERLAY_ID(313)

#endif // BWPORT_APP_TH_AWARD_H
