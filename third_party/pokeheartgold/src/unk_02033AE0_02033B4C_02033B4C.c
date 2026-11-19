#include "global.h"
#include "comm_ring.h"

int sub_02033B78(CommRing *ring, u8 *buffer, int size);

int sub_02033C14(CommRing *ring, int index);

int sub_02033B4C(CommRing *ring, u8 *buffer, int size);

int sub_02033B4C(CommRing *ring, u8 *buffer, int size) {
    int n = sub_02033B78(ring, buffer, size);
    ring->startIndex = sub_02033C14(ring, ring->startIndex + n);
    return n;
}
