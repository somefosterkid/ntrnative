#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"
#include "pokepic.h"
#include "unk_02016EDC.h"

struct SysTask;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

void sub_020170C4(SysTask *task, void *monAnim);

void sub_02016F40(PokepicAnimManager *monAnimMan, Pokepic *monSprite, const struct UnkStruct_0207294C *animTemplate, u8 index);

void sub_02016F40(PokepicAnimManager *monAnimMan, Pokepic *monSprite, const struct UnkStruct_0207294C *animTemplate, u8 index) {
    int animNum = animTemplate->animation;
    int startDelay = animTemplate->startDelay;

    ((index < monAnimMan->animCount) ? (void)0 : GF_AssertFail());
    ((monAnimMan->anims[index].active == 0) ? (void)0 : GF_AssertFail());

    MI_CpuClear8(&monAnimMan->anims[index], sizeof(PokemonAnim));

    monAnimMan->anims[index].active = 1;
    monAnimMan->anims[index].sprite = monSprite;

    if (animNum >= 143) {
        animNum = 0;
        startDelay = 0;
    }

    monAnimMan->anims[index].animNum = animNum;

    if (monAnimMan->flipSprite) {
        monAnimMan->anims[index].flipSprite = animTemplate->flipSprite;
    } else {
        monAnimMan->anims[index].flipSprite = 0;
    }

    monAnimMan->anims[index].scriptData = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_0_9_0, monAnimMan->anims[index].animNum, monAnimMan->heapID);
    monAnimMan->anims[index].scriptPtr = (u32 *)monAnimMan->anims[index].scriptData;
    monAnimMan->anims[index].endAnim = 0;
    monAnimMan->anims[index].completed = 0;
    monAnimMan->anims[index].waitForTransform = 0;
    monAnimMan->anims[index].yNormalization = Y_NORMALIZATION_OFF;
    monAnimMan->anims[index].fadeActive = 0;
    monAnimMan->anims[index].task = SysTask_CreateOnMainQueue(sub_020170C4, &monAnimMan->anims[index], 0);
    monAnimMan->anims[index].startDelay = startDelay;
    monAnimMan->anims[index].originalX = Pokepic_GetAttr(monSprite, MON_SPRITE_X_CENTER);
    monAnimMan->anims[index].originalY = Pokepic_GetAttr(monSprite, MON_SPRITE_Y_CENTER);
    monAnimMan->anims[index].translateX = 0;
    monAnimMan->anims[index].translateY = 0;
    monAnimMan->anims[index].offsetX = 0;
    monAnimMan->anims[index].offsetY = 0;
    monAnimMan->anims[index].scaleX = 0;
    monAnimMan->anims[index].scaleY = 0;
    monAnimMan->anims[index].rotationZ = 0;
}
