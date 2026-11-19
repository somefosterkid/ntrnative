#ifndef POKEHEARTGOLD_OVERLAY_96_SPRITE_RES_MAN_H
#define POKEHEARTGOLD_OVERLAY_96_SPRITE_RES_MAN_H

#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

// Ov96SpriteResDesc: one 0x10-byte sprite description (a Pokemon model texture + palette pick), read by
// ov96_021E8C70 / ov96_021E9104 / ov96_021E8BB4 and copied into the entry by ov96_021E8FE0.
typedef struct Ov96SpriteResDesc {
    u16 species;
    u16 unk_2;
    u16 isLarge; // 8x8 tiles when set, 4x4 otherwise
    u8 unk_6;
    u8 unk_7;
    s32 unk_8;
    u32 unk_C;
} Ov96SpriteResDesc; // size = 0x10

// Ov96SpriteResEntry: one loaded sprite (char/pltt/cell/anim resources + the sprite itself).
typedef struct Ov96SpriteResEntry {
    SpriteResource *charRes;
    SpriteResource *plttRes;
    SpriteResource *cellRes;
    SpriteResource *animRes;
    Sprite *sprite;
    Ov96SpriteResDesc desc;
    s32 unk_24;
    BOOL unk_28;
} Ov96SpriteResEntry; // size = 0x2c

// Ov96SpriteResMan: the sprite-resource manager allocated by ov96_021E8A30.
typedef struct Ov96SpriteResMan {
    enum HeapID heapID;
    u16 maxEntries;
    u16 numLoaded;
    s32 mode;
    s32 unk_C; // set to 0 when a load task starts, 1 when it ends
    BOOL ownsSpriteList;
    SpriteList *ownedSpriteList;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    Ov96SpriteResEntry *entries;
    u8 texBuf[0x800];
} Ov96SpriteResMan; // size = 0x958

// Ov96SpriteResLoadParams: the 0x44-byte parameter block copied by ov96_021E8B1C into the load task.
typedef struct Ov96SpriteResLoadParams {
    BOOL runToEnd;
    s32 unk_4;
    s32 unk_8;
    s32 vramType;
    NNS_G2D_VRAM_TYPE screen;
    u32 unk_14[12];
} Ov96SpriteResLoadParams; // size = 0x44

// Ov96SpriteResLoadTask: the 0x154-byte data of the SysTask ov96_021E8FB4 (allocated by ov96_021E8B1C).
typedef struct Ov96SpriteResLoadTask {
    u16 count;
    u16 pos;
    s32 baseIndex;
    s32 state;
    Ov96SpriteResMan *man;
    Ov96SpriteResLoadParams params;
    Ov96SpriteResDesc descs[16];
} Ov96SpriteResLoadTask; // size = 0x154

// Ov96SpriteSetElem: one slot of an Ov96SpriteSet (the entry it shows + up to two sprites on top of it).
typedef struct Ov96SpriteSetElem {
    Ov96SpriteResEntry *entry; // from ov96_021E8B8C; its sprite is shown by ov96_021E8BAC
    Sprite *spriteA;           // created when the resource source has flag 2; hidden when hideSpriteA is set
    Sprite *spriteB;           // created when the resource source has flag 4
    s32 unk_0C;                // 3/4/5 from the load params (spriteB)
    s32 unk_10;                // 3/4/5 from the load params (spriteA)
    s32 unk_14;                // 0x20 when the desc isLarge, else 0x10
    s32 unk_18;                // same as unk_14
    s32 unk_1C[3];
    s32 unk_28;                // ov96_021E90FC(entry), set when the load task ends
    s32 unk_2C[2];
    s32 unk_34;                // set by ov96_021EAC0C
    s32 unk_38;
    s32 priorityStep;          // 0xC
    BOOL hideSpriteA;
} Ov96SpriteSetElem; // size = 0x44

// Ov96SpriteSet: the 0x34C object of ov96_021EA854 (a sprite resource manager + up to 12 slots).
typedef struct Ov96SpriteSet {
    enum HeapID heapID;
    s32 count;        // slots in use (<= 12)
    s32 drawPriority; // base draw priority, slot i gets drawPriority + i
    BOOL loaded;      // 1 once the load task ov96_021EAA24 ended (ov96_021EAA00)
    SpriteList *spriteList;
    Ov96SpriteResMan *man;
    UnkStruct_ov96_021E9A78 *resSrc;
    Ov96SpriteSetElem elems[12];
} Ov96SpriteSet; // size = 0x34C

Ov96SpriteSet *ov96_021EA854(enum HeapID heapID, s32 count, s32 drawPriority, UnkStruct_ov96_021E9A78 *resSrc, SpriteList *spriteList);
void ov96_021EA894(Ov96SpriteSet *set);
SysTask *ov96_021EA8A8(Ov96SpriteSet *set, s32 count, Ov96SpriteResDesc *descs, Ov96SpriteResLoadParams *params, u32 priority, s32 baseIndex);
void ov96_021EAA24(SysTask *task, void *data);
Sprite *ov96_021EAA88(Ov96SpriteSet *set, s32 arg1, NNS_G2D_VRAM_TYPE screen, s32 resId);
void ov96_021EAB38(Ov96SpriteSetElem *elem, s32 flag);
void ov96_021EAB74(Ov96SpriteSetElem *elem, s32 flag);
void ov96_021EAB94(Ov96SpriteSetElem *elem, s32 flag);
void ov96_021EABA8(Ov96SpriteSetElem *elem, u32 priority);
void ov96_021EABDC(Ov96SpriteSetElem *elem, s32 step);
void ov96_021EABE0(Ov96SpriteSetElem *elem, u8 affine);
void ov96_021EABF4(Ov96SpriteSetElem *elem, VecFx32 *vec);
void ov96_021EAC08(Ov96SpriteSetElem *elem, s32 state);
void ov96_021EAC0C(Ov96SpriteSetElem *elem, s32 state);
BOOL ov96_021E8B88(Ov96SpriteResMan *man);
u8 ov96_021E90FC(Ov96SpriteResEntry *entry);
Sprite *ov96_021E8BAC(Ov96SpriteResEntry *entry);
u8 ov96_021E9CF4(UnkStruct_ov96_021E9A78 *res, s32 flag);
GF_2DGfxResMan *ov96_021E9D08(UnkStruct_ov96_021E9A78 *res, s32 index);
BOOL ov96_021EAA00(Ov96SpriteSet *set);
Ov96SpriteSetElem *ov96_021EAA04(Ov96SpriteSet *set, s32 index);
Ov96SpriteResEntry *ov96_021EAA20(Ov96SpriteSetElem *elem);
Ov96SpriteResEntry *ov96_021E8B8C(Ov96SpriteResMan *man, u8 index);
Ov96SpriteResDesc *ov96_021E8BB0(Ov96SpriteResEntry *entry);
void ov96_021E8BB4(Ov96SpriteResDesc *desc, enum HeapID heapID, void *dest);
Ov96SpriteResMan *ov96_021E8A30(enum HeapID heapID);
void ov96_021E8A44(Ov96SpriteResMan *man, u32 maxEntries, SpriteList *spriteList);
void ov96_021E8AAC(Ov96SpriteResMan *man);
void ov96_021E8B1C(Ov96SpriteResMan *man, s32 count, Ov96SpriteResDesc *descs, Ov96SpriteResLoadParams *params, u32 priority, s32 baseIndex);
void ov96_021E8BF4(Ov96SpriteResMan *man, s32 index, s32 screens, void *pltt);
void ov96_021EAF60(Ov96SpriteSet *set, s32 index, s32 screens, void *pltt);
void ov96_021E8C70(Ov96SpriteResMan *man, Ov96SpriteResEntry *entry, Ov96SpriteResDesc *desc, s32 vramType, u8 resId, s32 flag);
void ov96_021E8EE4(Ov96SpriteResMan *man, s32 unk1, s32 unk2, NNS_G2D_VRAM_TYPE vramType, u8 resId, u8 index);
s32 ov96_021E8FE0(Ov96SpriteResLoadTask *task);

#endif // POKEHEARTGOLD_OVERLAY_96_SPRITE_RES_MAN_H
