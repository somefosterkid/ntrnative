/* port/gpu piece G12 (texture cache, docs/gpu.md): the public objects of include/gpu.h's texture cache, shared by the CPU half
 * (g12_tcframe.c) and the GL half (g12_tcgl.c). OWNED BY G12.
 *
 * GpuTexCache: g12_texcache.c's directory behind a mutex, the upload queue (one G12Batch per frame that uploads or grows, in
 * frame order: the render of frame F applies every batch up to F) and the generation (bumped by each restart: a GL side that
 * cannot hold the cache's layers, the GL side recreated after another took from it). GpuTcFrame: one frame's map of texture
 * key -> layer (the frame's ready entries), looked up per polygon of the prep or an in-between picture on the GL thread. */
#ifndef GPU_G12_TCFRAME_H
#define GPU_G12_TCFRAME_H

#include <pthread.h>

#include "gpu.h"
#include "g12_texcache.h"

/* one frame's uploads, copied out of g12_frame's (its staging lives until the next g12_frame only) */
typedef struct G12Batch {
  struct G12Batch *next;
  u32 frame;
  u32 grow_cap[G12_NBUCKET];
  u32 nup;
  G12Upload *up;
  u8 *bytes;
} G12Batch;

enum { G12_QUEUE_MAX = 240 }; /* batches no render took: past this the cache starts again (a GL side that stopped drawing) */

struct GpuTexCache {
  pthread_mutex_t mu;
  G12Cache *c;
  u32 gen;     /* the generation (frames carry theirs: an older one draws without the cache) */
  u32 owner;   /* the GL side (G12Gl's serial) that took from this generation, 0 = none yet */
  G12Batch *head, *tail;
  u32 nbatch;
  u32 log_every, nframe; /* BWPORT_GPU_TCLOG */
  int mips;              /* the layers come with their mip chains (G12Options.mips) */
};

enum { G12_MAP_SLOTS = 4096 }; /* a power of two over twice VC_MAX_POLYS */

struct GpuTcFrame {
  u32 frame, gen; /* the cache's frame count and generation; frame 0 = never filled */
  u32 nready;     /* keys in the map */
  u64 key[G12_MAP_SLOTS];
  u32 val[G12_MAP_SLOTS]; /* 1 | bucket << 1 | layer << 16 */
};

/* the map's value of key (0: not ready this frame) */
u32 g12_tcf_find(const GpuTcFrame *f, u64 key);
/* the queue's batches freed (under c->mu) */
void g12_queue_clear(GpuTexCache *c);
/* a new generation: the directory forgotten, the queue cleared, nobody owns it (under c->mu) */
void g12_restart(GpuTexCache *c);

#endif
