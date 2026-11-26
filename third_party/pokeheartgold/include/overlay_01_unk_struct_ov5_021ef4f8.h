#ifndef POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021EF4F8_H
#define POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021EF4F8_H

// Per-scanline (HBlank) effect object of overlay_01_021FB5D4 (ov01_021FB5D4 allocates it, ov01_021FB82C stops it).

#include "global.h"

struct BufferManager;
struct HBlankSystem;
struct HBlankTask;
struct SysTask;

typedef struct UnkStruct_ov5_021EF4F8_t {
    u32 unk_00;
    int unk_04;
    int unk_08;
    u16 unk_0C[192];
    u16 unk_18C[192];
    struct BufferManager *bufferManager;
    struct HBlankSystem *unk_310;
    struct HBlankTask *unk_314;
    BOOL unk_318;
    struct SysTask *unk_31C;
    struct SysTask *unk_320;
    struct SysTask *unk_324;
} UnkStruct_ov5_021EF4F8;

#endif // POKEHEARTGOLD_OVERLAY_01_UNK_STRUCT_OV5_021EF4F8_H
