#include "global.h"

#include "overlay_96_ov96_digit_work.h"

void ov96_022196E4(Ov96DigitWork *work, s32 arg1);
void ov96_02219A5C(Ov96DigitWork *work, s32 value);
void ov96_02219B30(Ov96DigitWork *work);
void ov96_02219C30(Ov96DigitWork *work);

void ov96_022195C8(Ov96DigitWork *work);

void ov96_022195C8(Ov96DigitWork *work) {
    ov96_02219C30(work);
    ov96_022196E4(work, 0x3C);
    ov96_02219A5C(work, 0);
    ov96_02219B30(work);
}
