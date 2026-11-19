#include "global.h"

#include "constants/player_avatar.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "field_types_def.h"

enum PlayerTileMovement {
    TILE_MOVE_NONE = 0,
    TILE_MOVE_ICE,
    TILE_MOVE_BIKE_SLOPE,
    TILE_MOVE_BIKE_RAMP_EAST,
    TILE_MOVE_BIKE_RAMP_WEST,
    TILE_MOVE_CYCLING_ROAD_DOWN,
    TILE_MOVE_MAX,
};

typedef BOOL (*TileBehaviorFunc)(u8);

typedef struct {
    TileBehaviorFunc func;
    u32 tileMove;
} TileBehaviorCheck;

BOOL sub_0205E078(PlayerAvatar *playerAvatar, u32 tileBehavior, int dir);

extern const TileBehaviorCheck _020FCB88[];

u32 sub_0205D01C(PlayerAvatar *playerAvatar, int dir);

extern int (*const _020FCB7C[3])(PlayerAvatar *, int);
BOOL sub_0205D07C(PlayerAvatar *playerAvatar, u32 tileMove, int dir);

s32 sub_0205D1FC(PlayerAvatar *);
s32 sub_0205D09C(PlayerAvatar *playerAvatar, int dir);

u32 sub_0205D01C(PlayerAvatar *playerAvatar, int dir) {
    int i = 0;
    u32 tileBehavior = sub_0205F504(PlayerAvatar_GetMapObject(playerAvatar));

    if (sub_0205E078(playerAvatar, tileBehavior, dir) == TRUE) {
        return (enum PlayerTileMovement)2;
    }

    if (PlayerAvatar_CheckFlag1(playerAvatar) == TRUE) {
        return TILE_MOVE_NONE;
    }

    do {
        if (_020FCB88[i].func(tileBehavior) == TRUE) {
            return _020FCB88[i].tileMove;
        }

        i++;
    } while (_020FCB88[i].func != NULL);

    return TILE_MOVE_NONE;
}

BOOL sub_0205D07C(PlayerAvatar *playerAvatar, u32 tileMove, int dir) {
    if (_020FCB7C[tileMove](playerAvatar, dir) == TRUE) {
        return TRUE;
    }

    return FALSE;
}

s32 sub_0205D09C(PlayerAvatar *playerAvatar, int dir) {
    sub_0205D1FC(playerAvatar);
    return 0;
}
