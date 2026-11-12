// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/ddemo_main.h: the lines in use so far.
#ifndef BWPORT_DEMO_DDEMO_MAIN_H
#define BWPORT_DEMO_DDEMO_MAIN_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/particle.h"
#include "system/mcss.h"
#include "struct_decls.h"
#include "work_area.h"

// The scene that runs, which chooses what a frame draws
enum {
    DDEMO_SCENE_NONE,
    // The Pokémon one by one, with 3D on both screens for the particles
    DDEMO_SCENE_1ST,
    // The trainer and the team
    DDEMO_SCENE_2ND,
};

// Black 2's; twinned code confirms the values of DDEMOOBJ_POKE_FRONT, DDEMOOBJ_POKE_BACK, DDEMOOBJ_PLATE_TITLE,
// DDEMOOBJ_PLATE_NAME, DDEMOOBJ_FLASH, DDEMOOBJ_CONFETTI and DDEMOOBJ_COUNT in Black
// The actors
enum {
    // The first scene's: the Pokémon's front and back, the name plates and the flash
    DDEMOOBJ_POKE_FRONT,
    DDEMOOBJ_POKE_BACK,
    DDEMOOBJ_PLATE_TITLE,
    DDEMOOBJ_PLATE_NAME,
    DDEMOOBJ_FLASH,
    // The second scene's: the trainer on both screens, the plates and the confetti
    DDEMOOBJ_TRAINER_MAIN,
    DDEMOOBJ_TRAINER_SUB,
    DDEMOOBJ_PLATE_UPPER,
    DDEMOOBJ_PLATE_LOWER,
    DDEMOOBJ_CONFETTI,
    DDEMOOBJ_COUNT = DDEMOOBJ_CONFETTI + 24,
};

// Black 2's; twinned code confirms the values of DDEMO_CHR_POKE_FRONT, DDEMO_CHR_POKE_BACK and DDEMO_CHR_COUNT in Black
// The actors' resources
enum {
    DDEMO_CHR_POKE_FRONT,
    DDEMO_CHR_POKE_BACK,
    DDEMO_CHR_OBJ,
    DDEMO_CHR_TRAINER_MAIN,
    DDEMO_CHR_TRAINER_SUB,
    DDEMO_CHR_OBJ_SUB,
    DDEMO_CHR_COUNT,
};

// Black 2's; twinned code confirms the values of DDEMO_PAL_POKE_FRONT, DDEMO_PAL_POKE_BACK, DDEMO_PAL_FONT and
// DDEMO_PAL_COUNT in Black
enum {
    DDEMO_PAL_POKE_FRONT,
    DDEMO_PAL_POKE_BACK,
    DDEMO_PAL_OBJ,
    DDEMO_PAL_FONT,
    DDEMO_PAL_TRAINER_MAIN,
    DDEMO_PAL_TRAINER_SUB,
    DDEMO_PAL_OBJ_SUB,
    DDEMO_PAL_COUNT,
};

// Black 2's; twinned code confirms the values of DDEMO_CEL_POKE_FRONT, DDEMO_CEL_POKE_BACK and DDEMO_CEL_COUNT in Black
enum {
    DDEMO_CEL_POKE_FRONT,
    DDEMO_CEL_POKE_BACK,
    DDEMO_CEL_OBJ,
    DDEMO_CEL_TRAINER_MAIN,
    DDEMO_CEL_TRAINER_SUB,
    DDEMO_CEL_OBJ_SUB,
    DDEMO_CEL_COUNT,
};

// The texts drawn on sprites: the first scene's title and Pokémon, the second's title and trainer
enum {
    DDEMO_FOAM_TITLE,
    DDEMO_FOAM_NAME,
    DDEMO_FOAM_TITLE_2ND,
    DDEMO_FOAM_PLAYER,
    DDEMO_FOAM_COUNT,
};

// Black 2's; twinned code confirms the values of DDEMO_PTC_TYPE and DDEMO_PTC_NAME in Black
// The particle systems: the Pokémon's type's, and the name plates'
enum {
    DDEMO_PTC_TYPE,
    DDEMO_PTC_NAME,
    DDEMO_PTC_COUNT,
};

// Black 2's, not checked against Black. The SPL manager's work, whose emitters and particles hold pointers
#define DDEMO_PTC_WORK_SIZE WORK_AREA_SIZE(0x4800)
#define DDEMO_POKE_MAX 6

// The steps of a shake, after which the actor is back in place
#define DDEMO_SHAKE_COUNT 8

struct DDemoFontOam {
    BmpOamActor *oam;
    GFLBitmap *bmp;
};

// Black 2's; matching code confirms the offsets of param, vblankTask, scene, seHandle, g3dMgr, mcssSys, mcss, camera,
// mcssAnmEnd, ptc, font, fontSmall, msgData, wordSet, strbuf, clUnit, clact, chrRes, palRes, celRes, renderer, bmpOam,
// fontOam, mons, species, type, form, monIndex, monCount, shakeIndex, shakeBaseX, shakeBaseY, shakeX, shakeY, voice,
// count, mainSeq, subSeq, nextSeq and end in Black
struct DDemoWork {
    DendouDemoParam *param;
    TCB *vblankTask;
    // DDEMO_SCENE_*
    u32 scene;
    BGMStackPreload *seHandle;
    G3DManager *g3dMgr;
    u32 unk14;
    MCSSSystem *mcssSys;
    MCSS *mcss;
    G3DCamera *camera;
    // Set once the Pokémon's animation has played to its end
    BOOL mcssAnmEnd;
    ParticleSystem *ptc[DDEMO_PTC_COUNT];
    u8 ptcWork[DDEMO_PTC_COUNT][DDEMO_PTC_WORK_SIZE];
    Font *font;
    Font *fontSmall;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *strbuf;
    ClActUnit *clUnit;
    ClActor *clact[DDEMOOBJ_COUNT];
    u32 chrRes[DDEMO_CHR_COUNT];
    u32 palRes[DDEMO_PAL_COUNT];
    u32 celRes[DDEMO_CEL_COUNT];
    ClActRenderer *renderer;
    BmpOamSys *bmpOam;
    DDemoFontOam fontOam[DDEMO_FOAM_COUNT];
    // The team's Pokémon that are not Eggs, and the one being shown
    PartyPkm *mons[DDEMO_POKE_MAX];
    u16 species;
    u16 type;
    u8 form;
    u8 monIndex;
    u8 monCount;
    // The shake of an actor around its place
    u8 shakeIndex;
    s16 shakeBaseX;
    s16 shakeBaseY;
    s16 shakeX[DDEMO_SHAKE_COUNT];
    s16 shakeY[DDEMO_SHAKE_COUNT];
    u32 voice;
    // Counts frames, in the steps and in the wait sequence
    int count;
    int mainSeq;
    int subSeq;
    int nextSeq;
    BOOL end;
};

// pokebw2's DDemoMain_MainDouble3D
void func_ov180_021b6704(DDemoWork *wk);
// pokebw2's DDemoMain_Main3D
void func_ov180_021b66e4(DDemoWork *wk);
// pokebw2's DDemoMain_InitBg
void func_ov180_021b6210(void);
// pokebw2's DDemoMain_InitSound
void func_ov180_021b64ac(DDemoWork *wk);
// pokebw2's DDemoMain_GetPokeMax
void func_ov180_021b64dc(DDemoWork *wk);
// pokebw2's DDemoMain_ExitMsg
void func_ov180_021b6474(DDemoWork *wk);
// pokebw2's DDemoMain_InitVram
void func_ov180_021b61d4(u32 scene);
// pokebw2's DDemoMain_InitBgMode
void func_ov180_021b6224(void);
// pokebw2's DDemoMain_Init3D
void func_ov180_021b66a0(DDemoWork *wk);
// pokebw2's DDemoMain_SetBlendAlpha
void func_ov180_021b63e8(void);
// pokebw2's DDemoMain_Exit3D
void func_ov180_021b66d4(DDemoWork *wk);
// pokebw2's DDemoMain_GetPokeData
void func_ov180_021b654c(DDemoWork *wk);
// pokebw2's DDemoMain_LoadPokeVoice
void func_ov180_021b65a0(DDemoWork *wk);
// pokebw2's DDemoMain_SetTypeParticle
void func_ov180_021b67c4(DDemoWork *wk, u32 id);
// pokebw2's DDemoMain_SetNameParticle
void func_ov180_021b682c(DDemoWork *wk, u32 id);
// pokebw2's DDemoMain_DeleteNameParticle
void func_ov180_021b681c(DDemoWork *wk);
// pokebw2's DDemoMain_DeleteTypeParticle
void func_ov180_021b67b4(DDemoWork *wk);
// pokebw2's DDemoMain_InitBg2nd
void func_ov180_021b6254(void);
// pokebw2's DDemoMain_InitMcss
void func_ov180_021b68a8(DDemoWork *wk);
// pokebw2's DDemoMain_ExitBg2nd
void func_ov180_021b6340(void);
// pokebw2's DDemoMain_ExitMcss
void func_ov180_021b6914(DDemoWork *wk);
// pokebw2's DDemoMain_AddMcss
void func_ov180_021b692c(DDemoWork *wk);
// pokebw2's DDemoMain_MoveMcss
void func_ov180_021b699c(DDemoWork *wk, s32 move);
// pokebw2's DDemoMain_SetMcssCallBack
void func_ov180_021b69f8(DDemoWork *wk);
// pokebw2's DDemoMain_DelMcss
void func_ov180_021b6984(DDemoWork *wk);

// Black 2's, not checked against Black
// The members of ARCID_DENDOU_DEMO that the demo loads by number
enum {
    NARC_DDEMO_BG_NCLR = 1,
    NARC_DDEMO_OBJ_NCLR = 3,
    NARC_DDEMO_BG_NCGR = 22,
    NARC_DDEMO_OBJ_1ST_NCGR = 24,
    NARC_DDEMO_OBJ_2ND_NCGR = 25,
    NARC_DDEMO_OBJ_1ST_NCER = 26,
    NARC_DDEMO_OBJ_2ND_NCER = 27,
    NARC_DDEMO_BG_NSCR = 28,
    NARC_DDEMO_OBJ_1ST_NANR = 32,
    NARC_DDEMO_OBJ_2ND_NANR = 33,
    NARC_DDEMO_NAME_SPA = 51,
};

// pokebw2's DDemoMain_GetVramConfig
const BGSysVRAMConfig *func_ov180_021b61fc(u32 scene);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DDemoMain_InitMsg
void func_ov180_021b6418(DDemoWork *wk);
// pokebw2's DDemoMain_ExitSound
void func_ov180_021b64d0(DDemoWork *wk);
// pokebw2's DDemoMain_ExitBg
void func_ov180_021b621c(void);
// pokebw2's DDemoMain_InitParticle
void func_ov180_021b6764(void);
// pokebw2's DDemoMain_InitDouble3D
void func_ov180_021b6750(void);
// pokebw2's DDemoMain_InitVBlank1st
void func_ov180_021b6158(DDemoWork *wk);
// pokebw2's DDemoMain_ExitVBlank
void func_ov180_021b6188(DDemoWork *wk);
// pokebw2's DDemoMain_ExitDouble3D
void func_ov180_021b675c(void);
// pokebw2's DDemoMain_ExitParticle
void func_ov180_021b6770(void);
// pokebw2's DDemoMain_CreateTypeParticle
void func_ov180_021b6778(DDemoWork *wk);
// pokebw2's DDemoMain_CreateNameParticle
void func_ov180_021b67ec(DDemoWork *wk);
// pokebw2's DDemoMain_LoadBg
void func_ov180_021b6360(void);
// pokebw2's DDemoMain_InitVBlank2nd
void func_ov180_021b6170(DDemoWork *wk);

#endif // BWPORT_DEMO_DDEMO_MAIN_H
