#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "msgdata.h"

struct MsgData;


enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

typedef u32 TextColor;

extern const int ov70_02245910[];

void ov70_02245084(Window *window, String *string, int x, int y, int centered, TextColor textColor);

TextColor ov70_0223F2A0(int index, TextColor fallback);

void ov70_0223F2BC(Window *window, MsgData *gtsMessageLoader, int gender, int showIfEither, int y, int x, TextColor fallback);

void ov70_0223F2BC(Window *window, MsgData *gtsMessageLoader, int gender, int showIfEither, int y, int x, TextColor fallback) {
    if ((showIfEither == 0) && (gender == GENDER_NONE + 1)) {
        return;
    }

    String *string = NewString_ReadMsgData(gtsMessageLoader, ov70_02245910[gender]);

    if (x > 3) {
        ov70_02245084(window, string, x, y, 0, ov70_0223F2A0(gender, fallback));
    } else {
        ov70_02245084(window, string, 0, y, x, ov70_0223F2A0(gender, fallback));
    }

    String_Delete(string);
}
