/* port/vcore piece 10: master brightness and the display modes (docs/vcore.md, piece 10). OWNED BY PIECE 10. */
#ifndef VC_P10_H
#define VC_P10_H

#include "vc_types.h"

typedef struct VC_P10 {
  int unused;
} VC_P10;

/* MASTER_BRIGHT value mb (mode bits 14-15, factor bits 0-4) applied to one RGB6 pixel */
u32 vc_p10_bright(u32 px, u16 mb);

void vc_p10_vram_line(const u8 *bank, int y, u32 *dst);

#endif
