#include "global.h"
#include "error_handling.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

TransformData *sub_02017220(PokemonAnim *monAnim, u8 funcType) {
    for (u8 i = 0; i < 4; i++) {
        TransformData *retPtr = &(monAnim->transforms[i]);

        if (retPtr->active == 0) {
            MI_CpuClear8(retPtr, sizeof(TransformData));

            retPtr->active = 1;
            retPtr->func = _020F61BC[funcType].func;

            return retPtr;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 0L;
}

// The script word readers: step scriptPtr to the next word and read it. scriptPtr is a whole pointer;
// storing it through a 32-bit raw offset is what broke the 64-bit port.
void sub_0201726C(PokemonAnim *monAnim, int *outInt) {
    monAnim->scriptPtr++;
    *outInt = sub_02017214(monAnim->scriptPtr);
}

void sub_02017280(PokemonAnim *monAnim, u8 *outU8) {
    monAnim->scriptPtr++;
    *outU8 = sub_02017214(monAnim->scriptPtr);
}

void sub_02017294(PokemonAnim *monAnim, u8 *outIndex) {
    monAnim->scriptPtr++;
    *outIndex = sub_02017214(monAnim->scriptPtr);
    if (*outIndex >= NELEMS(monAnim->vars)) {
        GF_AssertFail();
    }
}
