#include "global.h"

#include "constants/sndseq.h"

#include "follow_mon.h"
#include "screen_fade.h"
#include "unk_02097B78.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"







void ov106_021E5C30(LegendaryCinematic *cinematic);

int HoOhCinematic_Init(LegendaryCinematic *cinematic);

int HoOhCinematic_Exit(LegendaryCinematic *cinematic);

int HoOhCinematic_WaitFade(LegendaryCinematic *cinematic);

int HoOhCinematic_PlayFirstScene(LegendaryCinematic *cinematic);

int HoOhCinematic_LoadSecondScene(LegendaryCinematic *cinematic);

int HoOhCinematic_PlaySecondScene(LegendaryCinematic *cinematic);

enum HoOhCinematicState {
    HO_OH_CINEMATIC_INIT,
    HO_OH_CINEMATIC_EXIT,
    HO_OH_CINEMATIC_WAIT_FADE,
    HO_OH_CINEMATIC_PLAY_FIRST_SCENE,
    HO_OH_CINEMATIC_LOAD_SECOND_SCENE,
    HO_OH_CINEMATIC_PLAY_SECOND_SCENE,
    HO_OH_CINEMATIC_DONE,
};

extern int (*const sScriptCinematicSubs_HoOh[HO_OH_CINEMATIC_DONE])(LegendaryCinematic *);

BOOL ScriptCinematic_HoOh(LegendaryCinematic *cinematic);

BOOL ScriptCinematic_HoOh(LegendaryCinematic *cinematic) {
    cinematic->state = sScriptCinematicSubs_HoOh[cinematic->state](cinematic);
    if (cinematic->state == HO_OH_CINEMATIC_DONE) {
        return FALSE;
    }
    ov106_021E5C30(cinematic);
    return TRUE;
}
