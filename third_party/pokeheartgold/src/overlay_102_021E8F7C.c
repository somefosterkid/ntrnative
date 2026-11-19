#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"

u8 ov102_021E8F7C(void *arg0);

struct CounterData;
struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct PoketchAnimation_AnimatedSpriteData;
struct PoketchAnimation_AnimationManager;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;
typedef struct PoketchAnimation_AnimationManager PoketchAnimation_AnimationManager;
typedef struct PoketchAnimation_AnimatedSpriteData PoketchAnimation_AnimatedSpriteData;
typedef struct PoketchAnimation_SpriteData {
    void *compressedSprite;
    void *compressedAnim;
    NNSG2dCellDataBank *cell;
    NNSG2dAnimBankData *anim;
    enum HeapID heapID;
} PoketchAnimation_SpriteData;
u16 MailMsg_GetMsgBank(u32 *activeList, u32 taskId);
typedef struct CounterData CounterData;
typedef struct CounterGraphics {
    const CounterData *counterData;
    BgConfig *bgConfig;
    u32 activeTasks[2 + 4];
    PoketchAnimation_AnimationManager *animMan;
    PoketchAnimation_AnimatedSpriteData *buttonAnimation;
    PoketchAnimation_AnimatedSpriteData *digitsAnimation[4];
    PoketchAnimation_SpriteData buttonSprites;
    PoketchAnimation_SpriteData digitSprites;
} CounterGraphics;
enum CounterGraphicsTask {
    COUNTER_GRAPHICS_INIT = 0,
    COUNTER_GRAPHICS_FREE,
    COUNTER_GRAPHICS_UPDATE_BUTTON,
    COUNTER_GRAPHICS_UPDATE_VALUE,
};
enum EasyChatType {
    EASY_CHAT_TYPE_ONE_WORD = 0,
    EASY_CHAT_TYPE_TWO_WORDS,
    EASY_CHAT_TYPE_SENTENCE,
};
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
u16 MailMsg_GetFieldI(const MailMessage *sentence, int slot);
typedef struct EasyChatArgs EasyChatArgs;
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
void *MailMsg_GetRawString(u32 *activeList, u32 taskId);
BOOL ov102_021E8F84(CounterGraphics *graphics, enum CounterGraphicsTask taskID);
u16 ov102_021E8F90(const EasyChatApp *easyChatApp, int slot);
void *ov102_021E8FA8(CounterGraphics *graphics, enum CounterGraphicsTask taskID);

s32 ov102_021E7A0C(void *);
void ov102_021E8FB4(u8 *arg0);
u16 ov102_021E8FC0(void *arg0);
u16 ov102_021E8FC8(void *arg0);
u16 ov102_021E8FD0(void *arg0);

u8 ov102_021E8F7C(void *arg0) {
    return *(u8 *)((u8 *)arg0 + 0x6B);
}

BOOL ov102_021E8F84(CounterGraphics *graphics, enum CounterGraphicsTask taskID) {
    return MailMsg_GetMsgBank(graphics->activeTasks, taskID);
}

u16 ov102_021E8F90(const EasyChatApp *easyChatApp, int slot) {
    if (easyChatApp->type == EASY_CHAT_TYPE_SENTENCE) {
        return MailMsg_GetFieldI(&easyChatApp->sentence, slot);
    } else {
        return easyChatApp->words[slot];
    }
}

void *ov102_021E8FA8(CounterGraphics *graphics, enum CounterGraphicsTask taskID) {
    return MailMsg_GetRawString(graphics->activeTasks, taskID);
}

void ov102_021E8FB4(u8 *arg0) {
    ov102_021E7A0C(arg0 + 0x64);
}

u16 ov102_021E8FC0(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x4E);
}

u16 ov102_021E8FC8(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x4C);
}

u16 ov102_021E8FD0(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x50);
}
