// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/intro.h: the lines in use so far.
#ifndef BWPORT_DEMO_INTRO_H
#define BWPORT_DEMO_INTRO_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/clact.h"

typedef struct IntroGraphic IntroGraphic;
typedef struct IntroParticle IntroParticle;

// pokebw2's IntroParticle_Create
// intro_particle.c
IntroParticle *func_ov204_021bfc24(IntroGraphic *graphic, HeapID heapId);
// pokebw2's IntroParticle_SetPos
void func_ov204_021bfce8(IntroParticle *particle, fx32 x, fx32 y, fx32 z);

// Black 2's, not checked against Black
#define INTRO_MODE_PLAYER_NAMED 7
#define INTRO_MODE_RIVAL_NAMED 10

typedef struct IntroMcss IntroMcss;

// pokebw2's IntroMcss_Free
void func_ov204_021bf5e0(IntroMcss *mcss);
// pokebw2's IntroMcss_Draw
void func_ov204_021bf610(IntroMcss *mcss);
// pokebw2's IntroMcss_Add
void func_ov204_021bf624(IntroMcss *mcss, fx32 x, fx32 y, fx32 z, const MCSSLoadInfo *info, u8 index);
// pokebw2's IntroMcss_AddPokemon
void func_ov204_021bf67c(IntroMcss *mcss, fx32 x, fx32 y, fx32 z, u32 species, u8 index);
// pokebw2's IntroMcss_SetVisible
void func_ov204_021bf720(IntroMcss *mcss, BOOL visible, u8 index);
// pokebw2's IntroMcss_SetAnimation
void func_ov204_021bf744(IntroMcss *mcss, u8 index, u32 animation, BOOL restartAtEnd);
// pokebw2's IntroMcss_IsAnimationEnded
BOOL func_ov204_021bf77c(IntroMcss *mcss, u8 index);
// pokebw2's IntroMcss_ClearAnimationEnded
void func_ov204_021bf788(IntroMcss *mcss, u8 index);
// pokebw2's IntroMcss_SetAlpha
void func_ov204_021bf794(IntroMcss *mcss, u8 index, u32 alpha);
void func_ov204_021bf7cc(IntroMcss *mcss, u8 index, BOOL a2);
// pokebw2's IntroMcss_MoveX
BOOL func_ov204_021bf804(IntroMcss *mcss, u8 index, fx32 step, fx32 target);
// pokebw2's IntroMcss_DecreaseY
BOOL func_ov204_021bf864(IntroMcss *mcss, fx32 step, fx32 min);
// pokebw2's IntroMcss_Update
void func_ov204_021bf898(IntroMcss *mcss);

// Black 2's; twinned code confirms the offsets of playerInfo, config and mode in Black
typedef struct {
    PlayerInfo *playerInfo;
    Config *config;
    // INTRO_MODE_*
    u32 mode;
    // ov162's creation of the save data, which runs while the intro does
    SaveControlIntr *saveTask;
    // The handle of the Pokémon's cry
    u32 pokeVoice;
    // INTRO_RESULT_*
    u32 result;
} IntroParam;

typedef struct IntroCmd IntroCmd;
typedef struct IntroG3d IntroG3d;

// pokebw2's IntroG3d_Free
void func_ov204_021bf9d0(IntroG3d *g3d);
// pokebw2's IntroCmd_Free
void func_ov204_021bef74(IntroCmd *cmd);
// pokebw2's IntroGraphic_Free
void func_ov204_021bdcf0(IntroGraphic *graphic);
// pokebw2's IntroCmd_Update
// Runs the script, and returns FALSE once it has ended
BOOL func_ov204_021bef88(IntroCmd *cmd);
// pokebw2's IntroG3d_Draw
void func_ov204_021bf9fc(IntroG3d *g3d);

// pokebw2's IntroG3d_Open
BOOL func_ov204_021bfa40(IntroG3d *g3d);
// pokebw2's IntroG3d_SetMode
void func_ov204_021bfae8(IntroG3d *g3d, u32 mode);
// pokebw2's IntroG3d_Animate
BOOL func_ov204_021bfb2c(IntroG3d *g3d);
// pokebw2's IntroG3d_AnimateBack
BOOL func_ov204_021bfba0(IntroG3d *g3d);
// pokebw2's IntroG3d_SetFrame
void func_ov204_021bfbc8(IntroG3d *g3d, u32 frame);

typedef struct IntroMsg IntroMsg;

// intro_msg.c
struct IntroMenuItem {
    u32 message;
    s32 value;
};

// IntroMsg_GetMenuResult's results
#define INTRO_MENU_NONE 0

#define INTRO_MENU_CHOSEN 1
#define INTRO_MENU_CANCELLED 2

// pokebw2's IntroMsg_Create
IntroMsg *func_ov204_021bf0a0(HeapID heapId);
// pokebw2's IntroMsg_Free
void func_ov204_021bf1a4(IntroMsg *msg);
// pokebw2's IntroMsg_LoadMessages
void func_ov204_021bf204(IntroMsg *msg, BOOL preload, u16 fileId);
// pokebw2's IntroMsg_Update
void func_ov204_021bf228(IntroMsg *msg);
// pokebw2's IntroMsg_Print
void func_ov204_021bf23c(IntroMsg *msg, u32 messageId, BOOL frame);
// pokebw2's IntroMsg_Clear
void func_ov204_021bf2d4(IntroMsg *msg);
// pokebw2's IntroMsg_GetPrintState
u32 func_ov204_021bf2e0(IntroMsg *msg);
// pokebw2's IntroMsg_UpdatePrint
BOOL func_ov204_021bf2f4(IntroMsg *msg);
// pokebw2's IntroMsg_CloseMenu
void func_ov204_021bf4e8(IntroMsg *msg);
// pokebw2's IntroMsg_UpdateMenu
void func_ov204_021bf528(IntroMsg *msg);
// pokebw2's IntroMsg_GetMenuResult
u32 func_ov204_021bf538(IntroMsg *msg, s32 *value);
// pokebw2's IntroMsg_ShowWaitIcon
void func_ov204_021bf560(IntroMsg *msg);

// Black 2's, not checked against Black
#define INTRO_MODE_START 1

// Black 2's, not checked against Black
#define INTRO_RESULT_DONE 0

// Black 2's; Black's scripts set it before the player's name is entered
#define INTRO_RESULT_ENTER_NAME 1

// Black 2's, not checked against Black
#define INTRO_RESULT_ENTER_RIVAL_NAME 2

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's IntroParticle_Free
void func_ov204_021bfcd0(IntroParticle *particle);
// pokebw2's IntroParticle_Update
void func_ov204_021bfce0(IntroParticle *particle);
// pokebw2's IntroMcss_Create
// intro_mcss.c
IntroMcss *func_ov204_021bf594(HeapID heapId, u32 mode);
// pokebw2's IntroGraphic_Create
// intro_graphic.c
IntroGraphic *func_ov204_021bdc18(u32 layout, u32 mode, HeapID heapId);
// pokebw2's IntroG3d_Create
// intro_g3d.c
IntroG3d *func_ov204_021bf8dc(IntroGraphic *graphic, u32 mode, HeapID heapId);
// pokebw2's IntroCmd_Create
// intro_cmd.c
IntroCmd *func_ov204_021bef20(IntroG3d *g3d, IntroParticle *particle, IntroMcss *mcss, IntroParam *param,
                          IntroGraphic *graphic, HeapID heapId);
// pokebw2's IntroGraphic_Update
void func_ov204_021bdd4c(IntroGraphic *graphic);
// pokebw2's IntroGraphic_Begin3D
void func_ov204_021bdd60(IntroGraphic *graphic);
// pokebw2's IntroGraphic_End3D
void func_ov204_021bdd6c(IntroGraphic *graphic);
// pokebw2's IntroG3d_SetVisible
void func_ov204_021bfae4(IntroG3d *g3d, BOOL visible);
// pokebw2's IntroMsg_OpenMenu
void func_ov204_021bf384(IntroMsg *msg, const IntroMenuItem *items, u32 count, BOOL a3);
// pokebw2's IntroMsg_GetWordSet
WordSet *func_ov204_021bf55c(IntroMsg *msg);
// pokebw2's IntroMsg_HideWaitIcon
void func_ov204_021bf580(IntroMsg *msg);
// pokebw2's IntroGraphic_GetClActUnit
ClActUnit *func_ov204_021bdd78(IntroGraphic *graphic);

// The sound sequences the intro preloads: their count and the list
extern const u32 data_ov204_021bfd20;
extern const u32 data_ov204_021bfd30[];

#endif // BWPORT_DEMO_INTRO_H
