// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/itemmenu.h: the lines in use so far.
#ifndef BWPORT_APP_ITEMMENU_H
#define BWPORT_APP_ITEMMENU_H

#include "types.h"
#include "app/bag_item.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "save/bag.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"
#include "struct_decls.h"

// The number of rows the item list shows
#define ITEMMENU_LIST_ROWS 6

// Black 2's, not checked against Black
// The rows of OBJ the list draws its items with, one more above and below the rows it shows
#define ITEMMENU_ROW_OBJS 8

// Black 2's, not checked against Black
typedef void (*ItemMenuState)(ItemMenuWork *work);

// Black's layout up to buttonsActive (itemmenu_disp.c's code confirms every offset); the fields after it are Black 2's,
// not checked against Black
struct ItemMenuWork {
    GameData *gameData;
    TrainerDataSave *trainerData;
    PlayerInfo *playerInfo;
    PlayerActionPerms *perms;
    u32 mode;
    void *cursor;
    BagSave *bag;
    BOOL isCycling;
    // A copy of the pocket, while its items are moved around
    BagItem items[310];
    ItemMenuState state;
    PrintWindow msgWindow;
    PrintWindow pocketNameWindow;
    PrintQueue *printQueue;
    PrintStream *printStream;
    // Whether the message was resumed after a pause
    BOOL streamResumed;
    KeyCursor *keyCursor;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    StrBuf *expandBuf;
    StrBuf *tempBuf;
    Font *font;
    u8 unk534[0x10];
    u16 heapId;
    u32 selIconPlt;
    u32 selIconChr;
    u32 selIconCel;
    u32 listCursorPlt;
    u32 unk558;
    u32 buttonPlt;
    u32 listCursorChr;
    u32 buttonChr;
    u32 listCursorCel;
    u32 rowNameCel;
    u32 buttonCel;
    u32 rowMarkPlt;
    u32 rowMarkChr;
    u32 rowMarkCel;
    u32 sortButtonPlt;
    u32 sortButtonChr;
    u32 rowNamePlt;
    u32 sortButtonCel;
    u32 pocketTabPlt;
    u32 pocketTabChr;
    u32 pocketTabCel;
    u32 typeIconChr[17];
    u32 typeIconPlt;
    u32 typeIconCel;
    u32 categoryIconChr[3];
    u32 rowNameChr[ITEMMENU_ROW_OBJS];
    GFLBitmap *rowNameBitmaps[ITEMMENU_ROW_OBJS];
    ClActor *rowNames[ITEMMENU_ROW_OBJS];
    u32 rowMarkTypes[ITEMMENU_ROW_OBJS];
    ClActor *rowMarks[ITEMMENU_ROW_OBJS];
    ButtonMan *buttonMan;
    TCB *vblankTask;
    ClActUnit *actorUnit;
    ClActor *selIcon;
    ClActor *listCursor;
    ClActor *scrollBar;
    ClActor *pocketTabs[5];
    ClActor *categoryIcons[3];
    ClActor *typeIcons[17];
    ClActor *buttons[7];
    ClActor *sortButton;
    TCBExManager *tcbManager;
    PrintWindow tmInfoWindow;
    PrintWindow itemNameWindow;
    PrintWindow countWindow;
    PrintWindow descWindow;
    PrintWindow quantityWindow;
    PrintWindow moneyWindow;
    PrintWindow moneyValueWindow;
    PrintWindow priceWindow;
    PrintWindow menuTitleWindow;
    AppTaskMenu *taskMenu;
    AppTaskMenuItem menuItems[APP_TASKMENU_ITEM_MAX];
    AppTaskMenuRes *taskMenuRes;
    u8 unk7e4[8];
    u32 quantityMode;
    s32 quantityRepeat;
    s32 quantity;
    u32 bg0Chars;
    u32 bg4Chars;
    u32 bg5Chars;
    u32 bg1Chars;
    u32 bg3Chars;
    s32 pocket;
    // The cursor's row on the screen, and the list's scroll, one less than the index of the top row
    s32 cursorRow;
    s32 scroll;
    // The scroll the list was last drawn at
    s32 drawnScroll;
    // Whether the list shows the copy in items, while items are moved around
    BOOL movingItem;
    u8 unk820[0x8];
    // The actions the item menu offers, by the row they are shown on
    u32 itemMenuActions[4];
    u32 cursorImageChars;
    u16 iconsDirty;
    u16 listDirty;
    u32 tmInfoRequest;
    u32 result;
    u32 menuAction;
    s32 item;
    BlinkPalAnm *paletteAnim;
    PaletteFade *paletteFade;
    BOOL buttonsActive;
    BOOL touchHeld;
    // Whether the item being moved was dragged
    BOOL touchMoved;
    u16 buttonAnim;
    u16 buttonAnimTimer;
    ItemMenuState buttonAnimNext;
    u32 savedRepeatWait;
    u32 savedRepeatStart;
};

// pokebw2's ItemMenu_IsItemRegistered
// Whether an item is registered for the Y button
BOOL func_ov070_021b83a0(ItemMenuWork *work, u32 item);
// pokebw2's ItemMenu_CanRegister
BOOL func_ov070_021b811c(ItemMenuWork *work);
// pokebw2's ItemMenu_GetSellPrice
// What a count of an item sells for
s32 func_ov070_021b619c(u32 item, s32 count, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ItemMenu_GetSlot
// The slot of an item of the list
BagItem *func_ov070_021b616c(ItemMenuWork *work, s32 index);

// The bag screen's proc, from the field menu
extern const GameProcFunctions BAG_PROC_FUNCTIONS;

#endif // BWPORT_APP_ITEMMENU_H
