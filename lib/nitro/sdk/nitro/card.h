// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/card.h, as it is (tools/port_lib.py)
#ifndef NITRO_CARD_H_
#define NITRO_CARD_H_

#include <nitro/misc.h>
#include <nitro/types.h>

#include <nitro/card/common.h>
#include <nitro/card/eeprom.h>
#include <nitro/card/flash.h>
#include <nitro/card/fram.h>
#include <nitro/card/rom.h>
#if SDK_VERSION_MAJOR == 5
#include <nitro/card/hash.h>
#endif
#include <nitro/card/pullOut.h>

#endif
