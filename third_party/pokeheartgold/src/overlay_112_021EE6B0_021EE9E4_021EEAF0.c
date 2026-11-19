#include "global.h"

#include "heap.h"
#include "overlay_112_pokewalker_app.h"
#include "math_util.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save_arrays.h"
#include "trainer_memo.h"

struct BoxPokemon;

struct CAPSULE;

struct Mail;

struct MailMessage;

struct PC_Box;

struct POKEWALKER;

struct PartyPokemon;

struct PlayerProfile;

struct Pokedex;

struct PokemonStorageSystem;

struct SEAL;

struct SaveData;

struct String;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

extern u64 _u32_div_f(u32 a, u32 b);

extern void ov112_021EE9A4(Pokemon *mon, void *a1, void *a2, u32 a3, int a4);

extern void ov112_021EE9E4(Pokemon *mon, u32 a1, void *a2, u32 a3, int a4);

void ov112_021EEAF0(Ov112PokewalkerApp *work, int useWalkerBox);

void ov112_021EEAF0(Ov112PokewalkerApp *work, int useWalkerBox) {
    PCStorage *storage;
    Pokemon *mon;
    Ov112PokewalkerMonRec *base;
    u16 walkerA;
    BoxPokemon *boxMon;
    u16 walkerB;
    u8 flag;
    u8 one;
    int boxno;
    int slotno;
    String *playerName;
    int i;
    Ov112PokewalkerMonRec *entry;
    String *str;
    u32 rem;

    storage = SaveArray_PCStorage_Get(work->saveData);
    mon = AllocMonZeroed((enum HeapID)0x9A);
    playerName = PlayerProfile_GetPlayerName_NewString(work->profile, (enum HeapID)0x9A);
    sub_02032688(work->pokewalker, &walkerA, &walkerB);
    if (useWalkerBox) {
        boxno = walkerB;
    } else {
        boxno = 0;
    }
    slotno = 0;
    entry = work->unk_9D7C;
    for (i = 0; i < 3; i++, entry++) {
        if (entry->unk_0 != 0) {
            rem = MTRandom() % 24;
            ov112_021EE9A4(mon, work->profile, entry, rem, 0);
            BoxMonSetTrainerMemo(Mon_GetBoxMon(mon), work->profile, 0, 0xE9, (enum HeapID)0x9A);
            boxMon = Mon_GetBoxMon(mon);
            PCStorage_FindFirstEmptySlot(storage, &boxno, &slotno);
            PCStorage_PlaceMonInBoxByIndexPair(storage, boxno, slotno, boxMon);
            Pokedex_SetMonSeenFlag(work->pokedex, mon);
            Pokedex_SetMonCaughtFlag(work->pokedex, mon);
        }
    }
    if ((u32)(work->unk_AABC << 26) >> 31) {
        str = String_New(0x10, (enum HeapID)0x9A);
        base = &work->unk_AD00;
        if (base->unk_0 != 0) {
            CopyU16ArrayToString(str, work->unk_AD1E);
            ZeroMonData(mon);
            boxMon = Mon_GetBoxMon(mon);
            rem = MTRandom() % 24;
            ov112_021EE9E4(mon, work->unk_AD14, base, rem, ((u32)(base->unk_E[0] << 30) >> 31));
            flag = (u32)(work->unk_AD2E << 31) >> 31;
            SetBoxMonData(boxMon, 0x9D, &flag);
            SetBoxMonData(boxMon, 0x91, str);
            BoxMonSetTrainerMemo(Mon_GetBoxMon(mon), work->profile, 4, work->unk_AD1A, (enum HeapID)0x9A);
            SetBoxMonData(boxMon, 10, work->unk_AD2F);
            one = 1;
            SetBoxMonData(boxMon, 0x6E, &one);
            SetBoxMonData(boxMon, 0x9B, work->unk_AD30);
            PCStorage_FindFirstEmptySlot(storage, &boxno, &slotno);
            PCStorage_PlaceMonInBoxByIndexPair(storage, boxno, slotno, boxMon);
            Pokedex_SetMonSeenFlag(work->pokedex, mon);
            Pokedex_SetMonCaughtFlag(work->pokedex, mon);
            if (work->unk_1E430 == NULL) {
                work->unk_1E430 = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
            }
        }
        String_Delete(str);
    }
    String_Delete(playerName);
    Heap_Free(mon);
}
