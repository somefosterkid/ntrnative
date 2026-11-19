#include "global.h"

#include "pm_agb_save.h"

u16 ov74_02233ACC(int gbaSectorID);

u16 ov74_02233ACC(int gbaSectorID) {
    if (gbaSectorID == 0) {
        return (u16)(ov74_0223D33C.header.saveSizes.trainerInfoSize);
    }
    if (gbaSectorID == 4) {
        return (u16)(ov74_0223D33C.header.saveSizes.gameStateSize - 3968 * 3);
    }
    if (gbaSectorID == 13) {
        return 2000;
    }
    return 3968;
}
