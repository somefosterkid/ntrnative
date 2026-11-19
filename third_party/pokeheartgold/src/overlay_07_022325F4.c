#include "global.h"

s32 ov07_0223251C(s32 arg0);

extern s32 ov07_0223738C;

s32 ov07_022325F4(s32 arg0);

s32 ov07_0223251C(s32);
extern s32 ov07_0223746C;
s32 ov07_0223261C(s32 arg0);
extern s32 ov07_02237474;
s32 ov07_02232630(s32 arg0);
extern s32 ov07_02237470;
s32 ov07_02232644(s32 arg0);

s32 ov07_022325F4(s32 arg0) {
    return *(s32 *)(&ov07_0223738C + ov07_0223251C(arg0) * 2);
}

extern s32 ov07_02237390;

s32 ov07_02232608(s32 arg0);

s32 ov07_02232608(s32 arg0) {
    return *(s32 *)(&ov07_02237390 + ov07_0223251C(arg0) * 2);
}

s32 ov07_0223261C(s32 arg0) {
    return *(s32 *)(&ov07_0223746C + ov07_0223251C(arg0) * 3);
}

s32 ov07_02232630(s32 arg0) {
    return *(s32 *)(&ov07_02237474 + ov07_0223251C(arg0) * 3);
}

s32 ov07_02232644(s32 arg0) {
    return *(s32 *)(&ov07_02237470 + ov07_0223251C(arg0) * 3);
}
