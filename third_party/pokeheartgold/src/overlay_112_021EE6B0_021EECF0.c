#include "global.h"
struct Ov112PokewalkerApp;

s32 ov112_021EE920(struct Ov112PokewalkerApp *);

s32 ov112_021EEA7C(void *);

s32 ov112_021EEAF0(void *, s32);

void ov112_021EECF0(void *arg0);

void ov112_021EECF0(void *arg0) {
    ov112_021EEAF0(arg0, 1);
    ov112_021EEA7C(arg0);
    ov112_021EE920(arg0);
}
