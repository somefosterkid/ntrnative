#include "global.h"
#include "overlay_108_safari_decoration_app.h"

#include "overlay_108_021E8850.h"
#include "system.h"
#include "unk_020210A0.h"

void ov108_021E9D30(SafariDecorationApp *);

u32 ov108_021E929C(SafariDecorationApp *param_1) {
    switch (param_1->initState) {
    case 0:
        ov108_021E9850(param_1);
        ov108_021E9A60(param_1);
        ov108_021E9C14(param_1);
        ov108_021E9C3C(param_1);
        ov108_021E9D30(param_1);
        ov108_021E9E10(param_1);
        ov108_021E9EB8(param_1);
        break;
    case 1:
        ov108_021EA50C(param_1);
        Main_SetVBlankIntrCB(ov108_021E979C, param_1);
        EnableTouchPad();
        InitializeTouchPad(4);
        param_1->initState = 0;
        return 1;
    }
    param_1->initState = param_1->initState + 1;
    return 0;
}
