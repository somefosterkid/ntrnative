// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_sync.h, the lines in use so far.
#ifndef BWPORT_GFL_NET_SYNC_H
#define BWPORT_GFL_NET_SYNC_H

#include "types.h"
#include "struct_decls.h"
#include "app/pokelist.h"

// Overlay 90's sync between two machines (net_sync.c, a guessed name), which works in the party list's parameters
// (pokebw2's NetSyncWork, a view of them): its state in unk62[0], its error in unk62[1] (the list ends once it is
// 1), and the count of the partner's packets of kind 0 in battleMsg. pokebw2's func_ov164_021998c0,
// func_ov164_021998c8 and func_ov164_021998d4
void func_ov090_021b6100(PokeListParam *param);
void func_ov090_021b6108(PokeListParam *param);
void func_ov090_021b6114(PokeListParam *param);

struct NetSyncPacket {
    u16 value;
    u8 kind;
    u8 pad;
};

struct NetSyncCommand {
    void (*callback)(u32 a0, u32 a1, const NetSyncPacket *packet, PokeListParam *param);
    void *arg;
};

void func_ov090_021b61e8(PokeListParam *ignored, u32 a1);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 func_ov090_021b6184(PokeListParam *param, u8 kind, u16 value);
void func_ov090_021b61c4(u32 a0, u32 a1, const NetSyncPacket *packet, PokeListParam *param);
BOOL func_ov090_021b61fc(PokeListParam *param, u32 a1);

#endif // BWPORT_GFL_NET_SYNC_H
