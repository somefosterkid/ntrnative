#include "global.h"
#include "overlay_59_apricorn_app.h"

void ov59_02238FF4(Ov59ApricornApp *, u32);

s32 ov59_022390A8(Ov59ApricornApp *, u32);

s32 ov59_02239F38(Ov59ApricornApp *arg0);

u32 ov59_022382F4(Ov59ApricornApp *param_1);

u32 ov59_022382F4(Ov59ApricornApp *param_1) {
    u16 sVar1;
    s32 iVar2;
    iVar2 = param_1->state;
    sVar1 = iVar2;
    switch (sVar1) {
    case 0:
        ov59_02238FF4(param_1, 2);
        param_1->state = (param_1->state) + 1;
        break;

    case 1:
        iVar2 = ov59_022390A8(param_1, 1);
        if (iVar2 != 0) {
            param_1->state += 1;
        }
        break;

    case 2:
        iVar2 = ov59_02239F38(param_1);
        switch (iVar2) {
        case 0:
            param_1->state = 0;
            return 9;
        case 1:
            param_1->state = 0;
            return 10;
        }
        break;
    }

    return 8;
}
