#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_DEE8_T_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_DEE8_T_H

#include "global.h"

#include "overlay_49_unk_struct_ov70_02263910.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225efd4.h"
#include "overlay_49_unk_struct_ov70_0225f098.h"
#include "overlay_49_unk_struct_ov70_0225f114.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

// ov49 app data, DS size 0x3f8. Field names carry the Platinum offsets; the DS offsets of the sub-objects are in the comments.

struct PlayerProfile;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_02260AD4_t;
struct UnkStruct_ov70_02261E10_t;
struct UnkStruct_ov70_022630A4_t;
struct UnkStruct_ov70_02269204_t;
struct UnkStruct_ov70_0226C6F8_t;

typedef struct UnkStruct_ov70_0225DEE8_sub1 {
    u8 unk_00[20];
    u8 unk_14[20];
} UnkStruct_ov70_0225DEE8_sub1;

typedef struct UnkStruct_ov70_0225DEE8_t {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04_0 : 4;
    u8 unk_04_4 : 4;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s32 unk_08;
    UnkStruct_ov70_0225DEE8_sub1 unk_0C;
    struct UnkStruct_ov66_0222DFF8_t *unk_34;
    UnkStruct_ov70_02263910 *unk_38;
    UnkStruct_ov70_0225E4EC unk_3C;  // DS 0x03c
    UnkStruct_ov70_0225F350 unk_1E8; // DS 0x184
    UnkStruct_ov70_0225F208 unk_340; // DS 0x2dc
    UnkStruct_ov70_0225E9C8 unk_35C; // DS 0x2f8
    UnkStruct_ov70_0225E9C8 unk_37C; // DS 0x318
    UnkStruct_ov70_0225EC20 unk_39C; // DS 0x338
    UnkStruct_ov70_0225F114 unk_3F4; // DS 0x390
    UnkStruct_ov70_0225F098 unk_404; // DS 0x3a0
    UnkStruct_ov70_0225EFD4 unk_428; // DS 0x3c4
    struct UnkStruct_ov70_02269204_t *unk_438; // DS 0x3d4
    BOOL unk_43C;
    struct UnkStruct_ov70_0226C6F8_t *unk_440;
    struct UnkStruct_ov70_0225CA20_t *unk_444;
    struct UnkStruct_ov70_02261E10_t *unk_448;
    struct UnkStruct_ov70_0225C894_t *unk_44C;
    struct UnkStruct_ov70_02260AD4_t *unk_450;
    struct UnkStruct_ov70_022630A4_t *unk_454;
    struct PlayerProfile *unk_458;
} UnkStruct_ov70_0225DEE8_t;

// sizeof(UnkStruct_ov70_0225DEE8_t) == 0x3f8

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_DEE8_T_H
