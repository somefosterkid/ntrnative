#include "global.h"

#include "overlay_manager.h"
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

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

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

u32 ov102_021EA238(UnkStruct_ov20_021D2128 *param0);

enum SoundScene {
    SOUND_SCENE_NONE = 0,
    SOUND_SCENE_TITLE_SCREEN,
    SOUND_SCENE_2,
    SOUND_SCENE_3,
    SOUND_SCENE_FIELD,
    SOUND_SCENE_BATTLE,
    SOUND_SCENE_CONTEST,
    SOUND_SCENE_7,
    SOUND_SCENE_8,
    SOUND_SCENE_9,
    SOUND_SCENE_10,
    SOUND_SCENE_11,
    SOUND_SCENE_12,
    SOUND_SCENE_13,
    SOUND_SCENE_14,
    SOUND_SCENE_15,
    SOUND_SCENE_16,
    SOUND_SCENE_17,
    SOUND_SCENE_18,
    SOUND_SCENE_19,
    SOUND_SCENE_20,
    SOUND_SCENE_21,
    SOUND_SCENE_VILLA_MUSIC_BOX,
    SOUND_SCENE_23,
    SOUND_SCENE_24,
    SOUND_SCENE_25,
    SOUND_SCENE_26,
    SOUND_SCENE_27,
    SOUND_SCENE_28,
    SOUND_SCENE_29,
    SOUND_SCENE_30,
    SOUND_SCENE_31,
    SOUND_SCENE_32,
    SOUND_SCENE_33,
    SOUND_SCENE_34,
    SOUND_SCENE_35,
    SOUND_SCENE_36,
    SOUND_SCENE_37,
    SOUND_SCENE_38,
    SOUND_SCENE_39,
    SOUND_SCENE_40,
    SOUND_SCENE_41,
    SOUND_SCENE_42,
    SOUND_SCENE_43,
    SOUND_SCENE_44,
    SOUND_SCENE_45,
    SOUND_SCENE_46,
    SOUND_SCENE_47,
    SOUND_SCENE_48,
    SOUND_SCENE_49,
    SOUND_SCENE_50,
    SOUND_SCENE_MAX,

    SOUND_SCENE_SUB_BAG = SOUND_SCENE_MAX,
    SOUND_SCENE_SUB_52,
    SOUND_SCENE_SUB_53,
    SOUND_SCENE_SUB_54,
    SOUND_SCENE_SUB_55,
    SOUND_SCENE_SUB_56,
    SOUND_SCENE_SUB_57,
    SOUND_SCENE_SUB_58,
    SOUND_SCENE_SUB_59,
    SOUND_SCENE_SUB_60,
    SOUND_SCENE_SUB_61,
    SOUND_SCENE_SUB_62,
    SOUND_SCENE_SUB_63,
    SOUND_SCENE_SUB_64,
    SOUND_SCENE_SUB_65,
    SOUND_SCENE_SUB_66,
    SOUND_SCENE_SUB_67,
    SOUND_SCENE_SUB_68,
};

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 bgmID, int unused);

EasyChatApp *ov102_021E7888(OverlayManager *appMan);

void EasyChat_TouchButtonCallback(u32 buttonIndex, enum TouchScreenButtonState buttonState, void *data);

BOOL EasyChat_Init(OverlayManager *appMan, int *state);

BOOL EasyChat_Init(OverlayManager *appMan, int *state) {
    EasyChatApp *easyChatApp;

    switch (*state) {
    case 0:
        Sound_SetSceneAndPlayBGM(SOUND_SCENE_SUB_62, 0, 0);
        Heap_Create(HEAP_ID_3, HEAP_ID_34, 32768);
        Heap_Create(HEAP_ID_3, HEAP_ID_35, 163840);
        easyChatApp = ov102_021E7888(appMan);
        ov102_021E940C(easyChatApp->unk_14, EASY_CHAT_SYSTASK_00);
        (*state)++;
        break;
    case 1:
        easyChatApp = OverlayManager_GetData(appMan);

        if (ov102_021E9464(easyChatApp->unk_14)) {
            if (easyChatApp->type == EASY_CHAT_TYPE_SENTENCE) {
                easyChatApp->pad_65[5] = ov102_021EA238(easyChatApp->unk_14);
            } else {
                easyChatApp->pad_65[5] = 0;
            }

            return 1;
        }

        break;
    }

    return 0;
}
