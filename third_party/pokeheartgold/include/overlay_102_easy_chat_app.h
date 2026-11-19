#ifndef POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_APP_H
#define POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_APP_H

// EasyChatApp: merged by tools/hg/promote.py from 50 m2c local copies (3 variants, base
// src/overlay_102_021E87B4_021E8C64_021E8CA4.c) and 59 accesses; fields at every accessed offset, pads elsewhere.

struct EasyChatArgs;
struct EasyChatWordList;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;

typedef struct EasyChatApp {
    struct EasyChatArgs *args;
    u32 type;
    struct { u16 type; u16 id; u16 words[2]; } sentence;
    u16 words[2];
    struct UnkStruct_ov20_021D2128_t *unk_14;
    struct EasyChatWordList *unk_18;
    int state;
    void *mainFunc;
    void *nextMainFunc;
    int taskFuncState;
    void *taskFunc;
    u16 pressedKeys;
    u16 heldKeysUnused;
    u16 pressedKeysRepeatable;
    u8 pad_36[0x2];
    int touchedButtonIndex;
    int unk_3C;
    struct TouchScreenActions *buttonAction;
    BOOL unk_44;
    BOOL unk_48;
    u16 unk_4C;
    u16 currentWordSlot;
    u16 categoryCursorPos;
    u16 prevCategoryCursorPos;
    struct { u16 unk_00; u16 unk_02; u16 unk_04; u8 unk_06; u8 unk_07; int unk_08; } unk_54;
    struct { s16 unk_00; s8 unk_02; s8 unk_03; } unk_60;
    u8 unk_64;
    u8 pad_65[6];
    u8 mode;
    struct { u8 unk_00; u8 unk_01; } unk_66;
    u8 pad_6E[0x6];
    u32 unk_74;
} EasyChatApp;

// sizeof(EasyChatApp) == 0x78

#endif // POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_APP_H
