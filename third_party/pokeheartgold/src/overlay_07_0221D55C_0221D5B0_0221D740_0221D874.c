#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "vram_transfer_manager.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_tracking.h"

int ov07_0221D3CC(BattleAnimSystem *system, int role);

void ov07_0221D4B0(SysTask *task, void *param);

void ov07_0221D874(BattleAnimSystem *system);

void ov07_0221D874(BattleAnimSystem *system) {
    u32 k = 0x4e21;
    u32 a;
    void *gfx;
    s32 ids[6];
    s32 base;
    s32 i;
    NarcId narcID;
    PokemonSpriteData *ent;
    s32 memberNo;
    s32 mode;
    ManagedSpriteTemplate tmpl;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    ManagedSprite *spr;
    s32 idx;
    s32 e;

    a = *++system->scriptPtr;
    mode = *(++system->scriptPtr);
    idx = *(++system->scriptPtr);
    base = *(++system->scriptPtr);
    system->scriptPtr++;
    base += k;
    ids[0] = base + (k >> 2) * system->context->attacker;
    ids[1] = base + (k >> 2) * system->context->attacker;
    ids[2] = base + (k >> 2) * system->context->attacker;
    ids[3] = base + (k >> 2) * system->context->attacker;
    ids[4] = 0;
    ids[5] = 0;
    e = ov07_0221D3CC(system, a);
    ent = system->context->pokemonSpriteData[e];
    narcID = (NarcId)ent->narcID;
    memberNo = ent->palette;
    gfx = ent->tiles;
    pokepic = ov07_0221FA48(system, e);
    if (pokepic != NULL) {
        x = Pokepic_GetAttr(pokepic, 0);
        y = Pokepic_GetAttr(pokepic, 1);
        y = y - Pokepic_GetAttr(pokepic, 0x29);
    }
    tmpl.x = x;
    tmpl.y = y;
    tmpl.z = 0;
    tmpl.animation = 0;
    tmpl.drawPriority = 100;
    tmpl.pal = 0;
    tmpl.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    tmpl.bgPriority = 1;
    tmpl.vramTransfer = 0;
    for (i = 0; i < 6; i++) {
        tmpl.resIdList[i] = ids[i];
    }
    spr = SpriteSystem_NewSprite(system->context->spriteSystem, system->pokemonSpriteManager, &tmpl);
    if (pokepic == NULL) {
        ManagedSprite_SetDrawFlag(spr, 0);
    } else if (Pokepic_GetAttr(pokepic, 6) == 1) {
        ManagedSprite_SetDrawFlag(spr, 0);
    }
    if (ov07_0221FA48(system, e) != 0) {
        NNSG2dImageProxy *img = Sprite_GetImageProxy(spr->sprite);
        GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_OBJ_CHAR_MAIN, img->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN], gfx, 0xc80);
    }
    if (ov07_0221FA48(system, e) != 0) {
        NNSG2dImagePaletteProxy *prox = Sprite_GetPaletteProxy(spr->sprite);
        u32 off = ObjPlttTransfer_GetPaletteVramOffset(prox, NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_LoadNarc(system->paletteData, narcID, memberNo, (enum HeapID)system->heapID, PLTTBUF_MAIN_OBJ, 0x20, off << 4);
    }
    if (system->pokemonSprites[idx] != NULL) {
        GF_AssertFail();
    }
    system->pokemonSprites[idx] = spr;
    system->usedPokemonSprites[idx] = 1;
    if (mode == 1 && ov07_0221FA48(system, e) != 0) {
        system->spriteTrackingTasks[idx] = Heap_Alloc((enum HeapID)system->heapID, sizeof(SpriteTrackingTask));
        system->spriteTrackingTasks[idx]->sprite = spr;
        system->spriteTrackingTasks[idx]->data.sprite = ov07_0221FA48(system, e);
        system->spriteTrackingTasks[idx]->data.frameCount = 0;
        system->spriteTrackingTasks[idx]->data.interval = 0;
        system->spriteTrackingTasks[idx]->task = SysTask_CreateOnMainQueue(ov07_0221D4B0, system->spriteTrackingTasks[idx], 0x1001);
    }
}
#else
// DS form: the order mwcc gives the spilled locals (idx, mode, memberNo, narcID, e, gfx) only comes out of the old
// 32-bit view of BattleAnimSystem; the typed form above is the one the PC builds.
typedef enum HeapID HeapID;

s32 ov07_0221D3CC(enum HeapID *, u32);

s32 ov07_0221FA48(enum HeapID *, s32);

typedef struct LoaderEnt {
    void *gfx;
    NarcId narc;
    s32 member;
} LoaderEnt;

typedef struct LoaderSys {
    u8 pad00[0x14];
    u16 stride;
    u8 pad16[0xAC - 0x16];
    SpriteSystem *spriteSystem;
    LoaderEnt *ents[1];
} LoaderSys;

typedef struct LoaderTask {
    ManagedSprite *sprite;
    u8 pad04;
    u8 pad05;
    u8 pad06[2];
    u32 unk_08;
    SysTask *task;
} LoaderTask;

typedef struct Loader {
    enum HeapID heapID;
    u8 pad04[0x14];
    u32 *cur;
    u8 pad1C[0xC0 - 0x1C];
    LoaderSys *sys;
    u8 padC4[4];
    PaletteData *palette;
    u8 padCC[0x138 - 0xCC];
    SpriteManager *spriteManager;
    ManagedSprite *sprites[5];
    int flags[5];
    LoaderTask *tasks[5];
} Loader;

void ov07_0221D4B0(SysTask *task, void *param);

void ov07_0221D874(Loader *p, s32 arg1, s32 arg2, s32 arg3);

void ov07_0221D874(Loader *p, s32 arg1, s32 arg2, s32 arg3) {
    u32 k = 0x4e21;
    u32 a;
    void *gfx;
    s32 ids[6];
    s32 base;
    s32 e;
    s32 i;
    NarcId narcID;
    LoaderEnt *ent;
    s32 memberNo;
    s32 mode;
    ManagedSpriteTemplate tmpl;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    ManagedSprite *spr;
    s32 idx;

    a = *++p->cur;
    mode = *(++p->cur);
    idx = *(++p->cur);
    base = *(++p->cur);
    p->cur++;
    base += k;
    ids[0] = base + (k >> 2) * p->sys->stride;
    ids[1] = base + (k >> 2) * p->sys->stride;
    ids[2] = base + (k >> 2) * p->sys->stride;
    ids[3] = base + (k >> 2) * p->sys->stride;
    ids[4] = 0;
    ids[5] = 0;
    e = ov07_0221D3CC((enum HeapID *)p, a);
    ent = p->sys->ents[e];
    narcID = ent->narc;
    memberNo = ent->member;
    gfx = ent->gfx;
    pokepic = (Pokepic *)ov07_0221FA48((enum HeapID *)p, e);
    if (pokepic != NULL) {
        x = Pokepic_GetAttr(pokepic, 0);
        y = Pokepic_GetAttr(pokepic, 1);
        y = y - Pokepic_GetAttr(pokepic, 0x29);
    }
    tmpl.x = x;
    tmpl.y = y;
    tmpl.z = 0;
    tmpl.animation = 0;
    tmpl.drawPriority = 100;
    tmpl.pal = 0;
    tmpl.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    tmpl.bgPriority = 1;
    tmpl.vramTransfer = 0;
    for (i = 0; i < 6; i++) {
        tmpl.resIdList[i] = ids[i];
    }
    spr = SpriteSystem_NewSprite(p->sys->spriteSystem, p->spriteManager, &tmpl);
    if (pokepic == NULL) {
        ManagedSprite_SetDrawFlag(spr, 0);
    } else if (Pokepic_GetAttr(pokepic, 6) == 1) {
        ManagedSprite_SetDrawFlag(spr, 0);
    }
    if (ov07_0221FA48((enum HeapID *)p, e) != 0) {
        NNSG2dImageProxy *img = Sprite_GetImageProxy(spr->sprite);
        GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_OBJ_CHAR_MAIN, img->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN], gfx, 0xc80);
    }
    if (ov07_0221FA48((enum HeapID *)p, e) != 0) {
        NNSG2dImagePaletteProxy *prox = Sprite_GetPaletteProxy(spr->sprite);
        u32 off = ObjPlttTransfer_GetPaletteVramOffset(prox, NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_LoadNarc(p->palette, narcID, memberNo, p->heapID, PLTTBUF_MAIN_OBJ, 0x20, off << 4);
    }
    if (p->sprites[idx] != NULL) {
        GF_AssertFail();
    }
    p->sprites[idx] = spr;
    p->flags[idx] = 1;
    if (mode == 1 && ov07_0221FA48((enum HeapID *)p, e) != 0) {
        p->tasks[idx] = Heap_Alloc(p->heapID, sizeof(LoaderTask));
        p->tasks[idx]->sprite = spr;
        p->tasks[idx]->unk_08 = ov07_0221FA48((enum HeapID *)p, e);
        p->tasks[idx]->pad04 = 0;
        p->tasks[idx]->pad05 = 0;
        p->tasks[idx]->task = SysTask_CreateOnMainQueue(ov07_0221D4B0, p->tasks[idx], 0x1001);
    }
}
#endif
