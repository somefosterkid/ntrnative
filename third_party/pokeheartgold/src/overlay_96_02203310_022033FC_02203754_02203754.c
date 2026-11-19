#include "global.h"

#include "overlay_96_ov96_02203468work.h"

void ov96_02203754(Ov96_02203468Work *work);

void ov96_02203754(Ov96_02203468Work *work) {
    u8 hit[12];
    VecFx32 diff;
    int i, j;
    for (i = 0; i < 12; i++) {
        hit[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        switch (work->balls[i].state) {
        case 1:
        case 2:
            for (j = 0; j < 12; j++) {
                if (i != j) {
                    switch (work->balls[j].state) {
                    case 1:
                    case 2:
                        VEC_Subtract(&work->balls[i].pos, &work->balls[j].pos, &diff);
                        if (VEC_Mag(&diff) < FX32_CONST(4)) {
                            hit[i] = 1;
                            hit[j] = 1;
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    for (i = 0; i < 12; i++) {
        if (hit[i]) {
            work->balls[i].state = 3;
            work->balls[i].unk_46 = 0;
            work->balls[i].unk_44 = 0;
        }
    }
}
