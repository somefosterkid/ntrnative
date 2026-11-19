#include "global.h"

#include "overlay_86_frontier_records_app.h"
#include "overlay_manager.h"
#include "sprite_system.h"
#include "gf_gfx_planes.h"

BOOL BattleArcadeGameBoard_Init(FrontierRecordsApp *app);

BOOL State_ViewRecord(FrontierRecordsApp *app);

BOOL BattleArcadeGameBoard_End(FrontierRecordsApp *app);

void BattleArcadeGameBoard_SetState(FrontierRecordsApp *app, int *state, int newState);

void LoadMainBackground(FrontierRecordsApp *app, enum GFBgLayer bgLayer);

void ov86_021E5F58(FrontierRecordsApp *app, enum GFBgLayer bgLayer);

void ov86_021E5E90(FrontierRecordsApp *arg0, s32 *arg1, s32 arg2);

void ov86_021E703C(FrontierRecordsApp *arg0);

int ov86_021E5A40(OverlayManager *appMan, int *state);

BOOL ov86_021E5B38(FrontierRecordsApp *app);

BOOL ov86_021E5BA0(FrontierRecordsApp *app);

BOOL ov86_021E5C94(FrontierRecordsApp *app);

void SpriteSystem_DrawSprites(SpriteManager *);

int ov86_021E5A40(OverlayManager *appMan, int *state) {
    FrontierRecordsApp *app = OverlayManager_GetData(appMan);

    switch (*state) {
    case 0:
        if (ov86_021E5B38(app) == TRUE) {
            ov86_021E5E90(app, state, 1);
        }
        break;
    case 1:
        if (ov86_021E5BA0(app) == TRUE) {
            ov86_021E5E90(app, state, 2);
        }
        break;
    case 2:
        if (ov86_021E5C94(app) == TRUE) {
            return TRUE;
        }
        break;
    }
    ov86_021E703C(app);
    SpriteSystem_DrawSprites(app->spriteManager);

    return FALSE;
}
