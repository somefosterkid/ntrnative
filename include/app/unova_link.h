// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/unova_link.h: the lines in use so far.
#ifndef BWPORT_APP_UNOVA_LINK_H
#define BWPORT_APP_UNOVA_LINK_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "system/printsys.h"
#include "struct_decls.h"

typedef struct KeySystemSeq KeySystemSeq;
typedef struct KeySystemScene KeySystemScene;
typedef struct KeySystemMsgWin KeySystemMsgWin;
typedef struct KeySystemMenu KeySystemMenu;
typedef struct KeySystemList KeySystemList;
typedef struct KeySystemMsgWinGroup KeySystemMsgWinGroup;
typedef struct KeySystemOamText KeySystemOamText;
typedef struct KeySystemScrollList KeySystemScrollList;
typedef struct KeySystemParticle KeySystemParticle;

// Black 2's, not checked against Black
// A step of the sequence, called each frame with its own state
typedef void (*KeySystemSeqFunc)(KeySystemSeq *seq, int *state, void *work);

// Black 2's, not checked against Black
// A scene: a screen that is set up, runs until main returns TRUE, and is torn down
typedef struct {
    void (*init)(void *work, HeapID heapId);
    BOOL (*main)(void *work);
    void (*exit)(void *work);
    void (*free)(void *work);
} KeySystemSceneFuncs;

// Black 2's, not checked against Black
// A list of windows to choose from, as KeySystemList_Create creates it
typedef struct {
    u32 msgId;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} KeySystemListItem;

// Black 2's, not checked against Black
#define KEY_SYSTEM_LIST_MAX 4

// Black 2's, not checked against Black
typedef struct {
    u32 bg;
    u32 unk04;
    u32 frameChar;
    u32 palette;
    MsgData *msgData;
    Font *font;
    u32 count;
    u32 cursor;
    KeySystemListItem items[KEY_SYSTEM_LIST_MAX];
    // Called with the index of an item, when it is chosen
    int (*select)(int index, void *arg);
    void *arg;
} KeySystemListSetup;

// Black 2's, not checked against Black
// A menu of choices, as KeySystemMenu_Create creates it
typedef struct {
    MsgData *msgData;
    Font *font;
    PrintQueue *printQueue;
    u32 msgIds[4];
    u32 count;
    u16 bg;
    u16 palette;
    u16 framePalette;
    u16 frameChar;
    // Whether B cancels
    BOOL cancelable;
    // What B chooses, and where the cursor starts
    u16 cancelValue;
    u16 cursor;
} KeySystemMenuSetup;

// Black 2's, not checked against Black
// A position in pixels
typedef struct {
    s32 x;
    s32 y;
} KeySystemPos;

// Black 2's, not checked against Black
// A straight move over a number of frames
typedef struct {
    KeySystemPos pos;
    KeySystemPos start;
    KeySystemPos end;
    fx32 stepX;
    fx32 stepY;
    int frame;
    int frames;
} KeySystemTween;

// Black 2's, not checked against Black
// A move in 3D that speeds up
typedef struct {
    VecFx32 pos;
    VecFx32 start;
    VecFx32 end;
    VecFx32 dir;
    fx32 speed;
    fx32 accel;
    int frame;
    int frames;
} KeySystemAccelMove;

// Black 2's, not checked against Black
// key_system_util.c
// How a KeySystemMsgWin prints: all at once through the print queue, or a character at a time, with the cursor and at
// the text speed, at the fast speed or without the cursor
enum {
    KEY_SYSTEM_MSG_PRINT,
    KEY_SYSTEM_MSG_STREAM,
    KEY_SYSTEM_MSG_PRINT_WAIT_ICON,
    KEY_SYSTEM_MSG_STREAM_FAST,
    // Streams without a cursor, and only finishes once KeySystemMsgWin_CreateCursor adds one
    KEY_SYSTEM_MSG_STREAM_NO_CURSOR,
    KEY_SYSTEM_MSG_IDLE,
};

// Black 2's, not checked against Black
// Where KeySystemMsgWin_SetPos puts the text in its window
enum {
    KEY_SYSTEM_ALIGN_TOP_LEFT,
    KEY_SYSTEM_ALIGN_CENTER,
    KEY_SYSTEM_ALIGN_CENTER_Y,
    KEY_SYSTEM_ALIGN_RIGHT,
};

// Black 2's, not checked against Black
// A window of KeySystemMsgWinGroup_Create
typedef struct {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u32 msgId;
} KeySystemMsgWinTemplate;

// Black 2's, not checked against Black
#define KEY_SYSTEM_SCROLL_LIST_MAX 12

// Black 2's, not checked against Black
typedef struct {
    MsgData *msgData;
    Font *font;
    ClActor *arrowDown;
    ClActor *arrowUp;
    u32 msgIds[KEY_SYSTEM_SCROLL_LIST_MAX];
    u32 values[KEY_SYSTEM_SCROLL_LIST_MAX];
    ClActor *icons[KEY_SYSTEM_SCROLL_LIST_MAX];
    u32 count;
    u16 bg;
    u16 palette;
    u16 unkA8;
    u16 cursor;
    u16 unkAC;
} KeySystemScrollListSetup;

struct UnovaLinkParam {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u32 unk4;
};

// Black 2's, not checked against Black
// Started by the game clear event, or from the start menu without a parameter
#define UNOVA_LINK_MODE_GAME_CLEAR 0

// Black 2's, not checked against Black
// The keys: the difficulty keys, the key that swaps Black City and White Forest, and the keys of the chambers
enum {
    KEY_SYSTEM_KEY_EASY,
    KEY_SYSTEM_KEY_CHALLENGE,
    KEY_SYSTEM_KEY_CITY,
    KEY_SYSTEM_KEY_IRON,
    KEY_SYSTEM_KEY_ICEBERG,
    KEY_SYSTEM_KEY_COUNT,
};

#endif // BWPORT_APP_UNOVA_LINK_H
