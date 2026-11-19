#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "follow_mon.h"
#include "screen_fade.h"
#include "unk_02097B78.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"







void ov106_021E5C30(LegendaryCinematic *cinematic);

int LugiaCinematic_Init(LegendaryCinematic *cinematic);

int LugiaCinematic_Exit(LegendaryCinematic *cinematic);

int LugiaCinematic_WaitFade(LegendaryCinematic *cinematic);

int LugiaCinematic_Play(LegendaryCinematic *cinematic);

enum LugiaCinematicState {
    LUGIA_CINEMATIC_INIT,
    LUGIA_CINEMATIC_EXIT,
    LUGIA_CINEMATIC_WAIT_FADE,
    LUGIA_CINEMATIC_PLAY,
    LUGIA_CINEMATIC_DONE,
};

extern int (*const sScriptCinematicSubs_Lugia[LUGIA_CINEMATIC_DONE])(LegendaryCinematic *);

BOOL ScriptCinematic_Lugia(LegendaryCinematic *cinematic);

BOOL ScriptCinematic_Lugia(LegendaryCinematic *cinematic) {
    cinematic->state = sScriptCinematicSubs_Lugia[cinematic->state](cinematic);
    if (cinematic->state == LUGIA_CINEMATIC_DONE) {
        return FALSE;
    }
    ov106_021E5C30(cinematic);
    return TRUE;
}
