/* port/gpu: the GL function table, shader programs, data textures (docs/gpu.md). OWNED BY THE SKELETON (+ G1). */
#include "gpu_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GpuGL gpu_gl;

GpuPassHook gpu_pass_hook;
void gpu_set_pass_hook(GpuPassHook fn) { gpu_pass_hook = fn; }

/* gpu_gl_uniform's table: open addressing by (program, name address), never more than 3/4 full (past that a lookup is
 * not kept) */
enum { UNI_CAP = 256 };
static struct {
  GLuint prog;
  const char *name;
  GLint loc;
} uni_tab[UNI_CAP];
static int uni_n;
static void uni_forget(void) {
  memset(uni_tab, 0, sizeof uni_tab);
  uni_n = 0;
}
GLint gpu_gl_uniform(GLuint prog, const char *name) {
  unsigned h = (unsigned)(((uintptr_t)name >> 2) ^ (uintptr_t)prog * 2654435761u) % UNI_CAP;
  for (int i = 0; i < UNI_CAP; i++) {
    unsigned k = (h + (unsigned)i) % UNI_CAP;
    if (uni_tab[k].name == NULL) {
      GLint loc = GL(GetUniformLocation)(prog, name);
      if (uni_n < UNI_CAP * 3 / 4) {
        uni_tab[k].prog = prog;
        uni_tab[k].name = name;
        uni_tab[k].loc = loc;
        uni_n++;
      }
      return loc;
    }
    if (uni_tab[k].prog == prog && uni_tab[k].name == name) return uni_tab[k].loc;
  }
  return GL(GetUniformLocation)(prog, name);
}

int gpu_gl_load(GpuGetProc getproc, int api, char *err, size_t errlen) {
  char missing[512] = "";
  int nmiss = 0;
#define GPU_GL_GET(type, name)                                                                                          gpu_gl.name = (type)getproc("gl" #name);                                                                              if (gpu_gl.name == NULL) {                                                                                              size_t l = strlen(missing);                                                                                           if (l + 40 < sizeof missing) snprintf(missing + l, sizeof missing - l, "%sgl%s", nmiss ? ", " : "", #name);           nmiss++;                                                                                                            }
  GPU_GL_LIST(GPU_GL_GET)
#undef GPU_GL_GET
  if (nmiss) {
    snprintf(err, errlen, "%d GL function%s missing: %s%s", nmiss, nmiss > 1 ? "s" : "", missing,
             strlen(missing) > 440 ? " ..." : "");
    return -1;
  }
  gpu_gl.api = api;
  uni_forget();
  const char *ver = (const char *)gpu_gl.GetString(GL_VERSION);
  if (ver != NULL && ver[0]) gpu_gl.api = strncmp(ver, "OpenGL ES", 9) == 0 ? GPU_API_ES : GPU_API_GL;
  if (ver == NULL) {
    snprintf(err, errlen, "glGetString(GL_VERSION) returned NULL: no GL context is current on this thread");
    return -1;
  }
  int maj = 0, min = 0, inv = gpu_gl.api == GPU_API_ES;
  if (!inv && sscanf(ver, "%d.%d", &maj, &min) == 2) inv = maj > 4 || (maj == 4 && min >= 3);
  if (!inv) {
    GLint next = 0;
    gpu_gl.GetIntegerv(GL_NUM_EXTENSIONS, &next);
    for (GLint i = 0; i < next && !inv; i++) {
      const char *x = (const char *)gpu_gl.GetStringi(GL_EXTENSIONS, (GLuint)i);
      inv = x != NULL && !strcmp(x, "GL_ARB_invalidate_subdata");
    }
  }
  gpu_gl.InvalidateFramebuffer = inv ? (PFNGLINVALIDATEFRAMEBUFFERPROC)getproc("glInvalidateFramebuffer") : NULL;
  gpu_gl.BindImageTexture = (void (*)(GLuint, GLuint, GLint, GLboolean, GLint, GLenum, GLenum))getproc("glBindImageTexture");
  gpu_gl.MemoryBarrier = (void (*)(GLbitfield))getproc("glMemoryBarrier");
  gpu_gl.FramebufferParameteri = (void (*)(GLenum, GLenum, GLint))getproc("glFramebufferParameteri");
  return 0;
}

void gpu_gl_invalidate(int n) {
  static const GLenum att[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  if (gpu_gl.InvalidateFramebuffer != NULL) gpu_gl.InvalidateFramebuffer(GL_FRAMEBUFFER, n, att);
}

const char *gpu_gl_error_name(GLenum e) {
  switch (e) {
  case GL_NO_ERROR: return "GL_NO_ERROR";
  case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
  case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
  case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
  default: return "GL_UNKNOWN_ERROR";
  }
}

int gpu_gl_errors(const char *where, char *err, size_t errlen) {
  int n = 0;
  size_t l = 0;
  for (GLenum e; n < 16 && (e = gpu_gl.GetError()) != GL_NO_ERROR; n++) {
    if (n == 0) l = (size_t)snprintf(err, errlen, "%s: ", where);
    else if (l + 2 < errlen) l += (size_t)snprintf(err + l, errlen - l, ", ");
    if (l + 1 < errlen) l += (size_t)snprintf(err + l, errlen - l, "%s (0x%x)", gpu_gl_error_name(e), e);
  }
  return n;
}

/* ---- shader sources: embedded, or from BWPORT_GPU_SHADERS=DIR (iterating without a rebuild) ---- */

typedef struct Src {
  char *name, *text;
} Src;

static int by_name(const void *a, const void *b) { return strcmp(((const Src *)a)->name, ((const Src *)b)->name); }

static char *read_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (f == NULL) return NULL;
  fseek(f, 0, SEEK_END);
  long n = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *t = malloc((size_t)n + 1);
  if (t != NULL && fread(t, 1, (size_t)n, f) != (size_t)n) {
    free(t);
    t = NULL;
  }
  if (t != NULL) t[n] = 0;
  fclose(f);
  return t;
}

/* the sources' table: embedded unless BWPORT_GPU_SHADERS names a directory, whose files replace the embedded ones of
 * the same name (the set of names stays the embedded one) */
static int sources(Src *out, int max) {
  const char *dir = getenv("BWPORT_GPU_SHADERS");
  int n = 0;
  for (const GpuShaderSrc *e = gpu_shader_src; e->name != NULL && n < max; e++, n++) {
    out[n].name = (char *)e->name;
    out[n].text = (char *)e->text;
    if (dir != NULL && dir[0]) {
      char path[1024];
      snprintf(path, sizeof path, "%s/%s", dir, e->name);
      char *t = read_file(path);
      if (t != NULL) out[n].text = t; /* leaked: a debugging path, once per program */
    }
  }
  qsort(out, (size_t)n, sizeof *out, by_name);
  return n;
}

static const char *ends(const char *s, const char *suf) {
  size_t a = strlen(s), b = strlen(suf);
  return a >= b && strcmp(s + a - b, suf) == 0 ? s : NULL;
}

/* the API's first lines: ES 3.00 with highp everywhere, or desktop 3.30 core (precision qualifiers are accepted
 * and ignored there) */
static const char *header(void) {
  if (gpu_gl.api == GPU_API_GL) return "#version 330 core\n#define GPU_GL 1\n";
  return "#version 300 es\nprecision highp float;\nprecision highp int;\nprecision highp usampler2D;\n"
         "precision highp sampler2D;\nprecision highp isampler2D;\n";
}

/* "+DEF" of the fast 3D renderer's programs on GL (shaders/g10_fast.frag): the extension it needs, right after #version; the
 * image modes are GLSL 4.50 (images, binding = ; glslang wants 4.50 for the interlock qualifier) */
static const char *def_ext(const char *def) {
  if (!strcmp(def, "G10_FETCH")) return "#extension GL_EXT_shader_framebuffer_fetch : require\n";
  if (!strcmp(def, "G10_ILOCK")) return "#extension GL_ARB_fragment_shader_interlock : require\n";
  if (!strcmp(def, "G10_IORDER")) return "#extension GL_INTEL_fragment_shader_ordering : require\n";
  return NULL;
}

static GLuint compile(GLenum kind, const char **parts, int nparts, char *err, size_t errlen, const char *what) {
  GLuint sh = GL(CreateShader)(kind);
  GL(ShaderSource)(sh, nparts, parts, NULL);
  GL(CompileShader)(sh);
  GLint ok = 0;
  GL(GetShaderiv)(sh, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char log[2048] = {0};
    GL(GetShaderInfoLog)(sh, sizeof log - 1, NULL, log);
    snprintf(err, errlen, "%s: %s", what, log);
    GL(DeleteShader)(sh);
    return 0;
  }
  return sh;
}

/* a program's text as the compiler gets it: the vertex shader's parts and the fragment shader's */
typedef struct Asm {
  const char *vparts[3], *parts[71];
  int nv, np;
  char kdef[96], fhead[400];
} Asm;

/* tf: a transform-feedback program (gpu_gl_program_tf): vs has the head before it, fs is ignored and an empty fragment
 * shader stands in */
static int assemble(Asm *a, const char *vs, const char *fs, int tf, char *err, size_t errlen) {
  Src src[64];
  int n = sources(src, 64);
  const char **parts = a->parts;
  int np = 0;
  const char *vtext = NULL, *ftext = NULL, *head = NULL;
  /* "name.frag#K": the fragment shader with G_K defined as K (multisampling: shaders/g00_head.glsl); "#Kf" also defines G_FILTER
   * (pass R's programs for Filter=bilinear, shaders/g04_texture.glsl); "#K+DEF" (or "#Kf+DEF") DEF as well (a second program
   * from the file: shaders/g06_post.frag's P_FAST). tools/gpu_msl.py builds the same text for Metal. */
  const char *hash = strchr(fs, '#'), *plus = strchr(fs, '+');
  size_t fsl = hash != NULL ? (size_t)(hash - fs) : strlen(fs);
  char *kdef = a->kdef;
  kdef[0] = 0;
  if (hash != NULL) {
    char *after;
    long kk = strtol(hash + 1, &after, 10);
    snprintf(kdef, sizeof a->kdef, "#define G_K %ld\n%s", kk, *after == 'f' ? "#define G_FILTER 1\n" : "");
    if (plus != NULL) snprintf(kdef + strlen(kdef), sizeof a->kdef - strlen(kdef), "#define %s 1\n", plus + 1);
  }
  /* a vertex shader's file as the fragment shader ("g02_setup.vert"): its body with S_FRAG defined, after the head only */
  const int vbody = !tf && fsl > 5 && !strncmp(fs + fsl - 5, ".vert", 5);
  if (vbody) snprintf(kdef, sizeof a->kdef, "#define S_FRAG 1\n");
  for (int i = 0; i < n; i++) {
    if (!strcmp(src[i].name, vs)) vtext = src[i].text;
    if (strlen(src[i].name) == fsl && !strncmp(src[i].name, fs, fsl)) ftext = src[i].text;
    if (!strcmp(src[i].name, "g00_head.glsl")) head = src[i].text;
  }
  if (tf) ftext = "void main() {}\n";
  if (vtext == NULL || ftext == NULL || head == NULL) {
    snprintf(err, errlen, "shader %s, %s or g00_head.glsl missing", vs, fs);
    return 0;
  }
  a->vparts[0] = header(), a->vparts[1] = head, a->vparts[2] = vtext;
  if (!tf) a->vparts[1] = vtext;
  a->nv = tf ? 3 : 2;
  const char *ext = plus != NULL ? def_ext(plus + 1) : NULL;
  if (ext != NULL) { /* the header's #version line (4.50 for the image modes), the extension, the rest of the header */
    const char *h = header(), *nl = strchr(h, '\n') + 1;
    if (strcmp(plus + 1, "G10_FETCH")) snprintf(a->fhead, sizeof a->fhead, "#version 450 core\n%s%s", ext, nl);
    else snprintf(a->fhead, sizeof a->fhead, "%.*s%s%s", (int)(nl - h), h, ext, nl);
  }
  parts[np++] = ext != NULL ? a->fhead : header();
  parts[np++] = kdef;
  if (!tf) parts[np++] = head;
  for (int i = 0; i < n && np < 68 && !tf && !vbody; i++) /* the libraries, by name: a piece's functions are seen by later ones */
    if (ends(src[i].name, ".glsl") && strcmp(src[i].name, "g00_head.glsl")) parts[np++] = src[i].text;
  parts[np++] = ftext;
  a->np = np;
  return 1;
}

static char *join(const char *const *parts, int n) {
  size_t l = 1;
  for (int i = 0; i < n; i++) l += strlen(parts[i]);
  char *t = malloc(l), *p = t;
  if (t == NULL) return NULL;
  for (int i = 0; i < n; i++) p += strlen(strcpy(p, parts[i]));
  *p = 0;
  return t;
}

int gpu_gl_program_text(const char *vs, const char *fs, int tf, char **vtext, char **ftext, char *err, size_t errlen) {
  Asm a;
  *vtext = *ftext = NULL;
  if (!assemble(&a, vs, fs, tf, err, errlen)) return -1;
  *vtext = join(a.vparts, a.nv);
  *ftext = join(a.parts, a.np);
  return *vtext != NULL && *ftext != NULL ? 0 : -1;
}

/* varyings != NULL: a transform-feedback program (gpu_gl_program_tf) */
static GLuint build_program(const char *vs, const char *fs, const char *const *varyings, int nvar, char *err, size_t errlen) {
  Asm a;
  if (!assemble(&a, vs, fs, varyings != NULL, err, errlen)) return 0;
  GLuint v = compile(GL_VERTEX_SHADER, a.vparts, a.nv, err, errlen, vs);
  if (v == 0) return 0;
  GLuint f = compile(GL_FRAGMENT_SHADER, a.parts, a.np, err, errlen, fs);
  if (f == 0) {
    GL(DeleteShader)(v);
    return 0;
  }
  GLuint p = GL(CreateProgram)();
  uni_forget(); /* p may be the id of a program deleted earlier: its locations go */
  GL(AttachShader)(p, v);
  GL(AttachShader)(p, f);
  if (varyings != NULL) GL(TransformFeedbackVaryings)(p, nvar, varyings, GL_INTERLEAVED_ATTRIBS);
  GL(LinkProgram)(p);
  GL(DeleteShader)(v);
  GL(DeleteShader)(f);
  GLint ok = 0;
  GL(GetProgramiv)(p, GL_LINK_STATUS, &ok);
  if (!ok) {
    char log[2048] = {0};
    GL(GetProgramInfoLog)(p, sizeof log - 1, NULL, log);
    snprintf(err, errlen, "link %s + %s: %s", vs, fs, log);
    GL(DeleteProgram)(p);
    return 0;
  }
  GLuint blk = GL(GetUniformBlockIndex)(p, "GpuU"); /* (a program that reads no field of it has none) */
  if (blk != GL_INVALID_INDEX) GL(UniformBlockBinding)(p, blk, 0);
  return p;
}

/* gpu_gl_uniforms' ring: more buffers than a frame and its two in-betweens draw */
enum { UBO_RING = 32 };
static GLuint ubo[UBO_RING];
static int ubo_next;

void gpu_gl_uniforms(const GpuU *u) {
  GLuint *b = &ubo[ubo_next];
  ubo_next = (ubo_next + 1) % UBO_RING;
  if (*b == 0) GL(GenBuffers)(1, b);
  GL(BindBuffer)(GL_UNIFORM_BUFFER, *b);
  GL(BufferData)(GL_UNIFORM_BUFFER, (GLsizeiptr)sizeof *u, u, GL_STREAM_DRAW);
  GL(BindBuffer)(GL_UNIFORM_BUFFER, 0);
  GL(BindBufferBase)(GL_UNIFORM_BUFFER, 0, *b);
}

void gpu_gl_uniforms_release(void) {
  for (int i = 0; i < UBO_RING; i++)
    if (ubo[i]) GL(DeleteBuffers)(1, &ubo[i]);
  memset(ubo, 0, sizeof ubo);
  ubo_next = 0;
}

/* the programs, one per (vs, fs) pair for the context's life. Apple's OpenGL on Metal keeps the render pipelines it made for a
 * program after the program is deleted: each GPU-path setup (a Resolution, AA or Renderer switch) compiled and deleted five
 * programs and left ~0.6 MB in the driver (M2, 11-01: +28 MB over 100 switches; heap showed ~5 more GLRRenderPipelineState a
 * setup). With BWPORT_GPU_SHADERS (shaders read from files) each call compiles afresh, as before, so an edit shows. */
static struct {
  const char *vs, *fs;
  GLuint p;
} progs[24];

GLuint gpu_gl_program(const char *vs, const char *fs, char *err, size_t errlen) {
  return gpu_gl_program_tf(vs, fs, NULL, 0, err, errlen);
}

GLuint gpu_gl_program_tf(const char *vs, const char *fs, const char *const *varyings, int nvar, char *err, size_t errlen) {
  const char *dir = getenv("BWPORT_GPU_SHADERS");
  if (dir != NULL && dir[0]) return build_program(vs, fs, varyings, nvar, err, errlen);
  int i = 0;
  for (; i < 24 && progs[i].p != 0; i++)
    if (!strcmp(progs[i].vs, vs) && !strcmp(progs[i].fs, fs)) return progs[i].p;
  GLuint p = build_program(vs, fs, varyings, nvar, err, errlen);
  if (p != 0 && i < 24) progs[i].vs = vs, progs[i].fs = fs, progs[i].p = p;
  return p;
}

void gpu_gl_program_done(GLuint p) {
  if (p == 0) return;
  for (int i = 0; i < 24 && progs[i].p != 0; i++)
    if (progs[i].p == p) return; /* kept for the next setup */
  GL(DeleteProgram)(p);
}

void gpu_gl_programs_forget(void) {
  memset(progs, 0, sizeof progs);
  memset(ubo, 0, sizeof ubo); /* (the buffers go with the context) */
}

/* persistent data textures (G11): a texture is allocated once and only grown (TexImage2D with no data), then filled with
 * TexSubImage2D: the whole rows of the data, then the last partial row. Rows past n keep the earlier frames' texels: the
 * shaders read only what they were given. The _diff form keeps a copy of what the texture holds and uploads only the
 * rows that changed. */
typedef struct DataCap {
  GLuint tex;
  int rows;
  unsigned char *shadow; /* _diff: the texture's content, rows x 1024 texels */
} DataCap;
static DataCap data_caps[64];

static DataCap *data_cap(GLuint tex) {
  for (int i = 0; i < 64; i++)
    if (data_caps[i].tex == tex) return &data_caps[i];
  for (int i = 0; i < 64; i++)
    if (data_caps[i].tex == 0) {
      data_caps[i].tex = tex;
      return &data_caps[i];
    }
  return NULL;
}

/* a data texture is being deleted: its record goes with it. A record that stayed kept the texture's id and its
 * shadow copy for ever (64 records, 6 per GpuCore: after ~10 Resolution changes a new texture found none free and was
 * re-specified on every frame; an id the driver handed out again found the old record's row count and shadow) */
void gpu_gl_data_forget(GLuint tex) {
  if (tex == 0) return;
  for (int i = 0; i < 64; i++)
    if (data_caps[i].tex == tex) {
      free(data_caps[i].shadow);
      memset(&data_caps[i], 0, sizeof data_caps[i]);
    }
}

/* binds *tex (made on the first call) with room for `rows` rows; 1 when it was (re)allocated */
static int data_room(GLuint *tex, GLenum ifmt, GLenum fmt, GLenum type, int texel_bytes, int rows, DataCap **capp) {
  int fresh = 0;
  if (*tex == 0) {
    GL(GenTextures)(1, tex);
    GL(BindTexture)(GL_TEXTURE_2D, *tex);
    /* texelFetch needs a complete texture: no mipmaps */
    GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    fresh = 1;
  } else {
    GL(BindTexture)(GL_TEXTURE_2D, *tex);
  }
  DataCap *c = data_cap(*tex);
  *capp = c;
  int have = c != NULL ? c->rows : 0;
  if (c == NULL || fresh || rows > have) {
    int h = rows;
    if (c != NULL && have > 0 && h < have + have / 2) h = have + have / 2; /* grow by half: few reallocations */
    if (h < 1) h = 1;
    GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
    GL(TexImage2D)(GL_TEXTURE_2D, 0, (GLint)ifmt, GPU_DATA_W, h, 0, fmt, type, NULL);
    if (c != NULL) {
      c->rows = h;
      free(c->shadow);
      c->shadow = NULL;
    }
    fresh = 1;
  }
  (void)texel_bytes;
  return fresh;
}

/* the pixel-unpack ring (gpu_gl_unpack_map): 2 buffers, each grown to the largest upload it took */
enum { UNPACK_RING = 2 };
static struct {
  GLuint buf;
  size_t size;
} unpack[UNPACK_RING];
static int unpack_next;

unsigned char *gpu_gl_unpack_map(size_t bytes) {
  if (bytes == 0) return NULL;
  int i = unpack_next;
  unpack_next = (unpack_next + 1) % UNPACK_RING;
  if (unpack[i].buf == 0) GL(GenBuffers)(1, &unpack[i].buf);
  GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, unpack[i].buf);
  if (unpack[i].size < bytes) {
    size_t size = (bytes + 0xFFFF) & ~(size_t)0xFFFF;
    GL(BufferData)(GL_PIXEL_UNPACK_BUFFER, (GLsizeiptr)size, NULL, GL_STREAM_DRAW);
    unpack[i].size = size;
  }
  void *m = GL(MapBufferRange)(GL_PIXEL_UNPACK_BUFFER, 0, (GLsizeiptr)bytes, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  if (m == NULL) GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, 0);
  return m;
}

int gpu_gl_unpack_unmap(void) {
  if (GL(UnmapBuffer)(GL_PIXEL_UNPACK_BUFFER)) return 0;
  GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, 0);
  return -1;
}

void gpu_gl_unpack_done(void) { GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, 0); }

void gpu_gl_unpack_release(void) {
  for (int i = 0; i < UNPACK_RING; i++)
    if (unpack[i].buf) GL(DeleteBuffers)(1, &unpack[i].buf);
  memset(unpack, 0, sizeof unpack);
}

static void data_rows(GLenum fmt, GLenum type, int texel_bytes, const unsigned char *data, int first, int nrows, int ntexels) {
  /* rows first..first+nrows-1 of data (ntexels in all, the last row may be partial) */
  if (nrows <= 0) return;
  const unsigned char *p = data + (size_t)first * GPU_DATA_W * texel_bytes;
  int full = nrows;
  int tail = 0;
  if ((first + nrows) * GPU_DATA_W > ntexels) {
    full = nrows - 1;
    tail = ntexels - (first + nrows - 1) * GPU_DATA_W;
  }
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  if (full > 0) GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, first, GPU_DATA_W, full, fmt, type, p);
  if (tail > 0)
    GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, first + full, tail, 1, fmt, type, p + (size_t)full * GPU_DATA_W * texel_bytes);
}

void gpu_gl_data_tex(GLuint *tex, GLenum ifmt, GLenum fmt, GLenum type, int texel_bytes, const void *data, int n) {
  DataCap *c;
  if (n < 1) n = 1;
  int rows = (n + GPU_DATA_W - 1) / GPU_DATA_W;
  data_room(tex, ifmt, fmt, type, texel_bytes, rows, &c);
  if (data != NULL) data_rows(fmt, type, texel_bytes, data, 0, rows, n);
}

void gpu_gl_data_tex_diff(GLuint *tex, GLenum ifmt, GLenum fmt, GLenum type, int texel_bytes, const void *data, int n) {
  DataCap *c;
  if (n < 1) n = 1;
  int rows = (n + GPU_DATA_W - 1) / GPU_DATA_W;
  int fresh = data_room(tex, ifmt, fmt, type, texel_bytes, rows, &c);
  if (data == NULL) return;
  size_t rowb = (size_t)GPU_DATA_W * texel_bytes, total = (size_t)n * texel_bytes;
  if (c == NULL) {
    data_rows(fmt, type, texel_bytes, data, 0, rows, n);
    return;
  }
  if (c->shadow == NULL) {
    c->shadow = calloc((size_t)c->rows, rowb);
    if (c->shadow == NULL) {
      data_rows(fmt, type, texel_bytes, data, 0, rows, n);
      return;
    }
    fresh = 1;
  }
  if (fresh) { /* the whole thing; the shadow holds the data (zero past n) */
    memset(c->shadow, 0, (size_t)c->rows * rowb);
    memcpy(c->shadow, data, total);
    data_rows(fmt, type, texel_bytes, data, 0, rows, n);
    return;
  }
  int r = 0;
  while (r < rows) {
    size_t off = (size_t)r * rowb, len = off + rowb <= total ? rowb : total - off;
    if (memcmp(c->shadow + off, (const unsigned char *)data + off, len) == 0) {
      r++;
      continue;
    }
    int r1 = r;
    while (r1 + 1 < rows) {
      size_t o2 = (size_t)(r1 + 1) * rowb, l2 = o2 + rowb <= total ? rowb : total - o2;
      if (memcmp(c->shadow + o2, (const unsigned char *)data + o2, l2) == 0) break;
      r1++;
    }
    size_t end = (size_t)(r1 + 1) * rowb < total ? (size_t)(r1 + 1) * rowb : total;
    memcpy(c->shadow + off, (const unsigned char *)data + off, end - off);
    data_rows(fmt, type, texel_bytes, data, r, r1 - r + 1, n);
    r = r1 + 1;
  }
}
