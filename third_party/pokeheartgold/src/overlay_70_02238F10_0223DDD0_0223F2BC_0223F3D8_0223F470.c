#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "msgdata.h"
#include "pm_string.h"
#include "wild_encounter.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


typedef u32 TextColor;

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

void ov70_0223F2BC(Window *window, MsgData *gtsMessageLoader, int gender, int showIfEither, int y, int x, TextColor fallback);

void ov70_0223F1D8(Window *, MsgData *, s32, u32, u32, u32);

void ov70_0223F370(Window *, MsgData *, s32, u32, u32, u32, u32);

void ov70_02245084(Window *, String *, u32, u32, u32, u32);

void ov70_0223F470(MsgData *gtsMessageLoader, MsgData *speciesMessageLoader, MessageFormat *unused, Window *windows, s32 species, s32 gender, s32 levelRange);

void ov70_0223F470(MsgData *gtsMessageLoader, MsgData *speciesMessageLoader, MessageFormat *unused, Window *windows, s32 species, s32 gender, s32 levelRange) {
    String *string;
    s32 iVar1;
    Window *window;

    string = NewString_ReadMsgData((MsgData *)gtsMessageLoader, 0x65);
    ov70_02245084(windows, string, 0, 0, 0, 0xf0200);
    for (iVar1 = 1, window = windows + 1; iVar1 < 3; iVar1 = iVar1 + 1) {
        FillWindowPixelBuffer(window, 0);
        window = window + 1;
    }
    ov70_0223F1D8(windows + 1, speciesMessageLoader, species, 0, 0, 0x10200);
    if (gender - 1U <= 1) {
        ov70_0223F2BC(windows + 1, gtsMessageLoader, gender, 0, 0, 0x46, 0x10200);
    }
    ov70_0223F370(windows + 2, gtsMessageLoader, levelRange, 0, 0, 0x10200, 0);
    String_Delete(string);
    return;
}
