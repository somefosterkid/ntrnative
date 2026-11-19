#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "pm_string.h"

struct BattleFrontier;
struct BattleTowerSave;
struct FieldBattleDTO;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct Menu;
struct MessageLoader;
struct StringTemplate;
struct WifiBattleTowerRecord;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct MessageLoader MessageLoader;
typedef struct Menu Menu;
enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};
typedef struct StringTemplate StringTemplate;
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
    MessageLoader *msgLoader;
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
    MessageLoader *msgLoader;
    u8 *script;
    StringTemplate *strTemplate;
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
    Menu *menu;
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
typedef struct FieldBattleDTO FieldBattleDTO;
typedef struct FrontierTrainer {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
} FrontierTrainer;
typedef struct FrontierPokemon {
    struct {
        u16 species : 11;
        u16 form : 5;
    };
    u16 item;
    u16 moves[4];
    u32 otID;
    u32 personality;
    union {
        struct {
            u32 hpIV : 5;
            u32 atkIV : 5;
            u32 defIV : 5;
            u32 speedIV : 5;
            u32 spAtkIV : 5;
            u32 spDefIV : 5;
            u32 setSpeciesAsNickname : 1;
            u32 : 1;
        };
        u32 combinedIVs;
    };
    union {
        struct {
            u8 hpEV;
            u8 atkEV;
            u8 defEV;
            u8 speedEV;
            u8 spAtkEV;
            u8 spDefEV;
        };
        u8 evList[STAT_MAX];
    };
    union {
        struct {
            u8 move0PPUps : 2;
            u8 move1PPUps : 2;
            u8 move2PPUps : 2;
            u8 move3PPUps : 2;
        };
        u8 combinedPPUps;
    };
    u8 language;
    u8 ability;
    u8 friendship;
    u16 nickname[10 + 1];
} FrontierPokemon;
typedef struct FrontierOpponent {
    FrontierTrainer trainer;
    FrontierPokemon pokemon[4];
} FrontierOpponent;
typedef struct WifiBattleTowerRecord WifiBattleTowerRecord;
typedef struct BattleTowerPartnerData {
    u32 otID;
    u16 monSetIDs[2];
    u32 personalities[2];
} BattleTowerPartnerData;
typedef struct BattleTowerSave BattleTowerSave;
typedef struct BattleTower {
    int sentinel;
    enum HeapID heapID;
    u32 rng;
    u8 nextOpponentNum;
    u8 currentBattle;
    u8 partySize;
    u8 challengeMode;
    u8 defeatedSevenTrainers : 1;
    u8 beatPalmer : 2;
    u8 isRetiring : 1;
    u8 ribbonsEarned : 1;
    u8 partnerID : 3;
    u8 playerGender;
    u8 partnerGender;
    u16 unk_14;
    u16 unk_16[2];
    u16 startingStreak;
    u16 roomNum;
    u32 victoryRecord;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u8 partySlots[4];
    u16 selectedSpecies[4];
    u16 selectedItems[4];
    u16 trainerIDs[7 * 2];
    Location unused;
    BattleTowerSave *towerSave;
    WifiBattleTowerRecord *wifiRecord;
    FrontierOpponent opponents[2];
    FrontierOpponent partners[5];
    BattleTowerPartnerData unk_7E8[5];
    u8 partnerConflicts[5];
    u16 commPayload[35];
    u16 commBuffer[35];
    int wonBattle;
    FieldBattleDTO *dto;
    u8 msgsReceived;
    u8 unk_8D5;
    u8 unk_8D6;
    u8 unused2;
    u16 unk_8D8;
    u16 unk_8DA;
} BattleTower;
void *Frontier_GetData(BattleFrontier *frontier);

BOOL ov80_02235F90(FrontierScriptContext *ctx);

BOOL ov80_02235F90(FrontierScriptContext *ctx) {
    BattleTower *battleTower = Frontier_GetData(ctx->frontierSystem->frontier);

    if (battleTower->msgsReceived < 2) {
        return 0;
    }

    battleTower->msgsReceived = 0;
    return 1;
}
