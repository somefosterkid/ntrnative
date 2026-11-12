/* port/vcore piece 6: affine and double-size OBJs (docs/vcore.md, piece 6). OWNED BY PIECE 6.
 * Follows GBATEK 15 (OBJ attributes), 16 (OBJ rotation/scaling parameters: the groups in the blank OAM halfwords,
 * the rotation centre in the middle of the OBJ, the double-size box) and 14; the matrix is the BG one of GBATEK 06
 * (x2 = A*(x1-x0) + B*(y1-y0) + x0 ...), PA-PD signed 8.8. */
#include "vc_shared.h"

/* PA, PB, PC, PD of parameter group g (0-31): the fourth halfword of OAM entries 4g .. 4g+3 (GBATEK 16) */
static void affine_params(const VCoreState *s, int e, int g, int p[4]) {
  const u8 *oam = vc_oam(s, e);
  for (int k = 0; k < 4; k++) {
    int ofs = (g * 4 + k) * 8 + 6;
    p[k] = (s16)(oam[ofs] | (oam[ofs + 1] << 8));
  }
}

/* the OBJ's top-left x (9 bits, signed) and y (8 bits) from the raw attributes (GBATEK 15: X 0-511, Y 0-255) */
static int obj_x(const VC_ObjAttr *o) {
  int x = o->attr[1] & 0x1FF;
  return x >= 256 ? x - 512 : x;
}
static int obj_y(const VC_ObjAttr *o) { return o->attr[0] & 0xFF; }

int vc_obj_affine_uv(const VCoreState *s, int e, const VC_ObjAttr *o, int x, int y, int *u, int *v) {
  int p[4];
  int bw = o->w << (o->dbl ? 1 : 0); /* the drawn box: doubled when double-size (the texture is not) */
  int bh = o->h << (o->dbl ? 1 : 0);
  /* position inside the box; x wraps at 512, y at 256 (GBATEK 15: a tall OBJ near y 255 wraps to the top) */
  int bx = (x - obj_x(o)) & 0x1FF;
  int by = (y - obj_y(o)) & 0xFF;
  *u = *v = 0;
  if (bx >= bw || by >= bh) return 0;
  affine_params(s, e, (o->attr[1] >> 9) & 31, p);
  /* the rotation centre is the middle of the box; the texture centre is the middle of the OBJ */
  int dx = bx - bw / 2, dy = by - bh / 2;
  int tu = ((p[0] * dx + p[1] * dy) >> 8) + o->w / 2;
  int tv = ((p[2] * dx + p[3] * dy) >> 8) + o->h / 2;
  if (tu < 0 || tu >= o->w || tv < 0 || tv >= o->h) return 0; /* outside the texture: transparent */
  *u = tu;
  *v = tv;
  return 1;
}

void vc_obj_affine_line(VCoreState *s, int e, const VC_ObjAttr *o, int y, VC_Line *ln) {
  int bw = o->w << (o->dbl ? 1 : 0);
  int bh = o->h << (o->dbl ? 1 : 0);
  int by = (y - obj_y(o)) & 0xFF;
  if (by >= bh) return;
  int x0 = obj_x(o);
  /* OBJ mosaic (oracle, probe t2 in docs/vcore.md p07): the row is the mosaic row (clamped to the box top); the
   * horizontal mosaic latches the texel at the first screen pixel of each block (x % n == 0) that lies inside the
   * texture, or at the first pixel inside the texture; the pixels in between repeat it, transparent or not. */
  int sy = y, n = 1, have = 0;
  u32 latch = 0;
  if (o->mosaic) {
    sy = vc_mosaic_obj_y(s, e, y);
    if (((sy - obj_y(o)) & 0xFF) >= bh) sy = obj_y(o);
    n = vc_mosaic_obj_hsize(s, e);
  }
  for (int i = 0; i < bw; i++) {
    int x = x0 + i;
    if (x < 0 || x >= VC_W) continue;
    int u, v;
    if (!vc_obj_affine_uv(s, e, o, x, sy, &u, &v)) continue;
    u32 px;
    if (o->mosaic && have && x % n != 0) {
      px = latch;
    } else {
      px = o->mode == 3 ? vc_obj_bitmap_texel(s, e, o, u, v) : vc_obj_tile_texel(s, e, o, u, v);
      latch = px;
      have = 1;
    }
    if (px) vc_obj_plot(s, e, o, x, px, ln);
  }
}
