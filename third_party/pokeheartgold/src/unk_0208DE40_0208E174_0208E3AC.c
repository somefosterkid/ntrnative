#include "global.h"

#include "pokemon.h"
#include "pokepic.h"
#include "unk_0208DE40.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

BoxPokemon *sub_0208A520(void *);

void sub_0208E3AC(PokemonSummary *arg0) {
    PokepicTemplate pokepicTemplate;
    BoxPokemon *boxPokemon;

    arg0->pokepicManager = PokepicManager_Create(HEAP_ID_19);
    boxPokemon = sub_0208A520(arg0);
    if (arg0->args->unk11 == 2) {
        GetBoxmonSpriteCharAndPlttNarcIds(&pokepicTemplate, boxPokemon, 2U, 0);
    } else {
        GetPokemonSpriteCharAndPlttNarcIds(&pokepicTemplate, (Pokemon *)boxPokemon, 2U);
    }
    NARC_ReadPokepicAnimScript(arg0->pokepicAnimNarc, arg0->pokepicAnimScript, arg0->mon.species, 1U);
    *((s32 *)&arg0->pokepicAnimReverseX) = 0;
    arg0->pokepic = PokepicManager_CreatePokepic(arg0->pokepicManager, &pokepicTemplate, 0xD0, 0x68, 0, 0, arg0->pokepicAnimScript, NULL);
    Pokepic_SetAttr(arg0->pokepic, 6, 1);
    Pokepic_SetAttr(arg0->pokepic, 0x23, 0);
}

void sub_0208E444(PokemonSummary *arg0) {
    if (arg0->mon.isEgg != 0) {
        sub_0207294C(arg0->pokepicAnimNarc, arg0->pokepicAnimManager, arg0->pokepic, 0U, 2, *((s32 *)&arg0->pokepicAnimReverseX), 0);
        return;
    }
    Pokepic_StartAnim(arg0->pokepic, 1);
    sub_0207294C(arg0->pokepicAnimNarc, arg0->pokepicAnimManager, arg0->pokepic, arg0->mon.species, 2, *((s32 *)&arg0->pokepicAnimReverseX), 0);
}

s32 sub_02017088(void *, u8);

void sub_0208E4B4(PokemonSummary *arg0) {
    sub_02017088(arg0->pokepicAnimManager, 0);
    PokepicManager_Delete(arg0->pokepicManager);
    sub_0208E3AC(arg0);
    sub_0208E444(arg0);
}
