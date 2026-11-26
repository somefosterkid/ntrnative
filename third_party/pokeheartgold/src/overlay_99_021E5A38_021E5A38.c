#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_98.h"
#include "overlay_99.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_98_ov98text_sys.h"

struct BgConfig;

struct ManagedSprite;

struct NARC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PokeathlonCourseRecord;

struct PokeathlonCourseRecordArgs;

struct PokeathlonCourseRecordSelection;

struct PokeathlonSpriteContext;

struct Ov98TextSys;

enum PokeathlonRecordScreenState {
    RECORD_SCREEN_STATE_FADE_IN,
    RECORD_SCREEN_STATE_WAIT_FADE_IN,
    RECORD_SCREEN_STATE_INPUT,
    RECORD_SCREEN_STATE_WAIT_FADE_OUT,
    RECORD_SCREEN_STATE_EXIT,
};

typedef struct PokeathlonCourseRecordSelection {
    s8 cursor;
    BOOL scrollForward;
    int page;
    BOOL scrolling;
    BOOL pageChanged;
} PokeathlonCourseRecordSelection;

typedef struct PokeathlonCourseRecord {
    PokeathlonCourseRecordArgs *args;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    Ov98TextSys *textContext;
    PokeathlonSpriteContext *spriteContext;
    ManagedSprite *sprites[26];
    PokeathlonCourseRecordSelection selection;
} PokeathlonCourseRecord;

BOOL ov99_021E6638(PokeathlonCourseRecord *work);

void ov99_021E5F74(void *arg0);

void ov99_021E5FE8(u32 *param_1, s32 param_2);

void ov99_021E6050(s32 *arg0, s32 arg1);

void ov99_021E6530(PokeathlonCourseRecord *param_1, u32 param_2);

void ov99_021E5D2C(void *param_1);

void ov98_0221F0EC(void);

BOOL PokeathlonCourseRecord_Main(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work = OverlayManager_GetData(man);

    ov99_021E5F74(work);
    if (work->selection.pageChanged == TRUE) {
        ov99_021E5FE8((u32 *)(work), work->selection.page);
        ov99_021E6050((s32 *)(work), work->selection.page);
        ov99_021E6530(work, work->selection.page);
        work->selection.pageChanged = FALSE;
        work->selection.scrolling = TRUE;
    }
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
        if (ov99_021E6638(work) && !work->selection.scrolling) {
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
        ov98_0221EB84(work->textContext, 17);
        ov99_021E5D2C((u8 *)(work));
        ov98_0221E684(work->spriteContext, work->sprites, 26);
        ov98_0221F0EC();
        return TRUE;
    default:
        GF_ASSERT(!"seq error!");
        break;
    }
    ov98_0221E6CC(work->spriteContext);
    return FALSE;
}
