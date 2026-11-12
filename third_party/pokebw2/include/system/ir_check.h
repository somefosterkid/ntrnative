#ifndef POKEBW2_SYSTEM_IR_CHECK_H
#define POKEBW2_SYSTEM_IR_CHECK_H

#include "types.h"

// Overlay 338, named descriptively (ir_check.c; no string names it): asks the card's infrared chip for its ID over
// the backup SPI bus. A genuine cartridge answers 0xaa; a copy has no chip. The battle (btl_main.c) and the beacons
// (game_beacon.c) load it to check that the game runs from a genuine card.
BOOL IrCheck_IsGenuineCard(void);

#endif // POKEBW2_SYSTEM_IR_CHECK_H
