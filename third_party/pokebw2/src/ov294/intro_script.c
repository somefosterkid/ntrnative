#include "types.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "demo/intro.h"
#include "demo/intro_script.h"
#include "gfl/fade.h"
#include "nitro/fx.h"

// The intro's scripts. The messages are those of the system message file 54, and the professor's sprite changes frames
// while she talks. Word 0 is the player's name and word 1 the rival's.

static const IntroCmdEntry sScriptBegin[] = {
    { INTRO_CMD_LOAD_GRAPHICS, { FALSE }, TRUE },
    { INTRO_CMD_LOAD_MESSAGES, { TRUE, 54 }, TRUE },
    { INTRO_CMD_GOTO_MODE_SCRIPT },
};

static const IntroCmdEntry sScriptStart[] = {
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_GREETING } },
};

static const IntroCmdEntry sScriptGreeting[] = {
    { INTRO_CMD_ADD_SPRITE, { INTRO_SPRITE_PROFESSOR, 0, 0, FX32_CONST(5.628) }, TRUE },
    { INTRO_CMD_ADD_SPRITE, { INTRO_SPRITE_POKEMON, SPECIES_CINCCINO, FX32_CONST(-5), FX32_CONST(-0.5) }, TRUE },
    { INTRO_CMD_SET_SPRITE_VISIBLE, { INTRO_SPRITE_POKEMON, FALSE } },
    { INTRO_CMD_SAVE_START },
    { INTRO_CMD_CLEAR_MESSAGE },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 2 } },
    { INTRO_CMD_BGM_PLAY, { SEQ_BGM_STARTING2 }, TRUE },
    { INTRO_CMD_BGM_FADE_IN, { 90 }, TRUE },
    { INTRO_CMD_WAIT, { 90 } },
    // The professor greets the player.
    { INTRO_CMD_TALK, { 6, INTRO_SPRITE_PROFESSOR, 1 } },
    // She introduces herself.
    { INTRO_CMD_TALK, { 7, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_POKEMON } },
};

static const IntroCmdEntry sScriptPokemon[] = {
    { INTRO_CMD_CLEAR_MESSAGE },
    { INTRO_CMD_CLEAR_SPRITE_ANIMATION_ENDED, { INTRO_SPRITE_PROFESSOR } },
    { INTRO_CMD_MOVE_SPRITE_X, { INTRO_SPRITE_PROFESSOR, FX32_CONST(0.2), FX32_CONST(8) } },
    { INTRO_CMD_WAIT_SPRITE_ANIMATION, { INTRO_SPRITE_PROFESSOR } },
    { INTRO_CMD_SET_SPRITE_ANIMATION, { INTRO_SPRITE_PROFESSOR, 2, TRUE } },
    { INTRO_CMD_WAIT, { 52 } },
    { INTRO_CMD_SE_PLAY, { SEQ_SE_NAGERU } },
    { INTRO_CMD_WAIT, { 60 } },
    { INTRO_CMD_SE_PLAY, { SEQ_SE_KON } },
    { INTRO_CMD_WAIT, { 26 } },
    { INTRO_CMD_SET_SPRITE_ANIMATION, { INTRO_SPRITE_PROFESSOR, 1, FALSE } },
    { INTRO_CMD_SE_PLAY, { SEQ_SE_BOWA2 }, TRUE },
    { INTRO_CMD_SET_SPRITE_VISIBLE, { INTRO_SPRITE_POKEMON, TRUE }, TRUE },
    { INTRO_CMD_PARTICLES, { FX32_CONST(-0.4), FX32_CONST(0.5) }, TRUE },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, -16, 0, 6 } },
    { INTRO_CMD_POKEMON_APPEAR, { SPECIES_CINCCINO } },
    // She explains what Pokemon are.
    { INTRO_CMD_TALK, { 8, INTRO_SPRITE_PROFESSOR, 1 } },
    // She says what Pokemon mean to people, and what her research is about.
    { INTRO_CMD_TALK, { 9, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_ABOUT_YOU } },
};

static const IntroCmdEntry sScriptAboutYou[] = {
    { INTRO_CMD_FADE_SPRITE, { INTRO_SPRITE_POKEMON, FALSE } },
    { INTRO_CMD_CLEAR_SPRITE_ANIMATION_ENDED, { INTRO_SPRITE_PROFESSOR } },
    { INTRO_CMD_MOVE_SPRITE_X, { INTRO_SPRITE_PROFESSOR, FX32_CONST(-0.2), 0 } },
    { INTRO_CMD_SET_SPRITE_ANIMATION, { INTRO_SPRITE_PROFESSOR, 1, FALSE } },
    // She asks the player to say something about themselves.
    { INTRO_CMD_TALK, { 10, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_FADE_SPRITE, { INTRO_SPRITE_PROFESSOR, FALSE } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_GENDER } },
};

static const IntroCmdEntry sScriptGender[] = {
    { INTRO_CMD_SET_MODEL_VISIBLE, { TRUE }, TRUE },
    { INTRO_CMD_OPEN_MODEL },
    // She asks the player's gender.
    { INTRO_CMD_TALK, { 11, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_CHOOSE_GENDER },
    { INTRO_CMD_IF, { INTRO_COND_PLAYER_IS_MALE } },
    // She confirms a male player.
    { INTRO_CMD_TALK, { 12, INTRO_SPRITE_PROFESSOR, 1 } },
    // She confirms a female player.
    { INTRO_CMD_TALK, { 13, INTRO_SPRITE_PROFESSOR, 1 } },
    // YES / NO
    { INTRO_CMD_YES_NO, { 19, 20, 1 } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_ASK_NAME } },
    { INTRO_CMD_MODEL_BACK },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_GENDER } },
};

static const IntroCmdEntry sScriptAskName[] = {
    { INTRO_CMD_SET_RESULT, { INTRO_RESULT_ENTER_NAME } },
    // She asks for the player's name.
    { INTRO_CMD_TALK, { 16, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_SAVE_PAUSE },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 2 } },
    { INTRO_CMD_END },
};

static const IntroCmdEntry sScriptPlayerNamed[] = {
    { INTRO_CMD_LOAD_GRAPHICS, { TRUE }, TRUE },
    { INTRO_CMD_CREATE_OBJ },
    { INTRO_CMD_ADD_SPRITE, { INTRO_SPRITE_PROFESSOR, 0, 0, FX32_CONST(5.628) }, TRUE },
    { INTRO_CMD_SAVE_RESUME },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 2 } },
    { INTRO_CMD_SET_WORD, { INTRO_WORD_PLAYER_NAME, 0 }, TRUE },
    { INTRO_CMD_IF, { INTRO_COND_PLAYER_IS_MALE } },
    // She asks to confirm the name.
    { INTRO_CMD_TALK, { 17, INTRO_SPRITE_PROFESSOR, 1 } },
    // The same, in the other branch.
    { INTRO_CMD_TALK, { 18, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_WAIT, { 1 } },
    // YES / NO
    { INTRO_CMD_YES_NO, { 19, 20, 1 } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_ASK_RIVAL_NAME } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_RENAME_PLAYER } },
};

static const IntroCmdEntry sScriptRenamePlayer[] = {
    { INTRO_CMD_SET_RESULT, { INTRO_RESULT_ENTER_NAME } },
    { INTRO_CMD_SAVE_PAUSE },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 2 } },
    { INTRO_CMD_END },
};

static const IntroCmdEntry sScriptAskRivalName[] = {
    { INTRO_CMD_SET_RESULT, { INTRO_RESULT_ENTER_RIVAL_NAME } },
    // She comments on the name.
    { INTRO_CMD_TALK, { 21, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_FADE_SPRITE, { INTRO_SPRITE_PROFESSOR, FALSE } },
    { INTRO_CMD_OPEN_BAND },
    { INTRO_CMD_OBJ_FADE_IN },
    // She asks about the player's friend, the rival.
    { INTRO_CMD_TALK, { 23, INTRO_SPRITE_PROFESSOR, 1 } },
    // She asks for the rival's name.
    { INTRO_CMD_TALK, { 24, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_SAVE_PAUSE },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 2 } },
    { INTRO_CMD_END },
};

static const IntroCmdEntry sScriptRivalNamed[] = {
    { INTRO_CMD_LOAD_GRAPHICS, { TRUE }, TRUE },
    { INTRO_CMD_CREATE_OBJ },
    { INTRO_CMD_ADD_SPRITE, { INTRO_SPRITE_PROFESSOR, 0, 0, FX32_CONST(5.628) }, TRUE },
    { INTRO_CMD_SAVE_RESUME },
    { INTRO_CMD_SET_SPRITE_VISIBLE, { INTRO_SPRITE_PROFESSOR, FALSE } },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 2 } },
    { INTRO_CMD_OPEN_BAND },
    { INTRO_CMD_OBJ_FADE_IN },
    { INTRO_CMD_SET_WORD, { INTRO_WORD_RIVAL_NAME, 1 }, TRUE },
    // She asks to confirm the rival's name.
    { INTRO_CMD_TALK, { 29, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_WAIT, { 1 } },
    // YES / NO
    { INTRO_CMD_YES_NO, { 19, 20, 1 } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_FAREWELL } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_RENAME_RIVAL } },
};

static const IntroCmdEntry sScriptRenameRival[] = {
    { INTRO_CMD_SET_RESULT, { INTRO_RESULT_ENTER_NAME } },
    { INTRO_CMD_SAVE_PAUSE },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 2 } },
    { INTRO_CMD_END },
};

static const IntroCmdEntry sScriptFarewell[] = {
    { INTRO_CMD_SET_RESULT, { INTRO_RESULT_DONE } },
    { INTRO_CMD_OBJ_FADE_OUT },
    { INTRO_CMD_CLOSE_BAND },
    { INTRO_CMD_FADE_SPRITE, { INTRO_SPRITE_PROFESSOR, TRUE } },
    { INTRO_CMD_SAVE_43 },
    { INTRO_CMD_SAVE_WAIT },
    { INTRO_CMD_SET_WORD, { INTRO_WORD_PLAYER_NAME, 0 }, TRUE },
    // She hands the player a Pokemon.
    { INTRO_CMD_TALK, { 32, INTRO_SPRITE_PROFESSOR, 1 } },
    // Her closing speech about the journey ahead.
    { INTRO_CMD_TALK, { 33, INTRO_SPRITE_PROFESSOR, 1 } },
    { INTRO_CMD_FADE_SPRITE, { INTRO_SPRITE_PROFESSOR, FALSE } },
    { INTRO_CMD_GOTO, { INTRO_SCRIPT_FINISH } },
};

static const IntroCmdEntry sScriptFinish[] = {
    { INTRO_CMD_BGM_FADE_OUT, { 60 } },
    { INTRO_CMD_FADE, { FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 6 } },
    { INTRO_CMD_FREE_OBJ },
    { INTRO_CMD_END },
};

static const IntroCmdEntry *sScripts[INTRO_SCRIPT_COUNT] = {
    [INTRO_SCRIPT_BEGIN] = sScriptBegin,
    [INTRO_SCRIPT_START] = sScriptStart,
    [INTRO_SCRIPT_GREETING] = sScriptGreeting,
    [INTRO_SCRIPT_POKEMON] = sScriptPokemon,
    [INTRO_SCRIPT_ABOUT_YOU] = sScriptAboutYou,
    [INTRO_SCRIPT_GENDER] = sScriptGender,
    [INTRO_SCRIPT_ASK_NAME] = sScriptAskName,
    [INTRO_SCRIPT_PLAYER_NAMED] = sScriptPlayerNamed,
    [INTRO_SCRIPT_RENAME_PLAYER] = sScriptRenamePlayer,
    [INTRO_SCRIPT_ASK_RIVAL_NAME] = sScriptAskRivalName,
    [INTRO_SCRIPT_RIVAL_NAMED] = sScriptRivalNamed,
    [INTRO_SCRIPT_RENAME_RIVAL] = sScriptRenameRival,
    [INTRO_SCRIPT_FAREWELL] = sScriptFarewell,
    [INTRO_SCRIPT_FINISH] = sScriptFinish,
};

const IntroCmdEntry *IntroScript_Get(u32 script) {
    return sScripts[script];
}
