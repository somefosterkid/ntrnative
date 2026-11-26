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

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

enum EasyChatTask {
    EASY_CHAT_SYSTASK_00 = 0,
    EASY_CHAT_SYSTASK_01,
    EASY_CHAT_SYSTASK_02,
    EASY_CHAT_SYSTASK_03,
    EASY_CHAT_SYSTASK_04,
    EASY_CHAT_SYSTASK_05,
    EASY_CHAT_SYSTASK_06,
    EASY_CHAT_SYSTASK_07,
    EASY_CHAT_SYSTASK_08,
    EASY_CHAT_SYSTASK_09,
    EASY_CHAT_SYSTASK_10,
    EASY_CHAT_SYSTASK_11,
    EASY_CHAT_SYSTASK_12,
    EASY_CHAT_SYSTASK_13,
    EASY_CHAT_SYSTASK_14,
    EASY_CHAT_SYSTASK_15,
    EASY_CHAT_SYSTASK_16,
    EASY_CHAT_SYSTASK_17,
    EASY_CHAT_SYSTASK_18,
    EASY_CHAT_SYSTASK_19,
    EASY_CHAT_SYSTASK_20,
    EASY_CHAT_SYSTASK_21,
    EASY_CHAT_SYSTASK_22,
    EASY_CHAT_SYSTASK_23,
    EASY_CHAT_SYSTASK_24,
    EASY_CHAT_SYSTASK_25,
    EASY_CHAT_SYSTASK_26,
    EASY_CHAT_SYSTASK_27,
    EASY_CHAT_SYSTASK_28,
};

void ov102_021E940C(UnkStruct_ov20_021D2128 *param0, int param1);

BOOL ov102_021E9464(UnkStruct_ov20_021D2128 *param0);

int ov102_021E8D1C(EasyChatApp *easyChatApp, int *param1);

void ov102_021E8D50(EasyChatApp *easyChatApp, EasyChatTaskFunc func);

int ov102_021E8D1C(EasyChatApp *easyChatApp, int *param1) {
    switch (*param1) {
    case 0:
        ov102_021E940C(easyChatApp->unk_14, EASY_CHAT_SYSTASK_01);
        (*param1)++;
        break;
    case 1:
        if (ov102_021E9464(easyChatApp->unk_14)) {
            return 1;
        }
        break;
    }

    return 0;
}

void ov102_021E8D50(EasyChatApp *easyChatApp, EasyChatTaskFunc func) {
    easyChatApp->taskFunc = func;
    easyChatApp->taskFuncState = 0;
}
