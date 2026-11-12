/* port/bridge/gpu_bridge_mtl.h: gpu_bridge.c's calls into its Metal half (gpu_bridge_mtl.m, Apple builds with BR_METAL) */
#ifndef GPU_BRIDGE_MTL_H
#define GPU_BRIDGE_MTL_H

#include <stdint.h>

/* the names libntr gets for a Metal picture: the core's texture names from BR_MTL_BASE up, a CPU picture as BR_MTL_UPLOADED */
enum { BR_MTL_BASE = 0x10000, BR_MTL_UPLOADED = 0xFFFF };

void br_mtl_use_present(void);         /* the core on the present's device and queue (before gpu_create) */
void gpu_mtl_ablate(unsigned mask);    /* port/gpu/src/gpu_mtl.h's GPU_ABL_* bits (test-only ablations, off = 0) */
const char *br_mtl_device_name(void);
void br_mtl_commit(int slot);          /* the frame's command buffer committed; its GPU time counted under slot (a pass, GPU_PASS_N) */
double br_mtl_finish(void);            /* committed and waited for; its GPU ms */
void br_mtl_counted(double ms);        /* a command buffer the core committed and waited for itself (timing mode), its GPU ms */
unsigned br_mtl_take(double *ms);      /* ms[GPU_PASS_N + 1]: the GPU ms finished since the last call; the command buffers' count */
void br_mtl_upload(const uint8_t *rgba, int w, int h); /* a CPU picture to the present (bwport_mtl_upload) */
void *br_mtl_tex_of(unsigned name);    /* libntr's name back to the id<MTLTexture> (NULL: the picture uploaded last) */

#endif
