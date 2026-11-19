#include "global.h"

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

extern u8 _0223B73C;

u8 ov74_02235AC4(enum MysteryGiftType giftType);

u8 ov74_02235AC4(enum MysteryGiftType giftType) {
    u8 sp0[36];
    u32 var_r2;
    s32 var_r3;
    u8 *var_r3_2;
    u8 *var_r4;
    u8 *var_r4_2;
    u8 temp_r1;

    var_r4 = &_0223B73C;
    var_r3_2 = sp0;
    var_r2 = 0x22;
    do {
        temp_r1 = *var_r4;
        var_r4 += 1;
        *var_r3_2 = temp_r1;
        var_r3_2 += 1;
        var_r2 -= 1;
    } while (var_r2 != 0);
    var_r3 = 0;
    if (sp0[0] != -1U) {
        var_r4_2 = sp0;
        do {
            if (giftType == *var_r4_2) {
                return *(sp0 + 1 + (var_r3 * 2));
            }
            var_r4_2 += 2;
            var_r3 += 1;
        } while (*var_r4_2 != -1U);
    }
    return 0;
}
