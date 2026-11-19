#include "global.h"

#include "pm_string.h"
#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatWordList;

struct TouchHitboxController;

struct UnkStruct_ov20_021D2128_t;

enum EasyChatMode {
    GROUP_MODE = 0,
    ABC_MODE,
};

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

void sub_02091C40(const EasyChatWordList *wordList, u32 group, u32 index, String *out);

void sub_02091C84(const EasyChatWordList *wordList, u32 letter, u32 index, String *out);

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

void ov102_021E8FFC(const EasyChatApp *easyChatApp, u32 param1, String *param2);

s32 ov102_021E878C(void *);
s32 ov102_021E9028(void *arg0);

void ov102_021E8FFC(const EasyChatApp *easyChatApp, u32 param1, String *param2) {
    if (easyChatApp->mode == GROUP_MODE) {
        sub_02091C40(easyChatApp->unk_18, easyChatApp->categoryCursorPos, param1, param2);
    } else {
        sub_02091C84(easyChatApp->unk_18, easyChatApp->categoryCursorPos, param1, param2);
    }
}

s32 ov102_021E9028(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x60) != 0) {
        return -1;
    }
    return ov102_021E878C(arg0 + 0x54);
}
