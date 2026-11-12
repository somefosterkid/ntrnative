// Host test of libntrsystem-boot.patch's multi-cell bank (NMCR) unpacking on a 64-bit target: the host copy's values
// against the file's, and that the copy is freed with the expanded heap block that holds the file (the block freed, or
// a child heap's memory given back to its parent as GFL_HeapDelete does) and replaced when a file at the same address
// is unpacked again.
//   port/tests/run_nmc_test.sh LIBNTR_SRC LIBNTR_BUILD LIBNTRSYSTEM_SRC LIBNTRSYSTEM_BUILD NMCR_FILE...
// With BWPORT_NNSYS (port/tests/res_load_test.py --nmc NMCR_FILE...): port/src/nnsys_load64.c's loader and lib/nnsys's
// expanded heap through port/extern/lib/nnsys/src/fnd/expheap.c's free hook (a file unpacked again: its block rebuilt
// in place, the same count)
#include <nitro.h>
#ifdef BWPORT_NNSYS
#include <nnsys.h>
u32 WIN_G2dNumUnpacked(void);
#define WIN_GetMultiCellBankCount() ((int)WIN_G2dNumUnpacked())
#define NNS_FndCreateExpHeap(start, size) NNS_FndCreateExpHeapEx(start, size, 0)   // the SDK's macros
#define NNS_FndAllocFromExpHeap(heap, size) NNS_FndAllocFromExpHeapEx(heap, size, 4)
#undef main   // the game's compile line (port_prefix.h) makes the game's main NitroMain
#else
#include <nnsys/fnd.h>
#include <nnsys/g2d/load/g2d_NMC_load.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures, checks;

#define CHECK(cond)                                                                                                    \
    do {                                                                                                               \
        checks++;                                                                                                      \
        if (!(cond)) {                                                                                                 \
            failures++;                                                                                                \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);                                                     \
        }                                                                                                              \
    } while (0)

static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 rd32(const u8 *p) { return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24; }
static void wr32(u8 *p, u32 v) { p[0] = (u8)v, p[1] = (u8)(v >> 8), p[2] = (u8)(v >> 16), p[3] = (u8)(v >> 24); }

// The bank starts after the 16-byte file header and the 8-byte block header; its offsets are from its start
#define BANK_OFFSET 24

// The host bank against the file it came from
static void CheckBank(const u8 *file, const NNSG2dMultiCellDataBank *bank) {
    const u8 *win = file + BANK_OFFSET;
    u16 num = rd16(win);
    const u8 *cells = win + rd32(win + 4);
    const u8 *hier = win + rd32(win + 8);
    u16 i;

    CHECK(bank != NULL);
    if (bank == NULL) {
        return;
    }
    CHECK(bank->numMultiCellData == num);
    CHECK((const u8 *)bank->pHierarchyDataArray == hier);
    for (i = 0; i < num; i++) {
        const NNSG2dMultiCellData *cell = NNS_G2dGetMultiCellDataByIdx(bank, i);

        CHECK(cell->numNodes == rd16(cells + 8 * i));
        CHECK(cell->numCellAnim == rd16(cells + 8 * i + 2));
        CHECK((const u8 *)cell->pHierDataArray == hier + rd32(cells + 8 * i + 4));
        // the first node's cell animation, read through the host pointer
        CHECK(cell->pHierDataArray[0].animSequenceIdx == rd16(hier + rd32(cells + 8 * i + 4)));
    }
    if (rd32(win + 16) == 0) {
        CHECK(bank->pExtendedData == NULL);
    }
}

static void *LoadFile(const char *path, u32 *size) {
    FILE *f = fopen(path, "rb");
    void *data;

    if (f == NULL) {
        printf("FAIL: can't open %s\n", path);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    *size = (u32)ftell(f);
    fseek(f, 0, SEEK_SET);
    data = malloc(*size);
    if (fread(data, 1, *size, f) != *size) {
        exit(1);
    }
    fclose(f);
    return data;
}

// A copy of the file with user cell attributes after it: 2 cells of 1 attribute each (0x11, 0x22)
static u8 *WithUserExData(const u8 *file, u32 size, u32 *outSize) {
    u32 ext = (size + 3) & ~3u;
    u8 *out = calloc(1, ext + 32);
    u8 *blk = out + ext;
    u8 *attrBank = blk + 8;

    memcpy(out, file, size);
    wr32(out + BANK_OFFSET + 16, ext - BANK_OFFSET); // pExtendedData
    wr32(blk, 0x55554355);                            // the block's type and size
    wr32(blk + 4, 32);
    attrBank[0] = 2;            // numCells
    attrBank[2] = 1;            // numAttribute
    wr32(attrBank + 4, 8);      // pCellAttrArray, from the attribute bank: right after it
    wr32(attrBank + 8, 16);     // cell 0's attributes
    wr32(attrBank + 12, 20);    // cell 1's
    wr32(attrBank + 16, 0x11);
    wr32(attrBank + 20, 0x22);
    *outSize = ext + 32;
    return out;
}

int main(int argc, char **argv) {
    static u8 arena[0x100000] ATTRIBUTE_ALIGN(32);
    NNSFndHeapHandle heap = NNS_FndCreateExpHeap(arena, sizeof(arena));
    void *blocks[16];
    int count = 0, i;

    if (argc < 2) {
        printf("usage: nmc_test NMCR_FILE...\n");
        return 2;
    }
    // each file in a block of its own
    for (i = 1; i < argc && count < 16; i++) {
        u32 size;
        void *data = LoadFile(argv[i], &size);
        NNSG2dMultiCellDataBank *bank = NULL;

        blocks[count] = NNS_FndAllocFromExpHeap(heap, size);
        memcpy(blocks[count], data, size);
        free(data);
        CHECK(NNS_G2dGetUnpackedMultiCellBank(blocks[count], &bank));
        CheckBank(blocks[count], bank);
        count++;
        CHECK(WIN_GetMultiCellBankCount() == count);
    }
    // again from the same file: the copy replaced, not one more
    {
        NNSG2dMultiCellDataBank *bank = NULL;

        CHECK(NNS_G2dGetUnpackedMultiCellBank(blocks[0], &bank));
        CheckBank(blocks[0], bank);
        CHECK(WIN_GetMultiCellBankCount() == count);
    }
    // freed with their blocks
    for (i = 0; i < count; i++) {
        NNS_FndFreeToExpHeap(heap, blocks[i]);
        CHECK(WIN_GetMultiCellBankCount() == count - i - 1);
    }
    // user cell attributes
    {
        u32 size, extSize;
        void *data = LoadFile(argv[1], &size);
        u8 *ext = WithUserExData(data, size, &extSize);
        void *block = NNS_FndAllocFromExpHeap(heap, extSize);
        NNSG2dMultiCellDataBank *bank = NULL;
        const NNSG2dUserExCellAttrBank *attrs;

        memcpy(block, ext, extSize);
        CHECK(NNS_G2dGetUnpackedMultiCellBank(block, &bank));
        CheckBank(block, bank);
        CHECK(bank->pExtendedData != NULL);
        attrs = (const NNSG2dUserExCellAttrBank *)((const NNSG2dUserExDataBlock *)bank->pExtendedData + 1);
        CHECK(attrs->numCells == 2 && attrs->numAttribute == 1);
        CHECK(attrs->pCellAttrArray[0].pAttr[0] == 0x11 && attrs->pCellAttrArray[1].pAttr[0] == 0x22);
        NNS_FndFreeToExpHeap(heap, block);
        CHECK(WIN_GetMultiCellBankCount() == 0);
        free(ext);
        free(data);
    }
    // in a child heap that is destroyed and whose memory goes back to its parent, as GFL_HeapDelete does
    {
        u32 size;
        void *data = LoadFile(argv[1], &size);
        void *memory = NNS_FndAllocFromExpHeap(heap, 0x10000);
        NNSFndHeapHandle child = NNS_FndCreateExpHeap(memory, 0x10000);
        void *block = NNS_FndAllocFromExpHeap(child, size);
        NNSG2dMultiCellDataBank *bank = NULL;

        memcpy(block, data, size);
        CHECK(NNS_G2dGetUnpackedMultiCellBank(block, &bank));
        CHECK(WIN_GetMultiCellBankCount() == 1);
        NNS_FndDestroyExpHeap(child);
        NNS_FndFreeToExpHeap(heap, memory);
        CHECK(WIN_GetMultiCellBankCount() == 0);
        free(data);
    }
    NNS_FndDestroyExpHeap(heap);
    printf("nmc_test: %d checks, %d failed (%d files)\n", checks, failures, count);
    return failures != 0;
}
