#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "launch_application.h"
#include "party_menu.h"
#include "task.h"
#include "unk_02037C94.h"
#include "unk_struct_0209_c1ec.h"

BOOL sub_02097078(const UnkStruct_0209C194 *param_1);

u32 sub_02097108(UnkStruct_0209C1EC *param_1);

u32 sub_02097108(UnkStruct_0209C1EC *param_1) {
    BOOL BVar1;
    s32 iVar2;
    PartyMenuArgs *pPVar3;

    BVar1 = FieldSystem_ApplicationIsRunning(param_1->fieldSystem);
    if (BVar1 == 0) {
        iVar2 = sub_02097078(param_1->unk_24);
        if (iVar2 == 0) {
            param_1->unk_00 = 5;
        } else {
            sub_020398D4(1, 1);
            pPVar3 = PartyMenu_LaunchApp_Unk5(param_1->fieldSystem, param_1->unk_04);
            param_1->partyMenu = pPVar3;
            param_1->unk_24->unk_00 = 1;
            param_1->unk_00 = 2;
        }
    }
    return 0;
}

extern OverlayManagerTemplate _02108EB0;

s32 sub_02097148(UnkStruct_0209C1EC *arg0);

s32 sub_02097148(UnkStruct_0209C1EC *arg0) {
    u8 v0;
    PartyMenuArgs *ptr;

    if (FieldSystem_ApplicationIsRunning(arg0->fieldSystem) == 0) {
        ptr = arg0->partyMenu;
        v0 = ptr->partySlot;
        Heap_Free(ptr);
        if (arg0->partyMenu->selectedAction == 1) {
            arg0->monSummary = PokemonSummary_CreateArgs(arg0->fieldSystem, HEAP_ID_3, 0);
            arg0->unk_04 = (s32)v0;
            arg0->monSummary->partySlot = v0;
            PokemonSummary_LearnForget_LaunchApp(arg0->fieldSystem, arg0->monSummary);
            arg0->unk_00 = 3;
        } else {
            arg0->unk_24->unk_04 = (s32)v0;
            FieldSystem_LaunchApplication(arg0->fieldSystem, &_02108EB0, arg0->unk_24);
            arg0->unk_24->unk_00 = 3;
            arg0->unk_00 = 4;
        }
    }
    return 0;
}

u32 sub_020971AC(UnkStruct_0209C1EC *param_1);

u32 sub_020971AC(UnkStruct_0209C1EC *param_1) {
    BOOL BVar1;
    PartyMenuArgs *pPVar2;

    BVar1 = FieldSystem_ApplicationIsRunning(param_1->fieldSystem);
    if (BVar1 == 0) {
        Heap_Free(param_1->monSummary);
        pPVar2 = PartyMenu_LaunchApp_Unk5(param_1->fieldSystem, param_1->unk_04);
        param_1->partyMenu = pPVar2;
        param_1->unk_00 = 2;
    }
    return 0;
}

u32 sub_020971D4(UnkStruct_0209C1EC *param_1);

u32 sub_020971D4(UnkStruct_0209C1EC *param_1) {
    BOOL BVar1;

    BVar1 = FieldSystem_ApplicationIsRunning(param_1->fieldSystem);
    if (BVar1 == 0) {
        param_1->unk_00 = 5;
    }
    return 0;
}

void sub_02097060(UnkStruct_0209C194 *param_1);

u32 sub_020971EC(UnkStruct_0209C1EC *param_1);

u32 sub_020971EC(UnkStruct_0209C1EC *param_1) {
    sub_02097060(param_1->unk_24);
    return 1;
}
