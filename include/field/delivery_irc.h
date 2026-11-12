// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/delivery_irc.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_DELIVERY_IRC_H
#define BWPORT_FIELD_DELIVERY_IRC_H

#include "types.h"
#include "field/delivery_beacon.h"
#include "gfl/heap.h"

// Black 2's; twinned code confirms the offsets of code, heapId, data and dataNum in Black
// The init of delivery_beacon.c, but with a halfword at offset 4
typedef struct {
    u32 code;
    u16 unk04;
    HeapID heapId;
    DeliveryData data[7];
    u32 dataNum;
} DeliveryIrcInit;

// The work is passed as void * by the screens that use it
void *func_ov010_02157dd8(const DeliveryIrcInit *init);
void func_ov010_02157e78(void *work);
// Starts the network, unless it is busy
BOOL func_ov010_02157e34(void *work);
// 1 once the data came or went, 2 if it came damaged, 3 if there was none to give
u8 func_ov010_02157e6c(void *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov010_02157e8c(void *work);

#endif // BWPORT_FIELD_DELIVERY_IRC_H
