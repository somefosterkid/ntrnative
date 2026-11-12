// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov259/local_tvt_callselect.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/local_tvt_local.h"
#include "constants/sound.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/save_control.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/wordset.h"

// The list of the persons the player can call on the Xtransceiver's lower screen, named after the ROM's
// "local_tvt_callselect.c": the plates of the persons, of which the player picks one, the scroll bar of a list
// longer than the screen, and the button that makes the call. Every name here is ours

// The plates that fit on the screen
#define LIST_VISIBLE 5
#define ITEM_HEIGHT 32
// How far the scroll bar's knob moves, and how fast
#define SCROLL_BAR_RANGE 120
#define SCROLL_BAR_SPEED 8

// The sequences of the list's actors
enum {
    LIST_ANIM_PLATE,
    LIST_ANIM_ICON,
    LIST_ANIM_ICON_SELECTED,
    LIST_ANIM_ICON_DARK,
    LIST_ANIM_SCROLL_BAR,
    LIST_ANIM_SCROLL_BAR_HELD,
    LIST_ANIM_PLATE_DECIDE,
    LIST_ANIM_PLATE_CURSOR,
};

// The steps of LocalTvtCallSelect_Main
enum {
    SEQ_SELECT,
    SEQ_CALL,
    SEQ_EXIT,
    SEQ_END,
};

// A person of the list
typedef struct {
    BOOL selected;
    BOOL cursor;
    // Dark while another person is selected
    BOOL bright;
    BOOL disabled;
    ClActor *plate;
    ClActor *icon;
    u32 unk18;
    BmpWin *window;
    int index;
} LocalTvtCallItem;

typedef struct {
    // Only a list longer than the screen has it
    BOOL enabled;
    BOOL held;
    BOOL moved;
    // The knob moves to the target on its own
    BOOL scrolling;
    ClActor *actor;
    int pos;
    int target;
} LocalTvtCallScrollBar;

// The bottom of the screen: a message while nobody is selected, then the button that makes the call
typedef struct {
    BmpWin *window;
    AppTaskMenuWin *button;
    AppTaskMenuRes *res;
    LocalTvtMessage *message;
    StrBuf *callStr;
    StrBuf *promptStr;
    u16 heapId;
    BOOL shown;
    BOOL active;
} LocalTvtCallButton;

typedef struct {
    u16 scroll;
    u16 count;
    // The button's when it is the count
    int cursor;
    BOOL changed;
    LocalTvtCallItem items[LOCAL_TVT_SELECT_MAX];
    LocalTvtCallScrollBar scrollBar;
    LocalTvtCallButton button;
    // The next key moves the cursor to the button
    BOOL cursorToButton;
} LocalTvtCallList;

struct LocalTvtCallSelect {
    u32 palette;
    u32 chars;
    u32 cellAnims;
    int seq;
    u32 result;
    LocalTvtCallSelectSetup setup;
    LocalTvtCallList list;
    LocalTvtReturnBar *returnBar;
    LocalTvtKeyTouch *keyTouch;
};

BOOL func_ov021_021a0380(LocalTvtCallButton *button);

// pokebw2's LocalTvtCallButton_IsShown
BOOL func_ov021_021a0380(LocalTvtCallButton *button) {
    if (button->button != NULL) {
        return TRUE;
    }
    return FALSE;
}

