// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/irc/src/irc_spi.h: the lines in use so far.
#ifndef BWPORT_FIELD_IRC_SPI_H
#define BWPORT_FIELD_IRC_SPI_H

#include "types.h"

// The card's infrared chip over the backup SPI bus (see net_devirc.h)

// Every byte on the wire is xored with this
#define IRC_XOR 0xaa

// pokebw2's IrcSpi_Init
void func_ov015_0217a60c(void);
// pokebw2's IrcSpi_Release
void func_ov015_0217a648(void);
// pokebw2's IrcSpi_Recv: reads a frame into buf and gives its size, 0 if there is none
u8 func_ov015_0217a7ac(u8 *buf);
// pokebw2's IrcSpi_Send
void func_ov015_0217a8ec(u8 *data, s32 size);

#endif // BWPORT_FIELD_IRC_SPI_H
