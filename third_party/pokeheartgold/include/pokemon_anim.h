#ifndef POKEHEARTGOLD_POKEMON_ANIM_H
#define POKEHEARTGOLD_POKEMON_ANIM_H

#include "constants/heap.h"

#include "transform_data.h"

// PokemonAnim: one running Pokemon animation script (narc 90), PokepicAnimManager.anims[animCount].
// DS layout checked with libclang -m32; sizeof(PokemonAnim) == 0x1D0.

struct Pokepic;
struct SysTask;

typedef struct PokemonAnim {
    struct Pokepic *sprite;    // 0x00
    struct SysTask *task;      // 0x04
    void *scriptData;          // 0x08 the narc member, freed when the script ends
    u32 *scriptPtr;            // 0x0C the current script word
    BOOL active;               // 0x10
    int animNum;               // 0x14
    int waitFrame;             // 0x18
    int endAnim;               // 0x1C
    BOOL completed;            // 0x20
    int vars[8];               // 0x24
    int commandCount;          // 0x44
    int loopMax;               // 0x48
    int loopCounter;           // 0x4C
    u32 *loopStart;            // 0x50
    int startDelay;            // 0x54
    int originalX;             // 0x58
    int originalY;             // 0x5C
    int translateX;            // 0x60
    int translateY;            // 0x64
    int offsetX;               // 0x68
    int offsetY;               // 0x6C
    int scaleX;                // 0x70
    int scaleY;                // 0x74
    int rotationZ;             // 0x78
    TransformData transforms[4]; // 0x7C
    u8 flipSprite;             // 0x1CC
    u8 waitForTransform;       // 0x1CD
    u8 yNormalization;         // 0x1CE
    u8 fadeActive;             // 0x1CF
} PokemonAnim;

// The PokepicAnimManager typedef is in unk_02016EDC.h.
struct PokepicAnimManager {
    PokemonAnim *anims;        // 0x0 animCount entries
    enum HeapID heapID;        // 0x4
    u8 flipSprite;             // 0x8
    u8 animCount;              // 0x9
    u8 padding_A[2];
};

// _020F61BC: the transform functions, by TRANSFORM_FUNC_*
typedef struct {
    void (*func)(TransformData *transform, PokemonAnim *monAnim);
    int paramCount;
    int transformTypeIndex;
} TransformFuncParameters;

extern const TransformFuncParameters _020F61BC[5];

// The script word readers and command helpers (unk_02016EDC_020170FC.c .. unk_02016EDC_02017220.c)
void sub_020170FC(PokemonAnim *monAnim);
int sub_020171F4(u32 *scriptPtr, u8 index, u8 one);
int sub_02017214(u32 *scriptPtr);
TransformData *sub_02017220(PokemonAnim *monAnim, u8 funcType);
void sub_0201726C(PokemonAnim *monAnim, int *outInt);
void sub_02017280(PokemonAnim *monAnim, u8 *outU8);
void sub_02017294(PokemonAnim *monAnim, u8 *outIndex);
void sub_020172B4(PokemonAnim *monAnim, u8 *outIndex1, u8 *outIndex2);
void sub_020172C8(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2);
void sub_0201733C(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2);
void sub_020173C8(PokemonAnim *monAnim, u8 *outDestIndex, int *outRadians, int *outAmplitude);
u8 sub_02017470(int *value1, int *value2);
void sub_020174BC(PokemonAnim *monAnim);
void sub_02017BC8(u8 calcType, int *originalValue, int *currentValue, int *nextValue);
void sub_02017BF8(u8 transformType, TransformData *transform, PokemonAnim *monAnim);
void sub_02017C78(PokemonAnim *monAnim, int funcType);

#endif // POKEHEARTGOLD_POKEMON_ANIM_H
