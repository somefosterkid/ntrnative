// NitroSystem's G2D file loaders on a 64-bit host (docs/nnsys-port.md): the entry points of lib/nnsys/src/g2d/load/*.c,
// which the PC build leaves out. On the DS a loader turns a file's 4-byte offsets into pointers in place; here the
// structs that hold pointers are 8-byte-pointer host structs, so each call builds host copies of them in ONE malloc'd
// block that points into the file for the raw data (characters, colours, OAM attributes, animation frames' contents,
// hierarchy data, user attribute values, glyphs). The file itself is not changed. Each struct is described by a table
// (L64Desc: its size in the file and on the host, and its fields: copied, or an offset turned into a pointer relative to
// the struct or to a base its parent gives); a format's builder walks its file with those tables, once to size the
// block and once to fill it.
// A block is registered with the file it came from and freed when the heap block holding the file is
// (WIN_G2dFreeUnpacked, from port/extern/lib/nnsys/src/fnd/expheap.c), which is when the data dies on the DS: 64 frees
// late, so that a use right after the free (harmless on the DS) stays harmless. The same file unpacked again (the
// same kind of block, a file read again into the same buffer) gets its block rebuilt where it was when it fits, so a
// pointer kept from the first unpack sees the new data, as the DS's in-place unpack gives. WIN_G2dResetUnpacked frees
// them all (the in-process restart, port/src/machine.c).
// The code is under BWPORT_NNSYS (every port build since 2026-11-07; port/tests' libntrsystem-era tools left it out).
// What libntrsystem's PC loaders did (the NitroSystem the port used until 2026-11-07) and this keeps (its boot, load64 and ios-nftbounds patches): a cell bank's cells,
// VRAM transfer data and user cell attributes, an animation bank's sequences with their own frames each and its user
// animation attributes, a multi-cell bank's multi-cells and user cell attributes, palettes, compressed palette info,
// character data and a font's information, width and code map chains as host structs; pStringBank NULL;
// pExtendedData and pVramTransferData NULL when the file has none. The font's width tables and code map info, which
// lib/nnsys's headers hold inline after the host struct, are copied (the whole of their block, no fixed tables).
#ifdef BWPORT_NNSYS
#include <nitro.h>
#include <nnsys.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void WIN_G2dFreeUnpacked(void *start, void *end);
void WIN_G2dResetUnpacked(void);

// ---- the tables

enum { OP_END, OP_COPY, OP_PTR, OP_PTRZ };   // copy SIZE bytes; offset -> pointer; the same, NULL when the offset is 0
enum { B_SELF, B_ARG };                       // an offset is from the struct itself, or from the base its parent gives

typedef struct {
    u8 op, base, fofs, size;
    u16 hofs;
} L64Field;

typedef struct {
    u16 fsize, hsize;   // the struct's size (array stride) in the file and on the host
    L64Field f[8];
} L64Desc;

#define COPY(fo, T, m) {OP_COPY, 0, fo, sizeof(((T *)0)->m), offsetof(T, m)}
#define PTR(fo, T, m, b) {OP_PTR, b, fo, 0, offsetof(T, m)}
#define PTRZ(fo, T, m, b) {OP_PTRZ, b, fo, 0, offsetof(T, m)}

static const L64Desc dChar = {28, sizeof(NNSG2dCharacterData), {
    COPY(0, NNSG2dCharacterData, H), COPY(2, NNSG2dCharacterData, W), COPY(4, NNSG2dCharacterData, pixelFmt),
    COPY(8, NNSG2dCharacterData, mappingType), COPY(12, NNSG2dCharacterData, characterFmt),
    COPY(16, NNSG2dCharacterData, szByte), PTR(20, NNSG2dCharacterData, pRawData, B_SELF)}};
static const L64Desc dPltt = {16, sizeof(NNSG2dPaletteData), {
    COPY(0, NNSG2dPaletteData, fmt), COPY(4, NNSG2dPaletteData, bExtendedPlt), COPY(8, NNSG2dPaletteData, szByte),
    PTR(12, NNSG2dPaletteData, pRawData, B_SELF)}};
static const L64Desc dPlttCmp = {8, sizeof(NNSG2dPaletteCompressInfo), {
    COPY(0, NNSG2dPaletteCompressInfo, numPalette), COPY(2, NNSG2dPaletteCompressInfo, pad16),
    PTR(4, NNSG2dPaletteCompressInfo, pPlttIdxTbl, B_SELF)}};
// cell banks (NCER): the pointers to host arrays are the builder's
static const L64Desc dCellBank = {24, sizeof(NNSG2dCellDataBank), {
    COPY(0, NNSG2dCellDataBank, numCells), COPY(2, NNSG2dCellDataBank, cellBankAttr),
    COPY(8, NNSG2dCellDataBank, mappingMode)}};
static const L64Desc dCell = {8, sizeof(NNSG2dCellData), {          // ARG: the OAM attributes after the last cell
    COPY(0, NNSG2dCellData, numOAMAttrs), COPY(2, NNSG2dCellData, cellAttr),
    PTR(4, NNSG2dCellData, pOamAttrArray, B_ARG)}};
static const L64Desc dCellBR = {16, sizeof(NNSG2dCellDataWithBR), {
    COPY(0, NNSG2dCellDataWithBR, cellData.numOAMAttrs), COPY(2, NNSG2dCellDataWithBR, cellData.cellAttr),
    PTR(4, NNSG2dCellDataWithBR, cellData.pOamAttrArray, B_ARG), COPY(8, NNSG2dCellDataWithBR, boundingRect)}};
static const L64Desc dVram = {8, sizeof(NNSG2dVramTransferData), {
    COPY(0, NNSG2dVramTransferData, szByteMax), PTR(4, NNSG2dVramTransferData, pCellTransferDataArray, B_SELF)}};
// user extended attributes: the block header, then the bank (NNSi_G2dGetUserExDataBlkByID finds it right after)
static const L64Desc dExBlk = {8, sizeof(NNSG2dUserExDataBlock), {
    COPY(0, NNSG2dUserExDataBlock, blkTypeID), COPY(4, NNSG2dUserExDataBlock, blkSize)}};
static const L64Desc dCellAttrBank = {8, sizeof(NNSG2dUserExCellAttrBank), {
    COPY(0, NNSG2dUserExCellAttrBank, numCells), COPY(2, NNSG2dUserExCellAttrBank, numAttribute)}};
static const L64Desc dCellAttr = {4, sizeof(NNSG2dUserExCellAttr), {     // ARG: the attribute bank
    PTR(0, NNSG2dUserExCellAttr, pAttr, B_ARG)}};
// animation banks (NANR, NMAR)
static const L64Desc dAnimBank = {24, sizeof(NNSG2dAnimBankData), {
    COPY(0, NNSG2dAnimBankData, numSequences), COPY(2, NNSG2dAnimBankData, numTotalFrames),
    PTR(12, NNSG2dAnimBankData, pAnimContents, B_SELF)}};
static const L64Desc dAnimSeq = {16, sizeof(NNSG2dAnimSequenceData), {
    COPY(0, NNSG2dAnimSequenceData, numFrames), COPY(2, NNSG2dAnimSequenceData, loopStartFrameIdx),
    COPY(4, NNSG2dAnimSequenceData, animType), COPY(8, NNSG2dAnimSequenceData, playMode)}};
static const L64Desc dAnimFrame = {8, sizeof(NNSG2dAnimFrameData), {    // ARG: the bank's anim contents
    PTR(0, NNSG2dAnimFrameData, pContent, B_ARG), COPY(4, NNSG2dAnimFrameData, frames),
    COPY(6, NNSG2dAnimFrameData, pad16)}};
static const L64Desc dAnimAttrBank = {8, sizeof(NNSG2dUserExAnimAttrBank), {
    COPY(0, NNSG2dUserExAnimAttrBank, numSequences), COPY(2, NNSG2dUserExAnimAttrBank, numAttribute)}};
static const L64Desc dAnimSeqAttr = {12, sizeof(NNSG2dUserExAnimSequenceAttr), {   // ARG: the attribute bank
    COPY(0, NNSG2dUserExAnimSequenceAttr, numFrames), COPY(2, NNSG2dUserExAnimSequenceAttr, pad16),
    PTR(4, NNSG2dUserExAnimSequenceAttr, pAttr, B_ARG)}};
static const L64Desc dAnimFrmAttr = {4, sizeof(NNSG2dUserExAnimFrameAttr), {       // ARG: the attribute bank
    PTR(0, NNSG2dUserExAnimFrameAttr, pAttr, B_ARG)}};
// multi-cell banks (NMCR)
static const L64Desc dMCBank = {20, sizeof(NNSG2dMultiCellDataBank), {
    COPY(0, NNSG2dMultiCellDataBank, numMultiCellData), COPY(2, NNSG2dMultiCellDataBank, pad16),
    PTR(8, NNSG2dMultiCellDataBank, pHierarchyDataArray, B_SELF)}};
static const L64Desc dMC = {8, sizeof(NNSG2dMultiCellData), {           // ARG: the bank's hierarchy data
    COPY(0, NNSG2dMultiCellData, numNodes), COPY(2, NNSG2dMultiCellData, numCellAnim),
    PTR(4, NNSG2dMultiCellData, pHierDataArray, B_ARG)}};
// fonts (NFTR): offsets from the file's start
static const L64Desc dFontInfo = {20, sizeof(NNSG2dFontInformation), {  // ARG: the file
    COPY(0, NNSG2dFontInformation, fontType), COPY(1, NNSG2dFontInformation, linefeed),
    COPY(2, NNSG2dFontInformation, alterCharIndex), COPY(4, NNSG2dFontInformation, defaultWidth),
    COPY(7, NNSG2dFontInformation, encoding), PTR(8, NNSG2dFontInformation, pGlyph, B_ARG)}};
static const L64Desc dFontWidth = {8, offsetof(NNSG2dFontWidth, widthTable), {
    COPY(0, NNSG2dFontWidth, indexBegin), COPY(2, NNSG2dFontWidth, indexEnd)}};
static const L64Desc dFontMap = {12, offsetof(NNSG2dFontCodeMap, mapInfo), {
    COPY(0, NNSG2dFontCodeMap, ccodeBegin), COPY(2, NNSG2dFontCodeMap, ccodeEnd),
    COPY(4, NNSG2dFontCodeMap, mappingMethod), COPY(6, NNSG2dFontCodeMap, reserved)}};

// ---- the walker

static u16 rd16(const u8 *p) {
    u16 v;
    memcpy(&v, p, 2);
    return v;
}

static u32 rd32(const u8 *p) {
    u32 v;
    memcpy(&v, p, 4);
    return v;
}

// a host struct from the file's SRC; DST NULL (the sizing pass) does nothing
static void Conv(const L64Desc *d, const u8 *src, void *dst, const u8 *arg) {
    const L64Field *f;

    if (dst == NULL) {
        return;
    }
    memset(dst, 0, d->hsize);
    for (f = d->f; f < d->f + 8 && f->op != OP_END; f++) {
        u8 *h = (u8 *)dst + f->hofs;
        u32 ofs;
        void *p;

        if (f->op == OP_COPY) {
            memcpy(h, src + f->fofs, f->size);
            continue;
        }
        ofs = rd32(src + f->fofs);
        p = f->op == OP_PTRZ && ofs == 0 ? NULL : (void *)((f->base == B_SELF ? src : arg) + ofs);
        memcpy(h, &p, sizeof p);
    }
}

// N of them, one after the other in the file and on the host
static void ConvN(const L64Desc *d, const u8 *src, u32 n, void *dst, const u8 *arg) {
    u32 i;

    for (i = 0; dst != NULL && i < n; i++) {
        Conv(d, src + i * d->fsize, (u8 *)dst + i * d->hsize, arg);
    }
}

// the block being built: BASE NULL while sizing
typedef struct {
    u8 *base;
    size_t used;
} L64Blk;

static void *Take(L64Blk *b, size_t size) {
    void *p = b->base != NULL ? b->base + b->used : NULL;

    b->used += (size + 7) & ~(size_t)7;
    return p;
}

#define SET(s, m, v)                                                                                                   \
    do {                                                                                                               \
        if ((s) != NULL) {                                                                                             \
            (s)->m = (v);                                                                                              \
        }                                                                                                              \
    } while (0)

// user extended cell attributes (cell and multi-cell banks): the block header, the bank, its attributes
static void *BuildCellAttrs(L64Blk *b, const u8 *blk) {
    const u8 *bank = blk + 8;
    u16 n = rd16(bank);
    NNSG2dUserExDataBlock *hb = Take(b, sizeof(NNSG2dUserExDataBlock) + sizeof(NNSG2dUserExCellAttrBank));
    NNSG2dUserExCellAttrBank *hbank = hb != NULL ? (NNSG2dUserExCellAttrBank *)(hb + 1) : NULL;
    NNSG2dUserExCellAttr *attrs = Take(b, sizeof(NNSG2dUserExCellAttr) * n);

    Conv(&dExBlk, blk, hb, NULL);
    Conv(&dCellAttrBank, bank, hbank, NULL);
    SET(hbank, pCellAttrArray, attrs);
    ConvN(&dCellAttr, bank + rd32(bank + 4), n, attrs, bank);
    return hb;
}

static void *BuildCellBank(L64Blk *b, const u8 *f) {
    NNSG2dCellDataBank *bank = Take(b, sizeof *bank);
    u16 n = rd16(f);
    const L64Desc *dc = rd16(f + 2) & NNS_G2D_CELLBK_ATTR_CELLWITHBR ? &dCellBR : &dCell;
    const u8 *cells = f + rd32(f + 4);
    void *hcells = Take(b, (size_t)dc->hsize * n);

    Conv(&dCellBank, f, bank, NULL);
    SET(bank, pCellDataArrayHead, hcells);
    ConvN(dc, cells, n, hcells, cells + (u32)dc->fsize * n);
    if (rd32(f + 12) != 0) {
        NNSG2dVramTransferData *v = Take(b, sizeof *v);

        Conv(&dVram, f + rd32(f + 12), v, NULL);
        SET(bank, pVramTransferData, v);
    }
    if (rd32(f + 20) != 0) {
        void *ex = BuildCellAttrs(b, f + rd32(f + 20));

        SET(bank, pExtendedData, ex);
    }
    return bank;
}

static void *BuildAnimBank(L64Blk *b, const u8 *f) {
    NNSG2dAnimBankData *bank = Take(b, sizeof *bank);
    u16 n = rd16(f), i;
    const u8 *seqs = f + rd32(f + 4), *frames = f + rd32(f + 8), *contents = f + rd32(f + 12);
    NNSG2dAnimSequenceData *hseqs = Take(b, sizeof(NNSG2dAnimSequenceData) * n);
    NNSG2dAnimFrameData *hframes = NULL;

    Conv(&dAnimBank, f, bank, NULL);
    SET(bank, pSequenceArrayHead, hseqs);
    // each sequence gets its own frames (a file's sequences may share theirs), one after the other
    for (i = 0; i < n; i++) {
        const u8 *s = seqs + i * dAnimSeq.fsize;
        u16 nf = rd16(s);
        NNSG2dAnimFrameData *hf = Take(b, sizeof(NNSG2dAnimFrameData) * nf);

        if (i == 0) {
            hframes = hf;
        }
        Conv(&dAnimSeq, s, hseqs != NULL ? &hseqs[i] : NULL, NULL);
        SET(hseqs != NULL ? &hseqs[i] : NULL, pAnmFrameArray, hf);
        ConvN(&dAnimFrame, frames + rd32(s + 12), nf, hf, contents);
    }
    SET(bank, pFrameArrayHead, n != 0 ? hframes : (NNSG2dAnimFrameData *)(b->base != NULL ? b->base + b->used : NULL));
    if (rd32(f + 20) != 0) {
        const u8 *blk = f + rd32(f + 20), *ab = blk + 8, *sattr = ab + rd32(ab + 4);
        u16 ns = rd16(ab);
        NNSG2dUserExDataBlock *hb = Take(b, sizeof(NNSG2dUserExDataBlock) + sizeof(NNSG2dUserExAnimAttrBank));
        NNSG2dUserExAnimAttrBank *hab = hb != NULL ? (NNSG2dUserExAnimAttrBank *)(hb + 1) : NULL;
        NNSG2dUserExAnimSequenceAttr *hsa = Take(b, sizeof(NNSG2dUserExAnimSequenceAttr) * ns);

        Conv(&dExBlk, blk, hb, NULL);
        Conv(&dAnimAttrBank, ab, hab, NULL);
        SET(hab, pAnmSeqAttrArray, hsa);
        for (i = 0; i < ns; i++) {
            const u8 *s = sattr + i * dAnimSeqAttr.fsize;
            u16 nf = rd16(s);
            NNSG2dUserExAnimFrameAttr *hfa = Take(b, sizeof(NNSG2dUserExAnimFrameAttr) * nf);

            Conv(&dAnimSeqAttr, s, hsa != NULL ? &hsa[i] : NULL, ab);
            SET(hsa != NULL ? &hsa[i] : NULL, pAnmFrmAttrArray, hfa);
            ConvN(&dAnimFrmAttr, ab + rd32(s + 8), nf, hfa, ab);
        }
        SET(bank, pExtendedData, hb);
    }
    return bank;
}

static void *BuildMCBank(L64Blk *b, const u8 *f) {
    NNSG2dMultiCellDataBank *bank = Take(b, sizeof *bank);
    u16 n = rd16(f);
    NNSG2dMultiCellData *hmc = Take(b, sizeof(NNSG2dMultiCellData) * n);

    Conv(&dMCBank, f, bank, NULL);
    SET(bank, pMultiCellDataArray, hmc);
    ConvN(&dMC, f + rd32(f + 4), n, hmc, f + rd32(f + 8));
    if (rd32(f + 16) != 0) {
        void *ex = BuildCellAttrs(b, f + rd32(f + 16));

        SET(bank, pExtendedData, ex);
    }
    return bank;
}

// the rest of a font block after its struct's file part (the width table, the code map's info), copied after the
// host struct, where lib/nnsys's headers have it
static u32 FontTail(const u8 *data, u32 fsize) {
    u32 size = rd32(data - 4);

    return size >= 8 + fsize ? size - 8 - fsize : 0;
}

static void *BuildFont(L64Blk *b, const u8 *file) {
    const u8 *info = (const u8 *)NNS_G2dFindBinaryBlock((NNSG2dBinaryFileHeader *)file, NNS_G2D_BINBLK_SIG_FINFDATA);
    NNSG2dFontInformation *hi;
    u32 o;

    if (info == NULL) {
        return NULL;
    }
    info += sizeof(NNSG2dBinaryBlockHeader);
    hi = Take(b, sizeof *hi);
    Conv(&dFontInfo, info, hi, file);
    if (rd32(info + 12) != 0) {
        NNSG2dFontWidth *prev = NULL;

        for (o = rd32(info + 12); o != 0; o = rd32(file + o + 4)) {
            u32 tail = FontTail(file + o, dFontWidth.fsize);
            NNSG2dFontWidth *w = Take(b, dFontWidth.hsize + tail);

            Conv(&dFontWidth, file + o, w, NULL);
            if (w != NULL) {
                memcpy(w->widthTable, file + o + dFontWidth.fsize, tail);
            }
            if (prev != NULL) {
                prev->pNext = w;
            } else {
                SET(hi, pWidth, w);
            }
            prev = w;
        }
    }
    if (rd32(info + 16) != 0) {
        NNSG2dFontCodeMap *prev = NULL;

        for (o = rd32(info + 16); o != 0; o = rd32(file + o + 8)) {
            u32 tail = FontTail(file + o, dFontMap.fsize);
            NNSG2dFontCodeMap *m = Take(b, dFontMap.hsize + tail);

            Conv(&dFontMap, file + o, m, NULL);
            if (m != NULL) {
                memcpy(m->mapInfo, file + o + dFontMap.fsize, tail);
            }
            if (prev != NULL) {
                prev->pNext = m;
            } else {
                SET(hi, pMap, m);
            }
            prev = m;
        }
    }
    return hi;
}

static void *BuildChar(L64Blk *b, const u8 *f) {
    void *h = Take(b, dChar.hsize);

    Conv(&dChar, f, h, NULL);
    return h;
}

static void *BuildPltt(L64Blk *b, const u8 *f) {
    void *h = Take(b, dPltt.hsize);

    Conv(&dPltt, f, h, NULL);
    return h;
}

static void *BuildPlttCmp(L64Blk *b, const u8 *f) {
    void *h = Take(b, dPlttCmp.hsize);

    Conv(&dPlttCmp, f, h, NULL);
    return h;
}

// ---- the blocks, by the file they came from

typedef struct {
    const void *file;
    u32 kind;      // the block signature it was built from
    void *mem;
    size_t cap;
} L64Unpacked;

#define FREE_DELAY 64
static L64Unpacked *sUnpacked;
static u32 sNumUnpacked, sMaxUnpacked;
static void *sFreeDelay[FREE_DELAY];
static u32 sFreeDelayIdx;
static pthread_mutex_t sLock = PTHREAD_MUTEX_INITIALIZER;

static void FreeLate(void *mem) {
    free(sFreeDelay[sFreeDelayIdx]);
    sFreeDelay[sFreeDelayIdx] = mem;
    sFreeDelayIdx = (sFreeDelayIdx + 1) % FREE_DELAY;
}

// the host structs of the block KIND of FILE (its data at DATA), built by BUILD
static void *Unpack(const void *file, u32 kind, const u8 *data, void *(*build)(L64Blk *, const u8 *)) {
    L64Blk b = {NULL, 0};
    void *res;
    u32 i;

    build(&b, data);
    pthread_mutex_lock(&sLock);
    for (i = 0; i < sNumUnpacked; i++) {
        if (sUnpacked[i].file == file && sUnpacked[i].kind == kind && sUnpacked[i].cap >= b.used) {
            break;
        }
    }
    if (i == sNumUnpacked) {
        if (sNumUnpacked == sMaxUnpacked) {
            u32 max = sMaxUnpacked != 0 ? sMaxUnpacked * 2 : 1024;
            L64Unpacked *p = realloc(sUnpacked, max * sizeof(L64Unpacked));

            if (p == NULL) {
                OS_Panic("nnsys_load64: out of memory");
            }
            sUnpacked = p;
            sMaxUnpacked = max;
        }
        sUnpacked[i].file = file;
        sUnpacked[i].kind = kind;
        sUnpacked[i].cap = b.used != 0 ? b.used : 8;
        sUnpacked[i].mem = malloc(sUnpacked[i].cap);
        if (sUnpacked[i].mem == NULL) {
            OS_Panic("nnsys_load64: out of memory");
        }
        sNumUnpacked++;
    }
    b.base = sUnpacked[i].mem;
    b.used = 0;
    res = build(&b, data);
    pthread_mutex_unlock(&sLock);
    return res;
}

void WIN_G2dFreeUnpacked(void *start, void *end) {
    u32 i = 0;

    pthread_mutex_lock(&sLock);
    while (i < sNumUnpacked) {
        if ((const u8 *)sUnpacked[i].file >= (u8 *)start && (const u8 *)sUnpacked[i].file < (u8 *)end) {
            FreeLate(sUnpacked[i].mem);
            sUnpacked[i] = sUnpacked[--sNumUnpacked];
        } else {
            i++;
        }
    }
    pthread_mutex_unlock(&sLock);
}

void WIN_G2dResetUnpacked(void) {
    u32 i;

    pthread_mutex_lock(&sLock);
    for (i = 0; i < sNumUnpacked; i++) {
        free(sUnpacked[i].mem);
    }
    for (i = 0; i < FREE_DELAY; i++) {
        free(sFreeDelay[i]);
        sFreeDelay[i] = NULL;
    }
    free(sUnpacked);
    sUnpacked = NULL;
    sNumUnpacked = sMaxUnpacked = 0;
    sFreeDelayIdx = 0;
    pthread_mutex_unlock(&sLock);
}

// how many blocks are registered (port/tests)
u32 WIN_G2dNumUnpacked(void) {
    return sNumUnpacked;
}

// ---- the entry points (lib/nnsys/src/g2d/load/*.c's, which the PC build leaves out)

NNSG2dBinaryBlockHeader *NNS_G2dFindBinaryBlock(NNSG2dBinaryFileHeader *file, u32 kind) {
    NNSG2dBinaryBlockHeader *blk = (NNSG2dBinaryBlockHeader *)((u8 *)file + file->headerSize);
    u16 i;

    for (i = 0; i < file->dataBlocks; i++) {
        if (blk->kind == kind) {
            return blk;
        }
        blk = (NNSG2dBinaryBlockHeader *)((u8 *)blk + blk->size);
    }
    return NULL;
}

// a loader: the block KIND's host structs, or NULL and FALSE when the file has no such block
static BOOL GetUnpacked(void *file, u32 kind, void *(*build)(L64Blk *, const u8 *), void **out) {
    NNSG2dBinaryBlockHeader *blk = NNS_G2dFindBinaryBlock(file, kind);

    if (blk == NULL) {
        *out = NULL;
        return FALSE;
    }
    *out = Unpack(file, kind, (const u8 *)(blk + 1), build);
    return TRUE;
}

BOOL NNS_G2dGetUnpackedCharacterData(void *file, NNSG2dCharacterData **character) {
    return GetUnpacked(file, NNS_G2D_BINBLK_SIG_CHARACTERDATA, BuildChar, (void **)character);
}

BOOL NNS_G2dGetUnpackedBGCharacterData(void *file, NNSG2dCharacterData **character) {
    return GetUnpacked(file, NNS_G2D_BINBLK_SIG_CHARACTERDATA, BuildChar, (void **)character);
}

BOOL NNS_G2dGetUnpackedPaletteData(void *file, NNSG2dPaletteData **palette) {
    return GetUnpacked(file, NNS_G2D_BINBLK_SIG_PALETTEDATA, BuildPltt, (void **)palette);
}

BOOL NNS_G2dGetUnpackedPaletteCompressInfo(void *file, NNSG2dPaletteCompressInfo **info) {
    return GetUnpacked(file, NNS_G2D_BINBLK_SIG_PALETTECOMPINFO, BuildPlttCmp, (void **)info);
}

BOOL NNS_G2dGetUnpackedCellBank(void *file, NNSG2dCellDataBank **cells) {
    return GetUnpacked(file, NNS_G2D_BLKSIG_CELLBANK, BuildCellBank, (void **)cells);
}

BOOL NNS_G2dGetUnpackedAnimBank(void *file, NNSG2dAnimBankData **anims) {
    return GetUnpacked(file, NNS_G2D_BLKSIG_ANIMBANK, BuildAnimBank, (void **)anims);
}

BOOL NNS_G2dGetUnpackedMCAnimBank(void *file, NNSG2dAnimBankData **anims) {
    return GetUnpacked(file, NNS_G2D_BLKSIG_ANIMBANK, BuildAnimBank, (void **)anims);
}

BOOL NNS_G2dGetUnpackedMultiCellBank(void *file, NNSG2dMultiCellDataBank **cells) {
    return GetUnpacked(file, NNS_G2D_BLKSIG_MULTICELLBANK, BuildMCBank, (void **)cells);
}

// the screen data holds no offsets: in place, as on the DS
BOOL NNS_G2dGetUnpackedScreenData(void *file, NNSG2dScreenData **screen) {
    NNSG2dBinaryBlockHeader *blk = NNS_G2dFindBinaryBlock(file, NNS_G2D_BINBLK_SIG_SCRDATA);

    *screen = blk != NULL ? (NNSG2dScreenData *)(blk + 1) : NULL;
    return blk != NULL;
}

const NNSG2dCellData *NNS_G2dGetCellDataByIdx(const NNSG2dCellDataBank *bank, u16 idx) {
    if (idx >= bank->numCells) {
        return NULL;
    }
    if (bank->cellBankAttr & NNS_G2D_CELLBK_ATTR_CELLWITHBR) {
        return &((NNSG2dCellDataWithBR *)bank->pCellDataArrayHead)[idx].cellData;
    }
    return &bank->pCellDataArrayHead[idx];
}

const NNSG2dAnimSequence *NNS_G2dGetAnimSequenceByIdx(const NNSG2dAnimBankData *bank, u16 idx) {
    if (bank->numSequences > idx) {
        return &bank->pSequenceArrayHead[idx];
    }
    return NULL;
}

const NNSG2dMultiCellData *NNS_G2dGetMultiCellDataByIdx(const NNSG2dMultiCellDataBank *bank, u16 idx) {
    if (idx < bank->numMultiCellData) {
        return &bank->pMultiCellDataArray[idx];
    }
    return NULL;
}

// lib/nnsys's g2d_NFT_load.c's checks: a version 1.0 file's glyphs get no rotation flags
static BOOL FontFileValid(const NNSG2dBinaryFileHeader *h, u16 version) {
    return h != NULL && h->signature == NNS_G2D_BINFILE_SIG_FONTDATA && h->version >= version;
}

BOOL NNSi_G2dGetUnpackedFont(void *pNftrFile, NNSG2dFontInformation **ppFont) {
    BOOL bOldVer = FALSE;

    if (!FontFileValid(pNftrFile, 0x102) && !FontFileValid(pNftrFile, 0x101)) {
        if (!FontFileValid(pNftrFile, 0x100)) {
            OS_Terminate();
        }
        bOldVer = TRUE;
    }
    if (NNS_G2dFindBinaryBlock(pNftrFile, NNS_G2D_BINBLK_SIG_FINFDATA) == NULL) {
        *ppFont = NULL;
        return FALSE;
    }
    *ppFont = Unpack(pNftrFile, NNS_G2D_BINBLK_SIG_FINFDATA, pNftrFile, BuildFont);
    if (bOldVer) {
        (*ppFont)->pGlyph->flags = 0;
    }
    return TRUE;
}

#endif // BWPORT_NNSYS
