#include "global.h"
#include "constants/heap.h"
#include "palette.h"
#include "battle/battle.h"
#include "battle/battle_party_context.h"

#if !(defined(SDK_PORT) || defined(NONMATCHING))
void ov10_0221BE20();
#endif

struct BattleSubscreen;

struct BattleSystem;

struct PaletteAnimator;

struct SysTask;

struct SysTaskQueue;

void CopyWindowPixelsToVram_TextMode(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

void SysTask_Destroy(SysTask *task);

typedef struct BattleSubscreen BattleSubscreen;

int BattleInput_GetKeyPressed(BattleSubscreen *btlSubscreen);

void BattleInput_SetKeyPressed(BattleSubscreen *btlSubscreen, int isCursorEnabled);

Party *SaveArray_Party_Alloc(enum HeapID heapID);

BOOL Party_AddMon(Party *party, Pokemon *mon);

int Party_GetCount(const Party *party);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

Pokemon *AllocMonZeroed(enum HeapID heapID);

void CopyPokemonToPokemon(Pokemon *src, Pokemon *dest);

void BattleController_EmitPartyMenuResult(BattleSystem *battleSys, int battler, int command);

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void Heap_Free(void *ptr);

void ov12_02237B0C(BattleSystem *battleSys);

void ov12_02237BB8(BattleSystem *battleSys);

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

void sub_0201649C(PaletteAnimator *param0, u8 param1);

Window *BattleSystem_GetWindow(BattleSystem *battleSys, int idx);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

Party *BattleSystem_GetParty(BattleSystem *battleSys, int battler);

int BattleSystem_GetPartySize(BattleSystem *battleSys, int battler);

Pokemon *BattleSystem_GetPartyMon(BattleSystem *battleSys, int battler, int slot);

BattleSubscreen *BattleSystem_GetBattleInput(BattleSystem *battleSys);

PaletteData *BattleSystem_GetPaletteData(BattleSystem *battleSys);

u8 ov12_0223AB0C(BattleSystem *battleSys, int battler);

PaletteAnimator *BattleSystem_GetMessageIcon(BattleSystem *battleSys);

int BattleSystem_GetBattlerIdPartner(BattleSystem *battleSys, int battler);

u32 MaskOfFlagNo(int index);

typedef struct PartyMenuData {
    BattleSystem *battleSys;
    BattlePartyContext *battlePartyCtx;
    u8 command;
    u8 battler;
    u8 state;
    u8 listMode;
    u8 partySlots[4];
    int canSwitch;
    u16 selectedBattleBagItem;
    u8 doublesSelection;
    u8 isCursorEnabled;
    u8 battlersSwitchingMask;
    u8 unused[3];
    u8 partyOrder[4][6];
} PartyMenuData;

void ov12_0225F4E0(SysTask *task, void *data);

void ov12_0225F4E0(SysTask *task, void *data) {
    PartyMenuData *partyMenuData = data;
    PaletteData *paletteData = BattleSystem_GetPaletteData(partyMenuData->battleSys);

    switch (partyMenuData->state) {
    case 0:
        Window *window = BattleSystem_GetWindow(partyMenuData->battleSys, 0);

        FillWindowPixelBuffer(window, 0xFF);
        CopyWindowPixelsToVram_TextMode(window);

        partyMenuData->isCursorEnabled = BattleInput_GetKeyPressed(BattleSystem_GetBattleInput(partyMenuData->battleSys));
        sub_0201649C(BattleSystem_GetMessageIcon(partyMenuData->battleSys), 1);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xC00, -8, 0, 7, 0);
        PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, -8, 0, 16, 0);
        partyMenuData->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            int i;
            int battler;
            int battler1, battler2;
            Pokemon *monSrc, *monDst;

            ov12_02237B0C(partyMenuData->battleSys);

            partyMenuData->battlePartyCtx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattlePartyContext));
            partyMenuData->battlePartyCtx->party = SaveArray_Party_Alloc(HEAP_ID_BATTLE);

            if ((BattleSystem_GetBattleType(partyMenuData->battleSys) & ((1 << 2) | (1 << 3))) == ((1 << 2) | (1 << 3))
                || BattleSystem_GetBattleType(partyMenuData->battleSys) == ((((1 << 1) | (1 << 0)) | (1 << 3) | (1 << 6)) | (1 << 7))) {
                if (ov12_0223AB0C(partyMenuData->battleSys, partyMenuData->battler) == 2) {
                    battler1 = partyMenuData->battler;
                    battler2 = BattleSystem_GetBattlerIdPartner(partyMenuData->battleSys, partyMenuData->battler);
                } else {
                    battler1 = BattleSystem_GetBattlerIdPartner(partyMenuData->battleSys, partyMenuData->battler);
                    battler2 = partyMenuData->battler;
                }

                monSrc = AllocMonZeroed(HEAP_ID_BATTLE);

                for (i = 0; i < 6; i++) {
                    Party_AddMon(partyMenuData->battlePartyCtx->party, monSrc);
                }

                Heap_Free(monSrc);

                for (i = 0; i < BattleSystem_GetPartySize(partyMenuData->battleSys, battler1); i++) {
                    monSrc = BattleSystem_GetPartyMon(partyMenuData->battleSys, battler1, partyMenuData->partyOrder[battler1][i]);
                    monDst = Party_GetMonByIndex(partyMenuData->battlePartyCtx->party, i * 2);
                    CopyPokemonToPokemon(monSrc, monDst);
                    partyMenuData->battlePartyCtx->pokemonPartySlots[i * 2] = partyMenuData->partyOrder[battler1][i];
                }

                for (i = 0; i < BattleSystem_GetPartySize(partyMenuData->battleSys, battler2); i++) {
                    monSrc = BattleSystem_GetPartyMon(partyMenuData->battleSys, battler2, partyMenuData->partyOrder[battler2][i]);
                    monDst = Party_GetMonByIndex(partyMenuData->battlePartyCtx->party, i * 2 + 1);
                    CopyPokemonToPokemon(monSrc, monDst);
                    partyMenuData->battlePartyCtx->pokemonPartySlots[i * 2 + 1] = partyMenuData->partyOrder[battler2][i];
                }

                if (ov12_0223AB0C(partyMenuData->battleSys, partyMenuData->battler) == 4) {
                    partyMenuData->battlePartyCtx->selectedPartyIndex = 1;
                } else {
                    partyMenuData->battlePartyCtx->selectedPartyIndex = 0;
                }
            } else {
                if (BattleSystem_GetBattleType(partyMenuData->battleSys) & (1 << 1)
                    && (BattleSystem_GetBattleType(partyMenuData->battleSys) & (1 << 3)) == 0) {
                    battler = partyMenuData->battler & 1;
                } else {
                    battler = partyMenuData->battler;
                }

                if (ov12_0223AB0C(partyMenuData->battleSys, partyMenuData->battler) == 4) {
                    partyMenuData->battlePartyCtx->selectedPartyIndex = 1;
                } else {
                    partyMenuData->battlePartyCtx->selectedPartyIndex = 0;
                }

                Party *party = BattleSystem_GetParty(partyMenuData->battleSys, partyMenuData->battler);

                for (i = 0; i < Party_GetCount(party); i++) {
                    monSrc = BattleSystem_GetPartyMon(partyMenuData->battleSys, battler, partyMenuData->partyOrder[battler][i]);
                    Party_AddMon(partyMenuData->battlePartyCtx->party, monSrc);
                    partyMenuData->battlePartyCtx->pokemonPartySlots[i] = partyMenuData->partyOrder[battler][i];
                }
            }

            partyMenuData->battlePartyCtx->battleSys = partyMenuData->battleSys;
            partyMenuData->battlePartyCtx->heapID = HEAP_ID_BATTLE;
            partyMenuData->battlePartyCtx->battlePartyExited = 0;
            partyMenuData->battlePartyCtx->moveToLearn = partyMenuData->canSwitch;
            partyMenuData->battlePartyCtx->doubleBattleFirstSelectedPartySlot = partyMenuData->doublesSelection;
            partyMenuData->battlePartyCtx->battlePartyMode = partyMenuData->listMode;
            partyMenuData->battlePartyCtx->selectedBattleBagItem = partyMenuData->selectedBattleBagItem;
            partyMenuData->battlePartyCtx->battler = partyMenuData->battler;
            partyMenuData->battlePartyCtx->isCursorEnabled = partyMenuData->isCursorEnabled;

            if ((partyMenuData->battlersSwitchingMask & MaskOfFlagNo(partyMenuData->battler)) == 0) {
                partyMenuData->battlePartyCtx->playerPokemonPartySlot = partyMenuData->partySlots[partyMenuData->battler];
            } else {
                partyMenuData->battlePartyCtx->playerPokemonPartySlot = 6;
            }

            if (BattleSystem_GetBattleType(partyMenuData->battleSys) & (1 << 3)) {
                partyMenuData->battlePartyCtx->partnerPokemonPartySlot = 6;
            } else if ((partyMenuData->battlersSwitchingMask & MaskOfFlagNo(BattleSystem_GetBattlerIdPartner(partyMenuData->battleSys, partyMenuData->battler))) == 0) {
                partyMenuData->battlePartyCtx->partnerPokemonPartySlot = partyMenuData->partySlots[BattleSystem_GetBattlerIdPartner(partyMenuData->battleSys, partyMenuData->battler)];
            } else {
                partyMenuData->battlePartyCtx->partnerPokemonPartySlot = 6;
            }

#if defined(SDK_PORT) || defined(NONMATCHING)
            ov08_0221BE20(partyMenuData->battlePartyCtx);
#else
            ov10_0221BE20(partyMenuData->battlePartyCtx); // DS: the object names the overlay-10 symbol, same address as ov08_0221BE20 (the party menu opener)
#endif
            partyMenuData->state++;
        }
        break;
    case 2:
        if (partyMenuData->battlePartyCtx->battlePartyExited) {
            ov12_02237BB8(partyMenuData->battleSys);
            BattleInput_SetKeyPressed(BattleSystem_GetBattleInput(partyMenuData->battleSys), partyMenuData->battlePartyCtx->isCursorEnabled);
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xC00, -8, 7, 0, 0);
            PaletteData_BeginPaletteFade(paletteData, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, -8, 16, 0, 0);
            partyMenuData->state++;
        }
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            sub_0201649C(BattleSystem_GetMessageIcon(partyMenuData->battleSys), 0);

            if (partyMenuData->battlePartyCtx->selectedPartyIndex == 6) {
                BattleController_EmitPartyMenuResult(partyMenuData->battleSys, partyMenuData->battler, 0xFF);
            } else {
                BattleController_EmitPartyMenuResult(partyMenuData->battleSys, partyMenuData->battler, 1 + partyMenuData->battlePartyCtx->pokemonPartySlots[partyMenuData->battlePartyCtx->selectedPartyIndex]);
            }

            BattleController_EmitClearCommand(partyMenuData->battleSys, partyMenuData->battler, partyMenuData->command);
            Heap_Free(partyMenuData->battlePartyCtx->party);
            Heap_Free(partyMenuData->battlePartyCtx);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
