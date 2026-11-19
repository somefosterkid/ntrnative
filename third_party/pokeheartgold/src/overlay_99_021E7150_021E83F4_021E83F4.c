#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_98.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_98_ov98text_sys.h"
#include "overlay_99_pokeathlon_event_record.h"

struct BgConfig;

struct ManagedSprite;

struct NARC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PokeathlonEventRecord;

struct PokeathlonEventRecordData;

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

typedef struct PokeathlonEventRecordData PokeathlonEventRecordData;

extern const char _021EA5E4[];

void ov99_021E8788(PokeathlonEventRecord *work);

void ov99_021E8828(PokeathlonEventRecord *work);

void ov99_021E8C50(PokeathlonEventRecord *work);

BOOL ov99_021E90F4(PokeathlonEventRecord *work);

BOOL PokeathlonEventRecord_Main(OverlayManager *man, int *state);

BOOL PokeathlonEventRecord_Main(OverlayManager *man, int *state) {
    PokeathlonEventRecord *work = OverlayManager_GetData(man);

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
        if (work->modeSwitchRequested) {
            work->linkMode ^= 1;
            ov99_021E8828(work);
        }
        if (work->eventChanged) {
            ov99_021E8788(work);
            ov99_021E8C50(work);
            work->eventChanged = FALSE;
        }
        if (ov98_0221EF24() || ov99_021E90F4(work)) {
            PlaySE(SEQ_SE_DP_SELECT);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            *state = RECORD_SCREEN_STATE_WAIT_FADE_OUT;
        }
        break;
    case RECORD_SCREEN_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            *state = RECORD_SCREEN_STATE_EXIT;
        }
        break;
    case RECORD_SCREEN_STATE_EXIT:
        return TRUE;
    default:
        GF_ASSERT(!_021EA5E4);
        break;
    }
    ov98_0221E6CC(work->spriteContext);
    return FALSE;
}
