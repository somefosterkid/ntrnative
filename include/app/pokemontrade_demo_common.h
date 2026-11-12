// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pokemontrade_demo_common.h: the lines in
// use so far.
#ifndef BWPORT_APP_POKEMONTRADE_DEMO_COMMON_H
#define BWPORT_APP_POKEMONTRADE_DEMO_COMMON_H

#include "types.h"
#include "app/pokemon_trade_local.h"
#include "spl/spl.h"

// pokebw2's PokemonTradeDemo_InitEmitter0
// The emitters' callbacks, which make them follow the paths of the 3D effects
void func_ov118_021d4a34(SPLEmitter *emitter);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PokemonTradeDemo_ShowActor
// Creates the sprite that the scenes' effects use, or restarts it
void func_ov118_021d434c(PokemonTradeWork *wk);
// pokebw2's PokemonTradeDemo_LoadResources
// Loads the sprite's resources
void func_ov118_021d43c8(PokemonTradeWork *wk);
// pokebw2's PokemonTradeDemo_Frames
// Does nothing and returns its argument. The frame that a scene's step happens at goes through it
int func_ov118_021d4480(int frame);
// pokebw2's PokemonTradeDemo_SetTimer
// Sets the frame counter of the scene
void func_ov118_021d4484(PokemonTradeWork *wk, int timer);
// pokebw2's PokemonTradeDemo_Finish
// Frees the effects and goes on to the save
void func_ov118_021d49c4(PokemonTradeWork *wk);
// pokebw2's PokemonTradeDemo_InitEmitter1
void func_ov118_021d4a94(SPLEmitter *emitter);

#endif // BWPORT_APP_POKEMONTRADE_DEMO_COMMON_H
