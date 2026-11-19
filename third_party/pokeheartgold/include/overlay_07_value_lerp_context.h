#ifndef POKEHEARTGOLD_OVERLAY_07_VALUE_LERP_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_VALUE_LERP_CONTEXT_H

// sizeof == 0x14 on the DS
typedef struct ValueLerpContext {
    s32 value;
    s32 data[4];
} ValueLerpContext;

#endif // POKEHEARTGOLD_OVERLAY_07_VALUE_LERP_CONTEXT_H
