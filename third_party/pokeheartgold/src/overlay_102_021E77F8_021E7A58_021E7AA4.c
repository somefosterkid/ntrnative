#include "global.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

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

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

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

int EasyChat_Main_OneWord(EasyChatApp *easyChatApp, int *param1);
int ov102_021E7D0C(struct EasyChatApp *arg0, int *arg1);

int EasyChat_Main_TwoWords(EasyChatApp *easyChatApp, int *param1);
int ov102_021E7D0C(struct EasyChatApp *arg0, int *arg1);

int EasyChat_Main_Sentence(EasyChatApp *easyChatApp, int *param1);
int ov102_021E821C(struct EasyChatApp *arg0, int *arg1);

void ov102_021E77F8(u32 arg0, u32 arg1, void *arg2);

int ov102_021E7AA4(EasyChatApp *easyChatApp, int *param1);

int ov102_021E7AA4(EasyChatApp *easyChatApp, int *param1) {
    static const EasyChatMainFunc funcs[] = {
        [EASY_CHAT_TYPE_ONE_WORD] = ov102_021E7D0C,
        [EASY_CHAT_TYPE_TWO_WORDS] = ov102_021E7D0C,
        [EASY_CHAT_TYPE_SENTENCE] = ov102_021E821C,
    };

    return funcs[easyChatApp->type](easyChatApp, param1);
}
