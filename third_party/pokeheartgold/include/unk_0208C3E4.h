#ifndef POKEHEARTGOLD_UNK_0208C3E4_H
#define POKEHEARTGOLD_UNK_0208C3E4_H

#include "global.h"
#include "bg_window.h"
#include "pokemon_types_def.h"

struct PokemonSummary;

enum PokemonSummaryAlignment {
    SUMMARY_ALIGN_LEFT = 0,
    SUMMARY_ALIGN_RIGHT,
    SUMMARY_ALIGN_CENTER,
};

enum PokemonSummaryPoffinMsg {
    SUMMARY_POFFIN_MSG_COOLNESS_ENHANCED = 0,
    SUMMARY_POFFIN_MSG_BEAUTY_ENHANCED,
    SUMMARY_POFFIN_MSG_CUTENESS_ENHANCED,
    SUMMARY_POFFIN_MSG_SMARTNESS_ENHANCED,
    SUMMARY_POFFIN_MSG_TOUGHNESS_ENHANCED,

    SUMMARY_POFFIN_MSG_NOTHING_CHANGED = 0xFE,
    SUMMARY_POFFIN_MSG_MON_WONT_EAT_MORE = 0xFF,
};

// Pokemon summary screen (arm9): every function takes the PokemonSummary app struct (pokemon_summary_app.h).
void sub_0208C3E4(struct PokemonSummary *summary);
void sub_0208C42C(struct PokemonSummary *summary);
void sub_0208C4E0(struct PokemonSummary *summary);
void sub_0208C560(struct PokemonSummary *summary);

void sub_0208C57C(struct PokemonSummary *summary);
void sub_0208C614(struct PokemonSummary *summary);

void sub_0208C6B4(struct PokemonSummary *summary);
s32 sub_0208C73C(struct PokemonSummary *summary);
void sub_0208C778(struct PokemonSummary *summary, Window *window, u32 color, u32 alignment);
void sub_0208C7F8(struct PokemonSummary *summary, s32 windowIndex, s32 msgId, s32 stat, s32 alignment);
void sub_0208C850(struct PokemonSummary *summary, s32 windowIndex, s32 msgId, s32 color, s32 alignment);
void sub_0208C87C(struct PokemonSummary *summary, s32 msgId, s32 number, u8 digits, u8 paddingMode);

#if defined(SDK_PORT) || defined(NONMATCHING)
u32 sub_0208C8C8(struct PokemonSummary *summary, u32 moveIndex, u32 slashEntryID, u32 curEntryID, u32 maxEntryID, u16 curValue, u16 maxValue, u8 digits, u8 xOffset, u8 yOffset);
#elif !defined(POKEMON_SUMMARY_DEFINES_C8C8)
// DS: the callers were compiled against a wider prototype (no narrowing in the caller); the definition takes u8
u32 sub_0208C8C8(struct PokemonSummary *summary, u32 moveIndex, u32 slashEntryID, u32 curEntryID, u32 maxEntryID, u16 curValue, u16 maxValue, u32 digits, u32 xOffset, u32 yOffset);
#endif

void sub_0208C9A4(struct PokemonSummary *summary);

void sub_0208CBD4(struct PokemonSummary *summary);
void sub_0208CC88(struct PokemonSummary *summary);

s32 sub_0208D078(struct PokemonSummary *summary);
void sub_0208D0A4(struct PokemonSummary *summary);
void sub_0208D178(struct PokemonSummary *summary);
void sub_0208D474(struct PokemonSummary *summary);
void sub_0208D520(struct PokemonSummary *summary);
void sub_0208D6B8(struct PokemonSummary *summary);
void sub_0208D728(struct PokemonSummary *summary);
void sub_0208D7C4(struct PokemonSummary *summary);
u32 sub_0208D884(struct PokemonSummary *summary, s32 moveIndex);
void sub_0208D9A0(struct PokemonSummary *summary, u16 move);

void sub_0208DAD4(struct PokemonSummary *summary);
void sub_0208DB1C(struct PokemonSummary *summary);
void sub_0208DB64(struct PokemonSummary *summary);

void sub_0208DBF0(struct PokemonSummary *summary);
void sub_0208DC68(struct PokemonSummary *summary, s32 param_2);

void sub_0208DD20(struct PokemonSummary *summary, s32 param_2);
void sub_0208DDA0(struct PokemonSummary *summary, enum PokemonSummaryPoffinMsg msg);

#endif // POKEHEARTGOLD_UNK_0208C3E4_H
