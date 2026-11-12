#ifndef BWPORT_FIELD_FIELD_MSGBG_H
#define BWPORT_FIELD_FIELD_MSGBG_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "nitro/fx/fx.h"
#include "gfl/g3d.h"
#include "nnsys/g2d.h"
#include "system/bmp_menulist.h"


// One of the four print slots of a FieldMsgBG, 0x24 bytes.
struct FieldMsgBGWin {
    FieldMsgBG *owner;         // 0x00
    s32 unk_4;                 // 0x04
    Font *font;                // 0x08
    PrintQueue *printQueue;    // 0x0c
    MsgData *msgData;          // 0x10
    BmpWin *unk_14;            // 0x14
    StrBuf *strBuf;            // 0x18
    PrintWindow printWin;      // 0x1c
};

// The page cursor blinking in the lower right corner of a window, 8 bytes (func_ov021_021927c0 sets it up).
struct MsgBGCursor {
    u8 shown;                  // 0x00  1 while it is drawn
    u8 frame;                  // 0x01  0..2, its bob
    u8 counter;                // 0x02
    u8 unk_3;
    GFLBitmap *bitmap;         // 0x04  its 8x8 tile
};

// A PrintStream printing into a window, 0xc bytes (func_ov021_02190c94).
struct MsgBGStream {
    s16 unk_0;
    s16 speed;                 // 0x02
    PrintStream *stream;       // 0x04
    FieldMsgBG *msgBG;         // 0x08
};

// A framed window printed through a print slot (the money window), 0xc bytes (MoneyWin_Create).
struct MsgBGSysWin {
    BmpWin *bmpWin;            // 0x00
    FieldMsgBGWin *win;        // 0x04
    FieldMsgBG *msgBG;         // 0x08
};

// The talk window on the message BG, 0x20 bytes (func_ov021_02190dcc).
struct MsgBGTalkWin {
    BmpWin *bmpWin;            // 0x00
    MsgBGStream *stream;       // 0x04
    FieldMsgBGWin *win;        // 0x08
    MsgData *msgData;          // 0x0c
    StrBuf *strBuf;            // 0x10
    FieldMsgBG *msgBG;         // 0x14
    MsgBGCursor cursor;        // 0x18
};

// A talk window the TalkMsgWinSys draws, 0x2c bytes (func_ov021_02191558).
struct MsgBGBalloonWin {
    MsgBGCursor cursor;        // 0x00
    StrBuf *strBuf;            // 0x08
    FieldMsgBG *msgBG;         // 0x0c
    TalkMsgWinSys *talkMsgWin; // 0x10
    MsgData *msgData;          // 0x14
    BmpWin *bmpWin;            // 0x18
    MsgBGStream *stream;       // 0x1c
    FieldMsgBGWin *win;        // 0x20
    u32 state;                 // 0x24  the state of its stream
    s16 scroll;                // 0x28
    u16 unk_2a;
};

// The name window of a speaker, drawn by the TalkMsgWinSys, 0x18 bytes, one of FieldMsgBG::nameWins
// (func_ov021_02191870).
struct MsgBGNameWin {
    StrBuf *strBuf;            // 0x00
    u32 index;                 // 0x04
    u32 slot;                  // 0x08  its window of the TalkMsgWinSys
    TalkMsgWinSys *talkMsgWin; // 0x0c
    u32 unk_10;
    u32 unk_14;
};

// The scrolling two-line window, 0x34 bytes (func_ov021_02191a10).
struct MsgBGScrollWin {
    FieldMsgBG *msgBG;         // 0x00
    BmpWin *bmpWin;            // 0x04
    GFLBitmap *bitmap1;        // 0x08
    GFLBitmap *bitmap2;        // 0x0c
    StrBuf *strBuf;            // 0x10
    StrBuf *lineBuf;           // 0x14
    u32 state;                 // 0x18
    s32 scrollY;               // 0x1c
    s32 scrollLine;            // 0x20
    u32 line;                  // 0x24
    MsgBGCursor cursor;        // 0x28
    Font *font;                // 0x30
};

// The sign window, 0x18 bytes (func_ov021_02192570).
struct MsgBGSignWin {
    BmpWin *bmpWin;            // 0x00
    u32 unk_4;
    FieldMsgBG *msgBG;         // 0x08
    GFLBitmap *bitmap;         // 0x0c  the tile the cursor restores
    MsgBGCursor cursor;        // 0x10
};

// One actor message window (a balloon) of a FieldMsgBG, 0x34 bytes.
struct ActorMsgWin {
    u8 unk_0;
    u8 mode;                   // 0x01
    s16 scroll;                // 0x02
    StrBuf *strBuf;            // 0x04
    TalkMsgWinSys *talkMsgWin; // 0x08
    MsgBGCursor cursor;        // 0x0c
    FieldMsgBG *msgBG;         // 0x14
    const VecFx32 *pos;        // 0x18
    VecFx32 posCopy;           // 0x1c
    u8 unk_28[0xc];
};

// A row of a list menu that func_ov021_02190a28 makes: its message, and the value the menu returns when it is
// chosen; the Union Room's menus return the rows of the menu that a row opens (pokebw2's ListMenuRow), which the
// option's value holds as an intptr_t
typedef struct ListMenuRow {
    u32 messageId;
    union {
        intptr_t value;
        const struct ListMenuRow *sub;
    };
} ListMenuRow;

// How the player advances a message: automatically, or with a held button, 4 bytes (pokebw2's MsgBGInput)
typedef struct MsgBGInput {
    u8 autoAdvance;            // 0x00
    u8 timer;                  // 0x01
    u8 held;                   // 0x02
    u8 pad;
} MsgBGInput;

// The message BG system of the field, 0x17c bytes (func_ov021_021901ec).
struct FieldMsgBG {
    HeapID heapId;             // 0x00
    u16 bgId;                  // 0x02
    u16 unk_4;                 // 0x04  the BG of the talk window
    u16 unk_6;                 // 0x06  the frame of the last window
    Font *font;                // 0x08
    PrintQueue *printQueue;    // 0x0c
    FieldMsgBGWin wins[4];     // 0x10
    MsgBGNameWin *nameWins[8]; // 0xa0
    ActorMsgWin actorWins[3];  // 0xc0
    TalkMsgWinSys *talkMsgWin; // 0x15c
    MsgBGInput input;          // 0x160  the message speed state (func_ov021_021931f4)
    TCBExManager *tcbEx;       // 0x164
    G3DCamera *camera;         // 0x168  what the talk windows' tails are projected with
    TCB *vblankTcb;            // 0x16c
    u8 resetBG2;               // 0x170  set the second BG up again before its next window (func_ov021_021904d0)
    u8 unk_171[3];
    void *cursorCharFile;      // 0x174  the NCGR of the page cursor
    NNSG2dCharacterData *cursorChar; // 0x178
};

// Creates the system with the camera the talk windows' tails are projected with, frees it, and draws the talk windows
FieldMsgBG *func_ov021_021901ec(HeapID heapId, G3DCamera *camera);
void func_ov021_021902bc(FieldMsgBG *param_1);
void func_ov021_0219047c(FieldMsgBG *param_1);
ListMenuOption *InitListMenuOptionHeap(u32 count, HeapID heapId);
// pokebw2's MsgBGPrinter_CreateMenuItemsFromTable: the options of count rows, their messages from msgData
ListMenuOption *func_ov021_02190a28(const ListMenuRow *rows, s32 count, MsgData *msgData, HeapID heapId);
ListMenuUI *func_ov021_0219097c(FieldMsgBG *param_1, ListMenuRequest *param_2, ListMenuOption *param_3);
void func_ov021_02190a14(ListMenuUI *param_1);
void func_ov021_02190b1c(ListMenuUI *param_1);
void func_ov021_02190a6c(ListMenuOption *options);
// Loads the system message box's palettes for a BG and heap (the code of func_ov021_02192fa4)
void func_ov021_021902a4(u8 bgId, u16 heapId);
MsgBGSysWin *func_ov021_021907a8(FieldMsgBG *param_1, MsgData *param_2);
void func_ov021_02190740(MsgBGSysWin *param_1, u16 param_2, u16 param_3, u32 param_4);
ListMenuUI *func_ov021_02190938(FieldMsgBG *param_1, const ListMenuRequest *param_2, ListMenuOption *param_3, u16 param_4, u16 param_5, BmpMenuListCursorCallback param_6, BmpMenuListPrintCallback param_7, void *param_8);
void func_ov021_02190764(MsgBGSysWin *param_1, u16 param_2, u16 param_3, StrBuf *param_4, u16 param_5, Font *param_6);

#endif // BWPORT_FIELD_FIELD_MSGBG_H
