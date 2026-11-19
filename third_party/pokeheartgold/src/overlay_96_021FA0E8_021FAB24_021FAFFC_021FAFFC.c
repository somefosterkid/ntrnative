#include "global.h"
#include "overlay_96_ov96_icon_work.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

typedef struct SortNode {
    struct SortNode *prev;
    struct SortNode *next;
    u16 key;
    u16 idx;
} SortNode;

void ov96_021FAFFC(UnkStruct_ov96_021F94A8 *param_1, u8 *param_2);

void ov96_021FAFFC(UnkStruct_ov96_021F94A8 *param_1, u8 *param_2) {
    SortNode nodes[13];
    int i, j;
    SortNode *n;
    SortNode *cur;
    u32 v;

    nodes[0].prev = NULL;
    nodes[0].next = &nodes[1];
    nodes[0].key = 0;
    nodes[0].idx = 0;
    for (i = 0; i < 12; i++) {
        cur = &nodes[0];
        n = &nodes[i + 1];
        n->key = (u8)((param_2[0x1c + (u8)(i / 2)] >> ((u8)(i % 2) * 4)) & 0xf);
        n->idx = i;
        n->next = NULL;
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->key > n->key) {
                cur->prev->next = n;
                n->prev = cur->prev;
                n->next = cur;
                cur->prev = n;
                break;
            }
            if (cur->key == n->key && cur->idx > n->idx) {
                cur->prev->next = n;
                n->prev = cur->prev;
                n->next = cur;
                cur->prev = n;
                break;
            }
        }
        if (j == i) {
            cur->next = n;
            n->prev = cur;
        }
    }
    ov96_021FC2B4(param_1->unk_228, 1);
    cur = &nodes[0];
    for (i = 0, j = 0x10; i < 12; i++, j += 0x20) {
        cur = cur->next;
        v = (u8)cur->key;
        if (v > 8) {
            v = 8;
        }
        ov96_021FC248(param_1->unk_228, cur->idx, j);
        ov96_021FC28C(param_1->unk_228, cur->idx, v);
        ov96_021FC2E0(param_1->unk_228, cur->idx, 1);
    }
}
