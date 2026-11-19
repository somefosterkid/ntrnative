#ifndef POKEHEARTGOLD_OVERLAY_01_SAVE_INFO_H
#define POKEHEARTGOLD_OVERLAY_01_SAVE_INFO_H

// SaveInfo: merged by tools/hg/promote.py from 6 m2c local copies (1 variants, base
// src/overlay_01_021F3F50_021F3F50.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

#include "constants/heap.h"

struct PlayTime;
struct TrainerInfo;

typedef struct SaveInfo {
    int pokedexCount;
    int mapLabelTextID;
    struct TrainerInfo *trainerInfo;
    struct PlayTime *playTime;
} SaveInfo;

// sizeof(SaveInfo) == 0x10 on the DS

// The save information box (SaveInfoWindow_New = ov01_021F4360, its Print / RemoveFromScreen / Delete helpers and the
// touch save app's ov01_021F42F8 / ov01_021F426C / ov01_021F434C). Every pointer-typed field moves on the host, so the
// users read the box by field only.
struct FieldSystem;
struct BgConfig;
struct Window;
struct MessageFormat;
struct MsgData;

typedef struct SaveInfoWindow {
    struct FieldSystem *fieldSystem;
    enum HeapID heapID;
    u8 bgLayer;
    struct BgConfig *bgConfig;
    struct Window *window;
    struct MessageFormat *strTemplate;
    struct MsgData *msgLoader;
    SaveInfo saveInfo;
    int width;
    int height;
} SaveInfoWindow; // size 0x34 on the DS

#endif // POKEHEARTGOLD_OVERLAY_01_SAVE_INFO_H
