#include "global.h"
#include "mail_message.h"
#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"

s32 ov102_021E7A4C(void *, s32 *);

s32 ov102_021E8D58(void *);

s32 ov102_021E8F5C(void *, s8, s8);

s32 ov102_021E940C(void *, s32);

s32 ov102_021E9464(void *);

s32 ov102_021EA258(void *);

extern s32 ov102_021E8D1C;

void ov102_021E8E84(void *arg0, s32 *arg1);

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatArgs EasyChatArgs;
BOOL sub_02090DC0(const EasyChatArgs *args, const u16 *words, const MailMessage *sentence);
typedef struct EasyChatWordList EasyChatWordList;
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
    MailMessage sentence;
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
enum EasyChatType {
    EASY_CHAT_TYPE_ONE_WORD = 0,
    EASY_CHAT_TYPE_TWO_WORDS,
    EASY_CHAT_TYPE_SENTENCE,
};
BOOL ov102_021E8EF8(EasyChatApp *easyChatApp);
BOOL ov102_021E8F14(EasyChatApp *easyChatApp);

void ov102_021E8E84(void *arg0, s32 *arg1) {
    s32 v0;
    s32 v1;

    v0 = *arg1;
    switch (v0) {
    case 0:
        ov102_021E8F5C(arg0 + 0x70, 1, 1);
        ov102_021E940C(*(void **)((u8 *)arg0 + 0x14), 0x16);
        *arg1 = 1;
        return;
    case 1:
        v1 = ov102_021EA258(*(void **)((u8 *)arg0 + 0x14));
        switch (v1) {
        case 0:
            ov102_021E940C(*(void **)((u8 *)arg0 + 0x14), 0x19);
            *arg1 = 2;
            return;
        case 1:
            ov102_021E7A4C(arg0, &ov102_021E8D1C);
            *arg1 = 2;
            return;
        }
        break;
    case 2:
        if (ov102_021E9464(*(void **)((u8 *)arg0 + 0x14)) != 0) {
            ov102_021E8D58(arg0);
        }
        return;
    }
}

BOOL ov102_021E8EF8(EasyChatApp *easyChatApp) {
    return !sub_02090DC0(easyChatApp->args, easyChatApp->words, &easyChatApp->sentence);
}

BOOL ov102_021E8F14(EasyChatApp *easyChatApp) {
    switch (easyChatApp->type) {
    case EASY_CHAT_TYPE_ONE_WORD:
        return easyChatApp->words[0] != 0xFFFF;
    case EASY_CHAT_TYPE_TWO_WORDS:
        return easyChatApp->words[0] != 0xFFFF && easyChatApp->words[1] != 0xFFFF;
    case EASY_CHAT_TYPE_SENTENCE:
        return MailMsg_AllFieldsAreInit(&easyChatApp->sentence);
    }

    return 0;
}
