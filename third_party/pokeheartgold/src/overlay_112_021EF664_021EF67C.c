#include "global.h"

typedef int (*StateFunc)(void *);

extern StateFunc ov112_021FF924[];

typedef struct Ctl {
    int a;
    int b;
    int state;
} Ctl;

u32 ov112_021EF67C(Ctl *ctl);

u32 ov112_021EF67C(Ctl *ctl) {
    ctl->state = ov112_021FF924[ctl->state](ctl);
    if (ctl->state == 0x25) {
        ctl->state = 0;
        return 3;
    }
    return 2;
}
