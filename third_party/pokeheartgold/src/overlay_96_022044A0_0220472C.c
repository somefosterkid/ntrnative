#include "global.h"

#include "error_handling.h"

typedef struct Ov96SortKey {
    u8 unk_00[6];
    u16 key;
} Ov96SortKey;

typedef struct Ov96SortNode {
    struct Ov96SortNode *prev;
    struct Ov96SortNode *next;
    Ov96SortKey *data;
} Ov96SortNode;

s32 ov96_0220472C(s32 arg0, Ov96SortKey **arg1, Ov96SortKey **arg2);

s32 ov96_0220472C(s32 arg0, Ov96SortKey **arg1, Ov96SortKey **arg2) {
    s32 sp0;
    Ov96SortKey **sp4;
    u8 sp8;
    Ov96SortNode nodes[10];
    u8 n;
    u8 i;
    u8 j;
    Ov96SortNode *node;
    Ov96SortNode *cur;
    Ov96SortNode *p;
    u16 key;

    sp0 = arg0;
    sp4 = arg1;
    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].data = NULL;
    sp8 = 0;
    n = sp8;
    i = n;
    do {
        if ((s32)i / 3 != sp0) {
            node = &nodes[n + 1];
            node->data = sp4[i];
            node->next = NULL;
            node->prev = NULL;
            cur = &nodes[0];
            for (j = 0; j < n; j += 1) {
                cur = cur->next;
                if (cur->data->key < node->data->key) {
                    cur->prev->next = node;
                    node->prev = cur->prev;
                    node->next = cur;
                    cur->prev = node;
                    break;
                }
            }
            if (j == n) {
                cur->next = node;
                node->prev = cur;
            }
            n += 1;
        }
        i += 1;
    } while (i < 12);
    j = 0;
    key = nodes[0].next->data->key;
    p = &nodes[0];
    for (; j < 9; j += 1) {
        p = p->next;
        arg2[j] = p->data;
        if (key == arg2[j]->key) {
            sp8 = (u8)(sp8 + 1);
        }
    }
    if (sp8 == 0) {
        GF_AssertFail();
    }
    return sp8;
}
