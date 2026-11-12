// Included first into each lib/nnsys C file the PC build compiles (port/meson.build's nnsys library, -include, after
// port_prefix.h), never into the game's files: the member names pokebw2's lib/nitro gives libntr's structs, as plain
// renames, and the DS registers lib/nnsys writes itself.
#ifndef BWPORT_NNSYS_LIB_H
#define BWPORT_NNSYS_LIB_H

#include <nitro.h>

// GXOamAttr (libntr nitro/gx/g2_oam.h): attr2's character name bits, and the fourth halfword (an affine parameter
// in every fourth OAM entry)
#define charName charNo
#define affineParam _3

// The geometry FIFO and the command ports lib/nnsys writes (g3d/gecom.c, sbc.c, g2d_OamSoftwareSpriteDraw.c): on libntr
// plain variables, here words of port/src/nnsys_g3stage.c's command list, which goes to libntr's simulator before its
// next geometry command or read. A port write is its command and, for those that take one, its parameter
volatile u32 *PORT_NnsG3Fifo(void);
volatile u32 *PORT_NnsG3Port(u32 op, u32 numParams);
void PORT_NnsCpuSend32(const void *src, volatile void *dest, u32 size);

#undef reg_G3X_GXFIFO
#define reg_G3X_GXFIFO (*PORT_NnsG3Fifo())
#undef reg_G3_MTX_MODE
#define reg_G3_MTX_MODE (*PORT_NnsG3Port(G3OP_MTX_MODE, 1))
#undef reg_G3_MTX_PUSH
#define reg_G3_MTX_PUSH (*PORT_NnsG3Port(G3OP_MTX_PUSH, 0))
#undef reg_G3_MTX_POP
#define reg_G3_MTX_POP (*PORT_NnsG3Port(G3OP_MTX_POP, 1))
#undef reg_G3_MTX_STORE
#define reg_G3_MTX_STORE (*PORT_NnsG3Port(G3OP_MTX_STORE, 1))
#undef reg_G3_MTX_RESTORE
#define reg_G3_MTX_RESTORE (*PORT_NnsG3Port(G3OP_MTX_RESTORE, 1))
#undef reg_G3_MTX_IDENTITY
#define reg_G3_MTX_IDENTITY (*PORT_NnsG3Port(G3OP_MTX_IDENTITY, 0))
#undef reg_G3_TEXIMAGE_PARAM
#define reg_G3_TEXIMAGE_PARAM (*PORT_NnsG3Port(G3OP_TEXIMAGE_PARAM, 1))
// a send to the FIFO appends its words (to any other address it is libntr's)
#define MIi_CpuSend32(src, dest, size) PORT_NnsCpuSend32((src), (dest), (size))

#endif
