#include "global.h"
#include "sprite_resources_header.h"
#include "pokemon_summary_app.h"

enum SummaryPoffinFeedMsg {
    SUMMARY_MSG_COOLNESS_ENHANCED = 0,
    SUMMARY_MSG_BEAUTY_ENHANCED,
    SUMMARY_MSG_CUTENESS_ENHANCED,
    SUMMARY_MSG_SMARTNESS_ENHANCED,
    SUMMARY_MSG_TOUGHNESS_ENHANCED,

    SUMMARY_MSG_NOTHING_CHANGED = 0xFE,
    SUMMARY_MSG_MON_WONT_EAT_MORE = 0xFF,
};

void sub_0208DDA0(PokemonSummary *summaryScreen, enum SummaryPoffinFeedMsg msg);

enum PokemonContestType {
    CONTEST_TYPE_COOL = 0,
    CONTEST_TYPE_BEAUTY = 1,
    CONTEST_TYPE_CUTE = 2,
    CONTEST_TYPE_SMART = 3,
    CONTEST_TYPE_TOUGH = 4,
    CONTEST_TYPE_MAX = 5,
};

enum SummaryState {
    SUMMARY_STATE_TRANSITION_IN = 0,
    SUMMARY_STATE_WAIT_TRANSITION,
    SUMMARY_STATE_HANDLE_INPUT,
    SUMMARY_STATE_SETUP_BATTLE_MOVE_INFO,
    SUMMARY_STATE_HIDE_BATTLE_MOVE_INFO,
    SUMMARY_STATE_SETUP_CONTEST_MOVE_INFO,
    SUMMARY_STATE_HIDE_CONTEST_MOVE_INFO,
    SUMMARY_STATE_MOVE_DETAILS,
    SUMMARY_STATE_MOVE_SWAP,
    SUMMARY_STATE_SELECT_MOVE,
    SUMMARY_STATE_WAIT_HM_MSG_INPUT,
    SUMMARY_STATE_SETUP_RIBBON_INFO,
    SUMMARY_STATE_HIDE_RIBBON_INFO,
    SUMMARY_STATE_RIBBON_SELECT,
    SUMMARY_STATE_SUBSCREEN_INPUT,
    SUMMARY_STATE_SETUP_POFFIN_FEED,
    SUMMARY_STATE_PRINT_CONTEST_STAT_MSGS,
    SUMMARY_STATE_WAIT_FINISH_POFFIN_FEED,
    SUMMARY_STATE_TRANSITION_OUT,
    SUMMARY_STATE_WAIT_EXIT,
};

int sub_02089608(PokemonSummary *summaryScreen);

int sub_02089608(PokemonSummary *summaryScreen) {
    if (gSystem.newKeys & (0x0001 | 0x0002)) {
        for (u8 i = 0; i < CONTEST_TYPE_MAX; i++) {
            if (summaryScreen->unk7BE & (1 << i)) {
                sub_0208DDA0(summaryScreen, (enum SummaryPoffinFeedMsg)(i));
                summaryScreen->unk7BE ^= (1 << i);

                if (summaryScreen->unk7BE == 0) {
                    return SUMMARY_STATE_TRANSITION_OUT;
                }
                break;
            }
        }
    }

    return SUMMARY_STATE_WAIT_FINISH_POFFIN_FEED;
}
