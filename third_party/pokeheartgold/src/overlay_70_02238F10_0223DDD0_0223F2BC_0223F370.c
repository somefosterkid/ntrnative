#include "global.h"
#include "bg_window.h"
#include "msgdata.h"

struct MsgData;

struct Window;

typedef u32 TextColor;

void ov70_0223F38C(Window *window, MsgData *gtsMessageLoader, int messageIndex, int centered, int x, int y, TextColor textColor, BOOL isRange);

void ov70_0223F370(Window *window, MsgData *gtsMessageLoader, int messageIndex, int centered, int y, TextColor textColor, BOOL isRange);

void ov70_0223F370(Window *window, MsgData *gtsMessageLoader, int messageIndex, int centered, int y, TextColor textColor, BOOL isRange) {
    ov70_0223F38C(window, gtsMessageLoader, messageIndex, centered, 0, y, textColor, isRange);
}
