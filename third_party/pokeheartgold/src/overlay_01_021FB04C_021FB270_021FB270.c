#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_01_021FB04C.h"

typedef struct {
    int pointsCount;
    int normalsCount;
    int constantsCount;
    int platesCount;
    int stripsCount;
    int accessListCount;
} BDHCHeader;

void ov01_021FB0B0(const BDHCHeader *bdhcHeader, Bdhc *bdhc, void **buffer);

void ov01_021FB110(NARC *narc, Bdhc *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB120(NARC *narc, Bdhc *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB134(NARC *narc, Bdhc *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB144(NARC *narc, Bdhc *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB154(NARC *narc, Bdhc *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB164(NARC *narc, Bdhc *bdhc, const BDHCHeader *bdhcHeader);

void ov01_021FB04C(NARC *narc, BDHCHeader *bdhcHeader);

void ov01_021FB270(NARC *narc, const int bdhcSize, Bdhc *bdhc, u8 *buffer) {
    BDHCHeader *bdhcHeader = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(BDHCHeader));

    ov01_021FB04C(narc, bdhcHeader);
    bdhc->stripsCount = bdhcHeader->stripsCount;
    ov01_021FB0B0(bdhcHeader, bdhc, (void **)&buffer);

    ov01_021FB110(narc, bdhc, bdhcHeader);
    ov01_021FB120(narc, bdhc, bdhcHeader);
    ov01_021FB134(narc, bdhc, bdhcHeader);
    ov01_021FB144(narc, bdhc, bdhcHeader);
    ov01_021FB154(narc, bdhc, bdhcHeader);
    ov01_021FB164(narc, bdhc, bdhcHeader);

    Heap_Free(bdhcHeader);
    bdhc->loaded = TRUE;
}
