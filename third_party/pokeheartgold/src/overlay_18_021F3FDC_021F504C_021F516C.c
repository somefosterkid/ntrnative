#include "global.h"

#include "application/pokedex/pokedex_internal.h"

extern BOOL (*ov18_021FA588[17])(PokedexAppData_UnkSub0868_State77_Sub4 *);

BOOL ov18_021F516C(PokedexAppData_UnkSub0868_State77_Sub4 *a0) {
    return (*ov18_021FA588[a0->animIndex])(a0);
}
