#include "global.h"
#include "pm_string.h"
#include "bg_window.h"
#include "msgdata.h"

struct BgConfig;

struct MsgData;

struct String;

struct MessageFormat;

struct Window;

void String_Delete(String *string);

void FillWindowPixelBuffer(Window *window, u8 val);


enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

typedef u32 TextColor;

void ov70_0223F1D8(Window *window, MsgData *speciesMessageLoader, int messageId, int centered, int y, TextColor textColor);

void ov70_0223F2BC(Window *window, MsgData *gtsMessageLoader, int gender, int param3, int y, int x, TextColor fallback);

void ov70_0223F370(Window *window, MsgData *gtsMessageLoader, int messageIndex, int centered, int y, TextColor textColor, BOOL isRange);

void ov70_02245084(Window *window, String *string, int x, int y, int centered, TextColor textColor);

extern void ov70_0223F3D8(MsgData *gtsMessageLoader, MsgData *speciesMessageLoader, MessageFormat *unused, Window windows[], int species, int gender, int levelRange);

void ov70_0223F3D8(MsgData *gtsMessageLoader, MsgData *speciesMessageLoader, MessageFormat *unused, Window windows[], int species, int gender, int levelRange) {
    String *string = NewString_ReadMsgData(gtsMessageLoader, 101);
    ov70_02245084(&windows[0], string, 0, 0, 0, ((TextColor)(((15 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));

    for (int i = 1; i < 3; i++) {
        FillWindowPixelBuffer(&windows[i], 0x0);
    }

    ov70_0223F1D8(&windows[1], speciesMessageLoader, species, 0, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));

    if ((gender == GENDER_MALE + 1) || (gender == GENDER_FEMALE + 1)) {
        ov70_0223F2BC(&windows[1], gtsMessageLoader, gender, 0, 0, 70, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    }

    ov70_0223F370(&windows[2], gtsMessageLoader, levelRange, 2, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0);
    String_Delete(string);
}
