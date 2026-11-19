#include "global.h"
#include "seal_case.h"
#include "pokemon_types_def.h"

u16 sub_0202DB54(void *arg0);

typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
typedef struct GlobalTrade {
    u8 storedPokemon[sizeof(Pokemon)];
    u16 pokemonListed;
    u16 neverSet_EE;
    u32 depositTradeDatestamp;
    u32 searchTradeDatestamp;
} GlobalTrade;
void sub_0202DB64(GlobalTrade *globalTrade, Pokemon *mon);
void sub_0202DB70(GlobalTrade *globalTrade, Pokemon *mon, int unused);

u16 sub_0202DB54(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0xEC);
}

void sub_0202DB5C(void *arg0, s16 arg1);

void sub_0202DB5C(void *arg0, s16 arg1) {
    *(s16 *)((u8 *)arg0 + 0xEC) = arg1;
}

void sub_0202DB64(GlobalTrade *globalTrade, Pokemon *mon) {
    MI_CpuCopyFast(globalTrade->storedPokemon, (void *)mon, sizeof(Pokemon));
}

void sub_0202DB70(GlobalTrade *globalTrade, Pokemon *mon, int unused) {
    MI_CpuCopyFast((void *)mon, globalTrade->storedPokemon, sizeof(Pokemon));
}
