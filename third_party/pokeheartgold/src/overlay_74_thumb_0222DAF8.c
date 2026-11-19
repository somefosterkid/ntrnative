#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"

int ov74_0222DAF8(WonderCardsAppData *appData, int current, int direction);
int ov74_0222DAF8(WonderCardsAppData *appData, int current, int direction) {
    int initial = current;

    while (1) {
        current += direction;

        if (current == 3) {
            current = 0;
        }

        if (current == -1) {
            current = 3 - 1;
        }

        if (initial == current) {
            break;
        }

        if (SaveMysteryGift_CardTagIsValid(appData->mysteryGift, current)) {
            break;
        }
    }

    return current;
}
