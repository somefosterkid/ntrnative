// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/mus_item_draw.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUS_ITEM_DRAW_H
#define BWPORT_APP_MUSICAL_MUS_ITEM_DRAW_H

#include "types.h"
#include "gfl/blact.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// pokebw2's MusItemDraw_UpdateSystem
void func_ov112_021f4990(MusItemDrawSys *sys);
// pokebw2's MusItemDraw_ChangeItem
void func_ov112_021f4d74(MusItemDrawSys *sys, MusItemDraw *item, u16 itemId, u16 material);
// pokebw2's MusItemDraw_InitSystem
MusItemDrawSys *func_ov112_021f48c0(BlActScene *blact, u16 count, HeapID heapId);
// pokebw2's MusItemDraw_SetUseOffset
void func_ov112_021f4ee8(MusItemDrawSys *sys, MusItemDraw *item, BOOL useOffset);
// pokebw2's MusItemDraw_AddItemWithMaterial
// Adds a prop that draws with a material the scene already has
MusItemDraw *func_ov112_021f4c1c(MusItemDrawSys *sys, u16 itemId, u32 material, const VecFx32 *pos);
// pokebw2's MusItemDraw_SetVisible
void func_ov112_021f4e6c(MusItemDrawSys *sys, MusItemDraw *item, BOOL visible);
// pokebw2's MusItemDraw_SetShadowPalette
// Gives the prop the shadow's palette
void func_ov112_021f4de0(MusItemDrawSys *sys, MusItemDraw *item, void *texResource);
// pokebw2's MusItemDraw_DelItem
void func_ov112_021f4d40(MusItemDrawSys *sys, MusItemDraw *item);
// pokebw2's MusItemDraw_TermSystem
void func_ov112_021f4948(MusItemDrawSys *sys);
// pokebw2's MusItemDraw_SetPosition
void func_ov112_021f4e8c(MusItemDrawSys *sys, MusItemDraw *item, const VecFx32 *pos);
// pokebw2's MusItemDraw_GetSize
void func_ov112_021f4ec4(MusItemDrawSys *sys, MusItemDraw *item, s16 *scaleX, s16 *scaleY);
// pokebw2's MusItemDraw_GetPosition
void func_ov112_021f4ea4(MusItemDrawSys *sys, MusItemDraw *item, VecFx32 *pos);
// pokebw2's MusItemDraw_CopyShadowPos
// Puts a shadow prop a little behind and below right of its prop
void func_ov112_021f4e2c(MusItemDrawSys *sys, MusItemDraw *item, MusItemDraw *shadow);
// pokebw2's MusItemDraw_ItemCanEquipPos
BOOL func_ov112_021f4afc(MusItemDrawSys *sys, u16 itemId, s32 pos);

// pokebw2's MusItemDraw_GetRotation
void func_ov112_021f4ee0(MusItemDrawSys *sys, MusItemDraw *item, u16 *rotation);
// pokebw2's MusItemDraw_GetItemOffset
void func_ov112_021f4f04(MusItemDraw *item, s32 *offset);
// pokebw2's MusItemDraw_GetUseOffset
BOOL func_ov112_021f4efc(MusItemDrawSys *sys, MusItemDraw *item);

// A prop being drawn
struct MusItemDraw {
    u16 active : 1;
    // Whether its actor needs its position, scale and rotation again
    u16 update : 1;
    // Whether it is moved by its offset from where it is worn
    u16 useOffset : 1;
    // Whether its palette is the shadow's
    u16 shadow : 1;
    // Whether its material is its own, to free with it
    u16 ownMaterial : 1;
    u16 rotation;
    u32 material;
    u32 actor;
    u32 texFileId;
    VecFx32 pos;
    s16 scaleX;
    s16 scaleY;
    MusicalItemData *data;
};

struct MusItemDrawSys {
    HeapID heapId;
    BlActScene *blact;
    MusItemDraw *items;
    // Overlay 210's table of the props
    void *itemData;
    u16 count;
    GXRgb shadowPalette[16];
};

// pokebw2's MusItemDraw_AddItem
MusItemDraw *func_ov112_021f4b88(MusItemDrawSys *sys, u16 itemId, void *texResource, const VecFx32 *pos);
// pokebw2's MusItemDraw_SetSize
void func_ov112_021f4eb4(MusItemDrawSys *sys, MusItemDraw *item, s16 scaleX, s16 scaleY);
// pokebw2's MusItemDraw_SetRotation
void func_ov112_021f4ed4(MusItemDrawSys *sys, MusItemDraw *item, u16 rotation);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MusItemDraw_FreeTexResource
void func_ov112_021f4b4c(void *resource);
// pokebw2's MusItemDraw_GetItemFlag7
BOOL func_ov112_021f4b24(MusItemDraw *item);
// pokebw2's MusItemDraw_CanEquipPos
BOOL func_ov112_021f4af0(MusItemDraw *item, u32 pos);
// pokebw2's MusItemDraw_IsItemOfPos
BOOL func_ov112_021f4b10(MusItemDrawSys *sys, u16 itemId, s32 pos);
// pokebw2's MusItemDraw_DrawSystem
void func_ov112_021f4aa4(MusItemDrawSys *sys);
// pokebw2's MusItemDraw_GetItemFlag9
BOOL func_ov112_021f4b30(MusItemDraw *item);
// pokebw2's MusItemDraw_LoadTexResource
// The texture of a prop, as a 3D resource
void *func_ov112_021f4b3c(u16 itemId);
// pokebw2's MusItemDraw_GetItemData
void *func_ov112_021f4f10(MusItemDrawSys *sys);

#endif // BWPORT_APP_MUSICAL_MUS_ITEM_DRAW_H
