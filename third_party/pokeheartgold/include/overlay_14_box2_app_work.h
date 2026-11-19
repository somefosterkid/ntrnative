#ifndef POKEHEARTGOLD_OVERLAY_14_BOX2_APP_WORK_H
#define POKEHEARTGOLD_OVERLAY_14_BOX2_APP_WORK_H

// Box2AppWork: merged by tools/hg/promote.py from 4 m2c local copies (2 variants, base
// src/overlay_14_021EB290_021EB290.c) and 38 accesses; fields at every accessed offset, pads elsewhere.

struct AppTaskMenu;
struct AppTaskMenuRes;
struct ArcTool;
struct BGWinFrame;
struct BmpWin;
struct ClActUnit;
struct ClActor;
struct CursorMove;
struct Font;
struct GFLBitmap;
struct MsgData;
struct PaletteFade;
struct PrintQueue;
struct StrBuf;
struct TCB;
struct WordSet;

typedef struct Box2AppWork {
    void *bmpOam;
    struct { void *oam; struct GFLBitmap *bitmap; } fontOam[9];
    u32 keyRepeatWait;
    u32 keyRepeatStart;
    struct TCB *vtask;
    struct { void *func; void *freq; void *work; u16 seq; u16 cnt; } vfunk;
    u32 unk68;
    struct PaletteFade *palFade;
    struct CursorMove *cursorMove;
    struct BGWinFrame *bgWinFrame;
    struct Font *font;
    struct Font *smallFont;
    struct MsgData *msgData;
    struct WordSet *wordSet;
    struct StrBuf *expandBuf;
    struct PrintQueue *printQueue;
    u8 unk90[4];
    struct { struct BmpWin *window; u8 flushPending; u8 unk_5_tail[0x3]; } windows[28];
    u8 flushChar[4];
    u32 cursorChars;
    BOOL nationalDex;
    u16 *regionalDex;
    struct { struct StrBuf *str; u16 color; u8 unk_8_pad[0x2]; u32 type; } yesNoItems[2];
    struct AppTaskMenuRes *yesNoRes;
    struct AppTaskMenu *yesNoMenu;
    u16 ynID;
    struct { u8 mode : 1; u8 id : 7; u8 pal1 : 4; u8 pal2 : 4; u8 seq; u8 cnt; u8 px; u8 py; u8 sx; u8 sy; } bawk;
    u8 pad_1AE[0x2];
    struct ArcTool *pokeIconArc;
    u8 pokeIconChar[512][30];
    u8 unk3DB4[30];
    u8 pokeIconId[37];
    u8 unk3DF7;
    u32 unk3DF8[30];
    u8 trayIconChar[1024][3];
    u8 pad_4A70[0x3e58];
    u16 unk_88C8;
    u16 unk_88CA;
    u8 pad_88CC[0x1550];
    struct ClActUnit *unk_9E1C;
    u8 pad_9E20[0x50];
    struct ClActUnit *unk9E70;
    struct ClActor *unk9E74[96];
    u32 unk9FF4[69];
    u32 unkA108[11];
    u32 unkA134[12];
    u16 unkA164;
    u8 unkA166[2];
    u32 curPos;
    u16 itemId;
    u16 unkA16E;
    u8 unkA170[2];
    u8 unkA172;
    u8 moveErr;
    u32 unkA174;
    s8 wallPx;
    u8 wallArea;
    u8 unkA17A[2];
    u8 unkA17C;
    u8 unkA17D;
    u8 pokePutKey;
    u8 unkA17F;
    u32 tpx;
    u32 tpy;
    u32 oldCurPos;
    u32 unkA18C;
    void *unkA190;
    u32 unkA194;
    u32 unkA198;
} Box2AppWork;

// sizeof(Box2AppWork) == 0xa19c

#endif // POKEHEARTGOLD_OVERLAY_14_BOX2_APP_WORK_H
