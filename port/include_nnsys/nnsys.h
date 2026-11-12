// The PC build's NitroSystem umbrella header once the port compiles our own NitroSystem (docs/nnsys-port.md; not wired
// into port/meson.build yet: today port/include/port_prefix.h's <nnsys.h> is libntrsystem's): the NitroSDK names our
// headers use that libntr lacks (nnsys_sdk.h), then the five headers of lib/nnsys/include/nnsys/. The library's own
// files also get nnsys_lib.h (-include).
#ifndef BWPORT_NNSYS_H
#define BWPORT_NNSYS_H

#include "nnsys_sdk.h"
#include "nnsys/fnd.h"
#include "nnsys/gfd.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "nnsys/snd.h"

#endif
