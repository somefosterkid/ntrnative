// port/include_plat/port_plat.h: the PC build's prefix header for Pokemon Platinum (pret pokeplatinum's C,
// tools/pokeplat_fetch.sh, tools/pokeplat_cc.py: -include port_plat.h before every file's own lines). Platinum's
// SDK spellings that libntr (TwlSDK 5) and lib/nnsys (Black's NitroSystem) spell otherwise or lack go here, as
// port/include_b2w2/port_b2w2.h does for pokebw2. docs/platinum.md has the failure table this answers.
#ifndef BWPORT_PORT_PLAT_H
#define BWPORT_PORT_PLAT_H

#include <nitro.h>
#include <nnsys.h>
#include "nnsys_gen4.h"

// Platinum's online headers (pret's NitroDWC, NitroWiFi: the online side is stubbed on the PC, as Black's): DWC's
// headers use NitroWiFi's socket types, which nitroWiFi.h declares only under SDK_ARM9 (libntr's PC builds don't
// define it)
#include <nitroWiFi/socket.h>
#include <nitroWiFi/wcm.h>
#include <nitroWiFi/ssl.h>
#include <nitroWiFi/so2soc.h>

// pret's prefix header (meson.build c_pch: include/pch/global_pch.h): GF_ASSERT, ALIGN_4, NELEMS, the game config
#include "pch/global_pch.h"
// its two pointer-as-u32 macros, at the host's pointer size
#undef ALIGN_PTR
#define ALIGN_PTR(ptr, alignment) ((void *)(((uintptr_t)(ptr) + (alignment - 1)) / (alignment) * (alignment)))
#undef XtOffset
#define XtOffset(pointer_type, field) ((unsigned int)(uintptr_t)&(((pointer_type)NULL)->field))

#endif // BWPORT_PORT_PLAT_H
