// The PC build compiles lib/gfl/src/gf_standard.c through this file (tools/port_sources.py). The game's buildSeed (the
// battle's random seed at each battle's start, and the game's own at boot) hashes its digest buffer before the buffer
// holds the digest: 20 bytes of whatever the stack held. On the DS that is the same for the same RAM; on the PC it is
// what host code (libntr, SDL, the C library) left on the thread's stack, which moves with the binary's layout and the
// length of the environment strings (the stack starts below them). So a battle took another course when the build or
// the environment changed (route legs ended on another V-blank: Black's gym4-e, Black 2's nim-elesa and r4-colress;
// found 2026-11-13). Here the buffer is zeroed first: the seed is the hash of OS_GetLowEntropyData's words alone (fixed
// in a bench run, BWPORT_BENCH_CLOCK). The DS build never sees this file.
#pragma push_macro("buildSeed")
#define buildSeed buildSeed_game
#include "../../../../../lib/gfl/src/gf_standard.c"
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
