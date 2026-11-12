/* port/vcore: the state and the functions the pieces call across (docs/vcore.md). OWNED BY THE SKELETON: a piece
 * never edits this file. Each function says which piece owns (implements) it; its body lives in that piece's file.
 * A piece's own data is its VC_Pnn struct (its own header pNN_*.h), reached as s->pNN.
 */
#ifndef VC_SHARED_H
#define VC_SHARED_H

#include "vc_types.h"

#include "p01_bg_text.h"
#include "p02_bg_affine.h"
#include "p03_bg_ext.h"
#include "p04_bg_large_mosaic.h"
#include "p05_obj.h"
#include "p06_obj_affine.h"
#include "p07_obj_bitmap.h"
#include "p08_window.h"
#include "p09_blend.h"
#include "p10_display.h"
#include "p11_capture.h"
#include "p12_vram.h"
#include "p13_matrix.h"
#include "p14_polygon.h"
#include "p15_light.h"
#include "p16_raster.h"
#include "p17_texture.h"
#include "p18_translucent.h"
#include "p19_post.h"
#include "p20_frame.h"
#include "p21_scale.h"
#include "p23_interp.h"

typedef struct VCoreState VCoreState;

/* the 3D pixel write piece 16 calls for each covered pixel (default vc_3d_plot, piece 18; tests may replace it) */
typedef void (*VC_PlotFn)(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 rgb6a5, int edge);

struct VCoreState {
  /* ---- the input (vc_state.c loads it from a .vcs file) ---- */
  u8 io9[VC_IO9_SIZE];   /* 0x04000000-0x040006FF: engine A, the 3D registers, VRAMCNT, POWCNT1 */
  u8 io9b[VC_IO9B_SIZE]; /* 0x04001000-0x0400106F: engine B */
  u8 vram[VC_VRAM_SIZE]; /* banks A-I (vc_bank_offset) */
  u8 pltt[0x800];        /* A BG 0x000, A OBJ 0x200, B BG 0x400, B OBJ 0x600 (BGR555) */
  u8 oam[0x800];         /* A 0x000, B 0x400 */
  u32 *gx;               /* the frame's 3D command stream: per entry a header op | n << 8 | flags << 16, n params */
  u32 gx_words;
  char *info;            /* the INFO chunk's text (key=value lines), NUL-terminated */
  u8 *lsht;              /* libntr's picture of the frame (256x384 RGBA8) or NULL */
  u8 *l3d;               /* libntr's 3D layer (256x192 RGBA8) or NULL */

  /* ---- the working state ---- */
  VC_Region rgn[VC_NREGIONS]; /* the VRAM views (piece 12, vc_vram_map) */
  VC_GeomOut geo;             /* the geometry engine's output (pieces 13-15, sorted by 18) */
  VC_Frame3D f3d;             /* the 3D framebuffer and layer (pieces 16-19) */
  VC_PlotFn plot;             /* = vc_3d_plot */
  VC_Line line;               /* the line being drawn */

  /* ---- the output ---- */
  u32 eng_out[2][VC_H][VC_W]; /* each engine's final picture (RGB6) */
  u32 screen[2][VC_H][VC_W];  /* top, bottom (RGB6) */

  /* ---- the pieces' own state ---- */
  VC_P01 p01; VC_P02 p02; VC_P03 p03; VC_P04 p04; VC_P05 p05;
  VC_P06 p06; VC_P07 p07; VC_P08 p08; VC_P09 p09; VC_P10 p10;
  VC_P11 p11; VC_P12 p12; VC_P13 p13; VC_P14 p14; VC_P15 p15;
  VC_P16 p16; VC_P17 p17; VC_P18 p18; VC_P19 p19; VC_P20 p20;
  VC_P21 p21;
  VC_P23 p23; /* in-between frames (research, off by default) */
};

/* ---- register and memory access (skeleton, inline) ---- */
static inline u16 vc_io16(const VCoreState *s, u32 ofs) { return (u16)(s->io9[ofs] | s->io9[ofs + 1] << 8); }
static inline u32 vc_io32(const VCoreState *s, u32 ofs) { return vc_io16(s, ofs) | (u32)vc_io16(s, ofs + 2) << 16; }
static inline u8 vc_io8(const VCoreState *s, u32 ofs) { return s->io9[ofs]; }
/* an engine's 2D register, ofs < 0x70 (engine B's from io9b) */
static inline u16 vc_eng16(const VCoreState *s, int e, u32 ofs) {
  const u8 *io = e ? s->io9b : s->io9;
  return (u16)(io[ofs] | io[ofs + 1] << 8);
}
static inline u32 vc_eng32(const VCoreState *s, int e, u32 ofs) {
  return vc_eng16(s, e, ofs) | (u32)vc_eng16(s, e, ofs + 2) << 16;
}
/* palette RAM: engine e, obj 0 = BG / 1 = OBJ, colour index 0-255 */
static inline u16 vc_pal(const VCoreState *s, int e, int obj, int idx) {
  u32 a = (u32)e * 0x400 + (u32)obj * 0x200 + (u32)(idx & 0xFF) * 2;
  return (u16)(s->pltt[a] | s->pltt[a + 1] << 8);
}
static inline const u8 *vc_oam(const VCoreState *s, int e) { return s->oam + e * 0x400; }
static inline u8 *vc_bank(VCoreState *s, int bank) { return s->vram + vc_bank_offset[bank]; }

/* VRAM through a region (piece 12 builds the regions; the OR of the banks mapped on a page) */
static inline u8 vc_rd8(const VC_Region *r, u32 a) {
  if (r->npages == 0) return 0;
  const VC_Page *p = &r->page[(a >> VC_PAGE_SHIFT) % r->npages];
  u32 o = a & (VC_PAGE_SIZE - 1);
  u8 v = 0;
  for (int i = 0; i < p->n; i++) v |= p->src[i][o];
  return v;
}
static inline u16 vc_rd16(const VC_Region *r, u32 a) {
  a &= ~1u;
  return (u16)(vc_rd8(r, a) | vc_rd8(r, a + 1) << 8);
}
static inline u32 vc_rd32(const VC_Region *r, u32 a) {
  a &= ~3u;
  return vc_rd16(r, a) | (u32)vc_rd16(r, a + 2) << 16;
}
/* the engine's BG / OBJ / ext palette regions */
static inline const VC_Region *vc_rgn_bg(const VCoreState *s, int e) { return &s->rgn[e ? VC_RGN_BBG : VC_RGN_ABG]; }
static inline const VC_Region *vc_rgn_obj(const VCoreState *s, int e) { return &s->rgn[e ? VC_RGN_BOBJ : VC_RGN_AOBJ]; }
static inline const VC_Region *vc_rgn_bgext(const VCoreState *s, int e) {
  return &s->rgn[e ? VC_RGN_BBGEXT : VC_RGN_ABGEXT];
}
static inline const VC_Region *vc_rgn_objext(const VCoreState *s, int e) {
  return &s->rgn[e ? VC_RGN_BOBJEXT : VC_RGN_AOBJEXT];
}

/* ======== the pieces' functions (each implemented in the piece's file) ======== */

/* ---- (1) text BGs: p01_bg_text.c ---- */
/* BG n of engine e as a text BG: line y (vertical mosaic already applied) into ln->bg[n] */
void vc_bg_text_line(VCoreState *s, int e, int n, int y, VC_Line *ln);

/* ---- (2) affine BGs: p02_bg_affine.c ---- */
/* BG n (2 or 3) as an affine (rotation/scaling) BG with 8-bit map entries */
void vc_bg_affine_line(VCoreState *s, int e, int n, int y, VC_Line *ln);
/* the affine BG's texel position (24.8 fixed, BG pixels << 8) at screen pixel x of line y: shared with 3 and 4 */
void vc_bg_affine_pos(const VCoreState *s, int e, int n, int x, int y, s32 *tx, s32 *ty);

/* ---- (3) extended BGs: p03_bg_ext.c ---- */
/* BG n (2 or 3) as an extended BG: affine 16-bit map entries, 256-colour or direct-colour bitmap (BGxCNT bits 7, 2) */
void vc_bg_ext_line(VCoreState *s, int e, int n, int y, VC_Line *ln);

/* ---- (4) the large bitmap + BG mosaic: p04_bg_large_mosaic.c ---- */
/* BG2 in BG mode 6: the 512x1024 / 1024x512 256-colour bitmap */
void vc_bg_large_line(VCoreState *s, int e, int y, VC_Line *ln);
/* the line BG n fetches on screen line y (vertical BG mosaic; y when BGxCNT bit 6 is off) */
int vc_mosaic_bg_y(const VCoreState *s, int e, int n, int y);
/* the horizontal BG mosaic over one BG's line (a no-op when BGxCNT bit 6 is off) */
void vc_mosaic_bg_line(const VCoreState *s, int e, int n, u32 *px);

/* ---- (5) normal OBJs, the OAM loop: p05_obj.c ---- */
/* OAM entry i of engine e decoded */
void vc_obj_decode(const VCoreState *s, int e, int i, VC_ObjAttr *o);
/* all OBJs of line y into ln->obj[] and ln->objwin[] (normal OBJs here; affine ones by 6; bitmap texels by 7) */
void vc_obj_line(VCoreState *s, int e, int y, VC_Line *ln);
/* a tile OBJ's texel (u, v in the OBJ's own pixels, 0..w-1 / 0..h-1): a VC_PX_* pixel without flags, 0 = clear */
u32 vc_obj_tile_texel(const VCoreState *s, int e, const VC_ObjAttr *o, int u, int v);
/* puts an OBJ's pixel at x of the line under the OBJ rules (priority, OBJ order, the window OBJs to 7) */
void vc_obj_plot(VCoreState *s, int e, const VC_ObjAttr *o, int x, u32 px, VC_Line *ln);

/* ---- (6) affine and double-size OBJs: p06_obj_affine.c ---- */
/* the texel (u, v) an affine OBJ shows at screen (x, y); 0 = outside the OBJ */
int vc_obj_affine_uv(const VCoreState *s, int e, const VC_ObjAttr *o, int x, int y, int *u, int *v);
/* draws affine OBJ o's part of line y (through vc_obj_tile_texel / vc_obj_bitmap_texel and vc_obj_plot) */
void vc_obj_affine_line(VCoreState *s, int e, const VC_ObjAttr *o, int y, VC_Line *ln);

/* ---- (7) bitmap OBJs, the OBJ window, OBJ mosaic: p07_obj_bitmap.c ---- */
/* a bitmap OBJ's (mode 3) texel: VC_PX_OPAQUE | VC_PX_BMPOBJ | alpha | colour, or 0 */
u32 vc_obj_bitmap_texel(const VCoreState *s, int e, const VC_ObjAttr *o, int u, int v);
/* an OBJ-window OBJ's opaque pixel at x (called by vc_obj_plot for mode 2) */
void vc_obj_window_plot(VCoreState *s, int e, const VC_ObjAttr *o, int x, VC_Line *ln);
/* the OBJ line a mosaic OBJ fetches for screen line y (vertical OBJ mosaic) */
int vc_mosaic_obj_y(const VCoreState *s, int e, int y);
/* the OBJ-relative column a mosaic OBJ shows at screen x (OBJ left edge ox) */
/* MOSAIC's OBJ H size (1-16) */
int vc_mosaic_obj_hsize(const VCoreState *s, int e);
int vc_mosaic_obj_x(const VCoreState *s, int e, int x, int ox);
/* the horizontal OBJ mosaic over the OBJ line (after all OBJs are drawn) */
void vc_mosaic_obj_line(const VCoreState *s, int e, VC_Line *ln);

/* ---- (8) windows: p08_window.c ---- */
/* ln->winmask[] for line y from WIN0/WIN1 (H, V), the OBJ window (ln->objwin) and WININ/WINOUT */
void vc_window_line(VCoreState *s, int e, int y, VC_Line *ln);

/* ---- (9) the compositor and the colour effects: p09_blend.c ---- */
/* the line's layers in priority order with the window mask, blending, brighten/darken, the 3D layer's alpha:
 * ln->out[] (RGB6) */
void vc_compose_line(VCoreState *s, int e, int y, VC_Line *ln);

/* ---- (10) master brightness and the display modes: p10_display.c ---- */
/* engine e's final line y into dst (RGB6): POWCNT1's engine enable, forced blank, DISPCNT display mode 0-3
 * (off / the graphics ln->out / a VRAM bank / main memory), then MASTER_BRIGHT */
void vc_display_line(VCoreState *s, int e, int y, const VC_Line *ln, u32 *dst);

/* ---- (11) display capture: p11_capture.c ---- */
/* engine A, line y: when DISPCAPCNT bit 31 is set, captures source A (ln->out, or the 3D layer) and/or source B
 * (a VRAM bank, or main memory) blended into the destination bank */
void vc_capture_line(VCoreState *s, int y, const VC_Line *ln);
/* after the frame: the capture finished (clears DISPCAPCNT bit 31 in io9 as the hardware does) */
void vc_capture_end(VCoreState *s);

/* ---- (12) VRAM mapping: p12_vram.c ---- */
/* builds s->rgn[] from VRAMCNT_A-I (io9 0x240-0x249) */
void vc_vram_map(VCoreState *s);
/* bank A-D (VC_BANK_A + DISPCNT bits 18-19 / DISPCAPCNT's bank fields) as display mode 2 and capture use it: its
 * memory when VRAMCNT has it enabled in LCDC mode (MST 0), else NULL */
u8 *vc_vram_lcdc(VCoreState *s, int bank);

/* ---- (13) the matrix engine and the command processor: p13_matrix.c ---- */
void vc_mtx_reset(VCoreState *s);
/* runs one geometry command (dispatches to 13/14/15/18 by opcode) */
void vc_gx_command(VCoreState *s, u32 op, const u32 *param, u32 nparam);
/* the matrix commands 0x10-0x1C */
void vc_cmd_mtx(VCoreState *s, u32 op, const u32 *param, u32 nparam);
/* the current matrices (valid until the next matrix command) */
const VC_Mtx *vc_mtx_clip(VCoreState *s); /* position x projection */
const VC_Mtx *vc_mtx_pos(VCoreState *s);
const VC_Mtx *vc_mtx_vec(VCoreState *s);
const VC_Mtx *vc_mtx_tex(VCoreState *s);
const VC_Mtx *vc_mtx_proj(VCoreState *s);
/* v' = v * m, 20.12: out[4] from in[4] (helper for 14 and 15) */
void vc_mtx_apply(const VC_Mtx *m, const s32 in[4], s32 out[4]);

/* ---- (14) vertices, polygon assembly, clipping, viewport: p14_polygon.c ---- */
void vc_poly_reset(VCoreState *s);
/* VTX_16..VTX_DIFF */
void vc_cmd_vtx(VCoreState *s, u32 op, const u32 *param, u32 nparam);
/* POLYGON_ATTR, TEXIMAGE_PARAM, PLTT_BASE, BEGIN_VTXS, END_VTXS, VIEWPORT, BOX_TEST, POS_TEST */
void vc_cmd_poly(VCoreState *s, u32 op, const u32 *param, u32 nparam);

/* ---- (15) lighting, materials, normals, texture coordinates: p15_light.c ---- */
void vc_light_reset(VCoreState *s);
/* COLOR, NORMAL, TEXCOORD, DIF_AMB, SPE_EMI, LIGHT_VECTOR, LIGHT_COLOR, SHININESS, VEC_TEST */
void vc_cmd_light(VCoreState *s, u32 op, const u32 *param, u32 nparam);
/* the colour and texture coordinates of a new vertex at object-space position pos (20.12, w = 1) */
void vc_vertex_attrs(VCoreState *s, const s32 pos[4], VC_Vertex *out);

/* ---- (16) the rasterizer: p16_raster.c ---- */
/* scan-converts polygon p (fill rules, interpolation, depth, 1-dot polygons), calling s->plot per pixel */
void vc_raster_poly(VCoreState *s, const VC_Poly *p);
/* the depth test of polygon p's pixel (less, or equal with POLYGON_ATTR bit 14): 1 = passes */
int vc_3d_depth_test(const VCoreState *s, const VC_Poly *p, int x, int y, u32 depth);

/* ---- (17) textures: p17_texture.c ---- */
/* the texel of polygon p's texture at (s, t) 12.4: RGB6 | alpha << 24 (alpha 0 = transparent); all 7 formats,
 * palettes, repeat / flip / clamp */
u32 vc_tex_sample(const VCoreState *s, const VC_Poly *p, s32 ts, s32 tt);
/* the pixel colour from the vertex colour (RGB6, alpha 0-31) and the texel: modulation, decal; toon / highlight
 * through vc_toon_blend (19); DISP3DCNT bit 0 off = the vertex colour */
u32 vc_tex_blend(const VCoreState *s, const VC_Poly *p, u32 vcolor, u32 texel);

/* ---- (18) translucency, sorting, polygon IDs, shadows: p18_translucent.c ---- */
/* SWAP_BUFFERS: marks translucent polygons and builds s->geo.order */
void vc_cmd_swap(VCoreState *s, const u32 *param, u32 nparam);
/* the framebuffer write of one pixel (the default s->plot): depth test, alpha test, opaque / translucent / shadow
 * rules, polygon IDs, the depth update */
void vc_3d_plot(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 rgb6a5, int edge);

/* ---- (19) clear image, edge marking, fog, toon / highlight, antialiasing: p19_post.c ---- */
/* the framebuffer before the polygons: CLEAR_COLOR / CLEAR_DEPTH, or the rear-plane bitmap (DISP3DCNT bit 14) */
void vc_3d_clear(VCoreState *s);
/* toon or highlight shading (POLYGON_ATTR mode 2, DISP3DCNT bit 1) of a pixel; called by vc_tex_blend */
u32 vc_toon_blend(const VCoreState *s, const VC_Poly *p, u32 vcolor, u32 texel);
/* after all polygons: edge marking, fog, antialiasing into s->f3d.out */
void vc_3d_post(VCoreState *s);

/* ---- (20) integration: p20_frame.c ---- */
/* the whole frame: the 3D (if engine A shows it), then each engine line by line, then the screens */
void vc_render_frame(VCoreState *s);
/* runs the .vcs command stream through the geometry engine and renders the 3D framebuffer */
void vc_3d_frame(VCoreState *s);
/* one line of engine e into s->eng_out[e][y] */
void vc_render_line(VCoreState *s, int e, int y);
/* BG n of line y: the BG mode table -> 1/2/3/4 or the 3D layer, with the mosaic */
void vc_bg_line(VCoreState *s, int e, int n, int y, VC_Line *ln);
/* the 3D layer's line y into ln->l3d (BG0HOFS scrolls it) */
void vc_3d_layer_line(VCoreState *s, int y, VC_Line *ln);

/* the N x frame (p21_scale.c, driven by p20): vcore_render with s->p21.scale > 1 */
void vc_render_frame_n(VCoreState *s);

/* ---- (24) the same 2D: p24_same.c ---- */
/* 1 when engine e's 1x 2D picture in state b would be the same as in state a: every input it reads is equal. a's regions
 * are mapped (vc_vram_map); b's need not be */
int vc_2d_same(const VCoreState *a, const VCoreState *b, int e);

/* ---- the skeleton (vc_state.c) ---- */
/* resets the working state for a new render (regions, geometry, framebuffers, outputs; keeps the input) */
void vc_reset_work(VCoreState *s);
/* s->geo alone cleared, as vc_reset_work leaves it (the geometry engine runs again over the same stream) */
void vc_reset_geo(VCoreState *s);
/* the INFO chunk's value for key, or NULL */
const char *vc_info(const VCoreState *s, const char *key, char *buf, size_t len);

#endif
