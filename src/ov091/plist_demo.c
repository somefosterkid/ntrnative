// Black's code at ov091 0x021bd8b0-0x021bdd78. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "app/pokelist.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/particle.h"
#include "gfl/str.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "nitro/rtc.h"
#include "constants/pokemon.h"
#include "system/rtc.h"
#include "gfl/rtc_cache.h"
#include "constants/species.h"
#include "gfl/sound.h"
#include "system/wordset.h"
#include "gfl/bmp.h"
#include "work_area.h"

void func_ov091_021bdabc(PokeListWork *wk);
void func_ov091_021bdbe0(PokeListWork *wk);

void func_ov091_021bd8b0(PokeListWork *wk) {
    wk->demoTimer = 0;
    wk->demoSe = 0;
    wk->voice = 0;
    GFL_BGSysSetBGEnabled(0, 0);
    func_ov091_021b7034(wk);
    func_ov091_021b6f28(wk);
    func_ov091_021bdabc(wk);
    wk->state = 0xf;
}

void func_ov091_021bd8e4(PokeListWork *wk) {
    u32 demo;
    void (*doneFunc)(PokeListWork *wk);

    func_ov091_021bdbe0(wk);
    func_ov091_021b7040(wk);
    func_ov091_021b6f0c(wk);
    func_ov091_021bba54(wk, wk->message);
    demo = wk->demo;
    if (demo == 1) {
        doneFunc = func_ov091_021b9e3c;
        if (wk->param->mode == 0) {
            doneFunc = func_ov091_021b9e20;
        }
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021b9788(wk, 0xa6, 1, doneFunc);
        func_ov091_021bb98c(wk, wk->message);
        return;
    }
    if (demo == 2) {
        doneFunc = func_ov091_021b9e3c;
        if (wk->param->mode == 0) {
            doneFunc = func_ov091_021b9e20;
        }
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021b9788(wk, 0xa6, 1, doneFunc);
        func_ov091_021bb98c(wk, wk->message);
        return;
    }
    if (demo == 3) {
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021b9788(wk, 0xa6, 1, (void *)func_ov091_021b9e3c);
        func_ov091_021bb98c(wk, wk->message);
        return;
    }
    wk->state = 0x13;
}

void func_ov091_021bd9c8(PokeListWork *wk) {
    u32 species;
    u32 form;

    if (wk->demoSe == 0) {
        wk->demoTimer++;
        if (wk->demoTimer == 1) {
            GFL_BGSysSetBGEnabled(0, 1);
            if (wk->demo == 1 || wk->demo == 2) {
                GFL_SndSEPlay(0x643);
            } else if (wk->demo == 3) {
                GFL_SndSEPlay(0x644);
            }
        }
        if (wk->demoTimer > wk->demoLength) {
            func_ov091_021bb200(wk, wk->plates[wk->cursorPos], wk->pkm, 0);
            wk->demoSe = 1;
        }
    }
    if (func_02051614(wk->particles) <= 0 && !GFL_SndPlayerIsActiveAny()) {
        if (wk->voice == 0) {
            species = PokeParty_GetParam(wk->pkm, PKM_PARAM_SPECIES, NULL);
            form = PokeParty_GetParam(wk->pkm, PKM_PARAM_FORM, NULL);
            wk->unk274 = PokeVoice_Play(species, form, 0x40, 0, 0, 0, 0, NULL);
            wk->voice = 1;
        } else if (!PokeVoice_IsPlaying(wk->unk274)) {
            wk->state = 0x10;
        }
    }
    func_02051574();
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    func_02051534();
    GFL_G3DSysReqSwapBuffers();
}

void func_ov091_021bdabc(PokeListWork *wk) {
    VecFx32 vec;
    ClActorPos pos;
    void *resource;
    void *work;
    u32 fileId;
    u8 count;
    u8 i;

    count = 0;
    fileId = 0x1c;
    if (wk->demo == 1 || wk->demo == 2) {
        fileId = 0x1b;
        count = 3;
        wk->demoLength = 0x41;
    } else if (wk->demo == 3) {
        count = 2;
        wk->demoLength = 0x26;
    }
    resource = func_0205121c(0x4c, fileId, wk->heapId);
    // the SPL manager's work, whose emitters and particles hold pointers
    work = allocConfigDSSoftwareFeature(wk->heapId, WORK_AREA_SIZE(0x5000), "plist_demo.c", 0xea);
    wk->particleHeap = work;
    wk->particles = func_02050c04(work, WORK_AREA_SIZE(0x5000), FALSE, wk->heapId);
    func_02051230(wk->particles, resource, FALSE, GFL_VBlankGetTCBMgr());
    func_ov091_021bb3d4(wk, wk->plates[wk->cursorPos], &pos);
    pos.x -= 40;
    pos.y += 4;
    vec.x = FX32_CONST(pos.x) / 38;
    vec.y = FX32_CONST(pos.y) / 38;
    vec.z = 64 * FX32_ONE;
    for (i = 0; i < count; i++) {
        func_020515b4(wk->particles, i, &vec);
    }
}



void func_ov091_021bdbe0(PokeListWork *wk) {
    func_02050db8(wk->particles);
    func_0203d7fc(wk->particleHeap);
}

BOOL func_ov091_021bdbf8(PokeListWork *wk, PartyPkm *pkm) {
    u32 result;

    result = PokeParty_GetParam(wk->pkm, 5, (void *)0x0);
    if (result != 0x1e7) {
        return 0;
    }
    result = PokeParty_GetParam(wk->pkm, 6, (void *)0x0);
    if (result != 0x70) {
        return 0;
    }
    result = PokeParty_GetParam(wk->pkm, 0x6f, (void *)0x0);
    return (u32)(result == 0);
}

void func_ov091_021bdc3c(PokeListWork *wk, PartyPkm *pkm) {
    PokeParty_ChangeForme(pkm, 1);
    addPkmToDex(wk->param->pokedex, pkm);
}

BOOL func_ov091_021bdc5c(PokeListWork *wk, PartyPkm *pkm) {
    u32 result;

    result = PokeParty_GetParam(wk->pkm, 6, (void *)0x0);
    if (result == 0x70) {
        return 0;
    }
    result = PokeParty_GetParam(wk->pkm, 5, (void *)0x0);
    if (result != 0x1e7) {
        return 0;
    }
    result = PokeParty_GetParam(wk->pkm, 0x6f, (void *)0x0);
    return (u32)(result == 1);
}

void func_ov091_021bdca0(PokeListWork *wk, PartyPkm *pkm) {
    PokeParty_ChangeForme(pkm, 0);
    addPkmToDex(wk->param->pokedex, pkm);
}

BOOL func_ov091_021bdcc0(PokeListWork *wk, PartyPkm *pkm) {
    u8 start;
    u8 end;
    RTCTime time;

    if (PokeParty_GetParam(wk->pkm, PKM_PARAM_SPECIES, NULL) != SPECIES_SHAYMIN) {
        return FALSE;
    }
    if (PokeParty_GetParam(wk->pkm, PKM_PARAM_FORM, NULL) != 0) {
        return FALSE;
    }
    if (PokeParty_GetParam(wk->pkm, PKM_PARAM_FATEFUL_ENCOUNTER, NULL) != 1) {
        return FALSE;
    }
    if (GetStatusCond(wk->pkm) & 0x20) {
        return FALSE;
    }
    start = GetLightChangeHoursForSeasons(wk->param->unk5F, 0);
    end = GetLightChangeHoursForSeasons(wk->param->unk5F, 3);
    RTC_GetCachedTime(&time);
    if (time.hour < start || time.hour >= end) {
        return FALSE;
    }
    return TRUE;
}


void func_ov091_021bdd58(PokeListWork *wk, PartyPkm *pkm) {
    PokeParty_ChangeForme(pkm, 1);
    addPkmToDex(wk->param->pokedex, pkm);
}
