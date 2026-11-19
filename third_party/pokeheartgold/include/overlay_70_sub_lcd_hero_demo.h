#ifndef POKEHEARTGOLD_OVERLAY_70_SUB_LCD_HERO_DEMO_H
#define POKEHEARTGOLD_OVERLAY_70_SUB_LCD_HERO_DEMO_H

// SubLcdHeroDemo: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_70_02238F10_0223FFCC_02240D74_02240F58_02240F58.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct WorldTradeWork;

typedef struct SubLcdHeroDemo {
    int seq;
    int y;
    u32 unk_8;
    struct WorldTradeWork *wk;
} SubLcdHeroDemo;

// sizeof(SubLcdHeroDemo) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_70_SUB_LCD_HERO_DEMO_H
