#include "global.h"
#include "mail_message.h"
#include "pokemon_types_def.h"
#include "touch_hitbox_controller.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_app.h"

s32 ov102_021E8D58(void *);

void ov102_021E8F5C(void *, s8, s8);

s32 ov102_021E940C(void *, s32);

s32 ov102_021E9464(void *);

s32 ov102_021EA258(void *);


void ov102_021E8E84(EasyChatApp *arg0, s32 *arg1);

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
BOOL sub_02090DC0(const EasyChatArgs *args, const u16 *words, const MailMessage *sentence);
typedef struct EasyChatWordList EasyChatWordList;
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
enum EasyChatType {
    EASY_CHAT_TYPE_ONE_WORD = 0,
    EASY_CHAT_TYPE_TWO_WORDS,
    EASY_CHAT_TYPE_SENTENCE,
};
BOOL ov102_021E8EF8(EasyChatApp *easyChatApp);
BOOL ov102_021E8F14(EasyChatApp *easyChatApp);

void ov102_021E8E84(EasyChatApp *arg0, s32 *arg1) {
    s32 v0;
    s32 v1;

    v0 = *arg1;
    switch (v0) {
    case 0:
        ov102_021E8F5C(&arg0->unk_70, 1, 1);
        ov102_021E940C(arg0->unk_14, 0x16);
        *arg1 = 1;
        return;
    case 1:
        v1 = ov102_021EA258(arg0->unk_14);
        switch (v1) {
        case 0:
            ov102_021E940C(arg0->unk_14, 0x19);
            *arg1 = 2;
            return;
        case 1:
            ov102_021E7A4C(arg0, ov102_021E8D1C);
            *arg1 = 2;
            return;
        }
        break;
    case 2:
        if (ov102_021E9464(arg0->unk_14) != 0) {
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
