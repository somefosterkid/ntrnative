// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_season_banner.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_SEASON_BANNER_H
#define BWPORT_FIELD_EVENT_SEASON_BANNER_H

#include "types.h"
#include "struct_decls.h"

GameEvent *EventSeasonBanner_CreateStandalone(GameSystem *gsys, u8 startSeason, u8 endSeason);

// Black 2's, not checked against Black
typedef void (*EventSeasonBannerCallback)(void *arg);

GameEvent *EventSeasonBanner_CreateFieldTransitionEx(GameSystem *gsys, Field *field, u8 startSeason, u8 endSeason,
                                                     EventSeasonBannerCallback callback, void *callbackArg);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *EventSeasonBanner_CreateFieldTransition(GameSystem *gsys, Field *field, u8 startSeason, u8 endSeason);

#endif // BWPORT_FIELD_EVENT_SEASON_BANNER_H
