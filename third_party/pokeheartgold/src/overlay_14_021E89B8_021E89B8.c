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
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(void *param_1, u32 param_2, u32 param_3);

extern const u8 ov14_021F7D0C[];

u32 ov14_021E89B8(u8 **param_1) {
    s32 uVar1;
    PokemonSummaryArgs *parentWork;
    Party *pPVar2;
    s32 iVar3;
    PlayerProfile *pPVar4;
    BOOL BVar5;
    SaveSpecialRibbons *pSVar6;
    u32 uVar7;
    OverlayManager *pOVar8;

    param_1[6] = (u8 *)Heap_Alloc(HEAP_ID_9, sizeof(PokemonSummaryArgs));
    parentWork = (PokemonSummaryArgs *)param_1[6];
    if (*(u8 *)((u8 *)param_1 + 0x21) < 0x1e) {
        pPVar2 = (Party *)ov14_021E60C0((u8 *)param_1, (u32) * (u8 *)((u8 *)param_1 + 0x1f), 0);
        parentWork->party = pPVar2;
        parentWork->unk11 = 2;
        parentWork->partyCount = 0x1e;
        uVar1 = *(u8 *)((u8 *)param_1 + 0x21);
    } else {
        parentWork->party = (Party *)param_1[2];
        parentWork->unk11 = 1;
        iVar3 = Party_GetCount((Party *)param_1[2]);
        parentWork->partyCount = (u8)iVar3;
        uVar1 = *(u8 *)((u8 *)param_1 + 0x21) - 0x1e;
    }
    parentWork->partySlot = uVar1;
    parentWork->options = (Options *)param_1[4];
    pPVar4 = Save_PlayerData_GetProfile(*(SaveData **)*param_1);
    sub_0208AD34(parentWork, pPVar4);
    parentWork->unk12 = 0;
    sub_02089D40(parentWork, (u8 *)ov14_021F7D0C);
    parentWork->unk16 = 0;
    ((u8 *)parentWork)[0x17] = 0;
    parentWork->moveToLearn = 0;
    BVar5 = SaveArray_IsNatDexEnabled(*(SaveData **)*param_1);
    parentWork->natDexEnabled = BVar5;
    pSVar6 = Save_SpecialRibbons_Get(*(SaveData **)*param_1);
    parentWork->ribbons = pSVar6;
    *(u32 *)parentWork->unk24 = 0;
    parentWork->unk28 = 0;
    uVar7 = sub_02088288(*(SaveData **)*param_1);
    parentWork->unk2C = uVar7;
    BVar5 = sub_0208828C(*(SaveData **)*param_1);
    parentWork->isFlag982Set = BVar5;
    parentWork->menuInputStatePtr = *(MenuInputStateMgr **)(*param_1 + 4);
    pOVar8 = OverlayManager_New((OverlayManagerTemplate *)&gOverlayTemplate_PokemonSummary, parentWork, HEAP_ID_9);
    param_1[5] = (u8 *)pOVar8;
    return 0;
}
