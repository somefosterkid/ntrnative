// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_category.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual_category.h"
#include "app/manual_common.h"
#include "app/manual_data.h"
#include "app/manual_list.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"

// The manual's list of categories, named after the ROM's "manual_category.c": the categories that have a title the
// player can read

struct ManualCategory {
    ManualCategoryParam *param;
    ManualCommon *common;
    ManualListParam listParam;
    ManualList *list;
    TCB *vblankTask;
};

static void func_ov225_021bb850(TCB *tcb, void *data);

// pokebw2's ManualCategory_Create
ManualCategory *func_ov225_021bb6e0(ManualCategoryParam *param, ManualCommon *common) {
    u16 i;
    u16 count;
    ManualCategory *category = GFL_HeapAllocate(common->heapId, sizeof(ManualCategory), TRUE, "manual_category.c", 92);

    category->param = param;
    param->result = MANUAL_LIST_RESULT_RETURN;
    category->param->selected = 0;
    category->common = common;
    category->listParam.type = MANUAL_LIST_CATEGORY;
    count = func_ov225_021b9c50(common->data);
    category->listParam.count = 0;
    category->listParam.items =
        GFL_HeapAllocate(category->common->heapId, count * sizeof(ManualListItem), TRUE, "manual_category.c", 113);
    for (i = 0; i < count; i++) {
        u16 open = 0;
        u16 titleCount = func_ov225_021b9c34(category->common->data, i);
        u16 j;

        for (j = 0; j < titleCount; j++) {
            u16 title = func_ov225_021b9c40(category->common->data, i, j);
            u16 condition = func_ov225_021b9bd4(category->common->data, title);

            if (func_ov225_021b9c58(category->common->data, condition, category->common->gameData)) {
                open++;
            }
        }
        if (open != 0) {
            category->listParam.items[category->listParam.count].id = i;
            category->listParam.items[category->listParam.count].msg =
                func_ov225_021b9c28(category->common->data, i);
            category->listParam.items[category->listParam.count].mark = MANUAL_LIST_MARK_NONE;
            category->listParam.count++;
        }
    }
    category->listParam.scroll = category->param->scroll;
    category->listParam.cursor = category->param->cursor;
    category->list = func_ov225_021ba65c(&category->listParam, common);
    category->vblankTask = GFL_VBlankTCBAdd(func_ov225_021bb850, category, 1);
    return category;
}

// pokebw2's ManualCategory_Free
void func_ov225_021bb7f4(ManualCategory *category) {
    GFL_TCBRemove(category->vblankTask);
    category->param->scroll = category->listParam.scroll;
    category->param->cursor = category->listParam.cursor;
    if (category->listParam.result == MANUAL_LIST_RESULT_RETURN) {
        category->param->result = MANUAL_LIST_RESULT_RETURN;
    } else if (category->listParam.result == MANUAL_LIST_RESULT_DECIDE) {
        category->param->result = MANUAL_LIST_RESULT_DECIDE;
        category->param->selected = category->listParam.items[category->param->cursor].id;
    }
    func_ov225_021ba740(category->list);
    GFL_HeapFree(category->listParam.items);
    GFL_HeapFree(category);
}

// Returns whether the screen has ended
// pokebw2's ManualCategory_Main
BOOL func_ov225_021bb844(ManualCategory *category) {
    return func_ov225_021ba760(category->list);
}

// pokebw2's ManualCategory_VBlank
static void func_ov225_021bb850(TCB *tcb, void *data) {
}
