#ifndef POKEHEARTGOLD_OVERLAY_07_AFTERIMAGE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_AFTERIMAGE_CONTEXT_H

#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

typedef BOOL (*XYTransformFunc)(XYTransformContext *);

// sizeof == 0xB0 on the DS
typedef struct AfterimageContext {
    s16 x;
    s16 y;
    XYTransformContext transforms[4];
    XYTransformFunc transformFunc;
    struct ManagedSprite *sprites[4];
    u16 delay;
    u16 delayCounter;
    u8 count;
    u8 activeCount;
    u8 mode;
    u8 unk_AF;
} AfterimageContext;

#endif // POKEHEARTGOLD_OVERLAY_07_AFTERIMAGE_CONTEXT_H
