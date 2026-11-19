#include "global.h"

#include "overlay_manager.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_70_gtsapplication_state.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"





struct SpriteList;

typedef u16 charcode_t;

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};

typedef void *(*TradePhaseNewFunc)(TradeSequenceData *);

typedef BOOL (*TradePhaseOverlayFunc)(void *, int *);

typedef void (*TradePhaseFreeFunc)(void *);

void ov71_022473F0(void);

extern const struct {
    TradePhaseNewFunc New;
    TradePhaseOverlayFunc Run;
    TradePhaseFreeFunc ov42_0222940C;
    int tradeTypeFlags;
} ov71_0224BBEC[7];

int TradeSequence_Main(OverlayManager *appMan, int *state);

int TradeSequence_Main(OverlayManager *appMan, int *state) {
    TradeSequenceData *sequenceData = OverlayManager_GetData(appMan);

    if (sequenceData->currentPhase < NELEMS(ov71_0224BBEC)) {
        if (ov71_0224BBEC[sequenceData->currentPhase].tradeTypeFlags & sequenceData->animationTemplate->tradeType) {
            if (sequenceData->currentPhaseData == NULL) {
                sequenceData->currentPhaseData = ov71_0224BBEC[sequenceData->currentPhase].New(sequenceData);
            }

            if (ov71_0224BBEC[sequenceData->currentPhase].Run(sequenceData->currentPhaseData, state)) {
                ov71_0224BBEC[sequenceData->currentPhase].ov42_0222940C(sequenceData->currentPhaseData);
                sequenceData->currentPhaseData = NULL;

                sequenceData->currentPhase++;
                (*state) = 0;
            }
        } else {
            sequenceData->currentPhase++;
            (*state) = 0;
        }

        ov71_022473F0();

        return 0;
    } else {
        return 1;
    }
}
