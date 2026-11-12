/* port/vcore: the skeleton's own tests (the loader, the outputs, the shared helpers). OWNED BY THE SKELETON. */
#include "vct.h"

#include <stdlib.h>

VCT_TEST(core_vcs_roundtrip) {
  VCoreState *s = vct_state();
  vct_io32(s, VC_REG_DISPCNT, 0x00010100);
  vct_eng16(s, VC_ENG_B, VC_REG_BG0CNT, 0x1234);
  vc_bank(s, VC_BANK_I)[0x3FFF] = 0xAB;
  vct_pal(s, VC_ENG_B, 1, 255, 0x7C1F);
  s->oam[0x7FF] = 0x5A;
  u32 p[2] = {0x11112222, 0x3333};
  vct_gx(s, VC_OP_VTX_16, 2, p);
  vct_gx(s, VC_OP_END_VTXS, 0, NULL);
  const char *path = vct_tmp("roundtrip.vcs");
  VCT_EQ(vct_write_vcs(s, path), 0);
  VCoreState *t = vct_state();
  char err[128];
  VCT_EQ(vcore_load_vcs(t, path, err, sizeof err), 0);
  VCT_EQ(vc_io32(t, VC_REG_DISPCNT), 0x00010100);
  VCT_EQ(vc_eng16(t, VC_ENG_B, VC_REG_BG0CNT), 0x1234);
  VCT_EQ(vc_bank(t, VC_BANK_I)[0x3FFF], 0xAB);
  VCT_EQ(vc_pal(t, VC_ENG_B, 1, 255), 0x7C1F);
  VCT_EQ(t->oam[0x7FF], 0x5A);
  VCT_EQ(t->gx_words, 4);
  VCT_EQ(t->gx[0], VC_OP_VTX_16 | 2 << 8);
  VCT_EQ(t->gx[2], 0x3333);
  VCT_EQ(t->gx[3], VC_OP_END_VTXS);
  char buf[64];
  VCT_CHECK(vc_info(t, "source", buf, sizeof buf) != NULL);
  VCT_CHECK(strcmp(buf, "vct") == 0);
  remove(path);
}

VCT_TEST(core_vcs_rejects_garbage) {
  VCoreState *s = vct_state();
  char err[128];
  VCT_CHECK(vcore_load_vcs_mem(s, "NOPE....", 8, err, sizeof err) != 0);
  static const u8 trunc[16] = {'V', 'C', 'S', 'T', 1, 0, 0, 0, 'I', 'O', '9', ' ', 0xFF, 0xFF, 0, 0};
  VCT_CHECK(vcore_load_vcs_mem(s, trunc, sizeof trunc, err, sizeof err) != 0);
}

VCT_TEST(core_bank_layout) {
  u32 at = 0;
  for (int b = 0; b < VC_NBANKS; b++) {
    VCT_EQ(vc_bank_offset[b], at);
    at += vc_bank_size[b];
  }
  VCT_EQ(at, VC_VRAM_SIZE);
}

VCT_TEST(core_region_or_and_wrap) {
  VCoreState *s = vct_state();
  static u8 a[VC_PAGE_SIZE], b[VC_PAGE_SIZE];
  a[2] = 0x0F;
  b[2] = 0x30;
  b[3] = 0x81;
  VC_Region *r = &s->rgn[VC_RGN_BBG];
  r->npages = 8;
  r->page[1].src[0] = a;
  r->page[1].src[1] = b;
  r->page[1].n = 2;
  VCT_EQ(vc_rd8(r, 0x4002), 0x3F);
  VCT_EQ(vc_rd16(r, 0x4002), 0x813F);
  VCT_EQ(vc_rd16(r, 0x4003), 0x813F);        /* 16-bit reads are aligned */
  VCT_EQ(vc_rd8(r, 0x4002 + 8 * VC_PAGE_SIZE), 0x3F); /* wraps at 128 KB */
  VCT_EQ(vc_rd8(r, 0x0002), 0);              /* page 0 unmapped */
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_ABG], 0), 0); /* an empty region */
}

VCT_TEST(core_colour_conversion) {
  VCT_EQ(vc_555_to_rgb6(0x7FFF), VC_RGB6(62, 62, 62));
  VCT_EQ(vc_555_to_rgb6(0x001F), VC_RGB6(62, 0, 0));
  VCoreState *s = vct_state();
  s->screen[1][0][0] = VC_RGB6(63, 32, 1);
  u8 *rgba = (u8 *)malloc(256 * 192 * 4);
  vcore_screen_rgba(s, 1, rgba);
  VCT_EQ(rgba[0], 255);
  VCT_EQ(rgba[1], 130);
  VCT_EQ(rgba[2], 4);
  VCT_EQ(rgba[3], 255);
  free(rgba);
}

VCT_TEST(core_png_writer) {
  u8 px[4 * 3 * 2];
  for (int i = 0; i < (int)sizeof px; i++) px[i] = (u8)(i * 7);
  const char *path = vct_tmp("t.png");
  VCT_EQ(vcore_write_png(path, px, 3, 2), 0);
  FILE *f = fopen(path, "rb");
  VCT_CHECK(f != NULL);
  u8 head[24];
  size_t n = fread(head, 1, sizeof head, f);
  fclose(f);
  remove(path);
  VCT_EQ(n, 24);
  VCT_CHECK(memcmp(head, "\x89PNG\r\n\x1a\n", 8) == 0);
  VCT_CHECK(memcmp(head + 12, "IHDR", 4) == 0);
  VCT_EQ(head[19], 3); /* width */
  VCT_EQ(head[23], 2); /* height */
}

VCT_TEST(core_blank_state_renders_white) {
  VCoreState *s = vct_state();
  vcore_render(s);
  const u32 *top = vcore_screen(s, 0), *bot = vcore_screen(s, 1);
  /* the blank state is display mode 0 on both engines: white, whatever POWCNT1 says (q10's oracle table, p10) */
  u32 ta = 0, bb = 0;
  for (int i = 0; i < VC_W * VC_H; i++) { ta |= top[i]; bb |= bot[i]; }
  VCT_EQ(ta, 0x3F3F3F);
  VCT_EQ(bb, 0x3F3F3F);
}

VCT_TEST(core_mtx_apply) {
  VC_Mtx m;
  memset(&m, 0, sizeof m);
  m.m[0] = m.m[5] = m.m[10] = m.m[15] = 0x1000;
  m.m[12] = 0x3000; /* translate x by 3 */
  m.m[5] = 0x2000;  /* scale y by 2 */
  s32 in[4] = {0x1000, 0x1000, -0x1000, 0x1000}, out[4];
  vc_mtx_apply(&m, in, out);
  VCT_EQ(out[0], 0x4000);
  VCT_EQ(out[1], 0x2000);
  VCT_EQ(out[2], -0x1000);
  VCT_EQ(out[3], 0x1000);
}

VCT_TEST(core_gx_stream_dispatch) {
  /* a stream with a SWAP_BUFFERS runs to the end without crashing on the stubs */
  VCoreState *s = vct_state();
  u32 p[16] = {0};
  vct_gx(s, VC_OP_MTX_MODE, 1, p);
  vct_gx(s, VC_OP_MTX_LOAD_4x4, 16, p);
  vct_gx(s, VC_OP_BEGIN_VTXS, 1, p);
  vct_gx(s, VC_OP_VTX_16, 2, p);
  vct_gx(s, VC_OP_SWAP_BUFFERS, 1, p);
  s->gx[s->gx_words - 2] = VC_OP_SWAP_BUFFERS | 9 << 8; /* a damaged count: clipped to the stream's end */
  vcore_render(s);
  VCT_CHECK(1);
}
