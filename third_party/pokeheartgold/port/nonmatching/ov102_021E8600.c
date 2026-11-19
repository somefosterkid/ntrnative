#include "global.h"

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatSentence;
struct EasyChatWordList;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;
enum EasyChatGroup {
    GROUP_POKEMON = 0,
    GROUP_POKEMON_2,
    GROUP_MOVE,
    GROUP_MOVE_2,
    GROUP_STATUS,
    GROUP_TRAINER,
    GROUP_PEOPLE,
    GROUP_GREETINGS,
    GROUP_LIFESTYLE,
    GROUP_FEELINGS,
    GROUP_TOUGH_WORDS,
    GROUP_UNION,
    EASY_CHAT_GROUP_COUNT,
    GROUP_MODE_CANCEL_INDEX = EASY_CHAT_GROUP_COUNT,
};
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2 ];
} EasyChatSentence;
typedef struct EasyChatArgs EasyChatArgs;
typedef struct EasyChatWordList EasyChatWordList;
enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};
typedef struct TouchScreenActions TouchScreenActions;
typedef struct EasyChatApp EasyChatApp;
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
struct EasyChatApp {
    EasyChatArgs *args;
    u32 type;
    EasyChatSentence sentence;
    u16 words[2 ];
    UnkStruct_ov20_021D2128 *unk_14;
    EasyChatWordList *unk_18;
    int state;
    EasyChatMainFunc mainFunc;
    EasyChatMainFunc nextMainFunc;
    int taskFuncState;
    EasyChatTaskFunc taskFunc;
    u16 pressedKeys;
    u16 heldKeysUnused;
    u16 pressedKeysRepeatable;
    int touchedButtonIndex;
    int unk_3C;
    TouchScreenActions *buttonAction;
    BOOL unk_44;
    BOOL unk_48;
    u16 unk_4C;
    u16 currentWordSlot;
    u16 categoryCursorPos;
    u16 prevCategoryCursorPos;
    UnkStruct_ov20_021D18BC unk_54;
    UnkStruct_ov20_021D0F64 unk_60;
    u8 unk_64;
    u8 mode;
    UnkStruct_ov20_021D1F34 unk_66;
};
void EasyChat_TouchButtonCallback(u32 buttonIndex, enum TouchScreenButtonState buttonState, void *data);

BOOL ov102_021E8600(EasyChatApp *easyChatApp);

BOOL ov102_021E8600(EasyChatApp *easyChatApp)
{
    static const struct {
        u8 up;
        u8 down;
        u8 left;
        u8 right;
    } adjacentGroups[] = {
        [GROUP_POKEMON] = { .up = 0xFE , .down = GROUP_MOVE_2, .left = GROUP_MOVE, .right = GROUP_POKEMON_2 },
        [GROUP_POKEMON_2] = { .up = 0xFE , .down = GROUP_STATUS, .left = GROUP_POKEMON, .right = GROUP_MOVE },
        [GROUP_MOVE] = { .up = 0xFE , .down = GROUP_TRAINER, .left = GROUP_POKEMON_2, .right = GROUP_POKEMON },
        [GROUP_MOVE_2] = { .up = GROUP_POKEMON, .down = GROUP_PEOPLE, .left = GROUP_TRAINER, .right = GROUP_STATUS },
        [GROUP_STATUS] = { .up = GROUP_POKEMON_2, .down = GROUP_GREETINGS, .left = GROUP_MOVE_2, .right = GROUP_TRAINER },
        [GROUP_TRAINER] = { .up = GROUP_MOVE, .down = GROUP_LIFESTYLE, .left = GROUP_STATUS, .right = GROUP_MOVE_2 },
        [GROUP_PEOPLE] = { .up = GROUP_MOVE_2, .down = GROUP_FEELINGS, .left = GROUP_LIFESTYLE, .right = GROUP_GREETINGS },
        [GROUP_GREETINGS] = { .up = GROUP_STATUS, .down = GROUP_TOUGH_WORDS, .left = GROUP_PEOPLE, .right = GROUP_LIFESTYLE },
        [GROUP_LIFESTYLE] = { .up = GROUP_TRAINER, .down = GROUP_UNION, .left = GROUP_GREETINGS, .right = GROUP_PEOPLE },
        [GROUP_FEELINGS] = { .up = GROUP_PEOPLE, .down = 0xFE , .left = GROUP_UNION, .right = GROUP_TOUGH_WORDS },
        [GROUP_TOUGH_WORDS] = { .up = GROUP_GREETINGS, .down = 0xFE , .left = GROUP_FEELINGS, .right = GROUP_UNION },
        [GROUP_UNION] = { .up = GROUP_LIFESTYLE, .down = 0xFE , .left = GROUP_TOUGH_WORDS, .right = GROUP_FEELINGS },
        [GROUP_MODE_CANCEL_INDEX] = { .up = GROUP_FEELINGS, .down = GROUP_POKEMON, .left = 0xFE , .right = 0xFE },
    };
    u32 cursorPos = easyChatApp->categoryCursorPos;


    if (cursorPos == 0xFE ) {
        cursorPos = GROUP_MODE_CANCEL_INDEX;
    }

    if (easyChatApp->categoryCursorPos != 0xFE ) {
        easyChatApp->prevCategoryCursorPos = easyChatApp->categoryCursorPos;

        if ((easyChatApp->pressedKeysRepeatable & (0x0040)) ) {
            easyChatApp->categoryCursorPos = adjacentGroups[cursorPos].up;
            return 1 ;
        }

        if ((easyChatApp->pressedKeysRepeatable & (0x0080)) ) {
            easyChatApp->categoryCursorPos = adjacentGroups[cursorPos].down;
            return 1 ;
        }

        if ((easyChatApp->pressedKeysRepeatable & (0x0020)) ) {
            easyChatApp->categoryCursorPos = adjacentGroups[cursorPos].left;
            return 1 ;
        }

        if ((easyChatApp->pressedKeysRepeatable & (0x0010)) ) {
            easyChatApp->categoryCursorPos = adjacentGroups[cursorPos].right;
            return 1 ;
        }
    } else {
        if ((easyChatApp->pressedKeysRepeatable & (0x0040)) ) {
            easyChatApp->categoryCursorPos = adjacentGroups[GROUP_MODE_CANCEL_INDEX].up + (easyChatApp->prevCategoryCursorPos % 3);
            return 1 ;
        }

        if ((easyChatApp->pressedKeysRepeatable & (0x0080)) ) {
            easyChatApp->categoryCursorPos = adjacentGroups[GROUP_MODE_CANCEL_INDEX].down + (easyChatApp->prevCategoryCursorPos % 3);
            return 1 ;
        }
    }

    return 0 ;
}
