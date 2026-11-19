#include "global.h"

#include "wireless_manager.h"

s32 sub_02032B0C(void);

s32 sub_02033080(void);

void sub_02033830(void);

void sub_02033858(void);

void sub_02033858(void) {
    s32 iVar1;

    iVar1 = _021D4124.sWirelessManager->state;
    if (iVar1 != 1) {
        if (((iVar1 != 6) && (iVar1 != 5)) && (iVar1 != 4)) {
            sub_02032844(3);
            sub_02033830();
            return;
        }
        sub_02032844(3);
        switch (_021D4124.sWirelessManager->connectionType) {
        case 1:
        case 5:
            iVar1 = sub_02033080();
            if (iVar1 == 0) {
                sub_02033830();
                return;
            }
            break;
        case 0:
        case 4:
            iVar1 = sub_02032B0C();
            if (iVar1 == 0) {
                sub_02033830();
            }
            break;
        }
    }
    return;
}
