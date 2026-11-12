// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_title.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual_common.h"
#include "app/manual_data.h"
#include "app/manual_list.h"
#include "app/manual_title.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"

// The manual's list of titles, named after the ROM's "manual_title.c": the titles the player can read, of a category
// or of them all, with a mark on those not read yet

struct ManualTitle {
    ManualTitleParam *param;
    ManualCommon *common;
    ManualListParam listParam;
    ManualList *list;
    TCB *vblankTask;
};

static void func_ov225_021bba70(TCB *tcb, void *data);

// pokebw2's ManualTitle_Create
ManualTitle *func_ov225_021bb854(ManualTitleParam *param, ManualCommon *common) {
    u16 i;
    ManualTitle *title = GFL_HeapAllocate(common->heapId, sizeof(ManualTitle), TRUE, "manual_title.c", 97);

    title->param = param;
    param->result = MANUAL_LIST_RESULT_RETURN;
    title->param->selected = 0;
    title->common = common;
    if (title->param->mode == MANUAL_TITLE_MODE_CATEGORY) {
        u16 category;
        u16 count;

        title->listParam.type = MANUAL_LIST_TITLE;
        category = title->param->category;
        count = func_ov225_021b9c34(title->common->data, category);
        title->listParam.count = 0;
        title->listParam.items =
            GFL_HeapAllocate(title->common->heapId, count * sizeof(ManualListItem), TRUE, "manual_title.c", 121);
        for (i = 0; i < count; i++) {
            u16 id = func_ov225_021b9c40(title->common->data, category, i);
            u16 condition = func_ov225_021b9bd4(title->common->data, id);

            if (func_ov225_021b9c58(title->common->data, condition, title->common->gameData)) {
                u16 flag = func_ov225_021b9be0(title->common->data, id);
                BOOL read = func_ov225_021b9c98(title->common->data, flag, title->common->gameData);

                title->listParam.items[title->listParam.count].id = id;
                title->listParam.items[title->listParam.count].msg = func_ov225_021b9bc8(title->common->data, id);
                title->listParam.items[title->listParam.count].mark =
                    read ? MANUAL_LIST_MARK_NONE : MANUAL_LIST_MARK_NEW;
                title->listParam.count++;
            }
        }
        title->listParam.scroll = title->param->scroll;
        title->listParam.cursor = title->param->cursor;
    } else {
        u16 count;

        title->listParam.type = MANUAL_LIST_TITLE;
        count = func_ov225_021b9c20(title->common->data);
        title->listParam.count = 0;
        title->listParam.items =
            GFL_HeapAllocate(title->common->heapId, count * sizeof(ManualListItem), TRUE, "manual_title.c", 178);
        for (i = 0; i < count; i++) {
            u16 condition = func_ov225_021b9bd4(title->common->data, i);

            if (func_ov225_021b9c58(title->common->data, condition, title->common->gameData)) {
                u16 flag = func_ov225_021b9be0(title->common->data, i);
                BOOL read = func_ov225_021b9c98(title->common->data, flag, title->common->gameData);

                title->listParam.items[title->listParam.count].id = i;
                title->listParam.items[title->listParam.count].msg = func_ov225_021b9bc8(title->common->data, i);
                title->listParam.items[title->listParam.count].mark =
                    read ? MANUAL_LIST_MARK_NONE : MANUAL_LIST_MARK_NEW;
                title->listParam.count++;
            }
        }
        title->listParam.scroll = title->param->scroll;
        title->listParam.cursor = title->param->cursor;
    }
    title->list = func_ov225_021ba65c(&title->listParam, common);
    title->vblankTask = GFL_VBlankTCBAdd(func_ov225_021bba70, title, 1);
    return title;
}

// pokebw2's ManualTitle_Free
void func_ov225_021bba14(ManualTitle *title) {
    GFL_TCBRemove(title->vblankTask);
    title->param->scroll = title->listParam.scroll;
    title->param->cursor = title->listParam.cursor;
    if (title->listParam.result == MANUAL_LIST_RESULT_RETURN) {
        title->param->result = MANUAL_LIST_RESULT_RETURN;
    } else if (title->listParam.result == MANUAL_LIST_RESULT_DECIDE) {
        title->param->result = MANUAL_LIST_RESULT_DECIDE;
        title->param->selected = title->listParam.items[title->param->cursor].id;
    }
    func_ov225_021ba740(title->list);
    GFL_HeapFree(title->listParam.items);
    GFL_HeapFree(title);
}

// Returns whether the screen has ended
// pokebw2's ManualTitle_Main
BOOL func_ov225_021bba64(ManualTitle *title) {
    return func_ov225_021ba760(title->list);
}

// pokebw2's ManualTitle_VBlank
static void func_ov225_021bba70(TCB *tcb, void *data) {
}
