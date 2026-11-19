#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "unk_02034354.h"

void ov74_02229CE0(MysteryGiftAppData *param0, SaveData *saveData, int param2);
int ov74_0222E7EC(WonderCardsAppData *appData);
int ov74_0222E85C(int *orderNums);
int ov74_0222E898(void);
int ov74_0222E7EC(WonderCardsAppData *appData) {
    WonderCard *wonderCard = appData->wonderCards[appData->selectedWondercardSlot];

    memcpy(&appData->mysteryGiftAppData.eventData.wonderCard, wonderCard, sizeof(WonderCard));
    memcpy(&appData->mysteryGiftAppData.eventData.header, &wonderCard->unk104, sizeof(UnkWonderCardSubstruct_104));

    appData->mysteryGiftAppData.eventData.wonderCard.shareMax = 0;
    appData->mysteryGiftAppData.eventData.header.unk8E_4 = 0;
    appData->mysteryGiftAppData.eventData.header.unk8E_5 = 1;

    ov74_02229CE0(&appData->mysteryGiftAppData, appData->saveData, 15);

    return 0L;
}
int ov74_0222E85C(int *orderNums) {
    int *minPtr = orderNums;
    int min = *orderNums;
    int minPos = 0;

    if (orderNums[1] < min) {
        min = orderNums[1], minPos = 1, minPtr = &orderNums[1];
    }

    if (orderNums[2] < min) {
        min = orderNums[2], minPos = 2, minPtr = &orderNums[2];
    }

    if (orderNums[3] < min) {
        min = orderNums[3], minPos = 3, minPtr = &orderNums[3];
    }

    *minPtr = 0x7fff * 0x7fff;
    return minPos + 1;
}
int ov74_0222E898(void) {
    int i;

    int connectedPlayers = 0;

    for (i = 1; i < 4 + 1; i++) {
        if (sub_02034818(i)) {
            connectedPlayers++;
        }
    }

    return connectedPlayers;
}
