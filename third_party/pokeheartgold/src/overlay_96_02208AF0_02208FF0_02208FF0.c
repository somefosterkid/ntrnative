#include "global.h"

#include "error_handling.h"
#include "overlay_96_hurdle_cpu.h"

typedef struct SortNode {
    struct SortNode *prev;
    struct SortNode *next;
    Ov96HurdleCpuCell *data;
} SortNode;

u8 ov96_02208FF0(Ov96HurdleCpuCell **src, Ov96HurdleCpuCell **dst) {
    u8 count;
    SortNode n[49];
    u8 i;
    u8 j;
    SortNode *node;
    SortNode *cur;
    SortNode *cur_2;
    s16 key;

    n[0].prev = &n[1];
    n[0].next = &n[1];
    count = 0;
    n[0].data = NULL;
    for (i = count; i < 48; i++) {
        node = &n[i + 1];
        node->data = src[i];
        node->next = NULL;
        node->prev = NULL;
        cur = &n[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->data->score < node->data->score) {
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
    key = n[0].next->data->score;
    cur_2 = &n[0];
    for (; j < 48; j++) {
        cur_2 = cur_2->next;
        dst[j] = cur_2->data;
        if (key == dst[j]->score) {
            count++;
        }
    }
    GF_ASSERT(count != 0);
    return count;
}
