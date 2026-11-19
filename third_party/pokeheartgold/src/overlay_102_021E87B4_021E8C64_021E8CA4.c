#include "global.h"

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatSentence;

struct EasyChatWordList;

struct TouchScreenActions;

struct UnkStruct_ov20_021D2128_t;

enum EasyChatType {
    EASY_CHAT_TYPE_ONE_WORD = 0,
    EASY_CHAT_TYPE_TWO_WORDS,
    EASY_CHAT_TYPE_SENTENCE,
};

enum EasyChatMode {
    GROUP_MODE = 0,
    ABC_MODE,
};

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2];
} EasyChatSentence;

void MailMsg_SetFieldI(EasyChatSentence *sentence, u32 index, u16 word);

typedef struct EasyChatArgs EasyChatArgs;

typedef struct EasyChatWordList EasyChatWordList;

u16 sub_02091C60(const EasyChatWordList *wordList, u32 group, u32 index);

u16 sub_02091CAC(const EasyChatWordList *wordList, u32 letter, u32 index);

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
    u16 words[2];
    UnkStruct_ov20_021D2128 *unk_14;
    EasyChatWordList *unk_18;
    int state;
    void *mainFunc;
    void *nextMainFunc;
    int taskFuncState;
    void *taskFunc;
    u16 pressedKeys;
    u16 heldKeysUnused;
    u16 pressedKeysRepeatable;
    u8 pad_36[2];
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
    u8 pad_65[6];
    u8 mode;
    UnkStruct_ov20_021D1F34 unk_66;
    u8 pad_6E[6];
    u32 unk_74;
};

BOOL ov102_021E8F14(EasyChatApp *easyChatApp);

void EasyChat_TouchButtonCallback(u32 buttonIndex, enum TouchScreenButtonState buttonState, void *data);

u32 ov102_021E8798(const UnkStruct_ov20_021D18BC *param0);

BOOL ov102_021E8CA4(EasyChatApp *easyChatApp);

BOOL ov102_021E8CA4(EasyChatApp *easyChatApp) {
    u32 v0 = *(u16 *)&easyChatApp->unk_60.unk_02;
    u16 v1;
    if (v0 == 0xFFFF) {
        v0 = ov102_021E8798(&easyChatApp->unk_54);
    }
    if (easyChatApp->mode == GROUP_MODE) {
        v1 = sub_02091C60(easyChatApp->unk_18, easyChatApp->categoryCursorPos, v0);
    } else {
        v1 = sub_02091CAC(easyChatApp->unk_18, easyChatApp->categoryCursorPos, v0);
    }
    switch (easyChatApp->type) {
    case EASY_CHAT_TYPE_ONE_WORD:
        easyChatApp->words[0] = v1;
        break;

    case EASY_CHAT_TYPE_TWO_WORDS:
        easyChatApp->words[easyChatApp->currentWordSlot] = v1;
        break;

    case EASY_CHAT_TYPE_SENTENCE:
        MailMsg_SetFieldI(&easyChatApp->sentence, easyChatApp->currentWordSlot, v1);
        break;
    }

    return ov102_021E8F14(easyChatApp);
}
