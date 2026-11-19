#include "global.h"
#include "overlay_81_battle_factory_app_args.h"
#include "pokemon_summary_args.h"
#include "field_system_def.h"
#include "overlay_80_field_frontier_dto.h"
#include "overlay_80_battle_factory.h"
#include "constants/heap.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BattleFactory;

struct BattleFactoryAppArgs;

struct BattleFactorySave;

struct BattleFrontier;

struct BattleRegulation;

struct BattleTower;

struct BgConfig;

struct Camera;

struct ChatotCry;

struct ListMenuCursor;

struct DynamicTerrainHeightManager;

struct MailMessage;

struct FieldBattleDTO;

struct FieldEffectManager;

struct FieldFrontierDTO;

struct TaskManager;

struct FieldWildBattleMetadata;

struct FogManager;

struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct G2dRenderer;

struct HoneyTreeShakeList;

struct JournalEntry;

struct LandDataManager;

struct ListMenu;

struct ListMenuTemplate;

struct Location;

struct MapHeaderData;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct ListMenu2D;

struct MsgData;

struct ModelAttributes;

struct Options;

struct PalPad;

struct Party;

struct PlayerAvatar;

struct PokedexMemory;

struct PokepicTemplate;

struct PokemonSummary;

struct RadarChain;

struct SaveData;

struct Signpost;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct TerrainAttributes;

struct TerrainCollisionManager;

struct TradeRoom;

struct TradeRoomArgs;

struct TradeRoomMonDisplayData;

struct TrainerCase;

struct PlayerProfile;

struct UnionRoomManager;

struct UnkStruct_ov5_021EB0E0;

struct WiFiList;

struct Window;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct String String;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct MsgData MsgData;

typedef struct ListMenuItem ListMenuItem;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

typedef struct ListMenu2D ListMenu2D;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

typedef struct SaveData SaveData;

typedef struct ChatotCry ChatotCry;

typedef struct Party Party;

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteResourcesHeader {
    const NNSG2dImageProxy *imageProxy;
    const NNSG2dCharacterData *charData;
    const NNSG2dImagePaletteProxy *plttProxy;
    NNSG2dCellDataBank *cellData;
    const NNSG2dCellAnimBankData *cellAnim;
    const NNSG2dMultiCellDataBank *multiCellData;
    const NNSG2dMultiCellAnimBankData *multiCellAnim;
    BOOL flag;
    u8 priority;
} SpriteResourcesHeader;

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct PlayerProfile PlayerProfile;

typedef struct MessageFormat MessageFormat;

typedef struct FrontierScriptManager FrontierScriptManager;

typedef struct FrontierScriptContext FrontierScriptContext;

typedef BOOL (*FrontierScrCmdFunc)(FrontierScriptContext *);

typedef BOOL (*FrontierShouldResumeScriptFunc)(FrontierScriptContext *);

struct FrontierScriptContext {
    FrontierScriptManager *frontierSystem;
    u8 padding[4];
    u16 unk_08[8];
    u8 stackPointer;
    u8 state;
    const u8 *scriptPtr;
    const u8 *stack[20];
    const FrontierScrCmdFunc *cmdTable;
    u32 cmdTableSize;
    u16 data[4];
    MsgData *msgLoader;
    u8 *scripts;
    FrontierShouldResumeScriptFunc shouldResume;
    u8 unk_8C;
};

typedef struct FrontierMenuManager FrontierMenuManager;

struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MsgData *msgLoader;
    u8 *script;
    MessageFormat *strTemplate;
    String *string;
    String *fmtString;
    u8 printerID;
    s8 numSetContexts;
    u16 scene;
    u8 unused[5];
    u8 movementCount;
    u8 isMsgBoxOpen;
    int unused2;
    FrontierMenuManager *menuMan;
    Window msgWindow;
    ListMenu2D *menu;
    u16 *unused3;
    void *savingIcon;
    int unused4[10];
    Window *castleHostInfoWin;
    Window *castlePlayer2InfoWin;
    u16 unk_B0;
    u16 unk_B2;
    u16 unk_B4;
    u16 unk_B6;
    u16 *unk_B8;
};

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};


int BattleFactoryApp_Init(OverlayManager *appMan, int *state);

int BattleFactoryApp_Main(OverlayManager *appMan, int *state);

int BattleFactoryApp_Exit(OverlayManager *appMan, int *state);

enum MapHeaderID {
    MAP_HEADER_EVERYWHERE,
    MAP_HEADER_NOTHING,
    MAP_HEADER_UNDERGROUND,
    MAP_HEADER_JUBILIFE_CITY,
    MAP_HEADER_JUBILIFE_CITY_MART,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_1,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_1F,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_2F,
    MAP_HEADER_POKETCH_CO_1F,
    MAP_HEADER_POKETCH_CO_2F,
    MAP_HEADER_POKETCH_CO_3F,
    MAP_HEADER_JUBILIFE_TV_1F,
    MAP_HEADER_JUBILIFE_TV_2F,
    MAP_HEADER_JUBILIFE_TV_3F,
    MAP_HEADER_JUBILIFE_TV_4F,
    MAP_HEADER_JUBILIFE_TV_2F_GALLERY,
    MAP_HEADER_JUBILIFE_TV_3F_GLOBAL_RANKING_ROOM,
    MAP_HEADER_JUBILIFE_TV_3F_GROUP_RANKING_ROOM,
    MAP_HEADER_JUBILIFE_TV_ELEVATOR,
    MAP_HEADER_JUBILIFE_CITY_SOUTH_HOUSE_1F,
    MAP_HEADER_JUBILIFE_CITY_SOUTH_HOUSE_2F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_3F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_4F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_2,
    MAP_HEADER_JUBILIFE_CITY_CONDOMINIUMS_1F,
    MAP_HEADER_JUBILIFE_CITY_CONDOMINIUMS_2F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_3F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_4F,
    MAP_HEADER_GLOBAL_TERMINAL_1F,
    MAP_HEADER_TRAINERS_SCHOOL,
    MAP_HEADER_JUBILIFE_CITY_SOUTHWEST_HOUSE_1F,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_3,
    MAP_HEADER_UNUSED_JUBILIFE_CITY_HOUSE_4,
    MAP_HEADER_CANALAVE_CITY,
    MAP_HEADER_CANALAVE_CITY_MART,
    MAP_HEADER_CANALAVE_CITY_GYM,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_1F,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_2F,
    MAP_HEADER_CANALAVE_LIBRARY_1F,
    MAP_HEADER_CANALAVE_LIBRARY_2F,
    MAP_HEADER_CANALAVE_LIBRARY_3F,
    MAP_HEADER_CANALAVE_CITY_SOUTHEAST_HOUSE,
    MAP_HEADER_CANALAVE_CITY_EAST_HOUSE,
    MAP_HEADER_CANALAVE_CITY_HARBOR_INN,
    MAP_HEADER_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE,
    MAP_HEADER_OREBURGH_CITY,
    MAP_HEADER_OREBURGH_CITY_MART,
    MAP_HEADER_OREBURGH_CITY_GYM,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_1F,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_2F,
    MAP_HEADER_OREBURGH_CITY_NORTHWEST_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_NORTHWEST_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_3F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_4F,
    MAP_HEADER_OREBURGH_CITY_NORTH_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_NORTH_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTH_HOUSE_3F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_NORTH_HOUSE_4F,
    MAP_HEADER_OREBURGH_CITY_MIDDLE_HOUSE,
    MAP_HEADER_MINING_MUSEUM,
    MAP_HEADER_OREBURGH_CITY_WEST_HOUSE,
    MAP_HEADER_OREBURGH_CITY_EAST_HOUSE_1F,
    MAP_HEADER_OREBURGH_CITY_EAST_HOUSE_2F,
    MAP_HEADER_UNUSED_OREBURGH_CITY_EAST_HOUSE_3F,
    MAP_HEADER_OREBURGH_CITY_SOUTH_HOUSE,
    MAP_HEADER_ETERNA_CITY,
    MAP_HEADER_ETERNA_CITY_MART,
    MAP_HEADER_ETERNA_CITY_GYM,
    MAP_HEADER_ETERNA_CITY_DP_GYM,
    MAP_HEADER_ETERNA_CITY_POKECENTER_1F,
    MAP_HEADER_ETERNA_CITY_POKECENTER_2F,
    MAP_HEADER_CYCLE_SHOP,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_1F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_2F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_3F,
    MAP_HEADER_TEAM_GALACTIC_ETERNA_BUILDING_4F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_1F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_2F,
    MAP_HEADER_ETERNA_CITY_CONDOMINIUMS_3F,
    MAP_HEADER_UNUSED_ETERNA_CITY_CONDOMINIUMS_4F,
    MAP_HEADER_ROUTE_206_CYCLING_ROAD_NORTH_GATE,
    MAP_HEADER_ETERNA_CITY_HERB_SHOP,
    MAP_HEADER_ETERNA_CITY_SOUTH_HOUSE,
    MAP_HEADER_ETERNA_CITY_EAST_HOUSE,
    MAP_HEADER_ETERNA_CITY_UNDERGROUND_MAN_HOUSE,
    MAP_HEADER_UNUSED_ETERNA_CITY_HOUSE,
    MAP_HEADER_HEARTHOME_CITY,
    MAP_HEADER_HEARTHOME_CITY_MART,
    MAP_HEADER_HEARTHOME_CITY_GYM_ENTRANCE_ROOM,
    MAP_HEADER_HEARTHOME_CITY_GYM_TRAINER_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_GYM_TRAINER_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_GYM_LEADER_ROOM,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_1,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_2,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_3,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_4,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_5,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_TRAINER_ROOM_6,
    MAP_HEADER_HEARTHOME_CITY_DP_GYM_LEADER_ROOM,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_1F,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_2F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_1F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_2F,
    MAP_HEADER_HEARTHOME_CITY_SOUTHEAST_HOUSE_ELEVATOR,
    MAP_HEADER_HEARTHOME_CITY_POKEMON_FAN_CLUB,
    MAP_HEADER_HEARTHOME_CITY_WEST_GATE_TO_AMITY_SQUARE,
    MAP_HEADER_HEARTHOME_CITY_EAST_GATE_TO_AMITY_SQUARE,
    MAP_HEADER_ROUTE_208_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_ROUTE_209_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_ROUTE_212_GATE_TO_HEARTHOME_CITY,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_1F,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_2F,
    MAP_HEADER_HEARTHOME_CITY_NORTHEAST_HOUSE_ELEVATOR,
    MAP_HEADER_HEARTHOME_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_POFFIN_HOUSE,
    MAP_HEADER_CONTEST_HALL_LOBBY,
    MAP_HEADER_CONTEST_HALL_STAGE_ONGOING_CONTEST,
    MAP_HEADER_FOREIGN_BUILDING,
    MAP_HEADER_PASTORIA_CITY,
    MAP_HEADER_PASTORIA_CITY_MART,
    MAP_HEADER_PASTORIA_CITY_GYM,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_1F,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_2F,
    MAP_HEADER_PASTORIA_CITY_OBSERVATORY_GATE_1F,
    MAP_HEADER_PASTORIA_CITY_OBSERVATORY_GATE_2F,
    MAP_HEADER_PASTORIA_CITY_SOUTHWEST_HOUSE,
    MAP_HEADER_PASTORIA_CITY_MIDDLE_HOUSE,
    MAP_HEADER_PASTORIA_CITY_EAST_HOUSE,
    MAP_HEADER_PASTORIA_CITY_NORTH_HOUSE,
    MAP_HEADER_PASTORIA_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY,
    MAP_HEADER_VEILSTONE_CITY_GYM,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_1F,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_2F,
    MAP_HEADER_GAME_CORNER,
    MAP_HEADER_VEILSTONE_STORE_1F,
    MAP_HEADER_VEILSTONE_STORE_2F,
    MAP_HEADER_VEILSTONE_STORE_3F,
    MAP_HEADER_VEILSTONE_STORE_4F,
    MAP_HEADER_VEILSTONE_STORE_5F,
    MAP_HEADER_VEILSTONE_STORE_ELEVATOR,
    MAP_HEADER_VEILSTONE_CITY_GALACTIC_WAREHOUSE,
    MAP_HEADER_VEILSTONE_CITY_PRIZE_EXCHANGE,
    MAP_HEADER_VEILSTONE_CITY_SOUTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_VEILSTONE_CITY_SOUTHWEST_HOUSE,
    MAP_HEADER_ROUTE_215_GATE_TO_VEILSTONE_CITY,
    MAP_HEADER_SUNYSHORE_CITY,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_1F,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_2F,
    MAP_HEADER_SUNYSHORE_CITY_MART,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_1,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_2,
    MAP_HEADER_SUNYSHORE_CITY_GYM_ROOM_3,
    MAP_HEADER_SUNYSHORE_MARKET,
    MAP_HEADER_SUNYSHORE_CITY_NORTHEAST_HOUSE,
    MAP_HEADER_SUNYSHORE_CITY_WEST_HOUSE,
    MAP_HEADER_SUNYSHORE_CITY_NORTHWEST_HOUSE,
    MAP_HEADER_UNUSED_SUNYSHORE_CITY_HOUSE_1,
    MAP_HEADER_UNUSED_SUNYSHORE_CITY_HOUSE_2,
    MAP_HEADER_SUNYSHORE_CITY_EAST_HOUSE,
    MAP_HEADER_VISTA_LIGHTHOUSE,
    MAP_HEADER_SNOWPOINT_CITY,
    MAP_HEADER_SNOWPOINT_CITY_MART,
    MAP_HEADER_SNOWPOINT_CITY_GYM,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_1F,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_2F,
    MAP_HEADER_SNOWPOINT_CITY_WEST_HOUSE,
    MAP_HEADER_SNOWPOINT_CITY_EAST_HOUSE,
    MAP_HEADER_POKEMON_LEAGUE,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_1F,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_2F,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_1F,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_AARON_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_AARON_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_BERTHA_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_BERTHA_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_FLINT_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_FLINT_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_LUCIAN_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_LUCIAN_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_ELEVATOR_TO_CHAMPION_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_CHAMPION_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME,
    MAP_HEADER_POKEMON_LEAGUE_HALL_OF_FAME,
    MAP_HEADER_FIGHT_AREA,
    MAP_HEADER_FIGHT_AREA_POKECENTER_1F,
    MAP_HEADER_FIGHT_AREA_POKECENTER_2F,
    MAP_HEADER_FIGHT_AREA_MART,
    MAP_HEADER_UNUSED_BATTLE_PARK_GATE_TO_FIGHT_AREA,
    MAP_HEADER_ROUTE_225_GATE_TO_FIGHT_AREA,
    MAP_HEADER_FIGHT_AREA_MIDDLE_HOUSE,
    MAP_HEADER_FIGHT_AREA_SOUTH_HOUSE,
    MAP_HEADER_UNUSED_FIGHT_AREA_HOUSE,
    MAP_HEADER_UNKNOWN_197,
    MAP_HEADER_OREBURGH_MINE_B1F,
    MAP_HEADER_OREBURGH_MINE_B2F,
    MAP_HEADER_VALLEY_WINDWORKS_OUTSIDE,
    MAP_HEADER_VALLEY_WINDWORKS_BUILDING,
    MAP_HEADER_ETERNA_FOREST_OUTSIDE,
    MAP_HEADER_ETERNA_FOREST,
    MAP_HEADER_FUEGO_IRONWORKS_OUTSIDE,
    MAP_HEADER_FUEGO_IRONWORKS_BUILDING,
    MAP_HEADER_UNKNOWN_206,
    MAP_HEADER_MT_CORONET_1F_SOUTH,
    MAP_HEADER_MT_CORONET_2F,
    MAP_HEADER_MT_CORONET_3F,
    MAP_HEADER_MT_CORONET_OUTSIDE_NORTH,
    MAP_HEADER_MT_CORONET_OUTSIDE_SOUTH,
    MAP_HEADER_MT_CORONET_4F_ROOMS_1_AND_2,
    MAP_HEADER_MT_CORONET_4F_ROOM_3,
    MAP_HEADER_MT_CORONET_5F,
    MAP_HEADER_MT_CORONET_6F,
    MAP_HEADER_MT_CORONET_1F_TUNNEL_ROOM,
    MAP_HEADER_MT_CORONET_1F_NORTH_ROOM_2,
    MAP_HEADER_MT_CORONET_1F_NORTH_ROOM_1,
    MAP_HEADER_MT_CORONET_B1F,
    MAP_HEADER_SPEAR_PILLAR,
    MAP_HEADER_SPEAR_PILLAR_DISTORTED,
    MAP_HEADER_UNKNOWN_222,
    MAP_HEADER_PASTORIA_CITY_DP_GREAT_MARSH,
    MAP_HEADER_UNKNOWN_224,
    MAP_HEADER_SOLACEON_RUINS_MANIAC_TUNNEL_ROOM,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2_NORTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1_NORTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2,
    MAP_HEADER_SOLACEON_RUINS_ROOM_1_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3,
    MAP_HEADER_SOLACEON_RUINS_ROOM_2_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5_SOUTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3_NORTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_3_SOUTHWEST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_4,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5,
    MAP_HEADER_SOLACEON_RUINS_ROOM_7,
    MAP_HEADER_SOLACEON_RUINS_ROOM_4_SOUTHEAST_DEAD_END,
    MAP_HEADER_SOLACEON_RUINS_ROOM_6_NORTHWEST_DEAD_END,
    MAP_HEADER_UNKNOWN_243,
    MAP_HEADER_VICTORY_ROAD_1F,
    MAP_HEADER_VICTORY_ROAD_2F,
    MAP_HEADER_VICTORY_ROAD_B1F,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_2,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_1,
    MAP_HEADER_VICTORY_ROAD_1F_ROOM_3,
    MAP_HEADER_UNKNOWN_250,
    MAP_HEADER_PAL_PARK,
    MAP_HEADER_UNKNOWN_252,
    MAP_HEADER_AMITY_SQUARE,
    MAP_HEADER_RAVAGED_PATH,
    MAP_HEADER_UNKNOWN_255,
    MAP_HEADER_FLOAROMA_MEADOW,
    MAP_HEADER_FLOAROMA_MEADOW_HOUSE,
    MAP_HEADER_OREBURGH_GATE_1F,
    MAP_HEADER_OREBURGH_GATE_B1F,
    MAP_HEADER_FULLMOON_ISLAND,
    MAP_HEADER_FULLMOON_ISLAND_FOREST,
    MAP_HEADER_STARK_MOUNTAIN_OUTSIDE,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_1,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_2,
    MAP_HEADER_STARK_MOUNTAIN_ROOM_3,
    MAP_HEADER_UNKNOWN_266,
    MAP_HEADER_SENDOFF_SPRING,
    MAP_HEADER_TURNBACK_CAVE_ENTRANCE,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_ROOM,
    MAP_HEADER_TURNBACK_CAVE_GIRATINA_ROOM,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_3,
    MAP_HEADER_FLOWER_PARADISE,
    MAP_HEADER_UNKNOWN_275,
    MAP_HEADER_UNKNOWN_276,
    MAP_HEADER_UNKNOWN_277,
    MAP_HEADER_SNOWPOINT_TEMPLE_1F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B1F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B2F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B3F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B4F,
    MAP_HEADER_SNOWPOINT_TEMPLE_B5F,
    MAP_HEADER_WAYWARD_CAVE_1F,
    MAP_HEADER_WAYWARD_CAVE_B1F,
    MAP_HEADER_RUIN_MANIAC_CAVE_SHORT,
    MAP_HEADER_TROPHY_GARDEN,
    MAP_HEADER_IRON_ISLAND,
    MAP_HEADER_IRON_ISLAND_1F,
    MAP_HEADER_IRON_ISLAND_B1F_LEFT_ROOM,
    MAP_HEADER_IRON_ISLAND_B1F_RIGHT_ROOM,
    MAP_HEADER_IRON_ISLAND_B2F_RIGHT_ROOM,
    MAP_HEADER_IRON_ISLAND_B2F_LEFT_ROOM,
    MAP_HEADER_IRON_ISLAND_B3F,
    MAP_HEADER_OLD_CHATEAU,
    MAP_HEADER_OLD_CHATEAU_DINING_AREA,
    MAP_HEADER_OLD_CHATEAU_SIDE_ROOMS,
    MAP_HEADER_OLD_CHATEAU_CORRIDOR,
    MAP_HEADER_OLD_CHATEAU_BACK_WEST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_WEST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_MIDDLE_EAST_ROOM,
    MAP_HEADER_OLD_CHATEAU_BACK_EAST_ROOM,
    MAP_HEADER_UNKNOWN_304,
    MAP_HEADER_GALACTIC_HQ_1F,
    MAP_HEADER_GALACTIC_HQ_2F,
    MAP_HEADER_GALACTIC_HQ_3F,
    MAP_HEADER_GALACTIC_HQ_4F,
    MAP_HEADER_GALACTIC_HQ_B1F,
    MAP_HEADER_GALACTIC_HQ_B2F,
    MAP_HEADER_LAKE_VERITY_LOW_WATER,
    MAP_HEADER_LAKE_VERITY,
    MAP_HEADER_VERITY_CAVERN,
    MAP_HEADER_LAKE_VALOR_DRAINED,
    MAP_HEADER_LAKE_VALOR,
    MAP_HEADER_VALOR_CAVERN,
    MAP_HEADER_LAKE_ACUITY_LOW_WATER,
    MAP_HEADER_LAKE_ACUITY,
    MAP_HEADER_ACUITY_CAVERN,
    MAP_HEADER_NEWMOON_ISLAND,
    MAP_HEADER_NEWMOON_ISLAND_FOREST,
    MAP_HEADER_UNUSED_BATTLE_PARK,
    MAP_HEADER_UNUSED_BATTLE_PARK_EXCHANGE_SERVICE_CORNER,
    MAP_HEADER_UNKNOWN_324,
    MAP_HEADER_UNKNOWN_325,
    MAP_HEADER_BATTLE_TOWER,
    MAP_HEADER_BATTLE_TOWER_ELEVATOR,
    MAP_HEADER_BATTLE_TOWER_CORRIDOR,
    MAP_HEADER_BATTLE_TOWER_CORRIDOR_MULTI,
    MAP_HEADER_BATTLE_TOWER_BATTLE_ROOM,
    MAP_HEADER_BATTLE_TOWER_MULTI_BATTLE_ROOM,
    MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_2P,
    MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_4P,
    MAP_HEADER_VERITY_LAKEFRONT,
    MAP_HEADER_UNUSED_VERITY_LAKEFRONT_HOUSE,
    MAP_HEADER_VALOR_LAKEFRONT,
    MAP_HEADER_RESTAURANT,
    MAP_HEADER_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE,
    MAP_HEADER_GRAND_LAKE_VALOR_LAKEFRONT_WEST_HOUSE,
    MAP_HEADER_ACUITY_LAKEFRONT,
    MAP_HEADER_SPRING_PATH,
    MAP_HEADER_ROUTE_201,
    MAP_HEADER_ROUTE_202,
    MAP_HEADER_ROUTE_203,
    MAP_HEADER_ROUTE_204_SOUTH,
    MAP_HEADER_ROUTE_204_NORTH,
    MAP_HEADER_ROUTE_205_SOUTH,
    MAP_HEADER_ROUTE_205_HOUSE,
    MAP_HEADER_ROUTE_205_NORTH,
    MAP_HEADER_ROUTE_206,
    MAP_HEADER_ROUTE_206_CYCLING_ROAD_SOUTH_GATE,
    MAP_HEADER_UNUSED_GATE_BETWEEN_ETERNA_CITY_ROUTE_206,
    MAP_HEADER_ROUTE_207,
    MAP_HEADER_ROUTE_208,
    MAP_HEADER_ROUTE_208_HOUSE,
    MAP_HEADER_ROUTE_209,
    MAP_HEADER_ROUTE_209_LOST_TOWER_1F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_2F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_3F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_4F,
    MAP_HEADER_ROUTE_209_LOST_TOWER_5F,
    MAP_HEADER_ROUTE_210_SOUTH,
    MAP_HEADER_ROUTE_210_NORTH,
    MAP_HEADER_ROUTE_210_GRANDMA_WILMA_HOUSE,
    MAP_HEADER_ROUTE_211_WEST,
    MAP_HEADER_ROUTE_211_EAST,
    MAP_HEADER_ROUTE_212_NORTH,
    MAP_HEADER_POKEMON_MANSION,
    MAP_HEADER_POKEMON_MANSION_MAIDS_ROOM,
    MAP_HEADER_POKEMON_MANSION_OFFICE,
    MAP_HEADER_ROUTE_212_SOUTH,
    MAP_HEADER_ROUTE_212_HOUSE,
    MAP_HEADER_ROUTE_213,
    MAP_HEADER_ROUTE_213_GATE_TO_PASTORIA_CITY,
    MAP_HEADER_FOOTSTEP_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_LOBBY,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_EAST_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE,
    MAP_HEADER_GRAND_LAKE_ROUTE_213_NORTHEAST_HOUSE,
    MAP_HEADER_ROUTE_214,
    MAP_HEADER_ROUTE_214_GATE_TO_VEILSTONE_CITY,
    MAP_HEADER_ROUTE_215,
    MAP_HEADER_ROUTE_216,
    MAP_HEADER_ROUTE_216_HOUSE,
    MAP_HEADER_ROUTE_217,
    MAP_HEADER_ROUTE_217_WEST_HOUSE,
    MAP_HEADER_ROUTE_217_NORTHEAST_HOUSE,
    MAP_HEADER_ROUTE_218,
    MAP_HEADER_ROUTE_218_GATE_TO_JUBILIFE_CITY,
    MAP_HEADER_ROUTE_218_GATE_TO_CANALAVE_CITY,
    MAP_HEADER_ROUTE_219,
    MAP_HEADER_ROUTE_221,
    MAP_HEADER_PAL_PARK_LOBBY,
    MAP_HEADER_ROUTE_221_HOUSE,
    MAP_HEADER_ROUTE_222,
    MAP_HEADER_ROUTE_222_WEST_HOUSE,
    MAP_HEADER_ROUTE_222_EAST_HOUSE,
    MAP_HEADER_ROUTE_222_GATE_TO_SUNYSHORE_CITY,
    MAP_HEADER_ROUTE_224,
    MAP_HEADER_ROUTE_225,
    MAP_HEADER_UNKNOWN_401,
    MAP_HEADER_UNKNOWN_402,
    MAP_HEADER_ROUTE_227,
    MAP_HEADER_UNKNOWN_404,
    MAP_HEADER_UNKNOWN_405,
    MAP_HEADER_ROUTE_228,
    MAP_HEADER_ROUTE_229,
    MAP_HEADER_UNKNOWN_408,
    MAP_HEADER_UNKNOWN_409,
    MAP_HEADER_RECORD_MIXING_ROOM,
    MAP_HEADER_TWINLEAF_TOWN,
    MAP_HEADER_TWINLEAF_TOWN_RIVAL_HOUSE_1F,
    MAP_HEADER_TWINLEAF_TOWN_RIVAL_HOUSE_2F,
    MAP_HEADER_TWINLEAF_TOWN_PLAYER_HOUSE_1F,
    MAP_HEADER_TWINLEAF_TOWN_PLAYER_HOUSE_2F,
    MAP_HEADER_TWINLEAF_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_TWINLEAF_TOWN_SOUTHWEST_HOUSE,
    MAP_HEADER_SANDGEM_TOWN,
    MAP_HEADER_SANDGEM_TOWN_MART,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_1F,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_2F,
    MAP_HEADER_SANDGEM_TOWN_POKEMON_RESEARCH_LAB,
    MAP_HEADER_SANDGEM_TOWN_COUNTERPART_HOUSE_1F,
    MAP_HEADER_SANDGEM_TOWN_COUNTERPART_HOUSE_2F,
    MAP_HEADER_SANDGEM_TOWN_HOUSE,
    MAP_HEADER_FLOAROMA_TOWN,
    MAP_HEADER_FLOAROMA_TOWN_MART,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_1F,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_2F,
    MAP_HEADER_FLOWER_SHOP,
    MAP_HEADER_FLOAROMA_TOWN_SOUTHEAST_HOUSE,
    MAP_HEADER_FLOAROMA_TOWN_MIDDLE_HOUSE,
    MAP_HEADER_SOLACEON_TOWN,
    MAP_HEADER_SOLACEON_TOWN_MART,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_1F,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_2F,
    MAP_HEADER_POKEMON_DAY_CARE,
    MAP_HEADER_SOLACEON_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_SOLACEON_TOWN_POKEMON_NEWS_PRESS,
    MAP_HEADER_SOLACEON_TOWN_NORTH_HOUSE,
    MAP_HEADER_SOLACEON_TOWN_EAST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_1F,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_2F,
    MAP_HEADER_CELESTIC_TOWN_NORTH_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_NORTHWEST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_NORTHEAST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_SOUTHWEST_HOUSE,
    MAP_HEADER_CELESTIC_TOWN_CAVE,
    MAP_HEADER_SURVIVAL_AREA,
    MAP_HEADER_SURVIVAL_AREA_MART,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_1F,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_2F,
    MAP_HEADER_BATTLEGROUND,
    MAP_HEADER_SURVIVAL_AREA_SOUTH_HOUSE,
    MAP_HEADER_SURVIVAL_AREA_NORTH_HOUSE,
    MAP_HEADER_RESORT_AREA,
    MAP_HEADER_UNUSED_RESORT_AREA_MART,
    MAP_HEADER_RESORT_AREA_POKECENTER_1F,
    MAP_HEADER_RESORT_AREA_POKECENTER_2F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_1F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_2F,
    MAP_HEADER_RESORT_AREA_RIBBON_SYNDICATE_ELEVATOR,
    MAP_HEADER_VILLA,
    MAP_HEADER_RESORT_AREA_HOUSE,
    MAP_HEADER_UNION_ROOM,
    MAP_HEADER_ROUTE_220,
    MAP_HEADER_ROUTE_223,
    MAP_HEADER_ROUTE_226,
    MAP_HEADER_UNKNOWN_470,
    MAP_HEADER_ROUTE_230,
    MAP_HEADER_SEABREAK_PATH,
    MAP_HEADER_UNKNOWN_473,
    MAP_HEADER_JUBILIFE_CITY_POKECENTER_B1F,
    MAP_HEADER_CANALAVE_CITY_POKECENTER_B1F,
    MAP_HEADER_OREBURGH_CITY_POKECENTER_B1F,
    MAP_HEADER_ETERNA_CITY_POKECENTER_B1F,
    MAP_HEADER_HEARTHOME_CITY_POKECENTER_B1F,
    MAP_HEADER_PASTORIA_CITY_POKECENTER_B1F,
    MAP_HEADER_VEILSTONE_CITY_POKECENTER_B1F,
    MAP_HEADER_SUNYSHORE_CITY_POKECENTER_B1F,
    MAP_HEADER_SNOWPOINT_CITY_POKECENTER_B1F,
    MAP_HEADER_POKEMON_LEAGUE_SOUTH_POKECENTER_B1F,
    MAP_HEADER_FIGHT_AREA_POKECENTER_B1F,
    MAP_HEADER_SANDGEM_TOWN_POKECENTER_B1F,
    MAP_HEADER_FLOAROMA_TOWN_POKECENTER_B1F,
    MAP_HEADER_SOLACEON_TOWN_POKECENTER_B1F,
    MAP_HEADER_CELESTIC_TOWN_POKECENTER_B1F,
    MAP_HEADER_SURVIVAL_AREA_POKECENTER_B1F,
    MAP_HEADER_RESORT_AREA_POKECENTER_B1F,
    MAP_HEADER_CANALAVE_CITY_WEST_HOUSE,
    MAP_HEADER_CAFE,
    MAP_HEADER_BATTLE_TOWER_BATTLE_SALON,
    MAP_HEADER_GALACTIC_HQ_CONTROL_ROOM,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_2F,
    MAP_HEADER_POKEMON_LEAGUE_NORTH_POKECENTER_B1F,
    MAP_HEADER_GALACTIC_HQ_LABORATORY,
    MAP_HEADER_ROUTE_225_HOUSE,
    MAP_HEADER_ROUTE_226_HOUSE,
    MAP_HEADER_ROUTE_227_HOUSE,
    MAP_HEADER_ROUTE_228_GATE_TO_ROUTE_226,
    MAP_HEADER_ROUTE_228_NORTH_HOUSE,
    MAP_HEADER_ROUTE_228_SOUTH_HOUSE,
    MAP_HEADER_GREAT_MARSH_1,
    MAP_HEADER_GREAT_MARSH_2,
    MAP_HEADER_GREAT_MARSH_3,
    MAP_HEADER_GREAT_MARSH_4,
    MAP_HEADER_GREAT_MARSH_5,
    MAP_HEADER_GREAT_MARSH_6,
    MAP_HEADER_HALL_OF_ORIGIN,
    MAP_HEADER_UNKNOWN_511,
    MAP_HEADER_RUIN_MANIAC_CAVE_LONG,
    MAP_HEADER_MANIAC_TUNNEL,
    MAP_HEADER_IRON_ISLAND_HOUSE,
    MAP_HEADER_SOLACEON_RUINS_ROOM_5_SOUTHEAST_DEADEND,
    MAP_HEADER_VISTA_LIGHTHOUSE_ELEVATOR,
    MAP_HEADER_JUBILIFE_CITY_SOUTHWEST_HOUSE_2F,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_1_ROOM_6,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_3,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_2_ROOM_6,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_1,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_2,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_3,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_4,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_5,
    MAP_HEADER_TURNBACK_CAVE_PILLAR_3_ROOM_6,
    MAP_HEADER_UNKNOWN_533,
    MAP_HEADER_UNKNOWN_534,
    MAP_HEADER_UNKNOWN_535,
    MAP_HEADER_UNKNOWN_536,
    MAP_HEADER_UNKNOWN_537,
    MAP_HEADER_UNKNOWN_538,
    MAP_HEADER_UNKNOWN_539,
    MAP_HEADER_UNKNOWN_540,
    MAP_HEADER_UNKNOWN_541,
    MAP_HEADER_UNKNOWN_542,
    MAP_HEADER_UNKNOWN_543,
    MAP_HEADER_UNKNOWN_544,
    MAP_HEADER_UNKNOWN_545,
    MAP_HEADER_UNKNOWN_546,
    MAP_HEADER_UNKNOWN_547,
    MAP_HEADER_UNKNOWN_548,
    MAP_HEADER_UNKNOWN_549,
    MAP_HEADER_UNKNOWN_550,
    MAP_HEADER_UNKNOWN_551,
    MAP_HEADER_UNKNOWN_552,
    MAP_HEADER_UNKNOWN_553,
    MAP_HEADER_UNKNOWN_554,
    MAP_HEADER_UNKNOWN_555,
    MAP_HEADER_UNKNOWN_556,
    MAP_HEADER_UNKNOWN_557,
    MAP_HEADER_CONTEST_HALL_STAGE_NO_CONTEST,
    MAP_HEADER_BATTLE_FRONTIER,
    MAP_HEADER_BATTLE_FRONTIER_GATE_TO_FIGHT_AREA,
    MAP_HEADER_UNKNOWN_561,
    MAP_HEADER_BATTLE_FACTORY,
    MAP_HEADER_BATTLE_HALL,
    MAP_HEADER_BATTLE_CASTLE,
    MAP_HEADER_BATTLE_ARCADE,
    MAP_HEADER_VEILSTONE_STORE_B1F,
    MAP_HEADER_GLOBAL_TERMINAL_2F,
    MAP_HEADER_GLOBAL_TERMINAL_3F,
    MAP_HEADER_GALACTIC_HQ_HALL,
    MAP_HEADER_UNKNOWN_570,
    MAP_HEADER_ROTOMS_ROOM,
    MAP_HEADER_UNKNOWN_572,
    MAP_HEADER_DISTORTION_WORLD_1F,
    MAP_HEADER_DISTORTION_WORLD_B1F,
    MAP_HEADER_DISTORTION_WORLD_B2F,
    MAP_HEADER_DISTORTION_WORLD_B3F,
    MAP_HEADER_DISTORTION_WORLD_B4F,
    MAP_HEADER_UNKNOWN_578,
    MAP_HEADER_DISTORTION_WORLD_B5F,
    MAP_HEADER_DISTORTION_WORLD_B6F,
    MAP_HEADER_DISTORTION_WORLD_B7F,
    MAP_HEADER_DISTORTION_WORLD_GIRATINA_ROOM,
    MAP_HEADER_DISTORTION_WORLD_TURNBACK_CAVE_ROOM,
    MAP_HEADER_SPEAR_PILLAR_DIALGA,
    MAP_HEADER_SPEAR_PILLAR_PALKIA,
    MAP_HEADER_WIFI_PLAZA_ENTRANCE,
    MAP_HEADER_IRON_ISLAND_IRON_RUINS,
    MAP_HEADER_IRON_RUINS,
    MAP_HEADER_MT_CORONET_ICEBERG_RUINS,
    MAP_HEADER_ICEBERG_RUINS,
    MAP_HEADER_ROUTE_228_ROCK_PEAK_RUINS,
    MAP_HEADER_ROCK_PEAK_RUINS,

    MAP_HEADER_INVALID,
    MAP_HEADER_DYNAMIC = 4095,
};

typedef struct BagCursor BagCursor;

typedef struct Options Options;

void *Heap_Alloc(u32 heapID, u32 size);

typedef struct JournalEntry JournalEntry;

typedef struct PalPad {
    u16 trainerName[7 + 1];
    u32 trainerId;
    u8 language;
    u8 gameCode;
    u8 gender;
    u8 padding_17;

    u32 associatedTrainerIds[16];
    u8 associatedTrainerGameCodes[16];
    u8 associatedTrainerLanguages[16];
    u8 associatedTrainerGenders[16];
} PalPad;

typedef struct FieldBattleDTO FieldBattleDTO;

typedef struct BattleFactorySave BattleFactorySave;

typedef struct Camera Camera;

typedef void (*BattleFrontierSubAppCallback)(void *);

FieldFrontierDTO *Frontier_GetLaunchArgs(BattleFrontier *frontier);

void *Frontier_GetData(BattleFrontier *frontier);

void Frontier_LaunchApplication(BattleFrontier *frontier, const OverlayManagerTemplate *appTemplate, void *appArgs, BOOL freeArgsAfter, BattleFrontierSubAppCallback finishCallback);

typedef struct Location {
    enum MapHeaderID mapHeaderID;
    int warpId;
    int x;
    int z;
    int faceDirection;
} Location;

static inline void Location_Set(Location *location, enum MapHeaderID mapHeaderID, int warpId, int x, int z, int faceDirection) {
    location->mapHeaderID = mapHeaderID;
    location->warpId = warpId;
    location->x = x;
    location->z = z;
    location->faceDirection = faceDirection;
}

typedef struct BattleTower BattleTower;

typedef struct BattleRegulation BattleRegulation;

typedef struct TaskManager TaskManager;

typedef BOOL (*TaskFunc)(TaskManager *);

struct TaskManager {
    TaskManager *prev;
    TaskFunc func;
    int state;
    void *env;
    int dummy10;
    void *dummy14;
    FieldSystem *fieldSys;
    int *dummy1C;
};

typedef struct PokemonSummary PokemonSummary;

typedef struct MapObjectManager MapObjectManager;

typedef struct MapLoadMode MapLoadMode;

typedef struct WiFiList WiFiList;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_02095E80_sub1;

typedef struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
} TradeRoomMonDisplayData;

typedef struct TradeRoomArgs TradeRoomArgs;

typedef struct TradeRoom TradeRoom;

struct TradeRoom {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    TradeRoomArgs *args;
    PokemonSummaryArgs summaryArgs;
    int summarySide;
    OverlayManager *appMan;
    int subAppActive;
    int phase;
    int connectStep;
    int partySendCount;
    int commMilestone;
    int partyReceiveCount;
    int didConfirmTrade;
    int partnerSyncStatus[2];
    int notificationMsgId;
    BOOL unk_6C;
    UnkStruct_02095E80_sub1 unk_70;
    int selectedSlot[2];
    u16 cursorGlowAngle;
    u16 unk_92[17][5];
    u16 unk_13C;
    u16 unk_13E;
    int subStepResult;
    u16 unk_144[3];
    u8 pad_156[2];
    u32 pendingDirection[2];
    u32 unk_154[3][2];
    int unk_16C;
    int unk_170;
    MessageFormat *bgConfig;
    MsgData *strTemplate;
    String *strTemplate2;
    String *unread_180;
    String *msgLoader;
    SpriteList *unk_188;
    struct String *playerNameStr;
    struct String *itemLabelStr;
    struct SpriteList *spriteList;
    G2dRenderer spriteRenderer;
    struct SpriteResourceCollection *spriteResourceCollections[4];
    struct SpriteResource *spriteResources[4][2];
    struct { struct NNSG2dImageProxy *imageProxy; struct NNSG2dCharacterData *charData; struct NNSG2dImagePaletteProxy *paletteProxy; struct NNSG2dCellDataBank *cellBank; struct NNSG2dAnimBankData *cellAnimBank; struct NNSG2dMultiCellDataBank *multiCellBank; struct NNSG2dAnimBankData *multiCellAnimBank; BOOL isVRamTransfer; u8 priority; u8 padding_21[3]; } mainSpriteResourcesHeader;
    struct { struct NNSG2dImageProxy *imageProxy; struct NNSG2dCharacterData *charData; struct NNSG2dImagePaletteProxy *paletteProxy; struct NNSG2dCellDataBank *cellBank; struct NNSG2dAnimBankData *cellAnimBank; struct NNSG2dMultiCellDataBank *multiCellBank; struct NNSG2dAnimBankData *multiCellAnimBank; BOOL isVRamTransfer; u8 priority; u8 padding_21[3]; } subSpriteResourcesHeader;
    struct Sprite *cursorSprites[14];
    struct Sprite *iconSprites[12];
    struct Sprite *itemIconSprites[12];
    struct Sprite *capsuleIconSprites[12];
    struct Sprite *detailCardSprites[6];
    struct Sprite *convergeIndicatorSprite;
    struct SysTask *unk_480[7];
    Window windows[34];
    struct ListMenu2D *yesNoMenu;
    struct ListMenuItem *actionMenuChoices;
    struct ListMenu2D *actionMenu;
    int yesNoMenuStep;
    int unk_6CC;
    int unk_6D0;
    int unk_6D4;
    VecFx32 unk_6D8[2];
    int unk_6F0;
    struct { u16 species; u16 heldItemType; u8 pokeBall; u8 isEgg; u8 form; u8 unk_8_pad[1]; u16 gender; u16 iconShouldFlip; int ballCapsuleId; } monDisplayData[13];
    int unk_7B8;
    int unk_7BC;
    int unk_7C0;
    int unk_7C4[7];
    void *unk_7E0;
    struct NNSG2dCharacterData *unk_7E4;
    void *iconGraphicsBuffers[12];
    struct NNSG2dCharacterData *iconCharacterData[12];
    u8 previewSpritePixelBuf[3200][2];
    struct { u16 narcID; u16 character; u16 palette; u16 spindaSpots; u8 dummy; u8 unk_C_pad[3]; u32 personality; } previewSpriteTemplate[2];
    int pendingPreviewLoadSide;
    u8 pad_2120[244];
    void *unk_2214;
    Party *unk_2218;
    Party *unk_221C;
    ChatotCry *subStepCallback;
    PalPad *playerParty;
    struct Party *partnerParty;
    struct ChatotCry *unk_2278;
    struct PalPad *palPad;
    PalPad palPadNetworkObject;
    int unk_2308;
    int unk_230C;
    int bgScrollOffset;
    int slideAnimFrame;
    int staggerCountdown;
    VecFx32 playerSlidePath[2];
    VecFx32 partnerSlidePath[2];
    u8 partyTransferBuffer[1424][2];
    u8 chatotCryBuffer[1004][2];
    int pendingRegistrationFlags[32];
    int registrationTargetNetId;
    int friendToDeleteSlot;
    ListMenu *palPadStrTemplate;
    struct MsgData *palPadMsgLoader;
    struct ListMenuItem *friendListChoices;
    struct ListMenu *friendListMenu;
    Window friendListWindow;
    struct WiFiList *wifiList;
    int tradeCompletedCount;
    int syncSaveState;
    int lastSyncedValue;
    int lastSyncedCmd;
    void *waitDialHandle;
    int connectionTimeoutArmed;
    int connectionTimeoutFrames;
};

typedef struct MapPropAnimationManager MapPropAnimationManager;

typedef struct MapPropOneShotAnimationManager MapPropOneShotAnimationManager;

typedef struct AreaDataManager AreaDataManager;

typedef struct ModelAttributes ModelAttributes;

typedef struct AreaLightManager AreaLightManager;

typedef struct DynamicTerrainHeightManager DynamicTerrainHeightManager;

typedef struct FieldEffectManager FieldEffectManager;

typedef struct FogManager FogManager;

typedef struct LandDataManager LandDataManager;

typedef struct MapPropManager MapPropManager;

typedef struct UnkStruct_ov5_021EB0E0 UnkStruct_ov5_021EB0E0;

typedef struct Signpost Signpost;

typedef struct MapHeaderData MapHeaderData;

typedef struct MapMatrix MapMatrix;

typedef struct PlayerAvatar PlayerAvatar;

typedef struct PokedexMemory PokedexMemory;

typedef struct RadarChain RadarChain;

typedef struct TerrainAttributes TerrainAttributes;

typedef struct TerrainCollisionManager TerrainCollisionManager;

typedef struct TrainerCase TrainerCase;

enum UnionRoomConnectState {
    UR_CONNECT_STATE_NONE = 0,
    UR_CONNECT_STATE_CONNECTED,
    UR_CONNECT_STATE_FAILED,
    UR_CONNECT_STATE_PEER_CONTACTED,
};

enum UnionRoomPendingConnection {
    UR_PENDING_CONNECTION_NONE = 0,
    UR_PENDING_CONNECTION_UNION,
    UR_PENDING_CONNECTION_DRAW,
};

typedef struct UnionRoomManager UnionRoomManager;

typedef void (*UnionRoomTask)(UnionRoomManager *);

struct UnionRoomManager {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    PlayerProfile *trainerInfo;
    SysTask *sysTask;
    UnionRoomTask task;
    int timer;
    u32 connectionID;
    enum UnionRoomConnectState connectState;
    enum UnionRoomPendingConnection pendingConnection;
    int unused_24;
    int unused_28;
    int unused_2C;
    u32 peerActivity;
    u32 proposedActivity;
    u32 unused_38;
    BOOL answerDeclined;
    u32 activityAnswer;
    BOOL chatEnded;
    u16 unused_48[10][2];
    u16 unused_70[40][2];
    WMbssDesc *bssDesc[16];
    MATHRandContext32 rand;
    int unused_168;
    int unused_16C;
    int unused_170;
    u16 unused_174;
    u8 partySelectionResults[2];
    MailMessage pendingSentence;
    BOOL sentencePending;
    TrainerCase *trainerCase;
    TrainerCase *receivedTrainerCases[2];
};

typedef struct HoneyTreeShakeList HoneyTreeShakeList;

typedef struct FieldWildBattleMetadata {
    u16 encounterAttempts;
    u16 wildMonDefeated;
} FieldWildBattleMetadata;

FS_EXTERN_OVERLAY(OVY_81);

void ov80_0222F7CC(void *data);

BOOL FrtCmd_095(FrontierScriptContext *ctx);

BOOL FrtCmd_095(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    FS_EXTERN_OVERLAY(battle_factory_app);

    extern const OverlayManagerTemplate ov80_0223BDB4;

    BattleFactory *battleFactory = Frontier_GetData(ctx->frontierSystem->frontier);
    BattleFactoryAppArgs *args = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleFactoryAppArgs));

    MI_CpuClear8(args, sizeof(BattleFactoryAppArgs));

    args->saveData = fieldData->saveData;
    args->challengeType = battleFactory->challengeType;
    args->isOpenLevel = battleFactory->isOpenLevel;
    args->isExchangeMode = FALSE;
    args->personalParty = battleFactory->playersParty;
    args->receivableParty = battleFactory->opponentsParty;
    args->battleFactory = battleFactory;

    Frontier_LaunchApplication(ctx->frontierSystem->frontier, &ov80_0223BDB4, args, 0, ov80_0222F7CC);
    return TRUE;
}
