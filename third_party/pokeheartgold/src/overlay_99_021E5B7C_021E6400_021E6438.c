#include "global.h"

#include "managed_sprite_template.h"
#include "overlay_98_sprite_ctx.h"
#include "overlay_99.h"
#include "overlay_99_pokeathlon_course_record.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov98_0221EF64(u32);

extern ManagedSpriteTemplate ov99_021E9840[];

void ov99_021E6438(PokeathlonCourseRecord *param_1);

void ov99_021E6438(PokeathlonCourseRecord *param_1) {
    u32 uVar1;
    ManagedSprite *managedSprite;
    s32 iVar2;
    s32 iVar5;
    ManagedSpriteTemplate *puVar3;
#if defined(SDK_PORT) || defined(NONMATCHING)
    ManagedSprite **puVar4;
    ManagedSprite **q;
#define SPRITE(p, k) ((p)[(k)-6])
#else
    u32 *puVar4;
    u32 *q;
#define SPRITE(p, k) ((ManagedSprite *)(p)[k])
#endif
    s32 j;
    puVar3 = ov99_021E9840;
    iVar5 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    puVar4 = param_1->sprites;
#else
    puVar4 = (u32 *)param_1;
#endif
    do {
        managedSprite = ov98_0221E6E0(param_1->spriteContext, puVar3);
#if defined(SDK_PORT) || defined(NONMATCHING)
        puVar4[0] = managedSprite;
#else
        puVar4[6] = (u32)managedSprite;
#endif
        ManagedSprite_SetAnimateFlag(managedSprite, 1);
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
        iVar5 = iVar5 + 1;
    } while (iVar5 < 0x1a);
    j = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    q = param_1->sprites;
#else
    q = (u32 *)param_1;
#endif
    do {
        iVar2 = ov99_021E5BB4(param_1->args, j);
        if (iVar2 == 0) {
            ManagedSprite_SetDrawFlag(SPRITE(q, 7), 0);
        }

        iVar2 = ov98_0221EF64(ov99_021E5B90(param_1->args, j));
        uVar1 = j >> 0x1f;
        if (iVar2 == 2) {
            if ((j % 2) == 0) {
                ManagedSprite_SetAnim(SPRITE(q, 0xc), 5);
            } else {
                ManagedSprite_SetAnim(SPRITE(q, 0xc), 6);
            }
        } else if (iVar2 == 1) {
            if ((j % 2) == 0) {
                ManagedSprite_SetAnim(SPRITE(q, 0xc), 2);
            } else {
                ManagedSprite_SetAnim(SPRITE(q, 0xc), 3);
            }
        } else {
            ManagedSprite_SetDrawFlag(SPRITE(q, 0xc), 0);
        }
        j = j + 1;
        q = q + 1;
    } while (j < 5);
    return;
}
#undef SPRITE
