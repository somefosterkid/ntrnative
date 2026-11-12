#ifndef BWPORT_FIELD_FIELD_3DBG_H
#define BWPORT_FIELD_FIELD_3DBG_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

// field_3dbg.c (pokebw2's ov036 field_3dbg.c): a textured quad drawn behind the field, at a distance from the camera
typedef struct Field3DBg Field3DBg;

// The quad's size and distance, and its polygon ID
typedef struct Field3DBgInit {
    u16 width;
    u16 height;
    fx32 distance;
    u8 polygonId;
} Field3DBgInit;

// The quad's texture: a file of the archive, its texture parameters and its alpha. The weathers' data holds two
typedef struct Field3DBgRes {
    u16 fileId;
    u8 texParams[6];
    u8 alpha;
    u8 unk09[3];
} Field3DBgRes;

Field3DBg *func_ov021_021a15a4(const Field3DBgInit *init, u32 heapId);
void func_ov021_021a15d4(Field3DBg *bg);
void func_ov021_021a15f4(Field3DBg *bg);
// Uploads a texture loaded since the last call
void func_ov021_021a1670(Field3DBg *bg);
// Frees the texture
void func_ov021_021a1680(Field3DBg *bg);
// Loads the texture, in place of the last one
void func_ov021_021a1698(Field3DBg *bg, ArcTool *arc, const Field3DBgRes *res, HeapID heapId);
void func_ov021_021a16d4(Field3DBg *bg, u32 enabled);
void func_ov021_021a16d8(Field3DBg *bg, u8 polygonId);
// Where the texture starts, in texels, and how it is scaled
void func_ov021_021a16dc(Field3DBg *bg, s32 texX);
void func_ov021_021a16e0(Field3DBg *bg, s32 texY);
void func_ov021_021a16e4(Field3DBg *bg, fx32 scaleX);
void func_ov021_021a16e8(Field3DBg *bg, fx32 scaleY);

#endif
