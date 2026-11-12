// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_btn_data.h: the lines
// in use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_BTN_DATA_H
#define BWPORT_APP_BATTLE_RECORDER_BR_BTN_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of BR_MENUID_MAX in Black
// The menus. Five belong to the Battle Recorder in the bag, five to the Global Link's Battle Videos and two to its
// musical photos
enum {
    BR_MENUID_BROWSE_TOP,
    BR_MENUID_GBV_TOP = 5,
    BR_MENUID_MUSICAL_TOP = 10,
    BR_MENUID_MAX = 12,
};

// Black 2's; twinned code confirms the values of BR_BTN_DATA_PARAM_X, BR_BTN_DATA_PARAM_Y, BR_BTN_DATA_PARAM_MSGID,
// BR_BTN_DATA_PARAM_ANMSEQ and BR_BTN_DATA_PARAM_VALID_TYPE in Black
// The parameters of a button
enum {
    BR_BTN_DATA_PARAM_X,
    BR_BTN_DATA_PARAM_Y,
    // The menu the button is in
    BR_BTN_DATA_PARAM_MENUID,
    BR_BTN_DATA_PARAM_MSGID,
    BR_BTN_DATA_PARAM_ANMSEQ,
    // BR_BTN_TYPE_*
    BR_BTN_DATA_PARAM_TYPE,
    // The menu or screen the button opens
    BR_BTN_DATA_PARAM_DATA1,
    // A message the button shows, or the mode of its screen
    BR_BTN_DATA_PARAM_DATA2,
    // Whether the button can be pressed
    BR_BTN_DATA_PARAM_VALID,
    // What a button that can't be pressed does: 1 shows the message BR_BTN_DATA_PARAM_UNVALID_DATA
    BR_BTN_DATA_PARAM_UNVALID_TYPE,
    BR_BTN_DATA_PARAM_UNVALID_DATA,
    // BR_BTN_VALID_TYPE_*
    BR_BTN_DATA_PARAM_VALID_TYPE,
    BR_BTN_DATA_PARAM_MAX,
};

// Black 2's; twinned code confirms the values of BR_BTN_VALID_TYPE_MINE and BR_BTN_VALID_TYPE_OTHER3 in Black
// What decides whether a button can be pressed: nothing, a saved video, or a saved musical photo
enum {
    BR_BTN_VALID_TYPE_NONE,
    BR_BTN_VALID_TYPE_MINE,
    BR_BTN_VALID_TYPE_OTHER1,
    BR_BTN_VALID_TYPE_OTHER2,
    BR_BTN_VALID_TYPE_OTHER3,
    BR_BTN_VALID_TYPE_MUSICAL,
};

typedef struct {
    u16 param[BR_BTN_DATA_PARAM_MAX];
} BrBtnData;

typedef struct {
    BrRecordInfo *recordInfo;
} BrBtnDataSetup;

// pokebw2's BrBtnData_Init
BrBtnDataSys *func_ov186_021fb5e4(const BrBtnDataSetup *cp_setup, HeapID heapId);
// pokebw2's BrBtnData_GetMaxNum
u32 func_ov186_021fb788(const BrBtnDataSys *cp_wk);
// pokebw2's BrBtnData_CreateStr
// The button's label, with the player's name for a saved video
StrBuf *func_ov186_021fb7d0(const BrBtnDataSys *cp_wk, const BrBtnData *cp_data, MsgData *msg, HeapID heapId);

// What a button does when pressed
enum {
    // Opens the menu BR_BTN_DATA_PARAM_DATA1
    BR_BTN_TYPE_SELECT,
    // Opens the menu BR_BTN_DATA_PARAM_DATA1 and shows the message BR_BTN_DATA_PARAM_DATA2
    BR_BTN_TYPE_MENU,
    // Returns to the menu before
    BR_BTN_TYPE_RETURN,
    // Leaves the Battle Recorder
    BR_BTN_TYPE_EXIT,
    // Starts the screen BR_BTN_DATA_PARAM_DATA1
    BR_BTN_TYPE_CHANGE_DISPLAY,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrBtnData_Exit
void func_ov186_021fb63c(BrBtnDataSys *p_wk);
// pokebw2's BrBtnData_GetData
const BrBtnData *func_ov186_021fb704(const BrBtnDataSys *cp_wk, int menuID, u16 btnID);
// pokebw2's BrBtnData_GetNum
// The number of buttons of a menu, and the most any menu has
u16 func_ov186_021fb760(const BrBtnDataSys *cp_wk, int menuID);
// pokebw2's BrBtnData_GetParam
u16 func_ov186_021fb7b0(const BrBtnData *cp_data, int paramID);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_BTN_DATA_H
