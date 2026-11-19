#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_59_app_0223b68c.h"
#include "overlay_59_juice_feed_app.h"

extern s32 ov59_0223C99C;

extern UnmanagedSpriteTemplate ov59_0223CA90;

extern UnmanagedSpriteTemplate ov59_0223CB08;

extern UnmanagedSpriteTemplate ov59_0223CB30;

void ov59_0223B8E4(Ov59JuiceFeedApp *arg0);

void ov59_0223B8E4(Ov59JuiceFeedApp *arg0) {
    s32 sp10;
    s32 *spC;
    s32 sp8;
    s32 sp4;
    s32 sp0;
    UnmanagedSpriteTemplate *var_r6;
    s32 temp_r0;
    s32 var_r7;
    s32 var_r7_2;
    void *temp_r4;
    void *temp_r4_2;

    var_r6 = &ov59_0223CA90;
    var_r7 = 0;
    do {
        ((Ov59App_0223B68C *)arg0)->unk_260[var_r7] = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, var_r6);
        Sprite_SetDrawFlag(((Ov59App_0223B68C *)arg0)->unk_260[var_r7], 1);
        Sprite_SetAnimActiveFlag(((Ov59App_0223B68C *)arg0)->unk_260[var_r7], 1);
        Sprite_SetPriority(((Ov59App_0223B68C *)arg0)->unk_260[var_r7], 1U);
        var_r6 = (void *)((u8 *)var_r6 + (0x28));
        var_r7 += 1;
    } while (var_r7 <= 2);
    Sprite_SetDrawFlag(arg0->sprites[2], 0);
    Sprite_SetAffineOverwriteMode(arg0->sprites[2], 2U);
    sp0 = 0;
    spC = &ov59_0223C99C;
    sp8 = 0x28;
    sp4 = 0x38;
    do {
        u32 var_r6_2;
        temp_r4 = (u8 *)arg0 + ((u32)((*spC + 3) << 0x18) >> 0x16);
        *(Sprite **)((u8 *)temp_r4 + 0x260) = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov59_0223CB08);
        Sprite_SetPositionXY(*(Sprite **)((u8 *)temp_r4 + 0x260), 0x50, (s16)sp8);
        Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)temp_r4 + 0x260), 1);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)temp_r4 + 0x260), 1);
        var_r7_2 = 0;
        var_r6_2 = 0x40;
        sp10 = (s32)sp4;
loop_4:
        temp_r4_2 = (u8 *)arg0 + ((u32)((var_r7_2 + ((*spC * 5) + 8)) << 0x18) >> 0x16);
        *(Sprite **)((u8 *)temp_r4_2 + 0x260) = SpriteSystem_CreateSpriteFromResourceHeader(arg0->spriteSystem, arg0->spriteManager, &ov59_0223CB30);
        Sprite_SetPositionXY(*(Sprite **)((u8 *)temp_r4_2 + 0x260), var_r6_2, (s16)sp10);
        Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)temp_r4_2 + 0x260), 1);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)temp_r4_2 + 0x260), 1);
        Sprite_SetAffineOverwriteMode(*(Sprite **)((u8 *)temp_r4_2 + 0x260), 2U);
        var_r7_2 += 1;
        var_r6_2 += 0x10;
        if (var_r7_2 < 5) {
            goto loop_4;
        }
        spC += 1;
        sp8 += 0x20;
        sp4 += 0x20;
        temp_r0 = sp0 + 1;
        sp0 = temp_r0;
    } while (temp_r0 < 5);
}
