#ifndef POKEHEARTGOLD_OVERLAY_08_BATTLE_SUB_MENU_CURSOR_H
#define POKEHEARTGOLD_OVERLAY_08_BATTLE_SUB_MENU_CURSOR_H

// BattleSubMenuCursor: merged by tools/hg/promote.py from 69 m2c local copies (1 variants, base
// src/overlay_08_0221C048.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct BattleCursor;
struct DpadMenuBox;

typedef struct BattleSubMenuCursor {
    struct BattleCursor *sprites;
    const struct DpadMenuBox *positions;
    u8 isVisible;
    u8 currentPositionIndex;
    u8 previousPositionIndex;
    u8 pad_B[0x1];
    u32 enabledPositionsMask;
} BattleSubMenuCursor;

// sizeof(BattleSubMenuCursor) == 0x10

#endif // POKEHEARTGOLD_OVERLAY_08_BATTLE_SUB_MENU_CURSOR_H
