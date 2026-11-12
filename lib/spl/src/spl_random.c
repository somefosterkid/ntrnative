// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/spl/src/spl_random.c, with Black's names.
#include "types.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_vec.h"
#include "spl_internal.h"

// SPL's random numbers. This compiler emits a file's functions last to first

u32 data_02146a8c;

void func_02058478(VecFx32 *vec) {
    vec->x = SPLRandom_Fx32(24);
    vec->y = SPLRandom_Fx32(24);
    vec->z = SPLRandom_Fx32(24);
    VEC_Normalize(vec, vec);
}

void func_0205841c(VecFx32 *vec) {
    vec->x = SPLRandom_Fx32(24);
    vec->y = SPLRandom_Fx32(24);
    vec->z = 0;
    VEC_Normalize(vec, vec);
}
