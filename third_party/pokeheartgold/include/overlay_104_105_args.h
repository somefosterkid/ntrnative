#ifndef POKEHEARTGOLD_OVERLAY_104_105_ARGS_H
#define POKEHEARTGOLD_OVERLAY_104_105_ARGS_H

#include "global.h"

#include "field/model_attributes.h"

// Arguments of the ov104 app (sub_0203FC68, launched by the task of sub_020977CC). DS 0x8.
// lights is the area light state the app applies to the global 3D state; it points into the launching task's environment.
typedef struct Ov104AppArgs {
    u8 mode;
    u8 gender;
    u8 timeOfDay;
    ModelAttributes *lights; // DS 0x4
} Ov104AppArgs;

// Arguments of the ov105 app (sub_0203FC90, launched by the task of sub_020979A8). DS 0x8.
typedef struct Ov105AppArgs {
    u8 mode;
    u8 timeOfDay;
    ModelAttributes *lights; // DS 0x4
} Ov105AppArgs;

#endif // POKEHEARTGOLD_OVERLAY_104_105_ARGS_H
