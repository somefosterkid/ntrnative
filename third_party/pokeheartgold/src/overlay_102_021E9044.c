#include "global.h"

#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchScreenActions;
struct UnkStruct_ov20_021D2128_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
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
u32 ov102_021E87A8(const UnkStruct_ov20_021D18BC *param0);

u32 ov102_021E9044(const EasyChatApp *easyChatApp);

u32 ov102_021E9044(const EasyChatApp *easyChatApp) {
    return ov102_021E87A8(&(easyChatApp->unk_54));
}
