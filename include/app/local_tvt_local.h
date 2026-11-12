// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/local_tvt_local.h: the lines in use so
// far.
#ifndef BWPORT_APP_LOCAL_TVT_LOCAL_H
#define BWPORT_APP_LOCAL_TVT_LOCAL_H

#include "types.h"
#include "gfl/clact.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// The communication Xtransceiver's archive, which has the frame, the cursors and the lower screen
#define ARC_LOCAL_TVT_COMMON 170

typedef struct LocalTvtMessage LocalTvtMessage;
typedef struct LocalTvtCallSelect LocalTvtCallSelect;

// Black 2's, not checked against Black
// How LocalTvtMessage_SetPosition places the text
enum {
    LOCAL_TVT_ALIGN_LEFT,
    LOCAL_TVT_ALIGN_CENTER,
};

// Black 2's, not checked against Black
#define LOCAL_TVT_SELECT_MAX 6

// Black 2's, not checked against Black
// The results of the list
enum {
    LOCAL_TVT_SELECT_NONE,
    LOCAL_TVT_SELECT_CALL,
    LOCAL_TVT_SELECT_EXIT,
};

// Black 2's, not checked against Black
typedef struct {
    RivalDataSave *rival;
    LocalTvtMessage *message;
    ClActUnit *unit;
    MsgData *msgData;
    // The messages of the names
    u32 names[LOCAL_TVT_SELECT_MAX];
    BOOL enabled[LOCAL_TVT_SELECT_MAX];
    int count;
} LocalTvtCallSelectSetup;

// The touch bar of the lower screen with its return button, which the B button presses as well
typedef struct LocalTvtReturnBar LocalTvtReturnBar;

// The input of a list by the keys or the touch screen, whichever the player used last. The cursor shows only with
// the keys
typedef struct LocalTvtKeyTouch LocalTvtKeyTouch;

// Black 2's, not checked against Black
// What the callbacks of the input return
enum {
    LOCAL_TVT_INPUT_NONE,
    // The cursor moved, or the touch only picked an entry
    LOCAL_TVT_INPUT_MOVE,
    LOCAL_TVT_INPUT_DECIDE,
};

// Black 2's, not checked against Black
typedef struct {
    void *work;
    int (*touch)(void *work);
    int (*key)(void *work);
    // Draws the list with or without its cursor
    void (*draw)(void *work, BOOL touchMode);
} LocalTvtKeyTouchSetup;

#endif // BWPORT_APP_LOCAL_TVT_LOCAL_H
