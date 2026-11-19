#include "global.h"
#include "trainer_case.h"

struct PlayTime;
enum Badge {
    BADGE_ID_COAL = 0,
    BADGE_ID_FOREST = 1,
    BADGE_ID_COBBLE = 2,
    BADGE_ID_FEN = 3,
    BADGE_ID_RELIC = 4,
    BADGE_ID_MINE = 5,
    BADGE_ID_ICICLE = 6,
    BADGE_ID_BEACON = 7,
    MAX_BADGES = 8,
};
typedef struct PlayTime PlayTime;
typedef u16 charcode_t;
charcode_t *CopyU16StringArrayN(charcode_t *dst, const charcode_t *src, u32 num);
struct PlayTime {
    u16 hours;
    u8 minutes;
    u8 seconds;
};
u16 GetIGTHours(const PlayTime *playTime);
u8 GetIGTMinutes(const PlayTime *playTime);

void sub_020692A0(u8 badgesInteractable, u8 gameVersion, u8 cardLevel, u8 gymLeadersToHide, u8 language, TrainerCase *trainerCase);
void sub_020692C4(u16 id, u8 gender, const u16 *name, u32 money, u32 seenPokemon, BOOL pokedexObtained, u32 score, TrainerCase *trainerCase);
void sub_02069308(u8 gameCompleted, const PlayTime *playTime, const RTCDate *adventureStartedDate, const RTCDate *firstCompletionDate, const RTCTime *firstCompletionTime, const u8 liveTimeDisplay, TrainerCase *trainerCase);

void sub_020692A0(u8 badgesInteractable, u8 gameVersion, u8 cardLevel, u8 gymLeadersToHide, u8 language, TrainerCase *trainerCase) {
    trainerCase->badgesInteractable = badgesInteractable;
    trainerCase->gameVersion = gameVersion;
    trainerCase->cardLevel = cardLevel;
    trainerCase->language = language;
    trainerCase->gymLeadersToHideUnused = gymLeadersToHide;
}

void sub_020692C4(u16 id, u8 gender, const u16 *name, u32 money, u32 seenPokemon, BOOL pokedexObtained, u32 score, TrainerCase *trainerCase) {
    trainerCase->trainerId = id;
    trainerCase->gender = gender;

    CopyU16StringArrayN(trainerCase->name, name, 7 + 1);

    trainerCase->money = money;
    trainerCase->seenPokemon = seenPokemon;
    trainerCase->pokedexObtained = pokedexObtained;
    trainerCase->score = score;
}

void sub_02069308(u8 gameCompleted, const PlayTime *playTime, const RTCDate *adventureStartedDate, const RTCDate *firstCompletionDate, const RTCTime *firstCompletionTime, const u8 liveTimeDisplay, TrainerCase *trainerCase) {
    trainerCase->playTimeHours = GetIGTHours(playTime);
    trainerCase->playTimeMinutes = GetIGTMinutes(playTime);
    trainerCase->adventureStartedYear = adventureStartedDate->year;
    trainerCase->adventureStartedMonth = adventureStartedDate->month;
    trainerCase->adventureStartedDay = adventureStartedDate->day;

    if (gameCompleted) {
        trainerCase->hofDebutYear = firstCompletionDate->year;
        trainerCase->hofDebutMonth = firstCompletionDate->month;
        trainerCase->hofDebutDay = firstCompletionDate->day;
        trainerCase->hofDebutHour = firstCompletionTime->hour;
        trainerCase->hofDebutMinute = firstCompletionTime->minute;
    } else {
        trainerCase->hofDebutYear = 0;
        trainerCase->hofDebutMonth = 0;
        trainerCase->hofDebutDay = 0;
        trainerCase->hofDebutHour = 0;
        trainerCase->hofDebutMinute = 0;
    }

    trainerCase->liveTimeDisplay = liveTimeDisplay;

    if (liveTimeDisplay) {
        trainerCase->playTime = playTime;
    } else {
        trainerCase->playTime = 0L;
    }
}
