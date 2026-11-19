#ifndef POKEHEARTGOLD_TRANSFORM_DATA_H
#define POKEHEARTGOLD_TRANSFORM_DATA_H

// One PokemonAnim transform slot (PokemonAnim.transforms[4]); func is run every frame by sub_020170FC.

struct PokemonAnim;

typedef struct TransformData {
    BOOL active;
    int vars[8];
    int *transformMemberPtr;
    int *animMemberPtr;
    u8 calcType;
    u8 startDelay;
    u8 pad_2E[0x2];
    int originalValue;
    int dummy_34;
    int dummy_38;
    int offsetX;
    int offsetY;
    int scaleX;
    int scaleY;
    int rotationZ;
    void (*func)(struct TransformData *transform, struct PokemonAnim *monAnim);
} TransformData;

// sizeof(TransformData) == 0x54

#endif // POKEHEARTGOLD_TRANSFORM_DATA_H
