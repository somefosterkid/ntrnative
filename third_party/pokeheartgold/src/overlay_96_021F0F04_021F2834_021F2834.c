#include "global.h"

#include "overlay_96_ov96_arena_work.h"

s32 _s32_div_f(s32, u32);

void ov96_021EB06C(void *param_1, u32 param_2, s32 param_3, u32 *param_4, s32 *param_5);

void ov96_021F2834(Ov96ArenaWork *p);

void ov96_021F2834(Ov96ArenaWork *p) {
    u8 tbl[4][12];
    u8 val[4];
    u8 cnt[4];
    u32 outA;
    s32 outB;
    u8 sel[4];
    s32 i;
    s32 j;
    s32 k;
    s32 r;

    if (p->unk_72B != 0) {
        for (i = 0; i < 4; i++) {
            cnt[i] = 0;
        }
        p->unk_72B = 0;
        p->unk_74E = 0;
        p->unk_74F = 0;
        for (i = 0; i < 12; i++) {
            s32 q = i / 3;
            s32 m = i % 3;
            Ov96ArenaMon *ent = &p->mon[q][m];
            if (ent->state == 1) {
                ov96_021EB06C(ent->spr, ent->pos.x / 0x1000, ent->pos.y / 0x1000, &outA, &outB);
                r = ov96_021F3180(p->unk_730, outA, outB, sel);
                if (r != 0) {
                    ent->unk_42 += r;
                    tbl[sel[0]][cnt[sel[0]]++] = i;
                    val[sel[0]] = r;
                }
            }
        }
        for (k = 0; k < 4; k++) {
            for (j = 0; j < cnt[k]; j++) {
                p->unk_750[p->unk_74E] = tbl[k][j];
                p->unk_75C[p->unk_74E] = val[k];
                p->unk_74E += 1;
            }
        }
    }
}
