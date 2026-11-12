// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_list.h: the lines in use so far.
#ifndef BWPORT_APP_MANUAL_LIST_H
#define BWPORT_APP_MANUAL_LIST_H

#include "types.h"
#include "app/manual_common.h"

typedef struct ManualList ManualList;

enum {
    MANUAL_LIST_CATEGORY,
    MANUAL_LIST_TITLE,
};

enum {
    MANUAL_LIST_RESULT_RETURN,
    MANUAL_LIST_RESULT_DECIDE,
};

enum {
    MANUAL_LIST_MARK_NONE,
    MANUAL_LIST_MARK_NEW,
};

typedef struct {
    u16 id;
    u16 msg;
    u32 mark;
} ManualListItem;

typedef struct {
    u32 type;
    u16 count;
    ManualListItem *items;
    u16 scroll;
    u16 cursor;
    u32 result;
} ManualListParam;

// pokebw2's ManualList_Create
ManualList *func_ov225_021ba65c(ManualListParam *param, ManualCommon *common);
// pokebw2's ManualList_Free
void func_ov225_021ba740(ManualList *list);
// pokebw2's ManualList_Main
// Returns whether the list has ended
BOOL func_ov225_021ba760(ManualList *list);

#endif // BWPORT_APP_MANUAL_LIST_H
