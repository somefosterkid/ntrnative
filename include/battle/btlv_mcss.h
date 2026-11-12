// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_mcss.h: the lines in use so far.
#ifndef BWPORT_BATTLE_BTLV_MCSS_H
#define BWPORT_BATTLE_BTLV_MCSS_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// pokebw2's BtlvMcss_SetAnimation
BOOL func_ov094_0220041c(BtlvMcss *work, int pos, int anim);

#include "gfl/heap.h"
#include "nitro/fx/fx.h"

// A sprite's position: 0 and 1 the Pokémon of a single battle, 2 to 7 those of the other battles, even on the player's
// side, and from 8 on the trainers. An enum, so MWCC keeps the first test of the loops over the positions
typedef enum {
    BTLV_MCSS_POS_FIRST = 0,
    BTLV_MCSS_POS_TRAINER = 8,
    BTLV_MCSS_POS_MAX = 14,
} BtlvMcssPos;

// Black 2's, not checked against Black
// What BtlvMcss_GetFlags returns of a sprite
#define BTLV_MCSS_FLAG_UNK0 (1 << 0)       // set and cleared with the substitute

// Black 2's, not checked against Black
#define BTLV_MCSS_FLAG_RARE (1 << 1)       // shiny
#define BTLV_MCSS_FLAG_SUBSTITUTE (1 << 2) // the sprite shows the substitute
#define BTLV_MCSS_FLAG_N_POKEMON (1 << 3)  // one of N's Pokémon
#define BTLV_MCSS_FLAG_FAMOUS (1 << 4)     // a Pokéstar fame of 4 or more

// A move of a sprite around a circle, which BtlvMcss_MoveCircle copies from pos on into its task
struct BtlvMcssCircle {
    BtlvMcss *work;   // 0x00
    s32 pos;          // 0x04
    s32 mode;         // 0x08  bit 0: the other way round; bits 1-2: the plane, 0: y-z, 1: x-z, 2: x-y
    u32 start;        // 0x0c  the point of the circle the sprite starts from, 0-3; mirrored for the enemy side
    fx32 radius1;     // 0x10
    fx32 radius2;     // 0x14
    s32 frames;       // 0x18  steps by turn
    s32 wait;         // 0x1c  frames between steps
    s32 count;        // 0x20  turns
    s32 turnWait;     // 0x24  frames to wait after each turn
    s32 angle;        // 0x28
    s32 speed;        // 0x2c
    s32 waitCount;    // 0x30
    s32 turnWaitLeft; // 0x34
    u32 seq;          // 0x38  the position's move counter when the task started
};

// Black 2's; twinned code confirms the offsets of work, pos, axis, angle, speed, amplitude, frames and seq in Black
// A shake of a sprite along an axis by a sine, which BtlvMcss_Shake copies into its task
struct BtlvMcssShake {
    BtlvMcss *work; // 0x00
    s32 pos;        // 0x04
    VecFx32 start;  // 0x08  the position, read when the shake starts
    s32 axis;       // 0x14  0: x, otherwise y
    fx32 angle;     // 0x18  the sine's angle, a 16-bit angle in fx32
    fx32 speed;     // 0x1c
    fx32 amplitude; // 0x20
    s32 frames;     // 0x24
    u32 seq;        // 0x28  the position's move counter when the task started
};

// pokebw2's BtlvMcss_Delete
void func_ov094_021feb78(BtlvMcss *work);
// pokebw2's BtlvMcss_Remove
void func_ov094_021ff0f8(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_SetSideScale
void func_ov094_021ff134(BtlvMcss *work, fx32 scaleNear, fx32 scaleFar);
// pokebw2's BtlvMcss_SetPosition
void func_ov094_021ff144(BtlvMcss *work, int pos, fx32 x, fx32 y, fx32 z);
// pokebw2's BtlvMcss_SetFlatAll
void func_ov094_021ff16c(BtlvMcss *work);
// pokebw2's BtlvMcss_ClearFlatAll
void func_ov094_021ff1b0(BtlvMcss *work);
// pokebw2's BtlvMcss_SetFlat
void func_ov094_021ff1f4(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_ClearFlat
void func_ov094_021ff228(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_SetAnimPause
void func_ov094_021ff304(BtlvMcss *work, int pos, int mode);
// pokebw2's BtlvMcss_SetVanish
void func_ov094_021ff374(BtlvMcss *work, int pos, int mode);
// pokebw2's BtlvMcss_GetDefaultPosForStyle
void func_ov094_021ff3fc(BtlvMcss *work, VecFx32 *out, int pos, u32 battleStyle);
// pokebw2's BtlvMcss_MovePosition
void func_ov094_021ff4c0(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_MoveVec518
void func_ov094_021ff5b0(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_MoveScale
void func_ov094_021ff630(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_MoveVec524
void func_ov094_021ff6b0(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_MoveVec51c
void func_ov094_021ff730(BtlvMcss *work, int pos, int type, VecFx32 *dest, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_SwapPositions
void func_ov094_021ff7b0(BtlvMcss *work, int pos1, int pos2);
// pokebw2's BtlvMcss_MoveAlpha
void func_ov094_021ff924(BtlvMcss *work, int pos, int type, int alpha, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_MoveLevel
void func_ov094_021ffbbc(BtlvMcss *work, int pos, int type, int level, s32 frames, s32 wait, s32 count);
// pokebw2's BtlvMcss_IsBusy
BOOL func_ov094_021ffcb0(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_IsAnyBusy
BOOL func_ov094_021ffd6c(BtlvMcss *work);
// pokebw2's BtlvMcss_StartPaletteFade
void func_ov094_021ffdb4(BtlvMcss *work, int pos, u8 startEvy, u8 endEvy, u8 wait, u32 color);
// pokebw2's BtlvMcss_GetWeight
u16 func_ov094_021fff0c(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_GetFlags
u32 func_ov094_021fff38(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_IsNoBounce
BOOL func_ov094_021fff68(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_ReloadSprite
void func_ov094_022001dc(BtlvMcss *work, int pos, const MCSSLoadInfo *info);
// pokebw2's BtlvMcss_PlayCry
u32 func_ov094_02200218(BtlvMcss *work, int pos, s32 speed, int volume, int arg4, int arg5, int arg6);
// pokebw2's BtlvMcss_GetBall
u32 func_ov094_022003c4(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_SetupPokemonLoadInfo
void func_ov094_022003ec(BtlvMcss *work, PartyPkm *pkm, MCSSLoadInfo *info, int pos);
// pokebw2's BtlvMcss_WatchAnimationEnd
void func_ov094_02200468(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_IsAnimationSet
BOOL func_ov094_02200494(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_ResetPosition
void func_ov094_022004bc(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_SetUnkB270
void func_ov094_0220055c(BtlvMcss *work, int pos, BOOL check);
// pokebw2's BtlvMcss_CheckChanged
BOOL func_ov094_022005c4(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_SaveVanish
void func_ov094_02200608(BtlvMcss *work);
// pokebw2's BtlvMcss_RestoreVanish
void func_ov094_02200654(BtlvMcss *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvMcss_Create
BtlvMcss *func_ov094_021feac0(u32 battleStyle, TCBManager *tcbMgr, HeapID heapId);
// pokebw2's BtlvMcss_Main
void func_ov094_021febac(BtlvMcss *work);
// pokebw2's BtlvMcss_Draw
void func_ov094_021fee9c(BtlvMcss *work);
// pokebw2's BtlvMcss_AddPokemon
void func_ov094_021feea8(BtlvMcss *work, PartyPkm *pkm, int pos);
// pokebw2's BtlvMcss_AddTrainer
void func_ov094_021ff088(BtlvMcss *work, u32 trainerType, int pos);
// pokebw2's BtlvMcss_GetVanish
BOOL func_ov094_021ff348(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_GetDefaultPos
void func_ov094_021ff3f4(BtlvMcss *work, VecFx32 *out, int pos);
// pokebw2's BtlvMcss_Blink
void func_ov094_021ff870(BtlvMcss *work, int pos, int type, s32 wait, s32 count);
// pokebw2's BtlvMcss_MoveCircle
void func_ov094_021ffa18(BtlvMcss *work, BtlvMcssCircle *param);
// pokebw2's BtlvMcss_Shake
void func_ov094_021ffb04(BtlvMcss *work, BtlvMcssShake *param);
// pokebw2's BtlvMcss_Exists
BOOL func_ov094_021ffd9c(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_ChangePokemon
void func_ov094_021ffe48(BtlvMcss *work, int pos, PartyPkm *pkm);
// pokebw2's BtlvMcss_GetUnk
u8 func_ov094_021fffac(BtlvMcss *work, int pos);
// pokebw2's BtlvMcss_SetSubstitute
void func_ov094_021fffd8(BtlvMcss *work, int pos, BOOL substitute, BOOL flag);
// pokebw2's BtlvMcss_Transform
void func_ov094_02200140(BtlvMcss *work, int targetPos, int pos);
// pokebw2's BtlvMcss_StartRotation
void func_ov094_02200344(BtlvMcss *work, int side, int dir, BOOL playSe);
// pokebw2's BtlvMcss_SetShadowVanish
void func_ov094_021ff470(BtlvMcss *work, int pos, u32 value);
// pokebw2's BtlvMcss_GetDefaultScale
fx32 func_ov094_021ff468(BtlvMcss *work, int pos, BOOL flat);

#endif // BWPORT_BATTLE_BTLV_MCSS_H

