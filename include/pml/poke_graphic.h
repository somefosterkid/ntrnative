// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/poke_graphic.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_PML_POKE_GRAPHIC_H
#define BWPORT_PML_POKE_GRAPHIC_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nnsys/g2d.h"

// The characters of the whole sprite as one cell
u32 GetPokemonSingleCellCharacterDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);

// pokebw2's PokeGra_LoadClActChars
u32 func_0202d81c(ArcTool *arc, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 personality,
                           u32 vramType, HeapID heapId);
// pokebw2's PokeGra_LoadClActPalette
// Load a Pokémon's palette, its characters (with a Spinda's spots drawn from personality) and its cells with their
// animations as cell actor resources, from the archive MakePokeGraArcHandle opens. The cells are the common ones of
// the app archive for the OBJ mapping
u32 func_0202d7d8(ArcTool *arc, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 vramType,
                             u16 offset, HeapID heapId);
// pokebw2's PokeGra_LoadClActCellAnims
u32 func_0202d898(u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 mapping, u32 vramType,
                               HeapID heapId);

// pokebw2's MakeTrGraArcHandle
// The same for a trainer class's front sprite
ArcTool *func_0202da04(HeapID heapId);
// pokebw2's TrGra_LoadClActPalette
u32 func_0202da18(ArcTool *arc, u32 trainerClass, u32 vramType, u16 offset, HeapID heapId);
// pokebw2's TrGra_LoadClActChars
u32 func_0202da48(ArcTool *arc, u32 trainerClass, u32 vramType, HeapID heapId);
// pokebw2's TrGra_LoadClActCellAnims
u32 func_0202da6c(u32 trainerClass, u32 mapping, u32 vramType, HeapID heapId);

// pokebw2's PokeGra_LoadClActPalette
// Load a Pokémon's palette, its characters (with a Spinda's spots drawn from personality) and its cells with their
// animations as cell actor resources, from the archive MakePokeGraArcHandle opens. The cells are the common ones of
// the app archive for the OBJ mapping
u32 func_0202d7d8(ArcTool *arc, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 vramType,
                             u16 offset, HeapID heapId);
// pokebw2's PokeGra_LoadClActCellAnims
u32 func_0202d898(u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg, u32 mapping, u32 vramType,
                               HeapID heapId);

// pokebw2's TrGra_ReplaceClActCharsAndPalette
// Replaces the data of loaded characters and palette with a trainer class's front sprite
void func_0202daa4(ArcTool *arc, u32 trainerClass, u32 chars, u32 palette, HeapID heapId);

// pokebw2's PokeGra_LoadClActCharsByBoxData
u32 func_0202d934(ArcTool *arc, BoxPkm *pkm, u32 dir, u32 vramType, HeapID heapId);
// pokebw2's PokeGra_LoadClActPaletteByBoxData
// The same for a Pokémon's data
u32 func_0202d8d0(ArcTool *arc, BoxPkm *pkm, u32 dir, u32 vramType, u16 offset, HeapID heapId);
// pokebw2's PokeGra_LoadClActCellAnimsByBoxData
u32 func_0202d9a4(BoxPkm *pkm, u32 dir, u32 mapping, u32 vramType, HeapID heapId);

// Which side of a Pokémon a sprite shows; anything but the front is the back
#define POKEGRA_DIR_FRONT 0

// Black 2's, not checked against Black
#define POKEGRA_DIR_BACK 1

// pokebw2's LoadSingleCellSpindaGraphicsByBoxData
// Loads a Pokémon's sprite as characters of one cell, with a Spinda's spots drawn from its personality, returning the
// file to free
void *func_0202d6f4(NNSG2dCharacterData **chars, BoxPkm *pkm, u32 dir, HeapID heapId);

// pokebw2's PokeGra_CellCharsToImage
// Rearranges a single-cell sprite's characters, its four OBJs one after the other, into one image 12 characters wide
void func_0202d214(NNSG2dCharacterData *chars, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The files of a Pokémon's sprite in GetPokemonGraphicsARCID's archive. Each takes the species, its form and sex,
// whether it is shiny, the direction (POKEGRA_DIR_*) and whether it is an egg. A form or sex without files of its own
// gets the species' files
u32 GetPokemonGraphicsARCID(void);
u32 GetPokemonPaletteDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
// The files of a trainer class's sprite in GetTrainerSpriteARCID's archive, with the flags of GetTrSpriteBaseDatID
u32 GetTrainerSpriteARCID(BOOL back);
u32 GetTrSpritePaletteDatID(u32 trainerClass, u32 flags);
ArcTool *MakePokeGraArcHandle(HeapID heapId);
// The characters of the sprite's parts, which GetPokemonMultiCellsDataNo puts together
u32 GetPokemonCharacterDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
u32 GetPokemonCellsDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
u32 GetPokemonCellAnimeDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
u32 GetPokemonMultiCellsDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
u32 GetPokemonMultiCellAnimeDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
u32 GetPokemonBinFileDataNo(u32 arcId, u32 species, u32 form, u32 sex, BOOL rare, u32 dir, BOOL egg);
u32 GetTrSpriteCharacter2DatID(u32 trainerClass, u32 flags);
u32 GetTrSpriteCellDatID(u32 trainerClass, u32 flags);
u32 GetTrSpriteCellAnmDatID(u32 trainerClass, u32 flags);
u32 GetTrSpriteMultiCellDatID(u32 trainerClass, u32 flags);
u32 GetTrSpriteMultiCellAnmDatID(u32 trainerClass, u32 flags);
u32 GetTrSpriteBinDatID(u32 trainerClass, u32 flags);

#endif // BWPORT_PML_POKE_GRAPHIC_H
