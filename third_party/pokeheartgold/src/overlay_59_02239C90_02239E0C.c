#include "global.h"
#include "overlay_59_apricorn_app.h"

u16 ov59_02239E0C(Ov59ApricornApp *arg0, Ov59JuiceRank *arg1, s32 arg2);

u16 ov59_02239E0C(Ov59ApricornApp *arg0, Ov59JuiceRank *arg1, s32 arg2) {
    u8 m = arg1->hi - 1;

    switch (arg2) {
    case 2:
        if (arg1->lo == 6) {
            return 0x7A;
        }
        if (arg1->lo == 5) {
            return 0x79;
        }
        if (arg1->hi == 0) {
            return arg1->lo + 0x7B;
        }
        return m + arg1->lo * 3 + 0x6A;
    case 1:
        if (arg1->lo == 6) {
            return 0x7A;
        }
        return m + arg1->lo * 4 + 0x56;
    case 0:
    default:
        if (arg1->lo == 6) {
            return 0x83;
        }
        if (arg1->lo == 5) {
            return m + 0x80;
        }
        return m + arg1->lo * 6 + 0x38;
    }
}
