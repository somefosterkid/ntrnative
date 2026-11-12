// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/dup_fitting_item.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_DUP_FITTING_ITEM_H
#define BWPORT_FIELD_DUP_FITTING_ITEM_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// A point of the touch screen
typedef struct {
    s32 x;
    s32 y;
} DupFittingPoint;

// A prop that the player owns, as the dressing room keeps it
typedef struct {
    u16 itemId;
    // The materials of the BlAct scene that hold its texture, in colors and dimmed
    u32 material;
    u32 dimMaterial;
    // Whether it is on the stage or worn
    BOOL used;
    // Whether the game flags it, which sorts it first
    BOOL isNew;
} DupFittingItemEntry;

typedef struct DupFittingItemNode DupFittingItemNode;

// pokebw2's DupFittingItemNode_ChangeEntry
// Shows another entry's prop
void func_ov133_021ba1ec(DupFittingItemNode *node, MusItemDrawSys *sys, DupFittingItemEntry *entry);
// pokebw2's DupFittingItemList_Create
// Creates a list that holds at most max nodes
DupFittingItemList *func_ov133_021ba0cc(HeapID heapId, u16 max);
// pokebw2's DupFittingItemNode_Create
// Makes a node that shows an entry's prop at a position of the scene
DupFittingItemNode *func_ov133_021ba19c(HeapID heapId, MusItemDrawSys *sys, DupFittingItemEntry *entry,
                                        const VecFx32 *pos);
// pokebw2's DupFittingItemList_AddTail
// Adds a node after the last one, or before the first one. Nothing happens when the list is full
void func_ov133_021ba100(DupFittingItemList *list, DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_SetPos
void func_ov133_021ba21c(DupFittingItemNode *node, const DupFittingPoint *pos);
// pokebw2's DupFittingItemNode_Delete
void func_ov133_021ba1d8(DupFittingItemNode *node, MusItemDrawSys *sys);
// pokebw2's DupFittingItemList_Remove
void func_ov133_021ba148(DupFittingItemList *list, DupFittingItemNode *node);
// pokebw2's DupFittingItemList_IsFull
BOOL func_ov133_021ba18c(DupFittingItemList *list);
// pokebw2's DupFittingItemNode_IsAtPoint
// Whether a point of the touch screen is on the node's prop, within 16 pixels of where its center is
BOOL func_ov133_021ba25c(DupFittingItemNode *node, s32 x, s32 y);
// pokebw2's DupFittingItemNode_SetEquipPos
void func_ov133_021ba254(DupFittingItemNode *node, s32 equipPos);
// pokebw2's DupFittingItemList_AddHead
void func_ov133_021ba124(DupFittingItemList *list, DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_SetOrigin
void func_ov133_021ba22c(DupFittingItemNode *node, const DupFittingPoint *origin);
// pokebw2's DupFittingItemNode_SetFrames
void func_ov133_021ba24c(DupFittingItemNode *node, u32 frames);
// pokebw2's DupFittingItemNode_GetOrigin
DupFittingPoint *func_ov133_021ba238(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_GetDistanceSq
// How far a point is from the node's prop, squared
u32 func_ov133_021ba324(DupFittingItemNode *node, s32 x, s32 y);

// Black 2's; matching code confirms the offsets of entry, nextEntry, draw, pos, scale, frames, equipPos, origin,
// baseScale and next, and its size in Black
struct DupFittingItemNode {
    u32 unk0;
    // The prop that it shows now, and the one that it will show when the drawing is next updated
    DupFittingItemEntry *entry;
    DupFittingItemEntry *nextEntry;
    MusItemDraw *draw;
    // Where it is on the touch screen
    DupFittingPoint pos;
    s32 scale;
    // How many frames it still moves for, when it goes back to the shelf
    u16 frames;
    // The position of the Pokémon that it is worn at, or 10 when it is not worn
    s32 equipPos;
    // Where it was before it moved
    DupFittingPoint origin;
    s32 baseScale;
    DupFittingItemNode *next;
};

// Black 2's; matching code confirms the offsets of heapId, count, max, head and tail, and its size in Black
// A list of nodes, with the most that it holds
struct DupFittingItemList {
    u16 heapId;
    u16 unk2;
    u16 count;
    u16 max;
    DupFittingItemNode *head;
    DupFittingItemNode *tail;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DupFittingItemNode_GetEntry
DupFittingItemEntry *func_ov133_021ba20c(DupFittingItemNode *node);
// pokebw2's DupFittingItemList_GetHead
DupFittingItemNode *func_ov133_021ba184(DupFittingItemList *list);
// pokebw2's DupFittingItemNode_GetNext
DupFittingItemNode *func_ov133_021ba208(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_GetDraw
MusItemDraw *func_ov133_021ba218(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_GetNextEntry
DupFittingItemEntry *func_ov133_021ba214(DupFittingItemNode *node);
// pokebw2's DupFittingItemList_GetCount
u16 func_ov133_021ba188(DupFittingItemList *list);
// pokebw2's DupFittingItemList_Free
void func_ov133_021ba0f8(DupFittingItemList *list);
// pokebw2's DupFittingItemNode_SetNextEntry
void func_ov133_021ba210(DupFittingItemNode *node, DupFittingItemEntry *entry);
// pokebw2's DupFittingItemNode_SetScale
void func_ov133_021ba244(DupFittingItemNode *node, s32 scale);
// pokebw2's DupFittingItemNode_GetPos
DupFittingPoint *func_ov133_021ba228(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_SetBaseScale
void func_ov133_021ba23c(DupFittingItemNode *node, s32 baseScale);
// pokebw2's DupFittingItemNode_GetScale
s32 func_ov133_021ba248(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_GetEquipPos
s32 func_ov133_021ba258(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_GetFrames
u16 func_ov133_021ba250(DupFittingItemNode *node);
// pokebw2's DupFittingItemNode_GetBaseScale
s32 func_ov133_021ba240(DupFittingItemNode *node);

#endif // BWPORT_FIELD_DUP_FITTING_ITEM_H
