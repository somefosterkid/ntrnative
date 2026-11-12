/* port/vcore: the DS video core's shared types (docs/vcore.md). OWNED BY THE SKELETON: a piece never edits this
 * file. A piece keeps its own data in its state struct (its pNN_*.h); data two pieces must share goes in a request
 * in the piece's report.
 *
 * Conventions (all pieces):
 * - Colours: 2D colours are the DS's BGR555 (r | g << 5 | b << 10). The 3D engine's and the screens' colours are
 *   18-bit "RGB6": r6 | g6 << 8 | b6 << 16 (melonDS's format), a 3D pixel's alpha (0-31) in bits 24-28.
 * - Fixed point as the hardware: matrices and vertex coordinates 20.12 (s32, 0x1000 = 1.0), texture coordinates
 *   12.4 (s16, 16 = one texel).
 * - Matrices are 4x4 row-major as the matrix commands send them (m[0..3] the first row); a vector is a row vector:
 *   v' = v * M, so m[12..14] is the translation (GBATEK 42).
 */
#ifndef VC_TYPES_H
#define VC_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

enum { VC_W = 256, VC_H = 192 };
enum { VC_ENG_A = 0, VC_ENG_B = 1 };
enum { VC_SCREEN_TOP = 0, VC_SCREEN_BOTTOM = 1 };

/* ---- colours ---- */
#define VC_RGB6(r, g, b) ((u32)(r) | ((u32)(g) << 8) | ((u32)(b) << 16))
#define VC_R6(c) ((c) & 0x3F)
#define VC_G6(c) (((c) >> 8) & 0x3F)
#define VC_B6(c) (((c) >> 16) & 0x3F)
#define VC_A5(c) (((c) >> 24) & 0x1F) /* a 3D pixel's alpha */
/* BGR555 -> RGB6 the way the 2D engine widens it (c5 << 1) */
static inline u32 vc_555_to_rgb6(u16 c) {
  return VC_RGB6((c & 0x1F) << 1, ((c >> 5) & 0x1F) << 1, ((c >> 10) & 0x1F) << 1);
}

/* ---- the I/O registers (offsets from 0x04000000; an engine B 2D register is the same offset + 0x1000) ---- */
enum {
  VC_REG_DISPCNT = 0x000, VC_REG_DISPSTAT = 0x004, VC_REG_VCOUNT = 0x006,
  VC_REG_BG0CNT = 0x008, VC_REG_BG1CNT = 0x00A, VC_REG_BG2CNT = 0x00C, VC_REG_BG3CNT = 0x00E,
  VC_REG_BG0HOFS = 0x010, VC_REG_BG0VOFS = 0x012, /* BGn: + 4 * n */
  VC_REG_BG2PA = 0x020, VC_REG_BG2PB = 0x022, VC_REG_BG2PC = 0x024, VC_REG_BG2PD = 0x026,
  VC_REG_BG2X = 0x028, VC_REG_BG2Y = 0x02C, /* BG3: + 0x10 */
  VC_REG_WIN0H = 0x040, VC_REG_WIN1H = 0x042, VC_REG_WIN0V = 0x044, VC_REG_WIN1V = 0x046,
  VC_REG_WININ = 0x048, VC_REG_WINOUT = 0x04A, VC_REG_MOSAIC = 0x04C,
  VC_REG_BLDCNT = 0x050, VC_REG_BLDALPHA = 0x052, VC_REG_BLDY = 0x054,
  VC_REG_DISP3DCNT = 0x060, VC_REG_DISPCAPCNT = 0x064, VC_REG_DISP_MMEM_FIFO = 0x068,
  VC_REG_MASTER_BRIGHT = 0x06C,
  VC_REG_VRAMCNT_A = 0x240, /* A-G 0x240-0x246, WRAMCNT 0x247, H 0x248, I 0x249 */
  VC_REG_POWCNT1 = 0x304,
  VC_REG_RDLINES_COUNT = 0x320,
  VC_REG_EDGE_COLOR = 0x330, /* 8 x u16 */
  VC_REG_ALPHA_TEST_REF = 0x340,
  VC_REG_CLEAR_COLOR = 0x350, VC_REG_CLEAR_DEPTH = 0x354, VC_REG_CLRIMAGE_OFFSET = 0x356,
  VC_REG_FOG_COLOR = 0x358, VC_REG_FOG_OFFSET = 0x35C,
  VC_REG_FOG_TABLE = 0x360,  /* 32 x u8 */
  VC_REG_TOON_TABLE = 0x380, /* 32 x u16 */
  VC_REG_GXSTAT = 0x600,
  VC_REG_DISP_1DOT_DEPTH = 0x610,
};
enum { VC_IO9_SIZE = 0x700, VC_IO9B_SIZE = 0x70 };

/* ---- VRAM: the nine banks, each its own memory (the .vcs VRAM chunk: A-I in this order) ---- */
enum { VC_BANK_A, VC_BANK_B, VC_BANK_C, VC_BANK_D, VC_BANK_E, VC_BANK_F, VC_BANK_G, VC_BANK_H, VC_BANK_I, VC_NBANKS };
enum { VC_VRAM_SIZE = 0xA4000 };
extern const u32 vc_bank_size[VC_NBANKS];   /* 128K x4, 64K, 16K, 16K, 32K, 16K */
extern const u32 vc_bank_offset[VC_NBANKS]; /* each bank's offset in VCoreState.vram (also its LCDC offset) */

/* A view of VRAM as one engine or unit sees it: 16 KB pages, each the OR of the banks mapped there (GBATEK 22: a
 * page with two banks reads both OR-ed). Built by piece 12 from VRAMCNT; read with vc_rd8/16/32 (vc_shared.h).
 * The address wraps at npages * 16 KB. */
enum { VC_PAGE_SHIFT = 14, VC_PAGE_SIZE = 1 << VC_PAGE_SHIFT, VC_PAGE_SRCS = 4, VC_MAX_PAGES = 48 };
typedef struct VC_Page {
  const u8 *src[VC_PAGE_SRCS]; /* the 16 KB of each bank mapped at this page */
  u8 n;                        /* how many: 0 = unmapped (reads 0) */
} VC_Page;
typedef struct VC_Region {
  u32 npages;
  VC_Page page[VC_MAX_PAGES];
} VC_Region;
enum {
  VC_RGN_ABG,     /* engine A BG, 0x06000000, 512 KB (32 pages) */
  VC_RGN_AOBJ,    /* engine A OBJ, 0x06400000, 256 KB (16) */
  VC_RGN_BBG,     /* engine B BG, 0x06200000, 128 KB (8) */
  VC_RGN_BOBJ,    /* engine B OBJ, 0x06600000, 128 KB (8) */
  VC_RGN_LCDC,    /* 0x06800000, 656 KB (41): the banks in LCDC mode */
  VC_RGN_ABGEXT,  /* engine A BG extended palettes: slots 0-3 of 8 KB (32 KB, 2 pages) */
  VC_RGN_AOBJEXT, /* engine A OBJ extended palette, 8 KB (1 page, the first half) */
  VC_RGN_BBGEXT,  /* engine B BG extended palettes, 32 KB */
  VC_RGN_BOBJEXT, /* engine B OBJ extended palette, 8 KB */
  VC_RGN_TEX,     /* texture image slots 0-3 of 128 KB (512 KB, 32 pages) */
  VC_RGN_TEXPAL,  /* texture palette slots 0-5 of 16 KB (96 KB, 6 pages) */
  VC_NREGIONS
};

/* ---- the 2D engine's line buffers (one engine, one line), filled in the order of vc_render_line ---- */
/* A layer pixel (bg[], obj[]): 0 = transparent. */
#define VC_PX_COLOR 0x7FFFu            /* BGR555 */
#define VC_PX_OPAQUE (1u << 15)        /* a pixel is there */
#define VC_PX_OBJPRIO_SHIFT 16         /* obj[] only: the OBJ's priority (OAM attr 2 bits 10-11), bits 16-17 */
#define VC_PX_SEMI (1u << 18)          /* obj[]: a semi-transparent OBJ (OBJ mode 1) */
#define VC_PX_BMPOBJ (1u << 19)        /* obj[]: a bitmap OBJ; its alpha (OAM attr 2 bits 12-15) in VC_PX_ALPHA */
#define VC_PX_MOSAIC (1u << 20)        /* obj[]: an OBJ with the mosaic flag (for the horizontal OBJ mosaic) */
#define VC_PX_ALPHA_SHIFT 24           /* obj[]: bitmap OBJ alpha 0-15, bits 24-27 */
#define VC_PX_OBJPRIO(px) (((px) >> VC_PX_OBJPRIO_SHIFT) & 3)

/* the window mask (winmask[]): WININ/WINOUT's per-window bit layout */
enum { VC_WIN_BG0 = 1, VC_WIN_BG1 = 2, VC_WIN_BG2 = 4, VC_WIN_BG3 = 8, VC_WIN_OBJ = 16, VC_WIN_EFFECT = 32 };

typedef struct VC_Line {
  u32 bg[4][VC_W]; /* each BG's pixels (VC_PX_*), as its renderer wrote them (BG0 stays empty when it is the 3D) */
  u32 l3d[VC_W];   /* engine A: the 3D layer's pixels (RGB6 | alpha << 24, alpha 0 = none) when BG0 is the 3D */
  u8 bg_on[4];     /* 1 where BG n was rendered this line (enabled in DISPCNT and present in the BG mode) */
  u8 bg3d;         /* 1 when BG0 is the 3D layer (engine A, DISPCNT bit 3) */
  u32 obj[VC_W];   /* the OBJ layer: the winning OBJ pixel (VC_PX_* incl. OBJ priority and flags) */
  u8 objwin[VC_W]; /* 1 where an OBJ-window OBJ (OBJ mode 2) has an opaque pixel */
  u8 winmask[VC_W]; /* per pixel which layers and effects show (VC_WIN_*); 0x3F when windows are off */
  u32 out[VC_W];   /* the composited line (RGB6), before display mode and master brightness */
} VC_Line;

/* One OAM entry, decoded (piece 5 fills it with vc_obj_decode; GBATEK 15). */
typedef struct VC_ObjAttr {
  u16 attr[3];  /* the raw words */
  int index;    /* 0-127 */
  int x, y;     /* the top-left: x -256..255, y -128..127 handled as the hardware wraps (piece 5 says how) */
  int w, h;     /* the OBJ's size in pixels (shape x size table) */
  int affine;   /* attr 0 bit 8 */
  int dbl;      /* affine: double size (attr 0 bit 9) */
  int hidden;   /* not affine and attr 0 bit 9 */
  int mode;     /* attr 0 bits 10-11: 0 normal, 1 semi-transparent, 2 OBJ window, 3 bitmap */
  int mosaic;   /* attr 0 bit 12 */
  int bpp8;     /* attr 0 bit 13 (tile OBJs) */
  int hflip, vflip; /* non-affine OBJs */
  int affine_param; /* affine: attr 1 bits 9-13 */
  int tile;     /* attr 2 bits 0-9 */
  int prio;     /* attr 2 bits 10-11 */
  int palette;  /* attr 2 bits 12-15 (4bpp palette, ext palette number, or bitmap alpha) */
} VC_ObjAttr;

/* ---- the 3D engine ---- */
enum { VC_MAX_VERTS = 6144, VC_MAX_POLYS = 2048, VC_POLY_MAXV = 10 };

typedef struct VC_Mtx {
  s32 m[16]; /* 4x4 row-major, 20.12 */
} VC_Mtx;

/* A vertex in vertex RAM (GBATEK 33, 44). */
typedef struct VC_Vertex {
  s32 clip[4];  /* x, y, z, w after the clip matrix (piece 14) */
  u8 color[3];  /* r, g, b 0-31: the vertex colour in force (COLOR, or lighting's result) (piece 15) */
  s16 tex[2];   /* s, t 12.4 after texture coordinate generation (piece 15) */
  s32 sx, sy;   /* screen position in pixels after the viewport transform, x right, y down (piece 14) */
  s32 sz;       /* the 24-bit z-buffer depth (piece 14); for w-buffering the rasterizer uses clip[3] */
  u8 clipped;   /* made by clipping (an interpolated vertex) */
} VC_Vertex;

/* A polygon in polygon RAM (GBATEK 33, 43, 44). */
typedef struct VC_Poly {
  u16 v[VC_POLY_MAXV]; /* vertex RAM indices in drawing order, after clipping */
  u8 nv;               /* 3-10 */
  u8 front;            /* front-facing (the culling's verdict, piece 14) */
  u8 translucent;      /* drawn in the translucent pass (piece 18 sets it at SWAP_BUFFERS) */
  u8 type;             /* the BEGIN_VTXS primitive type: 0 tris, 1 quads, 2 tri strip, 3 quad strip */
  u32 attr;            /* POLYGON_ATTR in force (GBATEK 43) */
  u32 texparam;        /* TEXIMAGE_PARAM in force (GBATEK 47) */
  u32 pltt;            /* PLTT_BASE in force */
  u32 viewport;        /* the VIEWPORT in force (x1 | y1 << 8 | x2 << 16 | y2 << 24, y from the bottom): the GPU path's N x positions (G9) */
  s32 ymin, ymax;      /* the screen y extent (piece 14) */
  u16 index;           /* its number in the order sent */
} VC_Poly;

/* The geometry engine's output for the frame (filled by pieces 13-15 while the stream runs; 18 sorts it). */
typedef struct VC_GeomOut {
  VC_Vertex vtx[VC_MAX_VERTS];
  u32 nvtx;
  VC_Poly poly[VC_MAX_POLYS];
  u32 npoly;
  u16 order[VC_MAX_POLYS]; /* the drawing order (piece 18 at SWAP_BUFFERS; identity until then) */
  u32 swap_param;          /* SWAP_BUFFERS' parameter: bit 0 manual translucent sort, bit 1 w-buffering */
  u8 swapped;              /* a SWAP_BUFFERS was run */
  u8 overflow;             /* polygon or vertex RAM ran full */
} VC_GeomOut;

/* The rendering engine's framebuffer (pieces 16-19). */
enum {
  VC_F3_FOG = 1,       /* the pixel's polygon has the fog flag (POLYGON_ATTR bit 15) */
  VC_F3_EDGE = 2,      /* an edge pixel of an opaque polygon (edge marking, antialiasing) */
  VC_F3_TRANS = 4,     /* a translucent polygon wrote the colour */
  VC_F3_STENCIL = 8,   /* the shadow volume mask (shadow polygons ID 0) */
  VC_F3_WRITTEN = 16,  /* a polygon wrote the pixel (not the clear image) */
};
typedef struct VC_Frame3D {
  u32 color[VC_H][VC_W];  /* RGB6 | alpha << 24 */
  u32 depth[VC_H][VC_W];  /* 24-bit */
  u8 opaque_id[VC_H][VC_W]; /* polygon ID of the opaque pixel (or the clear ID) */
  u8 trans_id[VC_H][VC_W];  /* polygon ID of the last translucent polygon there, 0xFF none */
  u8 flags[VC_H][VC_W];     /* VC_F3_* */
  u8 coverage[VC_H][VC_W];  /* edge pixels' coverage 0-31 for antialiasing (piece 16 computes, 19 uses) */
  u32 out[VC_H][VC_W];      /* the 3D layer after edge marking, fog and antialiasing: RGB6 | alpha << 24 */
} VC_Frame3D;

/* the geometry command opcodes (GBATEK 36) */
enum {
  VC_OP_NOP = 0x00,
  VC_OP_MTX_MODE = 0x10, VC_OP_MTX_PUSH = 0x11, VC_OP_MTX_POP = 0x12, VC_OP_MTX_STORE = 0x13,
  VC_OP_MTX_RESTORE = 0x14, VC_OP_MTX_IDENTITY = 0x15, VC_OP_MTX_LOAD_4x4 = 0x16, VC_OP_MTX_LOAD_4x3 = 0x17,
  VC_OP_MTX_MULT_4x4 = 0x18, VC_OP_MTX_MULT_4x3 = 0x19, VC_OP_MTX_MULT_3x3 = 0x1A, VC_OP_MTX_SCALE = 0x1B,
  VC_OP_MTX_TRANS = 0x1C,
  VC_OP_COLOR = 0x20, VC_OP_NORMAL = 0x21, VC_OP_TEXCOORD = 0x22,
  VC_OP_VTX_16 = 0x23, VC_OP_VTX_10 = 0x24, VC_OP_VTX_XY = 0x25, VC_OP_VTX_XZ = 0x26, VC_OP_VTX_YZ = 0x27,
  VC_OP_VTX_DIFF = 0x28,
  VC_OP_POLYGON_ATTR = 0x29, VC_OP_TEXIMAGE_PARAM = 0x2A, VC_OP_PLTT_BASE = 0x2B,
  VC_OP_DIF_AMB = 0x30, VC_OP_SPE_EMI = 0x31, VC_OP_LIGHT_VECTOR = 0x32, VC_OP_LIGHT_COLOR = 0x33,
  VC_OP_SHININESS = 0x34,
  VC_OP_BEGIN_VTXS = 0x40, VC_OP_END_VTXS = 0x41,
  VC_OP_SWAP_BUFFERS = 0x50,
  VC_OP_VIEWPORT = 0x60,
  VC_OP_BOX_TEST = 0x70, VC_OP_POS_TEST = 0x71, VC_OP_VEC_TEST = 0x72,
};
/* the .vcs GX3D entry's flags (header word bits 16+) */
enum { VC_GXF_PROLOGUE = 1, VC_GXF_LIST = 2, VC_GXF_REG = 4 };

#endif
