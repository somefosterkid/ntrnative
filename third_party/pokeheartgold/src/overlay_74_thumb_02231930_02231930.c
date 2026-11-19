#include "global.h"
struct GBAMigrator;

#include "palPark_migration.h"
#include "save.h"
#include "save_arrays.h"
#include "system.h"
#include "overlay_74_gbamigrator.h"

u32 ov74_02233F68(void);

void ov74_02231930(struct GBAMigrator *arg0);

void ov74_02231930(GBAMigrator *arg0) {
    MigratedPokemon *mon;

    mon = Save_MigratedPokemon_Get(arg0->saveData);
    MigratedPokemon_RecordMigration(mon, ov74_02233F68());
    sub_0201A728(4);
    SaveGameNormal(arg0->saveData);
    sub_0201A738(4);
}
