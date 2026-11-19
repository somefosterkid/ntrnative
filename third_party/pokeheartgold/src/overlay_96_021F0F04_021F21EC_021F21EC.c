#include "global.h"

#include "overlay_96_ov96_arena_work.h"

s32 ov96_021F218C(Ov96ArenaMon *, Ov96ArenaMon *);

void ov96_021F21EC(Ov96ArenaWork *arg0);

void ov96_021F21EC(Ov96ArenaWork *arg0) {
    u8 i;
    u8 j;
    Ov96ArenaLink *row;
    Ov96ArenaMon *a;
    Ov96ArenaMon *b;

    for (i = 0; i < 12; i++) {
        row = _0221DCA0[i];
        j = 0;
        a = &arg0->mon[i / 3][i % 3];
        for (; j < 12; j++) {
            if (i == j) {
                row[j].linked = 0;
                row[j].other = NULL;
            } else if (j >= i) {
                b = &arg0->mon[j / 3][j % 3];
                if (a->state != 1 || b->state != 1) {
                    row[j].linked = 0;
                    _0221DCA0[j][i].linked = 0;
                    row[j].other = NULL;
                    _0221DCA0[j][i].other = NULL;
                } else if (ov96_021F218C(a, b)) {
                    row[j].linked = 1;
                    _0221DCA0[j][i].linked = 1;
                    row[j].other = b;
                    _0221DCA0[j][i].other = a;
                } else {
                    row[j].linked = 0;
                    _0221DCA0[j][i].linked = 0;
                    row[j].other = NULL;
                    _0221DCA0[j][i].other = NULL;
                }
            }
        }
    }
}
