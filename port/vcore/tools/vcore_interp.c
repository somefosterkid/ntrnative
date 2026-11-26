/* vcore_interp: the midpoint of two consecutive captured frames (docs/interp.md, p23). Offline; not wired into the player.
 *   vcore_interp A.vcs B.vcs OUTPREFIX [--f 0.5] [--scale N] [--prev P.vcs]
 * Writes OUTPREFIX_t.png (frame A), OUTPREFIX_mid.png (A's scene with its vertices blended toward B's by f, 2D held at A) and
 * OUTPREFIX_t1.png (frame B), and prints how the two frames' lists paired. The texcoord hold is the live one (vc_ip_texhold), and B's
 * lists with no partner are kept as live (vc_ip_keep; --prev: the keyframe before B, for their motion in the pair before). */
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
  const char *a = NULL, *b = NULL, *pre = NULL, *prev = NULL;
  double f = 0.5;
  const double limit = 48.0; /* (vc_ip_pair's limit: unused since M5) */
  int scale = 1;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--f") && i + 1 < argc) f = atof(argv[++i]);
    else if (!strcmp(argv[i], "--scale") && i + 1 < argc) scale = atoi(argv[++i]);
    else if (!strcmp(argv[i], "--prev") && i + 1 < argc) prev = argv[++i];
    else if (!a) a = argv[i];
    else if (!b) b = argv[i];
    else if (!pre) pre = argv[i];
  }
  if (!a || !b || !pre) {
    fprintf(stderr, "usage: vcore_interp A.vcs B.vcs OUTPREFIX [--f 0.5] [--scale N] [--prev P.vcs]\n");
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
  u8 *th = (u8 *)malloc(ra.nl + 1); /* the live texcoord hold (ip_common.h: 8 texels, 64 px) */
  vc_ip_texhold(&ra, &rb, map, th, 128, 64.0, NULL, NULL);
  VC_IpRec rp;
  memset(&rp, 0, sizeof rp);
  int *mp = NULL;
  if (prev != NULL) { /* B paired with the keyframe before it */
    VCoreState *sp = vcore_create();
    if (vcore_load_vcs(sp, prev, err, sizeof err)) {
      fprintf(stderr, "vcore_interp: %s\n", err);
      return 1;
    }
    vc_ip_record(sp);
    vc_ip_take(sp, &rp);
    mp = (int *)malloc(sizeof(int) * (rb.nl + 1));
    vc_ip_pair(&rb, &rp, mp, limit, &st);
  }
  VC_IpKeep *keep = (VC_IpKeep *)malloc(sizeof *keep * (rb.nl + 1));
  double enter[VC_IP_NENTER][16];
  VC_IpStats sa_st;
  vc_ip_pair(&ra, &rb, map, limit, &sa_st); /* (again: st was the pair before's) */
  int nkeep = vc_ip_windows(&ra, &rb, map, mp ? &rp : NULL, mp, &sa_st, keep, enter);
  if (vc_ip_state_on()) { /* M4 (as IpGather): the texture-bytes test, a texture state switching at the middle, alpha blended */
    vc_ip_texsig(sb, &rb);
    vc_ip_texsig(sa, &ra);
    vc_ip_texhold(&ra, &rb, map, th, 128, 64.0, NULL, NULL);
    nkeep = vc_ip_state(sa, &ra, &rb, map, th, keep, nkeep);
  }
  for (int k = 0; k < nkeep; k++) printf("kept %d after %d motion %.1f %.1f\n", keep[k].list, keep[k].after, keep[k].dx, keep[k].dy);
  vc_ip_apply_begin(sa, &rb, map, th, (s32)(f * 4096.0));
  vc_ip_apply_keep(sa, keep, nkeep);
  vc_ip_apply_enter(sa, (const double (*)[16])enter);
  snprintf(path, sizeof path, "%s_mid.png", pre);
  shot(sa, path);
  vc_ip_apply_end(sa);
  return 0;
}
