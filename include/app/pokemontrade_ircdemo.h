// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pokemontrade_ircdemo.h: the lines in use
// so far.
#ifndef BWPORT_APP_POKEMONTRADE_IRCDEMO_H
#define BWPORT_APP_POKEMONTRADE_IRCDEMO_H

#include "types.h"
#include "app/pokemon_trade_local.h"
#include "system/mcss.h"

// pokebw2's PokemonTradeIrcDemo_Init
// The step that sets the scene up, after the common file's steps
void func_ov117_021d4bf4(PokemonTradeWork *wk);
// pokebw2's PokemonTradeIrcDemo_ShowActor
// Creates the sprite that the scene's effects use, or restarts it
void func_ov117_021d434c(PokemonTradeWork *wk);
// pokebw2's PokemonTradeIrcDemo_SetTimer
// Sets the frame counter of the scene
void func_ov117_021d4484(PokemonTradeWork *wk, int timer);
// pokebw2's PokemonTradeIrcDemo_Finish
// Frees the effects and goes on to the save
void func_ov117_021d49c4(PokemonTradeWork *wk);
// pokebw2's PokemonTradeIrcDemo_ShowSprite
// Shows a sprite still, in front of the scene
void func_ov117_021d49f4(MCSS *mcss, PokemonTradeWork *wk);
// pokebw2's PokemonTradeIrcDemo_StartMove
// Plays the voice of the other machine's Pokémon and makes a sprite hop or bounce where it is
void func_ov117_021d4a48(PokemonTradeWork *wk, int index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PokemonTradeIrcDemo_LoadResources
// Loads the sprite's resources
void func_ov117_021d43c8(PokemonTradeWork *wk);
// pokebw2's PokemonTradeIrcDemo_Frames
// Does nothing and returns its argument. The frame that a step happens at goes through it
int func_ov117_021d4480(int frame);

#endif // BWPORT_APP_POKEMONTRADE_IRCDEMO_H
