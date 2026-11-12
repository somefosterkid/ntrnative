/* port/vcore's unit-test framework (docs/vcore.md, "Unit tests"). OWNED BY THE SKELETON.
 *
 *   VCT_TEST(p01_map_size_64x32) {          // the name starts with the piece's pNN_
 *     VCoreState *s = vct_state();          // a zeroed state, freed after the test
 *     vct_eng16(s, VC_ENG_A, VC_REG_BG0CNT, 0x4004);
 *     ...
 *     VCT_EQ(s->line.bg[0][5], VC_PX_OPAQUE | 0x001F);
 *   }
 *
 * Tests register themselves (a constructor per test): a piece adds tests to its own tests/tNN_xxx.c, or adds more
 * .c files to tests/ (the build lists the directory: `meson setup --reconfigure build/vcore` after adding one).
 * A failed VCT_CHECK / VCT_EQ ends the test. VCT_TODO marks a test not written yet (not a failure).
 */
#ifndef VCT_H
#define VCT_H

#include "vc_shared.h"
#include "vcore.h"

#include <stdio.h>

typedef void (*vct_fn)(void);
void vct_register(const char *name, const char *file, vct_fn fn);
void vct_fail(const char *file, int line, const char *fmt, ...);
void vct_todo(const char *msg);
int vct_failed(void); /* the current test has failed */

#define VCT_TEST(name)                                                                                                \
  static void name(void);                                                                                             \
  __attribute__((constructor)) static void name##_vct_register(void) { vct_register(#name, __FILE__, name); }         \
  static void name(void)

#define VCT_CHECK(cond)                                                                                               \
  do {                                                                                                                \
    if (!(cond)) {                                                                                                    \
      vct_fail(__FILE__, __LINE__, "%s", #cond);                                                                      \
      return;                                                                                                         \
    }                                                                                                                 \
  } while (0)

#define VCT_EQ(a, b)                                                                                                  \
  do {                                                                                                                \
    long long vct_a_ = (long long)(a), vct_b_ = (long long)(b);                                                       \
    if (vct_a_ != vct_b_) {                                                                                           \
      vct_fail(__FILE__, __LINE__, "%s == %s: 0x%llx != 0x%llx", #a, #b, vct_a_, vct_b_);                             \
      return;                                                                                                         \
    }                                                                                                                 \
  } while (0)

#define VCT_TODO(msg)                                                                                                 \
  do {                                                                                                                \
    vct_todo(msg);                                                                                                    \
    return;                                                                                                           \
  } while (0)

/* ---- helpers ---- */
/* a zeroed VCoreState (no registers, no VRAM mapping); freed when the test ends */
VCoreState *vct_state(void);
/* register writes: io9 (engine A, 3D, VRAMCNT, POWCNT1) and an engine's 2D registers */
void vct_io8(VCoreState *s, u32 ofs, u8 v);
void vct_io16(VCoreState *s, u32 ofs, u16 v);
void vct_io32(VCoreState *s, u32 ofs, u32 v);
void vct_eng16(VCoreState *s, int e, u32 ofs, u16 v);
void vct_eng32(VCoreState *s, int e, u32 ofs, u32 v);
/* palette entry (obj 0 = BG, 1 = OBJ) */
void vct_pal(VCoreState *s, int e, int obj, int idx, u16 bgr555);
/* maps region r (VC_RGN_*) straight onto buf (size bytes, a multiple of 16 KB, or less for one page) without
 * piece 12: a piece's tests read VRAM through it */
void vct_map_flat(VCoreState *s, int r, const u8 *buf, u32 size);
/* appends a geometry command to s's 3D stream (as a .vcs GX3D entry) */
void vct_gx(VCoreState *s, u32 op, u32 nparam, const u32 *param);
/* writes s's input as a .vcs file (INFO, IO9, IO9B, VRAM, PLTT, OAM, GX3D). 0 on success */
int vct_write_vcs(const VCoreState *s, const char *path);
/* when VCT_EMIT=DIR is set: writes s's input as DIR/name.vcs, a synthetic frame for vcore_render, the oracle and
 * tools/vcore_compare.py corpus --corpus DIR (docs/vcore.md, "Done-check"); a no-op otherwise. Name it pNN_... */
void vct_emit(const VCoreState *s, const char *name);
/* a scratch file path for this run (under $TMPDIR or /tmp) */
const char *vct_tmp(const char *name);

#endif
