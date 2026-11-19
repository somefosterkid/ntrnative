#include "global.h"
#include "overlay_99_pokeathlon_event_record.h"

s32 ov99_021E94CC(void *, u16, s32, s32);

s32 ov99_021E8544(void *arg0, s32 arg1);

s32 ov99_021E8544(void *arg0, s32 arg1) {
    if (ov99_021E94CC(*(void **)arg0, ((PokeathlonEventRecord *)arg0)->cursor, arg1, *((s32 *)&((PokeathlonEventRecord *)arg0)->linkMode)) == 0xFFFF) {
        return 1;
    }
    return 0;
}
