// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_btn_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's menus: the buttons of each menu, which this file copies and marks as valid by what is saved.
// The name is the ROM's string, from GFL_HeapAllocate's asserts

#include "types.h"
#include "app/battle_recorder/br_btn_data.h"
#include "app/battle_recorder/br_core.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "system/wordset.h"

// The menus that lead to a menu, as the menu and the button pressed in each, ending in BR_BTN_LINK_END
typedef struct {
    u32 menuID;
    u32 btnID;
} BrBtnLink;

#define BR_BTN_LINK_END 0xffffffff

typedef struct {
    u16 max;
    const BrBtnData *cp_data;
    const BrBtnLink *cp_link;
} BrBtnDataTbl;

struct BrBtnDataSys {
    BrBtnDataSetup setup;
    BrBtnData data[];
};

static void func_ov186_021fb644(BrBtnDataSys *p_wk);

// Declared in the order that gives the ROM's layout, which MWCC sorts by size
// pokebw2's sc_link_menu00
static const BrBtnLink data_ov186_02200378[] = {
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu01
static const BrBtnLink data_ov186_022003c0[] = {
    { 0, 0 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu02
static const BrBtnLink data_ov186_022003f8[] = {
    { 0, 0 },
    { 1, 1 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu03
static const BrBtnLink data_ov186_02200410[] = {
    { 0, 0 },
    { 1, 2 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu07
static const BrBtnLink data_ov186_022003e0[] = {
    { 5, 0 },
    { 6, 0 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu04
static const BrBtnLink data_ov186_02200428[] = {
    { 0, 0 },
    { 1, 2 },
    { 3, 1 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu09
static const BrBtnLink data_ov186_022003b0[] = {
    { 5, 2 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu06
static const BrBtnLink data_ov186_02200390[] = {
    { 5, 0 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu10
static const BrBtnLink data_ov186_02200388[] = {
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu05
static const BrBtnLink data_ov186_02200380[] = {
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu08
static const BrBtnLink data_ov186_022003a0[] = {
    { 5, 1 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_link_menu11
static const BrBtnLink data_ov186_022003d0[] = {
    { 10, 2 },
    { BR_BTN_LINK_END, BR_BTN_LINK_END },
};

// pokebw2's sc_btn_menu00
static const BrBtnData data_ov186_02200610[] = {
#include "asset/br_btn_data_data_ov186_02200610.inc"
};

// pokebw2's sc_btn_menu01
static const BrBtnData data_ov186_02200670[] = {
#include "asset/br_btn_data_data_ov186_02200670.inc"
};

// pokebw2's sc_btn_menu05
static const BrBtnData data_ov186_022004d8[] = {
#include "asset/br_btn_data_data_ov186_022004d8.inc"
};

// pokebw2's sc_btn_menu06
static const BrBtnData data_ov186_02200730[] = {
#include "asset/br_btn_data_data_ov186_02200730.inc"
};

// pokebw2's sc_btn_menu03
static const BrBtnData data_ov186_02200568[] = {
#include "asset/br_btn_data_data_ov186_02200568.inc"
};

// pokebw2's sc_btn_menu04
static const BrBtnData data_ov186_02200790[] = {
#include "asset/br_btn_data_data_ov186_02200790.inc"
};

// pokebw2's sc_btn_menu07
static const BrBtnData data_ov186_022005b0[] = {
#include "asset/br_btn_data_data_ov186_022005b0.inc"
};

// pokebw2's sc_btn_menu02
static const BrBtnData data_ov186_022006d0[] = {
#include "asset/br_btn_data_data_ov186_022006d0.inc"
};

// pokebw2's sc_btn_menu10
static const BrBtnData data_ov186_02200520[] = {
#include "asset/br_btn_data_data_ov186_02200520.inc"
};

// pokebw2's sc_btn_menu09
static const BrBtnData data_ov186_022004a8[] = {
#include "asset/br_btn_data_data_ov186_022004a8.inc"
};

// pokebw2's sc_btn_menu08
static const BrBtnData data_ov186_02200448[] = {
#include "asset/br_btn_data_data_ov186_02200448.inc"
};

// pokebw2's sc_btn_menu11
static const BrBtnData data_ov186_02200478[] = {
#include "asset/br_btn_data_data_ov186_02200478.inc"
};

// pokebw2's sc_btn_data_tbl
static const BrBtnDataTbl data_ov186_022007f0[BR_MENUID_MAX] = {
    { NELEMS(data_ov186_02200610), data_ov186_02200610, data_ov186_02200378 }, { NELEMS(data_ov186_02200670), data_ov186_02200670, data_ov186_022003c0 },
    { NELEMS(data_ov186_022006d0), data_ov186_022006d0, data_ov186_022003f8 }, { NELEMS(data_ov186_02200568), data_ov186_02200568, data_ov186_02200410 },
    { NELEMS(data_ov186_02200790), data_ov186_02200790, data_ov186_02200428 }, { NELEMS(data_ov186_022004d8), data_ov186_022004d8, data_ov186_02200380 },
    { NELEMS(data_ov186_02200730), data_ov186_02200730, data_ov186_02200390 }, { NELEMS(data_ov186_022005b0), data_ov186_022005b0, data_ov186_022003e0 },
    { NELEMS(data_ov186_02200448), data_ov186_02200448, data_ov186_022003a0 }, { NELEMS(data_ov186_022004a8), data_ov186_022004a8, data_ov186_022003b0 },
    { NELEMS(data_ov186_02200520), data_ov186_02200520, data_ov186_02200388 }, { NELEMS(data_ov186_02200478), data_ov186_02200478, data_ov186_022003d0 },
};

// pokebw2's BrBtnData_Init
BrBtnDataSys *func_ov186_021fb5e4(const BrBtnDataSetup *cp_setup, HeapID heapId) {
    BrBtnDataSys *p_wk;
    u32 size;
    u32 num = 0;
    int i;

    for (i = 0; i < BR_MENUID_MAX; i++) {
        num += data_ov186_022007f0[i].max;
    }
    size = sizeof(BrBtnDataSys) + sizeof(BrBtnData) * num;
    p_wk = GFL_HeapAllocate(heapId, size, FALSE, "br_btn_data.c", 1065);
    MI_CpuFill8(p_wk, 0, size);
    p_wk->setup = *cp_setup;
    func_ov186_021fb644(p_wk);
    return p_wk;
}

// pokebw2's BrBtnData_Exit
void func_ov186_021fb63c(BrBtnDataSys *p_wk) {
    GFL_HeapFree(p_wk);
}

// Copies the buttons and marks those of saved videos and photos valid when they are saved
// pokebw2's BrBtnData_Load
static void func_ov186_021fb644(BrBtnDataSys *p_wk) {
    const BrRecordInfo *cp_info = p_wk->setup.recordInfo;
    int i;
    u32 j;
    int idx = 0;

    for (i = 0; i < BR_MENUID_MAX; i++) {
        u32 max = data_ov186_022007f0[i].max;
        const BrBtnData *cp_data = data_ov186_022007f0[i].cp_data;

        for (j = 0; j < max; j++) {
            p_wk->data[idx] = cp_data[j];
            switch (p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID_TYPE]) {
            case BR_BTN_VALID_TYPE_MINE:
                p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID] = cp_info->isValid[0];
                break;
            case BR_BTN_VALID_TYPE_OTHER1:
            case BR_BTN_VALID_TYPE_OTHER2:
            case BR_BTN_VALID_TYPE_OTHER3: {
                u32 id = p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID_TYPE] - BR_BTN_VALID_TYPE_OTHER1;

                GFL_ASSERT(id < 4);
                p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID] = cp_info->isValid[1 + id];
                if (p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID]) {
                    p_wk->data[idx].param[BR_BTN_DATA_PARAM_MSGID] = 26;
                }
                break;
            }
            case BR_BTN_VALID_TYPE_MUSICAL:
                p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID] = cp_info->hasMusicalShot;
                break;
            default:
                p_wk->data[idx].param[BR_BTN_DATA_PARAM_VALID] = TRUE;
                break;
            }
            idx++;
        }
    }
}

// pokebw2's BrBtnData_GetData
const BrBtnData *func_ov186_021fb704(const BrBtnDataSys *cp_wk, int menuID, u16 btnID) {
    int i;
    u32 idx;

    GFL_ASSERT(menuID < BR_MENUID_MAX);
    GFL_ASSERT(btnID < data_ov186_022007f0[menuID].max);
    idx = 0;
    for (i = 0; i < menuID; i++) {
        idx += data_ov186_022007f0[i].max;
    }
    return &cp_wk->data[idx + btnID];
}

// pokebw2's BrBtnData_GetNum
u16 func_ov186_021fb760(const BrBtnDataSys *cp_wk, int menuID) {
    GFL_ASSERT(menuID < BR_MENUID_MAX);
    return data_ov186_022007f0[menuID].max;
}

// pokebw2's BrBtnData_GetMaxNum
u32 func_ov186_021fb788(const BrBtnDataSys *cp_wk) {
    u32 max = 0;
    int i;

    for (i = 0; i < BR_MENUID_MAX; i++) {
        if (max < func_ov186_021fb760(cp_wk, i)) {
            max = func_ov186_021fb760(cp_wk, i);
        }
    }
    return max;
}

// pokebw2's BrBtnData_GetParam
u16 func_ov186_021fb7b0(const BrBtnData *cp_data, int paramID) {
    GFL_ASSERT(paramID < BR_BTN_DATA_PARAM_MAX);
    return cp_data->param[paramID];
}

// pokebw2's BrBtnData_CreateStr
StrBuf *func_ov186_021fb7d0(const BrBtnDataSys *cp_wk, const BrBtnData *cp_data, MsgData *msg, HeapID heapId) {
    StrBuf *str;
    StrBuf *src;
    WordSet *wordset;
    int validType = func_ov186_021fb7b0(cp_data, BR_BTN_DATA_PARAM_VALID_TYPE);
    BOOL isName = FALSE;
    u32 id;

    if (validType >= BR_BTN_VALID_TYPE_MINE && validType <= BR_BTN_VALID_TYPE_OTHER3) {
        id = validType - BR_BTN_VALID_TYPE_MINE;
        isName = TRUE;
    }

    if (isName) {
        if (cp_wk->setup.recordInfo->isValid[id]) {
            wordset = GFL_WordSetSystemCreateDefault(HEAPID_TAIL(heapId));
            func_0201ee0c(wordset, 0, cp_wk->setup.recordInfo->name[id], cp_wk->setup.recordInfo->sex[id], TRUE, 2);
            src = GFL_MsgDataLoadStrbufNew(msg, func_ov186_021fb7b0(cp_data, BR_BTN_DATA_PARAM_MSGID));
            str = GFL_StrBufCreate(GFL_StrBufGetCharCount(src) + 7, heapId);
            GFL_WordSetFormatStrbuf(wordset, str, src);
            GFL_WordSetSystemFree(wordset);
            GFL_StrBufFree(src);
        } else {
            str = GFL_MsgDataLoadStrbufNew(msg, func_ov186_021fb7b0(cp_data, BR_BTN_DATA_PARAM_MSGID));
        }
    } else {
        str = GFL_MsgDataLoadStrbufNew(msg, func_ov186_021fb7b0(cp_data, BR_BTN_DATA_PARAM_MSGID));
    }
    return str;
}
