// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/mus_item_draw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_item_draw.h"
#include "constants/arc.h"
#include "field/musical.h"
#include "gfl/arc.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nnsys/gfd.h"
#include "app/musical/mus_item_data.h"

// Overlay 209's mus_item_draw.c: the props of the musical, drawn as billboard actors. A prop's texture comes from
// ARCID_MUSICAL_ITEM, its size, offset and the positions it fits from overlay 210's table

// ARCID_MUSICAL_ITEM holds the props' textures, then two other files. Out of range is drawn as prop 100

static u16 func_ov112_021f4aa8(int itemId);
static void func_ov112_021f4ac8(MusItemDraw *item, u8 *width, u8 *height);
static u16 func_ov112_021f4b54(MusItemDrawSys *sys);
static MusItemDraw *func_ov112_021f4c94(MusItemDrawSys *sys, u16 index, const VecFx32 *pos, s16 scaleX, s16 scaleY);

static const BOOL data_ov112_021f8300 = TRUE;

// pokebw2's MusItemDraw_InitSystem
MusItemDrawSys *func_ov112_021f48c0(BlActScene *blact, u16 count, HeapID heapId) {
    int i;
    MusItemDrawSys *sys = GFL_HeapAllocate(heapId, sizeof(MusItemDrawSys), FALSE, "mus_item_draw.c", 85);

    sys->items = GFL_HeapAllocate(heapId, sizeof(MusItemDraw) * count, FALSE, "mus_item_draw.c", 87);
    sys->heapId = heapId;
    sys->count = count;
    sys->blact = blact;
    for (i = 0; i < sys->count; i++) {
        sys->items[i].active = FALSE;
        sys->items[i].update = FALSE;
    }
    sys->itemData = func_ov112_021f4f14(heapId);
    for (i = 0; i < 16; i++) {
        sys->shadowPalette[i] = GX_RGB(22, 18, 22);
    }
    return sys;
}

// pokebw2's MusItemDraw_TermSystem
void func_ov112_021f4948(MusItemDrawSys *sys) {
    int i;

    NNS_GfdClearVramTransferManagerTask();
    func_ov112_021f4f40(sys->itemData);
    for (i = 0; i < sys->count; i++) {
        if (sys->items[i].active == TRUE) {
            func_ov112_021f4d40(sys, &sys->items[i]);
        }
    }
    GFL_HeapFree(sys->items);
    GFL_HeapFree(sys);
}

// pokebw2's MusItemDraw_UpdateSystem
void func_ov112_021f4990(MusItemDrawSys *sys) {
    int i;

    for (i = 0; i < sys->count; i++) {
        MusItemDraw *item = &sys->items[i];

        if (sys->items[i].active == TRUE && sys->items[i].update == TRUE) {
            fx32 dx;
            fx32 dy;
            VecFx32 pos;
            s32 offset[2];
            s16 scaleX;
            s16 scaleY;
            u8 width;
            u8 height;

            if (item->useOffset == TRUE) {
                func_ov112_021f4f60(item->data, offset);
                if (item->scaleX < 0) {
                    offset[0] *= -1;
                }
                dx = offset[0] * FX_CosIdx(item->rotation) - offset[1] * FX_SinIdx(item->rotation);
                dy = FX_SinIdx(item->rotation) * offset[0] + offset[1] * FX_CosIdx(item->rotation);
            } else {
                dx = 0;
                dy = 0;
            }
            pos.x = (item->pos.x - dx) / 16;
            pos.y = (FX32_CONST(192) - (item->pos.y - dy)) / 16;
            pos.z = item->pos.z;
            BlActScene_SetActorPos(sys->blact, item->actor, &pos);
            func_ov112_021f4ac8(item, &width, &height);
            scaleX = item->scaleX * width;
            scaleY = item->scaleY * height;
            func_0204f684(sys->blact, item->actor, &scaleX, &scaleY);
            func_0204f7c4(sys->blact, item->actor, &item->rotation);
            sys->items[i].update = FALSE;
        }
    }
}

// pokebw2's MusItemDraw_DrawSystem
void func_ov112_021f4aa4(MusItemDrawSys *sys) {
}

// pokebw2's MusItemDraw_GetTexFileId
static u16 func_ov112_021f4aa8(int itemId) {
    if (itemId >= (u16)GFL_ArcSysGetDataMax(ARCID_MUSICAL_ITEM) - 2) {
        return 100;
    }
    return itemId;
}

// pokebw2's MusItemDraw_GetFaceSize
static void func_ov112_021f4ac8(MusItemDraw *item, u8 *width, u8 *height) {
    u16 texWidth;
    u16 texHeight;

    func_0204edd0(func_ov112_021f4f70(item->data), &texWidth, &texHeight);
    *width = texWidth / 32;
    *height = texHeight / 32;
}

// pokebw2's MusItemDraw_CanEquipPos
BOOL func_ov112_021f4af0(MusItemDraw *item, u32 pos) {
    return func_ov112_021f4f74(item->data, pos);
}

// pokebw2's MusItemDraw_ItemCanEquipPos
BOOL func_ov112_021f4afc(MusItemDrawSys *sys, u16 itemId, s32 pos) {
    return func_ov112_021f4ff4(func_ov112_021f4f54(sys->itemData, itemId), pos);
}

// pokebw2's MusItemDraw_IsItemOfPos
BOOL func_ov112_021f4b10(MusItemDrawSys *sys, u16 itemId, s32 pos) {
    return func_ov112_021f5064(func_ov112_021f4f54(sys->itemData, itemId), pos);
}

// pokebw2's MusItemDraw_GetItemFlag7
BOOL func_ov112_021f4b24(MusItemDraw *item) {
    return func_ov112_021f50d0(item->data);
}

// pokebw2's MusItemDraw_GetItemFlag9
BOOL func_ov112_021f4b30(MusItemDraw *item) {
    return func_ov112_021f50e0(item->data);
}

// pokebw2's MusItemDraw_LoadTexResource
void *func_ov112_021f4b3c(u16 itemId) {
    return GFL_G3DSysReadArcSysResource(ARCID_MUSICAL_ITEM, func_ov112_021f4aa8(itemId));
}

// pokebw2's MusItemDraw_FreeTexResource
void func_ov112_021f4b4c(void *resource) {
    GFL_G3DResFree(resource);
}

// pokebw2's MusItemDraw_GetFreeIndex
static u16 func_ov112_021f4b54(MusItemDrawSys *sys) {
    int i;

    for (i = 0; i < sys->count; i++) {
        if (sys->items[i].active == FALSE) {
            break;
        }
    }
    if (i == sys->count) {
        return 0;
    }
    return i;
}

// pokebw2's MusItemDraw_AddItem
MusItemDraw *func_ov112_021f4b88(MusItemDrawSys *sys, u16 itemId, void *texResource, const VecFx32 *pos) {
    int index = func_ov112_021f4b54(sys);
    u32 size;
    u8 width;
    u8 height;

    sys->items[index].texFileId = func_ov112_021f4aa8(itemId);
    sys->items[index].data = func_ov112_021f4f54(sys->itemData, itemId);
    size = func_ov112_021f4f70(sys->items[index].data);
    func_ov112_021f4ac8(&sys->items[index], &width, &height);
    sys->items[index].material =
        BlActScene_AddMaterialNewTex(sys->blact, texResource, 0, size, width * 32, height * 32);
    sys->items[index].ownMaterial = TRUE;
    func_ov112_021f4c94(sys, index, pos, width, height);
    return &sys->items[index];
}

// pokebw2's MusItemDraw_AddItemWithMaterial
MusItemDraw *func_ov112_021f4c1c(MusItemDrawSys *sys, u16 itemId, u32 material, const VecFx32 *pos) {
    int index = func_ov112_021f4b54(sys);
    u8 width;
    u8 height;

    sys->items[index].texFileId = func_ov112_021f4aa8(itemId);
    sys->items[index].data = func_ov112_021f4f54(sys->itemData, itemId);
    func_ov112_021f4f70(sys->items[index].data);
    func_ov112_021f4ac8(&sys->items[index], &width, &height);
    sys->items[index].material = material;
    sys->items[index].ownMaterial = FALSE;
    func_ov112_021f4c94(sys, index, pos, width, height);
    return &sys->items[index];
}

// pokebw2's MusItemDraw_InitActor
static MusItemDraw *func_ov112_021f4c94(MusItemDrawSys *sys, u16 index, const VecFx32 *pos, s16 scaleX, s16 scaleY) {
    sys->items[index].actor =
        BlActScene_AddNewActor(sys->blact, sys->items[index].material, FX32_ONE, FX32_ONE, pos, 31, 0, 0);
    BlActScene_SetActorHidden(sys->blact, sys->items[index].actor, &data_ov112_021f8300);
    sys->items[index].active = TRUE;
    sys->items[index].update = TRUE;
    sys->items[index].useOffset = TRUE;
    sys->items[index].shadow = FALSE;
    sys->items[index].pos.x = pos->x;
    sys->items[index].pos.y = pos->y;
    sys->items[index].pos.z = pos->z;
    sys->items[index].scaleX = scaleX;
    sys->items[index].scaleY = scaleY;
    sys->items[index].rotation = 0;
    return &sys->items[index];
}

// pokebw2's MusItemDraw_DelItem
void func_ov112_021f4d40(MusItemDrawSys *sys, MusItemDraw *item) {
    BlActScene_ClearActorMaterial(sys->blact, item->actor);
    if (item->ownMaterial == TRUE) {
        func_0204f1a0(sys->blact, item->material);
    }
    item->active = FALSE;
    item->update = FALSE;
}

// pokebw2's MusItemDraw_ChangeItem
void func_ov112_021f4d74(MusItemDrawSys *sys, MusItemDraw *item, u16 itemId, u16 material) {
    item->texFileId = func_ov112_021f4aa8(itemId);
    item->data = func_ov112_021f4f54(sys->itemData, itemId);
    item->shadow = FALSE;
    func_0204f604(sys->blact, item->actor, &material);
    if (item->ownMaterial == TRUE) {
        func_0204f1a0(sys->blact, item->material);
    }
    item->ownMaterial = FALSE;
    item->material = material;
    item->active = TRUE;
    item->update = TRUE;
}

// pokebw2's MusItemDraw_SetShadowPalette
void func_ov112_021f4de0(MusItemDrawSys *sys, MusItemDraw *item, void *texResource) {
    if (item->shadow == FALSE) {
        u32 plttAddr;
        NNSGfdPlttKey plttKey;

        func_0204f2cc(sys->blact, item->material, &plttAddr);
        func_0204f32c(sys->blact, item->material, &plttKey);
        NNS_GfdRegisterNewVramTransferTask(1, plttAddr, sys->shadowPalette, NNS_GfdGetPlttKeySize(plttKey));
        item->shadow = TRUE;
    }
}

// pokebw2's MusItemDraw_CopyShadowPos
void func_ov112_021f4e2c(MusItemDrawSys *sys, MusItemDraw *item, MusItemDraw *shadow) {
    shadow->pos.x = item->pos.x + FX32_CONST(2);
    shadow->pos.y = item->pos.y + FX32_CONST(2);
    shadow->pos.z = item->pos.z - FX32_CONST(0.5);
    shadow->scaleX = item->scaleX;
    shadow->scaleY = item->scaleY;
    shadow->rotation = item->rotation;
    shadow->useOffset = item->useOffset;
}

// pokebw2's MusItemDraw_SetVisible
void func_ov112_021f4e6c(MusItemDrawSys *sys, MusItemDraw *item, BOOL visible) {
    BlActScene_SetActorHidden(sys->blact, item->actor, &visible);
    item->update = TRUE;
}

// pokebw2's MusItemDraw_SetPosition
void func_ov112_021f4e8c(MusItemDrawSys *sys, MusItemDraw *item, const VecFx32 *pos) {
    item->pos.x = pos->x;
    item->pos.y = pos->y;
    item->pos.z = pos->z;
    item->update = TRUE;
}

// pokebw2's MusItemDraw_GetPosition
void func_ov112_021f4ea4(MusItemDrawSys *sys, MusItemDraw *item, VecFx32 *pos) {
    pos->x = item->pos.x;
    pos->y = item->pos.y;
    pos->z = item->pos.z;
}

// pokebw2's MusItemDraw_SetSize
void func_ov112_021f4eb4(MusItemDrawSys *sys, MusItemDraw *item, s16 scaleX, s16 scaleY) {
    item->scaleX = scaleX;
    item->scaleY = scaleY;
    item->update = TRUE;
}

// pokebw2's MusItemDraw_GetSize
void func_ov112_021f4ec4(MusItemDrawSys *sys, MusItemDraw *item, s16 *scaleX, s16 *scaleY) {
    *scaleX = item->scaleX;
    *scaleY = item->scaleY;
}

// pokebw2's MusItemDraw_SetRotation
void func_ov112_021f4ed4(MusItemDrawSys *sys, MusItemDraw *item, u16 rotation) {
    item->rotation = rotation;
    item->update = TRUE;
}

// pokebw2's MusItemDraw_GetRotation
void func_ov112_021f4ee0(MusItemDrawSys *sys, MusItemDraw *item, u16 *rotation) {
    *rotation = item->rotation;
}

// pokebw2's MusItemDraw_SetUseOffset
void func_ov112_021f4ee8(MusItemDrawSys *sys, MusItemDraw *item, BOOL useOffset) {
    item->useOffset = useOffset;
}

// pokebw2's MusItemDraw_GetUseOffset
BOOL func_ov112_021f4efc(MusItemDrawSys *sys, MusItemDraw *item) {
    return item->useOffset;
}

// pokebw2's MusItemDraw_GetItemOffset
void func_ov112_021f4f04(MusItemDraw *item, s32 *offset) {
    func_ov112_021f4f60(item->data, offset);
}

// pokebw2's MusItemDraw_GetItemData
void *func_ov112_021f4f10(MusItemDrawSys *sys) {
    return sys->itemData;
}
