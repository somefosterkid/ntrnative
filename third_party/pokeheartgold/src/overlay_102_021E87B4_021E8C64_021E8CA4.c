#include "global.h"
#include "mail_message_types.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"
#include "overlay_102_easy_chat_app.h"

struct EasyChatApp;

struct EasyChatArgs;

struct MailMessage;

struct EasyChatWordList;

struct TouchHitboxController;

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


typedef struct EasyChatWordList EasyChatWordList;

u16 sub_02091C60(const EasyChatWordList *wordList, u32 group, u32 index);

u16 sub_02091CAC(const EasyChatWordList *wordList, u32 letter, u32 index);

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

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
