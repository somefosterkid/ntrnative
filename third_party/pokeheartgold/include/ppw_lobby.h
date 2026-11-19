#ifndef POKEHEARTGOLD_PPW_LOBBY_H
#define POKEHEARTGOLD_PPW_LOBBY_H

#include "global.h"

typedef enum PPW_LOBBY_RESULT {
    PPW_LOBBY_RESULT_SUCCESS,
    PPW_LOBBY_RESULT_ERROR_ALLOC,
    PPW_LOBBY_RESULT_ERROR_SESSION,
    PPW_LOBBY_RESULT_ERROR_PARAM,
    PPW_LOBBY_RESULT_ERROR_STATE,
    PPW_LOBBY_RESULT_ERROR_CHANNEL,
    PPW_LOBBY_RESULT_ERROR_NODATA,
    PPW_LOBBY_RESULT_ERROR_CONDITION,
    PPW_LOBBY_RESULT_MAX
} PPW_LOBBY_RESULT;

typedef enum PPW_LOBBY_CHANNEL_KIND {
    PPW_LOBBY_CHANNEL_KIND_MAIN,
    PPW_LOBBY_CHANNEL_KIND_FOOT1,
    PPW_LOBBY_CHANNEL_KIND_FOOT2,
    PPW_LOBBY_CHANNEL_KIND_CLOCK,
    PPW_LOBBY_CHANNEL_KIND_NEWS,
    PPW_LOBBY_CHANNEL_KIND_INVALID
} PPW_LOBBY_CHANNEL_KIND;

typedef enum PPW_LOBBY_TIME_EVENT {
    PPW_LOBBY_TIME_EVENT_LOCK,
    PPW_LOBBY_TIME_EVENT_NEON_A0,
    PPW_LOBBY_TIME_EVENT_NEON_A1,
    PPW_LOBBY_TIME_EVENT_NEON_A2,
    PPW_LOBBY_TIME_EVENT_NEON_A3,
    PPW_LOBBY_TIME_EVENT_NEON_A4,
    PPW_LOBBY_TIME_EVENT_NEON_A5,
    PPW_LOBBY_TIME_EVENT_NEON_B0,
    PPW_LOBBY_TIME_EVENT_NEON_B1,
    PPW_LOBBY_TIME_EVENT_NEON_B2,
    PPW_LOBBY_TIME_EVENT_NEON_B3,
    PPW_LOBBY_TIME_EVENT_NEON_C0,
    PPW_LOBBY_TIME_EVENT_NEON_C1,
    PPW_LOBBY_TIME_EVENT_NEON_C2,
    PPW_LOBBY_TIME_EVENT_NEON_C3,
    PPW_LOBBY_TIME_EVENT_MINIGAME_END,
    PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START,
    PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END,
    PPW_LOBBY_TIME_EVENT_PARADE,
    PPW_LOBBY_TIME_EVENT_CLOSE
} PPW_LOBBY_TIME_EVENT;

typedef struct PPW_LobbySystemProfile {
    s64 enterTime;
    PPW_LOBBY_CHANNEL_KIND subChannelKind;
} PPW_LobbySystemProfile;

typedef struct PPW_LobbyRecruitInfo {
    s32 gameKind;
    u32 maxNum;
    u32 currentNum;
    u32 paramVal;
    u8 paramBuf[32];
    char matchMakingString[20];
} PPW_LobbyRecruitInfo;

typedef struct PPW_LobbyTimeInfo {
    s64 currentTime;
    s64 openedTime;
    s64 lockedTime;
} PPW_LobbyTimeInfo;

typedef struct PPW_LobbyVipRecord {
    s32 profileId;
    s32 key;
} PPW_LobbyVipRecord;

typedef struct PPW_LobbyQuestionnaireRecord {
    s32 questionSerialNo;
    s32 questionNo;
    u16 questionSentence[110];
    u16 answer[3][18];
    u8 multiLanguageSummarizeFlags[12];
    BOOL isSpecialWeek;
} PPW_LobbyQuestionnaireRecord;

typedef struct PPW_LobbyQuestionnaire {
    PPW_LobbyQuestionnaireRecord currentQuestionnaireRecord;
    PPW_LobbyQuestionnaireRecord lastQuestionnaireRecord;
    s32 lastResult[3];
    s32 lastMultiResult[3];
} PPW_LobbyQuestionnaire;

#endif // POKEHEARTGOLD_PPW_LOBBY_H
