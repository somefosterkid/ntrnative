/* vcore_interp: the midpoint of two consecutive captured frames (docs/interp.md, p23). Offline; not wired into the player.
 *   vcore_interp A.vcs B.vcs OUTPREFIX [--f 0.5] [--scale N] [--limit PX]
 * Writes OUTPREFIX_t.png (frame A), OUTPREFIX_mid.png (A's scene with its vertices blended toward B's by f, 2D held at A) and
 * OUTPREFIX_t1.png (frame B), and prints how the two frames' lists paired. */
#include "vc_shared.h"
#include "vcore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int shot(VCoreState *s, const char *path) {
  static uint8_t buf[256 * 384 * 4];
  int w = 0, h = 0;
  vcore_render(s);
  const uint8_t *pic = vcore_picture(s, &w, &h);
  if (pic == NULL) {
    vcore_frame_rgba(s, buf);
    pic = buf;
    w = 256;
    h = 384;
  }
  return vcore_write_png(path, pic, w, h);
}

int main(int argc, char **argv) {
  const char *a = NULL, *b = NULL, *pre = NULL;
  double f = 0.5, limit = 48.0;
  int scale = 1;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--f") && i + 1 < argc) f = atof(argv[++i]);
    else if (!strcmp(argv[i], "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--limit") && i + 1 < argc) limit = atof(argv[++i]);
    else if (!a) a = argv[i];
    else if (!b) b = argv[i];
    else if (!pre) pre = argv[i];
  }
  if (!a || !b || !pre) {
    fprintf(stderr, "usage: vcore_interp A.vcs B.vcs OUTPREFIX [--f 0.5] [--scale N] [--limit PX]\n");
    return 2;
  }
  VCoreState *sa = vcore_create(), *sb = vcore_create();
  char err[256], path[512];
  if (vcore_load_vcs(sa, a, err, sizeof err) || vcore_load_vcs(sb, b, err, sizeof err)) {
    fprintf(stderr, "vcore_interp: %s\n", err);
    return 1;
  }
  vcore_set_scale(sa, scale);
  vcore_set_scale(sb, scale);
  VC_IpRec ra, rb;
  memset(&ra, 0, sizeof ra);
  memset(&rb, 0, sizeof rb);
  vc_ip_record(sa);
  vc_ip_take(sa, &ra);
  vc_ip_record(sb);
  vc_ip_take(sb, &rb);
  int *map = (int *)malloc(sizeof(int) * (ra.nl + 1));
  VC_IpStats st;
  vc_ip_pair(&ra, &rb, map, limit, &st);
  printf("lists %d/%d vertices %u/%u: strict %d loose %d removed %d added %d jumps %d flips %d, paired vertices %u (%.1f%%), "
         "motion mean %.2f max %.1f px\n",
         st.nl_a, st.nl_b, st.nv_a, st.nv_b, st.strict, st.loose, st.removed, st.added, st.jumps, st.behind, st.vt_paired,
         100.0 * st.vt_paired / (st.nv_a ? st.nv_a : 1), st.disp_mean, st.disp_max);
  snprintf(path, sizeof path, "%s_t.png", pre);
  shot(sa, path);
  snprintf(path, sizeof path, "%s_t1.png", pre);
  shot(sb, path);
  vc_ip_apply_begin(sa, &rb, map, NULL, (s32)(f * 4096.0));
  snprintf(path, sizeof path, "%s_mid.png", pre);
  shot(sa, path);
  vc_ip_apply_end(sa);
  return 0;
}
