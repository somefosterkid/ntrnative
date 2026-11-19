#include "global.h"
#include "msgdata.h"

struct ArcTool;

struct BlinkPalAnm;

struct BmpWin;

struct ClActRenderer;

struct ClActUnit;

struct ClActor;

struct Font;

struct MsgData;

struct PrintQueue;

struct PrintStream;

struct StrBuf;

struct TCBExManager;

struct MessageFormat;

typedef struct ArcTool ArcTool;

typedef struct BmpWin BmpWin;

typedef struct Font Font;

typedef struct PrintStream PrintStream;

typedef struct StrBuf StrBuf;

typedef struct TCBExManager TCBExManager;

typedef struct MessageFormat WordSet;

typedef struct BlinkPalAnm BlinkPalAnm;

typedef struct PrintQueue PrintQueue;

typedef struct {
    BmpWin *window;
    u8 flushPending;
} PrintWindow;

typedef struct ClActUnit ClActUnit;

typedef struct ClActor ClActor;

typedef struct ClActRenderer ClActRenderer;

struct BlinkPalAnm {

    u16 *unk_00;
    u16 *unk_04;

    u16 *unk_08;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u16 unk_12;
    u32 unk_14;
    u16 unk_18;
    u8 unk_1A[0x2];
};


typedef struct {
    void *buffer[2];
    NNSG2dScreenData *screen[2];
} WifiNoteScreens;

typedef struct {
    u8 pad_0[80];
    WordSet *wordSet;
    MsgData *msgData;
    ArcTool *arc;
    PrintWindow title;
    StrBuf *titleStr;
    StrBuf *titleTemp;
    StrBuf *str;
    StrBuf *temp;
    u32 unk24;
    TCBExManager *tcbManager;
    PrintStream *stream;
    BOOL streamPaused;
    PrintQueue *queue;
    PrintWindow *windows[64];
    ClActUnit *unit;
    ClActUnit *scrollUnit;
    ClActRenderer *renderer;
    u32 chars[2];
    u32 palettes[2];
    u32 cellAnims[2];
    ClActor *arrows[2];
    ClActor *returnButton;
    ClActor *exitButton;
    WifiNoteScreens screens;
    Font *font;
    u16 heapId;
    BlinkPalAnm *blink[2];
} WifiNoteDraw;

void ov43_0222AAA4(WifiNoteDraw *draw, u64 code);

void ov43_0222AAA4(WifiNoteDraw *draw, u64 code) {
    BufferIntegerAsString(draw->wordSet, 1, code / 100000000, 4, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(draw->wordSet, 2, (code / 10000) % 10000, 4, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferIntegerAsString(draw->wordSet, 3, code % 10000, 4, PRINTING_MODE_LEADING_ZEROS, 1);
}
