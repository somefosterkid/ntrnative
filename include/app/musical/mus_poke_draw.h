// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/mus_poke_draw.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUS_POKE_DRAW_H
#define BWPORT_APP_MUSICAL_MUS_POKE_DRAW_H

#include "types.h"
#include "app/musical/musical_mcss.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// Where a prop goes on a Pokémon, from the marker cell of its position
typedef struct {
    // Whether the cell was drawn
    BOOL valid;
    MusicalMcssCellInfo info;
} MusPokeDrawEquipPos;

// pokebw2's MusPokeDraw_DelPoke
void func_ov112_021f5330(MusPokeDrawSys *sys, MusPokeDraw *poke);
// pokebw2's MusPokeDraw_TermSystem
void func_ov112_021f51b0(MusPokeDrawSys *sys);
// pokebw2's MusPokeDraw_DrawSystem
void func_ov112_021f51f4(MusPokeDrawSys *sys);
// pokebw2's MusPokeDraw_AddPoke
// Adds a Pokémon, and its back as well when withBack is set
MusPokeDraw *func_ov112_021f5204(MusPokeDrawSys *sys, MusicalPoke *musPoke, BOOL withBack);
// pokebw2's MusPokeDraw_GetMarkPos4
VecFx32 *func_ov112_021f545c(MusPokeDraw *poke);

// A Pokémon being drawn
struct MusPokeDraw {
    BOOL active;
    // Whether it shows its front rather than its back
    BOOL isFront;
    MusicalMcss *mcss;
    MusicalMcss *front;
    // NULL when it has no back
    MusicalMcss *back;
    MusPokeDrawEquipPos equips[9];
    // The offsets of its marker cells of kinds 4 and 5, and whether they were drawn
    VecFx32 markPos4;
    VecFx32 markPos5;
    BOOL markValid4;
    BOOL markValid5;
};

// Black 2's; twinned code confirms the offsets of mcssSys and pokes in Black
struct MusPokeDrawSys {
    HeapID heapId;
    MusicalMcssSys *mcssSys;
    MusPokeDraw pokes[8];
};

// pokebw2's MusPokeDraw_SetVisible
void func_ov112_021f5374(MusPokeDraw *poke, BOOL visible);
// pokebw2's MusPokeDraw_IsVisible
BOOL func_ov112_021f5388(MusPokeDraw *poke);
// pokebw2's MusPokeDraw_StartAnime
void func_ov112_021f539c(MusPokeDraw *poke);
// pokebw2's MusPokeDraw_TurnAround
// Turns the Pokémon around, to its back or its front
void func_ov112_021f53d8(MusPokeDraw *poke);
// pokebw2's MusPokeDraw_SetFront
void func_ov112_021f5414(MusPokeDraw *poke, BOOL front);
// pokebw2's MusPokeDraw_GetEquipPos
MusPokeDrawEquipPos *func_ov112_021f5450(MusPokeDraw *poke, u32 pos);
// pokebw2's MusPokeDraw_GetMarkPos5
VecFx32 *func_ov112_021f5464(MusPokeDraw *poke);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MusPokeDraw_UpdateSystem
void func_ov112_021f51e8(MusPokeDrawSys *sys);
// pokebw2's MusPokeDraw_InitSystem
MusPokeDrawSys *func_ov112_021f5164(HeapID heapId);
// pokebw2's MusPokeDraw_SetTexBase
void func_ov112_021f5438(MusPokeDrawSys *sys, u32 base);
// pokebw2's MusPokeDraw_SetPlttBase
void func_ov112_021f5444(MusPokeDrawSys *sys, u32 base);
// pokebw2's MusPokeDraw_SetPosition
void func_ov112_021f5350(MusPokeDraw *poke, VecFx32 *pos);
// pokebw2's MusPokeDraw_SetScale
void func_ov112_021f535c(MusPokeDraw *poke, VecFx32 *scale);
// pokebw2's MusPokeDraw_SetRotation
void func_ov112_021f5368(MusPokeDraw *poke, u16 rotation);
// pokebw2's MusPokeDraw_StopAnime
void func_ov112_021f53b4(MusPokeDraw *poke);
// pokebw2's MusPokeDraw_ChangeAnime
void func_ov112_021f53cc(MusPokeDraw *poke, u16 anime);
// pokebw2's MusPokeDraw_SetFlip
void func_ov112_021f5424(MusPokeDraw *poke, BOOL flip);

#endif // BWPORT_APP_MUSICAL_MUS_POKE_DRAW_H
