// port/gpu: the declarations every pass shares (docs/gpu.md, "The data"). OWNED BY THE SKELETON.
// The fragment shaders are: the API's header, this file, every other *.glsl by name (the pieces' functions), the
// pass's main (*.frag). GLSL ES 3.00 only (no extensions); integers where vcore is integer.

// G_K: multisampling, k x k samples per output pixel (1 = none; 2 = 4 samples). gpu_gl_program defines it from the program's
// name ("g00_raster.frag#2"); the passes are compiled once per k so the samples' state stays in registers (constant indices).
#ifndef G_K
#define G_K 1
#endif
const int G_S = G_K * G_K;

uniform highp usampler2D uPoly; // GpuPolyRec (src/g02_spans.h), 2 texels per polygon in drawing order
#ifdef GPU_SPAN_BUF
// the Metal core: pass S's kernel writes the records into a buffer pass R reads as it is (src/gpu_mtl.m)
layout(std430, binding = 1) readonly buffer GpuSpanIn { uvec4 uSpanBuf[]; };
#else
uniform highp usampler2D uSpan; // GpuSpan, 12 texels per record (polygon x row), float bits (src/g02_setup.h)
#endif
uniform highp usampler2D uBand; // per band of uBandRows rows: its first list entry, its count
uniform highp usampler2D uList; // the bins' polygon lists (drawing order), RGBA32UI: k, attr, first record - first row + bias, first row | last row << 16 (clipped to the band)
// Every non-opaque uniform of every program, one std140 block (src/gpu_uni.h's GpuU, the same fields in the same order: GL binds
// it as a uniform buffer, Metal as a buffer; SPIR-V has no loose uniforms). A pass sets the fields its program reads.
#ifdef GPU_SPIRV
layout(std140, binding = 0) uniform GpuU {
#else
layout(std140) uniform GpuU {
#endif
  uint uEdgeCol[8];  // pass P: EDGE_COLOR
  uint uFogTab[32];  // pass P: FOG_TABLE
  vec4 aMetrics;     // SMAA weights: SMAA_RT_METRICS, 1 / width, 1 / height, width, height
  int uScale;        // the grid the spans are on and the samples lie on: N k (k = G_K)
  int uSnap;         // k (= G_K): the look options act on the N grid
  int uBandRows;     // 8 N k
  uint uDisp3d;      // DISP3DCNT
  uint uSwap;        // SWAP_BUFFERS' parameter (bit 0 manual sort, bit 1 w-buffer)
  uint uClearColor;  // CLEAR_COLOR
  uint uClearDepth;  // CLEAR_DEPTH
  uint uAlphaRef;    // ALPHA_TEST_REF & 31
  int uTilesX;       // pass R, the bins: columns per band
  int uTileW;        // pass R: a bin's width in pixels of the N k grid
  uint uFlat;        // pass R, GPU_FLAT: every covered pixel opaque white, the edge flag and coverage kept (the span check)
  uint uFilter;      // G9: 1 = bilinear on the decoded texel grid at N > 1 (Resolution; uScale > uSnap) (g4_filtered); 0 = nearest (the DS)
  uint uClrOff;      // CLRIMAGE_OFFSET (g06_clear.glsl)
  int uStage;        // pass E's stage
  uint uFogColor;    // pass P: FOG_COLOR
  uint uFogOffset;   // pass P: FOG_OFFSET
  uint uEdgeFine;    // pass P: Edge=fine
  int cScale;        // pass C: N (the output)
  int cScale3d;      // pass C: the 3D texture's scale
  uint cHave3d;      // pass C: 1 when c3d is bound
  uint cSmaaOn;      // pass C: 1: the 3D layer is read through SMAA's neighbourhood blending (shaders/g08_smaa.glsl)
  uint cCompose;     // pass C: 1 when engine A shows the compositor's picture (mode 1, powered, not blanked)
  uint cATop;        // pass C: 1 when engine A is on the top screen
  uint cObjOn;       // pass C: DISPCNT bit 12
  uint cPrios;       // pass C: BGxCNT priorities, 2 bits each
  uint cBld;         // pass C: BLDCNT
  int cEva;          // pass C: the coefficients clamped to 16
  int cEvb;
  int cEvy;
  uint cBackdrop;    // pass C: palette entry 0 as RGB6
  int cHofs;         // pass C: BG0HOFS & 0x1FF
  uint cBright;      // pass C: MASTER_BRIGHT
  int uNFind;        // pass S: the find entries
  uint uZero;        // pass S: 0 (s_k, g02_setup.vert)
  int uFirst;        // pass S: the draw's first record (src/g02_setup.c draws in chunks on some drivers)
  int uPart;         // pass S's fragment form: the part
  int uN;            // pass S's fragment form and kernel: the records
  int uR;            // pass S's fragment form: a band's rows
  float aLuma;       // SMAA edges: the luma step an edge needs (0..1)
};

// the pixel flags (vcore's VC_F3_*)
const uint F_FOG = 1u, F_EDGE = 2u, F_TRANS = 4u, F_STENCIL = 8u, F_WRITTEN = 16u;
// the polygon table's flags (GPU_PF_*)
const uint PF_TRANS = 1u, PF_FRONT = 2u, PF_WBUF = 4u, PF_WIRE = 8u, PF_WHOLE = 16u;
// a record's row flags (GPU_ROW_*)
const uint ROW_ALLEDGE = 1u, ROW_WIRE = 2u, ROW_AA = 4u, ROW_VERTL = 8u, ROW_COV0 = 16u, ROW_DSGRID = 32u, ROW_MASK2 = 64u;

const int GPU_LIST_BIAS = 1048576; // the list's first-record word is first record - first row + this (src/g00_frame.c)
ivec2 g_at(int i) { return ivec2(i & 1023, i >> 10); }
// polygon k's texel t: t0 = attr, texparam, pltt, flags; t1 = first row, last row, first record, alpha
uvec4 g_poly(int k, int t) { return texelFetch(uPoly, g_at(k * 2 + t), 0); }
#ifdef GPU_SPAN_BUF
vec4 g_span(int rec, int t) { return uintBitsToFloat(uSpanBuf[rec * 12 + t]); }
#else
vec4 g_span(int rec, int t) { return uintBitsToFloat(texelFetch(uSpan, g_at(rec * 12 + t), 0)); }
#endif

// pass R's output, read by passes E and P: a 2D array of G_S layers at the output size N; sample (i, j) of pixel (x, y) is layer
// j k + i. k = 1: one RGBA32UI texel a pixel: depth, opaque ID | translucent ID << 8 | flags << 16 | coverage << 24,
// r6 | g6 << 8 | b6 << 16 | a5 << 24, the last polygon (debug, SMAA's detector). k = 2: one RG32UI texel a sample, what E and P
// read: depth | opaque ID << 24 (sample 0 only: | 1 << 30 when a sample of the pixel has the edge flag, | 1 << 31 when the four
// give pass P the same value without their neighbours: g00_raster.frag), r6 | g6 << 6 | b6 << 12 | a5 << 18 | F_FOG, F_EDGE,
// F_TRANS << 23 | F_WRITTEN << 26 | coverage << 27: half the bytes pass R stores and E and P fetch (the A18 Pro 2026-11, town at
// 3x: R -0.17 ms, E -0.26, P -0.07 at the passes rows' clock); the translucent ID, F_STENCIL and the last polygon stay in pass R.
// g_samp reads a sample by its place m on the N k grid (m >= 0), at k = 2 in k = 1's form (those three fields 0).
uniform highp usampler2DArray uSamp;
const int G_KS = G_K / 2; // log2 k (k = 1 or 2)
#if G_K == 2
uvec4 g_unpack(uvec2 t) {
  return uvec4(t.x & 0xFFFFFFu, ((t.x >> 24) & 63u) | (((t.y >> 23) & 7u) << 16) | (((t.y >> 26) & 1u) << 20) | ((t.y >> 27) << 24),
               (t.y & 63u) | (((t.y >> 6) & 63u) << 8) | (((t.y >> 12) & 63u) << 16) | (((t.y >> 18) & 31u) << 24), 0u);
}
uvec4 g_samp(ivec2 m) { return g_unpack(texelFetch(uSamp, ivec3(m >> G_KS, ((m.y & 1) << 1) + (m.x & 1)), 0).xy); }
#else
uvec4 g_samp(ivec2 m) { return texelFetch(uSamp, ivec3(m >> G_KS, ((m.y & (G_K - 1)) << G_KS) + (m.x & (G_K - 1))), 0); }
#endif
ivec2 g_samp_size() { return textureSize(uSamp, 0).xy * G_K; }

// the rendering engine's state of one pixel (vcore's VC_Frame3D at one x, y), kept in registers by pass R
struct GPixel {
  uvec4 color;    // r6, g6, b6, a5
  uint depth;     // 24 bits
  uint opaque_id; // 6 bits
  uint trans_id;  // 6 bits, 255 = none
  uint flags;     // F_*
  uint cov;       // the edge coverage 0-31 (antialiasing)
  uint last;      // the drawing-order index of the last polygon that changed the pixel (debug), 0xFFFFFFFF none
};

// one polygon's values at one pixel (G3's g_pixel)
struct GPx {
  int x, row;
  uvec3 rgb;  // the vertex colour, 6 bits
  ivec2 st;   // texture coordinates, 12.4, floored
  float w;    // perspective w
  uint depth; // z or w, 24 bits
  bool edge;  // an edge pixel (edge marking, antialiasing)
  uint cov;   // its coverage 0-31
};
