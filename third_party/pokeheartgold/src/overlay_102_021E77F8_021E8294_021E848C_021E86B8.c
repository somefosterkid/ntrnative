#include "global.h"

#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatWordList;

struct TouchHitboxController;

struct UnkStruct_ov20_021D2128_t;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;

typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;

int ov102_021EC4F8(u32 index);

int ov102_021EC514(u32 index);

int ov102_021EC530(u32 index);

int ov102_021EC54C(u32 index);

int ov102_021EC568(u32 index);

int ov102_021EC584(u32 index);

BOOL ov102_021E86B8(EasyChatApp *easyChatApp);

BOOL ov102_021E86B8(EasyChatApp *easyChatApp) {
    if (easyChatApp->categoryCursorPos != 0xFE) {
        int cursorPos = 0xFF;

        if (easyChatApp->pressedKeysRepeatable & (0x0040)) {
            cursorPos = ov102_021EC4F8(easyChatApp->categoryCursorPos);
        } else if (easyChatApp->pressedKeysRepeatable & (0x0080)) {
            cursorPos = ov102_021EC514(easyChatApp->categoryCursorPos);
        } else if (easyChatApp->pressedKeysRepeatable & (0x0010)) {
            cursorPos = ov102_021EC54C(easyChatApp->categoryCursorPos);
        } else if (easyChatApp->pressedKeysRepeatable & (0x0020)) {
            cursorPos = ov102_021EC530(easyChatApp->categoryCursorPos);
        }

        if (cursorPos != 0xFF) {
            easyChatApp->prevCategoryCursorPos = easyChatApp->categoryCursorPos;
            easyChatApp->categoryCursorPos = cursorPos;
            return 1;
        }
    } else {
        if (easyChatApp->pressedKeysRepeatable & (0x0040)) {
            easyChatApp->categoryCursorPos = ov102_021EC584(easyChatApp->prevCategoryCursorPos);
            return 1;
        }

        if (easyChatApp->pressedKeysRepeatable & (0x0080)) {
            easyChatApp->categoryCursorPos = ov102_021EC568(easyChatApp->prevCategoryCursorPos);
            return 1;
        }
    }

    return 0;
}
