// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/net_ring_buff.c, ported by tools/port_file.py.
#include "types.h"
#include "gfl/net_ring_buff.h"
#include "gfl/net_system.h"
#include "gfl/std.h"

static int func_02036568(const NetRingBuff *ring, u8 *dest, int size, int max);
static int func_02036624(const NetRingBuff *ring, int pos);
static int func_02036640(NetRingBuff *ring);

void func_0203640c(NetRingBuff *ring, u8 *pDataArea, int size) {
    ring->pDataArea = pDataArea;
    ring->size = (s16)size;
    ring->startPos = 0;
    ring->endPos = 0;
}

void func_02036428(NetRingBuff *ring, const u8 *pDataArea, int size) {
    int i;
    int j;

    if (func_02036640(ring) <= size) {
        func_02039548();
        return;
    }
    j = 0;
    for (i = ring->endPos; i < ring->endPos + size; i++, j++) {
        GFL_ASSERT_AT(68, pDataArea);
        ring->pDataArea[func_02036624(ring, i)] = pDataArea[j];
    }
    ring->endPos = func_02036624(ring, i);
}

int func_020364d4(NetRingBuff *ring, u8 *dest, int size, int max) {
    int read = func_02036568(ring, dest, size, max);

    ring->startPos = func_02036624(ring, ring->startPos + read);
    return read;
}

u8 func_02036500(NetRingBuff *ring) {
    u8 byte;

    if (func_020364d4(ring, &byte, 1, 1) == 1) {
        return byte;
    }
    GFL_ASSERT_AT(108, 0);
    return 0;
}

int func_02036540(NetRingBuff *ring) {
    u16 high = func_02036500(ring) << 8;

    return (u16)(high | func_02036500(ring));
}

static int func_02036568(const NetRingBuff *ring, u8 *dest, int size, int max) {
    int i;
    int j = 0;

    for (i = ring->startPos; i < ring->startPos + size; i++) {
        if (ring->endPos == func_02036624(ring, i) || j == max) {
            return j;
        }
        dest[j] = ring->pDataArea[func_02036624(ring, i)];
        j++;
    }
    return j;
}

// The bytes written but not read
int func_020365e8(NetRingBuff *ring) {
    if (ring->startPos > ring->endPos) {
        return ring->size + ring->endPos - ring->startPos;
    }
    return ring->endPos - ring->startPos;
}

// The bytes that can be written
int func_0203660c(NetRingBuff *ring) {
    return ring->size - func_020365e8(ring);
}

static int func_02036624(const NetRingBuff *ring, int pos) {
    return pos % ring->size;
}

static int func_02036640(NetRingBuff *ring) {
    int end;

    if (ring->startPos > ring->endPos) {
        end = ring->size + ring->endPos;
    } else {
        end = ring->endPos;
    }
    end -= ring->startPos;
    return ring->size - end;
}

void func_0203666c(NetRingBuff *ring) {
}
