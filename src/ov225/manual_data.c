// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual.h"
#include "app/manual_data.h"
#include "constants/flags.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "save/event_work.h"
#include "save/misc_savedata.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"

// The manual's tables, named after the ROM's "manual_data.c". The titles and the categories are each a file of
// records of 16-bit words, with a file of the records' offsets after their count

// The words of a title: after these, the message and the picture of each page
enum {
    TITLE_MSG = 3,
    TITLE_CONDITION,
    TITLE_READ_FLAG,
    TITLE_PAGE_COUNT,
    TITLE_PAGES,
};

// The words of a category: after these, its titles
enum {
    CATEGORY_MSG = 1,
    CATEGORY_TITLE_COUNT,
    CATEGORY_TITLES,
};

// The picture of the manual's cover
#define DEFAULT_IMAGE 72

// The offsets of a file's records
typedef struct {
    u16 count;
    u16 offsets[1];
} ManualOffsets;

struct ManualData {
    u8 *titles;
    ManualOffsets *titleOffsets;
    u8 *categories;
    ManualOffsets *categoryOffsets;
};

static u16 *func_ov225_021b9ce0(ManualData *data, u16 title);
u16 func_ov225_021b9cf0(ManualData *data, u16 title);
static u16 *func_ov225_021b9cfc(ManualData *data, u16 category);
static u16 func_ov225_021b9d0c(ManualData *data, u16 category);

// pokebw2's ManualData_Create
ManualData *func_ov225_021b9b3c(ArcTool *arc, HeapID heapId) {
    u32 size;
    ManualData *data = GFL_HeapAllocate(heapId, sizeof(ManualData), TRUE, "manual_data.c", 156);

    data->titles = GFL_ArcToolReadHeapNewLZGetLen(arc, MANUAL_ARC_TITLE_DATA, FALSE, heapId, &size);
    data->titleOffsets = GFL_ArcToolReadHeapNewLZGetLen(arc, MANUAL_ARC_TITLE_OFFSETS, FALSE, heapId, &size);
    data->categories = GFL_ArcToolReadHeapNewLZGetLen(arc, MANUAL_ARC_CATEGORY_DATA, FALSE, heapId, &size);
    data->categoryOffsets = GFL_ArcToolReadHeapNewLZGetLen(arc, MANUAL_ARC_CATEGORY_OFFSETS, FALSE, heapId, &size);
    return data;
}

// pokebw2's ManualData_Free
void func_ov225_021b9ba4(ManualData *data) {
    GFL_HeapFree(data->categoryOffsets);
    GFL_HeapFree(data->categories);
    GFL_HeapFree(data->titleOffsets);
    GFL_HeapFree(data->titles);
    GFL_HeapFree(data);
}

// pokebw2's ManualData_GetTitleMsg
u16 func_ov225_021b9bc8(ManualData *data, u16 title) {
    return func_ov225_021b9ce0(data, title)[TITLE_MSG];
}

// pokebw2's ManualData_GetTitleCondition
u16 func_ov225_021b9bd4(ManualData *data, u16 title) {
    return func_ov225_021b9ce0(data, title)[TITLE_CONDITION];
}

// pokebw2's ManualData_GetTitleReadFlag
u16 func_ov225_021b9be0(ManualData *data, u16 title) {
    return func_ov225_021b9ce0(data, title)[TITLE_READ_FLAG];
}

// pokebw2's ManualData_GetTitlePageCount
u16 func_ov225_021b9bec(ManualData *data, u16 title) {
    return func_ov225_021b9ce0(data, title)[TITLE_PAGE_COUNT];
}

// pokebw2's ManualData_GetPageMsg
u16 func_ov225_021b9bf8(ManualData *data, u16 title, u16 page) {
    return func_ov225_021b9ce0(data, title)[TITLE_PAGES + page * 2];
}

// pokebw2's ManualData_GetPageImage
u16 func_ov225_021b9c0c(ManualData *data, u16 title, u16 page) {
    return func_ov225_021b9ce0(data, title)[TITLE_PAGES + 1 + page * 2];
}

// pokebw2's ManualData_GetTitleCount
u16 func_ov225_021b9c20(ManualData *data) {
    return data->titleOffsets->count;
}

// pokebw2's ManualData_GetCategoryMsg
u16 func_ov225_021b9c28(ManualData *data, u16 category) {
    return func_ov225_021b9cfc(data, category)[CATEGORY_MSG];
}

// pokebw2's ManualData_GetCategoryTitleCount
u16 func_ov225_021b9c34(ManualData *data, u16 category) {
    return func_ov225_021b9cfc(data, category)[CATEGORY_TITLE_COUNT];
}

// pokebw2's ManualData_GetCategoryTitle
u16 func_ov225_021b9c40(ManualData *data, u16 category, u16 index) {
    return func_ov225_021b9cfc(data, category)[CATEGORY_TITLES + index];
}

// pokebw2's ManualData_GetCategoryCount
u16 func_ov225_021b9c50(ManualData *data) {
    return data->categoryOffsets->count;
}

// pokebw2's ManualData_IsOpen
BOOL func_ov225_021b9c58(ManualData *data, u16 condition, GameData *gameData) {
    BOOL open = FALSE;

    switch (condition) {
    case MANUAL_CONDITION_NONE:
        open = TRUE;
        break;
    case MANUAL_CONDITION_C_GEAR:
        if (func_020090e0(func_02009210(GameData_GetSaveControl(gameData)))) {
            open = TRUE;
        }
        break;
    case MANUAL_CONDITION_FLAG:
        if (EventWork_FlagGet(GameData_GetEventWork(gameData), EVENT_FLAG_0x0960)) {
            open = TRUE;
        }
        break;
    }
    return open;
}

// pokebw2's ManualData_IsRead
BOOL func_ov225_021b9c98(ManualData *data, u16 flag, GameData *gameData) {
    return func_0200edec(getHollow_RivalData(GameData_GetSaveControl(gameData)), flag);
}

// pokebw2's ManualData_SetRead
void func_ov225_021b9cb0(ManualData *data, u16 flag, GameData *gameData) {
    func_0200ee14(getHollow_RivalData(GameData_GetSaveControl(gameData)), flag);
}

// pokebw2's ManualData_IsImageValid
BOOL func_ov225_021b9cc8(ManualData *data, u16 image) {
    if (image != MANUAL_IMAGE_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ManualData_GetDefaultImage
u16 func_ov225_021b9cdc(ManualData *data) {
    return DEFAULT_IMAGE;
}

// pokebw2's ManualData_GetTitle
static u16 *func_ov225_021b9ce0(ManualData *data, u16 title) {
    return (u16 *)(data->titles + func_ov225_021b9cf0(data, title));
}

// pokebw2's ManualData_GetTitleOffset
u16 func_ov225_021b9cf0(ManualData *data, u16 title) {
    return data->titleOffsets->offsets[title];
}

// pokebw2's ManualData_GetCategory
static u16 *func_ov225_021b9cfc(ManualData *data, u16 category) {
    return (u16 *)(data->categories + func_ov225_021b9d0c(data, category));
}

// pokebw2's ManualData_GetCategoryOffset
static u16 func_ov225_021b9d0c(ManualData *data, u16 category) {
    return data->categoryOffsets->offsets[category];
}
