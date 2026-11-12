// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_act_poke.h: the lines in use
// so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACT_POKE_H
#define BWPORT_APP_MUSICAL_STA_ACT_POKE_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/blact.h"
#include "gfl/heap.h"
#include "app/musical/musical_system.h"

// pokebw2's StaActPoke_GetPosition
void func_ov112_021f8044(StaActPokeSys *sys, StaActPoke *poke, VecFx32 *pos);

// pokebw2's StaActPoke_SetScrollOffset
void func_ov112_021f75dc(StaActPokeSys *sys, u16 scroll);
// pokebw2's StaActPoke_TermSystem
void func_ov112_021f7120(StaActPokeSys *sys);
// pokebw2's StaActPoke_UpdateSystem
void func_ov112_021f7160(StaActPokeSys *sys);
// pokebw2's StaActPoke_DrawSystem
void func_ov112_021f75a0(StaActPokeSys *sys);
// pokebw2's StaActPoke_UpdateSystem_Item
// Places the props, after the sprites are drawn and their marker cells known, and runs their effects
void func_ov112_021f718c(StaActPokeSys *sys);
// pokebw2's StaActPoke_CreatePoke
StaActPoke *func_ov112_021f760c(StaActPokeSys *sys, MusicalPoke *musPoke);
// pokebw2's StaActPoke_StopAnime
void func_ov112_021f80b0(StaActPokeSys *sys, StaActPoke *poke);
// pokebw2's StaActPoke_SetShowItem
void func_ov112_021f8154(StaActPokeSys *sys, StaActPoke *poke, BOOL show);
// pokebw2's StaActPoke_SetPosition
void func_ov112_021f8054(StaActPokeSys *sys, StaActPoke *poke, VecFx32 *pos);
// pokebw2's StaActPoke_SetPositionOffset
void func_ov112_021f8084(StaActPokeSys *sys, StaActPoke *poke, VecFx32 *offset);

// Black 2's, not checked against Black
typedef void (*StaActPokeItemFunc)(StaActPokeSys *sys, StaActPoke *poke);

// Black 2's; twinned code confirms the offsets of timer, pos, effect and speed in Black
// The effect of the prop being used
struct StaActPokeItemWork {
    u16 timer;
    // The equip position of the prop
    u32 pos;
    StaActEffect *effect;
    fx32 speed;
    u32 unk10;
};

// Black 2's; twinned code confirms the offsets of active, update, isFront, showItems, noBounce, noFlip, pos, scale,
// offset, rotation, animating, blinkTimer, flip, draw, shadowActor, itemRes, items, equips, showItem, itemWork and
// itemFunc in Black
struct StaActPoke {
    BOOL active;
    // Whether the sprite needs its position, rotation and scale again
    BOOL update;
    BOOL isFront;
    // Whether its props are shown
    BOOL showItems;
    // The personal data's flags that keep it from bouncing and from turning around
    BOOL noBounce;
    BOOL noFlip;
    VecFx32 pos;
    VecFx32 scale;
    VecFx32 offset;
    // How far rotating moves the sprite
    VecFx32 rotOffset;
    u16 rotation;
    BOOL animating;
    // Frames until the sprite's second frame shows, and while it does
    u16 blinkTimer;
    // Whether it faces the other way
    BOOL flip;
    MusPokeDraw *draw;
    u32 shadowActor;
    void *itemRes[9];
    MusItemDraw *items[9];
    MusicalPokeEquip *equips[9];
    BOOL showItem[9];
    StaActPokeItemWork itemWork;
    StaActPokeItemFunc itemFunc;
};

struct StaActPokeSys {
    HeapID heapId;
    // NULL in the photo
    StaActing *stage;
    BlActScene *blact;
    MusPokeDrawSys *pokeDraw;
    MusItemDrawSys *itemDraw;
    StaActPoke pokes[4];
    // How far the stage has scrolled, in pixels
    u16 scroll;
    u32 shadowMaterial;
    // How many props' effects are running, which play a sound until the last ends
    u8 effectCount;
};

// pokebw2's StaActPoke_DeletePoke
void func_ov112_021f77a0(StaActPokeSys *sys, StaActPoke *poke);
// pokebw2's StaActPoke_GetShowFlg
BOOL func_ov112_021f8120(StaActPokeSys *sys, StaActPoke *poke);
// pokebw2's StaActPoke_StartItemEffect
// Starts the effect of the prop at an equip position
void func_ov112_021f79a8(StaActPokeSys *sys, StaActPoke *poke, u32 pos);
// pokebw2's StaActPoke_IsItemEffect
BOOL func_ov112_021f7a80(StaActPokeSys *sys, StaActPoke *poke);
// pokebw2's StaActPoke_SetRotate
void func_ov112_021f8068(StaActPokeSys *sys, StaActPoke *poke, u16 rotation);
// pokebw2's StaActPoke_StartAnime
void func_ov112_021f80a0(StaActPokeSys *sys, StaActPoke *poke);
// pokebw2's StaActPoke_ChangeAnime
void func_ov112_021f80c8(StaActPokeSys *sys, StaActPoke *poke, u8 anime);
// pokebw2's StaActPoke_SetShowFlg
void func_ov112_021f80d4(StaActPokeSys *sys, StaActPoke *poke, BOOL show);
// pokebw2's StaActPoke_SetFlip
void func_ov112_021f812c(StaActPokeSys *sys, StaActPoke *poke, BOOL flip);
// pokebw2's StaActPoke_SetFront
void func_ov112_021f8140(StaActPokeSys *sys, StaActPoke *poke, BOOL front);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActPoke_InitSystem
StaActPokeSys *func_ov112_021f709c(HeapID heapId, StaActing *stage, MusPokeDrawSys *pokeDraw,
                                     MusItemDrawSys *itemDraw, BlActScene *blact);

#endif // BWPORT_APP_MUSICAL_STA_ACT_POKE_H
