// The PC build's infrared: the card's IR chip, which the game drives itself through the card's SPI registers
// (0x040001a0 MCCNT0, 0x040001a2 MCD0), is not there. Two entry points:
//   - src/ov015/irc_spi.c (include/field/irc_spi.h: the C-Gear's infrared link, under irc_link.c, net_irc.c and
//     net_devirc.c): the chip is there but nothing answers, so a receive gets no bytes and a send goes nowhere;
//     irc_link.c's waits time out into the game's own "the connection failed" path. port/exclude.txt leaves
//     irc_spi.c out (libntr's registers are plain variables, so the compiled file would read back what it wrote and
//     get no bytes as well, taking the card's backup lock for each transfer).
//   - src/ov231/ir_check.c's func_ov231_021809d8: the game's own anti-piracy check beside DS Protect's, that the
//     cartridge is genuine (a genuine card has the IR chip and answers its ID command with 0xaa). game_beacon.c
//     turns the C-Gear's beacons off and btl_main.c gives up a battle reward when it says FALSE, so the PC answers
//     as a genuine cartridge. port/exclude.txt has to leave ir_check.c out (compiled, it reads 0 from the register
//     variable and says FALSE): a link with both fails loudly rather than run as a pirated copy.
#include "types.h"
#include "field/irc_spi.h"
#include "system/ir_check.h"

// ---- src/ov015/irc_spi.c ----

// Takes the card's lock ID for the transfers
void func_ov015_0217a60c(void) {
}

// Releases it
void func_ov015_0217a648(void) {
}

// Receives what the chip has: its first byte is the count, here none
u8 func_ov015_0217a7ac(u8 *buf) {
    buf[0] = 0;
    return 0;
}

// Sends size bytes: no one is there
void func_ov015_0217a8ec(u8 *data, s32 size) {
    (void)data, (void)size;
}

// ---- src/ov231/ir_check.c ----

// pokebw2's IrCheck_IsGenuineCard
BOOL func_ov231_021809d8(void) {
    return TRUE;
}
