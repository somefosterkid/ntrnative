// (Black / White) The battle sweep's field half (tools/battle_sweep_bw.py): BWPORT_BW_TESTBATTLE=SPECIES:LEVEL starts a scripted
// wild battle (the CallWildBattleEx script command's event, field_encount.c, then the field's re-entry as the script's
// CallWildBattleEnd does) against that species once the field has been free for 90 frames, and says `test: wild battle` and, when
// the field is free again, `test: battle over` (+ `sweep: end battle` with BWPORT_BW_TESTLIST, exit 0 with BWPORT_BW_TESTEXIT).
// The battle's own half is port/include/port_bw_sweep_battle.h. Included once by the game_event.c stand-in (Black / White).
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_event.h"
#include "system/game_event.h"
#include "system/game_system.h"

extern void PORT_SweepEnd(const char *why, int exitNow);

// the wild battle's event (Black 2 / White 2's stand-in port/extern_b2w2/src/system/game_event.c defines PSW_WILD_EVENT first)
#ifndef PSW_WILD_EVENT
#define PSW_WILD_EVENT(enc, sp, lv) func_ov021_021aa0a0((enc), (sp), (lv), 0)
#endif

static int sBwSweepSp, sBwSweepLv, sBwSweepState, sBwSweepFree;

static GameEventReturnCode PortBwSweep_Event(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys = GameEvent_GetGameSystem(event);

    switch (*state) {
    case 0:
        (*state)++;
        GameEvent_ChainNext(event, PSW_WILD_EVENT(Field_GetEncountSystem(GSYS_GetField(gsys)), (u16)sBwSweepSp, (u8)sBwSweepLv));
        return GAMEEVENT_CONTINUE;
    case 1:
        (*state)++;
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, GSYS_GetField(gsys), 0, 0, 1, 0, 0));
        return GAMEEVENT_CONTINUE;
    }
    return GAMEEVENT_DONE;
}

static void PortBwSweep_Frame(GameSystem *gsys) {
    const char *e = getenv("BWPORT_BW_TESTBATTLE");
    int free = GSYS_GetNowEvent(gsys) == NULL;

    if (e == NULL || sBwSweepState >= 3) {
        return;
    }
    if (sBwSweepState == 0) {
        sBwSweepFree = free ? sBwSweepFree + 1 : 0;
        if (sBwSweepFree >= 90 && sscanf(e, "%d:%d", &sBwSweepSp, &sBwSweepLv) == 2) {
            sBwSweepState = 1;
            GSYS_SetNowEvent(gsys, GameEvent_Create(gsys, NULL, PortBwSweep_Event, 4));
            fprintf(stderr, "test: wild battle species %d level %d at V-blank %u\n", sBwSweepSp, sBwSweepLv, (unsigned)OS_GetVBlankCount());
        }
    } else if (sBwSweepState == 1 && !free) {
        sBwSweepState = 2;
    } else if (sBwSweepState == 2 && free) {
        sBwSweepState = 3;
        fprintf(stderr, "test: battle over at V-blank %u\n", (unsigned)OS_GetVBlankCount());
        if (getenv("BWPORT_BW_TESTLIST") != NULL) {
            PORT_SweepEnd("battle", getenv("BWPORT_BW_TESTEXIT") != NULL);
        }
        if (getenv("BWPORT_BW_TESTEXIT") != NULL) {
            fflush(NULL);
            _exit(0);
        }
    }
}
