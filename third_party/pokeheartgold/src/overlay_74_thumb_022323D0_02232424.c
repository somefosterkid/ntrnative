#include "global.h"
#include "overlay_74_gbamigrator.h"
struct GBAMigrator;

s32 AGB_GetBoxMonData(void *, s32, void *);

extern s32 ov74_0223C9A8;

extern u16 ov74_0223C9D8;

s32 ov74_02232424(struct GBAMigrator *arg0, s32 arg1);

s32 ov74_02232424(GBAMigrator *arg0, s32 arg1) {
    s32 temp_r0;
    u16 *var_r2;
    temp_r0 = AGB_GetBoxMonData((((*((u8 **)&arg0->pokemonStorage)) + 4) + (0x960 * (*((s32 *)&arg0->currentBox)))) + (0x50 * arg1), 0xC, 0);
    if ((*((u16 *)(((u8 *)(&ov74_0223C9A8)) + 0x30))) != 0) {
        var_r2 = &ov74_0223C9D8;
        do {
            if (temp_r0 == (*var_r2)) {
                return 1;
            }

            var_r2 += 1;
        } while ((*var_r2) != 0);
    }
    return 0;
}
