// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/sta_act_poke.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_item_draw.h"
#include "app/musical/mus_poke_draw.h"
#include "app/musical/sta_act_effect.h"
#include "app/musical/sta_act_poke.h"
#include "app/musical/sta_acting.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "field/musical.h"
#include "gfl/blact.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "pml/personal.h"
#include "app/musical/mus_item_data.h"
#include "math.h"

// Overlay 209's sta_act_poke.c: the Pokémon on the stage. Positions are in fixed point pixels, y down, which the
// sprites and the props get scaled down by 16 and y up

// The personal data's flag that keeps a Pokémon from turning around (not from swan)
#define PERSONAL_NO_FLIP 34

// The kinds of a prop's effect, as overlay 210's table gives them
enum {
    STA_ACT_POKE_ITEM_SPIN,
    STA_ACT_POKE_ITEM_PARTICLE,
    STA_ACT_POKE_ITEM_FALL,
    STA_ACT_POKE_ITEM_THROW,
    STA_ACT_POKE_ITEM_PULSE,
};

// How many frames a prop's effect lasts
#define STA_ACT_POKE_ITEM_FRAMES 180

static void func_ov112_021f7204(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f732c(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f75d8(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f77ec(StaActPokeSys *sys, StaActPoke *poke, u16 species);
static void func_ov112_021f7a94(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7a98(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7aec(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7ba8(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7c90(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7d30(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7e2c(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7ecc(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7fa8(StaActPokeSys *sys, StaActPoke *poke);
static void func_ov112_021f7fac(StaActPokeSys *sys, StaActPoke *poke);

// pokebw2's STA_ACT_POKE_ITEM_SE
static const u32 data_ov112_021f832c[] = {
    SEQ_SE_MSCL_16, SEQ_SE_MSCL_17, SEQ_SE_MSCL_18, SEQ_SE_W319_02, SEQ_SE_MSCL_19,
};

StaActPokeSys *func_ov112_021f709c(HeapID heapId, StaActing *stage, MusPokeDrawSys *pokeDraw,
                                     MusItemDrawSys *itemDraw, BlActScene *blact) {
    u8 i;
    StaActPokeSys *sys = GFL_HeapAllocate(heapId, sizeof(StaActPokeSys), FALSE, "sta_act_poke.c", 147);

    sys->heapId = heapId;
    sys->stage = stage;
    sys->pokeDraw = pokeDraw;
    sys->itemDraw = itemDraw;
    sys->scroll = 0;
    sys->blact = blact;
    sys->effectCount = 0;
    for (i = 0; i < 4; i++) {
        sys->pokes[i].active = FALSE;
        sys->pokes[i].update = FALSE;
    }
    sys->shadowMaterial = func_0204efc8(sys->blact, ARCID_MUSICAL, 29, 3, 0x33, 64, 64);
    return sys;
}

// pokebw2's StaActPoke_TermSystem
void func_ov112_021f7120(StaActPokeSys *sys) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sys->pokes[i].active == TRUE) {
            func_ov112_021f77a0(sys, &sys->pokes[i]);
        }
    }
    BlActScene_FreeMaterial(sys->blact, sys->shadowMaterial);
    GFL_HeapFree(sys);
}

// pokebw2's StaActPoke_UpdateSystem
void func_ov112_021f7160(StaActPokeSys *sys) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sys->pokes[i].active == TRUE) {
            func_ov112_021f7204(sys, &sys->pokes[i]);
        }
    }
}

// pokebw2's StaActPoke_UpdateSystem_Item
void func_ov112_021f718c(StaActPokeSys *sys) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sys->pokes[i].active == TRUE) {
            func_ov112_021f732c(sys, &sys->pokes[i]);
            if (sys->stage != NULL) {
                u32 j;

                for (j = 0; j < func_ov132_021f9e44(sys->stage); j++) {
                    if (sys->pokes[i].itemFunc != NULL) {
                        sys->pokes[i].itemFunc(sys, &sys->pokes[i]);
                    }
                }
            }
        }
    }
    func_ov112_021f4990(sys->itemDraw);
}

// pokebw2's StaActPoke_UpdatePoke
static void func_ov112_021f7204(StaActPokeSys *sys, StaActPoke *poke) {
    VecFx32 *mark4 = func_ov112_021f545c(poke->draw);
    VecFx32 *mark5 = func_ov112_021f5464(poke->draw);
    VecFx32 pos;
    VecFx32 scale;
    VecFx32 center;
    MtxFx33 rotation;
    u16 angle;

    VEC_Add(&poke->pos, &poke->offset, &pos);
    angle = poke->rotation;
    center.x = 0;
    center.y = mark5->y;
    MTX_RotZ33_(&rotation, -FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_MultVec33(&center, &rotation, &poke->rotOffset);
    VEC_Subtract(&center, &poke->rotOffset, &poke->rotOffset);
    pos.x = (pos.x - mark4->x - poke->rotOffset.x) / 16;
    pos.y = (FX32_CONST(192) - (poke->rotOffset.y + (pos.y - mark4->y))) / 16;
    func_ov112_021f5350(poke->draw, &pos);
    func_ov112_021f5368(poke->draw, poke->rotation);
    scale.x = poke->scale.x * 16;
    scale.y = poke->scale.y * 16;
    scale.z = poke->scale.z * 16;
    if (poke->flip == TRUE) {
        scale.x *= -1;
    }
    func_ov112_021f535c(poke->draw, &scale);
    poke->update = FALSE;
    if (poke->animating == TRUE) {
        poke->blinkTimer += GFL_RandomLC(2);
        if (poke->blinkTimer > 120) {
            func_ov112_021f5424(poke->draw, TRUE);
        } else {
            func_ov112_021f5424(poke->draw, FALSE);
        }
        if (poke->blinkTimer > 125) {
            poke->blinkTimer = GFL_RandomLC(120) / 3;
        }
    }
}

// pokebw2's StaActPoke_UpdateItems
static void func_ov112_021f732c(StaActPokeSys *sys, StaActPoke *poke) {
    int i;
    VecFx32 pos;

    for (i = 0; i < 9; i++) {
        u32 equipPos = i;

        // A Pokémon facing the other way wears its left props on the right
        if (poke->flip == TRUE) {
            if (i == 7) {
                equipPos = 8;
            } else if (i == 8) {
                equipPos = 7;
            } else if (i == 0) {
                equipPos = 1;
            } else if (i == 1) {
                equipPos = 0;
            }
        }
        if (poke->items[i] != NULL && poke->showItem[i] == TRUE) {
            if (poke->showItems == TRUE && poke->isFront == TRUE) {
                MusPokeDrawEquipPos *equip = func_ov112_021f5450(poke->draw, equipPos);

                if (equip->valid == TRUE) {
                    BOOL flipped = equip->info.scale.x < 0 ? TRUE : FALSE;
                    u16 rot = 0x10000 - equip->info.rotation;
                    u16 itemRot = (0x10000 + equip->info.cellRotation + poke->equips[i]->unk2) % 0x10000;
                    fx32 scaleX;
                    MtxFx33 rotation;
                    VecFx32 center;
                    VecFx32 offset;

                    func_ov112_021f545c(poke->draw);
                    scaleX = equip->info.scale.x;
                    MTX_RotZ33_(&rotation, -FX_SinIdx(rot), FX_CosIdx(rot));
                    MTX_MultVec33(&equip->info.offset, &rotation, &offset);
                    if (flipped == TRUE) {
                        itemRot = 0x10000 - itemRot;
                    }
                    if (func_ov112_021f4b30(poke->items[i]) == FALSE && flipped == TRUE) {
                        scaleX = -scaleX;
                    }
                    MTX_MultVec33(&equip->info.center, &rotation, &center);
                    VEC_Subtract(&equip->info.center, &center, &center);
                    pos.x = equip->info.pos.x + offset.x + FX32_CONST(128) + FX32_CONST(sys->scroll);
                    pos.y = equip->info.pos.y + offset.y + FX32_CONST(96);
                    if (func_ov112_021f4b24(poke->items[i]) == TRUE) {
                        pos.z = poke->pos.z - FX32_CONST(20);
                    } else {
                        if (equipPos == 7 || equipPos == 8) {
                            pos.z = poke->pos.z + FX32_CONST(10) - (poke->equips[i]->slot << 11);
                        } else {
                            pos.z = poke->pos.z + FX32_CONST(5) - (poke->equips[i]->slot << 11);
                        }
                    }
                    func_ov112_021f4e8c(sys->itemDraw, poke->items[i], &pos);
                    func_ov112_021f4ed4(sys->itemDraw, poke->items[i], itemRot - rot);
                    func_ov112_021f4eb4(sys->itemDraw, poke->items[i], scaleX / 16 / 4, equip->info.scale.y / 16 / 4);
                    func_ov112_021f4e6c(sys->itemDraw, poke->items[i], TRUE);
                }
            } else {
                func_ov112_021f4e6c(sys->itemDraw, poke->items[i], FALSE);
            }
        }
    }
    func_ov112_021f545c(poke->draw);
    pos.x = poke->pos.x / 16;
    pos.y = (FX32_CONST(192) - poke->pos.y) / 16;
    pos.z = 50;
    BlActScene_SetActorPos(sys->blact, poke->shadowActor, &pos);
}

// pokebw2's StaActPoke_DrawSystem
void func_ov112_021f75a0(StaActPokeSys *sys) {
    int i;

    for (i = 0; i < 4; i++) {
        if (sys->pokes[i].active == TRUE && func_ov112_021f8120(sys, &sys->pokes[i]) == TRUE) {
            func_ov112_021f75d8(sys, &sys->pokes[i]);
        }
    }
}

// pokebw2's StaActPoke_DrawPoke
static void func_ov112_021f75d8(StaActPokeSys *sys, StaActPoke *poke) {
}

// pokebw2's StaActPoke_SetScrollOffset
void func_ov112_021f75dc(StaActPokeSys *sys, u16 scroll) {
    u8 i;

    sys->scroll = scroll;
    // BUG: The loop runs over 9 Pokémon, where the system has 4
#ifdef BUGFIX
    for (i = 0; i < 4; i++) {
#else
    for (i = 0; i < 9; i++) {
#endif
        if (sys->pokes[i].active == TRUE) {
            sys->pokes[i].update = TRUE;
        }
    }
}

// pokebw2's StaActPoke_CreatePoke
StaActPoke *func_ov112_021f760c(StaActPokeSys *sys, MusicalPoke *musPoke) {
    u8 i;
    StaActPoke *poke;
    void *personal;
    BOOL show;

    // BUG: The search runs over 9 Pokémon, where the system has 4
#ifdef BUGFIX
    for (i = 0; i < 4; i++) {
#else
    for (i = 0; i < 9; i++) {
#endif
        if (sys->pokes[i].active == FALSE) {
            break;
        }
    }
    poke = &sys->pokes[i];
    sys->pokes[i].active = TRUE;
    poke->update = TRUE;
    poke->isFront = TRUE;
    poke->showItems = FALSE;
    poke->noBounce = FALSE;
    poke->noFlip = FALSE;
    poke->itemFunc = NULL;
    poke->draw = func_ov112_021f5204(sys->pokeDraw, musPoke, TRUE);
    personal = PML_PersonalLoad(musPoke->species, musPoke->form, sys->heapId);
    if (PML_PersonalGetParam(personal, PERSONAL_NO_BOUNCE) == TRUE) {
        poke->noBounce = TRUE;
    }
    if (PML_PersonalGetParam(personal, PERSONAL_NO_FLIP) == TRUE) {
        poke->noFlip = TRUE;
    }
    PML_PersonalFree(personal);
    poke->pos.x = 0;
    poke->pos.y = 0;
    poke->pos.z = 0;
    poke->offset.x = 0;
    poke->offset.y = 0;
    poke->offset.z = 0;
    poke->scale.x = FX32_ONE;
    poke->scale.y = FX32_ONE;
    poke->scale.z = FX32_ONE;
    func_ov112_021f77ec(sys, poke, musPoke->species);
    poke->rotation = 0;
    for (i = 0; i < 9; i++) {
        u16 itemId = musPoke->equips[i].itemId;

        poke->equips[i] = &musPoke->equips[i];
        if (itemId != 0xff) {
            poke->showItem[i] = TRUE;
            poke->itemRes[i] = func_ov112_021f4b3c(itemId);
            poke->items[i] = func_ov112_021f4b88(sys->itemDraw, itemId, poke->itemRes[i], &poke->pos);
            func_ov112_021f4eb4(sys->itemDraw, poke->items[i], FX32_ONE / 4, FX32_ONE / 4);
            func_ov112_021f4e6c(sys->itemDraw, poke->items[i], FALSE);
        } else {
            poke->showItem[i] = FALSE;
            poke->itemRes[i] = NULL;
            poke->items[i] = NULL;
        }
    }
    poke->shadowActor = BlActScene_AddNewActor(sys->blact, sys->shadowMaterial, 0x400, 0x300, &poke->pos, 30, 0, 0);
    show = TRUE;
    BlActScene_SetActorHidden(sys->blact, poke->shadowActor, &show);
    poke->blinkTimer = GFL_RandomLC(150) / 2;
    poke->animating = TRUE;
    return poke;
}

// pokebw2's StaActPoke_DeletePoke
void func_ov112_021f77a0(StaActPokeSys *sys, StaActPoke *poke) {
    u8 i;

    BlActScene_ClearActorMaterial(sys->blact, poke->shadowActor);
    for (i = 0; i < 9; i++) {
        if (poke->items[i] != NULL) {
            func_ov112_021f4d40(sys->itemDraw, poke->items[i]);
            func_ov112_021f4b4c(poke->itemRes[i]);
            poke->itemRes[i] = NULL;
            poke->items[i] = NULL;
        }
    }
    func_ov112_021f5330(sys->pokeDraw, poke->draw);
    poke->active = FALSE;
}

// The species whose sprites are drawn a little narrower, wider or shorter
// pokebw2's StaActPoke_SetScaleBySpecies
static void func_ov112_021f77ec(StaActPokeSys *sys, StaActPoke *poke, u16 species) {
    if (species == SPECIES_DONPHAN) {
        poke->scale.x = 0xfb3;
    } else if (species == SPECIES_GROTLE || species == SPECIES_STOUTLAND) {
        poke->scale.x = 0xfc0;
    } else if (species == SPECIES_GRAVELER || species == SPECIES_FORRETRESS || species == SPECIES_SUICUNE) {
        poke->scale.x = 0xfcc;
    } else if (species == SPECIES_VILEPLUME || species == SPECIES_NUMEL) {
        poke->scale.x = 0x1040;
    } else if (species == SPECIES_BLASTOISE || species == SPECIES_NIDORINO || species == SPECIES_POLIWHIRL ||
               species == SPECIES_ALAKAZAM || species == SPECIES_GOLEM || species == SPECIES_MR_MIME ||
               species == SPECIES_SCYTHER || species == SPECIES_KABUTOPS || species == SPECIES_HERACROSS ||
               species == SPECIES_SMEARGLE || species == SPECIES_GROVYLE || species == SPECIES_COMBUSKEN ||
               species == SPECIES_LINOONE || species == SPECIES_SWELLOW || species == SPECIES_BRELOOM ||
               species == SPECIES_ROSELIA || species == SPECIES_ANORITH || species == SPECIES_STUNKY ||
               species == SPECIES_GABITE || species == SPECIES_MANDIBUZZ) {
        poke->scale.y = 0xfcc;
    } else if (species == SPECIES_VULPIX || species == SPECIES_SNEASEL || species == SPECIES_MAGCARGO ||
               species == SPECIES_GLALIE || species == SPECIES_FRILLISH || species == SPECIES_HAXORUS) {
        poke->scale.y = 0xfd9;
    } else if (species == SPECIES_PIDGEOT || species == SPECIES_PERSIAN || species == SPECIES_MAGMAR ||
               species == SPECIES_TAUROS || species == SPECIES_QUILAVA || species == SPECIES_TYPHLOSION ||
               species == SPECIES_AMPHAROS || species == SPECIES_STEELIX || species == SPECIES_LARVITAR ||
               species == SPECIES_TYRANITAR || species == SPECIES_MEDICHAM || species == SPECIES_BANETTE ||
               species == SPECIES_REGICE || species == SPECIES_PIPLUP || species == SPECIES_STARAPTOR ||
               species == SPECIES_FLOATZEL || species == SPECIES_ABOMASNOW || species == SPECIES_COTTONEE ||
               species == SPECIES_SIGILYPH || species == SPECIES_GOLURK || species == SPECIES_RESHIRAM) {
        poke->scale.y = 0xfe6;
    } else if (species == SPECIES_NINETALES || species == SPECIES_VIGOROTH) {
        poke->scale.y = 0xff3;
    } else if (species == SPECIES_HIPPOPOTAS) {
        poke->scale.x = 0x1040;
        poke->scale.y = 0xfcc;
    } else if (species == SPECIES_GOLETT) {
        poke->scale.x = 0xfc0;
        poke->scale.y = 0xfcc;
    }
}

// pokebw2's StaActPoke_StartItemEffect
void func_ov112_021f79a8(StaActPokeSys *sys, StaActPoke *poke, u32 pos) {
    if (poke->items[pos] != NULL && poke->itemFunc == NULL) {
        u8 type = func_ov112_021f514c(func_ov112_021f4f10(sys->itemDraw), poke->equips[pos]->itemId);

        poke->itemWork.timer = 0;
        poke->itemWork.pos = pos;
        poke->itemWork.effect = NULL;
        GFL_SEPlayKeepVol(data_ov112_021f832c[type], 2);
        sys->effectCount++;
        switch (type) {
        case STA_ACT_POKE_ITEM_SPIN:
            func_ov112_021f7a94(sys, poke);
            poke->itemFunc = func_ov112_021f7a98;
            break;
        case STA_ACT_POKE_ITEM_PARTICLE:
            func_ov112_021f7aec(sys, poke);
            poke->itemFunc = func_ov112_021f7ba8;
            break;
        case STA_ACT_POKE_ITEM_FALL:
            func_ov112_021f7c90(sys, poke);
            poke->itemFunc = func_ov112_021f7d30;
            break;
        case STA_ACT_POKE_ITEM_THROW:
            func_ov112_021f7e2c(sys, poke);
            poke->itemFunc = func_ov112_021f7ecc;
            break;
        case STA_ACT_POKE_ITEM_PULSE:
            func_ov112_021f7fa8(sys, poke);
            poke->itemFunc = func_ov112_021f7fac;
            break;
        }
    }
}

// pokebw2's StaActPoke_IsItemEffect
BOOL func_ov112_021f7a80(StaActPokeSys *sys, StaActPoke *poke) {
    if (poke->itemFunc != NULL) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's StaActPoke_ItemSpinInit
static void func_ov112_021f7a94(StaActPokeSys *sys, StaActPoke *poke) {
}

// pokebw2's StaActPoke_ItemSpin
static void func_ov112_021f7a98(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    poke->equips[work->pos]->unk2 += 0x1000;
    work->timer++;
    if (work->timer >= 192) {
        poke->itemFunc = NULL;
        sys->effectCount--;
        if (sys->effectCount == 0) {
            GFL_SndPlayerStop(2);
        }
    }
}

// pokebw2's StaActPoke_ItemParticleInit
static void func_ov112_021f7aec(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    VecFx32 pos;
    StaActEffectSys *effectSys = func_ov132_021f9dc0(sys->stage);
    MusPokeDrawEquipPos *equip = func_ov112_021f5450(poke->draw, poke->itemWork.pos);

    work->effect = func_ov132_021f9edc(effectSys, 53);
    if (poke->isFront == TRUE && equip->valid == TRUE) {
        func_ov112_021f4ea4(sys->itemDraw, poke->items[work->pos], &pos);
        pos.x = pos.x / 16;
        pos.y = (FX32_CONST(192) - pos.y) / 16;
        pos.z = pos.z + FX32_ONE;
    } else {
        VEC_Add(&poke->pos, func_ov112_021f5464(poke->draw), &pos);
        pos.x = pos.x / 16;
        pos.y = (FX32_CONST(192) - pos.y) / 16;
        pos.z = pos.z - FX32_CONST(28);
    }
    func_ov132_021f9f74(work->effect, 0, &pos);
}

// pokebw2's StaActPoke_ItemParticle
static void func_ov112_021f7ba8(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    VecFx32 pos;
    MusPokeDrawEquipPos *equip = func_ov112_021f5450(poke->draw, poke->itemWork.pos);

    if (poke->isFront == TRUE && equip->valid == TRUE) {
        func_ov112_021f4ea4(sys->itemDraw, poke->items[work->pos], &pos);
        pos.x = pos.x / 16;
        pos.y = (FX32_CONST(192) - pos.y) / 16;
        pos.z = pos.z + FX32_ONE;
    } else {
        VEC_Add(&poke->pos, func_ov112_021f5464(poke->draw), &pos);
        pos.x = pos.x / 16;
        pos.y = (FX32_CONST(192) - pos.y) / 16;
        pos.z = pos.z - FX32_CONST(28);
    }
    func_ov132_021f9fd0(work->effect, 0, &pos);
    work->timer++;
    if (work->timer >= STA_ACT_POKE_ITEM_FRAMES) {
        StaActEffectSys *effectSys = func_ov132_021f9dc0(sys->stage);

        func_ov132_021f9fb4(work->effect, 0);
        func_ov132_021f9f58(effectSys, work->effect);
        poke->itemFunc = NULL;
        sys->effectCount--;
        if (sys->effectCount == 0) {
            GFL_SndPlayerStop(2);
        }
    }
}

// pokebw2's StaActPoke_ItemFallInit
static void func_ov112_021f7c90(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    MusPokeDrawEquipPos *equip = func_ov112_021f5450(poke->draw, poke->itemWork.pos);

    poke->showItem[poke->itemWork.pos] = FALSE;
    if (poke->isFront == FALSE || equip->valid == FALSE) {
        VecFx32 pos;

        VEC_Add(&poke->pos, func_ov112_021f5464(poke->draw), &pos);
        pos.z -= FX32_CONST(28);
        func_ov112_021f4e8c(sys->itemDraw, poke->items[work->pos], &pos);
    }
    func_ov112_021f4e6c(sys->itemDraw, poke->items[work->pos], TRUE);
    func_ov112_021f4ed4(sys->itemDraw, poke->items[work->pos], 0);
    func_ov112_021f4eb4(sys->itemDraw, poke->items[work->pos], FX32_ONE / 4, FX32_ONE / 4);
}

// pokebw2's StaActPoke_ItemFall
static void func_ov112_021f7d30(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    u16 timer = work->timer;
    fx16 sin = FX_SinIdx((timer * 0x200) % 0x10000);
    VecFx32 pos;

    func_ov112_021f4ea4(sys->itemDraw, poke->items[work->pos], &pos);
    pos.x += FX_Mul(timer / 3 * 205, sin);
    pos.y -= FX32_CONST(0.75);
    func_ov112_021f4e8c(sys->itemDraw, poke->items[work->pos], &pos);
    func_ov112_021f4ed4(sys->itemDraw, poke->items[work->pos], sin >> 1);
    work->timer++;
    if (work->timer >= STA_ACT_POKE_ITEM_FRAMES) {
        func_ov112_021f4e6c(sys->itemDraw, poke->items[work->pos], FALSE);
        poke->itemFunc = NULL;
        sys->effectCount--;
        if (sys->effectCount == 0) {
            GFL_SndPlayerStop(2);
        }
    }
}

// pokebw2's StaActPoke_ItemThrowInit
static void func_ov112_021f7e2c(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    MusPokeDrawEquipPos *equip = func_ov112_021f5450(poke->draw, poke->itemWork.pos);

    poke->showItem[poke->itemWork.pos] = FALSE;
    if (poke->isFront == FALSE || equip->valid == FALSE) {
        VecFx32 pos;

        VEC_Add(&poke->pos, func_ov112_021f5464(poke->draw), &pos);
        func_ov112_021f4e8c(sys->itemDraw, poke->items[work->pos], &pos);
    }
    work->speed = FX32_CONST(-3);
    func_ov112_021f4e6c(sys->itemDraw, poke->items[work->pos], TRUE);
    func_ov112_021f4ed4(sys->itemDraw, poke->items[work->pos], 0);
    func_ov112_021f4eb4(sys->itemDraw, poke->items[work->pos], FX32_ONE / 4, FX32_ONE / 4);
}

// pokebw2's StaActPoke_ItemThrow
static void func_ov112_021f7ecc(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    VecFx32 pos;
    s16 scaleX;
    s16 scaleY;

    func_ov112_021f4ea4(sys->itemDraw, poke->items[work->pos], &pos);
    work->speed += 0xa4;
    pos.x -= 0xb33;
    pos.y += work->speed;
    pos.z = FX32_CONST(200);
    func_ov112_021f4e8c(sys->itemDraw, poke->items[work->pos], &pos);
    func_ov112_021f4ec4(sys->itemDraw, poke->items[work->pos], &scaleX, &scaleY);
    scaleX += 12;
    scaleY += 12;
    func_ov112_021f4eb4(sys->itemDraw, poke->items[work->pos], scaleX, scaleY);
    work->timer++;
    if (work->timer >= STA_ACT_POKE_ITEM_FRAMES) {
        func_ov112_021f4e6c(sys->itemDraw, poke->items[work->pos], FALSE);
        poke->itemFunc = NULL;
        sys->effectCount--;
        if (sys->effectCount == 0) {
            GFL_SndPlayerStop(2);
        }
    }
}

// pokebw2's StaActPoke_ItemPulseInit
static void func_ov112_021f7fa8(StaActPokeSys *sys, StaActPoke *poke) {
}

// pokebw2's StaActPoke_ItemPulse
static void func_ov112_021f7fac(StaActPokeSys *sys, StaActPoke *poke) {
    StaActPokeItemWork *work = &poke->itemWork;
    u16 idx = (work->timer * 0x800) % 0x10000;

    work->timer++;
    if (work->timer >= STA_ACT_POKE_ITEM_FRAMES) {
        poke->itemFunc = NULL;
        sys->effectCount--;
        if (sys->effectCount == 0) {
            GFL_SndPlayerStop(2);
        }
    } else {
        func_ov112_021f4eb4(sys->itemDraw, poke->items[work->pos], FX_SinIdx(idx) / 8 + FX32_ONE / 4,
                            FX_CosIdx(idx) / 8 + FX32_ONE / 4);
    }
}

// pokebw2's StaActPoke_GetPosition
void func_ov112_021f8044(StaActPokeSys *sys, StaActPoke *poke, VecFx32 *pos) {
    pos->x = poke->pos.x;
    pos->y = poke->pos.y;
    pos->z = poke->pos.z;
}

// pokebw2's StaActPoke_SetPosition
void func_ov112_021f8054(StaActPokeSys *sys, StaActPoke *poke, VecFx32 *pos) {
    poke->pos.x = pos->x;
    poke->pos.y = pos->y;
    poke->pos.z = pos->z;
    poke->update = TRUE;
}

// pokebw2's StaActPoke_SetRotate
void func_ov112_021f8068(StaActPokeSys *sys, StaActPoke *poke, u16 rotation) {
    poke->rotation = rotation;
    if (poke->noBounce == TRUE) {
        poke->rotation = 0;
    }
    poke->update = TRUE;
}

// pokebw2's StaActPoke_SetPositionOffset
void func_ov112_021f8084(StaActPokeSys *sys, StaActPoke *poke, VecFx32 *offset) {
    poke->offset.x = offset->x;
    poke->offset.y = offset->y;
    poke->offset.z = offset->z;
    if (poke->noBounce == TRUE) {
        poke->offset.y = 0;
    }
    poke->update = TRUE;
}

// pokebw2's StaActPoke_StartAnime
void func_ov112_021f80a0(StaActPokeSys *sys, StaActPoke *poke) {
    func_ov112_021f539c(poke->draw);
    poke->animating = TRUE;
}

// pokebw2's StaActPoke_StopAnime
void func_ov112_021f80b0(StaActPokeSys *sys, StaActPoke *poke) {
    func_ov112_021f53b4(poke->draw);
    poke->animating = FALSE;
    func_ov112_021f5424(poke->draw, FALSE);
}

// pokebw2's StaActPoke_ChangeAnime
void func_ov112_021f80c8(StaActPokeSys *sys, StaActPoke *poke, u8 anime) {
    func_ov112_021f53cc(poke->draw, anime);
}

// pokebw2's StaActPoke_SetShowFlg
void func_ov112_021f80d4(StaActPokeSys *sys, StaActPoke *poke, const BOOL show) {
    int i;

    func_ov112_021f5374(poke->draw, show);
    for (i = 0; i < 9; i++) {
        if (poke->items[i] != NULL && poke->showItem[i] == TRUE) {
            func_ov112_021f4e6c(sys->itemDraw, poke->items[i], show);
        }
    }
    BlActScene_SetActorHidden(sys->blact, poke->shadowActor, &show);
}

// pokebw2's StaActPoke_GetShowFlg
BOOL func_ov112_021f8120(StaActPokeSys *sys, StaActPoke *poke) {
    return func_ov112_021f5388(poke->draw);
}

// pokebw2's StaActPoke_SetFlip
void func_ov112_021f812c(StaActPokeSys *sys, StaActPoke *poke, BOOL flip) {
    if (poke->noFlip == TRUE) {
        poke->flip = FALSE;
    } else {
        poke->flip = flip;
    }
    poke->update = TRUE;
}

// pokebw2's StaActPoke_SetFront
void func_ov112_021f8140(StaActPokeSys *sys, StaActPoke *poke, BOOL front) {
    poke->isFront = front;
    func_ov112_021f5414(poke->draw, front);
    poke->update = TRUE;
}

// pokebw2's StaActPoke_SetShowItem
void func_ov112_021f8154(StaActPokeSys *sys, StaActPoke *poke, BOOL show) {
    poke->showItems = show;
    poke->update = TRUE;
}
