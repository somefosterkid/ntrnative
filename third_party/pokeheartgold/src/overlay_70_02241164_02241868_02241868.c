#include "global.h"

#include "error_handling.h"
#include "overlay_70_gtsapplication_state.h"

void *ov70_02241868(void *arg0, s32 arg1);

void *ov70_02241868(void *arg0, s32 arg1) {
    if (arg1 == 9) {
        return &((GTSApplicationState *)arg0)->searchResults[((GTSApplicationState *)arg0)->selectedSearchResult].pokemon;
    } else if (arg1 == 10) {
        return &((GTSApplicationState *)arg0)->receivedListing.pokemon;
    } else if (arg1 == 8) {
        return &((GTSApplicationState *)arg0)->receivedListing.pokemon;
    } else {
        GF_AssertFail();
        return NULL;
    }
}
