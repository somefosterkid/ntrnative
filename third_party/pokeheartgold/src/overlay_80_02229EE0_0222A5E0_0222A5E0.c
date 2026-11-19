#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

typedef struct FrontierPokemonBase {
    u16 species;
    u16 moves[4];
    u8 evFlags;
    u8 nature;
    u16 item;
    u16 form;
} FrontierPokemonBase;

typedef struct FrontierTrainerBase {
    u16 trainerType;
    u16 numSets;
    u16 setIDs[];
} FrontierTrainerBase;

void ov80_02229EF4(FrontierPokemonBase *monData, int narcIdx, enum NarcId narcID);

u16 LCRandom(void);

extern BOOL ov80_0222A5E0(FrontierTrainerBase *trainer, const u16 partnersSpecies[], const u16 partnersItems[], int partnersNumMons, int numMons, u16 selectedMons[], enum HeapID heapID);

BOOL ov80_0222A5E0(FrontierTrainerBase *trainer, const u16 partnersSpecies[], const u16 partnersItems[], int partnersNumMons, int numMons, u16 selectedMons[], enum HeapID heapID) {
    ((numMons <= 6) ? (void)0 : GF_AssertFail());

    FrontierPokemonBase newMon[6];
    int narcIdx;
    int i = 0;
    int partnerConflictCounter = 0;

    while (i != numMons) {
        narcIdx = trainer->setIDs[LCRandom() % trainer->numSets];
        ov80_02229EF4(&newMon[i], narcIdx, (enum NarcId)205);

        int j;
        for (j = 0; j < i; j++) {
            if (newMon[j].species == newMon[i].species || newMon[j].item == newMon[i].item) {
                break;
            }
        }

        if (j != i) {
            continue;
        }

        if (partnerConflictCounter < 50) {
            for (j = 0; j < partnersNumMons; j++) {
                if (newMon[i].species == partnersSpecies[j] || newMon[i].item == partnersItems[j]) {
                    break;
                }
            }

            if (j != partnersNumMons) {
                partnerConflictCounter++;
                continue;
            }
        }

        selectedMons[i] = narcIdx;
        i++;
    }

    if (partnerConflictCounter >= 50) {
        return 1;
    }

    return 0;
}
