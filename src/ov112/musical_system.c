// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov210/musical_system.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/musical_system.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "pml/poke_party.h"
#include "save/save_control.h"

// Overlay 210's musical_system.c, named after its string: the musical's Pokémon and the program's data

// The archives of the four built-in programs, one each (our name)
#define ARCID_MUSICAL_PROGRAM 111
// The first program that is the downloaded one, read from the save data
#define MUSICAL_PROGRAM_DOWNLOAD 4

#define MUSICAL_ITEM_NONE 0xff

MusicalPoke *func_ov112_021f675c(PartyPkm *pkm, HeapID heapId) {
    MusicalPoke *poke;
    s32 i;

    poke = GFL_HeapAllocate(heapId, sizeof(MusicalPoke), TRUE, "musical_system.c", 141);
    poke->owner = 4;
    poke->pkm = pkm;
    poke->points = 0;
    for (i = 0; i < 9; i++) {
        poke->equips[i].itemId = MUSICAL_ITEM_NONE;
        poke->equips[i].unk2 = 0;
        poke->unk54[i] = 0;
    }
    poke->species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    poke->sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
    poke->form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    poke->rare = PokeParty_IsRare(pkm);
    poke->personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    return poke;
}

MusicalPoke *func_ov112_021f67e0(u16 species, u8 sex, u8 form, u8 rare, u32 personality, HeapID heapId) {
    s32 i;
    MusicalPoke *poke;
    s32 j;

    poke = GFL_HeapAllocate(heapId, sizeof(MusicalPoke), TRUE, "musical_system.c", 165);
    poke->owner = 4;
    poke->pkm = NULL;
    poke->points = 0;
    for (i = 0; i < 9; i++) {
        poke->equips[i].itemId = MUSICAL_ITEM_NONE;
        poke->equips[i].unk2 = 0;
        poke->unk54[i] = 0;
    }
    for (j = 0; j < 4; j++) {
        poke->unk4C[j] = 0;
    }
    poke->unk78 = 0;
    poke->species = species;
    poke->sex = sex;
    poke->form = form;
    poke->rare = rare;
    poke->personality = personality;
    return poke;
}

Ov210Work *func_ov112_021f685c(HeapID heapId) {
    Ov210Work *work = GFL_HeapAllocate(heapId, sizeof(Ov210Work), FALSE, "musical_system.c", 194);

    work->unk4 = NULL;
    work->msgArc = NULL;
    work->script = NULL;
    work->soundData = NULL;
    work->sound[0] = NULL;
    work->sound[1] = NULL;
    work->sound[2] = NULL;
    return work;
}

// pokebw2's MusicalSystem_FreeProgramData
void func_ov112_021f6888(Ov210Work *work) {
    if (work->unk4 != NULL) {
        GFL_HeapFree(work->unk4);
    }
    if (work->msgArc != NULL) {
        GFL_HeapFree(work->msgArc);
    }
    if (work->script != NULL) {
        GFL_HeapFree(work->script);
    }
    if (work->soundData != NULL) {
        GFL_HeapFree(work->soundData);
    }
    GFL_HeapFree(work);
}

void func_ov112_021f68bc(Ov210Work *work, SaveControl *save, GameData *gameData, u8 program, HeapID heapId) {
    void *download;
    ArcTool *arc;
    u32 size0;
    u32 size1;
    u32 size2;
    u32 *soundData;

    work->program = program;
    if (program >= MUSICAL_PROGRAM_DOWNLOAD) {
        download = func_0200c52c(gameData, heapId);
        arc = GFL_ArcSysCreateMemoryHandle(func_0200c6d8(download), func_0200c6dc(download), HEAPID_TAIL(heapId));
    } else {
        arc = GFL_ArcSysCreateFileHandle(ARCID_MUSICAL_PROGRAM + program, HEAPID_TAIL(heapId));
    }

    work->unk4 = GFL_ArcToolReadHeapNewLZGetLen(arc, 0, FALSE, heapId, &work->size[0]);
    work->msgArc = GFL_ArcToolReadHeapNewLZGetLen(arc, 1, FALSE, heapId, &work->size[1]);
    work->script = GFL_ArcToolReadHeapNewLZGetLen(arc, 2, FALSE, heapId, &work->size[2]);

    size0 = GFL_ArcToolGetDataLength(arc, 4);
    size1 = GFL_ArcToolGetDataLength(arc, 3);
    size2 = GFL_ArcToolGetDataLength(arc, 5);
    work->soundDataSize = size0 + 12 + size1 + size2;
    soundData = GFL_HeapAllocate(heapId, work->soundDataSize, TRUE, "musical_system.c", 260);
    work->soundData = soundData;
    work->sound[0] = soundData + 3;
    work->sound[1] = (u8 *)work->sound[0] + size0;
    work->sound[2] = (void *)(size1 + (uintptr_t)work->sound[1]);
    soundData[0] = size0;
    soundData[1] = size1;
    soundData[2] = size2;
    GFL_ArcToolRead(arc, 4, work->sound[0]);
    GFL_ArcToolRead(arc, 3, work->sound[1]);
    GFL_ArcToolRead(arc, 5, work->sound[2]);
    GFL_ArcToolFree(arc);

    if (program >= MUSICAL_PROGRAM_DOWNLOAD) {
        func_0200c5a4(download);
    }
}


