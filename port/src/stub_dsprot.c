// The PC build's DS Protect (lib/dsprot, overlay 230, which the PC build doesn't compile): the six checks the game
// calls answer as on a genuine cartridge on a DS. Each of lib/dsprot/src/dsprot_main.c's checks runs its tests and
// calls callback(param) when they say what the check's name says, returning what the callback returns; otherwise
// it returns the tests' sum, which is never what the game's callbacks return (~param). On a genuine cartridge, the
// ROM test and the MAC address test pass and the dummy test always passes, so:
//   DetectFlashcart, DetectEmulator and DetectDummy never call the callback (the game's anti-piracy functions:
//     src/system/game_system.c's func_02011d40 and func_02011d7c add the anti-piracy VBlank task);
//   DetectNotFlashcart, DetectNotEmulator and DetectNotDummy always do.
// The callers (src/system/game_system.c, src/ov010/event_battle.c, src/ov020/event_mapchange.c) pass a callback
// that calls param and returns ~param, and compare the result with ~param as a u32.
#include "types.h"
#include "dsprot/dsprot.h"

typedef u32 (*StubDSProtCallback)(void *param);

// What a check returns when it doesn't call the callback: the queue's sum in dsprot_main.c, a product of primes plus
// the tests' results; any value other than ~param does, and param is a function's address, so ~param is never 0
#define STUB_DSPROT_NOT_CALLED 0

static u32 StubDSProt_Call(void *callback, void *param) {
    return ((StubDSProtCallback)callback)(param);
}

// __DSProt_DetectFlashcart: the ROM test passes, so the callback isn't called
u32 func_ov230_021882a0(void *callback, void *param, u32 unused) {
    (void)callback, (void)param, (void)unused;
    return STUB_DSPROT_NOT_CALLED;
}

// __DSProt_DetectNotFlashcart
u32 func_ov230_021882dc(void *callback, void *param, u32 unused) {
    (void)unused;
    return StubDSProt_Call(callback, param);
}

// __DSProt_DetectEmulator: the MAC address test passes
u32 func_ov230_02188318(void *callback, void *param, u32 unused) {
    (void)callback, (void)param, (void)unused;
    return STUB_DSPROT_NOT_CALLED;
}

// __DSProt_DetectNotEmulator
u32 func_ov230_02188354(void *callback, void *param, u32 unused) {
    (void)unused;
    return StubDSProt_Call(callback, param);
}

// __DSProt_DetectDummy: the dummy test never fails
u32 func_ov230_02188390(void *callback, void *param, u32 unused) {
    (void)callback, (void)param, (void)unused;
    return STUB_DSPROT_NOT_CALLED;
}

// __DSProt_DetectNotDummy
u32 func_ov230_021883cc(void *callback, void *param, u32 unused) {
    (void)unused;
    return StubDSProt_Call(callback, param);
}
