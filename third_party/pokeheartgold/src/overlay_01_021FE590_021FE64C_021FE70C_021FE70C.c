#include "global.h"

#include "billboard.h"
#include "overworld_anim_manager.h"
#include "overlay_01_021FE6B4.h"

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F1C(Billboard *billboard, fx32 frameNum);

void ov01_021F1640(OverworldAnimManager *animMan);

void ov01_021FE70C(OverworldAnimManager *effect, BerryPatchSparkleEffect *sparkleEffect) {

    fx32 animationSpeeds[5] = { 4096, 4096, 8192, 8192, 4096 };

    if (sparkleEffect->isAnimating == 1) {
        sparkleEffect->isAnimating = 0;
        sparkleEffect->animationFrame++;

        if (sparkleEffect->animationFrame >= ((int)(sizeof(animationSpeeds) / sizeof(*(animationSpeeds))))) {
            ov01_021F1640(effect);
            return;
        }

        sub_02023F1C(sparkleEffect->graphicsObject, 0);
    }

    if (sub_02023F04(sparkleEffect->graphicsObject, animationSpeeds[sparkleEffect->animationFrame]) != 1) {
        return;
    }

    sparkleEffect->isAnimating = 1;
}
