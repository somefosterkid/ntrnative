#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokepic.h"
#include "unk_0203A3B0.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02240458(BgConfig *arg0);

void ov81_02241364(BattleFactoryApp *arg0);

void ov81_0224275C(BattleFactoryApp *arg0);

s32 ov81_02242B38(void *);

void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite);

void *ov81_02242EA4(BattleFactoryAppPokeballSprite *ballSprite);


s32 ov81_02240D2C(void);
s32 ov81_02240D64(void *);
void ov81_02242BC8(void);
s32 ov81_02242FDC(BgConfig *, void *);
s32 ov81_022430E8(void *);
void ov81_02240CD4(void *arg0);

void ov81_02240BB0(BattleFactoryApp *app) {
    Pokepic *pokepic;
    s32 v3;
    s32 v4;
    s32 v5;
    struct BattleFactoryAppPokeballSprite **v0;

    ov81_0224275C(app);
    v3 = 0;
    if ((s32) app->numPokeballs > 0) {
        v0 = (struct BattleFactoryAppPokeballSprite **)app;
        do {
            BattleFactoryAppPokeballSprite *ballSprite = *(BattleFactoryAppPokeballSprite **)((u8 *)v0 + offsetof(BattleFactoryApp, ballSprites));
            if (ballSprite != NULL) {
                *(void **)((u8 *)v0 + offsetof(BattleFactoryApp, ballSprites)) = ov81_02242EA4(ballSprite);
            }
            v3 += 1;
            v0 += 1;
        } while (v3 < (s32) app->numPokeballs);
    }
    ov81_02242D74(app->unk_388);
    ov81_02242D74(app->unk_38C);
    ov81_02242D74(app->unk_390);
    ov81_02242D74(app->monIconSprites[0]);
    ov81_02242D74(app->monIconSprites[1]);
    ov81_02241364(app);
    ov81_02241FC0(app);
    ov81_02242170(app);
    sub_0203A914();
    PaletteData_FreeBuffers(app->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->plttData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->plttData);
    app->plttData = NULL;
    ov81_02242B38(&app->spriteMan);
    v4 = 0;
    do {
        pokepic = ((Pokepic *)app->monSprites[v4]);
        if (pokepic != NULL) {
            Pokepic_Delete(pokepic);
        }
        v4 += 1;
    } while (v4 < 3);
    PokepicManager_Delete(((PokepicManager *)app->monSpriteMan));
    DestroyMsgData(((MsgData *)app->msgLoader));
    MessageFormat_Delete(((MessageFormat *)app->strTemplate));
    String_Delete(app->displayStr);
    String_Delete(app->fmtStr);
    v5 = 0;
    do {
        String_Delete(app->menuStr[v5]);
        v5 += 1;
    } while (v5 < 4);
    FontID_Release(FONT_UNK_4);
    ov81_0224300C(app->windows);
    ov81_02240458(app->bgConfig);
    NARC_Delete(app->narc);
    GF_3DVramMan_Delete(((GF3DVramMan *)app->g3dPipeline));
}

void ov81_02240CD4(void *arg0) {
    ov81_02240D2C();
    *((GF3DVramMan **)&((BattleFactoryApp *)arg0)->g3dPipeline) = GF_3DVramMan_Create(HEAP_ID_100, 0, 2, 0, 2, ov81_02242BC8);
    ((BattleFactoryApp *)arg0)->bgConfig = BgConfig_Alloc(HEAP_ID_100);
    ov81_02240D64(arg0);
    ov81_02242FDC(((BattleFactoryApp *)arg0)->bgConfig, ((BattleFactoryApp *)arg0)->windows);
    if (*((s32 *)&((BattleFactoryApp *)arg0)->numMonsToFade) == 2) {
        ov81_022430E8(&((BattleFactoryApp *)arg0)->windows[2]);
    }
}
