// Black 2's and White 2's buildSeed (port/extern/lib/gfl/src/gf_standard.c's twin, which says why): pokebw2's
// src/gfl/gf_standard.c compiled through this file, its buildSeed with the digest buffer zeroed before it is hashed (the
// buffer held host stack garbage, so a battle's seed moved with the binary's layout and the environment's length).
#pragma push_macro("buildSeed")
#define buildSeed buildSeed_game
#include "../../../../third_party/pokebw2/src/gfl/gf_standard.c"
#pragma pop_macro("buildSeed")
void buildSeed(MATHRandContext32 *context) {
    MATHSHA1Context sha1;
    u32 entropy[8];
    u8 digest[MATH_SHA1_DIGEST_SIZE];
    u64 seed;

    MI_CpuClear8(digest, sizeof(digest));
    OS_GetLowEntropyData(entropy);
    MATH_SHA1Init(&sha1);
    MATH_SHA1Update(&sha1, digest, sizeof(digest));
    MATH_SHA1Update(&sha1, entropy, sizeof(entropy));
    MATH_SHA1GetHash(&sha1, digest);
    MI_CpuCopy8(digest, &seed, sizeof(seed));
    MATH_InitRand32(context, seed);
}
