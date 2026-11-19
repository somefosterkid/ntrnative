#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_98.h"
#include "overlay_99.h"
#include "overlay_99_pokeathlon_medals.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "wild_encounter.h"
#include "overlay_98_ov98text_sys.h"

struct BgConfig;

struct GridInputHandler;

struct ManagedSprite;

struct NARC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PokeathlonMedals;

struct PokeathlonMedalsArgs;

struct PokeathlonMedalsFlags;

struct PokeathlonSpriteContext;

struct Ov98TextSys;

BOOL ov98_0221EF24(void);

enum PokeathlonRecordScreenState {
    RECORD_SCREEN_STATE_FADE_IN,
    RECORD_SCREEN_STATE_WAIT_FADE_IN,
    RECORD_SCREEN_STATE_INPUT,
    RECORD_SCREEN_STATE_WAIT_FADE_OUT,
    RECORD_SCREEN_STATE_EXIT,
};

extern const char _021EA5D8[];

void ov99_021E7794(PokeathlonMedals *work);

BOOL ov99_021E7BC8(PokeathlonMedals *work);

BOOL ov99_021E7598(PokeathlonMedals *work);

BOOL PokeathlonMedals_Main(OverlayManager *man, int *state) {
    PokeathlonMedals *work = OverlayManager_GetData(man);

    switch (*state) {
    case RECORD_SCREEN_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, work->heapId);
        *state = RECORD_SCREEN_STATE_WAIT_FADE_IN;
        break;
    case RECORD_SCREEN_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            *state = RECORD_SCREEN_STATE_INPUT;
        }
        break;
    case RECORD_SCREEN_STATE_INPUT:
        if (work->flags.refreshSpecies) {
            ov99_021E7794(work);
            work->flags.refreshSpecies = FALSE;
        }
        if (work->flags.scrolling) {
            if (!ov99_021E7598(work)) {
                break;
            }
            work->flags.scrolling = FALSE;
            break;
        }
        if (!ov99_021E7BC8(work) && !ov98_0221EF24()) {
            break;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
        *state = RECORD_SCREEN_STATE_WAIT_FADE_OUT;
        break;
    case RECORD_SCREEN_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            *state = RECORD_SCREEN_STATE_EXIT;
        }
        break;
    case RECORD_SCREEN_STATE_EXIT:
        return TRUE;
    default:
        GF_ASSERT(!_021EA5D8);
        break;
    }
    ov98_0221E6CC(work->spriteContext);
    return FALSE;
}
