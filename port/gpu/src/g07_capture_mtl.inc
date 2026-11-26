/* port/gpu: display capture at N x on Metal (include/g07_capture.h, shaders/g07_capture.frag; docs/gpu.md "Display capture"),
 * included by gpu_mtl.m. The GL side's steps (src/g07_capture.c) in the frame's command buffer: source A (pass C again for
 * engine A without master brightness, gpu_mtl_capsrc), the capture into a scratch target and a blit into its bank's rows, and
 * display mode 2's rows drawn again from the bank (loading pass C's output, as GL draws over it). The uniforms are block GpuK. */

struct GpuMtlK {
  int scale;
  Pipe *pcap, *pshow;
  __strong id<MTLTexture> bank[4]; /* 256 N x 256 N R16Uint, made on first use */
  unsigned n_bank[4];              /* their names (gpu_mtl_k_bank; 0: none yet) */
  __strong id<MTLTexture> scratch; /* 256 N x 192 N R16Uint: the capture before it is copied into its bank */
  __strong id<MTLTexture> none;    /* 1 x 1 RGBA8Uint: k3d when there is no 3D layer */
  MData b1x[GPU_RING];             /* source B's bank as the game left it, R16Uint 256 x 256 (a ring: earlier frames read theirs) */
  __strong id<MTLCommandBuffer> last[GPU_RING];
  int cur;
};

GpuMtlK *gpu_mtl_k_create(int scale, char *err, size_t errlen) {
  @autoreleasepool {
    if (dev_init(err, errlen) != 0) return NULL;
    MTLPixelFormat fc = MTLPixelFormatR16Uint, fs = MTLPixelFormatRGBA8Unorm;
    Pipe *pc = pipe_render("g07_capture.frag", 1, &fc, err, errlen);
    Pipe *ps = pc != NULL ? pipe_render("g07_capture.frag#1+K_SHOW", 1, &fs, err, errlen) : NULL;
    if (pc == NULL || ps == NULL) return NULL;
    if (pc->kub < 0 || ps->kub < 0) {
      snprintf(err, errlen, "g07_capture.frag: no gK buffer");
      return NULL;
    }
    GpuMtlK *k = (GpuMtlK *)calloc(1, sizeof *k);
    if (k == NULL) {
      snprintf(err, errlen, "out of memory");
      return NULL;
    }
    k->scale = scale;
    k->pcap = pc;
    k->pshow = ps;
    k->scratch = target(fc, VC_W * scale, VC_H * scale, 0);
    MTLTextureDescriptor *d = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Uint width:1 height:1
                                                                             mipmapped:NO];
    d.usage = MTLTextureUsageShaderRead;
    k->none = [sDev newTextureWithDescriptor:d];
    return k;
  }
}

void gpu_mtl_k_destroy(GpuMtlK *k) {
  @autoreleasepool {
    if (k == NULL) return;
    for (int i = 0; i < GPU_RING; i++) {
      wait_cb(k->last[i]);
      k->last[i] = nil;
      mdata_free(&k->b1x[i]);
    }
    for (int b = 0; b < 4; b++) name_free(k->n_bank[b]), k->bank[b] = nil;
    k->scratch = k->none = nil;
    free(k);
  }
}

static id<MTLTexture> k_bank(GpuMtlK *k, int b) {
  if (k->bank[b] == nil) k->bank[b] = target(MTLPixelFormatR16Uint, VC_W * k->scale, 256 * k->scale, 0);
  return k->bank[b];
}
unsigned gpu_mtl_k_bank(GpuMtlK *k, int b) {
  @autoreleasepool {
    if (k->n_bank[b] == 0) k->n_bank[b] = name_new(k_bank(k, b));
    return k->n_bank[b];
  }
}

/* a render pass of p into t, rows y0 .. y0 + h - 1 (load: keep what the pass does not write) */
static id<MTLRenderCommandEncoder> k_pass(id<MTLTexture> t, int load, int y0, int w, int h, const Pipe *p, const GpuK *u) {
  MTLRenderPassDescriptor *d = [MTLRenderPassDescriptor renderPassDescriptor];
  d.colorAttachments[0].texture = t;
  d.colorAttachments[0].loadAction = load ? MTLLoadActionLoad : MTLLoadActionDontCare;
  d.colorAttachments[0].storeAction = MTLStoreActionStore;
  pt_render(d, p->key);
  id<MTLRenderCommandEncoder> e = [cb() renderCommandEncoderWithDescriptor:d];
  e.label = @(p->key);
  [e setRenderPipelineState:p->rp];
  [e setViewport:(MTLViewport){0, (double)y0, (double)w, (double)h, 0, 1}];
  [e setFragmentBytes:u length:sizeof *u atIndex:(NSUInteger)p->kub];
  for (int i = 0; i < p->nsmp; i++) [e setFragmentSamplerState:sNearest atIndex:(NSUInteger)p->smpi[i]];
  return e;
}

int gpu_mtl_k_capture(GpuMtlK *k, const GpuK *u, int bank, int bankB, unsigned texA, unsigned tex3d, const uint8_t *bank1x,
                      int h, int woff) {
  @autoreleasepool {
    const int N = k->scale;
    id<MTLTexture> dst = k_bank(k, bank), srcB = k_bank(k, bankB);
    id<MTLTexture> ta = (__bridge id<MTLTexture>)gpu_mtl_texture(texA), t3 = (__bridge id<MTLTexture>)gpu_mtl_texture(tex3d);
    if (ta == nil) return -1;
    if (bank1x != NULL) {
      k->cur = (k->cur + 1) % GPU_RING;
      wait_cb(k->last[k->cur]);
      mdata_put(&k->b1x[k->cur], MTLPixelFormatR16Uint, 2, 256, bank1x, 256 * 256);
    }
    id<MTLTexture> t1 = k->b1x[k->cur].tex != nil ? k->b1x[k->cur].tex : srcB; /* (unread without a bank: kHaveB 0) */
    id<MTLRenderCommandEncoder> e = k_pass(k->scratch, 0, 0, VC_W * N, h * N, k->pcap, u);
    ftex(e, k->pcap, "kSrcA", ta);
    ftex(e, k->pcap, "k3d", t3 != nil ? t3 : k->none);
    ftex(e, k->pcap, "kBank", srcB);
    ftex(e, k->pcap, "kBank1x", t1);
    pass_end(e);
    /* into the bank's rows woff .. woff + h - 1, wrapping at 256 */
    int first = h < 256 - woff ? h : 256 - woff;
    id<MTLBlitCommandEncoder> b = [cb() blitCommandEncoder];
    [b copyFromTexture:k->scratch sourceSlice:0 sourceLevel:0 sourceOrigin:MTLOriginMake(0, 0, 0)
            sourceSize:MTLSizeMake((NSUInteger)(VC_W * N), (NSUInteger)(first * N), 1) toTexture:dst destinationSlice:0
      destinationLevel:0 destinationOrigin:MTLOriginMake(0, (NSUInteger)(woff * N), 0)];
    if (first < h)
      [b copyFromTexture:k->scratch sourceSlice:0 sourceLevel:0 sourceOrigin:MTLOriginMake(0, (NSUInteger)(first * N), 0)
              sourceSize:MTLSizeMake((NSUInteger)(VC_W * N), (NSUInteger)((h - first) * N), 1) toTexture:dst destinationSlice:0
        destinationLevel:0 destinationOrigin:MTLOriginMake(0, 0, 0)];
    [b endEncoding];
    k->last[k->cur] = cb();
    return 0;
  }
}

int gpu_mtl_k_show(GpuMtlK *k, unsigned out, const GpuK *u, int bank) {
  @autoreleasepool {
    id<MTLTexture> t = (__bridge id<MTLTexture>)gpu_mtl_texture(out);
    if (t == nil) return -1;
    id<MTLRenderCommandEncoder> e = k_pass(t, 1, u->y0, VC_W * k->scale, VC_H * k->scale, k->pshow, u);
    ftex(e, k->pshow, "kBank", k_bank(k, bank));
    pass_end(e);
    return 0;
  }
}

unsigned gpu_mtl_capsrc(GpuMtlC *c, const uint32_t *stage, const uint32_t *src, unsigned serial, int plan, unsigned tex3d,
                        unsigned smaa, unsigned capbank, const GpuU *u) {
  @autoreleasepool {
    c_upload(c, stage, src, serial); /* (pass C reused for this frame did not) */
    if (c->stage[c->cur].buf == nil) return 0;
    if (c->cap == nil) {
      c->cap = target(MTLPixelFormatRGBA8Unorm, c->w, VC_H * c->scale, 0);
      c->n_cap = name_new(c->cap);
    }
    id<MTLTexture> t3 = (__bridge id<MTLTexture>)gpu_mtl_texture(tex3d), ts = (__bridge id<MTLTexture>)gpu_mtl_texture(smaa);
    if (plan) {
      id<MTLRenderCommandEncoder> e = pass_begin(1, &c->plan, NULL, c->pplan, u);
      ftex(e, c->pplan, "cStage", c->stage[c->cur].tex);
      pass_end(e);
    }
    id<MTLRenderCommandEncoder> e = pass_begin(1, &c->cap, NULL, c->pipe, u);
    ftex(e, c->pipe, "cPlan", c->plan);
    ftex(e, c->pipe, "cSrc1x", c->src[c->cur].tex);
    ftex(e, c->pipe, "c3d", t3 != nil ? t3 : c->stage[c->cur].tex);
    ftex(e, c->pipe, "cSmaa", t3 != nil && ts != nil ? ts : c->none);
    id<MTLTexture> tk = (__bridge id<MTLTexture>)gpu_mtl_texture(capbank);
    ftex(e, c->pipe, "cCapBank", tk != nil ? tk : c->src[c->cur].tex);
    pass_end(e);
    c->last[c->cur] = cb();
    return c->n_cap;
  }
}
