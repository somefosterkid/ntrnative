#include "global.h"

#include "constants/moves.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "bg_window.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "hall_of_fame.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "overlay_64_hall_of_fame_showcase.h"
#include "unk_02019BA4.h"

struct BgConfig;

struct GridInputHandler;

struct HallOfFame;

struct HallOfFameShowcase;

struct HofDisplayMon;

struct ManagedSprite;

struct MessageFormat;

struct MsgData;

struct NARC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PokedexData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct Window;

enum HallOfFameShowcaseMainState {
    HOF_SHOWCASE_MAIN_FADE_IN,
    HOF_SHOWCASE_MAIN_WAIT_FADE_IN,
    HOF_SHOWCASE_MAIN_INPUT,
    HOF_SHOWCASE_MAIN_WAIT_FADE_OUT,
};

void ov64_021E605C(HallOfFameShowcase *showcase);

void ov64_021E6288(void);

BOOL ov64_021E62C8(HallOfFameShowcase *showcase);

BOOL HallOfFameShowcase_Main(OverlayManager *man, int *state);

BOOL HallOfFameShowcase_Main(OverlayManager *man, int *state) {
    HallOfFameShowcase *showcase = OverlayManager_GetData(man);

    switch (*state) {
    case HOF_SHOWCASE_MAIN_FADE_IN:
        ov64_021E6288();
        (*state)++;
        break;
    case HOF_SHOWCASE_MAIN_WAIT_FADE_IN:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case HOF_SHOWCASE_MAIN_INPUT:
        if (!ov64_021E62C8(showcase)) {
            (*state)++;
        }
        break;
    case HOF_SHOWCASE_MAIN_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    ov64_021E605C(showcase);
    SpriteSystem_DrawSprites(showcase->spriteManager);
    return FALSE;
}
