#include "global.h"

#include "pm_string.h"
#include "unk_0200A090.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "trainer_case.h"

struct BgConfig;
struct SpriteList;
struct SpriteResourceCollection;
struct TrainerCase;
enum Badge {
    BADGE_ID_COAL = 0,
    BADGE_ID_FOREST = 1,
    BADGE_ID_COBBLE = 2,
    BADGE_ID_FEN = 3,
    BADGE_ID_RELIC = 4,
    BADGE_ID_MINE = 5,
    BADGE_ID_ICICLE = 6,
    BADGE_ID_BEACON = 7,
    MAX_BADGES = 8,
};
void FillBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, u16 fillVal, u8 x, u8 y, u8 width, u8 height, u8 palette);
enum TrainerCardWindow {
    TRAINER_CARD_WINDOW_ID = 0,
    TRAINER_CARD_WINDOW_NAME,
    TRAINER_CARD_WINDOW_MONEY,
    TRAINER_CARD_WINDOW_POKEDEX,
    TRAINER_CARD_WINDOW_SCORE,
    TRAINER_CARD_WINDOW_TIME,
    TRAINER_CARD_WINDOW_ADVENTURE_STARTED,
    TRAINER_CARD_WINDOW_HOF_DEBUT,
    TRAINER_CARD_WINDOW_TIMES_LINKED,
    TRAINER_CARD_WINDOW_LINK_BATTLES,
    TRAINER_CARD_WINDOW_LINK_TRADES,
    TRAINER_CARD_WINDOW_COUNT,
};
typedef struct BadgePolishingState {
    s8 previousXDirection;
    s8 previousYDirection;
    s8 currentXDirection;
    s8 currentYDirection;
    u8 soundEffectIndex;
} BadgePolishingState;
typedef struct BadgeChimeState {
    u8 badgeChimePlayers[6 - 3 + 1][2];
} BadgeChimeState;
typedef struct TrainerCaseSpriteData {
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    SpriteResourceCollection *spriteResourceCollection[(GF_GFX_RES_TYPE_ANIM + 1)];
    SpriteResource *spriteResources[(GF_GFX_RES_TYPE_ANIM + 1)];
    u8 padding[16];
    Sprite *sprites[MAX_BADGES * 3 + 1];
    void *badgePaletteBuffers[MAX_BADGES];
    NNSG2dPaletteData *badgePalettes[MAX_BADGES];
} TrainerCaseSpriteData;
typedef struct TrainerCaseApp {
    BgConfig *bgConfig;
    Window windows[TRAINER_CARD_WINDOW_COUNT];
    TrainerCase *trainerCase;
    u8 signature[24 * 8 * (8 * 8)];
    void *trainerSprite;
    NNSG2dCharacterData *trainerCharacterData;
    BOOL viewingBack;
    BOOL polishingEnabled;
    fx32 cardXScale;
    fx32 cardYScale;
    fx32 badgeCaseCoverYScale;
    int touchedRectangleIndex;
    BOOL badgeCaseCoverMoving;
    int lastTouchedX;
    int lastTouchedY;
    TrainerCaseSpriteData spriteData;
    int cardScalingSpeed;
    u8 padding[4];
    BOOL badgeCaseButtonPushed;
    BadgeChimeState badgeChimeState;
    void *trainerScreenDataNSCRBuffer;
    NNSG2dScreenData *trainerScreenData;
    BadgePolishingState badgePolishingState;
    String *unusedString;
    String *colonString;
    u8 polishingProgress[MAX_BADGES];
    u8 badgeCaseOpenState;
    u8 badgeCaseTransitionIndex;
    u8 subState;
    u8 badgeCaseButtonAnimTimer;
    u8 badgeCaseButtonAnimIndex;
    u8 badgeCaseButtonState;
    u8 timer;
} TrainerCaseApp;

void ov51_021E6CCC(TrainerCaseApp *trainerCaseApp);
void ov51_021E6CF0(const u8 *signature1Bpp, u8 *signature8BppOut);

void ov51_021E6CCC(TrainerCaseApp *trainerCaseApp) {
    FillBgTilemapRect(trainerCaseApp->bgConfig, GF_BG_LYR_SUB_3, 0, 20, 6, 6, 9, 16);
}

void ov51_021E6CF0(const u8 *signature1Bpp, u8 *signature8BppOut) {
    for (int pixel = 0; pixel < 24 * 8 * (8 * 8); pixel++) {
        int indexPart1 = pixel / (8 * 8);
        u8 indexPart2 = (pixel / 8) % 8;
        u8 shift = pixel % 8;

        signature8BppOut[pixel] = (signature1Bpp[indexPart1 * 8 + indexPart2] >> shift) & 0x1;
    }
}
