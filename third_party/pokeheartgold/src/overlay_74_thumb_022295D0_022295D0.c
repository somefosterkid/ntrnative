#include "global.h"

#include "constants/heap.h"

#include "options.h"
#include "overlay_manager.h"
#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "sys_task.h"
#include "pokemon_types_def.h"
#include "mystery_gift.h"
#include "pokedex.h"
#include "screen_fade_internal.h"
#include "overlay_74_ranger_link_manager.h"
#include "player_data.h"
#include "gf_gfx_planes.h"
#include "font.h"
#include "message_format.h"
#include "overlay_74_main_menu_window.h"
#include "render_window.h"

BOOL ov74_02230FD4(void);

void ov74_02231008(void);

enum RangerLinkRangerCommState {
    RANGER_COMM_STATE_INIT = 0,
    RANGER_COMM_STATE_WAIT_READY,
    RANGER_COMM_STATE_2,
    RANGER_COMM_STATE_3,
    RANGER_COMM_STATE_4,
    RANGER_COMM_STATE_UNUSED_5,
    RANGER_COMM_STATE_UNUSED_6,
    RANGER_COMM_STATE_7,
    RANGER_COMM_STATE_8,
    RANGER_COMM_STATE_9,
    RANGER_COMM_STATE_WAIT_RANGER_RESPONSE,
    RANGER_COMM_STATE_11,
    RANGER_COMM_STATE_12,
    RANGER_COMM_STATE_13,
    RANGER_COMM_STATE_14,
    RANGER_COMM_STATE_15,
    RANGER_COMM_STATE_16,
    RANGER_COMM_STATE_17,
    RANGER_COMM_STATE_18,
    RANGER_COMM_STATE_TRANSFER_COMPLETED,
    RANGER_COMM_STATE_WAIT_TRANSFER_COMPLETED,
    RANGER_COMM_STATE_UNUSED_21,
    RANGER_COMM_STATE_GOTO_CONNECTION_LOST,
    RANGER_COMM_STATE_CONNECTION_LOST,
    RANGER_COMM_STATE_UNUSED_24,
    RANGER_COMM_STATE_UNUSED_25,
    RANGER_COMM_STATE_CONNECTION_CANCELED,
    RANGER_COMM_STATE_DONE,
    RANGER_COMM_STATE_SHUTDOWN_WIRELESS_AND_REBOOT,
    RANGER_COMM_STATE_29,
    RANGER_COMM_STATE_30
};

BOOL ov74_022295D0(RangerLinkManager *rangerLinkMan, enum RangerLinkRangerCommState *state, BOOL param2, u32 cancelKey);

BOOL ov74_022295D0(RangerLinkManager *rangerLinkMan, enum RangerLinkRangerCommState *state, BOOL param2, u32 cancelKey) {
    if (--rangerLinkMan->timeout == 0) {
        ov74_02231008();
        *state = RANGER_COMM_STATE_CONNECTION_LOST;
        return 0;
    }

    if (param2) {
        if (!ov74_02230FD4()) {
            ov74_02231008();
            *state = RANGER_COMM_STATE_GOTO_CONNECTION_LOST;
            return 0;
        }
    }

    if (cancelKey && (gSystem.newKeys & (cancelKey))) {
        ov74_02231008();
        *state = RANGER_COMM_STATE_CONNECTION_CANCELED;
        return 1;
    }

    return 0;
}
