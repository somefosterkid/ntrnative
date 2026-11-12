// What SPL's files share among themselves. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/spl/src/spl_internal.h, with Black's symbol names; pokebw2's names are in the comments.
#ifndef BWPORT_SPL_INTERNAL_H
#define BWPORT_SPL_INTERNAL_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "spl/spl.h"

// A linear congruential generator, its high bits the random value (gSPLRandomState)
extern u32 data_02146a8c;

static inline u32 SPLRandom_Next(void) {
    data_02146a8c = data_02146a8c * 0x5eedf715 + 0x1b0cb173;
    return data_02146a8c;
}

// The top bits of the next value, as a signed fixed-point number: a fraction of [-1, 1) for 24 bits
static inline fx32 SPLRandom_Fx32(u32 bits) {
    return (fx32)SPLRandom_Next() >> (32 - bits);
}

// A random value in [-range, range), range an integer or a fixed-point number. A macro, as the library reads range
// after drawing the number
#define SPLRandom_Range(range) (((range) * (s32)(SPLRandom_Next() >> 23) - ((range) << 8)) >> 8)

// value, more or less by a random part of up to variance out of 255
#define SPLRandom_Vary(value, variance)                                                                               \
    (((value) * (255 + (variance) - (((variance) * (s32)(SPLRandom_Next() >> 24)) >> 7))) >> 8)

// A random unit vector, or one in the xy plane (SPLRandom_VecFx32, SPLRandom_VecFx32_XY)
void func_02058478(VecFx32 *vec);
void func_0205841c(VecFx32 *vec);

// Emit the emitter's particles for this frame, or a particle's children, taking them from the free list
// (SPLEmitter_EmitParticles, SPLEmitter_EmitChildren)
void func_020569a4(SPLEmitter *emitter, SPLList *freeList);
void func_02056684(SPLParticle *parent, SPLEmitter *emitter, SPLList *freeList);

// SPLEmitter_Init, SPLEmitter_Update, and SPLEmitter_Draw, which draws the manager's draw emitter
void func_02053a60(SPLEmitter *emitter, SPLResource *resource, const VecFx32 *pos);
void func_02053218(SPLManager *mgr, SPLEmitter *emitter);
void func_02052fc8(SPLManager *mgr);

// The animations, given the particle's life rate from 0 to 255
typedef void (*SPLAnimFunc)(SPLParticle *particle, SPLResource *resource, int lifeRate);

// SPLAnim_Scale, Color, Alpha, Texture, ChildScale and ChildAlpha
void func_02057e68(SPLParticle *particle, SPLResource *resource, int lifeRate);
void func_02057c9c(SPLParticle *particle, SPLResource *resource, int lifeRate);
void func_02057bc0(SPLParticle *particle, SPLResource *resource, int lifeRate);
void func_02057b5c(SPLParticle *particle, SPLResource *resource, int lifeRate);
void func_02057b10(SPLParticle *particle, SPLResource *resource, int lifeRate);
void func_02057ab8(SPLParticle *particle, SPLResource *resource, int lifeRate);

// Draw a particle, or a child, of the manager's draw emitter
typedef void (*SPLDrawFunc)(SPLManager *mgr, SPLParticle *particle);

// SPLDraw_Billboard, DirBillboard, Polygon and DirPolygon, then the same for children
void func_02055fd4(SPLManager *mgr, SPLParticle *particle);
void func_02055498(SPLManager *mgr, SPLParticle *particle);
void func_02054a68(SPLManager *mgr, SPLParticle *particle);
void func_0205429c(SPLManager *mgr, SPLParticle *particle);
void func_02055b80(SPLManager *mgr, SPLParticle *particle);
void func_02054db8(SPLManager *mgr, SPLParticle *particle);
void func_02054718(SPLManager *mgr, SPLParticle *particle);
void func_02053e1c(SPLManager *mgr, SPLParticle *particle);

#endif // BWPORT_SPL_INTERNAL_H
