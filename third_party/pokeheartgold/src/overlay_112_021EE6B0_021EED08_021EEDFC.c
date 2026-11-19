#include "global.h"

typedef int (*StateFunc)(void *);

extern StateFunc ov112_021FF8F8[];

typedef struct Ctl {
    int a;
    int b;
    int state;
} Ctl;

u32 ov112_021EEDFC(Ctl *ctl);

u32 ov112_021EEDFC(Ctl *ctl) {
    ctl->state = ov112_021FF8F8[ctl->state](ctl);
    if (ctl->state == 0xb) {
        ctl->state = 0;
        return 3;
    }
    return 2;
}
