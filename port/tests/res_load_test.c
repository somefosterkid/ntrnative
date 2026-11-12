// libntrsystem's resource loaders (and lib/spl's SPLManager_LoadResources) on a 64-bit host, on the ROM's own files
// (port/tests/res_load_test.py extracts them and builds this with the PC build's compile commands, under ASan).
// Each file is read into a fresh host buffer and given to the loader; the result is checked against an independent
// parse of an untouched copy (the DS layout: 4-byte offsets). Then every result of a kind is checked again after all
// files of that kind were loaded (results that shared static buffers were overwritten by later loads), and freed
// through the heap hook (WIN_G2dFreeUnpacked). NFTR only when named: the game's fonts (a/0/2/3) are read by GFL's own
// code, not NitroSystem's, and libntrsystem's NFTR loader runs off their width blocks.
// With BWPORT_NNSYS (res_load_test.py --nnsys) the loaders are port/src/nnsys_load64.c's on lib/nnsys's headers
// instead (and the G3D accessors lib/nnsys's); a file unpacked twice must then give the same host structs. With
// RES_DUMP=FILE every result is written out field by field, pointers as file offsets (F<hex>), host structs (H) or 0,
// so the two builds' dumps can be compared: identical = the same results.
// usage: res_load_test DIR [KIND ...]     (DIR/<KIND>/<n>.bin)
#include <nitro.h>
#ifdef BWPORT_NNSYS
#include <nnsys.h>
void WIN_G2dFreeUnpacked(void *start, void *end);
u32 WIN_G2dNumUnpacked(void);
// NitroSystem inlines the test uses that lib/nnsys's headers don't have (the SDK's own definitions)
static const NNSG2dUserExCellAttr *NNS_G2dGetUserExCellAttr(const NNSG2dUserExCellAttrBank *bank, u16 idx)
{
    return idx < bank->numCells ? &bank->pCellAttrArray[idx] : NULL;
}
static u32 NNS_G2dGetUserExCellAttrValue(const NNSG2dUserExCellAttr *attr) { return attr->pAttr[0]; }
static NNSG3dResDataBlockHeader *NNS_G3dGetDataBlockHeaderByIdx(const NNSG3dResFileHeader *h, u32 idx)
{
    return idx < h->dataBlocks ? (NNSG3dResDataBlockHeader *)((u8 *)h + ((const u32 *)(h + 1))[idx]) : NULL;
}
static u8 *NNS_G3dGetSbc(const NNSG3dResMdl *mdl) { return (u8 *)mdl + mdl->ofsSbc; }
#else
#include <nnsys/g2d/g2d_Load.h>
#include <nnsys/g2d/load/g2d_NCG_load.h>
#include <nnsys/g2d/load/g2d_NCL_load.h>
#include <nnsys/g2d/load/g2d_NSC_load.h>
#include <nnsys/g2d/load/g2d_NCE_load.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
#include <nnsys/g2d/load/g2d_NMC_load.h>
#include <nnsys/g2d/load/g2d_NFT_load.h>
#include <nnsys/g2d/fmt/g2d_Anim_data.h>
#include <nnsys/g3d.h>
#endif
#include <spl/spl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#undef main   // the port's headers make the game's main NitroMain

typedef struct { const char *name; u8 *orig; u32 size; u8 *buf; void *res; void *res2; u8 *buf2; } Item;

static int sErrors, sFileErrors;
static const char *sCur;

static void fail(const char *fmt, ...)
{
    char msg[400];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof msg, fmt, ap);
    va_end(ap);
    if (sFileErrors++ == 0) {
        sErrors++;
        if (sErrors <= 8)
            printf("  FAIL %s: %s\n", sCur, msg);
    }
}
#define CHECK(cond, ...) do { if (!(cond)) fail(__VA_ARGS__); } while (0)

static u16 r16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 r32(const u8 *p) { return (u32)(p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24); }

// the block of kind KIND (as the file has it) in the untouched copy: its data offset (after the 8-byte header), or 0
static u32 block(const u8 *f, const char *kind)
{
    u32 p = r16(f + 12), n = r16(f + 14), i;
    for (i = 0; i < n; i++) {
        if (memcmp(f + p, kind, 4) == 0)
            return p + 8;
        p += r32(f + p + 4);
    }
    return 0;
}

static u8 *readf(const char *path, u32 *size)
{
    FILE *fp = fopen(path, "rb");
    long n;
    u8 *p;
    if (!fp)
        return NULL;
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    p = malloc((n + 64) & ~15);
    fread(p, 1, n, fp);
    fclose(fp);
    *size = (u32)n;
    return p;
}

static u8 *copy(const Item *it)
{
    u8 *b = malloc((it->size + 64) & ~15);
    memcpy(b, it->orig, it->size);
    return b;
}

// pointer P must be the untouched copy's offset OFS, seen in the work buffer
#define AT(it, p, ofs) ((const u8 *)(p) == (it)->buf + (ofs))

// nnsys_load64.c: the same file unpacked again gives the same host structs (rebuilt in place), as the DS's in-place
// unpack of a file read again into its buffer does
#define TWICE(T, fn, file, first)                                                                                      \
    do {                                                                                                               \
        T *again_ = NULL;                                                                                              \
        CHECK(fn(file, &again_) && again_ == (first), #fn " twice: %p then %p", (void *)(first), (void *)again_);     \
    } while (0)

// ---- NCGR
static void load_ncgr(Item *it)
{
    NNSG2dCharacterData *cd = NULL, *bg = NULL;
    it->buf = copy(it);
    CHECK(NNS_G2dGetUnpackedCharacterData(it->buf, &cd) && cd, "GetUnpackedCharacterData failed");
    it->res = cd;
    {
        u8 *b2 = copy(it);
        CHECK(NNS_G2dGetUnpackedBGCharacterData(b2, &bg) && bg, "GetUnpackedBGCharacterData failed");
        it->res2 = bg;
        it->buf2 = b2;
        if (bg) {
            u32 o = block(it->orig, "RAHC");
            CHECK(bg->pRawData == b2 + o + r32(it->orig + o + 20), "BG: pRawData");
            CHECK(bg->szByte == r32(it->orig + o + 16) && bg->H == r16(it->orig + o) && bg->W == r16(it->orig + o + 2),
                  "BG: fields");
        }
    }
#ifdef BWPORT_NNSYS
    TWICE(NNSG2dCharacterData, NNS_G2dGetUnpackedCharacterData, it->buf, cd);
#else
    {
        NNSG2dCharacterPosInfo *pi = NULL;
        u32 o = block(it->orig, "SOPC");
        BOOL ok = NNS_G2dGetUnpackedCharacterPosInfo(it->buf, &pi);
        CHECK(o ? (ok && (u8 *)pi == it->buf + o) : !ok, "GetUnpackedCharacterPosInfo");
    }
#endif
}
static void check_ncgr(Item *it)
{
    const NNSG2dCharacterData *cd = it->res;
    const u8 *f = it->orig;
    u32 o = block(f, "RAHC");
    if (!cd)
        return;
    CHECK(o != 0, "no CHAR block");
    CHECK(cd->H == r16(f + o) && cd->W == r16(f + o + 2), "H W %d %d / %d %d", cd->H, cd->W, r16(f + o), r16(f + o + 2));
    CHECK(cd->pixelFmt == r32(f + o + 4) && cd->mappingType == r32(f + o + 8) && cd->characterFmt == r32(f + o + 12),
          "fmt fields");
    CHECK(cd->szByte == r32(f + o + 16), "szByte");
    CHECK(AT(it, cd->pRawData, o + r32(f + o + 20)), "pRawData");
    CHECK(memcmp(cd->pRawData, f + o + r32(f + o + 20), cd->szByte) == 0, "raw data differs");
}

// ---- NCLR
static void load_nclr(Item *it)
{
    NNSG2dPaletteData *pd = NULL;
    NNSG2dPaletteCompressInfo *ci = NULL;
    BOOL ok;
    it->buf = copy(it);
    CHECK(NNS_G2dGetUnpackedPaletteData(it->buf, &pd) && pd, "GetUnpackedPaletteData failed");
    it->res = pd;
    ok = NNS_G2dGetUnpackedPaletteCompressInfo(it->buf, &ci);
    CHECK(block(it->orig, "PMCP") ? ok && ci : !ok, "GetUnpackedPaletteCompressInfo: %d", ok);
    it->res2 = ok ? ci : NULL;
#ifdef BWPORT_NNSYS
    TWICE(NNSG2dPaletteData, NNS_G2dGetUnpackedPaletteData, it->buf, pd);
#endif
}
static void check_nclr(Item *it)
{
    const NNSG2dPaletteData *pd = it->res;
    const NNSG2dPaletteCompressInfo *ci = it->res2;
    const u8 *f = it->orig;
    u32 o = block(f, "TTLP"), c = block(f, "PMCP");
    if (!pd)
        return;
    CHECK(pd->fmt == r32(f + o) && pd->bExtendedPlt == r32(f + o + 4) && pd->szByte == r32(f + o + 8),
          "fmt bExt szByte");
    {
        u32 sz = r32(f + o + 8), avail = r32(f + o - 4) - 8 - r32(f + o + 12);
        if (sz > avail)
            sz = avail;        // a few files say more than the block holds
        CHECK(memcmp(pd->pRawData, f + o + r32(f + o + 12), sz) == 0, "palette colours differ (szByte 0x%x)",
              r32(f + o + 8));
    }
    if (ci) {
        CHECK(ci->numPalette == r16(f + c), "cmp numPalette");
        CHECK(AT(it, ci->pPlttIdxTbl, c + r32(f + c + 4)), "cmp pPlttIdxTbl");
        CHECK(memcmp(ci->pPlttIdxTbl, f + c + r32(f + c + 4), ci->numPalette * 2) == 0, "cmp table differs");
    }
}

// ---- NSCR
static void load_nscr(Item *it)
{
    NNSG2dScreenData *sd = NULL;
    it->buf = copy(it);
    CHECK(NNS_G2dGetUnpackedScreenData(it->buf, &sd) && sd, "GetUnpackedScreenData failed");
    it->res = sd;
}
static void check_nscr(Item *it)
{
    const NNSG2dScreenData *sd = it->res;
    const u8 *f = it->orig;
    u32 o = block(f, "NRCS");
    if (!sd)
        return;
    CHECK(AT(it, sd, o), "screen data where");
    CHECK(sd->screenWidth == r16(f + o) && sd->screenHeight == r16(f + o + 2) && sd->szByte == r32(f + o + 8), "fields");
    CHECK(memcmp(sd->rawData, f + o + 12, sd->szByte) == 0, "screen data differs");
}

// ---- NCER
static void load_ncer(Item *it)
{
    NNSG2dCellDataBank *cb = NULL;
    it->buf = copy(it);
    CHECK(NNS_G2dGetUnpackedCellBank(it->buf, &cb) && cb, "GetUnpackedCellBank failed");
    it->res = cb;
#ifdef BWPORT_NNSYS
    TWICE(NNSG2dCellDataBank, NNS_G2dGetUnpackedCellBank, it->buf, cb);
#endif
}
static void check_ncer(Item *it)
{
    const NNSG2dCellDataBank *cb = it->res;
    const u8 *f = it->orig;
    u32 o = block(f, "KBEC"), i;
    u32 n, attr, arr, br, oam, vt, ex;
    if (!cb)
        return;
    n = r16(f + o); attr = r16(f + o + 2); arr = o + r32(f + o + 4);
    vt = r32(f + o + 12); ex = r32(f + o + 20);
    br = attr & NNS_G2D_CELLBK_ATTR_CELLWITHBR;
    oam = arr + n * (br ? 16 : 8);
    CHECK(cb->numCells == n && cb->cellBankAttr == attr && cb->mappingMode == r32(f + o + 8), "bank fields");
    for (i = 0; i < n && i < cb->numCells; i++) {
        const NNSG2dCellData *c = NNS_G2dGetCellDataByIdx(cb, (u16)i);
        const u8 *fc = f + arr + i * (br ? 16 : 8);
        CHECK(c && c->numOAMAttrs == r16(fc) && c->cellAttr == r16(fc + 2), "cell %u fields", i);
        CHECK(c && AT(it, c->pOamAttrArray, oam + r32(fc + 4)), "cell %u pOamAttrArray", i);
        if (br && c) {
            const NNSG2dCellBoundingRectS16 *r = &((const NNSG2dCellDataWithBR *)c)->boundingRect;
            CHECK(r->maxX == (s16)r16(fc + 8) && r->maxY == (s16)r16(fc + 10) && r->minX == (s16)r16(fc + 12) &&
                      r->minY == (s16)r16(fc + 14), "cell %u bounding rect", i);
        }
    }
    if (vt) {
        const NNSG2dVramTransferData *v = cb->pVramTransferData;
        CHECK(v && v->szByteMax == r32(f + o + vt), "vram transfer szByteMax");
        CHECK(v && AT(it, v->pCellTransferDataArray, o + vt + r32(f + o + vt + 4)), "vram transfer array");
    } else {
        CHECK(cb->pVramTransferData == NULL, "pVramTransferData not NULL");
    }
    if (ex) {
        const NNSG2dUserExCellAttrBank *ab = NNS_G2dGetUserExCellAttrBankFromCellBank(cb);
        u32 ab_o = o + ex + 8, ac = ab_o + r32(f + ab_o + 4);
        CHECK(ab != NULL, "NNS_G2dGetUserExCellAttrBank NULL (blk 0x%x)", r32(f + o + ex));
        if (ab) {
            CHECK(ab->numCells == r16(f + ab_o) && ab->numAttribute == r16(f + ab_o + 2), "ex bank fields");
            for (i = 0; i < ab->numCells; i++) {
                const NNSG2dUserExCellAttr *ca = NNS_G2dGetUserExCellAttr(ab, (u16)i);
                CHECK(ca && AT(it, ca->pAttr, ab_o + r32(f + ac + 4 * i)), "ex attr %u", i);
                CHECK(ca && NNS_G2dGetUserExCellAttrValue(ca) == r32(f + ab_o + r32(f + ac + 4 * i)), "ex attr %u value", i);
            }
        }
    } else {
        CHECK(cb->pExtendedData == NULL, "pExtendedData not NULL");
    }
}

// ---- NANR / NMAR
static void load_nanr(Item *it)
{
    NNSG2dAnimBankData *ab = NULL;
    it->buf = copy(it);
    if (memcmp(it->orig, "RAMN", 4) == 0)
        CHECK(NNS_G2dGetUnpackedMCAnimBank(it->buf, &ab) && ab, "GetUnpackedMCAnimBank failed");
    else
        CHECK(NNS_G2dGetUnpackedAnimBank(it->buf, &ab) && ab, "GetUnpackedAnimBank failed");
    it->res = ab;
#ifdef BWPORT_NNSYS
    TWICE(NNSG2dAnimBankData, NNS_G2dGetUnpackedAnimBank, it->buf, ab);
#endif
}
static void check_nanr(Item *it)
{
    const NNSG2dAnimBankData *ab = it->res;
    const u8 *f = it->orig;
    u32 o = block(f, "KNBA"), i, j;
    u32 ns, seq, frm, cont, ex;
    if (!ab)
        return;
    ns = r16(f + o); seq = o + r32(f + o + 4); frm = o + r32(f + o + 8); cont = o + r32(f + o + 12);
    ex = r32(f + o + 20);
    CHECK(ab->numSequences == ns && ab->numTotalFrames == r16(f + o + 2), "bank fields");
    CHECK(AT(it, ab->pAnimContents, cont), "pAnimContents");
    for (i = 0; i < ns && i < ab->numSequences; i++) {
        const NNSG2dAnimSequenceData *s = NNS_G2dGetAnimSequenceByIdx(ab, (u16)i);
        const u8 *fs = f + seq + 16 * i;
        CHECK(s && s->numFrames == r16(fs) && s->loopStartFrameIdx == r16(fs + 2) && s->animType == r32(fs + 4) &&
                  s->playMode == r32(fs + 8), "seq %u fields", i);
#ifdef BWPORT_NNSYS
        CHECK(s && s - ab->pSequenceArrayHead == i, "seq %u index", i);
#else
        CHECK(s && NNS_G2dGetAnimSequenceIndex(ab, s) == i, "seq %u NNS_G2dGetAnimSequenceIndex", i);
#endif
        for (j = 0; s && j < s->numFrames; j++) {
            const u8 *ff = f + frm + r32(fs + 12) + 8 * j;
            CHECK(AT(it, s->pAnmFrameArray[j].pContent, cont + r32(ff)), "seq %u frame %u pContent", i, j);
            CHECK(s->pAnmFrameArray[j].frames == r16(ff + 4), "seq %u frame %u frames", i, j);
        }
    }
    if (ex) {
        const NNSG2dUserExAnimAttrBank *xb = NNS_G2dGetUserExAnimAttrBank(ab);
        u32 xo = o + ex + 8, sa = xo + r32(f + xo + 4);
        CHECK(xb != NULL, "NNS_G2dGetUserExAnimAttrBank NULL (blk 0x%x)", r32(f + o + ex));
        if (xb) {
            CHECK(xb->numSequences == r16(f + xo) && xb->numAttribute == r16(f + xo + 2), "ex bank fields");
            for (i = 0; i < xb->numSequences; i++) {
                const NNSG2dUserExAnimSequenceAttr *a = NNS_G2dGetUserExAnimSequenceAttr(xb, (u16)i);
                const u8 *fa = f + sa + 12 * i;
                CHECK(a && a->numFrames == r16(fa), "ex seq %u numFrames", i);
                CHECK(a && AT(it, a->pAttr, xo + r32(fa + 4)), "ex seq %u pAttr", i);
                for (j = 0; a && j < a->numFrames; j++) {
                    const u8 *fr = f + xo + r32(fa + 8) + 4 * j;
                    CHECK(AT(it, a->pAnmFrmAttrArray[j].pAttr, xo + r32(fr)), "ex seq %u frame %u pAttr", i, j);
                }
            }
        }
    } else {
        CHECK(ab->pExtendedData == NULL, "pExtendedData not NULL (%p) when the file has none", ab->pExtendedData);
    }
}

// ---- NMCR
static void load_nmcr(Item *it)
{
    NNSG2dMultiCellDataBank *mb = NULL;
    it->buf = copy(it);
    CHECK(NNS_G2dGetUnpackedMultiCellBank(it->buf, &mb) && mb, "GetUnpackedMultiCellBank failed");
    it->res = mb;
#ifdef BWPORT_NNSYS
    TWICE(NNSG2dMultiCellDataBank, NNS_G2dGetUnpackedMultiCellBank, it->buf, mb);
#endif
}
static void check_nmcr(Item *it)
{
    const NNSG2dMultiCellDataBank *mb = it->res;
    const u8 *f = it->orig;
    u32 o = block(f, "KBCM"), i, n, arr, hier, ex;
    if (!mb)
        return;
    n = r16(f + o); arr = o + r32(f + o + 4); hier = o + r32(f + o + 8); ex = r32(f + o + 16);
    CHECK(mb->numMultiCellData == n, "numMultiCellData");
    CHECK(AT(it, mb->pHierarchyDataArray, hier), "pHierarchyDataArray");
    for (i = 0; i < n && i < mb->numMultiCellData; i++) {
        const NNSG2dMultiCellData *m = NNS_G2dGetMultiCellDataByIdx(mb, (u16)i);
        const u8 *fm = f + arr + 8 * i;
        CHECK(m && m->numNodes == r16(fm) && m->numCellAnim == r16(fm + 2), "mc %u fields", i);
        CHECK(m && AT(it, m->pHierDataArray, hier + r32(fm + 4)), "mc %u pHierDataArray", i);
    }
    if (ex) {
        const NNSG2dUserExCellAttrBank *ab = NNS_G2dGetUserExCellAttrBankFromMCBank(mb);
        u32 ab_o = o + ex + 8, ac = ab_o + r32(f + ab_o + 4);
        CHECK(ab != NULL, "GetUserExCellAttrBankFromMCBank NULL");
        for (i = 0; ab && i < ab->numCells; i++) {
            const NNSG2dUserExCellAttr *ca = NNS_G2dGetUserExCellAttr(ab, (u16)i);
            CHECK(ca && AT(it, ca->pAttr, ab_o + r32(f + ac + 4 * i)), "ex attr %u", i);
        }
    } else {
        CHECK(mb->pExtendedData == NULL, "pExtendedData not NULL");
    }
}

// ---- NFTR (the game uses its own font format; libntrsystem's loader tested anyway)
static void load_nftr(Item *it)
{
    NNSG2dFontInformation *fi = NULL;
    it->buf = copy(it);
    CHECK(NNSi_G2dGetUnpackedFont(it->buf, &fi) && fi, "NNSi_G2dGetUnpackedFont failed");
    it->res = fi;
}
static void check_nftr(Item *it)
{
    const NNSG2dFontInformation *fi = it->res;
    const u8 *f = it->orig;
    u32 o = block(f, "FNIF");
    const NNSG2dFontWidth *w;
    const NNSG2dFontCodeMap *m;
    u32 wo, mo;
    if (!fi)
        return;
    CHECK(fi->fontType == f[o] && fi->linefeed == (s8)f[o + 1] && fi->alterCharIndex == r16(f + o + 2), "info fields");
    CHECK(AT(it, fi->pGlyph, r32(f + o + 8)), "pGlyph");
    // the offsets are from the file's start to a block's data
    for (w = fi->pWidth, wo = r32(f + o + 12); wo; wo = r32(f + wo + 4), w = w->pNext) {
        CHECK(w != NULL, "width chain shorter than the file's");
        if (!w)
            break;
        CHECK(w->indexBegin == r16(f + wo) && w->indexEnd == r16(f + wo + 2), "width block fields");
#ifdef BWPORT_NNSYS   // lib/nnsys's headers hold the table after the struct: a copy
        CHECK(memcmp(w->widthTable, f + wo + 8, (w->indexEnd - w->indexBegin + 1) * 3) == 0, "widthTable");
#else
        CHECK(AT(it, w->widthTable, wo + 8), "widthTable");
#endif
    }
    CHECK(w == NULL, "width chain longer than the file's");
    for (m = fi->pMap, mo = r32(f + o + 16); mo; mo = r32(f + mo + 8), m = m->pNext) {
        CHECK(m != NULL, "cmap chain shorter than the file's");
        if (!m)
            break;
        CHECK(m->ccodeBegin == r16(f + mo) && m->ccodeEnd == r16(f + mo + 2) && m->mappingMethod == r16(f + mo + 4),
              "cmap fields");
#ifdef BWPORT_NNSYS
        CHECK(memcmp(m->mapInfo, f + mo + 12, r32(f + mo - 4) - 20) == 0, "cmap mapInfo");
#else
        CHECK(AT(it, m->mapInfo, mo + 12), "cmap mapInfo");
#endif
    }
    CHECK(m == NULL, "cmap chain longer than the file's");
}

// ---- G3D (offset-based: the accessors on host pointers)
static void load_g3d(Item *it)
{
    it->buf = copy(it);
    it->res = it->buf;
}
static void check_g3d(Item *it)
{
    const u8 *f = it->orig;
    NNSG3dResFileHeader *h = (NNSG3dResFileHeader *)it->buf;
    u32 nblk = r16(f + 14), i, j;
    CHECK(h->dataBlocks == nblk, "dataBlocks");
    for (i = 0; i < nblk; i++) {
        u32 bo = r32(f + 16 + 4 * i);
        const u8 *blk = (const u8 *)NNS_G3dGetDataBlockHeaderByIdx(h, i);
        CHECK(AT(it, blk, bo), "block %u", i);
        if (memcmp(f + bo, "MDL0", 4) == 0) {
            NNSG3dResMdlSet *ms = NNS_G3dGetMdlSet(h);
            u32 d = bo + 8, n = f[d + 1], units = d + r16(f + d + 6);
            CHECK(AT(it, ms, bo), "NNS_G3dGetMdlSet");
            for (j = 0; ms && j < n; j++) {
                NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(ms, j);
                u32 mo = bo + r32(f + units + 4 + 4 * j);
                CHECK(AT(it, mdl, mo), "model %u", j);
                if (mdl) {
                    NNSG3dResNodeInfo *ni = NNS_G3dGetNodeInfo(mdl);
                    NNSG3dResMatData *mat;
                    // size, ofsSbc, ofsMat, ofsShp, ofsEvpMtx, the model info, the node info in place
                    CHECK((u8 *)ni > (u8 *)mdl && (u8 *)ni < (u8 *)mdl + 0x80, "model %u NNS_G3dGetNodeInfo", j);
                    CHECK(AT(it, NNS_G3dGetSbc(mdl), mo + r32(f + mo + 4)), "model %u sbc", j);
                    CHECK(AT(it, NNS_G3dGetMat(mdl), mo + r32(f + mo + 8)), "model %u mat", j);
                    CHECK(AT(it, NNS_G3dGetShp(mdl), mo + r32(f + mo + 12)), "model %u shp", j);
                    if (mdl->info.numMat) {
                        mat = NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(mdl), 0);
                        CHECK((u8 *)mat > it->buf && (u8 *)mat < it->buf + it->size, "model %u mat 0 out of the file", j);
                    }
                    if (mdl->info.numShp) {
                        NNSG3dResShpData *shp = NNS_G3dGetShpDataByIdx(NNS_G3dGetShp(mdl), 0);
                        CHECK((u8 *)shp > it->buf && (u8 *)shp < it->buf + it->size, "model %u shp 0 out of the file", j);
                    }
                }
            }
        } else if (memcmp(f + bo, "TEX0", 4) == 0) {
            NNSG3dResTex *tex = NNS_G3dGetTex(h);
            CHECK(AT(it, tex, bo), "NNS_G3dGetTex");
            if (tex) {
                CHECK(NNS_G3dTexGetRequiredSize(tex) == (u32)r16(f + bo + 12) << 3, "NNS_G3dTexGetRequiredSize");
                CHECK(NNS_G3dPlttGetRequiredSize(tex) == (u32)r16(f + bo + 0x30) << 3, "NNS_G3dPlttGetRequiredSize");
            }
        } else {
            // an animation block: the dict's entries
            u32 d = bo + 8, n = f[d + 1], units = d + r16(f + d + 6);
            for (j = 0; j < n; j++) {
                void *anm = NNS_G3dGetAnmByIdx(h, j);
                CHECK(AT(it, anm, bo + r32(f + units + 4 + 4 * j)), "anm %u", j);
            }
        }
    }
}

// models bound to their file's own textures and palettes (fake VRAM keys): what GFL's g3d does with real ones
static int sBindTried, sBindOK;
static void bind_own_tex(Item *it)
{
    NNSG3dResFileHeader *h = (NNSG3dResFileHeader *)it->buf;
    NNSG3dResMdlSet *ms = NNS_G3dGetMdlSet(h);
    NNSG3dResTex *tex = NNS_G3dGetTex(h);
    u32 j, sz4;
    if (!ms || !tex)
        return;
    sz4 = NNS_G3dTex4x4GetRequiredSize(tex);
    NNS_G3dTexSetTexKey(tex, NNS_GfdMakeTexKey(0x1000, NNS_G3dTexGetRequiredSize(tex), FALSE),
                        sz4 ? NNS_GfdMakeTexKey(0x20000, sz4, TRUE) : 0);
    NNS_G3dPlttSetPlttKey(tex, NNS_GfdMakePlttKey(0x100, NNS_G3dPlttGetRequiredSize(tex)));
    for (j = 0; j < ms->dict.numEntry; j++) {
        NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(ms, j);
        BOOL t = NNS_G3dBindMdlTex(mdl, tex), p = NNS_G3dBindMdlPltt(mdl, tex);
        sBindTried++;
        sBindOK += t && p;
    }
}

// ---- SPA (lib/spl's SPLManager_LoadResources, func_02052a50)
void func_02052a50(SPLManager *mgr, const void *data);
void MI_CpuFill8(void *dest, u8 data, u32 size) { memset(dest, data, size); }
static void *spl_alloc(u32 size) { return calloc(1, size + 16); }
static void load_spa(Item *it)
{
    SPLManager *mgr = calloc(1, sizeof(SPLManager));
    it->buf = copy(it);
    mgr->alloc = spl_alloc;
    func_02052a50(mgr, it->buf);
    it->res = mgr;
}
static void check_spa(Item *it)
{
    SPLManager *mgr = it->res;
    const u8 *f = it->orig;
    u32 i, to = r32(f + 0x18), p = to;
    CHECK(mgr->resourceCount == r16(f + 8) && mgr->textureCount == r16(f + 10), "counts");
    for (i = 0; i < mgr->textureCount; i++) {
        CHECK(AT(it, mgr->textures[i].data, p), "texture %u where (struct sizes off)", i);
        CHECK(memcmp(f + p, " TPS", 4) == 0, "texture %u signature", i);
        p += r32(f + p + 28);
    }
    CHECK(mgr->textureCount == 0 || p == to + r32(f + 0x14), "texture end");
    if (mgr->textureCount)
        CHECK(AT(it, mgr->textures[0].data, 0x20 + r32(f + 0x10)), "resources' size: textures at 0x%x, file says 0x%x",
              (u32)((u8 *)mgr->textures[0].data - it->buf), 0x20 + r32(f + 0x10));
}

// ---- RES_DUMP: every result field by field, the same text from either set of loaders when their results agree
static FILE *sDump;
static const u8 *sDumpBuf;
static u32 sDumpSize;
static void P(const char *name, const void *p)
{
    if (p == NULL)
        fprintf(sDump, " %s=0", name);
    else if ((const u8 *)p >= sDumpBuf && (const u8 *)p < sDumpBuf + sDumpSize + 16)
        fprintf(sDump, " %s=F%x", name, (u32)((const u8 *)p - sDumpBuf));
    else
        fprintf(sDump, " %s=H", name);
}
static void V(const char *name, u32 v) { fprintf(sDump, " %s=%x", name, v); }
static void HEX(const char *name, const void *p, u32 n)
{
    const u8 *b = p;
    u32 i, h = 2166136261u;
    for (i = 0; i < n; i++)
        h = (h ^ b[i]) * 16777619u;
    fprintf(sDump, " %s=%x/%x", name, n, h);
}
static void dump_cellattrs(const void *ex)
{
    const NNSG2dUserExDataBlock *blk = ex;
    const NNSG2dUserExCellAttrBank *ab = (const NNSG2dUserExCellAttrBank *)(blk + 1);
    u32 i;
    fprintf(sDump, "\n  ex"); V("id", blk->blkTypeID); V("size", blk->blkSize);
    V("numCells", ab->numCells); V("numAttribute", ab->numAttribute); P("array", ab->pCellAttrArray);
    for (i = 0; i < ab->numCells; i++)
        P("a", ab->pCellAttrArray[i].pAttr);
}
static void dump_item(const char *kind, Item *it)
{
    u32 i, j;
    sDumpBuf = it->buf; sDumpSize = it->size;
    fprintf(sDump, "%s", it->name);
    if (!strcmp(kind, "NCGR")) {
        const NNSG2dCharacterData *c = it->res;
        if (c) {
            V("H", c->H); V("W", c->W); V("pixelFmt", c->pixelFmt); V("mappingType", c->mappingType);
            V("characterFmt", c->characterFmt); V("szByte", c->szByte); P("pRawData", c->pRawData);
        }
    } else if (!strcmp(kind, "NCLR")) {
        const NNSG2dPaletteData *pd = it->res;
        const NNSG2dPaletteCompressInfo *ci = it->res2;
        if (pd) {
            V("fmt", pd->fmt); V("bExt", pd->bExtendedPlt); V("szByte", pd->szByte); P("pRawData", pd->pRawData);
        }
        if (ci) {
            V("numPalette", ci->numPalette); V("pad16", ci->pad16); P("pPlttIdxTbl", ci->pPlttIdxTbl);
        }
    } else if (!strcmp(kind, "NSCR")) {
        P("screen", it->res);
    } else if (!strcmp(kind, "NCER")) {
        const NNSG2dCellDataBank *cb = it->res;
        if (cb) {
            V("numCells", cb->numCells); V("attr", cb->cellBankAttr); V("mappingMode", cb->mappingMode);
            P("pCellDataArrayHead", cb->pCellDataArrayHead); P("pStringBank", cb->pStringBank);
            for (i = 0; i < cb->numCells; i++) {
                const NNSG2dCellData *c = NNS_G2dGetCellDataByIdx(cb, (u16)i);
                fprintf(sDump, "\n  cell"); V("n", c->numOAMAttrs); V("attr", c->cellAttr); P("oam", c->pOamAttrArray);
                if (cb->cellBankAttr & NNS_G2D_CELLBK_ATTR_CELLWITHBR) {
                    const NNSG2dCellBoundingRectS16 *r = &((const NNSG2dCellDataWithBR *)c)->boundingRect;
                    V("maxX", (u16)r->maxX); V("maxY", (u16)r->maxY); V("minX", (u16)r->minX); V("minY", (u16)r->minY);
                }
            }
            P("pVramTransferData", cb->pVramTransferData);
            if (cb->pVramTransferData) {
                V("szByteMax", cb->pVramTransferData->szByteMax);
                P("transfers", cb->pVramTransferData->pCellTransferDataArray);
            }
            P("pExtendedData", cb->pExtendedData);
            if (cb->pExtendedData)
                dump_cellattrs(cb->pExtendedData);
        }
    } else if (!strcmp(kind, "NANR") || !strcmp(kind, "NMAR")) {
        const NNSG2dAnimBankData *ab = it->res;
        if (ab) {
            V("numSequences", ab->numSequences); V("numTotalFrames", ab->numTotalFrames);
            P("pSequenceArrayHead", ab->pSequenceArrayHead); P("pFrameArrayHead", ab->pFrameArrayHead);
            P("pAnimContents", ab->pAnimContents); P("pStringBank", ab->pStringBank);
            for (i = 0; i < ab->numSequences; i++) {
                const NNSG2dAnimSequenceData *s = &ab->pSequenceArrayHead[i];
                fprintf(sDump, "\n  seq"); V("numFrames", s->numFrames); V("loop", s->loopStartFrameIdx);
                V("animType", s->animType); V("playMode", s->playMode);
                V("frameIdx", (u32)(s->pAnmFrameArray - ab->pFrameArrayHead));
                for (j = 0; j < s->numFrames; j++) {
                    P("c", s->pAnmFrameArray[j].pContent); V("f", s->pAnmFrameArray[j].frames);
                    V("p", s->pAnmFrameArray[j].pad16);
                }
            }
            P("pExtendedData", ab->pExtendedData);
            if (ab->pExtendedData) {
                const NNSG2dUserExDataBlock *blk = ab->pExtendedData;
                const NNSG2dUserExAnimAttrBank *xb = (const NNSG2dUserExAnimAttrBank *)(blk + 1);
                fprintf(sDump, "\n  ex"); V("id", blk->blkTypeID); V("size", blk->blkSize);
                V("numSequences", xb->numSequences); V("numAttribute", xb->numAttribute);
                for (i = 0; i < xb->numSequences; i++) {
                    const NNSG2dUserExAnimSequenceAttr *a = &xb->pAnmSeqAttrArray[i];
                    fprintf(sDump, "\n  exseq"); V("numFrames", a->numFrames); V("pad16", a->pad16); P("pAttr", a->pAttr);
                    for (j = 0; j < a->numFrames; j++)
                        P("a", a->pAnmFrmAttrArray[j].pAttr);
                }
            }
        }
    } else if (!strcmp(kind, "NMCR")) {
        const NNSG2dMultiCellDataBank *mb = it->res;
        if (mb) {
            V("num", mb->numMultiCellData); V("pad16", mb->pad16); P("pMultiCellDataArray", mb->pMultiCellDataArray);
            P("pHierarchyDataArray", mb->pHierarchyDataArray); P("pStringBank", mb->pStringBank);
            for (i = 0; i < mb->numMultiCellData; i++) {
                const NNSG2dMultiCellData *m = &mb->pMultiCellDataArray[i];
                fprintf(sDump, "\n  mc"); V("numNodes", m->numNodes); V("numCellAnim", m->numCellAnim);
                P("pHierDataArray", m->pHierDataArray);
            }
            P("pExtendedData", mb->pExtendedData);
            if (mb->pExtendedData)
                dump_cellattrs(mb->pExtendedData);
        }
    } else if (!strncmp(kind, "NFTR", 4)) {
        const NNSG2dFontInformation *fi = it->res;
        const NNSG2dFontWidth *w;
        const NNSG2dFontCodeMap *m;
        if (fi) {
            V("fontType", fi->fontType); V("linefeed", (u8)fi->linefeed); V("alter", fi->alterCharIndex);
            V("left", (u8)fi->defaultWidth.left); V("glyphWidth", fi->defaultWidth.glyphWidth);
            V("charWidth", (u8)fi->defaultWidth.charWidth); V("encoding", fi->encoding); P("pGlyph", fi->pGlyph);
            for (w = fi->pWidth; w; w = w->pNext) {
                fprintf(sDump, "\n  width"); V("begin", w->indexBegin); V("end", w->indexEnd);
                HEX("table", w->widthTable, (w->indexEnd - w->indexBegin + 1) * 3);
            }
            for (m = fi->pMap; m; m = m->pNext) {
                u32 n = m->mappingMethod == NNS_G2D_MAPMETHOD_DIRECT ? 2
                        : m->mappingMethod == NNS_G2D_MAPMETHOD_TABLE ? (m->ccodeEnd - m->ccodeBegin + 1) * 2
                        : 2 + 4 * m->mapInfo[0];
                fprintf(sDump, "\n  map"); V("begin", m->ccodeBegin); V("end", m->ccodeEnd);
                V("method", m->mappingMethod); V("reserved", m->reserved); HEX("info", m->mapInfo, n);
            }
        }
    }
    fprintf(sDump, "\n");
}

typedef struct { const char *kind; void (*load)(Item *); void (*check)(Item *); } Kind;
static const Kind sKinds[] = {
    {"NCGR", load_ncgr, check_ncgr}, {"NCLR", load_nclr, check_nclr}, {"NSCR", load_nscr, check_nscr},
    {"NCER", load_ncer, check_ncer}, {"NANR", load_nanr, check_nanr}, {"NMAR", load_nanr, check_nanr},
    {"NMCR", load_nmcr, check_nmcr}, {"NFTR", load_nftr, check_nftr}, {"NFTRDWC", load_nftr, check_nftr},
    {"NSBMD", load_g3d, check_g3d}, {"NSBTX", load_g3d, check_g3d}, {"NSBCA", load_g3d, check_g3d},
    {"NSBTP", load_g3d, check_g3d}, {"NSBTA", load_g3d, check_g3d}, {"NSBMA", load_g3d, check_g3d},
    {"NSBVA", load_g3d, check_g3d}, {"SPA", load_spa, check_spa},
};

int main(int argc, char **argv)
{
    const char *dir = argv[1];
    u32 k;
    int total = 0;
    if (getenv("RES_DUMP"))
        sDump = fopen(getenv("RES_DUMP"), "w");
    for (k = 0; k < sizeof sKinds / sizeof sKinds[0]; k++) {
        const Kind *K = &sKinds[k];
        Item *items;
        int n = 0, i, bad1, bad2, a;
        char path[600];
        if (argc > 2) {
            for (a = 2; a < argc && strcmp(argv[a], K->kind); a++)
                ;
            if (a == argc)
                continue;
        } else if (strcmp(K->kind, "NFTR") == 0) {
            continue;
        }
        for (;; n++) {
            u32 sz;
            snprintf(path, sizeof path, "%s/%s/%d.bin", dir, K->kind, n);
            FILE *fp = fopen(path, "rb");
            if (!fp)
                break;
            fclose(fp);
        }
        items = calloc(n + 1, sizeof(Item));
        sErrors = 0;
        for (i = 0; i < n; i++) {
            snprintf(path, sizeof path, "%s/%d.bin", K->kind, i);
            items[i].name = strdup(path);
            snprintf(path, sizeof path, "%s/%s/%d.bin", dir, K->kind, i);
            items[i].orig = readf(path, &items[i].size);
            sCur = items[i].name; sFileErrors = 0;
            K->load(&items[i]);
            K->check(&items[i]);
        }
        bad1 = sErrors;
        sErrors = 0;
        for (i = 0; i < n; i++) {   // all of them again, after the whole kind was loaded
            sCur = items[i].name; sFileErrors = 0;
            K->check(&items[i]);
        }
        bad2 = sErrors;
        if (sDump) {
            for (i = 0; i < n; i++)
                dump_item(K->kind, &items[i]);
        }
#if defined(WIN_G2D_ALIGN8) || defined(BWPORT_NNSYS)
        for (i = 0; i < n; i++)     // the heap hook's free, as when the game frees the files
            WIN_G2dFreeUnpacked(items[i].buf, items[i].buf + items[i].size);
        for (i = 0; i < n; i++)
            if (items[i].buf2)
                WIN_G2dFreeUnpacked(items[i].buf2, items[i].buf2 + items[i].size);
#endif
#ifdef BWPORT_NNSYS
        if (WIN_G2dNumUnpacked() != 0) {
            printf("  FAIL %s: %u blocks left registered after the files' frees\n", K->kind, WIN_G2dNumUnpacked());
            bad2++;
        }
#endif
        printf("%-6s %5d files: %s%d bad right after loading, %d bad after loading all %d\n", K->kind, n,
               bad1 || bad2 ? "" : "OK ", bad1, bad2, n);
        if (strcmp(K->kind, "NSBMD") == 0) {
            for (i = 0; i < n; i++)
                bind_own_tex(&items[i]);
            printf("NSBMD  models bound to their file's own textures and palettes: %d of %d fully\n", sBindOK, sBindTried);
        }
        fflush(stdout);
        total += bad1 + bad2;
    }
    return total ? 1 : 0;
}
