#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "menu_input_state.h"
#include "overlay_18.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02013FDC.h"
#include "unk_02092BB8.h"
#include "wild_encounter.h"
#include "application/pokedex/pokedex_internal.h"

struct Background;

struct BgConfig;

struct MenuInputStateMgr;

struct PlayerProfile;

struct Pokedex;

struct PokedexAppData;

struct PokedexArgs;

struct PokepicTemplate;

struct UnkStruct_02014E30;

struct UnkStruct_02092BB8;

extern const UnkStruct_02014E30 ov18_021FA338;

extern const u8 ov18_021FB5B4[];

void ov18_021F69E8(PokedexAppData *pokedexApp, u16 species, u8 form, u8 gender, u8 facing);

void ov18_021F69E8(PokedexAppData *pokedexApp, u16 species, u8 form, u8 gender, u8 facing) {
    PokepicTemplate template;
    UnkStruct_02014E30 rect;
    void *buf;
    u8 *converted;
    u32 personality;

    rect = ov18_021FA338;
    buf = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, 0xC80);
    if (species == 327) {
        personality = Pokedex_GetSeenSpindaPersonality(pokedexApp->args->pokedex, 0);
    } else {
        personality = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, species, gender, facing, 0, form, personality);
    sub_02014510((NarcId)(template.narcID), template.charDataID, HEAP_ID_POKEDEX_APP, &rect, buf, personality, 0, 2, species);
    converted = Convert4bppTo8bpp(buf, 0xC80, 0xF, HEAP_ID_POKEDEX_APP);
    BG_LoadCharTilesData(pokedexApp->bgConfig, 7, converted, 0x1900, 1);
    Heap_Free(converted);
    Heap_Free(buf);
    LoadRectToBgTilemapRect(pokedexApp->bgConfig, 7, ov18_021FB5B4, 0xB, 0xB, 0xA, 0xA);
    ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, 7);
}
