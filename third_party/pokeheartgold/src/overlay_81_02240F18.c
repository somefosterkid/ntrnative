#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_2d.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"

struct OverlayManager;
struct BattleFactoryAppCursorSprite;
struct BattleFactoryAppPanelSprite;
struct BattleFactoryAppPokeballSprite;
struct G3DPipelineBuffers;
struct Menu;
struct ListMenu2DTemplate;
struct MsgData;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct PokemonSummary;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
typedef struct BattleFactoryAppCursorSprite BattleFactoryAppCursor;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
enum BattleFactoryAppWindows {
    WINDOW_PLAYERS_NAME = 0,
    WINDOW_PARTNERS_NAME,
    WINDOW_SELECTED_MON_NAME_1,
    WINDOW_SELECTED_MON_NAME_2,
    WINDOW_SELECTED_MON_NAME_3,
    WINDOW_MESSAGE_BOX,
    WINDOW_UNUSED,
    WINDOW_MENU,
    WINDOW_PARTNERS_MON_NAME_1,
    WINDOW_PARTNERS_MON_NAME_2,
    NUM_FACTORY_APP_WINDOWS,
};
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct Menu Menu;
u8 ov81_02240F18(u8 challengeType);
BOOL ov81_02240F28(BattleFactoryApp *app);

u8 ov81_02240F18(u8 challengeType) {
    switch (challengeType) {
    case 0:
    case 1:
        return 3;
    }

    return 2;
}

BOOL ov81_02240F28(BattleFactoryApp *app) {
    return !!app->menuWasCancelled;
}

void ov81_02240F38(BattleFactoryApp *app, u8 menuWasCancelled) {
    app->menuWasCancelled = menuWasCancelled;
}
