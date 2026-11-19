#include "global.h"
#include "billboard.h"

void sub_020236BC(Billboard *billboard);

BOOL sub_020242AC(BillboardList *list, Billboard *billboard);
void sub_020242E4(Billboard *sentinel, Billboard *billboard);
void sub_02024308(Billboard *billboard);

BOOL sub_020242AC(BillboardList *list, Billboard *billboard) {
    if (list->freeBillboardHead <= 0) {
        return 0;
    }
    sub_020236BC(billboard);
    list->freeBillboardHead--;
    list->freeBillboards[list->freeBillboardHead] = billboard;
    return 1;
}

void sub_020242E4(Billboard *sentinel, Billboard *billboard) {
    billboard->prev = sentinel->prev;
    sentinel->prev->next = billboard;
    billboard->next = sentinel;
    sentinel->prev = billboard;
}

void sub_02024308(Billboard *billboard) {
    billboard->prev->next = billboard->next;
    billboard->next->prev = billboard->prev;
}
