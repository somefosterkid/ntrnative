#include "global.h"

#include "overlay_112_pokewalker_app.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "string_util.h"

struct BoxPokemon;

struct CAPSULE;

struct Mail;

struct MailMessage;

struct PC_Box;

struct Party;

struct PartyPokemon;

struct PokemonStorageSystem;

struct SEAL;

struct SaveData;

struct UnkStruct_ov112_021EF3F8;

struct UnkStruct_ov112_021EF3F8_Box;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct UnkStruct_ov112_021EF3F8_Box {
    u32 species;
    u32 personality;
    u16 unk8;
    u16 level;
    u16 item;
    u16 unkE;
    u16 shiny;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 nickname[12];
} UnkStruct_ov112_021EF3F8_Box;

typedef struct UnkStruct_ov112_021EF3F8 {
    u32 personality;
    u16 species;
    u8 shiny;
    u8 level;
    u8 unk8;
    u8 unk9;
    u16 nickname[11];
    u8 flags[6];
    u16 item;
    u8 unk28;
    u8 pad[3];
} UnkStruct_ov112_021EF3F8;

void ov112_021F04DC(void *arg0, void *arg1);

s32 ov112_021EF1F0(PCStorage *storage, u32 boxno, u32 slotno, void *arg3);

void ov112_021EF3F8(int box, int slot, Ov112PokewalkerApp *work);

void ov112_021EF3F8(int box, int slot, Ov112PokewalkerApp *work) {
    UnkStruct_ov112_021EF3F8 data;
    UnkStruct_ov112_021EF3F8_Box res;
    Ov112PokewalkerLink *sub = &work->link;
    u8 i;
    if ((box != (-1)) && (slot != (-1))) {
        data.unk9 = 2;
        if (box == 0x12) {
            Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(work->saveData), slot);
            int bits;
            data.personality = GetMonData(mon, 0, 0L);
            data.species = GetMonData(mon, 5, 0L);
            data.level = GetMonData(mon, 0x70, 0L);
            GetMonData(mon, 0x75, data.nickname);
            data.shiny = MonIsShiny(mon);
            data.unk8 = GetMonData(mon, 0x6F, 0L);
            bits = GetMonData(mon, 0xB, 0L);
            for (i = 0; i < 6; i++) {
                if ((bits >> i) & 1) {
                    data.flags[i] = 1;
                } else {
                    data.flags[i] = 0;
                }
            }

            data.item = GetMonData(mon, 6, 0L);
            data.unk28 = GetMonData(mon, 0xA1, 0L);
            ov112_021F04DC(sub->subScreen, &data);
            return;
        }
        ov112_021EF1F0(SaveArray_PCStorage_Get(work->saveData), box, slot, &res);
        data.personality = res.personality;
        data.species = res.species;
        data.level = res.level;
        CopyU16StringArrayN(data.nickname, res.nickname, 11);
        data.shiny = res.shiny;
        data.unk8 = res.unk12;
        {
            int bits = res.unk16;
            for (i = 0; i < 6; i++) {
                if ((bits >> i) & 1) {
                    data.flags[i] = 1;
                } else {
                    data.flags[i] = 0;
                }
            }
        }
        data.item = res.item;
        data.unk28 = res.unk14;
        ov112_021F04DC(sub->subScreen, &data);
    } else {
        data.unk9 = 0;
        ov112_021F04DC(sub->subScreen, &data);
    }
}
