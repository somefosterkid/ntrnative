// The PC build's DPW (lib/dpw: the GTS's and the Wi-Fi Battle Subway's server library, over GameSpy's game stats),
// which the PC build doesn't compile: with no connection, every request ends at once with the error lib/dpw gives
// when it can't start one (dpw_tr.c's -13, dpw_battle_tower.c's -5), as its IsFinished and GetResult report it. The
// states are lib/dpw's: 1 is idle (a request has ended), 0 is not started.
#include "types.h"
#include "dpw/dpw_bt.h"
#include "dpw/dpw_tr.h"

#define STUB_DPW_TR_NO_REQUEST -13
#define STUB_DPW_BT_NO_REQUEST -5

static int sStubTrState;
static s32 sStubTrResult;
static int sStubBtState;
static s32 sStubBtResult;

static void StubDpwTr_Fail(void) {
    sStubTrState = 1;
    sStubTrResult = STUB_DPW_TR_NO_REQUEST;
}

static void StubDpwBt_Fail(void) {
    sStubBtState = 1;
    sStubBtResult = STUB_DPW_BT_NO_REQUEST;
}

// ---- the GTS (dpw_tr.c) ----

void func_ov114_021c7ba0(s32 pid, u64 friendKey, int a2) {
    (void)pid, (void)friendKey, (void)a2;
    sStubTrState = 1;
    sStubTrResult = 0;
}

void func_ov114_021c7c1c(void) {
}

void func_ov114_021c8658(void) {
    sStubTrState = 0;
}

BOOL func_ov114_021c866c(void) {
    return sStubTrState == 1;
}

s32 func_ov114_021c86a8(void) {
    return sStubTrResult;
}

void func_ov114_021c86b8(Dpw_Tr_Data *data, const void *sign, u32 size) {
    (void)data, (void)sign, (void)size;
    StubDpwTr_Fail();
}

void func_ov114_021c8770(void) {
    StubDpwTr_Fail();
}

void func_ov114_021c87fc(Dpw_Tr_Data *result) {
    (void)result;
    StubDpwTr_Fail();
}

void func_ov114_021c887c(Dpw_Tr_Data *result) {
    (void)result;
    StubDpwTr_Fail();
}

void func_ov114_021c88fc(void) {
    StubDpwTr_Fail();
}

void func_ov114_021c8978(void) {
    StubDpwTr_Fail();
}

// DpwTr_Cancel: nothing is running
void func_ov114_021c89f0(void) {
}

void func_ov114_021c8b18(const Dpw_Tr_PokemonSearchData *search, s32 maxNum, Dpw_Tr_Data *result) {
    (void)search, (void)maxNum, (void)result;
    StubDpwTr_Fail();
}

void func_ov114_021c8bc4(const Dpw_Tr_PokemonSearchDataEx *search, Dpw_Tr_Data *result) {
    (void)search, (void)result;
    StubDpwTr_Fail();
}

void func_ov114_021c8c68(s32 id, Dpw_Tr_Data *upload, Dpw_Tr_Data *result, const void *sign, u32 size) {
    (void)id, (void)upload, (void)result, (void)sign, (void)size;
    StubDpwTr_Fail();
}

void func_ov114_021c8d14(void) {
    StubDpwTr_Fail();
}

void func_ov114_021c8da0(void) {
    StubDpwTr_Fail();
}

void func_ov114_021c8e18(Dpw_Common_Profile *profile, Dpw_Common_ProfileResult *result) {
    (void)profile, (void)result;
    StubDpwTr_Fail();
}

// ---- the Wi-Fi Battle Subway (dpw_battle_tower.c) ----

void func_ov114_021c8f2c(s32 pid, u64 friendKey, int server) {
    (void)pid, (void)friendKey, (void)server;
    sStubBtState = 1;
    sStubBtResult = 0;
}

void func_ov114_021c8fa8(void) {
}

void func_ov114_021c93bc(void) {
    sStubBtState = 0;
}

BOOL func_ov114_021c93d0(void) {
    return sStubBtState == 1;
}

s32 func_ov114_021c940c(void) {
    return sStubBtResult;
}

void func_ov114_021c941c(int kind) {
    (void)kind;
    StubDpwBt_Fail();
}

void func_ov114_021c94a0(int kind, int room, void *result) {
    (void)kind, (void)room, (void)result;
    StubDpwBt_Fail();
}

void func_ov114_021c9530(int kind, int room, u8 win, const void *team, const void *sign, u32 id) {
    (void)kind, (void)room, (void)win, (void)team, (void)sign, (void)id;
    StubDpwBt_Fail();
}

void func_ov114_021c9634(void) {
}

void func_ov114_021c96cc(void) {
    StubDpwBt_Fail();
}

void func_ov114_021c9744(Dpw_Common_Profile *profile, Dpw_Common_ProfileResult *result) {
    (void)profile, (void)result;
    StubDpwBt_Fail();
}
