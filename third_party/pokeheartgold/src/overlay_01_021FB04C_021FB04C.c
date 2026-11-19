#include "global.h"

#include "filesystem.h"

typedef struct {
    int pointsCount;
    int normalsCount;
    int constantsCount;
    int platesCount;
    int stripsCount;
    int accessListCount;
} BDHCHeader;
typedef struct BDHCPlate {
    u16 firstPointIndex;
    u16 secondPointIndex;
    u16 normalIndex;
    u16 constantIndex;
} BDHCPlate;
typedef struct BDHCPoint {
    fx32 x;
    fx32 z;
} BDHCPoint;
typedef struct BDHCStrip {
    fx32 scanline;
    u16 accessListElementCount;
    u16 accessListStartIndex;
} BDHCStrip;
typedef struct BDHC {
    BDHCPlate *plates;
    fx32 *constants;
    BDHCStrip *strips;
    u16 *accessList;
    BDHCPoint *points;
    VecFx32 *normals;
    BOOL loaded;
    int stripsCount;
} BDHC;

void ov01_021FB04C(NARC *narc, BDHCHeader *bdhcHeader);
void ov01_021FB0B0(const BDHCHeader *bdhcHeader, BDHC *bdhc, void **buffer);

void ov01_021FB04C(NARC *narc, BDHCHeader *bdhcHeader) {
    u16 magic[4];

    MI_CpuClear32(bdhcHeader, sizeof(BDHCHeader));

    NARC_ReadFile(narc, 4, magic);
    NARC_ReadFile(narc, 2, &bdhcHeader->pointsCount);
    NARC_ReadFile(narc, 2, &bdhcHeader->normalsCount);
    NARC_ReadFile(narc, 2, &bdhcHeader->constantsCount);
    NARC_ReadFile(narc, 2, &bdhcHeader->platesCount);
    NARC_ReadFile(narc, 2, &bdhcHeader->stripsCount);
    NARC_ReadFile(narc, 2, &bdhcHeader->accessListCount);
}

void ov01_021FB0B0(const BDHCHeader *bdhcHeader, BDHC *bdhc, void **buffer) {
    int offset = 0;

    void *ptr = (u8 *)*buffer;
    bdhc->points = ptr;
    offset += (sizeof(BDHCPoint) * bdhcHeader->pointsCount);

    ptr = (u8 *)*buffer + offset;
    bdhc->normals = ptr;
    offset += (sizeof(VecFx32) * bdhcHeader->normalsCount);

    ptr = (u8 *)*buffer + offset;
    bdhc->constants = ptr;
    offset += (sizeof(fx32) * bdhcHeader->constantsCount);

    ptr = (u8 *)*buffer + offset;
    bdhc->plates = ptr;
    offset += (sizeof(BDHCPlate) * bdhcHeader->platesCount);

    ptr = (u8 *)*buffer + offset;
    bdhc->strips = ptr;
    offset += sizeof(BDHCStrip) * bdhcHeader->stripsCount;

    ptr = (u8 *)*buffer + offset;
    bdhc->accessList = ptr;
    offset += sizeof(u16) * bdhcHeader->accessListCount;

    ((offset <= 0x9000) ? (void)0 : GF_AssertFail());
}
