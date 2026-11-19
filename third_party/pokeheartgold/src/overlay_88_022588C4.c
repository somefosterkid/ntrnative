#include "global.h"

#include "constants/heap.h"

#include "overlay_45_wifi_plaza_system.h"
#include "overlay_88_wifi_plaza_news.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "system.h"

enum {
    WIFI_PLAZA_NEWS_MAIN_STATE_FADE_IN = 0,
    WIFI_PLAZA_NEWS_MAIN_STATE_WAIT_FADE_IN,
    WIFI_PLAZA_NEWS_MAIN_STATE_RUN,
    WIFI_PLAZA_NEWS_MAIN_STATE_FADE_OUT,
    WIFI_PLAZA_NEWS_MAIN_STATE_WAIT_FADE_OUT,
};

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);

void ov45_0222A4A8(WiFiPlazaSystem *param0);

void ov45_0222A520(WiFiPlazaSystem *param0, BOOL param1);

BOOL ov45_0222D844(void);

void ov88_02258A70(WiFiPlazaNewsData *plazaNewsData, const WiFiPlazaSystem *plazaSys);

BOOL ov88_02258EFC(WiFiPlazaNewsCountdown *countdown, struct UnkStruct_ov66_0223177C_t *timeSource, const UnkStruct_ov66_02231428 *closeTime);

BOOL ov88_022590D8(WiFiPlazaNewsAnimState *animState, struct UnkStruct_ov66_0223177C_t *playerList);

BOOL ov88_02259404(WiFiPlazaNewsNameList *nameList, struct UnkStruct_ov66_0223177C_t *playerList, const WiFiPlazaSystem *plazaSys, enum HeapID heapID);

void ov88_02259818(WiFiPlazaNewsCaptions *newsCaptions, u32 captionIndex);

BOOL ov88_022588C4(OverlayManager *appMan, int *state);

BOOL ov88_022588C4(OverlayManager *appMan, int *state) {
    WiFiPlazaNewsData *plazaNewsData;
    WiFiPlazaApplicationArgs *args;
    BOOL done;

    plazaNewsData = OverlayManager_GetData(appMan);
    args = OverlayManager_GetArgs(appMan);

    switch (*state) {
    case WIFI_PLAZA_NEWS_MAIN_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_114);
        ov45_0222A520(args->plazaSystem, 1);
        (*state)++;
        break;
    case WIFI_PLAZA_NEWS_MAIN_STATE_WAIT_FADE_IN:
        done = IsPaletteFadeFinished();

        if (done == 1) {
            (*state)++;
        }
        break;
    case WIFI_PLAZA_NEWS_MAIN_STATE_RUN:
        if ((gSystem.newKeys & PAD_BUTTON_B) || (ov45_0222A330(args->plazaSystem) == 1) || (ov45_0222D844() == 1)) {
            if (ov45_0222A330(args->plazaSystem) == 1) {
                ov45_0222A4A8(args->plazaSystem);
            }
            (*state) = WIFI_PLAZA_NEWS_MAIN_STATE_FADE_OUT;
        } else {
            done = ov88_02258EFC(&plazaNewsData->countdown, plazaNewsData->playerList, plazaNewsData->closeTime);

            if (done == 1) {
                ov88_02259818(&plazaNewsData->captions, WIFI_PLAZA_NEWS_CAPTION_COUNTDOWN);
            }

            done = ov88_022590D8(&plazaNewsData->animState, plazaNewsData->playerList);

            if (done == 1) {
                ov88_02259818(&plazaNewsData->captions, WIFI_PLAZA_NEWS_CAPTION_PLAYER_COUNT);
            }

            done = ov88_02259404(&plazaNewsData->nameList, plazaNewsData->playerList, args->plazaSystem, HEAP_ID_114);

            if (done == 1) {
                ov88_02259818(&plazaNewsData->captions, WIFI_PLAZA_NEWS_CAPTION_LATEST_NAME);
            }
        }
        break;
    case WIFI_PLAZA_NEWS_MAIN_STATE_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_114);
        (*state)++;
        break;
    case WIFI_PLAZA_NEWS_MAIN_STATE_WAIT_FADE_OUT:
        done = IsPaletteFadeFinished();

        if (done == 1) {
            return 1;
        }
        break;
    }

    ov88_02258A70(plazaNewsData, args->plazaSystem);

    return 0;
}
