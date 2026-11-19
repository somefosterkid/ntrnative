#include "global.h"

#include "pokemon_summary_app.h"

#include "system.h"
#include "unk_0208805C.h"
#include "screen_fade.h"

s32 sub_02089658(void);

s32 sub_02089670(void);

u8 sub_02089680(PokemonSummary *dummy);

s32 sub_02089658(void) {
    if (3 & gSystem.newKeys) {
        return 0x15;
    }
    return 0x12;
}

s32 sub_02089670(void) {
    App_StartScreenFade(1U, HEAP_ID_19);
    return 0x16;
}

u8 sub_02089680(PokemonSummary *dummy) {
    return IsPaletteFadeFinished() == 1;
}
