#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "mystery_gift.h"
#include "pokemon.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

enum MysteryGiftType {
    MYST_GIFT_NONE = 0,
    MYST_GIFT_POKEMON,
    MYST_GIFT_EGG,
    MYST_GIFT_ITEM,
    MYST_GIFT_BATTLE_REG,
    MYST_GIFT_DECORATION_GOOD,
    MYST_GIFT_COSMETICS,
    MYST_GIFT_MANAPHY_EGG,
    MYST_GIFT_MEMBER_CARD,
    MYST_GIFT_OAKS_LETTER,
    MYST_GIFT_AZURE_FLUTE,
    MYST_GIFT_POKETCH_APP,
    MYST_GIFT_SECRET_KEY,
    MYST_GIFT_UNKNOWN,
    MYST_GIFT_TYPE_MAX,
};

Sprite *ov74_02235930(enum DSScreen screen, Sprite *sprite, int x, int y, int animationIdx);

s32 ov74_02235728(s32, s32, s32, s32, s32, s32);

s32 ov74_0223589C(s32, s32);

s32 ov74_02235B14(Sprite *, Pokemon *, u32, u32, u8 *, PokemonSpriteTemplate *);

void ov74_02235C10(MainMenuUtilManager *param0, enum MysteryGiftType giftType, WonderCard *wonderCard);

void ov74_02235C10(MainMenuUtilManager *param0, enum MysteryGiftType giftType, WonderCard *wonderCard) {
    Pokemon *v1;
    u32 v0;
    ov74_02235728(0x34, 0xA, 0, 0xB, 0xC, 1);
    ov74_0223589C(0, 0x100000);
    param0->mysteryGiftSprite = ov74_02235930(DS_SCREEN_SUB, param0->mysteryGiftSprite, 0x80, 0, 1);
    v1 = &wonderCard->data.pokemon.mon;
    switch (giftType) {
    case MYST_GIFT_UNKNOWN:

    case MYST_GIFT_POKEMON:
        v0 = GetMonData(v1, 5, 0L);
        ov74_02235B14(param0->mysteryGiftSprite, v1, v0, GetMonData(v1, 0x70, 0L), param0->monSpriteBuffer, &param0->monSpriteTemplate);
        return;

    case MYST_GIFT_EGG:
        ov74_02235B14(param0->mysteryGiftSprite, v1, 0x1EEU, 0U, param0->monSpriteBuffer, &param0->monSpriteTemplate);
        return;

    case MYST_GIFT_MANAPHY_EGG:
        ov74_02235B14(param0->mysteryGiftSprite, v1, 0x1EEU, 1U, param0->monSpriteBuffer, &param0->monSpriteTemplate);
        return;
    }
}
