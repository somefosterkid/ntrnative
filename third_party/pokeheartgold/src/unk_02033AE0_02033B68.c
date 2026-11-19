#include "global.h"
#include "comm_ring.h"

int sub_02033B4C(CommRing *ring, u8 *buffer, int size);

u8 sub_02033B68(void *arg0);

int sub_02033C14(CommRing *ring, int index);
int sub_02033B78(CommRing *ring, u8 *buffer, int size);
int sub_02033BC4(CommRing *ring);

u8 sub_02033B68(void *arg0) {
    u8 v0;

    sub_02033B4C(arg0, &v0, 1);
    return v0;
}

int sub_02033B78(CommRing *ring, u8 *buffer, int size) {
    int j = 0;

    for (int i = ring->startIndex; i < ring->startIndex + size; i++, j++) {
        if (ring->endIndex == sub_02033C14(ring, i)) {
            return j;
        }

        buffer[j] = ring->buffer[sub_02033C14(ring, i)];
    }

    return j;
}

int sub_02033BC4(CommRing *ring) {
    if (ring->startIndex > ring->endIndex) {
        return ring->size + ring->endIndex - ring->startIndex;
    }

    return ring->endIndex - ring->startIndex;
}
