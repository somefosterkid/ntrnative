/* port/vcore piece 20: integration: the frame, the 3D layer, the screens (docs/vcore.md, piece 20). OWNED BY PIECE 20.
 * Spec: GBATEK under <ref>/gbatek/ (the sections docs/vcore.md lists); the stubs below draw nothing. */
#define _POSIX_C_SOURCE 200809L
#include "vc_shared.h"
#include "vcore.h"

#include <pthread.h>
#include <stdlib.h>
#include <time.h>

/* stage timers for vcore_render --bench (nanoseconds of the running thread's CPU time, summed over frames) */
double vc_stage_ns[VC_NSTAGE];
double vc_nwall_ns[VC_NWALL]; /* the N x frame's stages in wall-clock time (vcore_render --bench --scale N) */
int vc_threads = 1; /* 0 = the sequential frame (vcore_render --seq, for timing it against the threaded one) */
static double now_ns(void) {
  struct timespec ts;
#if defined(_WIN32) && defined(CLOCK_MONOTONIC)
  /* Windows (winpthreads): the thread CPU clock ticks in 15.6 ms steps (the timing readout showed 0 or 15.6): wall time */
  clock_gettime(CLOCK_MONOTONIC, &ts);
#elif defined(CLOCK_THREAD_CPUTIME_ID)
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts); /* this thread's CPU time: steady on a loaded machine */
#else
  timespec_get(&ts, TIME_UTC);
#endif
  return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}
static double wall_ns(void) { /* wall time; with the threads off (--seq) this thread's CPU time, which a loaded machine leaves alone */
#ifdef CLOCK_MONOTONIC
  struct timespec ts;
  if (!vc_threads) return now_ns();
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
#else
  return now_ns();
#endif
}
#define WALL(k, stmt) do { double t0_ = wall_ns(); stmt; vc_nwall_ns[k] += wall_ns() - t0_; } while (0)
#define STAGE(k, stmt) do { double t0_ = now_ns(); stmt; vc_stage_ns[k] += now_ns() - t0_; } while (0)

static void vc_render_line_ln(VCoreState *s, int e, int y, VC_Line *ln);
static void vc_render_line_a1(VCoreState *s, int y, VC_Line *ln);
static void vc_render_line_a2(VCoreState *s, int y, VC_Line *ln);
static void frame_3d_n(VCoreState *s, int also_1x);
static void picture_n(VCoreState *s, int a_top);

/* Threads: the 3D frame (geometry, raster, post) runs on its own thread while engine B (it has no 3D)
 * runs on another and engine A draws everything but its 3D layer; A's compositing follows the 3D join. Every stage
 * reads the same input and writes its own buffers (the 2D pieces keep no state; each engine draws into its own
 * VC_Line), so the picture is the sequential render's, byte for byte. A display capture writes VRAM during engine A's
 * lines and engine B may read that bank, so with a capture running B waits for A (as the sequential frame had it). */
typedef struct FrameJob {
  VCoreState *s;
  int e; /* engine for the 2D job */
  int cap; /* the N x 3D job: a display capture runs, so the 1x 3D too */
} FrameJob;

static void *job_3d(void *arg) {
  vc_3d_frame(((FrameJob *)arg)->s);
  return NULL;
}

static void *job_3d_n(void *arg) {
  FrameJob *j = (FrameJob *)arg;
  frame_3d_n(j->s, j->cap);
  return NULL;
}

static void render_engine(VCoreState *s, int e, VC_Line *ln) {
  if (e == VC_ENG_B && s->p20.keep_b != NULL) {
    memcpy(s->eng_out[VC_ENG_B], s->p20.keep_b, sizeof s->eng_out[0]);
    return;
  }
  STAGE(VC_ST_2DA + e, for (int y = 0; y < VC_H; y++) vc_render_line_ln(s, e, y, ln));
}

static void *job_2d(void *arg) {
  FrameJob *j = (FrameJob *)arg;
  render_engine(j->s, j->e, &j->s->p20.line_b);
  return NULL;
}

/* run fn(arg) on a new thread (8 MB stack: the rasterizer's locals); 0 when the thread could not start */
static int spawn(pthread_t *t, void *(*fn)(void *), void *arg) {
  pthread_attr_t at;
  if (pthread_attr_init(&at) != 0) return 0;
  pthread_attr_setstacksize(&at, 8u << 20);
  int ok = pthread_create(t, &at, fn, arg) == 0;
  pthread_attr_destroy(&at);
  return ok;
}

void vc_render_frame(VCoreState *s) {
  STAGE(VC_ST_MAP, vc_vram_map(s));
  FrameJob j3 = {s, 0, 0}, jb = {s, VC_ENG_B, 0};
  VCTask t3, tb;
  int th3 = vc_threads && vcore_task_start(&t3, job_3d, &j3);
  if (!th3) job_3d(&j3);
  int capture = (vc_io32(s, VC_REG_DISPCAPCNT) >> 31) & 1;
  int thb = vc_threads && !capture && s->p20.keep_b == NULL && vcore_task_start(&tb, job_2d, &jb);
  /* engine A: with a 3D layer, draw all but that layer's pixels while the 3D frame renders, then composite */
  int uses3d = (vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT) & 8) != 0;
  if (uses3d && th3) {
    STAGE(VC_ST_2DA, for (int y = 0; y < VC_H; y++) vc_render_line_a1(s, y, &s->p20.stage[y]));
    vcore_task_join(&t3);
    STAGE(VC_ST_2DA, for (int y = 0; y < VC_H; y++) vc_render_line_a2(s, y, &s->p20.stage[y]));
    s->p20.stage_ok = !capture; /* a display capture writes VRAM during the lines: no second 3D pass */
  } else {
    if (th3) vcore_task_join(&t3);
    render_engine(s, VC_ENG_A, &s->line);
  }
  if (thb) vcore_task_join(&tb);
  else render_engine(s, VC_ENG_B, &s->line);
  vc_capture_end(s);
  /* the screens: POWCNT1 bit 15 set = engine A on the top screen (GBATEK 30) */
  int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  memcpy(s->screen[a_top ? VC_SCREEN_TOP : VC_SCREEN_BOTTOM], s->eng_out[VC_ENG_A], sizeof s->eng_out[0]);
  memcpy(s->screen[a_top ? VC_SCREEN_BOTTOM : VC_SCREEN_TOP], s->eng_out[VC_ENG_B], sizeof s->eng_out[0]);
}

/* The frame at N x (p21; vcore_render with a scale above 1). Engine A's 3D layer is drawn at N x and composited at N x with
 * its 2D layers (the DS's pixels, scaled); engine B and every other display mode are the 1x screens scaled by N. The 1x
 * 3D is drawn only on a frame with a display capture (the capture holds the DS's pixels; then the 1x lines are complete
 * too). The 1x screens of other frames lack the lines that show the 3D layer. */
void vc_render_frame_n(VCoreState *s) {
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  int on = (vc_io16(s, VC_REG_POWCNT1) >> 1) & 1;
  int use_n = (dispcnt & 8) && ((dispcnt >> 16) & 3) == 1 && on && !(dispcnt & 0x80);
  if (!vc_n_begin(s)) {
    vc_render_frame(s);
    return;
  }
  if (!use_n) {
    vc_render_frame(s);
    vc_n_picture_from_screens(s);
    return;
  }
  STAGE(VC_ST_MAP, vc_vram_map(s));
  int capture = (vc_io32(s, VC_REG_DISPCAPCNT) >> 31) & 1;
  FrameJob j3 = {s, 0, capture}, jb = {s, VC_ENG_B, 0};
  VCTask t3, tb;
  int th3 = vc_threads && vcore_task_start(&t3, job_3d_n, &j3);
  if (!th3) job_3d_n(&j3);
  int thb = vc_threads && !capture && s->p20.keep_b == NULL && vcore_task_start(&tb, job_2d, &jb);
  STAGE(VC_ST_2DA, for (int y = 0; y < VC_H; y++) vc_render_line_a1(s, y, &s->p20.stage[y]));
  if (th3) vcore_task_join(&t3);
  STAGE(VC_ST_2DA, for (int y = 0; y < VC_H; y++) if (capture || !s->p20.stage[y].bg3d) vc_render_line_a2(s, y, &s->p20.stage[y]));
  if (thb) vcore_task_join(&tb);
  else render_engine(s, VC_ENG_B, &s->line);
  vc_capture_end(s);
  int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  memcpy(s->screen[a_top ? VC_SCREEN_TOP : VC_SCREEN_BOTTOM], s->eng_out[VC_ENG_A], sizeof s->eng_out[0]);
  memcpy(s->screen[a_top ? VC_SCREEN_BOTTOM : VC_SCREEN_TOP], s->eng_out[VC_ENG_B], sizeof s->eng_out[0]);
  WALL(4, picture_n(s, a_top));
  vc_n_picture_done(s);
  s->p20.stage_ok = capture ? 0 : 2;
}

/* In-between pictures (docs/interp.md): the same frame state drawn again with only the 3D redone (its vertices blended another
 * way, p23), after a vcore_render that left the 2D done: engine A's layers (stage[]) and engine B's screen stay. Byte for byte
 * the full render's picture (BWPORT_INTERP_VERIFY in the bridge checks). Returns 0 when the last render cannot be reused (a
 * frame without a 3D layer, a capture frame, the threads off): the caller renders in full. */
int vc_render_3d_again(VCoreState *s) {
  int mode = s->p20.stage_ok;
  if (mode == 0) return 0;
  vc_reset_geo(s);
  memset(&s->f3d, 0, sizeof s->f3d);
  if (mode == 1) {
    vc_3d_frame(s);
    for (int y = 0; y < VC_H; y++)
      if (s->p20.stage[y].bg3d) vc_render_line_a2(s, y, &s->p20.stage[y]);
    int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
    memcpy(s->screen[a_top ? VC_SCREEN_TOP : VC_SCREEN_BOTTOM], s->eng_out[VC_ENG_A], sizeof s->eng_out[0]);
    return 1;
  }
  if (!vc_n_begin(s)) return 0;
  frame_3d_n(s, 0);
  int a_top = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  picture_n(s, a_top);
  vc_n_picture_done(s);
  return 1;
}

/* The rasterizer in horizontal strips: every strip draws all polygons that reach it, in order, but only
 * its own rows. Rows are independent (the plot writes only its pixel's row; the default plot keeps no other state), so
 * the picture is the sequential one byte for byte. Only with the default plot (tests replace it, and keep p16.st).
 *: the strips are cut by an estimated cost per row (a polygon's bounding-box width on each row it covers) so
 * they hold about the same work, there are 3 per thread and the threads take the next one as they finish (the flyover's
 * equal bands were 2.8 ms against 1 ms), a polygon is skipped before p16 sees it when it misses the strip (p16 built the
 * texture context first), and the threads live in a pool (p20.pool, freed by vc_p20_destroy) instead of being created
 * every frame. */
#define VC_MAX_BANDS 16
#define VC_STRIPS_PER 2
int vc_bands = 4;

typedef struct RPool {
  pthread_t th[VC_MAX_BANDS];
  int nth;       /* workers started (the caller's thread works too) */
  int quit;
  unsigned gen;  /* a frame's job number */
  int pending;   /* workers still on this job */
  pthread_mutex_t m;
  pthread_cond_t go, done;
  VCoreState *s; /* the job */
  int job;       /* 0 the 1x raster's strips, 1 the N x raster's, 2 the N x picture's rows, 3 the N x clear's, 4 the N x post's */
  int phase;     /* job 4: the post's phase */
  int a_top;     /* job 2 */
  int nstrip, ys[VC_MAX_BANDS * VC_STRIPS_PER + 1];
  int next;      /* the next strip to take (atomic) */
  s16 pmin[VC_MAX_POLYS], pmax[VC_MAX_POLYS]; /* per polygon in drawing order: its y extent (pmin > pmax: skip) */
} RPool;

static void strip_rows(RPool *rp, int y0, int y1) {
  VCoreState *s = rp->s;
  for (u32 k = 0; k < s->geo.npoly; k++) {
    if (rp->pmax[k] < y0 || rp->pmin[k] >= y1) continue;
    vc_raster_band(s, &s->geo.poly[s->geo.order[k]], y0, y1);
  }
}
static void work_strips(RPool *rp) {
  for (;;) {
    int i = __atomic_fetch_add(&rp->next, 1, __ATOMIC_RELAXED);
    if (i >= rp->nstrip) return;
    if (rp->job == 0) strip_rows(rp, rp->ys[i], rp->ys[i + 1]);
    else if (rp->job == 1) vc_n_raster_rows(rp->s, rp->ys[i], rp->ys[i + 1]);
    else if (rp->job == 2) vc_n_picture_rows(rp->s, rp->a_top, rp->ys[i], rp->ys[i + 1]);
    else if (rp->job == 3) vc_n_clear_rows(rp->s, rp->ys[i], rp->ys[i + 1]);
    else vc_n_post_rows(rp->s, rp->phase, rp->ys[i], rp->ys[i + 1]);
  }
}
static void *pool_main(void *arg) {
  RPool *rp = (RPool *)arg;
  unsigned seen = 0;
  pthread_mutex_lock(&rp->m);
  for (;;) {
    while (rp->gen == seen && !rp->quit) pthread_cond_wait(&rp->go, &rp->m);
    if (rp->quit) break;
    seen = rp->gen;
    pthread_mutex_unlock(&rp->m);
    work_strips(rp);
    pthread_mutex_lock(&rp->m);
    if (--rp->pending == 0) pthread_cond_signal(&rp->done);
  }
  pthread_mutex_unlock(&rp->m);
  return NULL;
}

static void pool_free(RPool *rp) {
  if (!rp) return;
  pthread_mutex_lock(&rp->m);
  rp->quit = 1;
  pthread_cond_broadcast(&rp->go);
  pthread_mutex_unlock(&rp->m);
  for (int i = 0; i < rp->nth; i++) pthread_join(rp->th[i], NULL);
  pthread_mutex_destroy(&rp->m);
  pthread_cond_destroy(&rp->go);
  pthread_cond_destroy(&rp->done);
  free(rp);
}

static RPool *pool_make(int nworkers) {
  RPool *rp = (RPool *)calloc(1, sizeof *rp);
  if (!rp) return NULL;
  pthread_mutex_init(&rp->m, NULL);
  pthread_cond_init(&rp->go, NULL);
  pthread_cond_init(&rp->done, NULL);
  for (int i = 0; i < nworkers; i++)
    if (spawn(&rp->th[rp->nth], pool_main, rp)) rp->nth++;
  return rp;
}

void vc_p20_destroy(VCoreState *s) {
  pool_free((RPool *)s->p20.pool);
  s->p20.pool = NULL;
}

/* cut the rows into strips of about equal estimated cost */
static void plan_strips(RPool *rp, int nstrip) {
  VCoreState *s = rp->s;
  double cost[VC_H + 1];
  for (int y = 0; y <= VC_H; y++) cost[y] = 0;
  for (u32 k = 0; k < s->geo.npoly; k++) {
    const VC_Poly *p = &s->geo.poly[s->geo.order[k]];
    int ymn = 1 << 30, ymx = -(1 << 30), xmn = 1 << 30, xmx = -(1 << 30);
    if (p->nv >= 1 && p->nv <= VC_POLY_MAXV) {
      for (int i = 0; i < p->nv; i++) {
        const VC_Vertex *v = &s->geo.vtx[p->v[i]];
        if (v->sy < ymn) ymn = v->sy;
        if (v->sy > ymx) ymx = v->sy;
        if (v->sx < xmn) xmn = v->sx;
        if (v->sx > xmx) xmx = v->sx;
      }
    }
    rp->pmin[k] = (s16)(ymn < -1 ? -1 : ymn > VC_H ? VC_H : ymn);
    rp->pmax[k] = (s16)(ymx < -1 ? -1 : ymx > VC_H ? VC_H : ymx);
    if (ymn > ymx) { rp->pmin[k] = 1 << 14; rp->pmax[k] = -1; continue; }
    int lo = ymn < 0 ? 0 : ymn, hi = ymx >= VC_H ? VC_H - 1 : ymx;
    if (lo > hi) continue;
    if (xmn < 0) xmn = 0;
    if (xmx > VC_W) xmx = VC_W;
    double w = (xmx > xmn ? xmx - xmn : 0) * 0.5 + 6.0; /* a row's setup and about half the box */
    cost[lo] += w;
    cost[hi + 1] -= w;
  }
  double tot = 0, acc = 0;
  for (int y = 0; y < VC_H; y++) { acc += cost[y]; cost[y] = acc + 1.0; tot += cost[y]; }
  rp->nstrip = nstrip;
  rp->ys[0] = 0;
  double run = 0;
  int y = 0;
  for (int i = 1; i < nstrip; i++) {
    double target = tot * i / nstrip;
    while (y < VC_H && run + cost[y] <= target) run += cost[y++];
    rp->ys[i] = y;
  }
  rp->ys[nstrip] = VC_H;
}

/* a pool of at least nb - 1 workers. It only grows: the jobs cut their rows by nb (not by the pool's size) and the
 * workers take strips as they come, so a bigger pool draws the same picture. (It was rebuilt whenever the count
 * changed: at N x a capture frame's 1x raster wants vc_bands, the N x jobs more, two rebuilds a frame.) */
static RPool *pool_get(VCoreState *s, int nb) {
  RPool *rp = (RPool *)s->p20.pool;
  if (rp && rp->nth < nb - 1) {
    pool_free(rp);
    rp = NULL;
  }
  if (!rp) s->p20.pool = rp = pool_make(nb - 1);
  return rp;
}

/* run the job set up in rp (rp->s, job, nstrip, ys) on the workers and the caller */
static void pool_go(RPool *rp) {
  rp->next = 0;
  if (rp->nth > 0) {
    pthread_mutex_lock(&rp->m);
    rp->pending = rp->nth;
    rp->gen++;
    pthread_cond_broadcast(&rp->go);
    pthread_mutex_unlock(&rp->m);
  }
  work_strips(rp);
  if (rp->nth > 0) {
    pthread_mutex_lock(&rp->m);
    while (rp->pending > 0) pthread_cond_wait(&rp->done, &rp->m);
    pthread_mutex_unlock(&rp->m);
  }
}

static int bands_env_set;
static void bands_init(void) {
  static int env_done;
  if (!env_done) {
    const char *v = getenv("BWPORT_VCORE_BANDS");
    if (v) { vc_bands = atoi(v); bands_env_set = 1; }
    env_done = 1;
  }
}

static void raster_all(VCoreState *s) {
  bands_init();
  int nb = vc_threads ? vc_bands : 1;
  if (nb > VC_MAX_BANDS) nb = VC_MAX_BANDS;
  if (nb <= 1 || s->plot != vc_3d_plot) {
    for (u32 k = 0; k < s->geo.npoly; k++) vc_raster_band(s, &s->geo.poly[s->geo.order[k]], 0, VC_H);
    return;
  }
  RPool *rp = pool_get(s, nb);
  if (!rp) {
    for (u32 k = 0; k < s->geo.npoly; k++) vc_raster_band(s, &s->geo.poly[s->geo.order[k]], 0, VC_H);
    return;
  }
  rp->s = s;
  rp->job = 0;
  plan_strips(rp, nb * VC_STRIPS_PER);
  pool_go(rp);
}

/* the N x raster and picture: the same pool, more bands the bigger the picture (the rows are N times as many) */
static int bands_n(int n) {
  bands_init();
  int nb = vc_threads ? vc_bands : 1;
  if (!bands_env_set && nb > 1 && n > 1) nb += 2 * (n - 1);
  return nb > VC_MAX_BANDS ? VC_MAX_BANDS : nb;
}

static void raster_n(VCoreState *s) {
  int nb = bands_n(s->p21.scale);
  RPool *rp = nb > 1 ? pool_get(s, nb) : NULL;
  if (!rp) {
    vc_n_raster_rows(s, 0, VC_H * (s->p21.scale < 1 ? 1 : s->p21.scale));
    return;
  }
  rp->s = s;
  rp->job = 1;
  rp->nstrip = nb * VC_STRIPS_PER;
  vc_n_plan_strips(s, rp->nstrip, rp->ys);
  pool_go(rp);
}

/* the N x clear and post on the pool too: rows in equal strips */
static RPool *rows_n(VCoreState *s, int job) {
  int nb = bands_n(s->p21.scale);
  RPool *rp = nb > 1 ? pool_get(s, nb) : NULL;
  if (!rp) return NULL;
  int H = VC_H * (s->p21.scale < 1 ? 1 : s->p21.scale);
  rp->s = s;
  rp->job = job;
  rp->nstrip = nb * VC_STRIPS_PER;
  if (rp->nstrip > H) rp->nstrip = H;
  for (int i = 0; i <= rp->nstrip; i++) rp->ys[i] = H * i / rp->nstrip;
  return rp;
}

static void clear_n(VCoreState *s) {
  RPool *rp = rows_n(s, 3);
  if (!rp) {
    vc_n_clear(s);
    return;
  }
  pool_go(rp);
}

static void post_n(VCoreState *s) {
  RPool *rp = rows_n(s, 4);
  if (!rp) {
    vc_n_post(s);
    return;
  }
  int np = vc_n_post_begin(s);
  for (int ph = 0; ph < np; ph++) {
    rp->phase = ph;
    pool_go(rp);
  }
}

static void picture_n(VCoreState *s, int a_top) {
  int nb = bands_n(s->p21.scale);
  RPool *rp = nb > 1 ? pool_get(s, nb) : NULL;
  if (!rp) {
    vc_n_picture_rows(s, a_top, 0, VC_H);
    return;
  }
  rp->s = s;
  rp->job = 2;
  rp->a_top = a_top;
  rp->nstrip = nb * VC_STRIPS_PER;
  for (int i = 0; i <= rp->nstrip; i++) rp->ys[i] = VC_H * i / rp->nstrip;
  pool_go(rp);
}

/* the geometry engine over the command stream; 0 when the stream holds no complete frame */
static int geometry_stream(VCoreState *s) {
  vc_mtx_reset(s);
  vc_poly_reset(s);
  vc_light_reset(s);
  u32 i = 0;
  double t0 = now_ns();
  while (i < s->gx_words) {
    u32 h = s->gx[i++];
    u32 op = h & 0xFF, n = (h >> 8) & 0xFF;
    if (n > s->gx_words - i) n = s->gx_words - i;
    vc_gx_command(s, op, s->gx + i, n);
    i += n;
  }
  vc_stage_ns[VC_ST_GEO] += now_ns() - t0;
  return s->geo.swapped != 0;
}

void vc_3d_frame(VCoreState *s) {
  if (!geometry_stream(s)) return; /* no complete frame in the stream: the 3D layer stays clear */
  STAGE(VC_ST_CLEAR, vc_3d_clear(s));
  STAGE(VC_ST_RASTER, raster_all(s));
  STAGE(VC_ST_POST, vc_3d_post(s));
}

/* the 3D frame at N x (p21): the N x buffers, and on a capture frame the 1x 3D too (the capture holds the DS's pixels) */
static void frame_3d_n(VCoreState *s, int also_1x) {
  if (!geometry_stream(s)) {
    vc_n_none(s);
    return;
  }
  WALL(0, vc_n_plan(s));
  WALL(1, clear_n(s));
  WALL(2, raster_n(s));
  WALL(3, post_n(s));
  if (also_1x) {
    STAGE(VC_ST_CLEAR, vc_3d_clear(s));
    STAGE(VC_ST_RASTER, raster_all(s));
    STAGE(VC_ST_POST, vc_3d_post(s));
  }
}

/* a line in two halves so that engine A can draw before the 3D layer exists: a1 = OBJs, BGs (the 3D BG only marked),
 * windows; a2 = the 3D layer, composite, display, capture */
static void bg_line(VCoreState *s, int e, int n, int y, VC_Line *ln, int defer3d);

static void vc_render_line_a1(VCoreState *s, int y, VC_Line *ln) {
  memset(ln, 0, sizeof *ln);
  memset(ln->winmask, 0x3F, sizeof ln->winmask);
  vc_obj_line(s, VC_ENG_A, y, ln);
  for (int n = 0; n < 4; n++) bg_line(s, VC_ENG_A, n, y, ln, 1);
  vc_window_line(s, VC_ENG_A, y, ln);
}

static void vc_render_line_a2(VCoreState *s, int y, VC_Line *ln) {
  if (ln->bg3d) vc_3d_layer_line(s, y, ln);
  vc_compose_line(s, VC_ENG_A, y, ln);
  vc_display_line(s, VC_ENG_A, y, ln, s->eng_out[VC_ENG_A][y]);
  vc_capture_line(s, y, ln);
}

static void vc_render_line_ln(VCoreState *s, int e, int y, VC_Line *ln) {
  memset(ln, 0, sizeof *ln);
  memset(ln->winmask, 0x3F, sizeof ln->winmask); /* everything shows unless a window is on (piece 8) */
  vc_obj_line(s, e, y, ln);                      /* checks DISPCNT bit 12 itself */
  for (int n = 0; n < 4; n++) bg_line(s, e, n, y, ln, 0);
  vc_window_line(s, e, y, ln);
  vc_compose_line(s, e, y, ln);
  vc_display_line(s, e, y, ln, s->eng_out[e][y]);
  if (e == VC_ENG_A) vc_capture_line(s, y, ln); /* the line is displayed from VRAM before the capture overwrites it (oracle) */
}

void vc_render_line(VCoreState *s, int e, int y) { vc_render_line_ln(s, e, y, &s->line); }

/* GBATEK 25: the BG kind of BG n in BG mode m: 0 none, 1 text, 2 affine, 3 extended, 4 large, 5 3D (BG0, bit 3) */
static const u8 kBgKind[8][4] = {
    {1, 1, 1, 1}, {1, 1, 1, 2}, {1, 1, 2, 2}, {1, 1, 1, 3},
    {1, 1, 2, 3}, {1, 1, 3, 3}, {5, 0, 4, 0}, {0, 0, 0, 0},
};

void vc_bg_line(VCoreState *s, int e, int n, int y, VC_Line *ln) { bg_line(s, e, n, y, ln, 0); }

static void bg_line(VCoreState *s, int e, int n, int y, VC_Line *ln, int defer3d) {
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  if (!(dispcnt & (1u << (8 + n)))) return;
  int kind = kBgKind[dispcnt & 7][n];
  if (n == 0 && (dispcnt & 8)) kind = e == VC_ENG_A ? 5 : kind; /* engine B has no 3D */
  else if (kind == 5) kind = 0;                                  /* mode 6 BG0 without the 3D: nothing */
  if (e == VC_ENG_B && kind == 4) kind = 0;                      /* engine B has no large bitmap */
  if (kind == 0) return;
  ln->bg_on[n] = 1;
  if (kind == 5) {
    ln->bg3d = 1;
    if (!defer3d) vc_3d_layer_line(s, y, ln);
    return;
  }
  int vy = vc_mosaic_bg_y(s, e, n, y);
  switch (kind) {
  case 1: vc_bg_text_line(s, e, n, vy, ln); break;
  case 2: vc_bg_affine_line(s, e, n, vy, ln); break;
  case 3: vc_bg_ext_line(s, e, n, vy, ln); break;
  case 4: vc_bg_large_line(s, e, vy, ln); break;
  }
  vc_mosaic_bg_line(s, e, n, ln->bg[n]);
}

/* GBATEK 55: the 3D layer is a 512 pixel wide BG0: the 256 pixel image, then 256 transparent pixels, wrapping.
 * BG0HOFS (9 bits) scrolls it horizontally; no vertical scroll, no mosaic. Alpha 0 = transparent. */
void vc_3d_layer_line(VCoreState *s, int y, VC_Line *ln) {
  int hofs = vc_eng16(s, VC_ENG_A, VC_REG_BG0HOFS) & 0x1FF;
  for (int x = 0; x < VC_W; x++) {
    int pos = (x + hofs) & 0x1FF;
    u32 px = pos < VC_W ? s->f3d.out[y][pos] : 0;
    ln->l3d[x] = ((px >> 24) & 0x1F) ? px : 0;
  }
}
