#include "global.h"

#include "overlay_74_main_menu_util.h"

void ov74_02236070(void);

void ov74_02236074(void);

void ov74_022360A0(void *arg0);

void ov74_02236070(void) {
}

void ov74_02236074(void) {
    OS_DisableIrq();
    OS_SetIrqFunction((1UL << 13), ov74_02236070);
    OS_EnableIrqMask(1UL << 13);
    OS_EnableIrq();
}

void ov74_022360A0(void *arg0) {
    ov74_0223D454.saveData = arg0;
    ov74_0223D454.savingStatus = 0;
}
