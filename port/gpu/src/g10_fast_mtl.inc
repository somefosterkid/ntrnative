/* port/gpu piece G10 on Metal: the fast 3D renderer's pass (docs/gpu-hwraster.md), in pass R's place; included by gpu_mtl.m (its
 * pipelines, data and passes). One render pass into pass R's layer: the rear plane (g10_clear.frag), the polygons' triangles
 * (g10_fast.vert/.frag: pass R's per-pixel rules on the attachment's state, programmable blending; the edge flag and coverage
 * from the polygons' edge functions). With 4 samples one fragment per output pixel draws its four samples (g10_fast4.vert/.frag,
 * g10_clear4.frag: the coverage tested in the fragment on the 2N grid, the polygon shaded once) straight into pass R's four
 * layers (the attachments, g00_raster.frag's r_out form with the edge and alike bits kept up to date by each fragment) and a fifth
 * attachment with the state those drop (the translucent IDs and stencil bits, a byte a sample), never stored (2026-11-04: before,
 * four RGBA32UI layers of a grid of their own, 64 bytes a pixel stored, and a pack pass that read them: that is still the GL way,
 * src/g10_fast_gl.c). Pass E, pass P and SMAA then run as for the exact renderer.
 * BWPORT_FAST4 picks the 4-sample form, for A/Bs on the phone (docs/gpu-hwraster.md, "Round four"): direct (the Mac's default, above)
 * or keep (g10_fast4.frag+G10_KEEP: the direct form without discard, the old values written back; the iOS default). */

enum { FAST4_DIRECT, FAST4_KEEP };
static int fast4_mode(void) {
  static int mode = -1;
  if (mode < 0) {
    static const char *const names[] = {"direct", "keep"};
    const char *v = getenv("BWPORT_FAST4");
    /* the default: keep on iOS, direct on the Mac. a test iPhone (A18 Pro, 2026-11-04, 3 rounds each, pinned): pass R's
     * slot at 4 samples direct 5.11 -> keep 4.14 ms (-19 %); the M2 runs direct fastest (keep +6 %) */
    mode = TARGET_OS_IPHONE ? FAST4_KEEP : FAST4_DIRECT;
    for (int i = 0; v != NULL && i < 2; i++)
      if (!strcmp(v, names[i])) mode = i;
    if (v != NULL) fprintf(stderr, "gpu: BWPORT_FAST4=%s: Fast's 4 samples %s\n", v, names[mode]);
  }
  return mode;
}
/* the triangles' fragment program for 4 samples (filt: Filter=bilinear's) */
static const char *fast4_fs(int filt) {
  static const char *const fs[2][2] = {{"g10_fast4.frag", "g10_fast4.frag#1f"},
                                       {"g10_fast4.frag+G10_KEEP", "g10_fast4.frag#1f+G10_KEEP"}};
  return fs[fast4_mode()][filt];
}

struct GpuFastMtl {
  Pipe *tri, *trif, *clr; /* trif: the triangles with Filter=bilinear, made at its first use */
  __strong id<MTLBuffer> v[GPU_RING], pe[GPU_RING]; /* the triangles' vertices, the polygons' edge functions (a ring entry each) */
  __strong id<MTLTexture> aux;                     /* k = 2: the fifth attachment (R32UI at N; memoryless where the GPU has it) */
  int ok;                                          /* 1: made; -1: a pipeline failed (the core draws exact) */
};

/* a render pipeline with its own vertex function: the fragment's bindings as pipe_render's, the vertex buffers hwV, hwU found by
 * name */
static Pipe *pipe_render_vs(const char *vs, const char *fs, int n, const MTLPixelFormat *f, char *err, size_t errlen) {
  char key[48];
  snprintf(key, sizeof key, "%s/%s/%d", vs, fs, (int)f[0]);
  int fresh;
  Pipe *p = pipe_slot(key, &fresh);
  if (p == NULL || !fresh) {
    if (p == NULL) snprintf(err, errlen, "too many pipelines");
    return p != NULL && p->rp != nil ? p : NULL;
  }
  id<MTLFunction> v = function(vs, err, errlen), fn = v ? function(fs, err, errlen) : nil;
  if (fn == nil) return NULL;
  MTLRenderPipelineDescriptor *d = [MTLRenderPipelineDescriptor new];
  d.vertexFunction = v;
  d.fragmentFunction = fn;
  for (int i = 0; i < n; i++) d.colorAttachments[i].pixelFormat = f[i];
  MTLAutoreleasedRenderPipelineReflection refl = nil;
  NSError *e = nil;
  p->rp = [sDev newRenderPipelineStateWithDescriptor:d options:MTLPipelineOptionBindingInfo reflection:&refl error:&e];
  if (p->rp == nil) {
    snprintf(err, errlen, "%s: %s", fs, e.localizedDescription.UTF8String);
    return NULL;
  }
  bindings(p, refl.fragmentBindings);
  p->hwv = p->hwu = p->hwe = -1;
  for (id<MTLBinding> b in refl.vertexBindings) {
    const char *nm = b.name.UTF8String;
    if (b.type == MTLBindingTypeBuffer && !strcmp(nm, "hwV")) p->hwv = (int)b.index;
    if (b.type == MTLBindingTypeBuffer && !strcmp(nm, "hwU")) p->hwu = (int)b.index;
    if (b.type == MTLBindingTypeBuffer && !strcmp(nm, "hwE")) p->hwe = (int)b.index;
  }
  return p;
}

/* the fast renderer's pipelines and target, made at the first call: 1 when it can draw */
int gpu_mtl_fast_ok(GpuCore *g) {
  struct GpuMtl *m = g->mtl;
  if (m == NULL) return 0;
  if (m->fast != NULL) return m->fast->ok > 0;
  @autoreleasepool {
    struct GpuFastMtl *f = m->fast = (struct GpuFastMtl *)calloc(1, sizeof *f);
    if (f == NULL) return 0;
    int k2 = g->k == 2;
    MTLPixelFormat f1 = MTLPixelFormatRGBA32Uint, fs = m->samp.pixelFormat, f5[5] = {fs, fs, fs, fs, MTLPixelFormatR32Uint};
    char err[512] = "";
    int k5 = k2; /* k5: the five attachments of the 4-sample forms */
    f->clr = pipe_render(k5 ? "g10_clear4.frag" : "g10_clear.frag", k5 ? 5 : 1, k5 ? f5 : &f1, err, sizeof err);
    if (f->clr)
      f->tri = pipe_render_vs(k5 ? "g10_fast4.vert" : "g10_fast.vert", k2 ? fast4_fs(0) : "g10_fast.frag", k5 ? 5 : 1,
                              k5 ? f5 : &f1, err, sizeof err);
    if (f->tri && k5) { /* the fifth attachment lives in the tile only: never loaded or stored */
      MTLTextureDescriptor *d = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatR32Uint width:(NSUInteger)g->w
                                                                                     height:(NSUInteger)g->h mipmapped:NO];
      d.storageMode = [sDev supportsFamily:MTLGPUFamilyApple1] ? MTLStorageModeMemoryless : MTLStorageModePrivate;
      d.usage = MTLTextureUsageRenderTarget;
      f->aux = [sDev newTextureWithDescriptor:d];
    }
    f->ok = f->tri != NULL && f->tri->hwv >= 0 && f->tri->hwe >= 0 && (!k5 || f->aux != nil) ? 1
                : -1;
    if (f->ok < 0) fprintf(stderr, "gpu: the fast 3D renderer: %s; the exact renderer draws\n", err[0] ? err : "no vertex buffer");
    return f->ok > 0;
  }
}

static void fast_release(struct GpuMtl *m) {
  struct GpuFastMtl *f = m->fast;
  if (f == NULL) return;
  for (int i = 0; i < GPU_RING; i++) f->v[i] = f->pe[i] = nil;
  f->aux = nil;
  free(f);
  m->fast = NULL;
}

/* n floats from src into *b (grown by half when short) */
static void fast_put(__strong id<MTLBuffer> *b, const float *src, int n) {
  size_t len = (size_t)(n > 0 ? n : 1) * sizeof(float);
  if (*b == nil || (*b).length < len) *b = [sDev newBufferWithLength:len + len / 2 options:MTLResourceStorageModeShared];
  if (n > 0) memcpy((*b).contents, src, (size_t)n * sizeof(float));
}

/* the pass (pass R's slot, ring entry ri; u: pass R's uniforms) */
static void fast_r(GpuCore *g, const GpuPrep *p, int ri, GpuU *u) {
  struct GpuMtl *m = g->mtl;
  struct GpuFastMtl *f = m->fast;
  const GpuFast *fv = &p->fast;
  Pipe *tri = f->tri;
  if (g->opt.filter && g->scale > g->k) { /* Filter=bilinear where it applies: its own program, as pass R's */
    if (f->trif == NULL) {
      MTLPixelFormat f1 = MTLPixelFormatRGBA32Uint, fs = m->samp.pixelFormat, f5[5] = {fs, fs, fs, fs, MTLPixelFormatR32Uint};
      char err[512];
      int k2 = g->k == 2, k5 = k2;
      f->trif = pipe_render_vs(k5 ? "g10_fast4.vert" : "g10_fast.vert", k2 ? fast4_fs(1) : "g10_fast.frag#1f", k5 ? 5 : 1,
                               k5 ? f5 : &f1, err, sizeof err);
      if (f->trif == NULL) {
        fprintf(stderr, "gpu: %s; textures unfiltered\n", err);
        f->trif = f->tri;
      }
    }
    tri = f->trif;
  }
  fast_put(&f->v[ri], fv->tri, fv->ntri * GPU_FAST_V);
  fast_put(&f->pe[ri], fv->pe[0], p->sp.npoly * VC_POLY_MAXV * 4);
  float hu[4] = {(float)(g->w * g->k), (float)(g->h * g->k), 0, 0};
  MTLRenderPassDescriptor *d = [MTLRenderPassDescriptor renderPassDescriptor];
  int k5 = g->k == 2;
  for (int i = 0; i < (k5 ? 4 : 1); i++) { /* pass R's layers: the four samples' with 4 samples */
    d.colorAttachments[i].texture = m->samp;
    d.colorAttachments[i].slice = (NSUInteger)i;
    d.colorAttachments[i].loadAction = MTLLoadActionDontCare; /* (the clear draw writes every pixel) */
    d.colorAttachments[i].storeAction = MTLStoreActionStore;
  }
  if (k5) {
    d.colorAttachments[4].texture = f->aux;
    d.colorAttachments[4].loadAction = MTLLoadActionDontCare;
    d.colorAttachments[4].storeAction = MTLStoreActionDontCare;
  }
  id<MTLRenderCommandEncoder> e = [cb() renderCommandEncoderWithDescriptor:d];
  e.label = @"g10_fast";
  [e setRenderPipelineState:f->clr->rp];
  if (f->clr->ubo >= 0) [e setFragmentBytes:u length:sizeof *u atIndex:(NSUInteger)f->clr->ubo];
  for (int i = 0; i < f->clr->nsmp; i++) [e setFragmentSamplerState:sNearest atIndex:(NSUInteger)f->clr->smpi[i]];
  ftex(e, f->clr, "uClrImg", m->clr[ri].tex);
  [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
  if (fv->ntri > 0 && !(sAblate & GPU_ABL_R)) { /* ablate=r: no triangle drawn (the rear plane and the stores stay) */
    [e setRenderPipelineState:tri->rp];
    if (tri->hwu >= 0) [e setVertexBytes:hu length:sizeof hu atIndex:(NSUInteger)tri->hwu];
    [e setVertexBuffer:f->v[ri] offset:0 atIndex:(NSUInteger)tri->hwv];
    if (tri->ubo >= 0) [e setFragmentBytes:u length:sizeof *u atIndex:(NSUInteger)tri->ubo];
    [e setVertexBuffer:f->pe[ri] offset:0 atIndex:(NSUInteger)tri->hwe];
    for (int i = 0; i < tri->nsmp; i++) [e setFragmentSamplerState:sNearest atIndex:(NSUInteger)tri->smpi[i]];
    ftex(e, tri, "uPoly", m->poly[ri].tex);
    ftex(e, tri, "uTex", m->tex[m->tr].tex);
    ftex(e, tri, "uTexPal", m->pal[m->tr].tex);
    [e drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:(NSUInteger)fv->ntri];
  }
  [e endEncoding];
}
