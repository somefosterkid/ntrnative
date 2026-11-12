// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mystery/mystery_util.h: the lines in use
// so far.
#ifndef BWPORT_APP_MYSTERY_MYSTERY_UTIL_H
#define BWPORT_APP_MYSTERY_MYSTERY_UTIL_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// A step of a sequence, called each frame with the sequence's state and work
typedef void (*MysterySeqFunc)(MysterySeq *seq, u32 *state, void *work);

// The answers of the menus' updates while they wait
#define MYSTERY_MENU_NONE ((u32) - 1)

// Black 2's, not checked against Black
struct MysteryYesNoSetup {
    MsgData *msgData;
    Font *font;
    PrintQueue *queue;
    u32 msgIds[3];
    u32 count;
    u16 bg;
    u16 palette;
    u16 framePalette;
    u16 frameChar;
};

// Black 2's; twinned code confirms the offsets of str in Black
// A window of MysteryTextWin, which prints a message or a string
struct MysteryTextWinEntry {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u32 msgId;
    // Printed instead of the message when not NULL
    StrBuf *str;
    u32 align;
    s16 textX;
    s16 textY;
    u16 color;
};

// A change of a MysteryTextWin's window, which MysteryTextWinCopy_Print makes when update is set
struct MysteryTextWinUpdate {
    BOOL update;
    u32 msgId;
    StrBuf *str;
    u32 align;
    s16 textX;
    s16 textY;
    u16 color;
};

// How a message window prints: all at once, a character at a time waiting for a key at the end, or all at once
// with the wait icon
enum {
    MYSTERY_PRINT_QUEUE,
    MYSTERY_PRINT_STREAM,
    MYSTERY_PRINT_WAIT_ICON,
    MYSTERY_PRINT_NONE,
};

// pokebw2's MysteryMsgWin_Delete
void func_ov121_021d9e04(MysteryMsgWin *win);
// pokebw2's MysteryTextWin_Clear
void func_ov121_021da2a8(MysteryTextWin *win);
// pokebw2's MysteryTextWin_Delete
void func_ov121_021da280(MysteryTextWin *win);
// pokebw2's MysteryTextWin_Update
BOOL func_ov121_021da2c8(MysteryTextWin *win);
// pokebw2's MysteryTextLine_Update
BOOL func_ov121_021d9b34(MysteryTextLine *line);
// pokebw2's MysteryTextLine_Create
// A window that prints one string, the line of a MysteryTextWin
MysteryTextLine *func_ov121_021d99a4(BOOL deferFlush, u16 bg, u8 x, u8 y, u8 width, u8 height, u8 palette,
                                        PrintQueue *queue, HeapID heapId);
// pokebw2's MysteryTextLine_Delete
void func_ov121_021d9a40(MysteryTextLine *line);
// pokebw2's MysteryTextLine_SetColor
void func_ov121_021d9b28(MysteryTextLine *line, u16 color);
// pokebw2's MysteryTextLine_SetPos
void func_ov121_021d9b2c(MysteryTextLine *line, s32 x, s32 y, u32 align);
// pokebw2's MysteryTextLine_PrintStr
void func_ov121_021d9ac8(MysteryTextLine *line, const StrBuf *str, Font *font);
// pokebw2's MysteryMsgWin_Create
MysteryMsgWin *func_ov121_021d9ca4(u16 bg, u8 palette, PrintQueue *queue, Font *font, HeapID heapId);
// pokebw2's MysteryMsgWin_DrawFrame
void func_ov121_021d9ffc(MysteryMsgWin *win, u16 frameChar, u8 framePalette);
// pokebw2's MysteryMsgWin_Print
// mode is a MYSTERY_PRINT_*
void func_ov121_021d9f08(MysteryMsgWin *win, MsgData *msgData, u32 msgId, u32 mode);
// pokebw2's MysteryYesNo_Update
u32 func_ov121_021da178(MysteryYesNo *menu);
// pokebw2's MysteryYesNo_Delete
void func_ov121_021da148(MysteryYesNo *menu);
// pokebw2's MysteryMsgWin_CreateSmall
// A message window of one line, which waits for a key after its stream
MysteryMsgWin *func_ov121_021d9d4c(u16 bg, u8 palette, PrintQueue *queue, Font *font, HeapID heapId);
// pokebw2's MysteryMsgWin_Update
void func_ov121_021d9e50(MysteryMsgWin *win);
// pokebw2's MysteryTextWin_ClearLine
void func_ov121_021da2f0(MysteryTextWin *win, u32 index);
// pokebw2's MysteryTextWinCopy_Print
void func_ov121_021da3ec(MysteryTextWinCopy *copy, const MysteryTextWinUpdate *updates, PrintQueue *queue);
// pokebw2's MysteryTextWinCopy_Delete
void func_ov121_021da3c0(MysteryTextWinCopy *copy);
// pokebw2's MysteryTextWinCopy_Apply
void func_ov121_021da4a8(MysteryTextWinCopy *copy);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MysterySeq_Create
MysterySeq *func_ov121_021da854(void *work, MysterySeqFunc func, HeapID heapId);
// pokebw2's MysteryTextWin_Create
MysteryTextWin *func_ov121_021da1bc(BOOL deferFlush, const MysteryTextWinEntry *entries, u32 count, u16 bg,
                                      u8 palette, PrintQueue *queue, MsgData *msgData, Font *font, HeapID heapId);
// pokebw2's MysterySeq_Delete
void func_ov121_021da890(MysterySeq *seq);
// pokebw2's MysterySeq_Main
void func_ov121_021da898(MysterySeq *seq);
// pokebw2's MysterySeq_IsEnd
BOOL func_ov121_021da8ac(MysterySeq *seq);
// pokebw2's MysterySeq_SetNext
void func_ov121_021da8b0(MysterySeq *seq, MysterySeqFunc func);
// pokebw2's MysteryYesNo_Create
MysteryYesNo *func_ov121_021da038(const MysteryYesNoSetup *setup, HeapID heapId);
// pokebw2's MysteryMsgWin_IsDone
BOOL func_ov121_021d9ff8(MysteryMsgWin *win);
// pokebw2's MysterySeq_End
void func_ov121_021da8b8(MysterySeq *seq);
// pokebw2's MysteryPal_Blend
void func_ov121_021dab50(u32 type, u16 *dest, u16 angle, u32 palette, const u16 *from, const u16 *to);
// pokebw2's MysteryTextWinCopy_Create
MysteryTextWinCopy *func_ov121_021da338(MysteryTextWin *textWin, HeapID heapId);
// pokebw2's MysteryTextWin_Flush
void func_ov121_021da318(MysteryTextWin *win);

#include "gfl/clact.h"

#define MYSTERY_MENU_CANCEL ((u32) - 2)

// Black 2's, not checked against Black
// The list menu of MysteryList: items are messages, or strings when msgData is NULL
struct MysteryListSetup {
    MsgData *msgData;
    Font *font;
    PrintQueue *queue;
    ClActor *cursor;
    // A message id each, or a string when msgData is NULL
    union {
        u32 msgId;
        StrBuf *str;
    } items[4];
    u32 count;
    u16 bg;
    u16 palette;
    u16 unk28;
    // The BG palette whose colors 10 to 13 gray out disabled items
    u16 bgPalette;
    s16 offsetY;
    s16 offsetX;
    u16 cursorSequence;
    // Called when the cursor moves, with work
    void (*onMove)(void *work);
    void *work;
    // Where the cursor is kept
    u32 *cursorPos;
};

// pokebw2's MysteryList_SetItemGrayed
void func_ov121_021da84c(MysteryList *list, u32 item, BOOL grayed);
// pokebw2's MysterySeq_Return
void func_ov121_021da8c4(MysterySeq *seq);
// pokebw2's MysteryList_Delete
void func_ov121_021da698(MysteryList *list);
// pokebw2's MysteryList_Update
u32 func_ov121_021da6d0(MysteryList *list);
// pokebw2's MysteryOamText_Clear
void func_ov121_021da978(MysteryOamText *oamText);

// pokebw2's MysteryList_UpdatePrint
void func_ov121_021da840(MysteryList *list);
// pokebw2's MysteryPal_BlendOne
void func_ov121_021daaa4(u32 type, u16 *dest, u16 angle, u8 palette, u8 index, u16 from, u16 to);
// pokebw2's MysterySeq_SetReturn
// Sets the state that MysterySeq_Return goes back to, after a step such as a message
void func_ov121_021da8c0(MysterySeq *seq, u32 state);
// pokebw2's MysteryOamText_Create
MysteryOamText *func_ov121_021da8cc(const ClActorSetup *setup, u16 width, u16 height, u32 palette, u8 paletteOffset,
                                      u32 surface, BmpOamSys *bmpOam, PrintQueue *queue, HeapID heapId);
// pokebw2's MysteryOamText_SetColor
void func_ov121_021da9ec(MysteryOamText *oamText, u16 color);
// pokebw2's MysteryOamText_SetAlign
void func_ov121_021da9f0(MysteryOamText *oamText, s32 x, s32 y, u32 align);
// pokebw2's MysteryOamText_Print
void func_ov121_021da994(MysteryOamText *oamText, MsgData *msgData, u32 msgId, Font *font);
// pokebw2's MysteryOamText_Delete
void func_ov121_021da958(MysteryOamText *oamText);
// pokebw2's MysteryOamText_Update
BOOL func_ov121_021da9f8(MysteryOamText *oamText);
// pokebw2's MysteryTextWin_Flush
void func_ov121_021da318(MysteryTextWin *win);
// pokebw2's MysteryTextWinCopy_Create
MysteryTextWinCopy *func_ov121_021da338(MysteryTextWin *textWin, HeapID heapId);
// pokebw2's MysteryList_Create
MysteryList *func_ov121_021da4dc(const MysteryListSetup *setup, HeapID heapId);

#endif // BWPORT_APP_MYSTERY_MYSTERY_UTIL_H

