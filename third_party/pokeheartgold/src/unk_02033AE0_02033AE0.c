#include "global.h"

#include "comm_ring.h"

void sub_02033AE0(CommRing *ring, void *arg1, s16 arg2);

void sub_02037974(void);
int sub_02033C14(CommRing *ring, int index);
int sub_02033BF4(CommRing *ring);
void sub_02033AF0(CommRing *ring, u8 *buffer, int size, int unused);

void sub_02033AE0(CommRing *ring, void *arg1, s16 arg2) {
    ring->buffer = arg1;
    ring->size = arg2;
    ring->startIndex = 0;
    ring->endIndex = 0;
    ring->backupEndIndex = 0;
}

void sub_02033AF0(CommRing *ring, u8 *buffer, int size, int unused) {
    int i;

    if (sub_02033BF4(ring) <= size) {
        sub_02037974();
        return;
    }

    int j = 0;

    for (i = ring->backupEndIndex; i < ring->backupEndIndex + size; i++, j++) {
        ((buffer) ? (void)0 : GF_AssertFail());
        ring->buffer[sub_02033C14(ring, i)] = buffer[j];
    }

    ring->backupEndIndex = sub_02033C14(ring, i);
}
