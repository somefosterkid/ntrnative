#include "global.h"

#include "overlay_58.h"
#include "overlay_manager.h"

s32 ov58_02237B98(Ov58ApricornBoxWork *work);

u32 ov58_02237BB0(Ov58ApricornBoxWork *work);

s32 ov58_02237BD4(Ov58ApricornBoxWork *work);

s32 ov58_02237C4C(Ov58ApricornBoxWork *work);

s32 ov58_02237C88(Ov58ApricornBoxWork *work);

s32 ov58_02237CA0(Ov58ApricornBoxWork *work);

BOOL ApricornBox_Main(OverlayManager *man, int *state);

BOOL ApricornBox_Main(OverlayManager *man, int *state) {
    Ov58ApricornBoxWork *pvVar1;
    s32 iVar2;

    pvVar1 = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        if (pvVar1->args->unk0 == 3) {
            *state = 3;
        } else {
            *state = 1;
        }
        break;
    case 1:
        iVar2 = ov58_02237B98(pvVar1);
        *state = iVar2;
        break;
    case 2:
        iVar2 = ov58_02237BB0(pvVar1);
        *state = iVar2;
        break;
    case 3:
        iVar2 = ov58_02237BD4(pvVar1);
        *state = iVar2;
        break;
    case 4:
        iVar2 = ov58_02237C4C(pvVar1);
        *state = iVar2;
        break;
    case 5:
        iVar2 = ov58_02237C88(pvVar1);
        *state = iVar2;
        break;
    case 6:
        iVar2 = ov58_02237CA0(pvVar1);
        *state = iVar2;
        break;
    case 7:
        return 1;
    }
    return 0;
}
