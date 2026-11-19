#include "global.h"

#include "constants/mmodel.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "follow_mon.h"
#include "gf_gfx_loader.h"
#include "overlay_106.h"
#include "screen_fade.h"
#include "sound.h"
#include "sprite_system.h"
#include "unk_02097B78.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"

struct ArceusBgPalette;


struct BgConfig;






struct ManagedSprite;

struct NARC;

struct SpriteManager;

struct SpriteSystem;

int ArceusCinematic_Init(LegendaryCinematic *cinematic);

int ArceusCinematic_Exit(LegendaryCinematic *cinematic);

int ArceusCinematic_WaitFade(LegendaryCinematic *cinematic);

int ArceusCinematic_Blend(LegendaryCinematic *cinematic);

int ArceusCinematic_RunScript(LegendaryCinematic *cinematic);

enum ArceusCinematicState {
    ARCEUS_CINEMATIC_INIT,
    ARCEUS_CINEMATIC_EXIT,
    ARCEUS_CINEMATIC_WAIT_FADE,
    ARCEUS_CINEMATIC_BLEND,
    ARCEUS_CINEMATIC_RUN_SCRIPT,
    ARCEUS_CINEMATIC_DONE,
};

extern int (*const sScriptCinematicSubs_Arceus[ARCEUS_CINEMATIC_DONE])(LegendaryCinematic *);

BOOL ScriptCinematic_Arceus(LegendaryCinematic *cinematic);

BOOL ScriptCinematic_Arceus(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus;
    cinematic->state = sScriptCinematicSubs_Arceus[cinematic->state](cinematic);
    if (cinematic->state == ARCEUS_CINEMATIC_DONE) {
        return FALSE;
    }
    arceus = cinematic->arceus;
    ov106_021E6668(arceus);
    SpriteSystem_DrawSprites(arceus->spriteManager);
    return TRUE;
}
