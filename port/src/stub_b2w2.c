// Black 2's and White 2's names (pokebw2's, the maintainers' notes) for the online library functions Black's stubs define under
// Black's names (port/src/stub_online.c, stub_dpw.c: each func_ there carries the SDK's name in its comment): Black 2's
// C calls them by the SDK's names. Each forwards to Black's stub, which keeps its meaning (no access point, no
// connection; the user data and friend keys as the SDK's). The types are the C ABI's: a pointer as void *, the
// integers as the stubs take them. Black's build references none of these names.
#include <string.h>   // memmove (MI_CpuCopy below)

// DWC_InitForProduction: Black's DWC_Init (DWC_INIT_RESULT_NOERROR; it keeps the game's allocator for DWC_Alloc)
int func_02059f6c(const char *gameName, unsigned gameCode, void *alloc, void *free);
int DWC_InitForProduction(const char *gameName, unsigned gameCode, void *alloc, void *free) {
    return func_02059f6c(gameName, gameCode, alloc, free);
}

unsigned func_ov009_0215c910(void *);
unsigned CPS_Resolve(void * a0) { return func_ov009_0215c910(a0); }   // stub_online.c
void func_ov114_021cab1c(void *, void *, unsigned);
void Crypt_SetAllocator(void * a0, void * a1, unsigned a2) { func_ov114_021cab1c(a0, a1, a2); }   // stub_online.c
int func_ov114_021caa70(void *, unsigned, void *, void *);
int Crypt_VerifySignature(void * a0, unsigned a1, void * a2, void * a3) { return func_ov114_021caa70(a0, a1, a2, a3); }   // stub_online.c
int func_0205984c(void *);
int DWC_CheckDirtyFlag(void * a0) { return func_0205984c(a0); }   // stub_online.c
int func_02059354(void *, unsigned long long);
int DWC_CheckFriendKey(void * a0, unsigned long long a1) { return func_02059354(a0, a1); }   // stub_online.c
int func_0205c88c(void);
int DWC_CheckInet(void) { return func_0205c88c(); }   // stub_online.c
void func_0205cc20(void);
void DWC_CleanupInet(void) { func_0205cc20(); }   // stub_online.c
void func_02059870(void *);
void DWC_ClearDirtyFlag(void * a0) { func_02059870(a0); }   // stub_online.c
void func_0205c758(void);
void DWC_ConnectInetAsync(void) { func_0205c758(); }   // stub_online.c
unsigned long long func_0205994c(void *);
unsigned long long DWC_CreateFriendKey(void * a0) { return func_0205994c(a0); }   // stub_online.c
void func_020596fc(void *);
void DWC_CreateUserData(void * a0) { func_020596fc(a0); }   // stub_online.c
int func_020598ec(void *, void *);
int DWC_GetGsProfileId(void * a0, void * a1) { return func_020598ec(a0, a1); }   // stub_online.c
int func_0205ca20(void);
int DWC_GetInetStatus(void) { return func_0205ca20(); }   // stub_online.c
int func_0205ccd8(void);
int DWC_GetLinkLevel(void) { return func_0205ccd8(); }   // stub_online.c
void func_0205c6ac(void *, unsigned short, unsigned short, unsigned short);
void DWC_InitInetEx(void * a0, unsigned short a1, unsigned short a2, unsigned short a3) { func_0205c6ac(a0, a1, a2, a3); }   // stub_online.c
int func_020596d8(void *);
int DWC_IsValidFriendData(void * a0) { return func_020596d8(a0); }   // stub_online.c
int func_ov009_02168124(void);
int DWC_NASLoginProcess(void) { return func_ov009_02168124(); }   // stub_online.c
int func_ov114_021c5db4(void);
int DWC_NdCancelAsync(void) { return func_ov114_021c5db4(); }   // stub_online.c
int func_ov114_021c5b80(void);
int DWC_NdCleanupAsync(void) { return func_ov114_021c5b80(); }   // stub_online.c
int func_ov114_021c5cfc(void *, void *, unsigned);
int DWC_NdGetFileAsync(void * a0, void * a1, unsigned a2) { return func_ov114_021c5cfc(a0, a1, a2); }   // stub_online.c
int func_ov114_021c5c38(void *, unsigned, unsigned);
int DWC_NdGetFileListAsync(void * a0, unsigned a1, unsigned a2) { return func_ov114_021c5c38(a0, a1, a2); }   // stub_online.c
int func_ov114_021c5e14(void *, void *);
int DWC_NdGetProgress(void * a0, void * a1) { return func_ov114_021c5e14(a0, a1); }   // stub_online.c
int func_ov114_021c5928(void *, void *, void *);
int DWC_NdInitAsync(void * a0, void * a1, void * a2) { return func_ov114_021c5928(a0, a1, a2); }   // stub_online.c
int func_ov114_021c5c04(void *, void *, void *);
int DWC_NdSetAttr(void * a0, void * a1, void * a2) { return func_ov114_021c5c04(a0, a1, a2); }   // stub_online.c
void func_0205c8e0(void);
void DWC_ProcessInet(void) { func_0205c8e0(); }   // stub_online.c
int func_ov009_02177138(void *, int);
int DWC_SetUserRecvCallback(void * a0, int a1) { return func_ov009_02177138(a0, a1); }   // stub_online.c
int DWC_SetUserRecvTimeoutCallback(void * a0, int a1) { return func_ov009_02177138(a0, a1); }   // stub_online.c
int DWC_SetUserSendCallback(void * a0, int a1) { return func_ov009_02177138(a0, a1); }   // stub_online.c
int func_0205d074(void);
int DWC_UpdateConnection(void) { return func_0205d074(); }   // stub_online.c
int func_0205d01c(void *, void *, void *);
int DWCi_BssDescToApInfo(void * a0, void * a1, void * a2) { return func_0205d01c(a0, a1, a2); }   // stub_online.c
unsigned func_0205d010(void);
unsigned DWCi_GetBssDescParseWorkSize(void) { return func_0205d010(); }   // stub_online.c
void func_ov114_021c89f0(void);
void DpwTr_Cancel(void) { func_ov114_021c89f0(); }   // stub_dpw.c
unsigned func_ov175_021e1434(void);
unsigned Gdb_GetState(void) { return func_ov175_021e1434(); }   // stub_online.c
unsigned func_ov175_021e11f0(void);
unsigned Gdb_Shutdown(void) { return func_ov175_021e11f0(); }   // stub_online.c

// NCFG_ReadConfigEx: Black's reading of the console's Wi-Fi settings (a console never set up)
int func_0205faa8(void *pages, int unused);
int NCFG_ReadConfigEx(void *work, int unused) { return func_0205faa8(work, unused); }

// CPS_SslAddRandomSeed: the SSL library's entropy, which no connection uses
void CPS_SslAddRandomSeed(void *seed, unsigned size) { (void)seed, (void)size; }

// IrCheck_IsGenuineCard (port/exclude_b2w2.txt leaves src/ov338/ir_check.c out: it waits on the card SPI registers):
// a genuine cartridge, as Black's func_ov231_021809d8 in port/src/stub_ir.c
int IrCheck_IsGenuineCard(void) { return 1; }

// ---- DS Protect (pokebw2's lib/dsprot, overlay 337 and overlay 165's copy; not compiled): a genuine cartridge ----
// The callers pick a pair of functions, the pass one and the tamper one, at random by the V-blank count's low bit
// (data_ov337_02182440 is the pass one's index in data_ov337_02182444), and call DSProt_CallRunChecks1 to 4, which on
// a genuine cartridge run their checks and call the pass one. The callers' checksums of these functions' first 37
// words are left out on the PC (pokebw2-dsprot.patch): the checks' code is not ARM code here. As Black's
// port/src/stub_dsprot.c.
typedef void *(*StubDSProtCallback)(void *arg0, void *arg1);
unsigned data_ov337_02182440;
StubDSProtCallback data_ov337_02182444[2];
void DSProt_Init(void) {}   // decrypts the checks' code
void *DSProt_CallRunChecks1(void *a0, void *a1) { return data_ov337_02182444[data_ov337_02182440](a0, a1); }
void *DSProt_CallRunChecks2(void *a0, void *a1) { return data_ov337_02182444[data_ov337_02182440](a0, a1); }
void *DSProt_CallRunChecks3(void *a0, void *a1) { return data_ov337_02182444[data_ov337_02182440](a0, a1); }
void *DSProt_CallRunChecks4(void *a0, void *a1) { return data_ov337_02182444[data_ov337_02182440](a0, a1); }
// Overlay 165's (the party list): the checks pass, so the callback runs (DSProt_CallCrash, the failure, stays a trap)
void *DSProt_CallRunChecks(StubDSProtCallback callback, void *a0, void *a1) { return callback ? callback(a0, a1) : 0; }

// ---- the SDK and runtime functions Black 2's C calls that libntr declares but lacks ----

// TwlSDK's MI_CpuCopy (src, dest, size: the bytes, the fastest way the alignments allow); a field message window's
// and others' copies
// (u32 and size_t as the target has them: unsigned long on win64; libntr declares MI_CpuCopy, port_prefix.h)
void MI_CpuCopy(const void *src, void *dest, u32 size) { memmove(dest, src, size); }

// The ARM EABI's unsigned division as pokebw2's include/system/aeabi.h calls it: the quotient in the low word, the
// remainder in the high one (field_goout_effect_data.c, unity_visitor.c)
unsigned long long _u32_div_f(unsigned dividend, unsigned divisor) {
    return (unsigned long long)(dividend / divisor) | (unsigned long long)(dividend % divisor) << 32;
}
