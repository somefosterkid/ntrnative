#include "global.h"
struct BattleFactoryApp;
#include "pokepic.h"
#include "heap.h"
#include "overlay_81.h"
#include "pokemon_summary_args.h"
#include "overlay_manager.h"
#include "overlay_81_battle_factory_app.h"

BattleFactoryAppPokeballSprite *ov81_022412C4(BattleFactoryApp *, s32);

void ov81_02242EB8(BattleFactoryAppPokeballSprite *ballSprite, BOOL draw);

void ov81_02242F48(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);

void ov81_02242FB0(BattleFactoryAppPokeballSprite *ballSprite, int palIndex);

s32 ov81_02241DDC(void *, s32);
void ov81_0223EC44(struct BattleFactoryApp *arg0);

s32 ov81_02240CD4(void *);

void ov81_0223EBE4(BattleFactoryApp *app) {
    s32 v1;

    v1 = 0;
    if ((s32) app->numPokeballs > 0) {
        do {
            app->ballSprites[v1] = ov81_022412C4(app, v1);
            ov81_02242EB8(app->ballSprites[v1], 1);
            if (!app->exchangeMonSelected) {
                ov81_02242F48(app->ballSprites[v1]);
                ov81_02242FB0(app->ballSprites[v1], 0);
                ov81_02242F94(app->ballSprites[v1], 0);
            }
            v1 += 1;
        } while (v1 < (s32) app->numPokeballs);
    }
}

void ov81_0223EC44(BattleFactoryApp *arg0) {
    u32 v0;
    unsigned short new_var2;
    void *new_var;
    ov81_02241DDC(arg0, 1);
    new_var = arg0;
    v0 = 0;
    if (((u32)(((BattleFactoryApp *)new_var)->numMonsToFade)) > (new_var2 = 0U)) {
        do {
            ov81_02241E68(arg0, v0, v0, 1);
            Pokepic_SetAttr(((Pokepic *)((BattleFactoryApp *)new_var)->monSprites[v0]), 6, 1);
            v0 += 1;
        } while (v0 < ((u32)(arg0->numMonsToFade)));
    }
}

s32 ov81_0223EC88(void *arg0) {
    if ((((BattleFactoryApp *)arg0)->subState == 0) && (OverlayManager_Run(((OverlayManager *)((BattleFactoryApp *)arg0)->monSummaryAppMan)) == 1)) {
        *((s32 *)&((BattleFactoryApp *)arg0)->startingSlot) = (s32)((BattleFactoryApp *)arg0)->monSummary->partySlot;
        ((BattleFactoryApp *)arg0)->unk_468 = (s32)((BattleFactoryApp *)arg0)->monSummary->partySlot;
        Heap_Free(((void *)((BattleFactoryApp *)arg0)->monSummary));
        Heap_Free(((OverlayManager *)((BattleFactoryApp *)arg0)->monSummaryAppMan));
        *((OverlayManager **)&((BattleFactoryApp *)arg0)->monSummaryAppMan) = NULL;
        ov81_02240CD4(arg0);
        *((u8 *)((u8 *)&((BattleFactoryApp *)arg0)->numPokeballs + 0x1)) = (u8)(*((u8 *)((u8 *)&((BattleFactoryApp *)arg0)->numPokeballs + 0x1)) & ~0x40);
        *((s32 *)&((BattleFactoryApp *)arg0)->unk_478) = 0;
        return 1;
    }
    return 0;
}
