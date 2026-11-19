#ifndef POKEHEARTGOLD_FIELD_TYPES_DEF_H
#define POKEHEARTGOLD_FIELD_TYPES_DEF_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "sys_task.h"

typedef struct Location {
    int mapId;
    int warpId;
    int x;
    int y;
    int direction;
} Location;

#ifndef POKEHEARTGOLD_FIELD_SYSTEM_TYPEDEF // also in field_system_def.h, which lighter files include instead of this header
#define POKEHEARTGOLD_FIELD_SYSTEM_TYPEDEF
typedef struct FieldSystem FieldSystem;
#endif
typedef struct TaskManager TaskManager;
typedef struct MapLoadManager MapLoadManager;
typedef struct LocalMapObject LocalMapObject;
typedef struct FieldMapObject FieldMapObject;
typedef struct PlayerAvatar PlayerAvatar;
typedef struct MapObjectManager MapObjectManager;
typedef struct BgEvent BgEvent;
typedef struct ObjectEvent ObjectEvent;
typedef struct WarpEvent WarpEvent;
typedef struct CoordEvent CoordEvent;
typedef struct GearPhoneRingManager GearPhoneRingManager;
typedef struct FieldTextureManager FieldTextureManager;

#endif // POKEHEARTGOLD_FIELD_TYPES_DEF_H
