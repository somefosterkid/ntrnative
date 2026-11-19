#include "global.h"

#include "save.h"
#include "player_data.h"

struct TrainerCaseSaveData;
struct PlayerProfile;
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
PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);
typedef struct TrainerCaseSaveDataBadge {
    int polish;
} TrainerCaseSaveDataBadge;
typedef struct TrainerCaseSaveData TrainerCaseSaveData;
TrainerCaseSaveData *Save_TrainerCard_Get(SaveData *saveData);
TrainerCaseSaveDataBadge *TrainerCard_GetBadgeShininessArr(TrainerCaseSaveData *tcSaveData);
extern int GetShininessOfBadgeI(u8 badgeID, TrainerCaseSaveDataBadge badges[]);
extern void SetShininessOfBadgeI(u8 badgeID, int polish, TrainerCaseSaveDataBadge badges[]);

void sub_0206759C(SaveData *saveData, s32 daysPassed);

void sub_0206759C(SaveData *saveData, s32 daysPassed) {
    if (daysPassed <= 0) {
        return;
    }

    TrainerCaseSaveData *tcSaveData = Save_TrainerCard_Get(saveData);
    TrainerCaseSaveDataBadge *badges = TrainerCard_GetBadgeShininessArr(tcSaveData);
    PlayerProfile *trainerInfo = Save_PlayerData_GetProfile(saveData);

    for (u8 badgeID = 0; badgeID < MAX_BADGES; badgeID++) {
        if (!PlayerProfile_TestBadgeFlag(trainerInfo, badgeID)) {
            continue;
        }

        int polish = GetShininessOfBadgeI(badgeID, badges);

        if (0 < polish && polish < 199 + 1) {
            polish -= 10 * daysPassed;
        } else {
            ((polish < 199 + 1) ? (void)0 : GF_AssertFail());
            polish = 0;
        }

        if (polish < 0) {
            polish = 0;
        }

        SetShininessOfBadgeI(badgeID, polish, badges);
    }
}
