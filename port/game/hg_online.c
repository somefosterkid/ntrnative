// HeartGold's online play on the PC: none, as Platinum's. HGSS calls the same NitroDWC, NitroWiFi (SOC_*, WCM_GetPhase),
// libvct and GameSpy (B64Encode) names as Platinum, with the same arguments, so Platinum's stubs serve it: plat_online.c
// without the Wi-Fi Plaza's lobby (PLATINUM only). Compiled into hg.o (its own static library in port/meson.build, with
// port/include_plat on the include path for dwc.h, vct.h and nitroWiFi/soc.h): hg.o keeps no global but hg_bwgame
// (tools/port_relink.py), so these stay HeartGold's own and do not meet Platinum's or Black 2's of the same names.
// The names HGSS calls that Black's SDK stubs already have (port/src/stub_online.c) bind to those, as Platinum's do.
#include "plat_online.c"

// The cartridge protection (lib/include/dsprot.h, the ds_protect overlay: closed code, no C in the decomp). The PC is a
// genuine cartridge that is neither a flash cart, a dummy nor an emulator: each check answers that, and calls its callback
// only when its condition holds (the Not* ones: dsprot.h), as on a real cartridge. FieldSystem_Init's key sums then stay
// coprime with 2441 and 4073, so ov124_02260D1C does not clobber the heap. DSProt_CallCrash is left out on purpose: a
// call aborts in undefined_stubs.c, loudly.
static u32 HgDsProt(void *callback, u32 holds) {
    if (holds && callback != NULL) {
        ((void (*)(void))callback)();
    }
    return holds;
}
u32 DSProt_DetectFlashcart(void *callback) { return HgDsProt(callback, 0); }
u32 DSProt_DetectNotFlashcart(void *callback) { return HgDsProt(callback, 1); }
u32 DSProt_DetectEmulator(void *callback) { return HgDsProt(callback, 0); }
u32 DSProt_DetectNotEmulator(void *callback) { return HgDsProt(callback, 1); }
u32 DSProt_DetectDummy(void *callback) { return HgDsProt(callback, 0); }
u32 DSProt_DetectNotDummy(void *callback) { return HgDsProt(callback, 1); }

// The cartridge's infrared chip (lib/asm/libirc.s: pret keeps it in assembly; HGSS-only, not NitroSDK, so it is not in
// libntr and Black never calls it). On the DS these drive the IR transceiver on the backup SPI bus (CARD_LockBackup +
// SPI byte transfers): the Pokewalker overlay (ov112) talks through it, and FieldMap's init (fieldmap.c ov01_021E662C)
// asks the chip's status twice and starts the cartridge-protection tasks (Task_AntipiracyMath) unless it answers 0xAA,
// the value a genuine HGSS cartridge's IR chip gives. The PC is a genuine cartridge with nothing in infrared range: the
// status is 0xAA, a read receives nothing (0 bytes, as the DS returns with no peer), a write goes nowhere. The wait
// cycles only pace the DS's SPI transfers (no state the game reads back).
void CARD_SetSpiReadWaitCycles(u8 cycles) { (void)cycles; }
void CARD_SetSpiWriteWaitCycles(u8 cycles) { (void)cycles; }
void CARD_SpiWaitInit(void) { }
s32 CARD_SpiWaitGetStatus(void) { return 0xAA; }
u32 CARD_SpiWaitReadRange(u8 *buf) { (void)buf; return 0; }
void CARD_SpiWaitWriteRange(u8 *buf, u8 size) { (void)buf; (void)size; }
