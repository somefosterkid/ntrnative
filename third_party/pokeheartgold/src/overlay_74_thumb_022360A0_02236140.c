#include "global.h"

#include "save.h"
#include "system.h"

#include "overlay_74_main_menu_util.h"

void ov74_02236140(void);

void ov74_02236140(void) {
    Save_Cancel(ov74_0223D454.saveData);
    ov74_0223D454.savingStatus = 3;
    sub_0201A738(4);
}

s32 ov74_0223615C(void);

s32 ov74_0223615C(void) {
    return ov74_0223D454.savingStatus;
}
