#include "global.h"

#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "safari_zone.h"
#include "save_vars_flags.h"
#include "sys_flags.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E59E4(SafariAreaCustomizer *arg0, SafariAreaCustomizerArgs *arg1);

s32 ov108_021E5D90(SafariAreaCustomizer *arg0);
void ov108_021E846C(SafariAreaCustomizer *arg0);
s32 ov108_021E5DB8(SafariAreaCustomizer *arg0);
int ov108_021E5E68(SafariAreaCustomizer *data);
u32 ov108_021E5AA0(SafariAreaCustomizer *param_1);
s32 ov108_021E5F10(SafariAreaCustomizer *arg0);
int ov108_021E5F38(SafariAreaCustomizer *app);
s32 ov108_021E6010(SafariAreaCustomizer *arg0);
u32 ov108_021E5AF0(SafariAreaCustomizer *param_1);
s32 ov108_021E6068(SafariAreaCustomizer *arg0);
s32 ov108_021E6090(SafariAreaCustomizer *arg0);
u32 ov108_021E5B48(SafariAreaCustomizer *param_1);

void ov108_021E59E4(SafariAreaCustomizer *arg0, SafariAreaCustomizerArgs *arg1) {
    Options *options;
    SafariZone *safari_zone;

    safari_zone = Save_SafariZone_Get(arg1->saveData);
    arg0->safariZone = safari_zone;
    SafariZone_CopyAreaSet(safari_zone, 0, &arg0->areaSet);
    options = Save_PlayerData_GetOptionsAddr(arg1->saveData);
    arg0->textSpeed = (s8)Options_GetTextSpeed(options);
    arg0->frame = (s8)Options_GetFrame(options);
    arg0->inTouchMode = MenuInputStateMgr_GetState(arg1->menuInputStatePtr);
    arg0->areaChanged = 0;
    arg0->args = arg1;
    if (SafariZone_GetObjectUnlockLevel(arg0->safariZone) != 0) {
        arg0->unk_184E3 = 1;
    }
}

void ov108_021E5A48(SafariAreaCustomizer *arg0);

void ov108_021E5A48(SafariAreaCustomizer *arg0) {
    SafariZone_SetAreaSet(arg0->safariZone, 0, &arg0->areaSet);
    ChangeFlag99D(Save_VarsFlags_Get(arg0->args->saveData), arg0->areaChanged);
    MenuInputStateMgr_SetState(arg0->args->menuInputStatePtr, (enum MenuInputState)arg0->inTouchMode);
}

s32 ov108_021E5C54(SafariAreaCustomizer *menuData);

s32 ov108_021E5A78(SafariAreaCustomizer *arg0);

s32 ov108_021E5A78(SafariAreaCustomizer *arg0) {
    if (ov108_021E5C54(arg0) != 0) {
        return 5;
    }
    return 0;
}

u32 ov108_021E5D0C(SafariAreaCustomizer *param_1);

s32 ov108_021E5A8C(SafariAreaCustomizer *arg0);

s32 ov108_021E5A8C(SafariAreaCustomizer *arg0) {
    if (ov108_021E5D0C(arg0) != 0) {
        return 7;
    }
    return 1;
}

u32 ov108_021E5AA0(SafariAreaCustomizer *param_1) {
    s32 sVar1;
    u32 uVar2;

    switch (param_1->unk_4) {
    case 0:
        sVar1 = ov108_021E5D90(param_1);
        param_1->unk_4 = sVar1;
        break;
    case 1:
        uVar2 = ov108_021E5DB8(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 2:
        uVar2 = ov108_021E5E68(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 4:
        param_1->unk_4 = 0;
        ov108_021E846C(param_1);
        return param_1->unk_C;
    }
    ov108_021E846C(param_1);
    return 2;
}

u32 ov108_021E5AF0(SafariAreaCustomizer *param_1) {
    s32 sVar1;
    u32 uVar2;

    switch (param_1->unk_4) {
    case 0:
        sVar1 = ov108_021E5F10(param_1);
        param_1->unk_4 = sVar1;
        break;
    case 1:
        uVar2 = ov108_021E5F38(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 2:
        uVar2 = ov108_021E5E68(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 3:
        uVar2 = ov108_021E6010(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 4:
        param_1->unk_4 = 0;
        ov108_021E846C(param_1);
        return param_1->unk_C;
    }
    ov108_021E846C(param_1);
    return 3;
}

u32 ov108_021E5B48(SafariAreaCustomizer *param_1) {
    s32 sVar1;
    u32 uVar2;

    switch (param_1->unk_4) {
    case 0:
        sVar1 = ov108_021E6068(param_1);
        param_1->unk_4 = sVar1;
        break;
    case 2:
        uVar2 = ov108_021E5E68(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 3:
        uVar2 = ov108_021E6090(param_1);
        param_1->unk_4 = uVar2;
        break;
    case 4:
        param_1->unk_4 = 0;
        ov108_021E846C(param_1);
        return param_1->unk_C;
    }
    ov108_021E846C(param_1);
    return 4;
}
