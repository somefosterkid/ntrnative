#ifndef POKEHEARTGOLD_OVERLAY_01_021F944C_H
#define POKEHEARTGOLD_OVERLAY_01_021F944C_H

#include "player_avatar.h"
#include "sprite.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "billboard.h"

struct LocalMapObject;
struct OverworldAnimManager;

// The work block of a 3D-model map object's billboard task (ov01_021FA97C allocates it, ov01_021FAB9C runs and frees it): 0x58 bytes on the DS.
typedef struct ModelMapObjectWork {
    s32 spriteId;
    s32 state;
    void *modelData;
    void *animData;
    void *texData;
    BillboardGfxSequence gfxSequence;
    Billboard *billboard;
    BillboardResources resources;
    struct LocalMapObject *mapObject;
    struct OverworldAnimManager *animManager;
} ModelMapObjectWork;

// A map object's billboard animation state: ov01_021F9610 saves it from a billboard, ov01_021F9630 restores it.
typedef struct BillboardAnimState {
    int draw;
    u16 unk_04;
    s16 animNum;
    fx32 frameNum;
} BillboardAnimState;

// One row of ov01_02207318 (asm data): 8 bytes on the DS (4 bytes + a pointer), 16 on the host. Index it; never step it by 8.
typedef struct BillboardSpriteInfo {
    u8 unk_00;
    u8 unk_01;
    u8 modelId; // key into UnkStruct_ov5_021ED0A4::unk_F8
    u8 animId; // key into UnkStruct_ov5_021ED0A4::unk_FC
    const BillboardAnim *anims;
} BillboardSpriteInfo;

// The work areas (sub_0205F3E8's block) of the billboard map objects, DS sizes 0x14 / 0x18 / 0x14: the billboard pointer is
// 4 bytes on the DS and 8 on the host, so the slot and the anim state after it must be reached through the fields.
typedef struct MapObjectBillboardWorkA { // ov01_021F73CC, ov01_021F7434, ov01_021F7DA8, ov01_021F7E28, the renderers ov01_021F772C..ov01_021F8AB0
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    s8 unk_03;
    Billboard *billboard;
    BillboardAnimState animState;
} MapObjectBillboardWorkA;

// The same 0x14-byte work area read by the state tables ov01_022089CC / ov01_022089D0 / ov01_022089E8 / ov01_02208AE4 / ov01_02208B34
// (dispatchers ov01_021F8B30 / 878C / 82F0 / 7E6C): the last facing direction and the last action sit in the first and third byte, the
// first bit of the fourth is a flag. ov01_021F8B30..ov01_021F8C88 and ov01_021F88F0..ov01_021F8AB0 reach the same bytes through WorkA.
typedef struct MapObjectBillboardWorkE {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    u8 unk_03_0 : 1;
    u8 unk_03_1 : 7;
    Billboard *billboard;
    BillboardAnimState animState;
} MapObjectBillboardWorkE;

// A billboard map object's state function (one row of the ov01_02208xxx tables): object, its billboard, its work area, facing direction.
typedef void (*MapObjectBillboardStateFuncA)(LocalMapObject *, Billboard *, MapObjectBillboardWorkA *, int);
typedef void (*MapObjectBillboardStateFuncE)(LocalMapObject *, Billboard *, MapObjectBillboardWorkE *, int);

typedef struct MapObjectBillboardWorkB { // ov01_021F7478, ov01_021F74B4, ov01_021F74C8
    Billboard *billboard;
    BillboardAnimState animState;
    s8 unk_10;
    u8 unk_11;
    s8 unk_12;
    s8 unk_13;
    s8 unk_14;
    s8 unk_15;
    u8 unk_16;
    u8 flags;
} MapObjectBillboardWorkB;

typedef struct MapObjectBillboardWorkC { // ov01_021F9140, ov01_021F90D0, ov01_021F90FC
    Billboard *billboard;
    BillboardAnimState animState;
    s32 unk_10;
} MapObjectBillboardWorkC;

typedef struct MapObjectBillboardWorkD { // sub_02063E50, sub_02063E70, sub_0206402C (the apricorn trees), DS size 0x18
    u32 spriteId;
    s32 unk_04;
    Billboard *billboard;
    BillboardAnimState animState;
} MapObjectBillboardWorkD;

// The billboard slot of a map object (Billboard **) is filled by these; every caller passes &work->billboard.
int ov01_021F94C0(LocalMapObject *obj, Billboard **billboard, int spriteId);
int ov01_021F9510(LocalMapObject *obj, Billboard **billboard);
void ov01_021F9574(LocalMapObject *obj, Billboard **billboard, int spriteId);
void ov01_021F95A8(LocalMapObject *obj, Billboard **billboard);
void ov01_021F9610(Billboard *billboard, BillboardAnimState *state);
void ov01_021F9630(Billboard *billboard, const BillboardAnimState *state);
void ov01_021F9654(LocalMapObject *obj, Billboard **billboard, int spriteId);
const BillboardSpriteInfo *sub_021FA248(s32 id);

BOOL ov01_021FA2D4(LocalMapObject *obj);
void ov01_021FA930(LocalMapObject *obj, u32 sprite);
// TODO: 13 other billboard map object files still pass raw-offset Sprite * slots; type them, then make this Billboard *.
BOOL ov01_021FA3E8(LocalMapObject *obj, void *billboard);
void ov01_021FA108(MapObjectManager *mgr, int gfxId, LocalMapObject *mapObject);
#if defined(SDK_PORT) || defined(UBFIX)
void ModelMapObjectTask_EndAll(void);
#endif

#endif // POKEHEARTGOLD_OVERLAY_01_021F944C_H
