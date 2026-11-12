// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/mcss.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_MCSS_H
#define BWPORT_SYSTEM_MCSS_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "nnsys/g2d.h"

// MCSS, the system that draws Pokémon and trainer sprites from multi-cell animations. It is mcss.c in the main module.
// The names are Black's dsd names; pokebw2's guesses are in the comments

typedef struct MCSS MCSS;

void func_02015c30(MCSS *mcss, VecFx32 *position); // MCSS_GetPosition
void func_02015c48(MCSS *mcss, VecFx32 *position); // MCSS_SetPosition
void func_02015e04(MCSS *mcss); // MCSS_PauseAnimation

typedef struct MCSSSystem MCSSSystem;

// The archive and files of a sprite's graphics, as SetupPokemonLoaderFSTool fills it in
struct MCSSLoadInfo {
    u32 arcId;
    u32 character;
    u32 palette;
    u32 cells;
    u32 cellAnime;
    u32 multiCells;
    u32 multiCellAnime;
    u32 bin;
    u32 unk20;
};

// Black 2's, not checked against Black
// Called with the callback's parameter and the animation's frame
typedef void (*MCSSAnimationCallback)(uintptr_t param, fx32 frame);

// A sprite's characters and palette on their way to VRAM, which a task loads in the V-blank. A palette that the
// system blends has no files: its palette data and colors are allocated
typedef struct {
    NNSG2dCharacterData *chars;
    NNSG2dPaletteData *pltt;
    NNSG2dImageProxy *imageProxy;
    NNSG2dImagePaletteProxy *plttProxy;
    void *charFile;
    void *plttFile;
    u32 charAddr;
    u32 plttAddr;
    MCSSSystem *system;
    MCSS *mcss;
    // Whether the task runs in the system's task manager, and not in the V-blank's
    BOOL inTcbMgr;
} MCSSLoad;

// Black 2's, not checked against Black
// Called with a sprite's files read and not yet loaded, to change its characters. Returning TRUE removes the callback
typedef BOOL (*MCSSLoadCallback)(const MCSSLoadInfo *info, MCSSLoad *load, u32 work);

void func_02014cb8(MCSSSystem *system, u32 arcId);
void func_02014cd4(MCSSSystem *system);
// pokebw2's MCSSSys_SetLoadCallback
void func_02014cf0(MCSSSystem *system, MCSSLoadCallback callback, u32 work);
void func_02014cf8(MCSSSystem *system, MCSS *mcss, const MCSSLoadInfo *info);
void func_02015c00(MCSSSystem *system);
// More of a sprite's state, which the battle view's sprites use
void func_02015c0c(MCSS *mcss);
void func_02015c1c(MCSS *mcss);
void func_02015c60(MCSS *mcss, VecFx32 *out);
void func_02015c78(MCSS *mcss, VecFx32 *a1);
void func_02015c94(MCSS *mcss, VecFx32 *out);
void func_02015cc4(MCSS *mcss, VecFx32 *out);
void func_02015cdc(MCSS *mcss, VecFx32 *a1);
void func_02015cf8(MCSS *mcss, VecFx32 *out);
void func_02015d14(MCSS *mcss, const VecFx32 *in);
void func_02015d30(MCSS *mcss, VecFx32 *a1);
void func_02015d4c(MCSS *mcss, VecFx32 *out);
void func_02015d64(MCSS *mcss, VecFx32 *in);
void func_02015d80(MCSS *mcss, u16 a1);
void func_02015d88(MCSS *mcss, u16 value);
void func_02015d94(MCSS *mcss, VecFx32 *a1);
void func_02015dd4(MCSS *mcss);
BOOL func_02015df8(MCSS *mcss);
void func_02015e28(MCSS *mcss);
void func_02015e70(MCSS *mcss);
BOOL func_02015e94(MCSS *mcss);
void func_02015ec4(MCSS *mcss);
// The sprite's multi-cell animation, whose animation controller NNS_G2dSetAnimCtrlCallBackFunctor takes
NNSG2dMultiCellAnimation *func_02015ee8(MCSS *mcss);
u16 func_02015eec(MCSS *mcss);
u8 func_02015ef4(MCSS *mcss);
u16 func_02015efc(MCSS *mcss, u8 index);
u16 func_02015f04(MCSS *mcss);
u16 func_02015f0c(MCSS *mcss);
s16 func_02015f14(MCSS *mcss);
s16 func_02015f1c(MCSS *mcss);
u8 func_02015f24(MCSS *mcss);
void func_02015f40(MCSS *mcss, u32 param,
                   BOOL (*func)(u32 param, const NNSG2dMultiCellHierarchyData *node, NNSG2dCellAnimation *cellAnim,
                                u16 nodeIdx));
// Fades the palette between two levels of a color, and whether the fade is running
void func_02015f50(MCSS *mcss, u8 startLevel, u8 endLevel, s32 wait, u32 color);
// Alpha from 0 to 31
u8 func_02015fac(MCSS *mcss);
void func_02015fd4(MCSS *mcss, u32 a1);
void func_02015ff0(MCSS *mcss, u32 a1);
BOOL func_0201600c(MCSS *mcss);
u8 func_02016068(MCSS *mcss);
void func_02016078(MCSSSystem *system, MCSS *mcss, int level);
void func_020160c0(MCSSSystem *system, u32 offset);
void func_020160c4(MCSS *mcss, fx32 speed);
// Called as a sprite's animation ends
void func_02016380(MCSS *mcss);
void func_02016394(MCSS *mcss, BOOL flag);
void func_020163b0(MCSSSystem *system, u32 value);
void func_020163b4(MCSSSystem *system, BOOL flag);

// Fades the palette between two levels of a color, and whether the fade is running

// Fades the palette between two levels of a color, and whether the fade is running

// Adds the sprite of a party Pokémon
MCSS *func_02017270(MCSSSystem *system, PartyPkm *pkm, u32 dir, fx32 x, fx32 y, fx32 z);
// Draws a Spinda's spots for its personality on the next sprite added
void func_020172ac(MCSSSystem *system, u32 personality);
// pokebw2's MCSS_DrawSpindaSpots
// Draws a Spinda's spots for its personality on a sprite's characters
void func_020172d8(void *chars, u32 personality);
void func_020173b4(MCSS *mcss);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The files of a Pokémon's sprite
void SetupPokemonLoaderFSTool(u32 species, u32 form, u32 sex, BOOL rare, BOOL egg, MCSSLoadInfo *info, u32 dir);
// The files of a trainer's sprite, from the front or the back
void GetTrainerSpriteResIDs(u32 trainerType, MCSSLoadInfo *info, BOOL back);
// pokebw2's MCSSSys_Create
MCSSSystem *func_02014b08(u32 count, HeapID heapId);
// pokebw2's func_0201aefc
void func_02016020(MCSSSystem *system, u32 value);
// pokebw2's func_0201af00
void func_02016024(MCSSSystem *system, u32 value);
// pokebw2's func_0201aacc
void func_02015bf0(MCSSSystem *system);
// pokebw2's MCSSSys_Remove
void func_02015ba4(MCSSSystem *system, MCSS *mcss);
// pokebw2's MCSSSys_Free
void func_02014ba8(MCSSSystem *system);
// pokebw2's MCSSSys_Update
// Steps the animation of every sprite that is not paused
void func_02014c08(MCSSSystem *system);
// pokebw2's MCSSSys_Draw
void func_02014d2c(MCSSSystem *system);
// pokebw2's MCSSSys_Add
MCSS *func_020159c8(MCSSSystem *system, fx32 x, fx32 y, fx32 z, const MCSSLoadInfo *info);
// pokebw2's MCSS_SetScale
void func_02015cac(MCSS *mcss, VecFx32 *scale);
// pokebw2's MCSS_SetAnimationEndCallback
// Sets the callback that runs when the sprite's animation reaches its last frame
void func_02015f2c(MCSS *mcss, uintptr_t param, MCSSAnimationCallback callback, u32 unused);
// SetupPokemonLoaderByBoxData for a party Pokémon
void func_02017100(PartyPkm *pkm, MCSSLoadInfo *info, u32 a2);
// pokebw2's MCSS_Show
void func_02015eb0(MCSS *mcss);
// pokebw2's MCSS_Hide
void func_02015ea0(MCSS *mcss);
// pokebw2's MCSS_SetAnimation
void func_02016028(MCSS *mcss, u32 animation);
// pokebw2's MCSS_ResumeAnimation
void func_02015e4c(MCSS *mcss);
// pokebw2's MCSS_SetAlpha
// Alpha from 0 to 31
void func_02015fbc(MCSS *mcss, u8 alpha);
void func_02015db0(MCSS *mcss);
void func_02015dc0(MCSS *mcss);
// pokebw2's MCSS_RestartAnimation
void func_02016374(MCSS *mcss);
void func_02014cec(MCSSSystem *system, TCBManager *tcbMgr);
u16 func_020160cc(MCSS *mcss);
void func_020160d4(MCSSSystem *system, MCSS *mcss, u8 evy, u32 color);
void func_0201624c(MCSSSystem *system, MCSS *mcss);

#endif // BWPORT_SYSTEM_MCSS_H
