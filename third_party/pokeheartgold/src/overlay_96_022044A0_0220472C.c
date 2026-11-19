#include "global.h"

#include "error_handling.h"

s32 ov96_0220472C(s32 arg0, void *arg1, void *arg2);

s32 ov96_0220472C(s32 arg0, void *arg1, void *arg2) {
    s32 sp0;
    void *sp4;
    u8 sp8;
    s32 nodes[10][3];
    u8 n;
    u8 i;
    u8 j;
    s32 *node;
    s32 *cur;
    s32 *p;
    u16 key;

    sp0 = arg0;
    sp4 = arg1;
    nodes[0][0] = (s32)nodes[1];
    nodes[0][1] = (s32)nodes[1];
    nodes[0][2] = 0;
    sp8 = 0;
    n = sp8;
    i = n;
    do {
        if ((s32)i / 3 != sp0) {
            node = nodes[n + 1];
            node[2] = ((s32 *)sp4)[i];
            node[1] = 0;
            node[0] = 0;
            cur = nodes[0];
            for (j = 0; j < n; j += 1) {
                cur = (s32 *)cur[1];
                if (*(u16 *)((u8 *)(s32 *)cur[2] + 6) < *(u16 *)((u8 *)(s32 *)node[2] + 6)) {
                    ((void **)cur[0])[1] = (u8 *)node;
                    node[0] = cur[0];
                    node[1] = (s32)cur;
                    cur[0] = (s32)node;
                    break;
                }
            }
            if (j == n) {
                cur[1] = (s32)node;
                node[0] = (s32)cur;
            }
            n += 1;
        }
        i += 1;
    } while (i < 12);
    j = 0;
    key = *(u16 *)((u8 *)(s32 *)((void **)nodes[0][1])[2] + 6);
    p = nodes[0];
    for (; j < 9; j += 1) {
        p = (s32 *)p[1];
        ((s32 *)arg2)[j] = p[2];
        if (key == *(u16 *)((u8 *)((void **)arg2)[j] + 6)) {
            sp8 = (u8)(sp8 + 1);
        }
    }
    if (sp8 == 0) {
        GF_AssertFail();
    }
    return sp8;
}
