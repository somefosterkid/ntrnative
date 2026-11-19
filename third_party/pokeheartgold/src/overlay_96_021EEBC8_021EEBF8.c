#include "global.h"

struct CommPlayer;

struct CommPlayerSent;

struct CommPlayerSys;

struct Field;

struct FldCommActEntry;

struct FldCommActSys;

struct GameCommSys;

struct GameData;

struct GameEvent;

struct GameProcManager;

struct GameSystem;

struct GameSystemProcData;

struct ISS;

struct MMSys;

typedef struct Field Field;

typedef struct GameCommSys GameCommSys;

typedef struct GameData GameData;

typedef struct GameEvent GameEvent;

typedef struct GameSystem GameSystem;

typedef struct GameProcManager GameProcManager;

typedef struct ISS ISS;

typedef struct MMSys MMSys;

typedef struct FldCommActSys FldCommActSys;

typedef struct GameSystemProcData GameSystemProcData;

typedef struct CommPlayerSys CommPlayerSys;

typedef struct CommPlayer CommPlayer;

typedef struct CommPlayerSent CommPlayerSent;

typedef u16 HeapID;

struct CommPlayer {
    VecFx32 pos;
    u32 unkC;
    u16 dir;
    u16 unk12;
    u8 active : 1;

    u8 hidden : 1;
    u8 exState : 4;
    void *actor;
};

struct CommPlayerSent {
    VecFx32 pos;
    u16 dir;
    u8 exState : 4;
    u8 busy : 1;
};

struct CommPlayerSys {
    GameSystem *gsys;
    FldCommActSys *act_ctrl;
    CommPlayer players[4];
    CommPlayerSent last;
    u32 unk88;
    u16 heapId;
    u8 unk8E;
    u8 unk8F;
    u8 unk90;
};

u32 ov96_021EEC0C(FldCommActSys *sys, s16 a1, s32 a2, s16 a3, u32 *a4, u32 *a5);

typedef struct FldCommActEntry FldCommActEntry;

struct FldCommActSys {
    u16 count;
    u16 unk2;
    MMSys *actorSystem;
    FldCommActEntry *entries;
};

typedef GameEvent *(*GameEventProvider)(GameSystem *gsys, void *args);

typedef enum {
    GAME_ENTRYPOINT_OPENING,
    GAME_ENTRYPOINT_FIELD_CONTINUE,
    GAME_ENTRYPOINT_DEBUG,
} GameEntryPoint;

struct GameSystemProcData {
    GameEntryPoint entryPoint;
    VecFx32 spawnPos;
    u16 zoneId;
    u16 unk12;
};

struct GameSystem {
    HeapID heapId;
    GameSystemProcData *procData;
    GameProcManager *procManager;

    u32 procManagerState;

    GameEventProvider eventProvider;
    void *eventProviderData;
    GameEvent *event;
    GameData *gameData;

    Field *field;
    GameCommSys *comm;
    u32 unk28;
    ISS *iss;
    u8 unk34;

    u8 eventRunning;

    u8 unk36;

    u8 unk37_0 : 1;
};

u32 ov96_021EEBF8(FldCommActSys *sys, s16 a1, s32 a2, s16 a3, u32 *a4, u32 *a5);

u32 ov96_021EEBF8(FldCommActSys *sys, s16 a1, s32 a2, s16 a3, u32 *a4, u32 *a5) {
    return ov96_021EEC0C(sys, a1, a2, a3, a4, a5);
}
