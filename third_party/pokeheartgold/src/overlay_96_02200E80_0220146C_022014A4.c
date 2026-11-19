#include "global.h"

typedef struct Ov96SortItem {
    u8 filler_00[0x10];
    s16 score;
} Ov96SortItem;

typedef struct Ov96SortNode {
    struct Ov96SortNode *prev;
    struct Ov96SortNode *next;
    Ov96SortItem *item;
} Ov96SortNode;

u8 ov96_022014A4(Ov96SortItem **items, Ov96SortItem **out);

u8 ov96_022014A4(Ov96SortItem **items, Ov96SortItem **out) {
    u8 count;
    Ov96SortNode nodes[17];
    u8 i;
    u8 j;
    Ov96SortNode *node;
    Ov96SortNode *cur;

    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    count = 0;
    nodes[0].item = NULL;
    for (i = count; i < 16; i++) {
        node = &nodes[i + 1];
        node->item = items[i];
        node->next = NULL;
        node->prev = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->item->score < node->item->score) {
                cur->prev->next = node;
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;
                break;
            }
        }
        if (j == i) {
            cur->next = node;
            node->prev = cur;
        }
    }
    j = 0;
    {
        Ov96SortNode *p;
        s16 t;

        t = nodes[0].next->item->score;
        p = &nodes[0];
        for (; j < 16; j++) {
            p = p->next;
            out[j] = p->item;
            if (t == out[j]->score) {
                count++;
            }
        }
    }
    GF_ASSERT(count != 0);
    return count;
}
