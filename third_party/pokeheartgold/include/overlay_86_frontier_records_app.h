#ifndef POKEHEARTGOLD_OVERLAY_86_FRONTIER_RECORDS_APP_H
#define POKEHEARTGOLD_OVERLAY_86_FRONTIER_RECORDS_APP_H

#include "bg_window.h"

struct FrontierSave;
struct GridInputHandler;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct Options;
struct OverlayManager;
struct PaletteData;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct UnkStruct_020163E0;

enum BattleFacility {
    FACILITY_TOWER = 1,
    FACILITY_FACTORY,
    FACILITY_FACTORY_OPEN,
    FACILITY_CASTLE,
    FACILITY_HALL,
    FACILITY_ARCADE,
    MAX_FACILITIES,
};

typedef struct FrontierRecordsPaletteFlash {
    u8 isBg : 1;
    u8 index : 7;
    u8 palA : 4;
    u8 palB : 4;
    u8 state;
    u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} FrontierRecordsPaletteFlash;

typedef struct FrontierRecordsList {
    u16 *entries;
    s32 count;
} FrontierRecordsList;

typedef struct FrontierRecordsApp {
    struct OverlayManager *man;
    u8 state;
    u8 nextState;
    u8 challengeType;
    u8 facility;
    u16 species;
    u8 pad_0A[2];
    struct BgConfig *bgConfig;
    Window windows[32];
    struct MsgData *msgData;
    struct MessageFormat *msgFmt;
    struct String *string;
    struct PaletteData *plttData;
    struct Options *options;
    struct SaveData *saveData;
    struct FrontierSave *frontierSave;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprites[2];
    Window buttonWindow;
    FrontierRecordsPaletteFlash flash;
    struct GridInputHandler *inputHandler;
    u16 category;
    u16 cursor;
    s8 page;
    s8 lastPage;
    u16 windowSet;
    FrontierRecordsList lists[26];
    struct UnkStruct_020163E0 *unk_330;
    u16 tilemapRects[3][16];
} FrontierRecordsApp; // size = 0x394

#endif // POKEHEARTGOLD_OVERLAY_86_FRONTIER_RECORDS_APP_H
