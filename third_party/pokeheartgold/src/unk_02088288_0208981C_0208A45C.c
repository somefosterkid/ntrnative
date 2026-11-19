#include "global.h"
struct PokemonSummary;

#include "party.h"
#include "pokemon.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

typedef struct UnkStruct_sub_0208A45C_v0 {
    Party *unk_0;
    u8 unk_4[0xF];
    u8 unk_13;
} UnkStruct_sub_0208A45C_v0;

s32 sub_0208A3CC(void *);

s8 sub_0208A45C(struct PokemonSummary *arg0, s32 arg1);

s8 sub_0208A45C(PokemonSummary *arg0, s32 arg1) {
    Pokemon *mon;
    s8 slot;
    UnkStruct_sub_0208A45C_v0 *v0;

    slot = arg0->args->partySlot;
loop_1:
    slot += arg1;
    if (((s32)slot < 0) || (v0 = *((void * *)&arg0->args), ((s32)slot >= (s32) v0->unk_13))) {
        return -1;
    }
    mon = Party_GetMonByIndex(v0->unk_0, (s32)slot);
    if ((GetMonData(mon, 5, NULL) != 0) && ((GetMonData(mon, 0x4C, NULL) == 0) || (sub_0208A3CC(arg0) == 1))) {
        return slot;
    }
    goto loop_1;
}
