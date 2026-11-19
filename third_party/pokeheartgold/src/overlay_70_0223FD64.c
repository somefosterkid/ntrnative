#include "global.h"

#include "pokemon.h"
#include "pokemon_types_def.h"
#include "game_stats.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02237F64(void *);

void ov70_02240D44(GTSApplicationState *, s16, s16);

u32 ov70_0223FD64(GTSApplicationState *param_1);

s32 ov70_02237F38(void);
s32 ov70_02237F58(void);
s32 ov70_022409C0(GTSApplicationState *, s32);
s32 ov70_0223FD98(GTSApplicationState *arg0);
s32 ov70_02237FB4(void);
s32 ov70_0223FE40(GTSApplicationState *arg0);
s32 ov70_0223FE60(GTSApplicationState *arg0);

u32 ov70_0223FD64(GTSApplicationState *param_1) {
    Pokemon_RemoveCapsule((Pokemon *)&param_1->receivedListing);
    ov70_02237F64(&param_1->receivedListing);
    param_1->currentScreenInstruction = 3;
    param_1->networkTimeoutCounter = 0;
    ov70_02240D44(param_1, 4, 6);
    return 3;
}

s32 ov70_0223FD98(GTSApplicationState *arg0) {
    s32 v0;
    u32 v1;

    if (ov70_02237F38() != 0) {
        v0 = ov70_02237F58();
        arg0->networkTimeoutCounter = 0;
        v1 = v0 + 0xF;
        switch (v1) {
        case 15:
            ov70_022409C0(arg0, 1);
            GameStats_Inc(*(GameStats **)((u8 *)(arg0->playerData) + 0x28), 0x2D);
            arg0->currentScreenInstruction = 0x1E;
            break;
        case 14:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x26;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x27;
            break;
        case 3:
        case 11:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x27;
            break;
        case 0:
        case 1:
        case 13:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x26;
            break;
        case 2:
            sub_020399EC();
            break;
        }
    } else {
        arg0->networkTimeoutCounter = (s32)(arg0->networkTimeoutCounter + 1);
        if (arg0->networkTimeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov70_0223FE40(GTSApplicationState *arg0) {
    ov70_02237FB4();
    arg0->currentScreenInstruction = 5;
    arg0->networkTimeoutCounter = 0;
    arg0->isPokemonListed = 1;
    return 3;
}

s32 ov70_0223FE60(GTSApplicationState *arg0) {
    s32 v0;
    u32 v1;

    if (ov70_02237F38() != 0) {
        v0 = ov70_02237F58();
        arg0->networkTimeoutCounter = 0;
        v1 = v0 + 0xF;
        switch (v1) {
        case 15:
            arg0->currentScreenInstruction = 0x21;
            break;
        case 3:
        case 11:
            arg0->commsErrorMessage = v0;
            arg0->currentScreenInstruction = 0x27;
            break;
        case 0:
        case 1:
        case 10:
        case 12:
        case 13:
        case 14:
            sub_02039AD8(4);
            break;
        case 2:
            sub_020399EC();
            break;
        }
    } else {
        arg0->networkTimeoutCounter = (s32)(arg0->networkTimeoutCounter + 1);
        if (arg0->networkTimeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}
