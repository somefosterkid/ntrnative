#ifndef POKEHEARTGOLD_UNK_02088288_H
#define POKEHEARTGOLD_UNK_02088288_H

#include "bg_window.h"
#include "filesystem.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokemon_summary_args.h"
#include "pokemon_types_def.h"
#include "save_special_ribbons.h"

extern const OverlayManagerTemplate gOverlayTemplate_PokemonSummary;

BOOL PokemonSummary_Init(OverlayManager *manager, int *state);
BOOL PokemonSummary_Main(OverlayManager *manager, int *state);
BOOL PokemonSummary_Exit(OverlayManager *manager, int *state);

struct PokemonSummary;

u32 sub_02088288(SaveData *);
BOOL sub_0208828C(SaveData *);
void sub_020885DC(void *arg);
void sub_02088610(void);
void sub_02088630(BgConfig *bgConfig);
void sub_020887C4(struct PokemonSummary *summary, NARC *unused, NARC *narc);
void sub_0208877C(BgConfig *bgConfig);
void sub_0208887C(void);
void sub_02088894(struct PokemonSummary *summary);
void sub_0208895C(struct PokemonSummary *summary);
void sub_02088AF8(struct PokemonSummary *summary);
void sub_020889D0(struct PokemonSummary *summary, NARC *narc);
void sub_020897C0(struct PokemonSummary *summary);
void sub_02089CB4(struct PokemonSummary *summary);
void sub_02089D40(PokemonSummaryArgs *, const u8 *);
void sub_0208AD34(PokemonSummaryArgs *, PlayerProfile *);
u32 Pokemon_GetStatusIconId(Pokemon *pokemon);

#endif // POKEHEARTGOLD_UNK_02088288_H
