#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "message_format.h"
#include "pm_string.h"
#include "wild_encounter.h"

s32 ov18_021F09D8(PokedexAppData *param_1, s32 param_2);

s32 ov18_021F09D8(PokedexAppData *param_1, s32 param_2) {
    u8 cVar1;
    u16 species;
    s32 bVar2;
    String *string;

    species = param_1->curSpecies;
    switch (species) {
    case 0xC9:
        return 0x79;
    case 0x1A6:
    case 0x1A7:
        return (param_1->seenForms[param_2] ^ 0x80) + 0x74;
    case 0x19C:
    case 0x19D:
        return (param_1->seenForms[param_2] ^ 0x80) + 0x76;
    case 0x182:
        return (param_1->seenForms[param_2] ^ 0x80) + 0x91;
    case 0x1EC:
        return (param_1->seenForms[param_2] ^ 0x80) + 0x95;
    case 0x1E7:
        return (param_1->seenForms[param_2] ^ 0x80) + 0x97;
    case 0x1DF:
        return (param_1->seenForms[param_2] ^ 0x80) + 0x99;
    case 0x15F:
        return (param_1->seenForms[param_2] ^ 0x80) + 0xa0;
    case 0x1A5:
        return (param_1->seenForms[param_2] ^ 0x80) + 0xa4;
    case 0xAC:
        bVar2 = param_1->seenForms[param_2] ^ 0x80;
        if (bVar2 == 0) {
            return 0x72;
        }
        if (bVar2 == 1) {
            return 0x73;
        }
        return 0xa6;
    }
    cVar1 = param_1->seenForms[param_2];
    if (cVar1 == 1) {
        return 0x72;
    }
    if (cVar1 == 2) {
        return 0x73;
    }
    string = ov18_021E590C(species, 2, HEAP_ID_POKEDEX_APP);
    BufferString(param_1->msgFormat, 0, string, 2, 1, 2);
    String_Delete(string);
    return 0x9f;
}
