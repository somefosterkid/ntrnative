#ifndef POKEHEARTGOLD_OVERLAY_45_UNK_STRUCT_OV66_022336_C4_H
#define POKEHEARTGOLD_OVERLAY_45_UNK_STRUCT_OV66_022336_C4_H

#include "global.h"
#include "ppw_lobby.h"

struct SaveData;
struct WiFiHistory;
struct WiFiList;

typedef void (*UnkFuncPtr_ov66_0222DDF0_1)(s32, const void *, void *, BOOL);
typedef void (*UnkFuncPtr_ov66_0222DDF0_2)(s32, void *);
typedef void (*UnkFuncPtr_ov66_0222DDF0_3)(s32, const void *, void *);
typedef void (*UnkFuncPtr_ov66_0222DDF0_4)(PPW_LOBBY_TIME_EVENT, void *);
typedef void (*UnkFuncPtr_ov66_0222DDF0_5)(const void *, u32, void *);

typedef struct UnkStruct_ov66_022324F0 {
    UnkFuncPtr_ov66_0222DDF0_1 unk_00;
    UnkFuncPtr_ov66_0222DDF0_2 unk_04;
    UnkFuncPtr_ov66_0222DDF0_3 unk_08;
    UnkFuncPtr_ov66_0222DDF0_4 unk_0C;
    UnkFuncPtr_ov66_0222DDF0_5 unk_10;
} UnkStruct_ov66_022324F0;

typedef void (*UnkFuncPtr_ov66_022589B4)(s32, const void *, u32, void *);

typedef struct UnkStruct_ov66_022589B4 {
    UnkFuncPtr_ov66_022589B4 unk_00;
    u32 unk_04;
} UnkStruct_ov66_022589B4;

typedef struct UnkStruct_ov66_022336C4_sub2 {
    u16 country;
    u8 region;
    u8 unk_03;
} UnkStruct_ov66_022336C4_sub2;

typedef struct UnkStruct_ov66_022336C4_sub1 {
    s16 unk_00;
    u16 unk_02;
    s32 *unk_04;
} UnkStruct_ov66_022336C4_sub1;

typedef struct UnkStruct_ov66_022336C4_sub3 {
    const UnkStruct_ov66_022589B4 *unk_00;
    u32 unk_04;
    void *unk_08;
} UnkStruct_ov66_022336C4_sub3;

typedef struct UnkStruct_ov66_022336C4_sub4 {
    PPW_LobbyRecruitInfo lobbyRecruitInfo;
    u16 unk_44;
    u16 unk_46;
    s32 unk_48;
} UnkStruct_ov66_022336C4_sub4;

typedef struct UnkStruct_ov66_022336C4_sub5 {
    PPW_LobbyVipRecord *lobbyVipRecord;
    u16 unk_04;
    u16 unk_06;
} UnkStruct_ov66_022336C4_sub5;

typedef struct UnkStruct_ov66_022336C4_sub6 {
    PPW_LobbyQuestionnaire lobbyQuestionnaire;
    u32 unk_2D8;
} UnkStruct_ov66_022336C4_sub6;

typedef struct UnkStruct_ov66_022336C4 {
    struct SaveData *saveData;
    struct WiFiList *wiFiList;
    struct WiFiHistory *wiFiHistory;
    UnkStruct_ov66_022324F0 unk_0C;
    void *unk_20;
    u8 *unk_24;
    u32 unk_28;
    PPW_LobbySystemProfile sysProfile[20];
    UnkStruct_ov66_022336C4_sub1 unk_11C[2];
    u8 unk_12C;
    u8 unk_12D;
    u8 unk_12E;
    u8 unk_12F;
    u8 unk_130;
    u8 unk_131;
    u8 unk_132;
    u8 unk_133;
    UnkStruct_ov66_022336C4_sub2 unk_134[20];
    UnkStruct_ov66_022336C4_sub3 unk_184[2];
    PPW_LobbyTimeInfo timeInfo;
    u8 unk_1B4[1024];
    u32 unk_5B4;
    UnkStruct_ov66_022336C4_sub4 unk_5B8[3];
    u16 gameKind;
    u8 unk_69E;
    u8 unk_69F;
    UnkStruct_ov66_022336C4_sub5 unk_6A0;
    UnkStruct_ov66_022336C4_sub6 unk_6A8;
    PPW_LOBBY_RESULT result;
} UnkStruct_ov66_022336C4;

// DS layout (mwcc): sizeof(UnkStruct_ov66_022336C4) == 0x988

extern UnkStruct_ov66_022336C4 *_022577C0;

#endif // POKEHEARTGOLD_OVERLAY_45_UNK_STRUCT_OV66_022336_C4_H
