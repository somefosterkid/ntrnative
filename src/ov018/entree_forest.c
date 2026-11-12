// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/entree_forest.c, ported by tools/port_file.py.
#include "types.h"
#include "gfl/arc_util.h"
#include "system/game_data.h"
#include "save/player_info.h"
#include "pml/poke_party.h"
#include "gfl/heap.h"
#include "gfl/arc.h"
#include "field/entree_forest.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/symbol_map.h"
#include "field/zone.h"
#include "gfl/random.h"
#include "system/tpoke_data.h"
#include "struct_decls.h"

typedef struct {
    u16 moveCode;
    u8 areaW;
    u8 areaH;
} EntreeForestActorAppearance;

// Where the Pokemon of the forest stand, an x and a z for each of the first ten
const u8 data_ov018_021835f8[] = {
    13, 20, 18, 13, 18, 21, 13, 12, 9, 19, 22, 14, 22, 19, 9, 14, 18, 18, 13, 15,
};

const EntreeForestActorAppearance data_ov018_0218360c[] = {
    { 2, 0, 0 }, { 3, 1, 1 }, { 3, 2, 2 }, { 4, 0, 2 }, { 5, 2, 0 }, { 3, 2, 0 }, { 19, 0, 0 }, { 18, 0, 0 },
};

const ZoneNPC data_ov018_0218362c = {
    0, 0x1000, 15, 0, 0, 0x28f0, 1, 0, 0, 0, 0xffff, 0xffff, FALSE, { { 0, 0, 0 } },
};

const u8 data_ov018_02183650[] = {
#include "asset/entree_forest_data_ov018_02183650.inc"
};

const u8 data_ov018_02183678[] = {
#include "asset/entree_forest_data_ov018_02183678.inc"
};


FieldActor *EntreeForest_SpawnPkmActor(const EntreeForestSpawnContext *context, const EntreeForestPokemon *pokemon,
                                       u16 index, u16 x, u16 z, s32 y) {
    const EntreeForestActorAppearance *appearance;
    MMSys *actorSystem;
    u16 zoneId;
    ZoneNPC npc;
    ZoneNPCGridPosition *grid;

    appearance = &data_ov018_0218360c[pokemon->actorType];
    actorSystem = Field_GetActorSystem(context->field);
    zoneId = Field_GetPlayerStateZoneID(context->field);
    npc = data_ov018_0218362c;
    npc.uid = index;
    npc.modelId = GetPokemonFieldOBJCODE(context->pokemonData, pokemon->species, pokemon->sex, pokemon->form);
    npc.moveCode = appearance->moveCode;
    npc.areaW = appearance->areaW;
    npc.areaH = appearance->areaH;
    npc.param0 = context->actorIdBase + index;
    npc.param1 = pokemon->raw;
    npc.param2 = pokemon->raw >> 16;
    grid = &npc.pos.grid;
    grid->x = x;
    grid->z = z;
    grid->y = y;
    return CreateNewActorByEntityNoWKOBJCODE(actorSystem, &npc, zoneId);
}

void GetEntreeForestActorParamBits(u32 *paramBits, FieldActor *actor) {
    u32 low = GetActorUserParam(actor, 1);
    u32 high = GetActorUserParam(actor, 2);

    *paramBits = low | (high << 16);
}

u16 GetActorUserParam0(FieldActor *actor) {
    return GetActorUserParam(actor, 0);
}

PartyPkm *func_ov018_0217c2a0(HeapID heapId, GameData *gameData, const EntreeForestPokemon *pokemon) {
    u8 *levels;
    u8 level;
    u32 id;
    u32 pid;
    PartyPkm *pkm;

    levels = GFL_ArcSysReadHeapNewLZ(0xe3, 0, 0, heapId);
    level = levels[pokemon->species];
    GFL_HeapFree(levels);
    id = getIDAsUInt(GetGameDataPlayerInfo(gameData));
    pid = PML_GenPID(id, pokemon->species, pokemon->form, pokemon->sex, 0, 0);
    pkm = PokeParty_NewTempPkm(pokemon->species, level, -1, heapId);
    PokeParty_CreatePkm(pkm, pokemon->species, level, id, PKM_IVS_RANDOM, pid);
    PokeParty_SetHiddenAbil(pkm, pokemon->species, pokemon->form);
    PokeParty_ChangeForme(pkm, pokemon->form);
    if (pokemon->move != 0) {
        if (PokeParty_LearnMove(pkm, pokemon->move) == 0xffff) {
            PokeParty_SetLastMove(pkm, pokemon->move);
        }
    }
    return pkm;
}

void EntreeForest_SpawnAllPokemon(Field *field, u32 actorIdBase, const EntreeForestPokemon *pokemon, u32 unused, u32 area) {
    const u8 *positions;
    EntreeForestSpawnContext context;
    FieldActor *playerActor;
    u16 playerX;
    u16 playerZ;
    s32 i;
    s32 offsetX;
    s32 offsetZ;
    s32 x;
    s32 z;

    if (func_ov010_02166618(area)) {
        positions = data_ov018_02183678;
    } else if (func_ov010_02166640(area)) {
        positions = data_ov018_021835f8;
    } else {
        positions = data_ov018_02183650;
    }
    context.pokemonData = LoadTPokeData(Field_GetHeapID(field));
    context.field = field;
    context.actorIdBase = actorIdBase;
    playerActor = FieldPlayer_GetActor(Field_GetPlayer(field));
    playerX = GetGPosX(playerActor);
    playerZ = GetGPosZ(playerActor);
    for (i = 0; i < 20; i++) {
        if (pokemon[i].species != 0) {
            offsetX = GFL_RandomLCAlt(2) - 1;
            offsetZ = GFL_RandomLCAlt(2) - 1;
            x = positions[i * 2] + offsetX;
            z = positions[i * 2 + 1] + offsetZ;
            if (playerX == x && playerZ == z) {
                if (offsetX != 0) {
                    x -= offsetX;
                } else if (offsetZ != 0) {
                    z -= offsetZ;
                } else {
                    x++;
                }
            }
            EntreeForest_SpawnPkmActor(&context, &pokemon[i], i, x, z, 0);
        }
    }
    FreeTPokeData(context.pokemonData);
}

// The function of the Entree Forest that makes an actor of a Pokemon takes these two tables: it is in the text before
// this file's range. This stands in for it, so that the tables are sorted with the others
static void EntreeForest_UseActorTables(const EntreeForestActorAppearance **appearance, const ZoneNPC **npc) {
    *appearance = data_ov018_0218360c;
    *npc = &data_ov018_0218362c;
}
