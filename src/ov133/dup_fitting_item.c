// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov208/dup_fitting_item.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/dup_fitting_item.h"
#include "app/musical/mus_item_draw.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"

// Overlay 208's dup_fitting_item.c, named after its string: the lists and nodes of the props in the dressing room.
// Its function names are guessed from what the dressing room does with them

// pokebw2's DupFittingItemList_Create
DupFittingItemList *func_ov133_021ba0cc(HeapID heapId, u16 max) {
    DupFittingItemList *list = GFL_HeapAllocate(heapId, sizeof(DupFittingItemList), FALSE, "dup_fitting_item.c", 76);

    list->heapId = heapId;
    list->count = 0;
    list->max = max;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

// pokebw2's DupFittingItemList_Free
void func_ov133_021ba0f8(DupFittingItemList *list) {
    GFL_HeapFree(list);
}

// pokebw2's DupFittingItemList_AddTail
void func_ov133_021ba100(DupFittingItemList *list, DupFittingItemNode *node) {
    if (list->count < list->max) {
        if (list->head == NULL) {
            list->head = node;
        } else {
            list->tail->next = node;
        }
        list->tail = node;
        node->next = NULL;
        list->count++;
    }
}

// pokebw2's DupFittingItemList_AddHead
void func_ov133_021ba124(DupFittingItemList *list, DupFittingItemNode *node) {
    if (list->count < list->max) {
        if (list->head == NULL) {
            list->head = node;
            list->tail = node;
            node->next = NULL;
        } else {
            node->next = list->head;
            list->head = node;
        }
        list->count++;
    }
}

// pokebw2's DupFittingItemList_Remove
void func_ov133_021ba148(DupFittingItemList *list, DupFittingItemNode *node) {
    DupFittingItemNode *prev = NULL;
    DupFittingItemNode *cur = list->head;

    while (cur != node && cur != NULL) {
        prev = cur;
        cur = cur->next;
    }
    if (cur != NULL) {
        if (prev != NULL) {
            prev->next = cur->next;
        } else {
            list->head = cur->next;
        }
        if (list->tail == node) {
            list->tail = prev;
        }
        cur->next = NULL;
        list->count--;
    }
}

// pokebw2's DupFittingItemList_GetHead
DupFittingItemNode *func_ov133_021ba184(DupFittingItemList *list) {
    return list->head;
}

// pokebw2's DupFittingItemList_GetCount
u16 func_ov133_021ba188(DupFittingItemList *list) {
    return list->count;
}

// pokebw2's DupFittingItemList_IsFull
BOOL func_ov133_021ba18c(DupFittingItemList *list) {
    if (list->count == list->max) {
        return TRUE;
    }
    return FALSE;
}

DupFittingItemNode *func_ov133_021ba19c(HeapID heapId, MusItemDrawSys *sys, DupFittingItemEntry *entry,
                                        const VecFx32 *pos) {
    DupFittingItemNode *node = GFL_HeapAllocate(heapId, sizeof(DupFittingItemNode), FALSE, "dup_fitting_item.c", 204);

    node->next = NULL;
    node->entry = entry;
    node->draw = func_ov112_021f4c1c(sys, entry->itemId, entry->material, pos);
    node->frames = 0;
    node->equipPos = 10;
    return node;
}

// pokebw2's DupFittingItemNode_Delete
void func_ov133_021ba1d8(DupFittingItemNode *node, MusItemDrawSys *sys) {
    func_ov112_021f4d40(sys, node->draw);
    GFL_HeapFree(node);
}

// pokebw2's DupFittingItemNode_ChangeEntry
void func_ov133_021ba1ec(DupFittingItemNode *node, MusItemDrawSys *sys, DupFittingItemEntry *entry) {
    node->entry = entry;
    func_ov112_021f4d74(sys, node->draw, entry->itemId, entry->material);
}

// pokebw2's DupFittingItemNode_GetNext
DupFittingItemNode *func_ov133_021ba208(DupFittingItemNode *node) {
    return node->next;
}

// pokebw2's DupFittingItemNode_GetEntry
DupFittingItemEntry *func_ov133_021ba20c(DupFittingItemNode *node) {
    return node->entry;
}

// pokebw2's DupFittingItemNode_SetNextEntry
void func_ov133_021ba210(DupFittingItemNode *node, DupFittingItemEntry *entry) {
    node->nextEntry = entry;
}

// pokebw2's DupFittingItemNode_GetNextEntry
DupFittingItemEntry *func_ov133_021ba214(DupFittingItemNode *node) {
    return node->nextEntry;
}

// pokebw2's DupFittingItemNode_GetDraw
MusItemDraw *func_ov133_021ba218(DupFittingItemNode *node) {
    return node->draw;
}

// pokebw2's DupFittingItemNode_SetPos
void func_ov133_021ba21c(DupFittingItemNode *node, const DupFittingPoint *pos) {
    node->pos = *pos;
}

// pokebw2's DupFittingItemNode_GetPos
DupFittingPoint *func_ov133_021ba228(DupFittingItemNode *node) {
    return &node->pos;
}

// pokebw2's DupFittingItemNode_SetOrigin
void func_ov133_021ba22c(DupFittingItemNode *node, const DupFittingPoint *origin) {
    node->origin = *origin;
}

// pokebw2's DupFittingItemNode_GetOrigin
DupFittingPoint *func_ov133_021ba238(DupFittingItemNode *node) {
    return &node->origin;
}

// pokebw2's DupFittingItemNode_SetBaseScale
void func_ov133_021ba23c(DupFittingItemNode *node, s32 baseScale) {
    node->baseScale = baseScale;
}

// pokebw2's DupFittingItemNode_GetBaseScale
s32 func_ov133_021ba240(DupFittingItemNode *node) {
    return node->baseScale;
}

// pokebw2's DupFittingItemNode_SetScale
void func_ov133_021ba244(DupFittingItemNode *node, s32 scale) {
    node->scale = scale;
}

// pokebw2's DupFittingItemNode_GetScale
s32 func_ov133_021ba248(DupFittingItemNode *node) {
    return node->scale;
}

// pokebw2's DupFittingItemNode_SetFrames
void func_ov133_021ba24c(DupFittingItemNode *node, u32 frames) {
    node->frames = frames;
}

// pokebw2's DupFittingItemNode_GetFrames
u16 func_ov133_021ba250(DupFittingItemNode *node) {
    return node->frames;
}

// pokebw2's DupFittingItemNode_SetEquipPos
void func_ov133_021ba254(DupFittingItemNode *node, s32 equipPos) {
    node->equipPos = equipPos;
}

// pokebw2's DupFittingItemNode_GetEquipPos
s32 func_ov133_021ba258(DupFittingItemNode *node) {
    return node->equipPos;
}

// pokebw2's DupFittingItemNode_IsAtPoint
BOOL func_ov133_021ba25c(DupFittingItemNode *node, s32 x, s32 y) {
    s32 offset[2];
    u16 rotation;
    s16 dx;
    s16 dy;
    s32 py;

    func_ov112_021f4f04(node->draw, offset);
    if (func_ov112_021f4efc(NULL, node->draw) == TRUE) {
        fx16 sin;
        fx16 cos;
        s32 offX;
        s32 offY;

        func_ov112_021f4ee0(NULL, node->draw, &rotation);
        sin = FX_SinIdx(rotation);
        cos = FX_CosIdx(rotation);
        offX = FX_FX32_TO_F32(offset[0] * cos - offset[1] * sin);
        offY = FX_FX32_TO_F32(offset[0] * sin + offset[1] * cos);
        dx = node->pos.x - offX - x;
        py = node->pos.y - offY;
    } else {
        dx = node->pos.x - x;
        py = node->pos.y;
    }
    dy = py - y;
    if (dx < 16 && dx > -16 && dy < 16 && dy > -16) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DupFittingItemNode_GetDistanceSq
u32 func_ov133_021ba324(DupFittingItemNode *node, s32 x, s32 y) {
    s32 offset[2];
    u16 rotation;
    s16 dx;
    s16 dy;
    s32 py;

    func_ov112_021f4f04(node->draw, offset);
    if (func_ov112_021f4efc(NULL, node->draw) == TRUE) {
        fx16 sin;
        fx16 cos;
        s32 offX;
        s32 offY;

        func_ov112_021f4ee0(NULL, node->draw, &rotation);
        sin = FX_SinIdx(rotation);
        cos = FX_CosIdx(rotation);
        offX = FX_FX32_TO_F32(offset[0] * cos - offset[1] * sin);
        offY = FX_FX32_TO_F32(offset[0] * sin + offset[1] * cos);
        dx = node->pos.x - offX - x;
        py = node->pos.y - offY;
    } else {
        dx = node->pos.x - x;
        py = node->pos.y;
    }
    dy = py - y;
    return dx * dx + dy * dy;
}


