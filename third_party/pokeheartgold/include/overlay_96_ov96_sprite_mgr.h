#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_SPRITE_MGR_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_SPRITE_MGR_H

#include "constants/heap.h"

#include "g2d_renderer.h"
#include "filesystem_files_def.h"
#include "sprite.h"

struct GF_2DGfxResMan;
struct Sprite;
struct SpriteList;
struct SpriteResource;

// Ov96SpriteMgr: the 0x158 sprite / resource manager of the Pokeathlon (overlay 96) screens, allocated by
// ov96_021EB180 and freed by ov96_021EB21C. It owns the sprite list and renderer, one resource manager per
// graphics resource type (char, palette, cell, anim) and two tables: the loaded resources (res, one record per
// resource id) and the created sprites (slots, one record per sprite, handed out as struct Ov96Obj *).

#define OV96_SPRITE_RES_TYPES 4

// The 12 bytes the caller passes to ov96_021EB180 (rodata tables ov96_0221C0B8 / ov96_0221C2A8 / ov96_0221C404 ...).
typedef struct Ov96SpriteMgrParam {
    s32 maxSprites;                    // 0x0 sprite slots
    s32 maxRes;                        // 0x4 resource records
    u8 resCounts[OV96_SPRITE_RES_TYPES]; // 0x8 objects per resource manager
} Ov96SpriteMgrParam; // size 0xC

// One created sprite (Ov96SpriteMgr.slots[i]); DS size 0xC.
struct Ov96Obj {
    u16 inUse;             // 0x0
    u16 unk_2;             // 0x2
    struct Sprite *sprite; // 0x4
    u16 group;             // 0x8
    u16 resId;             // 0xA
};

// One loaded resource set (Ov96SpriteMgr.res[i]); DS size 0x14.
typedef struct Ov96SpriteMgrRes {
    s32 id;                                         // 0x0
    struct SpriteResource *res[OV96_SPRITE_RES_TYPES]; // 0x4 char, palette, cell, anim
} Ov96SpriteMgrRes;

typedef struct Ov96SpriteMgr {
    enum HeapID heapID;                                 // 0x000
    s32 numSprites;                                     // 0x004 slots handed out so far
    Ov96SpriteMgrParam param;                           // 0x008
    struct SpriteList *spriteList;                      // 0x014
    G2dRenderer renderer;                               // 0x018
    struct GF_2DGfxResMan *resMan[OV96_SPRITE_RES_TYPES]; // 0x140
    Ov96SpriteMgrRes *res;                              // 0x150
    struct Ov96Obj *slots;                                     // 0x154
} Ov96SpriteMgr; // size 0x158 on the DS

Ov96SpriteMgr *ov96_021EB180(enum HeapID heapID, const Ov96SpriteMgrParam *param);
void ov96_021EB21C(Ov96SpriteMgr *mgr);
void ov96_021EB29C(Ov96SpriteMgr *mgr, s32 index, s32 resId);
void ov96_021EB3A4(Ov96SpriteMgr *mgr);
struct SpriteResource **ov96_021EB5EC(Ov96SpriteMgr *mgr, s32 resId, u32 type);
struct Ov96Obj *ov96_021EB4F4(Ov96SpriteMgr *mgr, s32 resId, s32 group);
struct Sprite *ov96_021EB408(Ov96SpriteMgr *mgr, s32 unk_4, s32 vram, u8 resId, u8 group);
struct Ov96Obj *ov96_021EB3E4(Ov96SpriteMgr *mgr, s32 unk_4, s32 vram, u8 resId, u8 group);
void ov96_021EB2BC(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 resId, s32 vram);
void ov96_021EB2F4(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 resId, s32 vram, u8 plttNum);
void ov96_021EB334(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 resId);
void ov96_021EB36C(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 resId);

void ov96_021EB630(struct Ov96Obj *obj, u32 priority);
void ov96_021EB52C(struct Ov96Obj *obj, s32 arg1, s32 arg2);
void ov96_021EB564(struct Ov96Obj *obj, s32 seq);
void ov96_021EB588(struct Ov96Obj *obj, VecFx32 *pos);
VecFx32 *ov96_021EB594(struct Ov96Obj *obj);
struct Sprite *ov96_021EB5B8(struct Ov96Obj *obj);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_SPRITE_MGR_H
