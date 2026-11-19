#ifndef POKEHEARTGOLD_OVERLAY_67_APP_H
#define POKEHEARTGOLD_OVERLAY_67_APP_H

// Ov67App: the whole heap object of overlay 67 (Frontier records screen), 0x4AC bytes,
// allocated by ov67_021E5900 with OverlayManager_CreateAndGetData.

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "frontier_data.h"
#include "pokemon_types_def.h"

struct FrontierSave;
struct GridInputHandler;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct Options;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct WifiBattleTowerDownloadData;

typedef struct Ov67WifiTrainer {
    struct String *name;
    u8 anonNameIdx;
    u8 country;
    u8 region;
    u8 useDefaultName;
    MailMessage message;
} Ov67WifiTrainer; // 0x10

typedef struct UnkStruct_ov67_021E6B6C {
    u8 f0_0 : 1;
    u8 f0_1 : 7;
    u8 f1_0 : 4;
    u8 f1_4 : 4;
    u8 f2;
    u8 f3;
} UnkStruct_ov67_021E6B6C;

typedef struct Ov67App {
    /* 0x000 */ enum HeapID heapID;
    /* 0x004 */ u32 state;
    /* 0x008 */ u16 isRankingsScreen;
    /* 0x00A */ s8 unk_A;
    /* 0x00B */ u8 unk_B[0x5];
    /* 0x010 */ BgConfig *bgConfig;
    /* 0x014 */ u8 unk_14[0x4];
    /* 0x018 */ struct Options *options;
    /* 0x01C */ struct FrontierSave *frontierSave;
    /* 0x020 */ FrontierData *frontierData;
    /* 0x024 */ struct WifiBattleTowerDownloadData *downloadData;
    /* 0x028 */ struct MsgData *msgData;
    /* 0x02C */ struct MessageFormat *msgFmt;
    /* 0x030 */ struct String *string;
    /* 0x034 */ u8 unk_34[0x30];
    /* 0x064 */ struct String *defaultNames[2];
    /* 0x06C */ Window windows[35];
    /* 0x29C */ Ov67WifiTrainer wifiTrainers[30];
    /* 0x47C */ struct SpriteSystem *spriteSys;
    /* 0x480 */ struct SpriteManager *spriteMan;
    /* 0x484 */ struct ManagedSprite *sprites[4];
    /* 0x494 */ UnkStruct_ov67_021E6B6C unk_494;
    /* 0x498 */ u8 unk_498[0x4];
    /* 0x49C */ s32 unk_49C;
    /* 0x4A0 */ u16 unk_4A0;
    /* 0x4A2 */ u8 unk_4A2;
    /* 0x4A3 */ u8 unk_4A3;
    /* 0x4A4 */ struct GridInputHandler *unk_4A4;
    /* 0x4A8 */ s32 unk_4A8;
} Ov67App;

// sizeof(Ov67App) == 0x4AC

u32 ov67_021E5A6C(Ov67App *app);
BOOL ov67_021E5B44(Ov67App *app);
u32 ov67_021E5C04(Ov67App *app);
s32 ov67_021E5C44(Ov67App *app);
void ov67_021E5DA0(Ov67App *app);
void ov67_021E5E84(Ov67App *app);
void ov67_021E5EB0(Ov67App *app);
void ov67_021E6008(Ov67App *app);
void ov67_021E6080(Ov67App *app);
void ov67_021E60B4(Ov67App *app);
void ov67_021E60F4(Ov67App *app);
void ov67_021E6118(Window *window, struct String *string, u32 x, u32 y, enum Font fontId, u32 color, s32 align);
void ov67_021E6164(Ov67App *app, s32 windowIdx, s32 strno, s32 x, s32 y, enum Font fontId, s32 color, s32 align);
void ov67_021E61A0(Ov67App *app, s32 windowIdx, s32 strno, s32 x, s32 y, enum Font fontId, s32 color, s32 align);
void ov67_021E61E8(Ov67App *app);
void ov67_021E6220(Ov67App *app);
void ov67_021E63E8(Ov67App *app);
void ov67_021E6474(Ov67App *app);
void ov67_021E6490(Ov67App *app);
void ov67_021E6530(Ov67App *app);
void ov67_021E65C0(Ov67App *app);
void ov67_021E6688(Ov67App *app, u32 idx);
void ov67_021E6820(Ov67App *app);
void ov67_021E6990(Ov67App *app);
void ov67_021E69D8(Ov67App *app);
void ov67_021E6A08(Ov67App *app, s32 idx, s32 anim);
void ov67_021E6A28(Ov67App *app, s32 idx, s16 x, s16 y);
void ov67_021E6B6C(Ov67App *app, int arg1);
void ov67_021E6BB8(Ov67App *app);
void ov67_021E6BC4(Ov67App *app);
void ov67_021E6C04(Ov67App *app);
void ov67_021E6C14(Ov67App *app, s32 target);

#endif // POKEHEARTGOLD_OVERLAY_67_APP_H
