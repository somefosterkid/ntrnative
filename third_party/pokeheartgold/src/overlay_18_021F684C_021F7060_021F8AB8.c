#include "global.h"
struct PokedexEntryPopup;

#include "pokemon.h"
#include "overlay_18_pokedex_entry_popup.h"


void ov18_021F8AB8(PokedexEntryPopup *arg0) {
    s32 decrypt_result;

    decrypt_result = AcquireMonLock(arg0->mon);
    arg0->species = GetMonData(arg0->mon, 5, NULL);
    arg0->form = GetMonData(arg0->mon, 0x70, NULL);
    arg0->type1 = GetMonData(arg0->mon, 0xB1, NULL);
    arg0->type2 = GetMonData(arg0->mon, 0xB2, NULL);
    ReleaseMonLock(arg0->mon, decrypt_result);
}
