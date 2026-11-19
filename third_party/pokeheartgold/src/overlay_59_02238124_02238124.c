#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "unk_02031B0C.h"

struct SaveApricornBox;


void ov59_02238FF4(Ov59ApricornApp *param0, u32 param1);

void ov59_02238AB0(Ov59ApricornApp *param0, u32 param1);

void ov59_02238C40(Ov59ApricornApp *param0, u32 param1);

int ov59_0223A05C(Ov59ApricornApp *param0);

void ov59_02238F58(Ov59ApricornApp *param0);

int ov59_02238FB4(Ov59ApricornApp *param0);

int ov59_02238124(Ov59ApricornApp *param0);

int ov59_02238124(Ov59ApricornApp *param0) {
    int v0;
    u8 v1;
    switch (param0->state) {
    case 0:
        ov59_02238FF4(param0, 2);
        ov59_02238AB0(param0, 1);
        ov59_02238C40(param0, 11);
        param0->state++;
        break;

    case 1:
        v0 = ov59_0223A05C(param0);
        if (v0 != 6) {
            param0->unk_48 = v0;
            param0->unk_51 = 4;
            param0->state++;
        }
        break;

    case 2:
        v1 = param0->unk_51;
        param0->unk_51 -= 1;
        if (v1 == 0) {
            if (param0->unk_48 == 1) {
                ov59_02238FF4(param0, 0);
                ov59_02238AB0(param0, 0);
                ov59_02238C40(param0, 10);
                param0->state = 0;
                return 5;
            }
            ov59_02238C40(param0, 12);
            ov59_02238AB0(param0, 0);
            ov59_02238F58(param0);
            param0->state++;
        }
        break;

    case 3:
        v0 = ov59_02238FB4(param0);
        if (v0 < 0) {
            break;
        }
        param0->state = 0;
        if (v0 != 0) {
            ApricornBox_SetKurtApricorn(param0->apricornBox, param0->selected, param0->quantity);
            return 1;
        }
        ov59_02238FF4(param0, 0);
        ov59_02238C40(param0, 10);

    default:
        param0->state = 0;
        return 5;
    }

    return 6;
}
