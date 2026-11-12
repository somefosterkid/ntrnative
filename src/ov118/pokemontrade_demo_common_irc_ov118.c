// Black's code at ov118 0x021d64f4-0x021d6e04. Placed by tools/gap_files.py from 9 pokebw2; not
// src/ov118/pokemontrade_demo_down.c's: its asserts name pokemontrade_demo_common; pokemontrade_demo_common_irc.c is
// ov117's file (gap_setup.py adds _ov118). (pokemontrade_demo_common_irc_ov118.c: another module has a file of the
// name)

#include "types.h"
#include "constants/pokemon.h"
#include "app/pokemon_trade_local.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/sound.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/mcss.h"
#include "system/wordset.h"

struct ClActRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface *surfaces;
    u32 surfaceCount;
};
struct ClActUnit {
    ClActor *actors;
    ClActor *actorList;
    ClActRenderer *renderer;
    u16 count;
    union {
        struct {
            u8 rendererType : 4;
            u8 unk4 : 4;
        };
        struct {
            u8 : 4;
            u8 active : 1;
            u8 unk5 : 1;
            u8 unk6 : 1;
        };
    };
    u8 priority;
    ClActUnit *next;
    ClActUnit *prev;
};
typedef struct {
    u32 type;
    NNSG2dCellDataBank *cells;
    NNSG2dAnimBankData *anims;
    union {
        struct {
            NNSG2dCellAnimation cellAnim;
            u32 transferHandle;
        };
        struct {
            NNSG2dMultiCellAnimation mcAnim;
            NNSG2dMultiCellDataBank *mcBank;
            NNSG2dAnimBankData *mcAnims;
            void *mcWork;
        };
    };
} ClActAnim;
struct ClActor {
    ClActor *next;
    ClActor *prev;
    ClActUnit *unit;
    ClActorPos pos;
    ClActorPos affineCenter;
    ClActorScale scale;
    NNSG2dImageProxy imgProxy;
    NNSG2dImagePaletteProxy pltProxy;
    fx32 unk54;
    u16 rotation;
    u16 sequence;
    u16 charIdx;
    u16 plttIdx;
    u32 priority : 8;
    u32 unk60_8 : 4;
    u32 palette : 4;
    u32 bgPriority : 2;
    u32 affineMode : 2;
    u32 flip : 2;
    u32 objMode : 2;
    u32 unk60_24 : 1;
    u32 unk60_25 : 1;
    u32 visible : 1;
    u32 playMode : 4;
    u32 unk60_31 : 1;
    u32 unk64_0 : 1;
    ClActAnim anim;
};
struct ArcTool {
    FSFile file;
    u32 fimgOffset;
    u16 fatbOffset;
    u16 fileCount;
};
typedef struct {
    u16 langCount;
    u16 lineCount;
    u32 maxBlockSize;
    u32 unk8;
    u32 langOffsets[0];
} MsgFileHeader;
typedef struct {
    u32 offset;
    u16 length;
    u16 unk6;
} MsgEntry;
struct MsgData {
    MsgFileHeader *header;
    void *block;
    MsgEntry entry;
    u32 fileOffset;
    ArcTool *arc;
    HeapID heapId;
    u8 lang;
    u8 preloaded;
};

s32 _s32_div_f(s32 a, s32 b);
void func_ov118_021d6544(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner);
void func_ov118_021d67d4(PokemonTradeWork *param_1);
void func_ov118_021d67f8(PokemonTradeWork *wk);
void func_ov118_021d6918(PokemonTradeWork *wk);
void func_ov118_021d6f90(PokemonTradeWork *wk);

static const VecFx32 data_ov118_021d7cd0 = { 0, -0x17000, -0xa000 };
static const VecFx32 data_ov118_021d7cc4 = { 0, -0x17000, 0xa000 };

static VecFx32 data_ov118_021d82c0[18] = {
#include "asset/pokemontrade_demo_common_irc_ov118_data_ov118_021d82c0.inc"
};

static VecFx32 data_ov118_021d8398[18] = {
#include "asset/pokemontrade_demo_common_irc_ov118_data_ov118_021d8398.inc"
};

static VecFx32 data_ov118_021d8470[40] = {
#include "asset/pokemontrade_demo_common_irc_ov118_data_ov118_021d8470.inc"
};

static VecFx32 data_ov118_021d8880[20];

void func_ov118_021d6ba4(PokemonTradeWork *param_1);

void func_ov118_021d64f4(TradeFade *param_1) {
    s32 step;
    s32 brightness;

    if (param_1 != NULL) {
        if ((s32)param_1->frames >= (s32)param_1->frame) {
                        param_1->frame += 1;
            step = _s32_div_f(((s32)param_1->end - (s32)param_1->start) * 0x1000, (s32)param_1->frames);
            brightness = step * param_1->frame;
            brightness = (s32)(brightness + ((u32)(brightness >> 0xb) >> 0x14)) >> 0xc;
            G2_SetBlendBrightness(0x1e, brightness);
            G2S_SetBlendBrightness(0x1f, brightness);
        }
    }
}

void func_ov118_021d6544(PokemonTradeWork *wk, u8 *ownedByPlayer, u8 *noBounce, PlayerInfo *player, BOOL partner) {
    PartyPkm *pkm;
    int side;
    u32 id;
    u32 species;
    u32 form;
    u32 egg;
    u32 noBounceParam;
    void *personal;

    if (partner) {
        if (func_ov119_021d8aa8(wk)) {
            side = 1;
        } else {
            side = 0;
        }
    } else {
        if (func_ov119_021d8aa8(wk)) {
            side = 0;
        } else {
            side = 1;
        }
    }
    pkm = func_ov119_021d8fa4(wk, side);
    id = PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
    species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
    form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    egg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    if (egg == 0) {
        personal = PML_PersonalLoad(species, form, wk->heapId);
        noBounceParam = PML_PersonalGetParam(personal, PERSONAL_NO_BOUNCE);
        PML_PersonalFree(personal);
    }
    if (egg != 0) {
        *noBounce = TRUE;
    } else if (noBounceParam != 0) {
        *noBounce = TRUE;
    } else {
        *noBounce = FALSE;
    }
    if (id != getIDAsUInt(player)) {
        *ownedByPlayer = FALSE;
    } else {
        *ownedByPlayer = TRUE;
    }
}


void func_ov118_021d6600(PokemonTradeWork *wk) {
    ClActor *actor;
    ClActorSetup setup;

    actor = wk->work3D->actor;
    if (actor == NULL) {
        setup.x = 0x80;
        setup.y = 99;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        actor = func_0204b294(wk->clactUnit, wk->objRes[TRADE_OBJRES_CHAR_DEMO], wk->objRes[TRADE_OBJRES_PLTT_DEMO],
                              wk->objRes[TRADE_OBJRES_CELL_DEMO], &setup, 0, wk->heapId);
        wk->work3D->actor = actor;
        func_0204b92c(wk->work3D->actor, 1);
        func_0204b3dc(wk->work3D->actor, 1);
        return;
    }
    func_0204b9a0(actor);
}

void func_ov118_021d667c(PokemonTradeWork *wk) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0x67, wk->heapId);
    wk->objRes[TRADE_OBJRES_CHAR_DEMO] = func_0204a6c8(arc, 2, FALSE, CLACT_VRAM_MAIN, wk->heapId);
    wk->objRes[TRADE_OBJRES_PLTT_DEMO] = func_0204ac18(arc, 3, CLACT_VRAM_MAIN, 0xe0, 0, 1, wk->heapId);
    wk->objRes[TRADE_OBJRES_CELL_DEMO] = func_0204af28(arc, 1, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    wk->work3D->actor = NULL;
}

void func_ov118_021d66e8(PokemonTradeWork *wk) {
    ClActor *actor;

    actor = wk->work3D->actor;
    if (actor != NULL) {
        func_0204b3b4(actor);
    }
    if (wk->objRes[TRADE_OBJRES_CHAR_DEMO] != 0) {
        func_0204a8d4(wk->objRes[TRADE_OBJRES_CHAR_DEMO]);
        func_0204ada4(wk->objRes[TRADE_OBJRES_PLTT_DEMO]);
        func_0204afd8(wk->objRes[TRADE_OBJRES_CELL_DEMO]);
    }
    wk->objRes[TRADE_OBJRES_CHAR_DEMO] = 0;
    wk->objRes[TRADE_OBJRES_PLTT_DEMO] = 0;
    wk->objRes[TRADE_OBJRES_CELL_DEMO] = 0;
    wk->work3D->actor = NULL;
}

// pokebw2's PokemonTradeIrcDemo_Frames
int func_ov118_021d6734(int frame) {
    return frame;
}

void func_ov118_021d6738(PokemonTradeWork *wk, int timer) {
    wk->timer = timer;
}

void func_ov118_021d6744(PokemonTradeWork *wk) {
    wk->bgmTimer = 1;
}

void func_ov118_021d6750(PokemonTradeWork *param_1) {
    BOOL BVar1;
    NetHandle *handle;

    func_ov119_021e3bb0(param_1);
    if (param_1->unk854 == 1) {
        func_ov118_021d6544(param_1, &param_1->unk11ED, &param_1->unk11EF, param_1->partnerInfo, 1);
        func_ov118_021d6544(param_1, &param_1->unk11EE, &param_1->unk11F0, param_1->myInfo, 0);
        func_ov119_021e3230(param_1);
        BVar1 = func_ov119_021d8a24(param_1);
        if (BVar1 != 0) {
            handle = func_020399a8();
            func_02039c68(handle, 0x11, 8);
            func_ov119_021d8e84(param_1, func_ov118_021d67d4);
            return;
        }
        func_ov119_021d8e84(param_1, func_ov118_021d67f8);
    }
}

void func_ov118_021d67d4(PokemonTradeWork *param_1) {
    NetHandle *handle;
    BOOL BVar1;

    handle = func_020399a8();
    BVar1 = func_02039cb4(handle, 0x11, 8);
    if (BVar1 != 0) {
        func_ov119_021d8e84(param_1, func_ov118_021d67f8);
    }
}

void func_ov118_021d67f8(PokemonTradeWork *wk) {
    VecFx32 first;
    VecFx32 second;

    func_ov118_021d6738(wk, 0);
    wk->fade = GFL_HeapAllocate(wk->heapId, sizeof(TradeFade), TRUE, "pokemontrade_demo_common.c", 245);
    wk->fade->frames = func_ov118_021d6734(14);
    wk->fade->frame = 0;
    wk->fade->start = 0;
    wk->fade->end = -16;
    first = data_ov118_021d7cc4;
    func_ov119_021dfd24(wk->mcssMoves[0]);
    wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov118_021d6734(42), &first, wk->heapId);
    second = data_ov118_021d7cd0;
    func_ov119_021dfd24(wk->mcssMoves[1]);
    wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov118_021d6734(84), &second, wk->heapId);
    func_02015f2c(wk->mcss[0], (uintptr_t)wk, (MCSSAnimationCallback)func_ov119_021dfd0c, TRUE);
    func_02015f50(wk->mcss[1], 0, 16, (s8)(func_ov118_021d6734(14) / 16), 0);
    func_ov119_021e106c(wk);
    GFL_BGSysResetStdPalette(1, 0);
    GFL_BGSysResetStdPalette(5, 0);
    func_ov118_021d6738(wk, 0);
    func_ov119_021d8e84(wk, func_ov118_021d6918);
}

void func_ov118_021d6918(PokemonTradeWork *wk) {
    PartyPkm *pkm;

    func_ov118_021d64f4(wk->fade);
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
    if (wk->timer > func_ov118_021d6734(42)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0);
        func_ov119_021e64b8(wk);
        func_ov119_021e4888(wk);
        func_ov119_021e2680(wk);
        func_ov118_021d6738(wk, 0);
        pkm = func_ov119_021d8fa4(wk, func_ov119_021d8aa8(wk));
        PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
        if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0) {
            u32 species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
            u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
            PokeVoiceChatterInfo info;

            PokeVoice_CreateChatterInfo(&info);
            PokeVoice_Play(species, form, 0x40, 0, 0, 0, 0, &info);
        }
        if (wk->mcssMoves[0] != NULL) {
            GFL_HeapFree(wk->mcssMoves[0]);
            wk->mcssMoves[0] = NULL;
        }
        if (wk->unk11ED == 0) {
            VecFx32 pos;

            func_02015c30(wk->mcss[0], &pos);
            func_ov119_021dfd24(wk->mcssMoves[0]);
            if (wk->unk11EF != 0) {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 20, &pos, data_ov118_021d8880, wk->heapId);
            } else {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 18, &pos, data_ov118_021d82c0, wk->heapId);
            }
            func_ov119_021e36c8(wk, 2, 0, func_ov119_021d8fa4(wk, func_ov119_021d8aa8(wk)), TRUE);
            func_02015e04(wk->mcss[2]);
            func_02015ea0(wk->mcss[2]);
        } else {
            VecFx32 pos;

            func_02015c30(wk->mcss[0], &pos);
            func_ov119_021dfd24(wk->mcssMoves[0]);
            if (wk->unk11EF != 0) {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 20, &pos, data_ov118_021d8880, wk->heapId);
            } else {
                wk->mcssMoves[0] = func_ov119_021dfbb4(wk->mcss[0], 40, &pos, data_ov118_021d8470, wk->heapId);
            }
        }
        func_ov119_021d8e84(wk, func_ov118_021d6ba4);
    }
}



void func_ov118_021d6ad4(PokemonTradeWork *param_1) {
    MsgData *msgData;
    BOOL side;
    PartyPkm *pkm;
    func_0203d34c(0, param_1->heapId);
    if (param_1->type == 6) {
        msgData = param_1->msgData;
        GFL_MsgDataLoadStrbuf(msgData, 0x98, param_1->strbufTemplate);
    } else if (param_1->type == 8) {
        msgData = param_1->msgData;
        GFL_MsgDataLoadStrbuf(msgData, 0x9b, param_1->strbufTemplate);
    } else if (param_1->unk11ED != 0) {
        msgData = param_1->msgData;
        GFL_MsgDataLoadStrbuf(msgData, 0x82, param_1->strbufTemplate);
    } else {
        msgData = param_1->msgData;
        GFL_MsgDataLoadStrbuf(msgData, 0x81, param_1->strbufTemplate);
    }
    side = func_ov119_021d8aa8(param_1);
    pkm = func_ov119_021d8fa4(param_1, side);
    loadPokemonNicknameToStrbuf(param_1->wordSet, 0, pkm);
    copyVarForText(param_1->wordSet, 1, param_1->partnerInfo);
    GFL_WordSetFormatStrbuf(param_1->wordSet, param_1->strbuf, param_1->strbufTemplate);
    GFL_BGSysFillScrArea(4, 0, 0, 0, 0x20, 0x18, 0x11);
    func_ov119_021e0ff4(param_1, 1, 0);
    func_ov119_021e7208(param_1, 0x14);
    reg_G2S_DB_BLDCNT = 0;
}

void func_ov118_021d6ba4(PokemonTradeWork *param_1) {
    BOOL BVar1;
    VecFx32 VStack_20;

    if (param_1->timer == 10) {
        func_ov118_021d6ad4(param_1);
    }
    if ((param_1->unk11ED == 0) && (param_1->timer == 6)) {
        func_02015c30(param_1->mcss[0], &VStack_20);
        func_02015c48(param_1->mcss[2], &VStack_20);
        func_02015eb0(param_1->mcss[2]);
        func_02015ea0(param_1->mcss[0]);
        func_02015e4c(param_1->mcss[2]);
        param_1->mcssMoves[0]->mcss = param_1->mcss[2];
    }
    if ((200 < param_1->timer) && (BVar1 = func_ov119_021e12ac(param_1), BVar1 != 0)) {
        func_ov119_021e1098(param_1);
        if (param_1->unk11ED == 0) {
            func_ov119_021e36d8(param_1, 0);
            param_1->mcss[0] = param_1->mcss[2];
            param_1->mcss[2] = NULL;
        }
        if (param_1->mcssMoves[0] != NULL) {
            GFL_HeapFree(param_1->mcssMoves[0]);
            param_1->mcssMoves[0] = NULL;
        }
        func_ov119_021d8e84(param_1, func_ov118_021d6f90);
    }
    func_ov119_021dfc68(param_1->mcssMoves[0], param_1);
}

void func_ov118_021d6c78(PokemonTradeWork *wk) {
    func_ov118_021d66e8(wk);
    func_ov119_021e3034(wk->work3D, 6);
    func_ov119_021e2f2c(wk->work3D);
    GFL_HeapFree(wk->work3D);
    wk->work3D = NULL;
    func_ov119_021d8e84(wk, func_ov119_021dfda0);
}


void func_ov118_021d6ca8(PokemonTradeWork *wk, int index) {
    if (wk->mcssMoves[0] != NULL) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
    }
}

void func_ov118_021d6cc4(PokemonTradeWork *wk, int index) {
    PartyPkm *pkm;
    VecFx32 pos;

    pkm = func_ov119_021d8fa4(wk, 1 - func_ov119_021d8aa8(wk));
    PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
    if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0) {
        u32 species = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
        u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);

        PokeVoice_Play(species, form, 0x40, 0, 0, 0, 0, NULL);
    }
    if (wk->unk11EE == 0) {
        func_02015c30(wk->mcss[index], &pos);
        func_ov119_021dfd24(wk->mcssMoves[3]);
        if (wk->unk11F0 != 0) {
            wk->mcssMoves[3] = func_ov119_021dfbb4(wk->mcss[index], 20, &pos, data_ov118_021d8880, wk->heapId);
        } else {
            wk->mcssMoves[3] = func_ov119_021dfbb4(wk->mcss[index], 40, &pos, data_ov118_021d8470, wk->heapId);
        }
    } else {
        u32 pair[4] = { 2, 3, 0, 1 };
        MCSS *target = wk->mcss[pair[index]];

        func_02015c30(wk->mcss[index], &pos);
        func_ov119_021dfd24(wk->mcssMoves[3]);
        if (wk->unk11F0 != 0) {
            wk->mcssMoves[3] = func_ov119_021dfbb4(target, 20, &pos, data_ov118_021d8880, wk->heapId);
        } else {
            wk->mcssMoves[3] = func_ov119_021dfbb4(target, 18, &pos, data_ov118_021d8398, wk->heapId);
        }
    }
}

