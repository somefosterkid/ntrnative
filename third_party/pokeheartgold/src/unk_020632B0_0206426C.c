#include "global.h"
#include "player_avatar.h"

struct ApproachingTrainerTemplate;
struct MapObject;
struct PlayerAvatar;
typedef struct MapObject MapObject;
u32 MapObject_GetScriptID(const MapObject *mapObj);
u16 ScriptNumToTrainerNum(u16 scriptID);
BOOL TrainerNumIsDouble(u32 trainerID);
typedef struct ApproachingTrainerTemplate {
    int distance;
    int direction;
    int script;
    int trainerID;
    BOOL isDoubleBattle;
    MapObject *trainerMapObj;
} ApproachingTrainerTemplate;
u32 MapObject_GetType(const MapObject *mapObj);
enum TrainerType {
    TRAINER_TYPE_NONE = 0,
    TRAINER_TYPE_NORMAL = 1,
    TRAINER_TYPE_VIEW_ALL_DIRECTIONS = 2,
    TRAINER_TYPE_UNK_003 = 3,
    TRAINER_TYPE_FACE_SIDES = 4,
    TRAINER_TYPE_FACE_COUNTERCLOCKWISE = 5,
    TRAINER_TYPE_FACE_CLOCKWISE = 6,
    TRAINER_TYPE_SPIN_COUNTERCLOCKWISE = 7,
    TRAINER_TYPE_SPIN_CLOCKWISE = 8,
    TRAINER_TYPE_UNK_009 = 9,
    TRAINER_TYPE_UNK_010 = 10,
    TRAINER_TYPE_UNK_011 = 11,
};
int MapObject_GetFacingDirection(const MapObject *mapObj);
int MapObject_GetParam(const MapObject *mapObj, int index);
int sub_02064298(const MapObject *trainerMapObj);
int sub_0206439C(const MapObject *mapObj, int direction, int range, int playerX, int playerZ, int unused);
BOOL sub_02064468(const MapObject *mapObj, int direction, int distance);

void GetEngagingTrainerParams(ApproachingTrainerTemplate *apprTrTemplate, MapObject *trainerMapObj, int distance, int direction);
int sub_02064298(const MapObject *trainerMapObj);
int sub_020642C4(const MapObject *trainerMapObj, PlayerAvatar *playerAvatar, int *direction);

void GetEngagingTrainerParams(ApproachingTrainerTemplate *apprTrTemplate, MapObject *trainerMapObj, int distance, int direction) {
    apprTrTemplate->distance = distance;
    apprTrTemplate->direction = direction;
    apprTrTemplate->script = MapObject_GetScriptID(trainerMapObj);
    apprTrTemplate->trainerID = ScriptNumToTrainerNum(apprTrTemplate->script);
    apprTrTemplate->isDoubleBattle = TrainerNumIsDouble((u16)apprTrTemplate->trainerID);
    apprTrTemplate->trainerMapObj = trainerMapObj;
}

int sub_02064298(const MapObject *trainerMapObj) {
    int trainerType = MapObject_GetType(trainerMapObj);

    switch (trainerType) {
    case TRAINER_TYPE_FACE_SIDES:
    case TRAINER_TYPE_FACE_COUNTERCLOCKWISE:
    case TRAINER_TYPE_FACE_CLOCKWISE:
    case TRAINER_TYPE_SPIN_COUNTERCLOCKWISE:
    case TRAINER_TYPE_SPIN_CLOCKWISE:
        trainerType = TRAINER_TYPE_NORMAL;
        break;
    }

    return trainerType;
}

int sub_020642C4(const MapObject *trainerMapObj, PlayerAvatar *playerAvatar, int *direction) {
    int trainerSightRange, trainerFaceDir, playerX, playerZ, distance;

    int trainerType = sub_02064298(trainerMapObj);

    if (trainerType == TRAINER_TYPE_NORMAL) {
        playerX = PlayerAvatar_GetXCoord(playerAvatar);
        playerZ = PlayerAvatar_GetZCoord(playerAvatar);
        trainerFaceDir = MapObject_GetFacingDirection(trainerMapObj);
        trainerSightRange = MapObject_GetParam(trainerMapObj, 0);
        distance = sub_0206439C(trainerMapObj, trainerFaceDir, trainerSightRange, playerX, playerZ, 0);

        if (distance != -1) {
            if (!sub_02064468(trainerMapObj, trainerFaceDir, distance)) {
                *direction = trainerFaceDir;
                return distance;
            }
        }

        return -1;
    }

    if (trainerType == TRAINER_TYPE_VIEW_ALL_DIRECTIONS) {
        playerX = PlayerAvatar_GetXCoord(playerAvatar);
        playerZ = PlayerAvatar_GetZCoord(playerAvatar);
        trainerSightRange = MapObject_GetParam(trainerMapObj, 0);
        trainerFaceDir = 0;

        do {
            distance = sub_0206439C(trainerMapObj, trainerFaceDir, trainerSightRange, playerX, playerZ, 0);

            if (distance != -1) {
                if (!sub_02064468(trainerMapObj, trainerFaceDir, distance)) {
                    *direction = trainerFaceDir;
                    return distance;
                }
            }

            trainerFaceDir++;
        } while (trainerFaceDir < 4);

        return -1;
    }

    return -1;
}
