#include "global.h"

struct WaveOutParam;

enum SoundSystemParam {
    SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_HANDLE = 0,
    SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_HANDLE,
    SOUND_SYSTEM_PARAM_CURRENT_BANK_INFO,
    SOUND_SYSTEM_PARAM_CAPTURE_BUFFER,
    SOUND_SYSTEM_PARAM_FILTER_CALLBACK_PARAM,
    SOUND_SYSTEM_PARAM_BGM_FIXED,
    SOUND_SYSTEM_PARAM_ACTIVE_CRY,
    SOUND_SYSTEM_PARAM_FADE_COUNTER,
    SOUND_SYSTEM_PARAM_FOLLOW_UP_WAIT_FRAMES,
    SOUND_SYSTEM_PARAM_FOLLOW_UP_FADE_FRAMES,
    SOUND_SYSTEM_PARAM_CURRENT_BGM,
    SOUND_SYSTEM_PARAM_NEXT_BGM,
    SOUND_SYSTEM_PARAM_FIELD_BGM_PAUSED,
    SOUND_SYSTEM_PARAM_BGM_PAUSED,
    SOUND_SYSTEM_PARAM_FANFARE_DELAY,
    SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSED_PLAYBACK,
    SOUND_SYSTEM_PARAM_WAVE_OUT_PRIMARY_ALLOCATED,
    SOUND_SYSTEM_PARAM_WAVE_OUT_SECONDARY_ALLOCATED,
    SOUND_SYSTEM_PARAM_ECHO_ENABLED,
    SOUND_SYSTEM_PARAM_FIELD_BGM_BANK_STATE,
    SOUND_SYSTEM_PARAM_FILTER_SIZE,

    SOUND_SYSTEM_PARAM_MAIN_SCENE,
    SOUND_SYSTEM_PARAM_SUB_SCENE,

    SOUND_SYSTEM_PARAM_HEAP_STATE_EMPTY,
    SOUND_SYSTEM_PARAM_HEAP_STATE_PERSISTENT,
    SOUND_SYSTEM_PARAM_HEAP_STATE_BGM_BANK,
    SOUND_SYSTEM_PARAM_HEAP_STATE_SFX,
    SOUND_SYSTEM_PARAM_HEAP_STATE_BGM,
    SOUND_SYSTEM_PARAM_HEAP_STATE_SUB_SFX,
    SOUND_SYSTEM_PARAM_HEAP_STATE_FANFARE,

    SOUND_SYSTEM_PARAM_CHATOT_CRY_PLAYING,
    SOUND_SYSTEM_PARAM_DEFAULT_CHATOT_CRY,
    SOUND_SYSTEM_PARAM_FIELD_BGM,
    SOUND_SYSTEM_PARAM_CURRENT_WAVE_DATA,
    SOUND_SYSTEM_PARAM_WAVE_OUT_REVERSE_BUFFER,
    SOUND_SYSTEM_PARAM_CRY_DURATION_TASK,
    SOUND_SYSTEM_PARAM_CHATOT_CRY,

    SOUND_SYSTEM_PARAM_UNUSED_37,
    SOUND_SYSTEM_PARAM_UNUSED_38,
    SOUND_SYSTEM_PARAM_UNUSED_39,
    SOUND_SYSTEM_PARAM_UNUSED_40,

    SOUND_SYSTEM_PARAM_CRY_A_MOD,
    SOUND_SYSTEM_PARAM_CRY_A_PAN,
    SOUND_SYSTEM_PARAM_CRY_A_VOLUME,
    SOUND_SYSTEM_PARAM_CRY_A_HEAP_ID,
    SOUND_SYSTEM_PARAM_CRY_A_WAVE_ID,
    SOUND_SYSTEM_PARAM_CRY_A_DELAY,

    SOUND_SYSTEM_PARAM_CRY_B_MOD,
    SOUND_SYSTEM_PARAM_CRY_B_PAN,
    SOUND_SYSTEM_PARAM_CRY_B_VOLUME,
    SOUND_SYSTEM_PARAM_CRY_B_HEAP_ID,
    SOUND_SYSTEM_PARAM_CRY_B_WAVE_ID,
    SOUND_SYSTEM_PARAM_CRY_B_DELAY,

    SOUND_SYSTEM_PARAM_ALLOW_2_POKEMON_CRIES,

    SOUND_SYSTEM_PARAM_54,
};

void *GF_SdatGetAttrPtr(enum SoundSystemParam param);

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

enum WaveOutChannel {
    WAVE_OUT_CHANNEL_PRIMARY = 14,
    WAVE_OUT_CHANNEL_SECONDARY = 15,
};

enum SoundChannelConfig {
    SOUND_CHANNEL_CONFIG_DEFAULT = 0,
    SOUND_CHANNEL_CONFIG_TITLE,
    SOUND_CHANNEL_CONFIG_ENDING,
};

typedef struct WaveOutParam WaveOutParam;

void Sound_SetScene(int scene);

void Sound_ConfigureBGMChannelsAndReverb(enum SoundChannelConfig config);

void Sound_Impl_PlayFieldBGM(u16 bgmID, int unused);

void Sound_Impl_PlayBattleBGM(u16 bgmID, int unused);

void sub_020048AC(u16 bgmID, int unused);

void sub_02005280(u16 bgmID, int unused);

void sub_020052A4(u16 bgmID, int unused);

void Sound_Impl_LoadSubSceneSoundData(u8 scene);

void sub_020052E4(s32 scene, u16 bgmID, int unused);

BOOL sub_020056E8(WaveOutParam *param, enum WaveOutChannel channel);

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 bgmID, int unused);

void sub_02005AF8(enum SoundChannelConfig config);

void sub_0200508C(u16 bgmID, int unused);

void sub_02005228(u16 bgmID, int unused);

void sub_02005260(u16 bgmID, int unused);

void sub_020052C8(s32 scene);

BOOL PlayBGM(u16 seqno);

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 bgmID, int unused) {
    u8 *mainScene = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_MAIN_SCENE);
    u8 *subScene = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_SUB_SCENE);
    u16 *fanfareDelay = GF_SdatGetAttrPtr(SOUND_SYSTEM_PARAM_FANFARE_DELAY);
    if (scene < SOUND_SCENE_MAX) {
        if ((*mainScene) == scene) {
            return 0;
        }
    } else if ((*subScene) == scene) {
        return 0;
    }
    Sound_SetScene(scene);
    switch (scene) {
    case SOUND_SCENE_FIELD:
        sub_02005AF8(SOUND_CHANNEL_CONFIG_DEFAULT);
        sub_0200508C(bgmID, unused);
        *fanfareDelay = 0;
        break;

    case SOUND_SCENE_BATTLE:
        sub_02005228(bgmID, unused);
        break;

    case 11:
        sub_02005260(bgmID, unused);
        break;

    case SOUND_SCENE_CONTEST:
        sub_02005280(bgmID, unused);
        break;

    case SOUND_SCENE_7:
        sub_020052A4(bgmID, unused);
        break;

    case SOUND_SCENE_SUB_BAG:

    case 52:

    case 53:

    case 54:

    case 55:

    case 56:

    case 57:

    case 58:

    case 59:

    case 60:

    case 61:

    case 62:

    case 63:

    case 64:

    case 65:

    case 66:

    case 67:
    case 69:
    case 70:
    case 71:
    case 72:
    case 74:
        sub_020052C8(scene);
        break;

    case 68:
        sub_020052C8(scene);
        PlayBGM(bgmID);
        break;

    case SOUND_SCENE_TITLE_SCREEN:
        sub_02005AF8(SOUND_CHANNEL_CONFIG_TITLE);
        sub_020052E4(scene, bgmID, unused);
        break;

    case SOUND_SCENE_14:
        sub_02005AF8(SOUND_CHANNEL_CONFIG_ENDING);
        sub_020052E4(scene, bgmID, unused);
        break;

    case SOUND_SCENE_2:
        sub_02005AF8(SOUND_CHANNEL_CONFIG_DEFAULT);
        sub_020052E4(scene, bgmID, unused);
        break;

    case SOUND_SCENE_3:

    case SOUND_SCENE_8:

    case SOUND_SCENE_9:

    case SOUND_SCENE_10:

    case SOUND_SCENE_12:

    case SOUND_SCENE_13:

    case SOUND_SCENE_15:

    case SOUND_SCENE_16:

    case SOUND_SCENE_17:

    case SOUND_SCENE_18:

    case SOUND_SCENE_19:

    case SOUND_SCENE_20:

    case SOUND_SCENE_21:

    case SOUND_SCENE_23:
    case SOUND_SCENE_24:
    case SOUND_SCENE_25:
    case 73:
        sub_020052E4(scene, bgmID, unused);
        break;

    case SOUND_SCENE_VILLA_MUSIC_BOX:
        sub_020052E4(scene, bgmID, unused);
        break;

    default:
        return 0;
    }

    return 1;
}
