#ifndef POKEHEARTGOLD_OVERLAY_45_WIFI_PLAZA_SYSTEM_H
#define POKEHEARTGOLD_OVERLAY_45_WIFI_PLAZA_SYSTEM_H

// The Wi-Fi Plaza (lobby) system object: allocated 0x530 bytes in ov45_02229FF4, stored in the lobby
// task data (+0x4), ov45's state (+0x3c) and a global (+0x18c); every ov45/ov47/ov48/ov49/ov88/ov89/ov92
// accessor takes it. One definition for every file (they used to carry ~45 local copies).
// Platinum's ov66 UnkStruct_ov66_0222DFF8.

#include "global.h"

#include "player_data.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_unk_struct_ov66_022309a4.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"

struct UnkStruct_ov66_0223177C_t;
struct UnkStruct_0207E060_t;
struct WallpaperPasswordBank;
struct SaveData;

typedef struct UnkStruct_ov66_0222E908 {
    u32 questionNumber;
    u32 selectedAnswer;
} UnkStruct_ov66_0222E908;

typedef struct UnkStruct_ov66_02230914 {
    u16 unk_00[4];
} UnkStruct_ov66_02230914;

typedef struct UnkStruct_ov66_02231560_sub1 {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03_0 : 4;
    u8 unk_03_4 : 4;
} UnkStruct_ov66_02231560_sub1;

typedef struct UnkStruct_ov66_02231560 {
    UnkStruct_ov66_02231560_sub1 unk_00[50];
} UnkStruct_ov66_02231560;

typedef struct UnkStruct_ov68_0225DC74 {
    u32 unk_00[3];
    u64 unk_0C;
} UnkStruct_ov68_0225DC74;

typedef struct WiFiPlazaProfile {
    s32 unk_00;
    u32 trainerID;
    u16 trainerName[7 + 1];
    UnkStruct_ov66_02231428 unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    u16 partySpecies[6];
    u8 partyForms[6];
    u8 partyIsEgg[6];
    u8 trainerGender;
    u8 language;
    u16 trainerAppearanceIdx;
    u16 country;
    u8 region;
    u8 isNationalDexObtained;
    u8 isMainStoryCleared;
    u8 unk_41;
    u8 gameVersion;
    u8 unk_43;
    s64 startTimestamp;
    u8 unk_4C[12];
    s32 unk_58[12];
    u16 unk_88[2];
    UnkStruct_ov66_0222E908 unk_8C;
} WiFiPlazaProfile;

typedef struct UnkStruct_ov66_0222F6C4 {
    u16 unk_00[8];
    u16 unk_10[8];
    WiFiPlazaProfile profile;
    u32 unk_B4;
} UnkStruct_ov66_0222F6C4;

typedef struct UnkStruct_ov66_0222DFF8_sub2 {
    u8 unk_00[8];
} UnkStruct_ov66_0222DFF8_sub2;

typedef struct UnkStruct_ov66_022302B0 {
    BOOL unk_00;
    s32 unk_04;
    s32 unk_08[9];
    u32 unk_2C[9];
    u32 unk_50[9][3];
    u32 unk_BC[9][3];
    BOOL unk_128[9][3];
} UnkStruct_ov66_022302B0;

typedef struct UnkStruct_ov66_0222FA80 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
} UnkStruct_ov66_0222FA80;

typedef struct UnkStruct_ov66_0222FB64 {
    u8 unk_00_0 : 1;
    u8 unk_00_1 : 1;
    u8 unk_00_2 : 2;
    u8 unk_00_4 : 1;
    u8 unk_00_5 : 2;
    u8 unk_00_7 : 1;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F_0 : 4;
    u8 unk_0F_4 : 4;
} UnkStruct_ov66_0222FB64;

typedef struct UnkStruct_ov66_0222DFF8_sub1 {
    struct UnkStruct_ov66_0223177C_t *unk_00;
    BOOL unk_04;
    UnkStruct_ov66_02231560 unk_08;
    UnkStruct_ov66_02231428 unk_D0;
    s64 unk_D4;
    s64 unk_DC;
    PlayerProfile *trainerInfos[4];
} UnkStruct_ov66_0222DFF8_sub1;

typedef struct UnkStruct_ov66_022305B0 {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_022305B0;

typedef struct UnkStruct_ov66_0222FC68 {
    u32 unk_00;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C[20];
    UnkStruct_ov66_022305B0 unk_20;
    u16 unk_24;
    s16 unk_26;
} UnkStruct_ov66_0222FC68;

typedef struct UnkStruct_ov66_0222FC58 {
    u8 unk_00[20];
} UnkStruct_ov66_0222FC58;

typedef struct UnkStruct_ov66_0222FBC8 {
    s16 unk_00;
    u8 unk_02;
} UnkStruct_ov66_0222FBC8;

typedef struct UnkStruct_ov66_022307D4 {
    u8 unk_00[9];
    u8 unk_09[3];
    u8 unk_0C[20];
} UnkStruct_ov66_022307D4;

typedef struct UnkStruct_ov66_022308A0 {
    u8 unk_00[20];
    UnkStruct_ov66_02230914 unk_14[20];
    struct WallpaperPasswordBank *unk_B4;
} UnkStruct_ov66_022308A0;

typedef struct UnkStruct_ov66_0222FBF0 {
    PlayerProfile *trainerInfo;
    BOOL unk_04;
} UnkStruct_ov66_0222FBF0;

typedef struct UnkStruct_ov66_0222DFF8_sub3 {
    UnkStruct_ov68_0225DC74 unk_00;
    BOOL unk_14;
} UnkStruct_ov66_0222DFF8_sub3;

struct UnkStruct_ov66_0222DFF8_t {
    struct SaveData *saveData;
    UnkStruct_ov66_0222DFF8_sub1 unk_04;
    UnkStruct_ov66_0222FA80 unk_F8;
    UnkStruct_ov66_0222F6C4 unk_108;
    UnkStruct_ov66_0222FC68 unk_1C0;
    UnkStruct_ov66_0222FC58 unk_1E8;
    UnkStruct_ov66_0222FB64 unk_1FC;
    UnkStruct_ov66_022302B0 unk_20C;
    UnkStruct_ov66_0222FBC8 unk_3A0;
    UnkStruct_ov66_0222DFF8_sub2 unk_3A3;
    UnkStruct_ov66_022307D4 unk_3AB;
    UnkStruct_ov66_02232068 unk_3CB;
    UnkStruct_ov66_0222E294 unk_3DF;
    UnkStruct_ov66_022308A0 unk_3E4;
    UnkStruct_ov66_022309A4 unk_49C;
    struct UnkStruct_0207E060_t *unk_4B8;
    UnkStruct_ov66_02230A6C unk_4BC;
    UnkStruct_ov66_0222FBF0 unk_508;
    UnkStruct_ov66_0222DFF8_sub3 unk_510;
    enum HeapID heapID;
    u32 unk_52C;
};

// sizeof(WiFiPlazaSystem) == 0x530

#endif // POKEHEARTGOLD_OVERLAY_45_WIFI_PLAZA_SYSTEM_H
