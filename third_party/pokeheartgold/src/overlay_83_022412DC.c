#include "global.h"

#include "bg_window.h"
#include "pokemon_types_def.h"

struct Pokemon;
u32 GetMonData(Pokemon *mon, int param, void *dest);
void ClearFrameAndWindow2(Window *window, u8 skipTransfer);

BOOL ov83_022412DC(Pokemon *mon);
void ov83_02241354(Window *window);

BOOL ov83_022412DC(Pokemon *mon) {
    u8 isMissingPP = 0;

    if (GetMonData(mon, MON_DATA_MOVE1_PP, 0L) != GetMonData(mon, MON_DATA_MOVE1_MAX_PP, 0L)) {
        isMissingPP = 1;
    }

    if (GetMonData(mon, MON_DATA_MOVE2_PP, 0L) != GetMonData(mon, MON_DATA_MOVE2_MAX_PP, 0L)) {
        isMissingPP = 1;
    }

    if (GetMonData(mon, MON_DATA_MOVE3_PP, 0L) != GetMonData(mon, MON_DATA_MOVE3_MAX_PP, 0L)) {
        isMissingPP = 1;
    }

    if (GetMonData(mon, MON_DATA_MOVE4_PP, 0L) != GetMonData(mon, MON_DATA_MOVE4_MAX_PP, 0L)) {
        isMissingPP = 1;
    }

    return isMissingPP;
}

void ov83_02241354(Window *window) {
    ClearFrameAndWindow2(window, 1);
    ClearWindowTilemapAndScheduleTransfer(window);
}
