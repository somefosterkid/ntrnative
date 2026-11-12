/* gpu_glsl OUTDIR [es|gl]...: writes every shader program the GL cores can build (src/gpu_gl.c's text, without a context),
 * one file per shader, for an offline compiler (tools/gpu_glsl_check.py: glslang in strict ES mode, Arm's malioc, Qualcomm's
 * compiler; docs/gpu-compat.md). Names: <fs>.<api>.frag and <vs>.<api>.vert ('#' -> '_k', '+' -> '_'), and a list file
 * OUTDIR/programs.txt with one line a program: "api vs-file fs-file" (the pair a link check takes).
 *
 * The list below is every name the C sources in src/ hand gpu_gl_program / gpu_gl_program_tf; gpu_glsl_check.py fails when a name in
 * the C sources is missing from it. ES has the fast 3D renderer only with EXT_shader_framebuffer_fetch (src/g10_fast_gl.c's
 * fast_mode), GL also its image modes (GLSL 4.50 with an interlock or ordering extension). */
#include "gpu_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Prog {
  const char *vs, *fs;
  int tf;   /* gpu_gl_program_tf */
  int apis; /* 1: ES, 2: GL */
} Prog;

static const Prog kProgs[] = {
  /* pass R (src/g00_frame.c): one per k, and Filter=bilinear's ("#Kf", raster_prog) */
  {"g00_full.vert", "g00_raster.frag", 0, 3},
  {"g00_full.vert", "g00_raster.frag#2", 0, 3},
  {"g00_full.vert", "g00_raster.frag#1f", 0, 3},
  {"g00_full.vert", "g00_raster.frag#2f", 0, 3},
  /* pass P and the edge map (src/g00_frame.c, src/g06_post.c) */
  {"g00_full.vert", "g06_post.frag", 0, 3},
  {"g00_full.vert", "g06_post.frag#2", 0, 3},
  {"g00_full.vert", "g06_post.frag#2+P_FAST", 0, 3},
  {"g00_full.vert", "g06_edgemap.frag", 0, 3},
  {"g00_full.vert", "g06_edgemap.frag#2", 0, 3},
  /* pass S (src/g02_setup.c): transform feedback, and the fragment form */
  {"g02_setup.vert", "(transform feedback)", 1, 3},
  {"g00_full.vert", "g02_setup.vert", 0, 3},
  /* pass C (src/g07_compose.c) and its plan */
  {"g00_full.vert", "g07_compose.frag", 0, 3},
  {"g00_full.vert", "g07_compose.frag#1+C_PLAN", 0, 3},
  /* SMAA (src/g08_smaa.c) */
  {"g00_full.vert", "g08_smaa_edge.frag", 0, 3},
  {"g00_full.vert", "g08_smaa_weight.frag", 0, 3},
  {"g00_full.vert", "g08_smaa_blend.frag", 0, 3},
  /* the fast 3D renderer (src/g10_fast_gl.c): clear, triangles (per mode, and Filter=bilinear's), pack, the probe */
  {"g00_full.vert", "g10_clear.frag", 0, 3},
  {"g00_full.vert", "g10_clear4.frag", 0, 3},
  {"g00_full.vert", "g10_pack.frag", 0, 3},
  {"g10_fastgl.vert", "g10_clear.frag", 0, 3},
  {"g10_fastgl.vert", "g10_fast.frag#1+G10_FETCH", 0, 3},
  {"g10_fastgl.vert", "g10_fast.frag#1f+G10_FETCH", 0, 3},
  {"g10_fast4.vert", "g10_fast4.frag#1+G10_FETCH", 0, 3},
  {"g10_fast4.vert", "g10_fast4.frag#1f+G10_FETCH", 0, 3},
  {"g10_fastgl.vert", "g10_fast.frag#1+G10_ILOCK", 0, 2},
  {"g10_fastgl.vert", "g10_fast.frag#1f+G10_ILOCK", 0, 2},
  {"g10_fast4.vert", "g10_fast4.frag#1+G10_ILOCK", 0, 2},
  {"g10_fast4.vert", "g10_fast4.frag#1f+G10_ILOCK", 0, 2},
  {"g10_fastgl.vert", "g10_fast.frag#1+G10_IORDER", 0, 2},
  {"g10_fastgl.vert", "g10_fast.frag#1f+G10_IORDER", 0, 2},
  {"g10_fast4.vert", "g10_fast4.frag#1+G10_IORDER", 0, 2},
  {"g10_fast4.vert", "g10_fast4.frag#1f+G10_IORDER", 0, 2},
};

static void fname(char *out, size_t n, const char *name, const char *api, const char *stage, int tf) {
  char b[128];
  size_t j = 0;
  for (const char *p = name; *p && j + 3 < sizeof b; p++) {
    if (*p == '#') b[j++] = '_', b[j++] = 'k';
    else if (*p == '+') b[j++] = '_';
    else b[j++] = *p;
  }
  b[j] = 0;
  snprintf(out, n, "%s%s.%s.%s", b, tf ? "_tf" : "", api, stage);
}

static int put(const char *dir, const char *name, const char *text) {
  char path[1024];
  snprintf(path, sizeof path, "%s/%s", dir, name);
  FILE *f = fopen(path, "wb");
  if (f == NULL) return -1;
  fputs(text, f);
  return fclose(f);
}

/* the log's lines, indented, without the empty ones; other control bytes as \xNN */
static void show_log(const char *log) {
  int col = 0;
  for (const unsigned char *p = (const unsigned char *)log; *p; p++) {
    if (*p == '\n' || *p == '\r') {
      if (col) putchar('\n');
      col = 0;
      continue;
    }
    if (!col) fputs("    ", stdout);
    if (*p < 32 || *p == 127) printf("\\x%02x", *p);
    else putchar(*p);
    col++;
  }
  if (col) putchar('\n');
}

static GLuint compile1(GLenum kind, const char *text, char *log, size_t loglen, int *ok) {
  GLuint sh = GL(CreateShader)(kind);
  GL(ShaderSource)(sh, 1, &text, NULL);
  GL(CompileShader)(sh);
  GLint st = 0;
  GL(GetShaderiv)(sh, GL_COMPILE_STATUS, &st);
  size_t l = strlen(log);
  GLsizei got = 0;
  GL(GetShaderInfoLog)(sh, (GLsizei)(loglen - l - 1), &got, log + l);
  log[l + (got > 0 ? (size_t)got : 0)] = 0;
  *ok = st != 0;
  return sh;
}

static int has_ext(const char *name) {
  GLint n = 0;
  GL(GetIntegerv)(GL_NUM_EXTENSIONS, &n);
  for (GLint i = 0; i < n; i++) {
    const char *x = (const char *)GL(GetStringi)(GL_EXTENSIONS, (GLuint)i);
    if (x != NULL && !strcmp(x, name)) return 1;
  }
  return 0;
}

/* --compile: every program of the context's API compiled and linked by the context's own compiler (any EGL: Mesa, or
 * ANGLE by LD_LIBRARY_PATH, tools/gpu_glsl_check.py --angle). One line a program: ok / warn / FAIL / skip, the log
 * indented under it. Exit 1 when one fails. */
static int compile_all(int api) {
  char err[2048];
  GpuCtx *c = gpu_ctx_headless(api, err, sizeof err);
  if (c == NULL || gpu_gl_load(gpu_ctx_getproc(c), api, err, sizeof err) != 0) {
    printf("gpu_glsl: no context: %s\n", err);
    return 1;
  }
  printf("context %s; %s; %s\n", gpu_ctx_desc(c), (const char *)GL(GetString)(GL_RENDERER),
         (const char *)GL(GetString)(GL_VERSION));
  int a = gpu_gl.api == GPU_API_ES ? 1 : 2, nok = 0, nwarn = 0, nfail = 0, nskip = 0;
  int fetch = has_ext("GL_EXT_shader_framebuffer_fetch");
  for (size_t i = 0; i < sizeof kProgs / sizeof kProgs[0]; i++) {
    const Prog *p = &kProgs[i];
    if (!(p->apis & a)) continue;
    if ((strstr(p->fs, "G10_FETCH") && !fetch) || ((strstr(p->fs, "G10_ILOCK") || strstr(p->fs, "G10_IORDER")))) {
      printf("skip %s + %s: %s\n", p->vs, p->fs, strstr(p->fs, "G10_FETCH") ? "no EXT_shader_framebuffer_fetch" : "the image modes (desktop GL 4.5)");
      nskip++;
      continue;
    }
    char *vt, *ft;
    if (gpu_gl_program_text(p->vs, p->fs, p->tf, &vt, &ft, err, sizeof err) != 0) {
      printf("FAIL %s + %s: %s\n", p->vs, p->fs, err);
      nfail++;
      continue;
    }
    static char log[16384];
    log[0] = 0;
    int okv, okf, okl = 0;
    GLuint v = compile1(GL_VERTEX_SHADER, vt, log, sizeof log, &okv);
    GLuint f = compile1(GL_FRAGMENT_SHADER, ft, log, sizeof log, &okf);
    if (okv && okf) {
      GLuint pr = GL(CreateProgram)();
      GL(AttachShader)(pr, v);
      GL(AttachShader)(pr, f);
      GL(LinkProgram)(pr);
      GLint st = 0;
      GL(GetProgramiv)(pr, GL_LINK_STATUS, &st);
      okl = st != 0;
      size_t l = strlen(log);
      GLsizei got = 0;
      GL(GetProgramInfoLog)(pr, (GLsizei)(sizeof log - l - 1), &got, log + l);
      log[l + (got > 0 ? (size_t)got : 0)] = 0;
      GL(DeleteProgram)(pr);
    }
    GL(DeleteShader)(v);
    GL(DeleteShader)(f);
    const char *what = !(okv && okf && okl) ? "FAIL" : log[strspn(log, " \n")] ? "warn" : "ok";
    printf("%s %s + %s%s\n", what, p->vs, p->fs, !okv ? " (vertex)" : !okf ? " (fragment)" : !okl ? " (link)" : "");
    show_log(log);
    if (what[0] == 'F') nfail++;
    else if (what[0] == 'w') nwarn++;
    else nok++;
    free(vt);
    free(ft);
  }
  printf("gpu_glsl --compile: %d ok, %d with warnings, %d failed, %d skipped\n", nok, nwarn, nfail, nskip);
  gpu_ctx_destroy(c);
  return nfail ? 1 : 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: gpu_glsl OUTDIR [es|gl]...   the GL cores' shader programs as text\n"
                    "       gpu_glsl --compile [es|gl]    compile and link each in a headless context\n");
    return 2;
  }
  if (!strcmp(argv[1], "--compile")) return compile_all(argc > 2 && !strcmp(argv[2], "gl") ? GPU_API_GL : GPU_API_ES);
  const char *dir = argv[1];
  int want = 0;
  for (int i = 2; i < argc; i++) want |= !strcmp(argv[i], "es") ? 1 : !strcmp(argv[i], "gl") ? 2 : 0;
  if (want == 0) want = 3;
  char list[1024];
  snprintf(list, sizeof list, "%s/programs.txt", dir);
  FILE *lf = fopen(list, "w");
  if (lf == NULL) {
    fprintf(stderr, "gpu_glsl: cannot write %s\n", list);
    return 1;
  }
  int bad = 0, nprog = 0;
  for (int a = 1; a <= 2; a++) {
    if (!(want & a)) continue;
    gpu_gl.api = a == 1 ? GPU_API_ES : GPU_API_GL;
    const char *api = a == 1 ? "es" : "gl";
    for (size_t i = 0; i < sizeof kProgs / sizeof kProgs[0]; i++) {
      const Prog *p = &kProgs[i];
      if (!(p->apis & a)) continue;
      char *vt, *ft, err[256];
      if (gpu_gl_program_text(p->vs, p->fs, p->tf, &vt, &ft, err, sizeof err) != 0) {
        fprintf(stderr, "gpu_glsl: %s\n", err);
        bad++;
        continue;
      }
      char vn[200], fn[200];
      fname(vn, sizeof vn, p->vs, api, "vert", p->tf);
      fname(fn, sizeof fn, p->tf ? p->vs : p->fs, api, "frag", p->tf);
      if (put(dir, vn, vt) != 0 || put(dir, fn, ft) != 0) bad++;
      fprintf(lf, "%s %s %s %s %s\n", api, vn, fn, p->vs, p->fs);
      nprog++;
      free(vt);
      free(ft);
    }
  }
  fclose(lf);
  printf("gpu_glsl: %d programs in %s%s\n", nprog, dir, bad ? " (with errors)" : "");
  return bad ? 1 : 0;
}
