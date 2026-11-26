#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_manager.h"
#include "party.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_special_ribbons.h"
#include "unk_02088288.h"
#include "overlay_14.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(void *param_1, u32 param_2, u32 param_3);

extern const u8 ov14_021F7D0C[];

u32 ov14_021E89B8(UnkStruct_ov14_boxapp *param_1) {
    s32 uVar1;
    PokemonSummaryArgs *parentWork;
    Party *pPVar2;
    s32 iVar3;
    PlayerProfile *pPVar4;
    BOOL BVar5;
    SaveSpecialRibbons *pSVar6;
    u32 uVar7;
    OverlayManager *pOVar8;

    param_1->summaryArgs = Heap_Alloc(HEAP_ID_9, sizeof(PokemonSummaryArgs));
    parentWork = param_1->summaryArgs;
    if (param_1->unk21 < 0x1e) {
        pPVar2 = (Party *)ov14_021E60C0(param_1, (u32) param_1->unk1F, 0);
        parentWork->party = pPVar2;
        parentWork->unk11 = 2;
        parentWork->partyCount = 0x1e;
        uVar1 = param_1->unk21;
    } else {
        parentWork->party = param_1->party;
        parentWork->unk11 = 1;
        iVar3 = Party_GetCount(param_1->party);
        parentWork->partyCount = (u8)iVar3;
        uVar1 = param_1->unk21 - 0x1e;
    }
    parentWork->partySlot = uVar1;
    parentWork->options = param_1->options;
    pPVar4 = Save_PlayerData_GetProfile(param_1->unk_0->saveData);
    sub_0208AD34(parentWork, pPVar4);
    parentWork->unk12 = 0;
    sub_02089D40(parentWork, (u8 *)ov14_021F7D0C);
    parentWork->unk16 = 0;
    ((u8 *)parentWork)[0x17] = 0;
    parentWork->moveToLearn = 0;
    BVar5 = SaveArray_IsNatDexEnabled(param_1->unk_0->saveData);
    parentWork->natDexEnabled = BVar5;
    pSVar6 = Save_SpecialRibbons_Get(param_1->unk_0->saveData);
    parentWork->ribbons = pSVar6;
    *(u32 *)parentWork->unk24 = 0;
    parentWork->unk28 = 0;
    uVar7 = sub_02088288(param_1->unk_0->saveData);
    parentWork->unk2C = uVar7;
    BVar5 = sub_0208828C(param_1->unk_0->saveData);
    parentWork->isFlag982Set = BVar5;
    parentWork->menuInputStatePtr = param_1->unk_0->menuInputStatePtr;
    pOVar8 = OverlayManager_New((OverlayManagerTemplate *)&gOverlayTemplate_PokemonSummary, parentWork, HEAP_ID_9);
    param_1->unk_14 = pOVar8;
    return 0;
}
