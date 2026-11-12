#include "types.h"
#include "nnsys/g2d.h"

// Two tables of the character image loaders (1D mapping, 2D mapping), indexed by the mapping layer. Nothing in Black's
// ROM reads them: the functions that did were dropped by the link. They sit between g3d_system.c's .rodata and
// arc_util.c's, after g3d_system.c's 36-byte DEFAULT_LOOKAT, in an object of their own: the ROM names no such file, and
// as function-local statics of dropped code they come after the shared run of whichever file had them. The names are
// the ROM's addresses.
typedef void (*G2DImageLoadFunc)(const NNSG2dCharacterData *data, u32 baseAddr, NNS_G2D_VRAM_TYPE type,
                                 NNSG2dImageProxy *proxy);

const G2DImageLoadFunc data_020a134c[2] = { NNS_G2dLoadImage1DMapping, NNS_G2dLoadImage2DMapping };
const G2DImageLoadFunc data_020a1354[2] = { NNS_G2dLoadImage1DMapping, NNS_G2dLoadImage2DMapping };
