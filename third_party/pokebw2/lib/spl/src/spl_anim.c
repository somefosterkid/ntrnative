#include "types.h"
#include "nitro/fx.h"
#include "nitro/gx.h"
#include "spl/spl.h"
#include "spl_internal.h"

// The animations of a particle over its life, given as a life rate from 0 to 255

void SPLAnim_Scale(SPLParticle *particle, SPLResource *resource, int lifeRate) {
    const SPLScaleAnim *anim = resource->scaleAnim;
    int in = anim->in;
    int out = anim->out;

    if (lifeRate < in) {
        particle->animScale = anim->start + (anim->mid - anim->start) * lifeRate / in;
    } else if (lifeRate < out) {
        particle->animScale = anim->mid;
    } else {
        particle->animScale = anim->end + (anim->end - anim->mid) * (lifeRate - 255) / (255 - out);
    }
}

void SPLAnim_Color(SPLParticle *particle, SPLResource *resource, int lifeRate) {
    const SPLColorAnim *anim = resource->colorAnim;
    const SPLResourceHeader *header = resource->header;
    int in = anim->in;
    int peak = anim->peak;
    int out = anim->out;

    if (lifeRate < in) {
        particle->color = anim->start;
    } else if (lifeRate < peak) {
        int toR;
        int toB;
        int fromR;
        int toG;
        int fromG;
        int fromB;
        GXRgb from;
        GXRgb to;

        to = header->color;
        from = anim->start;
        toR = to & 0x1f;
        toG = (to >> 5) & 0x1f;
        toB = (to >> 10) & 0x1f;
        fromR = from & 0x1f;
        fromG = (from >> 5) & 0x1f;
        fromB = (from >> 10) & 0x1f;
        if (!anim->interpolate) {
            particle->color = GX_RGB(toR, toG, toB);
        } else {
            particle->color = GX_RGB(fromR + (lifeRate - in) * (toR - fromR) / (peak - in),
                                     fromG + (lifeRate - in) * (toG - fromG) / (peak - in),
                                     fromB + (lifeRate - in) * (toB - fromB) / (peak - in));
        }
    } else if (lifeRate < out) {
        int toB;
        int fromR;
        int toR;
        int fromG;
        int toG;
        int fromB;
        GXRgb from;
        GXRgb to;

        to = anim->end;
        from = header->color;
        fromR = from & 0x1f;
        fromG = (from >> 5) & 0x1f;
        fromB = (from >> 10) & 0x1f;
        toR = to & 0x1f;
        toG = (to >> 5) & 0x1f;
        toB = (to >> 10) & 0x1f;
        if (!anim->interpolate) {
            particle->color = GX_RGB(toR, toG, toB);
        } else {
            particle->color = GX_RGB(fromR + (lifeRate - peak) * (toR - fromR) / (out - peak),
                                     fromG + (lifeRate - peak) * (toG - fromG) / (out - peak),
                                     fromB + (lifeRate - peak) * (toB - fromB) / (out - peak));
        }
    } else {
        particle->color = anim->end;
    }
}

// The alpha is scaled down by a random part of randomRange every frame, which makes particles flicker
void SPLAnim_Alpha(SPLParticle *particle, SPLResource *resource, int lifeRate) {
    const SPLAlphaAnim *anim = resource->alphaAnim;
    int in = anim->in;
    int out = anim->out;
    int alpha;

    if (lifeRate < in) {
        alpha = anim->start + (anim->mid - anim->start) * lifeRate / in;
    } else if (lifeRate < out) {
        alpha = anim->mid;
    } else {
        alpha = anim->end + (anim->end - anim->mid) * (lifeRate - 255) / (255 - out);
    }
    particle->animAlpha = (alpha * (255 - ((anim->randomRange * (int)(SPLRandom_Next() >> 24)) >> 8))) >> 8;
}

void SPLAnim_Texture(SPLParticle *particle, SPLResource *resource, int lifeRate) {
    SPLTextureAnim *anim = resource->textureAnim;
    int i;

    for (i = 0; i < anim->count; i++) {
        if (lifeRate < anim->step * (i + 1)) {
            particle->texture = anim->textures[i];
            return;
        }
    }
}

void SPLAnim_ChildScale(SPLParticle *particle, SPLResource *resource, int lifeRate) {
    fx16 endScale = resource->childResource->endScale;

    particle->animScale = endScale + (endScale - FX16_ONE) * (lifeRate - 255) / 255;
}

// Child particles fade out over their lives
void SPLAnim_ChildAlpha(SPLParticle *particle, SPLResource *resource, int lifeRate) {
    particle->animAlpha = (255 - lifeRate) * 31 / 255;
}
