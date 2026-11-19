/* PORTOK hk39 */
#include "global.h"
#include "message_format.h"
#include "options.h"
#include "overlay_80_02237A70.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "system.h"
#include "touchscreen_list_menu.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
#include "yes_no_prompt.h"
struct SysTask;
struct SysTaskQueue;
struct System;
struct UnkStruct_ov83_0223E14C;
typedef struct UnkStruct_ov83_0223E14C {
    u8 unk_00[4];
    void *frontier;
    u8 subState;
    u8 challengeType;
    u8 printerID;
    u8 unk_0B[2];
    u8 selectedMonSlot;
    u8 flags;
    u8 unk_0F;
    u16 unk_10;
    u8 unk_12;
    u8 selectedMenuEntry;
    u8 numSlots;
    u8 exitSlot;
    u8 unk_16[0x0E];
    void *msgFormat;
    u8 unk_28[0x28];
    u8 header[0x60];
    u8 msgBox[0x458];
    void *options;
    void *saveData;
    u8 unk_510[0x268];
    void *bigSparkles;
    u8 unk_77C[0x28];
    void *party;
    u8 unk_7A8[0x5C];
    void *selectedMon;
    u8 unk_808[0x30];
    void *unk_838;
    u8 unk_83C[4];
    void *listMenu;
    u8 unk_844[4];
    u32 menuPos;
    void *yesNoMenu;
    u8 unk_850[0xC];
    void *gridInput;
    u8 gridSlot;
    u8 gridCount;
    s16 gridPage;
    u16 gridAction;
    u8 unk_866[2];
    u8 unk_868[4];
} UnkStruct_ov83_0223E14C;
extern const u32 ov83_02247F4C[];
extern const u16 ov83_02247D18[];
extern const u16 ov83_02247D48[];
extern const u16 ov83_02247D4E[];
void ov83_02240348(void *app);
void ov83_02240384(void *app);
void ov83_02240290(void *app);
void ov83_02240300(void *app);
void ov83_022402F4(void *app);
void ov83_02240334(void *app);
void ov83_0224037C(void *app);
void ov83_022403B8(void *app);
void ov83_022403C0(void *app, u32 x);
void ov83_02240514(void *app);
void ov83_02240664(void *app);
void ov83_02240748(void *app);
void ov83_022407FC(void *app);
void ov83_022408E0(void *app, int x);
void ov83_0224042C(void *app);
void ov83_02240C48(void *app, u32 a, u32 b, u32 c, u32 d);
void ov83_02240C60(void *app, u32 a, void *boxMon);
u32 ov83_02240EC4(void *app, u32 a, u32 b);
u32 ov83_02240F48(void *app, u32 a, u32 b);
u32 ov83_02240FAC(void *app, u32 a, u32 b);
u32 ov83_0223FD14(void *app, u32 msg, u32 flag);
void ov83_02241208(void *app, int dir);
void ov83_02241254(void *app, int dir);
u32 ov83_022412DC(void *mon);
void ov83_02241354(void *window);
void ov83_022415F4(void *app, u32 a, u32 b);
void ov83_022416A0(void *app, u32 a, u32 b);
void ov83_0224175C(void *app);
void ov83_02241770(void *app, void *window);
void ov83_02241B18(void *app);
void ov83_02241BC4(void *app, u32 a, u32 b);
void ov83_022428A8(void *app);
void ov83_02242AB4(void *app, u32 a, u32 b);
void ov83_02242DAC(void *app);
void ov83_02242DFC(void *app);
void ov83_02242E88(void *app);
void ov83_02242F18(void *app, u32 x);
u32 ov83_02242F2C(void *app);
void ov83_02247630(void *sparkles, u32 x, u32 y);
u32 ov83_02247768(u32 a, u32 b);
u8 ov83_0224777C(void *saveData, u32 challengeType, u32 kind);
void ov83_022477B0(int input, u32 se);
void ov83_022477EC(u32 a, u32 b, void *p);
void ov83_022478B4(void *yesNo);
void ov83_02247944(void *window, u32 frame);
int ov83_02247AD4(void *p);
void ov83_02247B04(void *p);
int ov83_02247BC4(void *p);
u32 ov83_02247CF0(void);
void ov80_02237FA4(void *frontier, u32 challengeType, u32 cost);

int ov83_0223E14C(UnkStruct_ov83_0223E14C *app);
int ov83_0223E14C(UnkStruct_ov83_0223E14C *app)
{
    u32 input;
    u32 rank;
    u32 cp;
    void *mon;
    u32 statA;
    u32 statB;

    switch (app->subState) {
    case 0: {
        u32 sync = (u32)(app->flags << 25) >> 30;

        if (sync == 1) {
            ov83_02240348(app);
            ov83_02247630(app->bigSparkles, 0xCC, 0x64);
            app->subState = 2;
            app->flags &= ~0x60;
            return 0;
        }
        if (sync == 2) {
            ov83_02240384(app);
            ov83_02247630(app->bigSparkles, 0xD3, 0x6A);
            app->subState = 8;
            app->flags &= ~0x60;
            return 0;
        }
        input = ov83_02247AD4(app->unk_838);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
            ov83_022402F4(app);
            ov83_02240300(app);
            app->subState = 1;
            break;
        case 4:
            PlaySE(0x5DC);
            return 1;
        case (u32)-2:
            if (app->selectedMonSlot != app->exitSlot) {
                ov83_02247B04(app->unk_838);
                ov83_02242AB4(app, 4, app->selectedMonSlot);
            }
            break;
        }
        break;
    }
    case 1:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        ov83_022477B0(input, 0x5DC);
        ov83_02242DAC(app);
        switch (input) {
        case 0:
            ov83_02240334(app);
            ov83_02240348(app);
            app->subState = 2;
            break;
        case 5:
            ov83_02240334(app);
            ov83_02240384(app);
            app->subState = 8;
            break;
        case 11:
        case (u32)-2:
            ov83_02240334(app);
            ov83_02240290(app);
            app->subState = 0;
            break;
        case (u32)-1:
        default:
            break;
        }
        break;
    case 2:
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        ov83_022477B0(input, 0x5DC);
        ov83_02242DFC(app);
        switch (input) {
        case 1:
        case 2:
        case 3:
            app->selectedMenuEntry = input;
            ov83_0224037C(app);
            ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
            rank = ov83_0224777C(app->saveData, app->challengeType, 0);
            if (rank < ov83_02247F4C[app->menuPos * 3]) {
                app->printerID = ov83_0223FD14(app, 0x21, 1);
                app->subState = 7;
                break;
            }
            ov83_02240C48(app, 0, ov83_02247D18[app->menuPos], 3, 0);
            app->printerID = ov83_0223FD14(app, 0x37, 1);
            ov83_02240514(app);
            app->subState = 3;
            break;
        case 4:
            rank = ov83_0224777C(app->saveData, app->challengeType, 0);
            if (rank == 3) {
                StopSE(0x5DC, 0);
                PlaySE(0x5F3);
                break;
            }
            app->selectedMenuEntry = input;
            ov83_0224037C(app);
            statA = sub_0205C1F0(app->challengeType);
            statB = sub_0205C268(sub_0205C1F0(app->challengeType));
            FrontierSave_GetStat(app->frontier, statA, statB);
            ov83_02240C48(app, 0, ov83_02247D48[rank], 4, 0);
            app->printerID = ov83_0223FD14(app, 0x26, 1);
            ov83_02240514(app);
            app->subState = 4;
            break;
        case 11:
        case (u32)-2:
            ov83_0224037C(app);
            ov83_02240300(app);
            app->subState = 1;
            break;
        case (u32)-1:
        default:
            break;
        }
        break;
    case 3:
        input = YesNoPrompt_HandleInput(app->yesNoMenu);
        switch (input) {
        case 1: {
            u32 menuPos;

            ov83_022478B4(&app->yesNoMenu);
            mon = Party_GetMonByIndex(app->party, ov83_02247768(app->numSlots, app->selectedMonSlot));
            statA = sub_0205C1F0(app->challengeType);
            statB = sub_0205C268(sub_0205C1F0(app->challengeType));
            cp = FrontierSave_GetStat(app->frontier, statA, statB);
            rank = ov83_0224777C(app->saveData, app->challengeType, 0);
            menuPos = app->menuPos;
            if (rank < ov83_02247F4C[menuPos * 3]) {
                ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                app->printerID = ov83_0223FD14(app, 0x21, 1);
                app->subState = 7;
                break;
            }
            if (cp < ov83_02247D18[menuPos]) {
                ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                app->printerID = ov83_0223FD14(app, 0x20, 1);
                app->subState = 7;
                break;
            }
            if (menuPos == 0) {
                u32 hp = GetMonData(mon, 0xA3, 0L );

                if (hp == GetMonData(mon, 0xA4, 0L )) {
                    app->printerID = ov83_0223FD14(app, 0x25, 1);
                    app->subState = 7;
                    break;
                }
            } else if (menuPos == 1) {
                if (ov83_022412DC(mon) == 0) {
                    app->printerID = ov83_0223FD14(app, 0x25, 1);
                    app->subState = 7;
                    break;
                }
            } else {
                u32 hp = GetMonData(mon, 0xA3, 0L );

                if (hp == GetMonData(mon, 0xA4, 0L ) && ov83_022412DC(mon) == 0) {
                    app->printerID = ov83_0223FD14(app, 0x25, 1);
                    app->subState = 7;
                    break;
                }
            }
            if (ov80_02237D8C(app->challengeType) == 0) {
                ov80_02237FA4(app->frontier, app->challengeType, ov83_02247D18[app->selectedMenuEntry - 1]);
                ov83_02241770(app, app->header);
                ov83_022415F4(app, app->selectedMonSlot, app->selectedMenuEntry);
                app->subState = 0x13;
                break;
            }
            app->flags |= 2;
            return 1;
        }
        case 2:
            ov83_022478B4(&app->yesNoMenu);
            ov83_02240348(app);
            app->subState = 2;
            break;
        }
        break;
    case 4:
        input = YesNoPrompt_HandleInput(app->yesNoMenu);
        switch (input) {
        case 1:
            ov83_022478B4(&app->yesNoMenu);
            statA = sub_0205C1F0(app->challengeType);
            statB = sub_0205C268(sub_0205C1F0(app->challengeType));
            cp = FrontierSave_GetStat(app->frontier, statA, statB);
            rank = ov83_0224777C(app->saveData, app->challengeType, 0);
            if (cp < ov83_02247D48[rank]) {
                ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                app->printerID = ov83_0223FD14(app, 0x29, 1);
                app->subState = 7;
                break;
            }
            if (ov80_02237D8C(app->challengeType) == 0) {
                ov83_02241BC4(app, app->selectedMonSlot, 4);
                app->subState = 5;
                break;
            }
            app->flags |= 2;
            return 1;
        case 2:
            ov83_022478B4(&app->yesNoMenu);
            ov83_02240348(app);
            app->subState = 2;
            break;
        }
        break;
    case 5:
        if (ov83_02240FAC(app, app->selectedMonSlot, app->selectedMenuEntry) == 1) {
            app->subState = 6;
        }
        break;
    case 6:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02240348(app);
            ov83_02247630(app->bigSparkles, 0xCC, 0x64);
            app->subState = 2;
        }
        break;
    case 7:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02241354(app->msgBox);
            ov83_02240348(app);
            app->subState = 2;
        }
        break;
    case 8:
        ov83_022477EC(2, 0, app->unk_868);
        input = TouchscreenListMenu_HandleInput(app->listMenu);
        ov83_022477B0(input, 0x5DC);
        ov83_02242E88(app);
        switch (input) {
        case (u32)-2:
            ov83_022403B8(app);
            ov83_02240300(app);
            app->subState = 1;
            break;
        case 6:
            app->selectedMenuEntry = input;
            ov83_022403B8(app);
            ov83_022403C0(app, 6);
            app->subState = 9;
            break;
        case 7:
            app->selectedMenuEntry = input;
            ov83_022403B8(app);
            rank = ov83_0224777C(app->saveData, app->challengeType, 1);
            if (rank == 1) {
                app->printerID = ov83_0223FD14(app, 0x36, 1);
                app->subState = 0xF;
                return 0;
            }
            ov83_022403C0(app, 7);
            app->subState = 9;
            break;
        case 8:
            rank = ov83_0224777C(app->saveData, app->challengeType, 1);
            if (rank == 3) {
                StopSE(0x5DC, 0);
                PlaySE(0x5F3);
                break;
            }
            app->selectedMenuEntry = input;
            ov83_022403B8(app);
            ov83_02240C48(app, 0, ov83_02247D4E[rank], 4, 0);
            app->printerID = ov83_0223FD14(app, 0x26, 1);
            ov83_02240514(app);
            app->subState = 0xC;
            break;
        case (u32)-1:
        default:
            break;
        }
        break;
    case 9:
        input = ov83_02247BC4(app->gridInput);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            app->gridSlot = input + 6 * app->gridPage;
            if (app->gridSlot < app->gridCount) {
                PlaySE(0x5DD);
                ov83_02242F18(app, (u16)input);
                app->subState = 10;
            }
            break;
        case 6:
            PlaySE(0x5E0);
            ov83_02242F18(app, 6);
            app->subState = 10;
            break;
        case 7:
            PlaySE(0x5E0);
            ov83_02242F18(app, 7);
            app->subState = 10;
            break;
        case (u32)-3:
            PlaySE(0x5DC);
            break;
        case 8:
        case (u32)-2:
            PlaySE(0x5DD);
            ov83_02242F18(app, 8);
            app->subState = 10;
            break;
        }
        break;
    case 10:
        if (ov83_02242F2C(app) == 1) {
            break;
        }
        switch (app->gridAction) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            ov83_022428A8(app);
            ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
            ov83_02240C48(app, 0, ov83_02240EC4(app, app->gridSlot, app->selectedMenuEntry), 3, 0);
            app->printerID = ov83_0223FD14(app, 0x37, 1);
            ov83_02240514(app);
            app->subState = 0xB;
            break;
        case 6:
            app->gridPage = app->gridPage - 1;
            if (app->gridPage < 0) {
                app->gridPage = (app->gridCount - 1) / 6;
            }
            ov83_02240664(app);
            ov83_02240748(app);
            ov83_022407FC(app);
            ov83_022408E0(app, GridInputHandler_GetNextInput(app->gridInput));
            app->subState = 9;
            break;
        case 7:
            app->gridPage = app->gridPage + 1;
            if ((app->gridCount - 1) / 6 < app->gridPage) {
                app->gridPage = 0;
            }
            ov83_02240664(app);
            ov83_02240748(app);
            ov83_022407FC(app);
            ov83_022408E0(app, GridInputHandler_GetNextInput(app->gridInput));
            app->subState = 9;
            break;
        case 8:
            ov83_0224042C(app);
            ov83_02240384(app);
            app->subState = 8;
            break;
        }
        break;
    case 11:
        input = YesNoPrompt_HandleInput(app->yesNoMenu);
        switch (input) {
        case 1:
            ov83_022478B4(&app->yesNoMenu);
            statA = sub_0205C1F0(app->challengeType);
            statB = sub_0205C268(sub_0205C1F0(app->challengeType));
            cp = FrontierSave_GetStat(app->frontier, statA, statB);
            if (cp < ov83_02240EC4(app, app->gridSlot, app->selectedMenuEntry)) {
                ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                app->printerID = ov83_0223FD14(app, 0x20, 1);
                app->subState = 0x10;
                break;
            }
            mon = app->selectedMon;
            if (GetMonData(mon, 6, 0L ) == 0) {
                if (ov80_02237D8C(app->challengeType) == 0) {
                    ov83_0224042C(app);
                    ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                    ov80_02237FA4(app->frontier, app->challengeType, ov83_02240EC4(app, app->gridSlot, app->selectedMenuEntry));
                    ov83_02241770(app, app->header);
                    ov83_022416A0(app, app->selectedMonSlot, ov83_02240F48(app, app->gridSlot, app->selectedMenuEntry));
                    app->subState = 0x13;
                    break;
                }
                app->unk_10 = ov83_02240F48(app, app->gridSlot, app->selectedMenuEntry);
                ov83_0224042C(app);
                ov83_02241354(app->msgBox);
                app->flags |= 2;
                return 1;
            }
            ov83_02240C60(app, 0, Mon_GetBoxMon(mon));
            BufferItemNameWithIndefArticle(app->msgFormat, 1, GetMonData(mon, 6, 0L ));
            ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
            app->printerID = ov83_0223FD14(app, 0x3C, 1);
            app->subState = 0x11;
            break;
        case 2:
            ov83_022478B4(&app->yesNoMenu);
            ov83_0224175C(app);
            app->subState = 9;
            break;
        }
        break;
    case 12:
        input = YesNoPrompt_HandleInput(app->yesNoMenu);
        switch (input) {
        case 1:
            ov83_022478B4(&app->yesNoMenu);
            statA = sub_0205C1F0(app->challengeType);
            statB = sub_0205C268(sub_0205C1F0(app->challengeType));
            cp = FrontierSave_GetStat(app->frontier, statA, statB);
            rank = ov83_0224777C(app->saveData, app->challengeType, 1);
            if (cp < ov83_02247D4E[rank]) {
                ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                app->printerID = ov83_0223FD14(app, 0x29, 1);
                app->subState = 0xF;
                break;
            }
            if (ov80_02237D8C(app->challengeType) == 0) {
                ov83_02241BC4(app, app->selectedMonSlot, 8);
                app->subState = 0xD;
                break;
            }
            app->flags |= 2;
            return 1;
        case 2:
            ov83_022478B4(&app->yesNoMenu);
            ov83_02240384(app);
            app->subState = 8;
            break;
        }
        break;
    case 13:
        if (ov83_02240FAC(app, app->selectedMonSlot, app->selectedMenuEntry) == 1) {
            app->subState = 0xE;
        }
        break;
    case 14:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02240384(app);
            ov83_02247630(app->bigSparkles, 0xD3, 0x6A);
            app->subState = 8;
        }
        break;
    case 15:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02241354(app->msgBox);
            ov83_02240384(app);
            app->subState = 8;
        }
        break;
    case 16:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_0224175C(app);
            app->subState = 9;
        }
        break;
    case 17:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            app->printerID = ov83_0223FD14(app, 0x3D, 1);
            ov83_02240514(app);
            app->subState = 0x12;
        }
        break;
    case 18:
        input = YesNoPrompt_HandleInput(app->yesNoMenu);
        switch (input) {
        case 1:
            ov83_022478B4(&app->yesNoMenu);
            ov83_02240664(app);
            if (ov80_02237D8C(app->challengeType) == 0) {
                ov83_0224042C(app);
                ov83_02247944((app)->msgBox, Options_GetFrame((app)->options)) ;
                ov80_02237FA4(app->frontier, app->challengeType, ov83_02240EC4(app, app->gridSlot, app->selectedMenuEntry));
                ov83_02241770(app, app->header);
                ov83_022416A0(app, app->selectedMonSlot, ov83_02240F48(app, app->gridSlot, app->selectedMenuEntry));
                app->subState = 0x13;
                break;
            }
            app->unk_10 = ov83_02240F48(app, app->gridSlot, app->selectedMenuEntry);
            ov83_0224042C(app);
            ov83_02241354(app->msgBox);
            app->flags |= 2;
            return 1;
        case 2:
            ov83_022478B4(&app->yesNoMenu);
            ov83_0224175C(app);
            app->subState = 9;
            break;
        }
        break;
    case 19:
        ov83_022477EC(2, 0, app->unk_868);
        if (ov83_02240FAC(app, app->selectedMonSlot, app->selectedMenuEntry) == 1) {
            app->subState = 0x14;
        }
        break;
    case 20:
        if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02241354(app->msgBox);
            ov83_02240290(app);
            app->subState = 0;
        }
        break;
    case 21:
        if (gSystem.newKeys & 0x20) {
            ov83_02241208(app, -1);
        } else if (gSystem.newKeys & 0x10) {
            ov83_02241208(app, 1);
        } else if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02241B18(app);
            ov83_02240300(app);
            app->subState = 1;
        }
        break;
    case 22:
        if (gSystem.newKeys & 0x20) {
            ov83_02241254(app, -1);
        } else if (gSystem.newKeys & 0x10) {
            ov83_02241254(app, 1);
        } else if (ov83_02247CF0() == 1) {
            PlaySE(0x5DC);
            ov83_02241B18(app);
            ov83_02240300(app);
            app->subState = 1;
        }
        break;
    }
    return 0;
}
