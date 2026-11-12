#include "types.h"
#include "spl/spl.h"

// SPL's linked lists, which keep their count. Only the front is pushed and popped

void SPLList_PushFront(SPLList *list, SPLNode *node) {
    if (list->first == NULL) {
        list->first = node;
        list->last = node;
        node->next = NULL;
        node->prev = node->next;
    } else {
        node->next = list->first;
        node->prev = NULL;
        list->first->prev = node;
        list->first = node;
    }
    list->count++;
}

SPLNode *SPLList_PopFront(SPLList *list) {
    SPLNode *node = NULL;
    SPLNode *first = list->first;

    if (first != NULL) {
        node = first;
        list->first = first->next;
        if (list->first != NULL) {
            first->next->prev = NULL;
        } else {
            list->first = NULL;
            list->last = NULL;
        }
        list->count--;
    }
    return node;
}

SPLNode *SPLList_Erase(SPLList *list, SPLNode *node) {
    SPLNode *next = node->next;

    if (next == NULL) {
        if (list->first == node) {
            list->first = NULL;
            list->last = NULL;
        } else {
            node->prev->next = NULL;
            list->last = list->last->prev;
        }
    } else if (list->first == node) {
        list->first = next;
        list->first->prev = NULL;
    } else {
        next->prev = node->prev;
        node->prev->next = node->next;
    }
    list->count--;
    return node;
}
