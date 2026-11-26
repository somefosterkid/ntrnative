#ifndef POKEHEARTGOLD_OVERLAY_112_POKEWALKER_APP_H
#define POKEHEARTGOLD_OVERLAY_112_POKEWALKER_APP_H

// Ov112PokewalkerApp: overlay 112's main Pokewalker app object, OverlayManager_CreateAndGetData(man, sizeof(Ov112PokewalkerApp),
// HEAP_ID_154) in ov112_021E76CC. Closed object (only ov112 code touches it). Fields sit at the DS offsets in the comments
// (one field at every offset the code reads or writes, blobs for regions only passed by address to the callees that own them,
// pads elsewhere); 0x24..0x1D750 holds no pointers, so its layout is the same on the PC. The DS size is 0x1F378 (checked below
// on the DS build), the PC size is bigger.

#include "bg_window.h"
#include "font.h"
#include "overlay_112_sub_screen.h"
#include "pokemon_types_def.h"

struct Bag;
struct BgConfig;
struct BoxPokemon;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NNSG2dCharacterData;
struct Options;
struct OverlayManager;
struct PlayerProfile;
struct POKEWALKER;
struct Pokedex;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct SysTask;
struct WaitingIcon;

// Ov112PokewalkerLink: the IR link block at +0x1D764 (ov112_021EF1CC / 021EF310 / 021EF03C / 021EF3F8 take its address).
typedef struct Ov112PokewalkerLink {
    s32 active;                      // 0x00
    void *unk_4;                     // 0x04: sub_020932E0
    Ov112SubScreen *subScreen;       // 0x08: ov112_021F039C
    u32 unk_C;                       // 0x0C
    u32 unk_10;                      // 0x10
    u32 unk_14;                      // 0x14
} Ov112PokewalkerLink;

typedef struct Ov112PokewalkerRec {
    u8 unk_0[0x2];
    s8 unk_2;
    s8 unk_3;
    s16 unk_4;
    s16 unk_6;
    s16 unk_8;
    s16 unk_A;
    u16 unk_C;
    s16 unk_E;
} Ov112PokewalkerRec; // 0x10

typedef struct Ov112PokewalkerMonRec {
    u16 unk_0; // species
    u8 unk_2[0xB];
    u8 unk_D; // bit-field views at the use sites
    u8 unk_E[0x2];
} Ov112PokewalkerMonRec; // 0x10

typedef struct Ov112PokewalkerItemRec {
    u16 unk_0;
    u8 unk_2[0x2];
} Ov112PokewalkerItemRec; // 0x4

typedef struct Ov112PokewalkerApp {
    u32 unk_0; // 0x0
    s32 unk_4; // 0x4
    s32 unk_8; // 0x8
    u32 unk_C; // 0xC
    u32 unk_10; // 0x10
    u16 unk_14; // 0x14
    u16 unk_16; // 0x16
    struct BgConfig *bgConfig; // 0x18
    struct OverlayManager *man; // 0x1C
    struct SaveData *saveData; // 0x20
    u8 unk_24[0x1000];
    u32 unk_1024; // 0x1024
    u32 unk_1028; // 0x1028
    u16 unk_102C; // 0x102C
    u16 unk_102E; // 0x102E
    u32 unk_1030; // 0x1030
    u8 unk_1034[0x38];
    u16 unk_106C[9]; // 0x106C
    u8 unk_107E; // 0x107E
    u8 unk_107F; // 0x107F
    u8 pad_1080[0xC];
    u8 unk_108C[0x4];
    s32 unk_1090; // 0x1090
    u8 unk_1094[0x53];
    u8 unk_10E7; // 0x10E7
    u8 pad_10E8[0x8];
    u32 unk_10F0; // 0x10F0
    u8 unk_10F4[0x8C50];
    u16 unk_9D44; // 0x9D44
    u8 pad_9D46[0xB];
    u8 unk_9D51; // 0x9D51
    u8 unk_9D52[0x2];
    u16 unk_9D54[14]; // 0x9D54: nickname
    u8 unk_9D70[0x8];
    u8 unk_9D78; // 0x9D78
    u8 pad_9D79[0x1];
    u16 unk_9D7A; // 0x9D7A
    Ov112PokewalkerMonRec unk_9D7C[3]; // 0x9D7C
    Ov112PokewalkerItemRec unk_9DAC[3]; // 0x9DAC
    Ov112PokewalkerItemRec unk_9DB8[10]; // 0x9DB8
    u8 pad_9DE0[0x1C];
    u8 unk_9DFC[0xC38];
    u8 unk_AA34[0x88];
    u8 unk_AABC; // 0xAABC
    u8 pad_AABD[0x243];
    Ov112PokewalkerMonRec unk_AD00; // 0xAD00
    u8 pad_AD10[0x4];
    u32 unk_AD14; // 0xAD14
    u8 pad_AD18[0x2];
    u16 unk_AD1A; // 0xAD1A
    u8 pad_AD1C[0x2];
    u16 unk_AD1E[8];
    u8 unk_AD2E; // 0xAD2E
    u8 unk_AD2F[0x1];
    u8 unk_AD30[0x2D2];
    u16 unk_B002; // 0xB002
    u8 pad_B004[0x180];
    u8 unk_B184[0x178C];
    u8 unk_C910[0x470];
    u8 unk_CD80[0x2450];
    u32 unk_F1D0; // 0xF1D0
    u16 unk_F1D4; // 0xF1D4
    u8 pad_F1D6[0x2];
    u8 unk_F1D8[0x28];
    u8 unk_F200[0x1634];
    u8 unk_10834[0xCF1C];
    struct SaveData *unk_1D750; // 0x1D750
    void *unk_1D754; // 0x1D754
    u32 unk_1D758; // 0x1D758
    u32 unk_1D75C; // 0x1D75C
    u16 unk_1D760; // 0x1D760
    u8 pad_1D762[0x2];
    Ov112PokewalkerLink link; // 0x1D764
    u16 unk_1D77C; // 0x1D77C
    u8 pad_1D77E[0x1A];
    s32 unk_1D798; // 0x1D798
    u8 unk_1D79C[0x10];
    u8 unk_1D7AC[0xC80];
    Pokemon *unk_1E42C; // 0x1E42C: the party mon picked (ov112_021EF31C), NULL when unk_1E430 holds a box mon
    struct BoxPokemon *unk_1E430; // 0x1E430
    struct Bag *bag; // 0x1E434
    struct PlayerProfile *profile; // 0x1E438
    struct Options *options; // 0x1E43C
    struct POKEWALKER *pokewalker; // 0x1E440
    struct Pokedex *pokedex; // 0x1E444
    struct MessageFormat *msgFormat; // 0x1E448
    struct MsgData *msgData; // 0x1E44C
    struct MsgData *msgData2; // 0x1E450
    struct String *unk_1E454; // 0x1E454
    struct String *unk_1E458; // 0x1E458
    struct String *unk_1E45C; // 0x1E45C
    struct String *unk_1E460; // 0x1E460
    struct String *unk_1E464; // 0x1E464
    struct String *unk_1E468; // 0x1E468
    struct String *unk_1E46C; // 0x1E46C
    struct String *unk_1E470; // 0x1E470
    struct String *unk_1E474; // 0x1E474
    u8 pad_1E478[0x1C];
    struct String *unk_1E494; // 0x1E494
    struct String *unk_1E498; // 0x1E498
    struct String *unk_1E49C; // 0x1E49C
    struct String *unk_1E4A0; // 0x1E4A0
    struct String *unk_1E4A4; // 0x1E4A4
    struct String *unk_1E4A8; // 0x1E4A8
    struct String *unk_1E4AC; // 0x1E4AC
    struct String *unk_1E4B0; // 0x1E4B0
    struct String *unk_1E4B4; // 0x1E4B4
    u8 pad_1E4B8[0x4];
    struct String *unk_1E4BC; // 0x1E4BC
    u8 pad_1E4C0[0x4C];
    struct String *unk_1E50C; // 0x1E50C
    struct String *unk_1E510; // 0x1E510
    struct String *unk_1E514; // 0x1E514
    struct String *unk_1E518; // 0x1E518
    u8 pad_1E51C[0x8];
    s32 unk_1E524; // 0x1E524
    struct SpriteSystem *spriteSystem; // 0x1E528
    struct SpriteManager *spriteManager; // 0x1E52C
    struct ManagedSprite *sprites[217]; // 0x1E530
    u8 pad_1E894[0x1CC];
    void *unk_1EA60; // 0x1EA60
    struct NNSG2dCharacterData *unk_1EA64; // 0x1EA64
    Window windows[20]; // 0x1EA68
    Window windows2[10]; // 0x1EBA8
    u8 unk_1EC48; // 0x1EC48
    u8 unk_1EC49; // 0x1EC49
    u8 unk_1EC4A; // 0x1EC4A
    u8 unk_1EC4B; // 0x1EC4B
    struct WaitingIcon *waitingIcon; // 0x1EC4C
    s32 unk_1EC50; // 0x1EC50
    s32 unk_1EC54; // 0x1EC54
    s8 unk_1EC58[30]; // 0x1EC58
    u16 unk_1EC76; // 0x1EC76
    s32 unk_1EC78; // 0x1EC78
    s32 unk_1EC7C; // 0x1EC7C
    Ov112PokewalkerRec unk_1EC80[100]; // 0x1EC80
    u8 unk_1F2C0[0x10];
    s32 unk_1F2D0; // 0x1F2D0
    u16 unk_1F2D4; // 0x1F2D4
    u16 unk_1F2D6; // 0x1F2D6
    struct SysTask *unk_1F2D8; // 0x1F2D8
    struct SysTask *unk_1F2DC; // 0x1F2DC
    u16 unk_1F2E0; // 0x1F2E0
    u16 unk_1F2E2; // 0x1F2E2
    s16 unk_1F2E4; // 0x1F2E4
    u8 pad_1F2E6[0x2];
    u8 unk_1F2E8[0x88];
    s16 unk_1F370; // 0x1F370
    u8 pad_1F372[0x2];
    s16 unk_1F374; // 0x1F374
    u16 unk_1F376; // 0x1F376
} Ov112PokewalkerApp;

#if !defined(SDK_PORT) && !defined(NONMATCHING)
typedef char Ov112PokewalkerApp_size_is_0x1F378[(sizeof(Ov112PokewalkerApp) == 0x1F378) ? 1 : -1];
#endif

// ov112 functions that take the app (one declaration each; the local prototypes are gone).
void ov112_021E7768(Ov112PokewalkerApp *arg);
void ov112_021E77E4(Ov112PokewalkerApp *arg0);
void ov112_021E795C(Ov112PokewalkerApp *arg0);
void ov112_021E7B18(Ov112PokewalkerApp *param_1);
void ov112_021E7C18(Ov112PokewalkerApp *arg0);
void ov112_021E7C6C(Ov112PokewalkerApp *arg0);
void ov112_021E7D7C(Ov112PokewalkerApp *arg0);
void ov112_021E7DA4(Ov112PokewalkerApp *arg0);
void ov112_021E7DC0(Ov112PokewalkerApp *param_1);
void ov112_021E7DFC(Ov112PokewalkerApp *arg0, void *dest);
void ov112_021E90FC(Window *window, Ov112PokewalkerApp *arg1);
void ov112_021E9148(u8 *param_1, Ov112PokewalkerApp *param_2, u8 *param_3, u8 *param_4);
s32 ov112_021E9338(Ov112PokewalkerApp *arg0);
void ov112_021E95A0(Ov112PokewalkerApp *arg0);
s32 ov112_021E9610(Ov112PokewalkerApp *arg0, u32 arg1);
void ov112_021E966C(Ov112PokewalkerApp *arg0, s32 arg1);
void ov112_021E96D0(Ov112PokewalkerApp *arg0, s32 arg1);
int ov112_021E9750(Ov112PokewalkerApp *app);
void ov112_021E98A4(Ov112PokewalkerApp *arg0);
void ov112_021E98E8(Ov112PokewalkerApp *param_1, u16 param_2);
void ov112_021E9A78(Ov112PokewalkerApp *param_1, u8 param_2);
void ov112_021E9C98(Ov112PokewalkerApp *param_1);
void ov112_021E9E30(Ov112PokewalkerApp *arg0);
void ov112_021E9F40(Ov112PokewalkerApp *arg0, s32 arg1);
void ov112_021E9F5C(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno, s32 arg3);
void ov112_021EA044(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno, s32 arg3);
u8 ov112_021EA08C(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno);
void ov112_021EA10C(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno);
void ov112_021EA17C(Ov112PokewalkerApp *arg0);
void ov112_021EA1AC(Ov112PokewalkerApp *arg0);
void ov112_021EA1F8(Ov112PokewalkerApp *arg0);
void ov112_021EA230(Ov112PokewalkerApp *app);
void ov112_021EA2C4(Ov112PokewalkerApp *param_1);
void ov112_021EA3C0(Ov112PokewalkerApp *app);
void ov112_021EA4F4(Ov112PokewalkerApp *arg0);
void ov112_021EA51C(Ov112PokewalkerApp *arg0, s32 arg1);
void ov112_021EA584(Ov112PokewalkerApp *arg0);
void ov112_021EA5A4(Ov112PokewalkerApp *param_1, s32 param_2);
void ov112_021EA60C(Ov112PokewalkerApp *param_1);
void ov112_021EA64C(Ov112PokewalkerApp *arg0);
void ov112_021EA670(Ov112PokewalkerApp *arg0, s32 arg1);
void ov112_021EA688(Ov112PokewalkerApp *arg0, s32 arg1);
void ov112_021EA6A0(Ov112PokewalkerApp *arg0, s32 arg1, s32 seqno);
void ov112_021EA6B8(Ov112PokewalkerApp *app, s32 idx, u16 x, u16 y);
s32 ov112_021EA838(Ov112PokewalkerApp *arg0);
void ov112_021EA984(Ov112PokewalkerApp *arg0);
void ov112_021EAA98(Ov112PokewalkerApp *param_1, u8 *param_2, s32 param_3);
s32 ov112_021EAAE4(Ov112PokewalkerApp *arg0, void *arg1);
void ov112_021EAB78(Ov112PokewalkerApp *arg0, s32 arg1, u32 species, u32 form);
int ov112_021EAC5C(Ov112PokewalkerApp *work);
void ov112_021EADD0(Ov112PokewalkerApp *param_1, s32 param_2);
u32 ov112_021EAF5C(Ov112PokewalkerApp *param_1);
s32 ov112_021EAFFC(Ov112PokewalkerApp *arg0);
s32 ov112_021EB1D8(Ov112PokewalkerApp *arg0);
s32 ov112_021EB1E8(Ov112PokewalkerApp *arg0);
s32 ov112_021EB208(Ov112PokewalkerApp *arg0);
s32 ov112_021EB244(Ov112PokewalkerApp *arg0);
s32 ov112_021EB388(Ov112PokewalkerApp *arg0);
s32 ov112_021EB3A8(Ov112PokewalkerApp *arg0);
s32 ov112_021EB3B8(Ov112PokewalkerApp *arg0);
s32 ov112_021EB3D8(Ov112PokewalkerApp *arg0);
s32 ov112_021EB44C(Ov112PokewalkerApp *arg0, s32 arg1);
s32 ov112_021EB568(Ov112PokewalkerApp *arg0);
s32 ov112_021EB5AC(Ov112PokewalkerApp *arg0);
s32 ov112_021EB5BC(Ov112PokewalkerApp *arg0);
s32 ov112_021EB5DC(Ov112PokewalkerApp *arg0);
s32 ov112_021EB664(Ov112PokewalkerApp *arg0);
u32 ov112_021EB780(Ov112PokewalkerApp *param_1, s32 arg1);
s32 ov112_021EB7D8(Ov112PokewalkerApp *arg0);
s32 ov112_021EB838(Ov112PokewalkerApp *arg0);
s32 ov112_021EB864(Ov112PokewalkerApp *arg0);
s32 ov112_021EB8C4(Ov112PokewalkerApp *arg0);
s32 ov112_021EB8E8(Ov112PokewalkerApp *arg0);
void ov112_021EB90C(Ov112PokewalkerApp *arg0, s32 arg1);
u32 ov112_021EB950(Ov112PokewalkerApp *param_1, u32 param_2);
s32 ov112_021EBA54(Ov112PokewalkerApp *arg0);
s32 ov112_021EBA6C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBAB4(Ov112PokewalkerApp *arg0);
s32 ov112_021EBB84(Ov112PokewalkerApp *arg0);
s32 ov112_021EBB98(Ov112PokewalkerApp *arg0);
s32 ov112_021EBBB0(Ov112PokewalkerApp *arg0);
s32 ov112_021EBBD8(Ov112PokewalkerApp *arg0);
s32 ov112_021EBC3C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBC7C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBCD8(Ov112PokewalkerApp *arg0, s32 arg1);
s32 ov112_021EBD9C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBDE8(Ov112PokewalkerApp *arg0);
s32 ov112_021EBE4C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBE6C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBE78(Ov112PokewalkerApp *arg0);
s32 ov112_021EBF18(Ov112PokewalkerApp *arg0);
s32 ov112_021EBF4C(Ov112PokewalkerApp *arg0);
s32 ov112_021EBF70(Ov112PokewalkerApp *arg0);
s32 ov112_021EBFC8(Ov112PokewalkerApp *arg0);
u32 ov112_021EC000(Ov112PokewalkerApp *param_1);
s32 ov112_021EC044(Ov112PokewalkerApp *param_1);
void ov112_021EC134(Ov112PokewalkerApp *arg0);
s32 ov112_021EC1A8(Ov112PokewalkerApp *arg0);
s32 ov112_021EC1E4(Ov112PokewalkerApp *arg0);
s32 ov112_021EC208(Ov112PokewalkerApp *arg0);
s32 ov112_021EC284(Ov112PokewalkerApp *arg0);
s32 ov112_021EC2A8(Ov112PokewalkerApp *arg0);
s32 ov112_021EC2E4(Ov112PokewalkerApp *arg0);
s32 ov112_021EC304(Ov112PokewalkerApp *arg0);
s32 ov112_021EC338(Ov112PokewalkerApp *arg0);
s32 ov112_021EC378(Ov112PokewalkerApp *arg0);
s32 ov112_021EC390(Ov112PokewalkerApp *arg0);
s32 ov112_021EC3B0(Ov112PokewalkerApp *arg0);
s32 ov112_021EC3C8(Ov112PokewalkerApp *arg0);
s32 ov112_021EC414(Ov112PokewalkerApp *arg0);
void ov112_021EC440(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2, s32 arg3);
void ov112_021EC460(Ov112PokewalkerApp *param_1, u32 param_2, u32 param_3, u32 param_4);
s32 ov112_021EC478(Ov112PokewalkerApp *arg0);
s32 ov112_021EC54C(Ov112PokewalkerApp *arg0);
s32 ov112_021EC55C(Ov112PokewalkerApp *arg0);
s32 ov112_021EC640(Ov112PokewalkerApp *arg0);
s32 ov112_021EC67C(Ov112PokewalkerApp *arg0);
s32 ov112_021EC70C(Ov112PokewalkerApp *arg0);
s32 ov112_021EC778(Ov112PokewalkerApp *arg0);
s32 ov112_021EC7B8(Ov112PokewalkerApp *arg0);
void ov112_021EC7D0(Ov112PokewalkerApp *param_1);
void ov112_021EC8A4(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2);
s32 ov112_021EC950(Ov112PokewalkerApp *arg0);
s32 ov112_021EC99C(Ov112PokewalkerApp *arg0);
void ov112_021ECA18(Ov112PokewalkerApp *arg0, s32 arg1);
s8 ov112_021ECA88(Ov112PokewalkerApp *arg0);
int ov112_021ECAA8(Ov112PokewalkerApp *work);
u32 ov112_021ECCC4(Ov112PokewalkerApp *param_1);
void ov112_021ECDA0(Ov112PokewalkerApp *param_1, s32 param_2);
s32 ov112_021ECE28(Ov112PokewalkerApp *arg0);
s32 ov112_021ECECC(Ov112PokewalkerApp *arg0);
s32 ov112_021ECEEC(Ov112PokewalkerApp *arg0);
s32 ov112_021ECEF8(Ov112PokewalkerApp *arg0);
s32 ov112_021ECF38(Ov112PokewalkerApp *arg0);
s32 ov112_021ECF88(Ov112PokewalkerApp *arg0);
s32 ov112_021ECFD4(Ov112PokewalkerApp *arg0);
s32 ov112_021ED020(Ov112PokewalkerApp *arg0);
s32 ov112_021ED054(Ov112PokewalkerApp *arg0);
s32 ov112_021ED090(Ov112PokewalkerApp *arg0);
void ov112_021ED0C8(Ov112PokewalkerApp *arg0);
void ov112_021ED0D0(Ov112PokewalkerApp *arg0);
s32 ov112_021ED1A0(Ov112PokewalkerApp *arg0);
s32 ov112_021ED224(Ov112PokewalkerApp *arg0);
void ov112_021ED25C(Ov112PokewalkerApp *arg0);
s32 ov112_021ED264(Ov112PokewalkerApp *arg0);
void ov112_021ED2F4(Ov112PokewalkerApp *arg0);
s32 ov112_021ED330(Ov112PokewalkerApp *arg0, s32 arg1);
u32 ov112_021ED35C(Ov112PokewalkerApp *param_1);
u32 ov112_021ED4D4(Ov112PokewalkerApp *param_1);
void ov112_021ED590(Ov112PokewalkerApp *param_1);
void ov112_021ED5DC(Ov112PokewalkerApp *arg0);
s32 ov112_021ED610(Ov112PokewalkerApp *arg0);
s32 ov112_021ED640(Ov112PokewalkerApp *arg0);
u32 ov112_021ED66C(Ov112PokewalkerApp *param_1);
void ov112_021ED7DC(Ov112PokewalkerApp *arg0_);
u32 ov112_021ED8DC(Ov112PokewalkerApp *param_1);
u32 ov112_021ED918(Ov112PokewalkerApp *param_1);
u32 ov112_021EDA4C(Ov112PokewalkerApp *param_1);
void ov112_021EDAF4(Ov112PokewalkerApp *arg0, s32 arg1);
u32 ov112_021EDB24(Ov112PokewalkerApp *param_1, s32 arg1);
s32 ov112_021EDD08(Ov112PokewalkerApp *arg0);
s32 ov112_021EDD30(Ov112PokewalkerApp *arg0);
s32 ov112_021EDD5C(Ov112PokewalkerApp *arg0);
s32 ov112_021EDD80(Ov112PokewalkerApp *arg0);
s32 ov112_021EDDB4(Ov112PokewalkerApp *arg0);
s32 ov112_021EDDE8(Ov112PokewalkerApp *arg0);
s32 ov112_021EDE1C(Ov112PokewalkerApp *arg0);
s32 ov112_021EDE68(Ov112PokewalkerApp *arg0);
u32 ov112_021EDF08(Ov112PokewalkerApp *param_1);
s32 ov112_021EDF4C(Ov112PokewalkerApp *arg0);
s32 ov112_021EDFE8(Ov112PokewalkerApp *arg0);
s32 ov112_021EE044(Ov112PokewalkerApp *arg0);
s32 ov112_021EE0F4(Ov112PokewalkerApp *arg0);
s32 ov112_021EE10C(Ov112PokewalkerApp *arg0);
s32 ov112_021EE12C(Ov112PokewalkerApp *arg0);
s32 ov112_021EE178(Ov112PokewalkerApp *arg0);
s32 ov112_021EE1A4(Ov112PokewalkerApp *arg0);
s32 ov112_021EE1BC(Ov112PokewalkerApp *arg0);
s32 ov112_021EE1DC(Ov112PokewalkerApp *arg0);
s32 ov112_021EE1E8(Ov112PokewalkerApp *arg0);
s32 ov112_021EE29C(Ov112PokewalkerApp *arg0);
s32 ov112_021EE2B4(Ov112PokewalkerApp *arg0);
s32 ov112_021EE2D4(Ov112PokewalkerApp *arg0);
s32 ov112_021EE31C(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2);
s32 ov112_021EE3E0(Ov112PokewalkerApp *arg0);
s32 ov112_021EE464(Ov112PokewalkerApp *arg0);
s32 ov112_021EE4B8(Ov112PokewalkerApp *arg0);
int ov112_021EE4D0(Ov112PokewalkerApp *work);
s32 ov112_021EE514(Ov112PokewalkerApp *arg0);
s32 ov112_021EE550(Ov112PokewalkerApp *arg0);
s32 ov112_021EE570(Ov112PokewalkerApp *arg0);
s32 ov112_021EE594(Ov112PokewalkerApp *arg0);
s32 ov112_021EE5B8(Ov112PokewalkerApp *arg0);
s32 ov112_021EE628(Ov112PokewalkerApp *arg0);
s32 ov112_021EE6B0(Ov112PokewalkerApp *arg0);
s32 ov112_021EE6D0(Ov112PokewalkerApp *arg0);
s32 ov112_021EE6F8(Ov112PokewalkerApp *arg0);
s32 ov112_021EE718(Ov112PokewalkerApp *arg0);
s32 ov112_021EE73C(Ov112PokewalkerApp *arg0);
s32 ov112_021EE754(Ov112PokewalkerApp *arg0);
s32 ov112_021EE778(Ov112PokewalkerApp *arg0);
void ov112_021EE920(Ov112PokewalkerApp *param_1);
void ov112_021EEA7C(Ov112PokewalkerApp *app);
void ov112_021EEAF0(Ov112PokewalkerApp *work, int useWalkerBox);
void ov112_021EECF0(Ov112PokewalkerApp *arg0);
int ov112_021EED08(Ov112PokewalkerApp *work);
s32 ov112_021EEE20(Ov112PokewalkerApp *arg0);
s32 ov112_021EEE2C(Ov112PokewalkerApp *arg0);
s32 ov112_021EEE4C(Ov112PokewalkerApp *arg0);
s32 ov112_021EEEC0(Ov112PokewalkerApp *arg0);
s32 ov112_021EEEE0(Ov112PokewalkerApp *arg0);
s32 ov112_021EEF30(Ov112PokewalkerApp *arg0);
s32 ov112_021EEF68(Ov112PokewalkerApp *arg0, s32 arg1);
u32 ov112_021EF03C(Ov112PokewalkerApp *param_1, s32 arg1);
s32 ov112_021EF100(Ov112PokewalkerApp *arg0);
void ov112_021EF31C(void *arg0, Ov112PokewalkerApp *arg1);
void ov112_021EF3F8(int box, int slot, Ov112PokewalkerApp *work);
s32 ov112_021EF568(Ov112PokewalkerApp *arg0);
u32 ov112_021EF5AC(Ov112PokewalkerApp *param_1);
s32 ov112_021EF62C(Ov112PokewalkerApp *arg0);
s32 ov112_021EF664(Ov112PokewalkerApp *arg0);
s32 ov112_021EF6A0(Ov112PokewalkerApp *arg0);
s32 ov112_021EF700(Ov112PokewalkerApp *arg0);
u32 ov112_021EF758(Ov112PokewalkerApp *app);
s32 ov112_021EF8D4(Ov112PokewalkerApp *arg0);
u32 ov112_021EF958(Ov112PokewalkerApp *param_1);
s32 ov112_021EF994(Ov112PokewalkerApp *arg0);
u32 ov112_021EF9D4(Ov112PokewalkerApp *param_1);
s32 ov112_021EFA1C(Ov112PokewalkerApp *arg0);
s32 ov112_021EFA44(Ov112PokewalkerApp *arg0);
s32 ov112_021EFA74(Ov112PokewalkerApp *arg0);
s32 ov112_021EFB14(Ov112PokewalkerApp *arg0);
s32 ov112_021EFCB0(Ov112PokewalkerApp *arg0);
s32 ov112_021EFCFC(Ov112PokewalkerApp *arg0);
s32 ov112_021EFD4C(Ov112PokewalkerApp *arg0);
s32 ov112_021EFDA8(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ov112_021EFEEC(Ov112PokewalkerApp *arg0);
u32 ov112_021EFF84(Ov112PokewalkerApp *app);
s32 ov112_021F00E0(Ov112PokewalkerApp *arg0);
u32 ov112_021F0114(Ov112PokewalkerApp *param_1);
u32 ov112_021F01CC(Ov112PokewalkerApp *app);
s32 ov112_021F0328(Ov112PokewalkerApp *arg0);
s32 ov112_021F0368(Ov112PokewalkerApp *arg0);

#endif // POKEHEARTGOLD_OVERLAY_112_POKEWALKER_APP_H
