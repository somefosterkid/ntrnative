#include "global.h"

#include "comm_player_manager.h"
#include "unk_02035900.h"

void sub_02057358(u8 arg0);

void sub_02057358(u8 arg0) {
    if (_021D41C4 != NULL) {
        if (_021D41C4->localMoveEnabled != arg0) {
            _021D41C4->localMoveEnabled = arg0;
        }
        sub_020376E0(0x3E, &_021D41C4->localMoveEnabled);
    }
}

void sub_02057384(u8 arg0);

void sub_02057384(u8 arg0) {
    if ((_021D41C4 != NULL) && (_021D41C4->localMoveEnabled != arg0)) {
        _021D41C4->localMoveEnabled = arg0;
        sub_020376E0(0x3E, &_021D41C4->localMoveEnabled);
    }
}
