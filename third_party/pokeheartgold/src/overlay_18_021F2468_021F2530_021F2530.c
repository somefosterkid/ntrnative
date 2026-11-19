#include "global.h"

#define OV18_021F2530_DEFINITION   /* see pokedex_internal.h: this file defines it with the u16 species */

#include "application/pokedex/pokedex_internal.h"
#include "pokedex.h"
#include "sprite_system.h"
#include "unk_02091278.h"
#include "unk_020912AC.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F2530(PokedexAppData *pokedexApp, u32 species, u32 first);
#else
void ov18_021F2530(PokedexAppData *pokedexApp, u16 species, u32 first);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F2530(PokedexAppData *pokedexApp, u32 species, u32 first) {
#else
void ov18_021F2530(PokedexAppData *pokedexApp, u16 species, u32 first) {
#endif
    s16 i;
    s16 shown;
    int diff;
    int lang;
    int dexFlag;
    int idx;
    s16 x, y;

    if (species == 0 || !Pokedex_GetInternationalViewFlag(pokedexApp->args->pokedex)) {
        for (i = first; i < first + 6; i++) {
            ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[i], FALSE);
        }
        return;
    }

    shown = 0;
    for (i = first + 5; i >= first; i--) {
        diff = i - first;
        lang = sub_02091294(sub_020912AC(diff));
        if (ov18_021E6D10(pokedexApp, species, lang) == TRUE || lang == 2) {
            dexFlag = LanguageToDexFlag(pokedexApp->unk_185C);
            idx = sub_020912AC(diff);
            if (idx == dexFlag) {
                ov18_021F118C(pokedexApp, i, idx);
            } else {
                ov18_021F118C(pokedexApp, i, idx + 6);
            }
            ManagedSprite_GetPositionXY(pokedexApp->unk_0670[i], &x, &y);
            ManagedSprite_SetPositionXY(pokedexApp->unk_0670[i], (5 - shown) * 24 + 124, y);
            ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[i], TRUE);
            shown++;
        } else {
            ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[i], FALSE);
        }
    }
}
