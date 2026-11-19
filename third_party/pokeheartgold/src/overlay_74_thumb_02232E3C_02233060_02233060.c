#include "global.h"
struct GBAMigrator;

#include "palPark_migration.h"
#include "save_arrays.h"
#include "overlay_74_gbamigrator.h"

s32 AGB_GetBoxMonData(void *, s32, void *);

s32 ov74_02233060(struct GBAMigrator *arg0);

s32 ov74_02233060(GBAMigrator *arg0) {
    MigratedPokemon *sp8;
    s32 sp4;
    void *sp0;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r5;
    s32 var_r6;
    s32 var_r7;
    sp0 = arg0;
    sp8 = Save_MigratedPokemon_Get(arg0->saveData);
    var_r6 = 0;
    sp4 = 0;
    var_r7 = 0;
    do {
        var_r4 = 0;
        var_r5 = 0;
loop_2:
        if (AGB_GetBoxMonData(((4 + (*((u8 **)&((GBAMigrator *)sp0)->pokemonStorage))) + var_r7) + (temp_r0 = var_r5), 5, 0) != 0) {
            var_r6 += 1;
        }

        var_r4 += 1;
        var_r5 += 0x50;
        if (var_r4 < 0x1E) {
            goto loop_2;
        }
        var_r7 += 0x960;
        temp_r0 = 1 + sp4;
        sp4 = temp_r0;
    } while (temp_r0 < 0xE);
    if (var_r6 < 6) {
        return 5;
    }
    if (MigratedPokemon_CountPokemon(sp8) != 0) {
        return 6;
    }
    return 0;
}
