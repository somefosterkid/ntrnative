#include "error_handling.h"
typedef struct Item { s32 id; u8 pad[5]; u8 rank; u8 pad2[2]; s32 key; s32 pad3[2]; } Item;
typedef struct Node { struct Node *prev; struct Node *next; Item *item; } Node;
typedef struct Owner { s32 count; s32 pad[2]; s32 mode; s32 pad3[5]; Item items[12]; Item *sorted[12]; } Owner;
void ov96_021E93B4(Owner *arg0, s32 arg1) {
    s32 mode;
    Node nodes[13];
    Node *n;
    Node *p;
    s32 i, j;
    s32 rank;
    s32 prevKey, tie, k;
    nodes[0].prev = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].item = NULL;
    switch (arg1) {
    case 0:
        mode = 1;
        break;
    case 1:
        mode = 1;
        break;
    case 2:
        mode = 2;
        break;
    default:
        GF_AssertFail();
        break;
    }
    arg0->mode = mode;
    for (i = 0; i < arg0->count; i++) {
        p = &nodes[0];
        j = i + 1;
        n = &nodes[j];
        n->item = &arg0->items[i];
        n->next = NULL;
        n->prev = NULL;
        for (j = 0; j < i; j++) {
            p = p->next;
            if (mode == 1) {
                if (p->item->key < n->item->key) {
                    p->prev->next = n;
                    n->prev = p->prev;
                    n->next = p;
                    p->prev = n;
                    break;
                } else if (p->item->key == n->item->key && p->item->id > n->item->id) {
                    p->prev->next = n;
                    n->prev = p->prev;
                    n->next = p;
                    p->prev = n;
                }
            } else {
                if (p->item->key > n->item->key) {
                    p->prev->next = n;
                    n->prev = p->prev;
                    n->next = p;
                    p->prev = n;
                    break;
                } else if (p->item->key == n->item->key && p->item->id > n->item->id) {
                    p->prev->next = n;
                    n->prev = p->prev;
                    n->next = p;
                    p->prev = n;
                }
            }
        }
        if (j == i) {
            p->next = n;
            n->prev = p;
        }
    }
    prevKey = -1;
    tie = 0;
    p = &nodes[0];
    rank = -1;
    for (k = 0; k < arg0->count; k++) {
        p = p->next;
        arg0->sorted[k] = p->item;
        if (prevKey != p->item->key) {
            prevKey = p->item->key;
            if (tie) {
                rank = k;
                tie = 0;
            } else {
                rank++;
            }
        } else {
            tie = 1;
        }
        arg0->sorted[k]->rank = rank;
    }
}
