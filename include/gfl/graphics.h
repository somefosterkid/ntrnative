// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/graphics.h, the lines in use so far.
#ifndef BWPORT_GFL_GRAPHICS_H
#define BWPORT_GFL_GRAPHICS_H

#include "types.h"
#include "nitro/gx.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

struct G3DTextDrawResource {
    u16 unk0;
    u16 unk2;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
};

BOOL G3DTextDraw_CreateResource(void *texture, const char *texName, void *a2, const char *plName, const StrBuf *text, u16 a5,
                                u16 a6, u16 color, HeapID heapId, G3DTextDrawResource *resource);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 func_ov010_0216fc4c(void);

#endif // BWPORT_GFL_GRAPHICS_H
