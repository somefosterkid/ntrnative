// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_ring_buff.h, the lines in use so
// far.
#ifndef BWPORT_GFL_NET_RING_BUFF_H
#define BWPORT_GFL_NET_RING_BUFF_H

#include "types.h"

// A ring buffer of bytes for the network library (net_ring_buff.c, a name the ROM does not embed; pokeplatinum's
// comm_ring_buffer.c is the same code)
typedef struct {
    u8 *pDataArea;
    int size;
    // Set by net_system.c when data from a machine arrives
    BOOL received;
    // Where reading and writing continue
    s16 startPos;
    volatile s16 endPos;
} NetRingBuff;

void func_0203640c(NetRingBuff *ring, u8 *pDataArea, int size);
// Writes size bytes, or sets the network error if they do not fit
void func_02036428(NetRingBuff *ring, const u8 *pDataArea, int size);
// Reads up to size bytes, and at most max, returning how many it read
int func_020364d4(NetRingBuff *ring, u8 *dest, int size, int max);
u8 func_02036500(NetRingBuff *ring);
// A big-endian u16
int func_02036540(NetRingBuff *ring);
int func_020365e8(NetRingBuff *ring);
int func_0203660c(NetRingBuff *ring);
// Makes what has been written readable, which needs nothing here
void func_0203666c(NetRingBuff *ring);

#endif // BWPORT_GFL_NET_RING_BUFF_H
