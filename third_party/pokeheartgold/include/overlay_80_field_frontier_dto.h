#ifndef POKEHEARTGOLD_OVERLAY_80_FIELD_FRONTIER_DTO_H
#define POKEHEARTGOLD_OVERLAY_80_FIELD_FRONTIER_DTO_H

// Launch arguments of the Battle Frontier app (allocated in ScrCmd_627 / sub_020968B0).

struct BagCursor;
struct FieldSystem;
struct Options;
struct SaveData;

typedef struct FieldFrontierDTO {
    void *facilityData;
    struct Options *options;
    struct SaveData *saveData;
    struct BagCursor *bagCursor;
    u32 unk_10;
    u32 unk_14;
    u32 mapHeaderID;
    void *subscreenCursorOn;
    u8 sceneID;
    struct FieldSystem *fieldSystem;
} FieldFrontierDTO;

// sizeof(FieldFrontierDTO) == 0x28

#endif // POKEHEARTGOLD_OVERLAY_80_FIELD_FRONTIER_DTO_H
