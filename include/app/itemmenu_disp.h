// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/itemmenu_disp.h: the lines in use so
// far.
#ifndef BWPORT_APP_ITEMMENU_DISP_H
#define BWPORT_APP_ITEMMENU_DISP_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's ItemMenuDisp_ShowBGs
void func_ov070_021b913c(void);
// pokebw2's ItemMenuDisp_ShowTMInfoBGs
void func_ov070_021b97cc(ItemMenuWork *work, BOOL show);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ItemMenuDisp_Init
void func_ov070_021b9280(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_FreeBGChars
void func_ov070_021b9758(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_ShowTMIcons
void func_ov070_021b97e8(ItemMenuWork *work, BOOL keys);
// pokebw2's ItemMenuDisp_DrawItemInfo
void func_ov070_021b9850(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_HideItemInfo
void func_ov070_021b9aac(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_Exit
void func_ov070_021b9af8(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_CreateWindows
void func_ov070_021b9bdc(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_Update
void func_ov070_021b9ed8(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_LoadListRes
void func_ov070_021b9ef4(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_CreateListActors
void func_ov070_021ba028(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_DrawList
void func_ov070_021ba218(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_UpdateScrollBar
void func_ov070_021ba6a4(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_DrawPocketName
void func_ov070_021baf38(ItemMenuWork *work, u32 pocket);
// pokebw2's ItemMenuDisp_SetPocketTab
void func_ov070_021ba96c(ItemMenuWork *work, u32 pocket);
// pokebw2's ItemMenuDisp_LoadPocketFrame
void func_ov070_021baaa4(ItemMenuWork *work, u32 pocket);
// pokebw2's ItemMenuDisp_SetMoveButtons
void func_ov070_021bb5dc(ItemMenuWork *work, BOOL a1);
// pokebw2's ItemMenuDisp_IsMessageDone
BOOL func_ov070_021badf0(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_SetButtonsActive
void func_ov070_021bb3f4(ItemMenuWork *work, BOOL active);
// pokebw2's ItemMenuDisp_ShowMessageNow
void func_ov070_021badac(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_CloseMenu
void func_ov070_021bb1ac(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_OpenYesNoMenu
void func_ov070_021bb0f8(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_DrawMoney
void func_ov070_021bafc4(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_SetListCursorPalette
void func_ov070_021bb398(ItemMenuWork *work, u32 state);
// pokebw2's ItemMenuDisp_GetPocketShortcut
u32 func_ov070_021bb368(u32 index);
// pokebw2's ItemMenuDisp_SlidePocketTabs
BOOL func_ov070_021bb690(ItemMenuWork *work);
// pokebw2's ItemMenuDisp_UpdateListCursor
void func_ov070_021ba41c(ItemMenuWork *work);

void func_ov070_021bacf0(ItemMenuWork *work, s32 mode);

void func_ov070_021ba654(ItemMenuWork *work);
void func_ov070_021bb5a4(ItemMenuWork *work);
void func_ov070_021ba9c0(ItemMenuWork *work, u32 *msgIds, s32 count);
void func_ov070_021ba4a4(ItemMenuWork *work);
void func_ov070_021bb73c(ItemMenuWork *work);
void func_ov070_021baf10(ItemMenuWork *work);
void func_ov070_021bb1c4(ItemMenuWork *work);
void func_ov070_021bb260(ItemMenuWork *work, s32 quantity);
void func_ov070_021ba9ac(ItemMenuWork *work);
void func_ov070_021bb0bc(ItemMenuWork *work);
void func_ov070_021bb580(ItemMenuWork *work, BOOL keys);
void func_ov070_021bb08c(ItemMenuWork *work);
void func_ov070_021badb8(ItemMenuWork *work, u32 item);
void func_ov070_021ba8c0(ItemMenuWork *work);
void func_ov070_021bae98(ItemMenuWork *work);

#endif // BWPORT_APP_ITEMMENU_DISP_H
