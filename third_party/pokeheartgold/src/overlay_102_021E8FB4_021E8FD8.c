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

enum EasyChatMode {
    GROUP_MODE = 0,
    ABC_MODE,
};

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

u32 sub_02091C38(const EasyChatWordList *wordList, u32 group);

u32 sub_02091C74(const EasyChatWordList *wordList, u32 letter);

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

u32 ov102_021E8FD8(const EasyChatApp *easyChatApp);

u32 ov102_021E8FD8(const EasyChatApp *easyChatApp) {
    if (easyChatApp->mode == GROUP_MODE) {
        return sub_02091C38(easyChatApp->unk_18, easyChatApp->categoryCursorPos);
    } else {
        return sub_02091C74(easyChatApp->unk_18, easyChatApp->categoryCursorPos);
    }
}
