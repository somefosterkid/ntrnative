#include "global.h"

#include "overlay_96_app_02210C98.h"

void ov96_02214ABC(Ov96App_02210C98_CpuPlayer *arg0);

void ov96_02214ABC(Ov96App_02210C98_CpuPlayer *arg0) {
    u8 b[3];
    u8 a[3];
    u8 i;
    u8 max;
    int second;
    int third;
    Ov96App_02210C98_Unit *p;

    for (i = 0; i < 3; i++) {
        p = arg0->units[i];
        b[i] = p->unk_73 + p->unk_70;
        a[i] = p->unk_73 + p->unk_75;
    }
    max = 0;
    for (i = 1; i < 3; i++) {
        if (b[i - 1] < b[i]) {
            max = i;
        }
    }
    if (max == 0) {
        if (a[1] >= a[2]) {
            second = 1;
            third = 2;
        } else {
            second = 2;
            third = 1;
        }
    } else if (max == 1) {
        if (a[0] >= a[2]) {
            second = 0;
            third = 2;
        } else {
            second = 2;
            third = 0;
        }
    } else if (a[0] >= a[1]) {
        second = 0;
        third = 1;
    } else {
        second = 1;
        third = 0;
    }
    arg0->slots[0].unit = arg0->units[max];
    arg0->slots[1].unit = arg0->units[second];
    arg0->slots[2].unit = arg0->units[third];
}
