#ifndef POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_YES_NO_PROMPT_H
#define POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_YES_NO_PROMPT_H

struct BgConfig;
struct YesNoPrompt;

// The touch yes/no prompt the Battle Castle apps embed (SelfApp 0x84C, OpponentApp 0x604):
// created by ov83_02247844, shown by ov83_02247864, hidden by ov83_022478B4, destroyed by ov83_02247858.
typedef struct BattleCastleYesNoPrompt {
    struct YesNoPrompt *prompt;
    BOOL isActive;
    struct BgConfig *bgConfig;
} BattleCastleYesNoPrompt;

#endif // POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_YES_NO_PROMPT_H
