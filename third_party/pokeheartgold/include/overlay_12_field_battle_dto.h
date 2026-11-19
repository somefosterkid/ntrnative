#ifndef POKEHEARTGOLD_OVERLAY_12_FIELD_BATTLE_DTO_H
#define POKEHEARTGOLD_OVERLAY_12_FIELD_BATTLE_DTO_H

// FieldBattleDTO: merged by tools/hg/promote.py from 11 m2c local copies (1 variants, base
// src/overlay_12_022378C0_02237D00.c) and 22 accesses; fields at every accessed offset, pads elsewhere.

struct Bag;
struct BagCursor;
struct CaptureAttempt;
struct ChatotCry;
struct GameRecords;
struct JournalEntry;
struct Options;
struct PCBoxes;
struct PalPad;
struct Party;
struct Pokedex;
struct Poketch;
struct SaveData;
struct TrainerInfo;
struct WiFiHistory;

typedef struct FieldBattleDTO {
    u32 battleType;
    struct Party *parties[4];
    int resultMask;
    int trainerIDs[4];
    struct { struct { u8 monDataType; u8 trainerType; u8 sprite; u8 partySize; u16 items[4]; u32 aiMask; u32 battleType; } header; u16 name[8]; struct { u16 type; u16 id; u16 words[2]; } winMsg; struct { u16 type; u16 id; u16 words[2]; } loseMsg; } trainer[4];
    struct TrainerInfo *trainerInfo[4];
    struct Bag *bag;
    struct BagCursor *bagCursor;
    struct Pokedex *pokedex;
    struct PCBoxes *pcBoxes;
    struct ChatotCry *chatotCries[4];
    struct Poketch *poketch;
    struct WiFiHistory *wiFiHistory;
    struct Options *options;
    struct CaptureAttempt *captureAttempt;
    struct { int totalTurns; int totalFainted; int totalDamage; } battleRecords;
    struct GameRecords *records;
    struct PalPad *palPad; // 0x148: HGSS has no journal here (BattleSetup.palPad)
    u32 battleBg;          // 0x14c: BattleSetup.battleBg, a scalar (Platinum's palPad pointer moved every later PC offset)
    u32 background;
    u32 terrain;
    int mapLabelTextID;
    int mapHeaderID;
    u32 timeOfDay;
    u32 mapEvolutionMethod;
    BOOL visitedContestHall;
    BOOL metBebe;
    int caughtBattlerIdx;
    int fieldWeather;
    int leveledUpMonsMask;
    u32 systemVersion[4];
    u32 battleStatusMask;
    int countSafariBalls;
    u32 rulesetMask;
    void *postBattleData;
    u32 seed;
    int linkPlayerPositions[4];
    u16 networkID;
    u8 recordingType;
    u8 dummy18B;
    int totalTurnsElapsed;
    u8 *subscreenCursorOn;
    u8 recordedChatter[4];
    struct SaveData *saveData;
    u32 recordingStopped;
} FieldBattleDTO;

// sizeof(FieldBattleDTO) == 0x1c8

#endif // POKEHEARTGOLD_OVERLAY_12_FIELD_BATTLE_DTO_H
