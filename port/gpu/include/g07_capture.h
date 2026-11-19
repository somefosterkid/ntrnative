/* port/gpu: display capture at N x (docs/gpu.md "Display capture"; shaders/g07_capture.frag), GL and Metal.
 *
 * The four VRAM banks A-D at N x: R16UI textures 256 N x 256 N (BGR555 | alpha bit 15, bank row r = DS rows r N .. r N + N - 1).
 * g07cap_capture writes a frame's DISPCAPCNT capture into its bank's texture at N x; g07cap_show draws engine A's half of
 * pass C's output again from the bank it shows (display mode 2). The caller (port/bridge/gpu_bridge.c) says which bank rows a
 * texture holds validly (the 256-bit row masks): it keeps the 1x bytes the game reads and decides when a row is stale. */
#ifndef G07_CAPTURE_H
#define G07_CAPTURE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct G07Cap G07Cap;

/* scale 2..GPU_MAX_SCALE; a current GL context with the functions loaded (gpu_create); NULL on failure (the reason in err) */
G07Cap *g07cap_create(int scale, char *err, size_t errlen);
/* the same on the Metal core (gpu_mtl.m; texture names as gpu_mtl_texture's); NULL in a build without Metal */
G07Cap *g07cap_create_metal(int scale, char *err, size_t errlen);
void g07cap_destroy(G07Cap *k);
int g07cap_scale(const G07Cap *k);

/* the capture of DISPCAPCNT cnt (enabled, size 1-3: 256 pixels wide) into its bank's texture, rows ((cnt >> 18) & 3) * 64 on
 * (mod 256). dispcnt: engine A's DISPCNT (source B's bank, the read offset's rule). texA: engine A's picture without master
 * brightness at N x (g07_compose_capsrc); tex3d / sc3d: the 3D layer and its scale (0: none) for source A = 3D. bank1x:
 * source B's bank (DISPCNT bits 18-19) as the game left it (128 KB), NULL when source B is not a bank in LCDC mode; rowsB:
 * the rows of that bank's texture that hold it at N x (the others are read from bank1x). 0 on success, else -(GL error). */
int g07cap_capture(G07Cap *k, uint32_t cnt, uint32_t dispcnt, unsigned texA, unsigned tex3d, int sc3d, const uint8_t *bank1x,
                   const uint32_t rowsB[8]);

/* engine A's half of out (pass C's output texture, 256 N x 384 N; atop: engine A is the top screen) drawn from bank's
 * texture where rows says (bit y: DS row y), with master brightness `bright`; the other rows are left as they are. */
int g07cap_show(G07Cap *k, unsigned out, int atop, int bank, unsigned bright, const uint32_t rows[8]);

#ifdef __cplusplus
}
#endif

#endif
