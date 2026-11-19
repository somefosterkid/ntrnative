#ifndef POKEHEARTGOLD_OVERLAY_99_OBJ99_H
#define POKEHEARTGOLD_OVERLAY_99_OBJ99_H

#include "constants/heap.h"

struct BgConfig;
struct ManagedSprite;
struct NARC;
struct Ov98TextSys;
struct PokeathlonCourseSummary;
struct PokeathlonSpriteContext;
struct UnkStruct_0203EFA0;

// The ov99_021E677C app object (HEAP_ID_132): the pokeathlon summary screen.
typedef struct Obj99 {
    struct PokeathlonCourseSummary *summary;       // 0x00
    struct BgConfig *bgConfig;                     // 0x04
    struct NARC *narc;                             // 0x08
    enum HeapID heap;                              // 0x0C
    struct Ov98TextSys *textSys;                   // 0x10
    struct PokeathlonSpriteContext *spriteContext; // 0x14
    struct ManagedSprite *sprites[67];             // 0x18
} Obj99;

// sizeof(Obj99) == 0x124

void ov99_021E6938(void *obj);
void ov99_021E695C(Obj99 *obj);
void ov99_021E69D8(Obj99 *obj);
void ov99_021E6A70(Obj99 *obj);
void ov99_021E6A9C(Obj99 *obj);
void ov99_021E6C14(Obj99 *obj);
void ov99_021E6C30(Obj99 *obj, u32 value, s32 first, u32 count, s16 x, u16 y, u8 step, s32 flag);
void ov99_021E6CF4(Obj99 *obj, u8 arg1);
void ov99_021E6D14(Obj99 *obj, u32 arg1);
void ov99_021E6D34(Obj99 *obj);
void ov99_021E6FD0(Obj99 *obj, struct UnkStruct_0203EFA0 *args);

#endif // POKEHEARTGOLD_OVERLAY_99_OBJ99_H
