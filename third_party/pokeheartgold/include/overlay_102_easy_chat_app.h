#ifndef POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_APP_H
#define POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_APP_H

// EasyChatApp: merged by tools/hg/promote.py from 50 m2c local copies (3 variants, base
// src/overlay_102_021E87B4_021E8C64_021E8CA4.c) and 59 accesses; fields at every accessed offset, pads elsewhere.

#include "mail_message_types.h"

struct EasyChatArgs;
struct EasyChatWordList;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;

typedef struct { u16 unk_00; u16 unk_02; u16 unk_04; u8 unk_06; u8 unk_07; int unk_08; } UnkStruct_ov20_021D18BC;
typedef struct { s16 unk_00; s8 unk_02; s8 unk_03; } UnkStruct_ov20_021D0F64;
typedef struct { u8 unk_00; u8 unk_01; } UnkStruct_ov20_021D1F34;
typedef struct { s8 unk_00; s8 unk_01; } UnkStruct_ov20_021D1F34_b;

struct EasyChatApp;
typedef int (*EasyChatMainFunc)(struct EasyChatApp *easyChatApp, int *state);

typedef struct EasyChatApp {
    struct EasyChatArgs *args;
    u32 type;
    MailMessage sentence;
    u16 words[2];
    struct UnkStruct_ov20_021D2128_t *unk_14;
    struct EasyChatWordList *unk_18;
    int state;
    EasyChatMainFunc mainFunc;
    EasyChatMainFunc nextMainFunc;
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
    UnkStruct_ov20_021D18BC unk_54;
    UnkStruct_ov20_021D0F64 unk_60;
    UnkStruct_ov20_021D0F64 unk_64;
    u8 pad_68[2];
    u8 unk_6A;
    u8 mode;
    UnkStruct_ov20_021D1F34 unk_66;
    s16 unk_6E;
    UnkStruct_ov20_021D1F34_b unk_70;
    u8 pad_72[2];
    u32 unk_74;
} EasyChatApp;

// sizeof(EasyChatApp) == 0x78

void ov102_021E7A4C(EasyChatApp *easyChatApp, EasyChatMainFunc mainFunc);
int ov102_021E7AA4(EasyChatApp *easyChatApp, int *state);
int ov102_021E8240(EasyChatApp *easyChatApp, int *state);
int ov102_021E8D1C(EasyChatApp *easyChatApp, int *state);

#endif // POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_APP_H
