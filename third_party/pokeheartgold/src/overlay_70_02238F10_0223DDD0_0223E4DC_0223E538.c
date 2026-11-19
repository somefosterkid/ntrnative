#include "global.h"

#include "pokemon.h"
#include "pokemon_types_def.h"

u32 ov70_0223E538(BoxPokemon *param_1);

u32 ov70_0223E538(BoxPokemon *param_1) {
    BOOL decrypt_result;
    u32 uVar1;
    u32 uVar2;

    decrypt_result = AcquireBoxMonLock(param_1);
    uVar1 = GetBoxMonData(param_1, 5, (void *)0x0);
    uVar2 = GetBoxMonData(param_1, 0x70, (void *)0x0);
    ReleaseBoxMonLock(param_1, decrypt_result);
    if ((s32)uVar2 > 0) {
        switch ((s32)uVar1) {
        case 0x1df:
        case 0x1e7:
        case 0x1ec:
            return 1;
        case 0xac:
            return 2;
        }
    }
    return 0;
}
