#include "global.h"

#include "system.h"
#include "trainer_card_main.h"
#include "unk_02005D10.h"

u32 ov51_021E6A54(TrainerCardMain *cardMain);

u32 ov51_021E6A54(TrainerCardMain *cardMain) {
    if ((gSystem.newKeys & 1) != 0) {
        if (cardMain->showingBack != 0 && !cardMain->isConnected) {
            PlaySE(0x5dc);
            return 4;
        }
    } else {
        if ((gSystem.newKeys & 2) != 0) {
            PlaySE(0x940);
            return 5;
        }
    }
    if ((gSystem.newKeys & 0x30) != 0) {
        return 3;
    }
    return 0;
}
