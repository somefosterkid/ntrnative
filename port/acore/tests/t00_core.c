/* port/acore: the skeleton's own tests (layouts, memory, registers, the .acs loader, the time base, WAV). */
#include "act.h"
#include <stdlib.h>
#include <string.h>

ACT_TEST(t00_layouts) {
  ACT_EQ(sizeof(AC_ExChannel), 0x54);
  ACT_EQ(sizeof(AC_Player), 0x24);
  ACT_EQ(sizeof(AC_Track), 0x40);
  ACT_EQ(offsetof(AC_Work, alarm), 0xF80);
  ACT_EQ(AC_SW_PLAYER_TICKS(0), 0x40);
  ACT_EQ(AC_SW_GLOBAL_VAR(0), 0x260);
  ACT_EQ(AC_DRIVER_PERIOD, 174592);
}

ACT_TEST(t00_memory) {
  AcoreState *s = act_state_hw();
  ac_wr32(s, 0x02100000, 0x11223344);
  ACT_EQ(ac_rd32(s, 0x02100000), 0x11223344);
  ACT_EQ(ac_rd16(s, 0x02100002), 0x1122);
  ACT_EQ(ac_rd8(s, 0x02500000), ac_rd8(s, 0x02100000)); /* the 4 MB mirror */
  ACT_EQ(ac_rd32(s, 0x03000000), 0);
}

ACT_TEST(t00_registers) {
  AcoreState *s = act_state_hw();
  act_reg(s, 0x04000400 + AC_CH_CNT, 0xA840007F, 4);
  ACT_EQ(ac_ch_reg32(s, 0, AC_CH_CNT), 0xA840007F);
  ac_hw_set_busy(s, 0, 0);
  ACT_EQ(ac_ch_reg32(s, 0, AC_CH_CNT), 0x2840007F);
  act_reg(s, AC_REG_SOUNDBIAS, 0x200, 2);
  ACT_EQ(ac_hw_read(s, 0x04000504, 2), 0x200);
}

static void w32(u8 *p, u32 v) { p[0] = (u8)v, p[1] = (u8)(v >> 8), p[2] = (u8)(v >> 16), p[3] = (u8)(v >> 24); }

ACT_TEST(t00_load_and_time) {
  u8 f[256];
  size_t n = 0;
  memcpy(f, "ACST", 4), w32(f + 4, 1), n = 8;
  memcpy(f + n, "INFO", 4), w32(f + n + 4, 8), memcpy(f + n + 8, "tick0=10", 8), n += 16;
  memcpy(f + n, "MEM ", 4), w32(f + n + 4, 16), w32(f + n + 8, 100), w32(f + n + 12, 0), w32(f + n + 16, 0x02100000);
  w32(f + n + 20, 0xCAFEF00D), n += 24;
  memcpy(f + n, "CMDS", 4), w32(f + n + 4, 36), w32(f + n + 8, 200), w32(f + n + 12, 0), w32(f + n + 16, 0);
  w32(f + n + 20, 1), w32(f + n + 24, AC_CMD_MASTER_VOLUME), w32(f + n + 28, 0x40), w32(f + n + 32, 0);
  w32(f + n + 36, 0), w32(f + n + 40, 0), n += 44;
  memcpy(f + n, "WAKE", 4), w32(f + n + 4, 8), w32(f + n + 8, 200), w32(f + n + 12, 0), n += 16;
  memcpy(f + n, "END ", 4), w32(f + n + 4, 8), w32(f + n + 8, 400000), w32(f + n + 12, 0), n += 16;
  AcoreState *s = act_state();
  char err[128];
  ACT_EQ(acore_load_acs_mem(s, f, n, err, sizeof err), 0);
  ACT_EQ(s->nev, 4);
  ACT_EQ(s->drv.tick0, 10);
  acore_run(s, 150);
  ACT_EQ(ac_rd32(s, 0x02100000), 0xCAFEF00D);
  ACT_EQ(s->drv.nqueue, 0);
  acore_render_all(s);
  ACT_EQ(s->drv.nqueue, 0); /* piece 15 runs the list at the wake */
  ACT_EQ(s->io[0x500] & 0x7F, 0x40); /* MASTER_VOLUME reached SOUNDCNT */
  ACT_EQ(s->drv.wakes, 2 + 3);  /* the WAKE (two passes) and the periodic wakes at 10, 174602, 349194 */
  ACT_EQ(s->nsamples, 391);     /* samples at 0, 1024, ... 399360 */
}

ACT_TEST(t00_wav) {
  s16 pcm[8] = {1, -1, 0x7FFF, -0x8000, 5, 6, 7, 8};
  const char *p = act_tmp("t00.wav");
  ACT_EQ(acore_write_wav(p, pcm, 4, ACORE_WAV_RATE), 0);
  size_t n = 0;
  unsigned rate = 0;
  int16_t *back = acore_read_wav(p, &n, &rate);
  ACT_CHECK(back != NULL);
  ACT_EQ(n, 4);
  ACT_EQ(rate, ACORE_WAV_RATE);
  ACT_EQ(memcmp(back, pcm, sizeof pcm), 0);
  free(back);
  remove(p);
}
