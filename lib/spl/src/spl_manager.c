// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/spl/src/spl_manager.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "nnsys/gfd.h"
#include "spl/spl.h"
#include "spl_internal.h"

// The manager: the resource file's emitters and textures, the pools of emitters and particles, and the update and
// drawing of the active emitters

// VRAM for the textures and palettes from NitroSystem's default managers, as addresses
// pokebw2's SPLManager_AllocTexVram
static u32 func_02052f90(u32 size, BOOL is4x4comp) {
    return NNS_GfdGetTexKeyAddr(NNS_GfdAllocTexVram(size, is4x4comp, 0));
}

// pokebw2's SPLManager_AllocPlttVram
static u32 func_02052f58(u32 size, BOOL is4pltt) {
    return NNS_GfdGetPlttKeyAddr(NNS_GfdAllocPlttVram(size, is4pltt, 0));
}

SPLManager *func_02052dc8(SPLAllocFunc alloc, u16 maxEmitters, u16 maxParticles, u16 fixPolyID, u16 minPolyID,
                           u16 maxPolyID) {
    SPLManager *mgr = alloc(sizeof(SPLManager));
    SPLEmitter *emitters;
    SPLParticle *particles;
    int i;

    MI_CpuFill8(mgr, 0, sizeof(SPLManager));
    mgr->maxEmitters = maxEmitters;
    mgr->maxParticles = maxParticles;
    mgr->minPolygonID = minPolyID;
    mgr->maxPolygonID = maxPolyID;
    mgr->currentPolygonID = mgr->minPolygonID;
    mgr->fixPolygonID = fixPolyID;
    mgr->reverseDrawOrder = FALSE;
    mgr->reserved = 0;
    mgr->alloc = alloc;
    mgr->activeEmitters.count = 0;
    mgr->inactiveEmitters.count = 0;
    mgr->unusedParticles.count = 0;
    mgr->activeEmitters.first = NULL;
    mgr->inactiveEmitters.first = NULL;
    mgr->unusedParticles.first = NULL;
    mgr->polygonAttr = 0;
    mgr->frame = 0;

    emitters = alloc(maxEmitters * sizeof(SPLEmitter));
    MI_CpuFill8(emitters, 0, maxEmitters * sizeof(SPLEmitter));
    for (i = 0; i < maxEmitters; i++) {
        func_020583cc((SPLList *)&mgr->inactiveEmitters, (SPLNode *)&emitters[i]);
    }

    particles = alloc(maxParticles * sizeof(SPLParticle));
    MI_CpuFill8(particles, 0, maxParticles * sizeof(SPLParticle));
    for (i = 0; i < maxParticles; i++) {
        func_020583cc(&mgr->unusedParticles, (SPLNode *)&particles[i]);
    }

    mgr->resources = NULL;
    mgr->textures = NULL;
    mgr->resourceCount = mgr->textureCount = 0;
    return mgr;
}

// Points the resources into the file, which must stay loaded, at their header, animations, child resource and
// behaviors, each there only if the header's flags say so
// pokebw2's SPLManager_LoadResources
void func_02052a50(SPLManager *mgr, const void *data) {
    const SPLFileHeader *file = data;
    u32 offset = sizeof(SPLFileHeader);
    int i;

    mgr->resourceCount = file->resourceCount;
    mgr->textureCount = file->textureCount;
    mgr->resources = mgr->alloc(mgr->resourceCount * sizeof(SPLResource));
    MI_CpuFill8(mgr->resources, 0, mgr->resourceCount * sizeof(SPLResource));

    for (i = 0; i < mgr->resourceCount; i++) {
        SPLResource *resource;
        SPLResourceFlags flags;

        resource = &mgr->resources[i];
        resource->header = (SPLResourceHeader *)((u8 *)data + offset);
        flags = resource->header->flags;
        offset += sizeof(SPLResourceHeader);

        if (flags.hasScaleAnim) {
            resource->scaleAnim = (SPLScaleAnim *)((u8 *)data + offset);
            offset += sizeof(SPLScaleAnim);
        } else {
            resource->scaleAnim = NULL;
        }
        if (flags.hasColorAnim) {
            resource->colorAnim = (SPLColorAnim *)((u8 *)data + offset);
            offset += sizeof(SPLColorAnim);
        } else {
            resource->colorAnim = NULL;
        }
        if (flags.hasAlphaAnim) {
            resource->alphaAnim = (SPLAlphaAnim *)((u8 *)data + offset);
            offset += sizeof(SPLAlphaAnim);
        } else {
            resource->alphaAnim = NULL;
        }
        if (flags.hasTextureAnim) {
            resource->textureAnim = (SPLTextureAnim *)((u8 *)data + offset);
            offset += sizeof(SPLTextureAnim);
        } else {
            resource->textureAnim = NULL;
        }
        if (flags.hasChildResource) {
            resource->childResource = (SPLChildResource *)((u8 *)data + offset);
            offset += sizeof(SPLChildResource);
        } else {
            resource->childResource = NULL;
        }

        resource->behaviorCount = flags.hasGravityBehavior + flags.hasRandomBehavior + flags.hasMagnetBehavior +
                                  flags.hasSpinBehavior + flags.hasCollisionPlaneBehavior +
                                  flags.hasConvergenceBehavior;
        if (resource->behaviorCount != 0) {
            SPLBehavior *behavior;

            resource->behaviors = mgr->alloc(resource->behaviorCount * sizeof(SPLBehavior));
            behavior = resource->behaviors;
            if (flags.hasGravityBehavior) {
                behavior->object = (u8 *)data + offset;
                behavior->applyFunc = func_020582cc;
                offset += sizeof(SPLGravityBehavior);
                behavior++;
            }
            if (flags.hasRandomBehavior) {
                behavior->object = (u8 *)data + offset;
                behavior->applyFunc = func_02058204;
                offset += sizeof(SPLRandomBehavior);
                behavior++;
            }
            if (flags.hasMagnetBehavior) {
                behavior->object = (u8 *)data + offset;
                behavior->applyFunc = func_02058180;
                offset += sizeof(SPLMagnetBehavior);
                behavior++;
            }
            if (flags.hasSpinBehavior) {
                behavior->object = (u8 *)data + offset;
                behavior->applyFunc = func_020580a8;
                offset += sizeof(SPLSpinBehavior);
                behavior++;
            }
            if (flags.hasCollisionPlaneBehavior) {
                behavior->object = (u8 *)data + offset;
                behavior->applyFunc = func_02057f8c;
                offset += sizeof(SPLCollisionPlaneBehavior);
                behavior++;
            }
            if (flags.hasConvergenceBehavior) {
                behavior->object = (u8 *)data + offset;
                behavior->applyFunc = func_02057ef8;
                offset += sizeof(SPLConvergenceBehavior);
            }
        } else {
            resource->behaviors = NULL;
        }
    }

    mgr->textures = mgr->alloc(mgr->textureCount * sizeof(SPLTexture));
    MI_CpuFill8(mgr->textures, 0, mgr->textureCount * sizeof(SPLTexture));
    for (i = 0; i < mgr->textureCount; i++) {
        SPLTexture *entry = &mgr->textures[i];
        const SPLTextureResource *texture = (const SPLTextureResource *)((u8 *)data + offset);

        entry->data = texture;
        entry->width = 1 << (texture->param.s + 3);
        entry->height = 1 << (texture->param.t + 3);
        entry->param = texture->param;
        offset += texture->resourceSize;
    }
}

// pokebw2's SPLManager_UploadTexturesEx
BOOL func_02052984(SPLManager *mgr, SPLTexVRAMAllocFunc vramAlloc) {
    int i;

    GX_BeginLoadTex();
    for (i = 0; i < mgr->textureCount; i++) {
        SPLTexture *texture = &mgr->textures[i];
        const SPLTextureResource *data = texture->data;

        if (data->param.useSharedTexture) {
            texture->texAddr = mgr->textures[data->param.sharedTexID].texAddr;
        } else {
            u32 addr = vramAlloc(data->textureSize, data->param.format == GX_TEXFMT_COMP4x4 ? TRUE : FALSE);

            GX_LoadTex((u8 *)texture->data + sizeof(SPLTextureResource), addr, data->textureSize);
            texture->texAddr = addr;
        }
    }
    GX_EndLoadTex();
    return TRUE;
}

// pokebw2's SPLManager_UploadPalettesEx
BOOL func_020528d0(SPLManager *mgr, SPLPalVRAMAllocFunc vramAlloc) {
    int i;

    GX_BeginLoadTexPltt();
    for (i = 0; i < mgr->textureCount; i++) {
        SPLTexture *texture = &mgr->textures[i];
        const SPLTextureResource *data = texture->data;
        u32 addr = 0;

        if (data->paletteSize != 0) {
            addr = vramAlloc(data->paletteSize, data->param.format == GX_TEXFMT_PLTT4 ? TRUE : FALSE);
            GX_LoadTexPltt((u8 *)texture->data + data->paletteOffset, addr, data->paletteSize);
        }
        texture->palAddr = addr;
    }
    GX_EndLoadTexPltt();
    return TRUE;
}

// pokebw2's SPLManager_UploadTextures
BOOL func_020528bc(SPLManager *mgr) {
    return func_02052984(mgr, func_02052f90);
}

// pokebw2's SPLManager_UploadPalettes
BOOL func_020528a8(SPLManager *mgr) {
    return func_020528d0(mgr, func_02052f58);
}

// pokebw2's SPLManager_Update
void func_02052770(SPLManager *mgr) {
    SPLEmitter *emitter;
    SPLEmitter *next;

    for (emitter = mgr->activeEmitters.first; emitter != NULL; emitter = next) {
        SPLResourceHeader *header = emitter->resource->header;

        next = emitter->next;
        if (!emitter->state.started && emitter->age >= header->startDelay) {
            emitter->state.started = TRUE;
            emitter->age = 0;
        }

        // Emitters with an update cycle update on one frame of every two
        if (!emitter->state.paused && (emitter->updateCycle == 0 || mgr->frame == emitter->updateCycle - 1)) {
            func_02053218(mgr, emitter);
        }

        if ((header->flags.autoTerminate && header->emitterLifeTime != 0 && emitter->state.started &&
             emitter->age > header->emitterLifeTime) ||
            emitter->state.terminate) {
            if (emitter->particles.count == 0 && emitter->childParticles.count == 0) {
                func_020583cc((SPLList *)&mgr->inactiveEmitters,
                                  func_02058300((SPLList *)&mgr->activeEmitters, (SPLNode *)emitter));
            }
        }
    }

    mgr->frame++;
    if (mgr->frame > 1) {
        mgr->frame = 0;
    }
}

// pokebw2's SPLManager_Draw
void func_020526a8(SPLManager *mgr, const MtxFx43 *viewMatrix) {
    SPLEmitter *emitter;

    G3X_AlphaBlend(TRUE);
    mgr->viewMatrix = viewMatrix;
    if (!mgr->reverseDrawOrder) {
        for (emitter = mgr->activeEmitters.first; emitter != NULL; emitter = emitter->next) {
            mgr->drawEmitter = emitter;
            if (!emitter->state.hidden) {
                func_02052fc8(mgr);
            }
        }
    } else {
        for (emitter = mgr->activeEmitters.last; emitter != NULL; emitter = emitter->prev) {
            mgr->drawEmitter = emitter;
            if (!emitter->state.hidden) {
                func_02052fc8(mgr);
            }
        }
    }
}

// pokebw2's SPLManager_CreateEmitter
SPLEmitter *func_02052634(SPLManager *mgr, int resourceID, const VecFx32 *pos) {
    SPLEmitter *emitter = NULL;

    if (mgr->inactiveEmitters.first != NULL) {
        emitter = (SPLEmitter *)func_02058380((SPLList *)&mgr->inactiveEmitters);
        func_02053a60(emitter, &mgr->resources[resourceID], pos);
        func_020583cc((SPLList *)&mgr->activeEmitters, (SPLNode *)emitter);
        if (emitter->resource->header->flags.autoTerminate) {
            emitter = NULL;
        }
    }
    return emitter;
}

// Creates an emitter at the origin, which the callback can set up before it is made active
// pokebw2's SPLManager_CreateEmitterWithCallback
SPLEmitter *func_020525a0(SPLManager *mgr, int resourceID, SPLEmitterCallback initCallback) {
    SPLEmitter *emitter = NULL;

    if (mgr->inactiveEmitters.first != NULL) {
        VecFx32 pos = { 0, 0, 0 };

        emitter = (SPLEmitter *)func_02058380((SPLList *)&mgr->inactiveEmitters);
        func_02053a60(emitter, &mgr->resources[resourceID], &pos);
        if (initCallback != NULL) {
            initCallback(emitter);
        }
        func_020583cc((SPLList *)&mgr->activeEmitters, (SPLNode *)emitter);
        if (emitter->resource->header->flags.autoTerminate) {
            emitter = NULL;
        }
    }
    return emitter;
}

// pokebw2's SPLManager_DeleteEmitter
void func_0205250c(SPLManager *mgr, SPLEmitter *emitter) {
    SPLNode *node;

    while ((node = func_02058380(&emitter->particles)) != NULL) {
        func_020583cc(&mgr->unusedParticles, node);
    }
    while ((node = func_02058380(&emitter->childParticles)) != NULL) {
        func_020583cc(&mgr->unusedParticles, node);
    }
    func_02058300((SPLList *)&mgr->activeEmitters, (SPLNode *)emitter);
    func_020583cc((SPLList *)&mgr->inactiveEmitters, (SPLNode *)emitter);
}

// pokebw2's SPLManager_DeleteAllEmitters
void func_020524c8(SPLManager *mgr) {
    SPLEmitter *emitter = mgr->activeEmitters.first;
    SPLEmitter *next;

    while (emitter != NULL) {
        next = emitter->next;
        func_0205250c(mgr, emitter);
        emitter = next;
    }
}
