/* port/gpu piece G12 (texture cache, docs/gpu.md): the cache's directory, on the CPU (the bridge's worker). OWNED BY G12.
 * Pure CPU, no GL: g12_frame takes one frame's flattened texture data (gpu_texdata_bytes: 512 KB of texture slots, 96 KB of
 * palette slots) and its polygons, and says for each polygon where its texture is (a bucket and a layer of the five layer
 * arrays, 32-512 texels a side) or that it takes the exact path; plus the list of layers to upload (RGBA8, decoded here).
 *
 * Invalidation: a dirty bitmap of 512-byte chunks (the frame's bytes against the cache's copy of the last frame's), and per
 * entry the byte ranges its decode read (g12_tc_decode's G12Ranges). An entry used this frame whose ranges touch a dirty chunk
 * compares its palette bytes (an exact copy, <= 512 B) and the xxHash64 of its texel bytes (and of a 4x4 texture's block words
 * and wider palette range); only a real change re-decodes. An entry re-decoded in 8 of the last 16 frames is volatile for 120
 * frames (exact path, no decode, no upload). Layers are allocated per bucket (capacity doubles while the budget allows, else
 * the least recently used entry idle for 120 frames gives its layer up).
 * One caller at a time (g12_tc_fill's scratch): the bridge runs g12_frame under a mutex, in frame order. */
#ifndef GPU_G12_TEXCACHE_H
#define GPU_G12_TEXCACHE_H

#include "g12_tcdecode.h"
#include "vc_types.h"

enum {
  G12_NBUCKET = 5,     /* layer sides 32, 64, 128, 256, 512 */
  G12_CHUNK = 512,     /* the dirty bitmap's granule (bytes) */
  G12_TEX_CHUNKS = G12_TEX_BYTES / G12_CHUNK,
  G12_PAL_CHUNKS = G12_PAL_BYTES / G12_CHUNK,
  G12_PAL_COPY = 512,  /* palette ranges up to this are kept as a copy (compared exactly); wider ones are hashed */
  G12_VOLATILE_FRAMES = 120,
  G12_IDLE_FRAMES = 120, /* an entry unused this long may give its layer up when a bucket is full */
};

typedef struct G12Options {
  u32 budget_mb;     /* the layer arrays' memory (RGBA8, x 4/3 with mips); 0 = 128 */
  u32 decode_texels; /* layer texels decoded per frame at most (the first decode of a frame always runs); 0 = 16 layers of 128 */
  int mips;          /* the layers will hold mip levels: memory counted x 4/3, transparent texels' colour filled */
  int check;         /* TCCHECK: every entry the cache keeps as valid is decoded again and compared (hash of the image) */
  u32 max_layers;    /* layers per array at most (the GL side's GL_MAX_ARRAY_TEXTURE_LAYERS); 0 = 256, GLES 3.0's minimum */
} G12Options;

/* where polygon i (the drawing order position: geo->poly[geo->order[i]], as uPoly) takes its texels */
typedef struct G12Use {
  s16 bucket; /* 0-4, or -1: the exact path (untextured, not decoded yet, volatile, too large, no room) */
  u16 layer;
  u16 size;   /* the layer's side (32 << bucket), 0 on the exact path */
  u8 why;     /* G12_USE_* */
  u8 pad;
} G12Use;

enum { G12_USE_READY, G12_USE_NOTEX, G12_USE_PENDING, G12_USE_VOLATILE, G12_USE_BIG, G12_USE_NOROOM };

/* a layer to upload before the frame's 3D: bucket's layer, S x S RGBA8 at staging + offset (level 0), then with mips (levels > 1)
 * levels 1..levels-1 one after the other (g12_tc_mips's layout: S/2 x S/2 down to 1 x 1; G12_UP_BYTES in all) */
typedef struct G12Upload {
  u8 bucket;
  u8 levels; /* 1, or log2(S) + 1 with mips */
  u16 layer, size;
  u32 offset;
} G12Upload;
#define G12_UP_BYTES(u) ((size_t)(u).size * (u).size * 4 + ((u).levels > 1 ? ((size_t)(u).size * (u).size * 4 - 4) / 3 : 0))

typedef struct G12Stats {
  /* this frame */
  u32 polys, textured, textures;        /* polygons, textured ones, distinct textures (entries) they use */
  u32 by_size[G12_NBUCKET], big;        /* textures by layer side; textures over 512 (never cached) */
  u32 vol, pending, noroom, captured;      /* textures volatile, waiting for a decode, without a layer; inside captured pages */
  u32 hits, news, changed, kept_pal, kept_hash; /* used and valid; new entries; re-decoded; dirty but equal (palette, hash) */
  u32 decodes, evicted, dropped, grows;   /* decodes run (texels below); layers taken back; entries forgotten; buckets grown */
  u32 checked, stale;                     /* TCCHECK */
  u32 dirty_tex, dirty_pal;               /* dirty chunks */
  u64 decoded_texels, used_bytes;         /* layer texels decoded; the layer bytes of this frame's textures */
  /* the cache */
  u32 entries, layers[G12_NBUCKET], cap[G12_NBUCKET];
  u64 bytes;                              /* the arrays' memory (cap x layer bytes) */
} G12Stats;

typedef struct G12Frame {
  u32 frame;               /* the cache's frame count (1 = the first) */
  u32 npoly;
  const G12Use *use;       /* npoly entries */
  /* arrays to make before the uploads, with grow_cap[b] layers (0 = unchanged): a grown array's old layers are gone, its
   * entries come back through the upload list as the decode budget allows (their polygons take the exact path meanwhile) */
  u32 grow_cap[G12_NBUCKET];
  u32 nup;
  const G12Upload *up;
  const u8 *staging;       /* valid until the next g12_frame */
  G12Stats st;
} G12Frame;

typedef struct G12Cache G12Cache;

G12Cache *g12_create(const G12Options *o);
void g12_destroy(G12Cache *c);
/* forget every entry and layer (the next frame is a first frame) */
void g12_reset(G12Cache *c);
/* the 64-bit key of a texture: TEXIMAGE_PARAM without texcoord generation (bits 30-31), without bit 29 for the formats it does
 * nothing for (1, 5, 6, 7), with the repeat/flip bits; and PLTT_BASE (13 bits) for the palette formats; 0 for format 0 */
u64 g12_key(u32 texparam, u32 pltt);
/* one frame: tex/pal = the flattened texture data (G12_TEX_BYTES, G12_PAL_BYTES), geo = its polygons, cap_pages = the 16 KB
 * texture pages that hold a display capture (bit p: bytes p * 16K..; for the log, 0 when unknown). out stays valid until the
 * next call. */
void g12_frame(G12Cache *c, const u8 *tex, const u8 *pal, const VC_GeomOut *geo, u32 cap_pages, G12Frame *out);
/* TCLOG: out's numbers as one line (no newline) */
int g12_log_line(const G12Frame *f, char *buf, int n);
/* the xxHash64 of n bytes (the algorithm of its specification) */
u64 g12_xxh64(const void *p, size_t n, u64 seed);

#endif
