#include "global.h"

#include "error_handling.h"
#include "overlay_96_arena_cpu.h"

typedef struct SortNode {
    struct SortNode *prev;
    struct SortNode *next;
    Ov96ArenaCpuCell *data;
} SortNode;

u8 ov96_021F3B38(u32 n, Ov96ArenaCpuCell **src, Ov96ArenaCpuCell **out) {
    u8 cnt;
    SortNode nodes[26];
    u8 i;
    u8 j;
    SortNode *cur;
    SortNode *p;
    SortNode *q;
    s32 key;

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    cnt = 0;
    nodes[0].data = NULL;
    for (i = 0; i < n; i++) {
        cur = &nodes[i + 1];
        cur->data = src[i];
        cur->next = NULL;
        cur->prev = NULL;
        p = &nodes[0];
        for (j = 0; j < i; j++) {
            p = p->next;
            if (p->data->score < cur->data->score) {
                p->prev->next = cur;
                cur->prev = p->prev;
                cur->next = p;
                p->prev = cur;
                break;
            }
        }
        if (j == i) {
            p->next = cur;
            cur->prev = p;
        }
    }
    key = nodes[0].next->data->score;
    q = &nodes[0];
    for (i = 0; i < n; i++) {
        q = q->next;
        out[i] = q->data;
        if (key == out[i]->score) {
            cnt++;
        }
    }
    if (cnt == 0) {
        GF_AssertFail();
    }
    return cnt;
}
