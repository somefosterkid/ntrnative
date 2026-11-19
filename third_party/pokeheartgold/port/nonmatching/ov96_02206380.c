#include "pokeathlon/pokeathlon.h"
void ov96_021EAF94(void *, s32, s32);
void ov96_02208374(void *, s32);
void *ov96_021E60C0(PokeathlonCourseData *, s32, s32);
s32 ov96_021EAB38(s32, s32);
s32 ov96_021EAC08(s32, s32);
s32 ov96_021EAC0C(s32, s32);
s32 ov96_021EAC5C(s32, s32);
s32 ov96_021EB52C(s32, s32, s32);
s32 ov96_021EB564(s32, s32);
s32 ov96_021EB588(s32, VecFx32 *);
s32 ov96_02206DEC(void *, u8 *, u8 *, u8, s32, u8 *, s32, s32, s32);
s32 ov96_022076E4(void *);
s32 ov96_02207718(u8 *, s32);
s32 ov96_02207C38(s32, s32, s32, s32);
s32 ov96_02208658(void *, u8, s32, s32, s32);
s32 ov96_02208840(void *, s32, u8);
s32 ov96_02208864(void *, s32);
s32 sub_0200606C(s32, s32);
s32 sub_02006190(s32);

void ov96_02206380(PokeathlonCourseData *arg0, s32 arg1, u8 *arg2, u8 *arg3) {
    s32 baseX;
    s32 baseY;
    u8 *p;
    u8 *data;
    s32 sprite;
    s32 anim;
    s32 state;
    s32 hidden;
    s32 y0;
    u8 *pw;
    u8 *ph;
    u8 *pd;
    s32 sx;
    s32 me;
    s32 slot;
    s32 x12;
    s32 y12;
    s32 sy;
    s32 first;
    s32 x;
    s32 i;
    u8 *q;
    s32 flag;
    s32 j;
    u8 *d;
    s32 y;
    u32 w;
    u8 lv;
    u16 cnt;
    u8 k;

    me = (u8)ov96_021E5F24(arg0);
    data = ov96_021E8A20(arg3 + 0xF0);
    p = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    w = *(u32 *)(data + arg1 * 4);
    baseX = w & 0x1FF;
    baseY = (w >> 9) & 0x1FF;
    w = *(u32 *)(data + 0x14);
    if ((u8)((w >> 18) & 1) != 0) {
        x = w & 0x1FF;
        y = (w >> 9) & 0x1FF;
        *(s32 *)(p + p[0x518] * 4 + 0x524) = x;
        *(s32 *)(p + p[0x518] * 4 + 0x534) = y;
        ov96_021EB52C(*(s32 *)(p + p[0x518] * 4 + 0x34C), 1, 1);
        ov96_021EB564(*(s32 *)(p + p[0x518] * 4 + 0x34C), 7);
        p[0x518] = (p[0x518] + 1) % 4;
        ov96_02207C38(x + 0x80 - baseX, y + 0x60 - baseY, 0x8A3, 1);
    }
    if ((u8)(1 & ((u8)((*(u32 *)(data + 0x14) >> 19) & 0xF) >> me)) != 0) {
        sub_0200606C(0x8CE, 3);
    }
    w = *(u32 *)(data + 0x10);
    if ((u8)((w >> 18) & 1) != 0) {
        x = w & 0x1FF;
        y = (w >> 9) & 0x1FF;
        *(s32 *)(p + p[0x51A] * 4 + 0x544) = x;
        *(s32 *)(p + p[0x51A] * 4 + 0x554) = y;
        ov96_021EB52C(*(s32 *)(p + p[0x51A] * 4 + 0x35C), 1, 1);
        ov96_021EB564(*(s32 *)(p + p[0x51A] * 4 + 0x35C), 4);
        p[0x51A] = (p[0x51A] + 1) % 4;
        ov96_02207C38(x + 0x80 - baseX, y + 0x60 - baseY, 0x8CD, 1);
    }
    q = p;
    for (j = 0; j < 4; j++, q += 4) {
        VecFx32 v = { 0, 0, 0 };
        y = (*(s32 *)(q + 0x534) + 0x60 - baseY) << 12;
        x = (*(s32 *)(q + 0x524) + 0x80 - baseX) << 12;
        v.x = x;
        v.y = y;
        ov96_021EB588(*(s32 *)(q + 0x34C), &v);
    }
    q = p;
    for (j = 0; j < 4; j++, q += 4) {
        VecFx32 v = { 0, 0, 0 };
        y = (*(s32 *)(q + 0x554) + 0x60 - baseY) << 12;
        x = (*(s32 *)(q + 0x544) + 0x80 - baseX) << 12;
        v.x = x;
        v.y = y;
        ov96_021EB588(*(s32 *)(q + 0x35C), &v);
    }
    ov96_022076E4(p);
    if (arg1 == 0) {
        first = 1;
    } else {
        first = 0;
    }
    pw = data;
    ph = data;
    pd = arg3 + 0x50;
    for (i = 0; i < 4; i++, pw += 4, arg2 += 0xB8, ph += 2, pd += 0x28) {
        VecFx32 v78;

        x = *(u32 *)pw & 0x1FF;
        y0 = (*(u32 *)pw >> 9) & 0x1FF;
        cnt = *(u16 *)(ph + 0x18);
        slot = (u8)((cnt >> 5) & 3);
        anim = (u8)((u8)((cnt >> 7) & 3) + 1);
        hidden = (u8)((cnt >> 9) & 1);
        state = (u8)(1 & ((u8)((*(u32 *)(data + 0x10) >> 19) & 0xF) >> i));
        sprite = *(s32 *)(arg2 + slot * 4);
        if (PokeathlonCourse_GetParticipantCount(arg0) <= i) {
            flag = 1;
            d = ov96_021E8A20(pd);
        } else {
            flag = 0;
            d = ov96_021E8A20(arg3);
        }
        if (arg1 == i) {
            ov96_02206DEC(*(void **)(p + 0x370), arg2, data, (u8)i, slot, d, first, 1, 0);
        } else {
            ov96_02206DEC(*(void **)(p + 0x370), arg2, data, (u8)i, slot, d, first, 0, flag);
        }
        sx = x - baseX + 0x80;
        sy = y0 - baseY + 0x60 + *(s16 *)(arg2 + 0x70);
        x12 = sx << 12;
        v78.x = x12;
        y12 = sy << 12;
        v78.y = y12;
        v78.z = 0;
        ov96_02208658(*(void **)(p + 0x370), (u8)i, slot, state, hidden);
        if (state == 0) {
            if ((sx >= -0x20 && sx <= 0x120) || (sy >= -0x20 && sy <= 0xE0)) {
                ov96_021EAB38(sprite, 1);
            } else {
                ov96_021EAB38(sprite, 0);
            }
        }
        q = arg2;
        for (j = 0; j < 3; j++, q += 4) {
            ov96_021EAF94(*(void **)q, sx, sy);
        }
        {
            VecFx32 v6c = { 0, 0, 0 };
            v6c.x = x12;
            v6c.y = y12;
            if (((u8 *)ov96_021E60C0(arg0, i, slot))[5] != 0) {
                v6c.y -= 0x18000;
            } else {
                v6c.y -= 0x10000;
            }
            ov96_021EB588(*(s32 *)(arg2 + 0x48), &v6c);
        }
        ov96_02207718(arg2, sy);
        if (state == 0) {
            flag = 0;
            if (hidden == 1) {
                if (hidden != arg2[0xA3]) {
                    ov96_021EB52C(*(s32 *)(arg2 + 0x48), 1, 1);
                }
                ov96_021EAC5C(sprite, 0x15);
                arg2[0x72] = 0;
                flag = 1;
                if (me == i) {
                    if (sub_02006190(3) == 0) {
                        ov96_02207C38(v78.x / 4096, v78.y / 4096, 0x890, flag);
                    }
                } else if (sub_02006190(4) == 0) {
                    ov96_02207C38(v78.x / 4096, v78.y / 4096, 0x890, 0);
                }
            } else {
                ov96_021EB52C(*(s32 *)(arg2 + 0x48), 1, flag);
            }
            if (flag == 0) {
                ov96_021EAC08(sprite, anim);
                if (arg2[0x72] != 0) {
                    ov96_021EAC5C(sprite, 0x1A);
                    arg2[0x72]--;
                } else {
                    ov96_021EAC5C(sprite, 0);
                }
            }
        } else {
            ov96_021EB52C(*(s32 *)(arg2 + 0x48), 1, 0);
            ov96_021EAC0C(sprite, anim);
        }
        arg2[0xA3] = hidden;
        lv = (u8)((*(u32 *)pw >> 28) & 0xF);
        ov96_02208840(*(void **)(p + 0x370), i, lv);
        if (arg2[0x73] < lv) {
            arg2[0x72] = 10;
            ov96_021EB564(*(s32 *)(arg2 + 0x54), 8);
            ov96_021EB52C(*(s32 *)(arg2 + 0x54), 1, 1);
            if (me == i) {
                ov96_02207C38(v78.x / 4096, v78.y / 4096, 0x5E2, 1);
            } else {
                ov96_02207C38(v78.x / 4096, v78.y / 4096, 0x5EB, 0);
            }
        } else if (state == 0 && arg2[0x73] > lv) {
            ov96_021EB564(*(s32 *)(arg2 + 0x54), 9);
            ov96_021EB52C(*(s32 *)(arg2 + 0x54), 1, 1);
        }
        arg2[0x73] = lv;
        ov96_021EB588(*(s32 *)(arg2 + 0x54), &v78);
        cnt = (*(u32 *)pw >> 18) & 0x3FF;
        if (arg1 == i) {
            ov96_02208374(*(void **)(p + 0x370), cnt);
        }
        if (*(u16 *)(arg2 + 0xAE) < cnt) {
            ov96_021EB564(*(s32 *)(arg2 + *(u16 *)(arg2 + 0x78) * 4 + 0x4C), 3);
            ov96_021EB52C(*(s32 *)(arg2 + *(u16 *)(arg2 + 0x78) * 4 + 0x4C), 1, 1);
            *(u16 *)(arg2 + 0x78) = (*(u16 *)(arg2 + 0x78) + 1) % 2;
            ov96_02208864(*(void **)(p + 0x370), i);
            if (me == i) {
                ov96_02207C38(v78.x / 4096, v78.y / 4096, 0x88F, 1);
            } else {
                ov96_02207C38(v78.x / 4096, v78.y / 4096, 0x88F, 0);
            }
        }
        for (k = 0; k < 2; k++) {
            ov96_021EB588(*(s32 *)(arg2 + k * 4 + 0x4C), &v78);
        }
        *(u16 *)(arg2 + 0xAE) = cnt;
    }
}
