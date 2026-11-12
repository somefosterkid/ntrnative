// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/sta_acting.h, the lines in use
// so far.
#ifndef BWPORT_APP_MUSICAL_STA_ACTING_H
#define BWPORT_APP_MUSICAL_STA_ACTING_H

// Overlay 132's sta_acting.c: the musical's stage, which plays the program's script with the Pokémon, the
// background, the objects, the lights, the effects and the audience

#include "types.h"
#include "struct_decls.h"
#include "app/musical/sta_act_obj.h"

// How many times the props' effects run this frame (pokebw2's StaActing_GetUpdateCount)
u32 func_ov132_021f9e44(StaActing *stage);

// pokebw2's StaActing_GetLightUpPoke
// The position of the Pokémon the spotlight follows, or 4 for none
u8 func_ov132_021f9e18(StaActing *stage);
// pokebw2's StaActing_GetPoke
StaActPoke *func_ov132_021f9da4(StaActing *stage, u8 pos);

// pokebw2's StaActing_UseItem
// Asks to use the prop at an equip position of the player's Pokémon
void func_ov132_021f9730(StaActing *stage, u32 equipPos);
// pokebw2's StaActing_IsUsingItem
// Whether a prop of the player's Pokémon is being used
BOOL func_ov132_021f9768(StaActing *stage);

// pokebw2's StaActing_GetLightSys
StaActLightSys *func_ov132_021f9dd4(StaActing *stage);
// pokebw2's StaActing_GetLight
StaActLight *func_ov132_021f9ddc(StaActing *stage, u8 index);

// pokebw2's StaActing_GetEffectSys
StaActEffectSys *func_ov132_021f9dc0(StaActing *stage);

// pokebw2's StaActing_StartAppealScript
// Runs the appeal script of a file for the Pokémon at pos, which is one of three by its points
void func_ov132_021f9140(StaActing *stage, u8 file, u8 pos);
// pokebw2's StaActing_StartPokeScript
// Runs the script of a file for the Pokémon of a mask
void func_ov132_021f91a8(StaActing *stage, u8 file, u8 pokeMask);
// pokebw2's StaActing_PrintMessage
// Prints a message of the stage's text, at once when wait is 0xff
void func_ov132_021f93d8(StaActing *stage, u16 msgId, u32 wait);
// pokebw2's StaActing_ClearMessage
void func_ov132_021f94b0(StaActing *stage);
// pokebw2's StaActing_SetAudienceFocus
// Makes the audience look at the Pokémon at pos, or with focus FALSE at none
void func_ov132_021f95cc(StaActing *stage, u8 pos, BOOL focus);
// pokebw2's StaActing_RefreshAudience
// Makes the audience look again at the lights
void func_ov132_021f95e8(StaActing *stage);
// pokebw2's StaActing_PlayStrm
// The program's sound: names guessed from the flags the stage keeps for them
void func_ov132_021f9664(StaActing *stage, u16 seq);
// pokebw2's StaActing_PlaySeq
void func_ov132_021f9680(StaActing *stage, u16 seq);
// pokebw2's StaActing_StopSeq
void func_ov132_021f9690(StaActing *stage);
// pokebw2's StaActing_PlayWave
void func_ov132_021f969c(StaActing *stage, u16 waveArc, u16 a2, u16 a3);
// pokebw2's StaActing_PlayBgm
void func_ov132_021f96b0(StaActing *stage, u32 bgm);
// pokebw2's StaActing_StopBgm
void func_ov132_021f96c4(StaActing *stage);
// pokebw2's StaActing_PlayApplause
// The applause, by the points of the Pokémon that did worst
void func_ov132_021f96d0(StaActing *stage);
// pokebw2's StaActing_SetNpcItemTiming
// When the other Pokémon use their props, chosen at random by a timing, 0 to 2, and for timing 1 only when the
// Pokémon at pos appeals
void func_ov132_021f9ad8(StaActing *stage, u32 timing, u8 pos);
// pokebw2's StaActing_EnableButtons
// The buttons of the player's props
void func_ov132_021f9d80(StaActing *stage);
// pokebw2's StaActing_DisableButtons
void func_ov132_021f9d90(StaActing *stage);
// pokebw2's StaActing_GetObj
StaActObj *func_ov132_021f9db0(StaActing *stage, u8 index);
// pokebw2's StaActing_GetEffect
StaActEffect *func_ov132_021f9dc4(StaActing *stage, u8 index);
// pokebw2's StaActing_SetEffect
void func_ov132_021f9dcc(StaActing *stage, StaActEffect *effect, u8 index);
// pokebw2's StaActing_SetLight
void func_ov132_021f9de8(StaActing *stage, StaActLight *light, u8 index);
// pokebw2's StaActing_SetScrollTarget
// Where the stage scrolls to
void func_ov132_021f9e08(StaActing *stage, u16 target);
// pokebw2's StaActing_GetPoints
// The points of the Pokémon at pos before the show, and those it won during it
u8 func_ov132_021f9e20(StaActing *stage, u8 pos);
// pokebw2's StaActing_GetBonusPoints
u8 func_ov132_021f9e34(StaActing *stage, u8 pos);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's StaActing_GetScrollOffset
// How far the stage has scrolled, in pixels
u16 func_ov132_021f9e04(StaActing *stage);
// pokebw2's StaActing_GetPokeSys
StaActPokeSys *func_ov132_021f9da0(StaActing *stage);
// pokebw2's StaActing_GetItemData
void *func_ov132_021f9e0c(StaActing *stage);
// pokebw2's StaActing_GetCurtainOffset
// How far the curtain has opened, from 0 (closed) to 0xe0
u16 func_ov132_021f9dfc(StaActing *stage);
// pokebw2's StaActing_StartAppealEffect
// The sparkles and the sound of the Pokémon at pos appealing
void func_ov132_021f9d18(StaActing *stage, u8 pos);
// pokebw2's StaActing_GetObjSys
StaActObjSys *func_ov132_021f9dac(StaActing *stage);
// pokebw2's StaActing_SetObj
void func_ov132_021f9db8(StaActing *stage, StaActObj *obj, u8 index);
// pokebw2's StaActing_GetAudience
StaActAudience *func_ov132_021f9df4(StaActing *stage);
// pokebw2's StaActing_SetCurtainOffset
void func_ov132_021f9e00(StaActing *stage, u16 offset);
// pokebw2's StaActing_SetScrollFixed
// Whether the stage stays where the Pokémon lined up at the end
void func_ov132_021f9e3c(StaActing *stage, BOOL fixed);
// pokebw2's StaActing_SetScriptScroll
// Whether a script scrolls the stage, which then doesn't follow the player's Pokémon
void func_ov132_021f9e40(StaActing *stage, BOOL scroll);

// A constant of sta_acting.c that only an accessor nothing calls reads, which puts it among the file's other data
// (pokebw2's STA_ACTING_UNUSED and StaActing_GetUnused)
extern const u32 data_ov132_021fda78;

static inline u32 StaActing_GetUnused(void) {
    return data_ov132_021fda78;
}

#endif // BWPORT_APP_MUSICAL_STA_ACTING_H
