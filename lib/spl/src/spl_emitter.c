// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/spl/src/spl_emitter.c, with Black's names.
#include "types.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "spl/spl.h"
#include "spl_internal.h"

// An emitter: its particles' life, from emission through animation, behaviors and movement, and their drawing

// An animation to run on every particle, over its life or its loop
typedef struct {
    SPLAnimFunc func;
    BOOL loop;
} SPLAnimEntry;

// Loads a texture, and scales texture coordinates from [0, 1] to its size
static void func_02053d30(SPLTexture *texture) {
    SPLTextureParam param = texture->param;

    G3_TexImageParam(param.format, GX_TEXGEN_TEXCOORD, param.s, param.t, param.repeat, param.flip, param.palColor0,
                     texture->texAddr);
    G3_TexPlttBase(texture->palAddr, param.format);
    G3_MtxMode(GX_MTXMODE_TEXTURE);
    G3_Identity();
    G3_Scale(texture->width * FX32_ONE, texture->height * FX32_ONE, 0);
    G3_MtxMode(GX_MTXMODE_POSITION);
}

// For particles without a texture animation, which all have the resource's texture
static void func_02053d2c(SPLTexture *texture) {
}

void func_02053a60(SPLEmitter *emitter, SPLResource *resource, const VecFx32 *pos) {
    SPLResourceHeader *header;

    emitter->resource = resource;
    emitter->state.all = 0;
    emitter->position.x = pos->x + emitter->resource->header->emitterBasePos.x;
    emitter->position.y = pos->y + emitter->resource->header->emitterBasePos.y;
    emitter->position.z = pos->z + emitter->resource->header->emitterBasePos.z;
    emitter->particleInitVelocity.x = 0;
    emitter->particleInitVelocity.y = 0;
    emitter->particleInitVelocity.z = 0;
    emitter->velocity.x = emitter->velocity.y = emitter->velocity.z = 0;
    emitter->age = 0;
    emitter->emissionCountFractional = 0;

    header = emitter->resource->header;
    emitter->axis = header->axis;
    emitter->initAngle = emitter->resource->header->initAngle;
    emitter->emissionCount = emitter->resource->header->emissionCount;
    emitter->radius = emitter->resource->header->radius;
    emitter->length = emitter->resource->header->length;
    emitter->initVelPositionAmplifier = emitter->resource->header->initVelPositionAmplifier;
    emitter->initVelAxisAmplifier = emitter->resource->header->initVelAxisAmplifier;
    emitter->baseScale = emitter->resource->header->baseScale;
    emitter->particleLifeTime = emitter->resource->header->particleLifeTime;
    emitter->color = GX_RGB(31, 31, 31);
    emitter->emissionInterval = emitter->resource->header->emissionInterval;
    emitter->baseAlpha = emitter->resource->header->baseAlpha;
    emitter->updateCycle = 0;
    emitter->reserved = 0;
    emitter->collisionPlaneHeight = FX32_MIN;

    emitter->textureS = FX16_ONE << emitter->resource->header->textureRepeatShiftS;
    emitter->textureT = FX16_ONE << emitter->resource->header->textureRepeatShiftT;
    if (emitter->resource->header->flipTextureS) {
        emitter->textureS *= -1;
    }
    if (emitter->resource->header->flipTextureT) {
        emitter->textureT *= -1;
    }
    if (emitter->resource->header->flags.hasChildResource) {
        emitter->childTextureS = FX16_ONE << emitter->resource->childResource->textureRepeatShiftS;
        emitter->childTextureT = FX16_ONE << emitter->resource->childResource->textureRepeatShiftT;
        if (emitter->resource->childResource->flipTextureS) {
            emitter->childTextureS *= -1;
        }
        if (emitter->resource->childResource->flipTextureT) {
            emitter->childTextureT *= -1;
        }
    }

    emitter->next = emitter->prev = NULL;
    emitter->particles.first = emitter->childParticles.first = NULL;
    emitter->particles.count = emitter->childParticles.count = 0;
    emitter->updateCallback = NULL;
    emitter->userDataPtr = NULL;
    emitter->unk98 = 0;
}

void func_02053218(SPLManager *mgr, SPLEmitter *emitter) {
    SPLParticle *next;
    SPLResource *resource = emitter->resource;
    SPLChildResource *child = resource->childResource;
    SPLResourceHeader *header = resource->header;
    SPLResourceFlags flags = header->flags;
    SPLParticle *particle;
    int i;
    int animCount = 0;
    fx32 airResistance = header->airResistance + 384;
    int behaviorCount = resource->behaviorCount;
    u8 lifeRates[2];
    SPLAnimEntry anims[4];
    SPLAnimEntry childAnims[4];
    VecFx32 acc;

    if (emitter->updateCallback != NULL) {
        emitter->updateCallback(emitter, SPL_EMITTER_CALLBACK_FRONT);
    }

    if ((header->emitterLifeTime == 0 || emitter->age < header->emitterLifeTime) &&
        emitter->age % emitter->emissionInterval == 0 && !emitter->state.terminate && !emitter->state.stopEmission &&
        emitter->state.started) {
        func_020569a4(emitter, &mgr->unusedParticles);
    }

    // Color and texture animations that start at random are not animated
    if (flags.hasScaleAnim) {
        anims[animCount].func = func_02057e68;
        anims[animCount].loop = resource->scaleAnim->loop;
        animCount++;
    }
    if (flags.hasColorAnim && !resource->colorAnim->randomStart) {
        anims[animCount].func = func_02057c9c;
        anims[animCount++].loop = resource->colorAnim->loop;
    }
    if (flags.hasAlphaAnim) {
        anims[animCount].func = func_02057bc0;
        anims[animCount++].loop = resource->alphaAnim->loop;
    }
    if (flags.hasTextureAnim && !resource->textureAnim->randomStart) {
        anims[animCount].func = func_02057b5c;
        anims[animCount].loop = resource->textureAnim->loop;
        animCount++;
    }

    for (particle = (SPLParticle *)emitter->particles.first; particle != NULL; particle = next) {
        next = particle->next;

        // The life rates over the particle's life and its loop
        lifeRates[0] = (particle->lifeTimeFactor * particle->age) >> 8;
        lifeRates[1] = particle->lifeRateOffset + ((particle->loopTimeFactor * particle->age) >> 8);
        for (i = 0; i < animCount; i++) {
            anims[i].func(particle, resource, lifeRates[anims[i].loop]);
        }

        acc.x = acc.y = acc.z = 0;
        if (flags.followEmitter) {
            particle->emitterPos = emitter->position;
        }
        for (i = 0; i < behaviorCount; i++) {
            resource->behaviors[i].applyFunc(resource->behaviors[i].object, particle, &acc, emitter);
        }

        particle->rotation += particle->angularVelocity;
        particle->velocity.x = (particle->velocity.x * airResistance) >> 9;
        particle->velocity.y = (particle->velocity.y * airResistance) >> 9;
        particle->velocity.z = (particle->velocity.z * airResistance) >> 9;
        particle->velocity.x += acc.x;
        particle->velocity.y += acc.y;
        particle->velocity.z += acc.z;
        particle->position.x += particle->velocity.x + emitter->velocity.x;
        particle->position.y += particle->velocity.y + emitter->velocity.y;
        particle->position.z += particle->velocity.z + emitter->velocity.z;

        if (flags.hasChildResource) {
            fx32 sinceDelay = (particle->age << FX32_SHIFT) -
                              (FX_Mul(particle->lifeTime << FX32_SHIFT, child->emissionDelay << FX32_SHIFT) >> 8);

            if (sinceDelay >= 0 && (sinceDelay >> FX32_SHIFT) % child->emissionInterval == 0) {
                func_02056684(particle, emitter, &mgr->unusedParticles);
            }
        }

        if (emitter->resource->header->flags.fixedPolygonID) {
            particle->polygonId = mgr->fixPolygonID;
        } else {
            particle->polygonId = mgr->currentPolygonID;
            mgr->currentPolygonID++;
            if (mgr->currentPolygonID > mgr->maxPolygonID) {
                mgr->currentPolygonID = mgr->minPolygonID;
            }
        }

        particle->age++;
        if (particle->age > particle->lifeTime) {
            func_020583cc(&mgr->unusedParticles, func_02058300(&emitter->particles, (SPLNode *)particle));
        }
    }

    if (flags.hasChildResource) {
        animCount = 0;
        if (child->hasScaleAnim) {
            childAnims[animCount].func = func_02057b10;
            childAnims[animCount].loop = FALSE;
            animCount++;
        }
        if (child->hasAlphaAnim) {
            childAnims[animCount].func = func_02057ab8;
            childAnims[animCount].loop = FALSE;
            animCount++;
        }
        if (!child->hasBehaviors) {
            behaviorCount = 0;
        }

        for (particle = (SPLParticle *)emitter->childParticles.first; particle != NULL; particle = next) {
            next = particle->next;

            lifeRates[0] = (particle->age << 8) / particle->lifeTime;
            for (i = 0; i < animCount; i++) {
                childAnims[i].func(particle, resource, lifeRates[0]);
            }

            acc.x = acc.y = acc.z = 0;
            if (child->followEmitter) {
                particle->emitterPos = emitter->position;
            }
            for (i = 0; i < behaviorCount; i++) {
                resource->behaviors[i].applyFunc(resource->behaviors[i].object, particle, &acc, emitter);
            }

            particle->rotation += particle->angularVelocity;
            particle->velocity.x = (particle->velocity.x * airResistance) >> 9;
            particle->velocity.y = (particle->velocity.y * airResistance) >> 9;
            particle->velocity.z = (particle->velocity.z * airResistance) >> 9;
            particle->velocity.x += acc.x;
            particle->velocity.y += acc.y;
            particle->velocity.z += acc.z;
            particle->position.x += particle->velocity.x + emitter->velocity.x;
            particle->position.y += particle->velocity.y + emitter->velocity.y;
            particle->position.z += particle->velocity.z + emitter->velocity.z;

            if (emitter->resource->header->flags.childFixedPolygonID) {
                particle->polygonId = mgr->fixPolygonID;
            } else {
                particle->polygonId = mgr->currentPolygonID;
                mgr->currentPolygonID++;
                if (mgr->currentPolygonID > mgr->maxPolygonID) {
                    mgr->currentPolygonID = mgr->minPolygonID;
                }
            }

            particle->age++;
            if (particle->age > particle->lifeTime) {
                func_020583cc(&mgr->unusedParticles, func_02058300(&emitter->childParticles, (SPLNode *)particle));
            }
        }
    }

    emitter->age++;
    if (emitter->updateCallback != NULL) {
        emitter->updateCallback(emitter, SPL_EMITTER_CALLBACK_BACK);
    }
}

static void func_0205312c(SPLManager *mgr) {
    SPLEmitter *emitter = mgr->drawEmitter;
    SPLResourceHeader *header = emitter->resource->header;
    SPLDrawFunc draw = NULL;
    void (*setTexture)(SPLTexture *texture);
    SPLParticle *particle;

    func_02053d30(&mgr->textures[header->texture]);
    switch (header->flags.drawType) {
    case SPL_DRAW_BILLBOARD:
        draw = func_02055fd4;
        break;
    case SPL_DRAW_DIRECTIONAL_BILLBOARD:
        draw = func_02055498;
        break;
    case SPL_DRAW_POLYGON:
        draw = func_02054a68;
        break;
    case SPL_DRAW_DIRECTIONAL_POLYGON:
        draw = func_0205429c;
        break;
    case SPL_DRAW_DIRECTIONAL_POLYGON_CENTER:
        draw = func_0205429c;
        break;
    }

    if (header->flags.hasTextureAnim) {
        setTexture = func_02053d30;
    } else {
        setTexture = func_02053d2c;
    }
    for (particle = (SPLParticle *)emitter->particles.first; particle != NULL; particle = particle->next) {
        setTexture(&mgr->textures[particle->texture]);
        draw(mgr, particle);
    }
}

static void func_02053044(SPLManager *mgr) {
    SPLEmitter *emitter = mgr->drawEmitter;
    SPLResource *resource = emitter->resource;
    SPLDrawFunc draw = NULL;
    SPLParticle *particle;

    if (!resource->header->flags.hasChildResource) {
        return;
    }

    func_02053d30(&mgr->textures[resource->childResource->texture]);
    switch (resource->childResource->drawType) {
    case SPL_DRAW_BILLBOARD:
        draw = func_02055b80;
        break;
    case SPL_DRAW_DIRECTIONAL_BILLBOARD:
        draw = func_02054db8;
        break;
    case SPL_DRAW_POLYGON:
        draw = func_02054718;
        break;
    case SPL_DRAW_DIRECTIONAL_POLYGON:
        draw = func_02053e1c;
        break;
    case SPL_DRAW_DIRECTIONAL_POLYGON_CENTER:
        draw = func_02053e1c;
        break;
    }

    for (particle = (SPLParticle *)emitter->childParticles.first; particle != NULL; particle = particle->next) {
        draw(mgr, particle);
    }
}

void func_02052fc8(SPLManager *mgr) {
    SPLResourceHeader *header = mgr->drawEmitter->resource->header;

    if (header->flags.drawChildrenFirst) {
        func_02053044(mgr);
        if (!header->flags.hideParent) {
            func_0205312c(mgr);
        }
    } else {
        if (!header->flags.hideParent) {
            func_0205312c(mgr);
        }
        func_02053044(mgr);
    }
}
