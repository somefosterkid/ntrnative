// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/delivery_beacon.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_DELIVERY_BEACON_H
#define BWPORT_FIELD_DELIVERY_BEACON_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct {
    // The size of the data and where it goes
    int datasize;
    u8 *pData;
    // Only beacons of this region and of one of these bits are taken
    u32 region;
    u32 mask;
} DeliveryData;

#define DELIVERY_BEACON_MAX_NUM 18

// Black 2's; matching code confirms the offsets of code, heapId, data and dataNum, and its size in Black
struct DeliveryInit {
    // The high byte of the network commands
    u32 code;
    u8 flag4;
    u8 pad5;
    HeapID heapId;
    DeliveryData data[7];
    u32 dataNum;
};

// The work is passed as void * by the events that use it
void *func_ov010_021576d0(const DeliveryInit *init);
// Starts receiving, unless the network is busy
BOOL func_ov010_021578a0(void *work);
// Whether any beacon came
BOOL func_ov010_021578f0(void *work);
// Whether every beacon came, after which the data is decrypted into the buffer
BOOL func_ov010_02157910(void *work);
void func_ov010_02157928(void *work);
void func_ov010_02157938(void *work);

#endif // BWPORT_FIELD_DELIVERY_BEACON_H
