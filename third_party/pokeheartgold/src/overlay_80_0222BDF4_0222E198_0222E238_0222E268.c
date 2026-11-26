#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_system.h"

#include "options.h"

void ov80_0222E2B8(FrontierSystem *param_1, void *, u32);

void ov80_0222E328(FrontierSystem *, void *, u32);

void ov80_0222E344(FrontierSystem *, int, u32, u32, int);

void ov80_0222E268(FrontierSystem *param_1, void *param_2, u32 param_3, u8 param_4, u8 *param_5);

void ov80_0222E268(FrontierSystem *param_1, void *param_2, u32 param_3, u8 param_4, u8 *param_5) {
    u8 uVar1;
    u8 uVar2;
    FrontierMap *pFVar3;
    FrontierLaunchArgs *pFVar4;
    u8 uVar5;
    u32 uVar6;

    uVar6 = param_4;
    ov80_0222E2B8(param_1, param_2, param_3);
    ov80_0222E328(param_1, param_2, param_3);
    if (param_5 == NULL) {
        pFVar3 = FrontierSystem_GetFrontierMap(param_1);
        pFVar4 = Frontier_GetLaunchArgs(pFVar3->unk8);
        uVar1 = Options_GetTextFrameDelay(pFVar4->options);
        uVar2 = 0;
        uVar5 = 1;
    } else {
        uVar1 = *param_5;
        uVar2 = param_5[1];
        uVar5 = param_5[2];
    }
    ov80_0222E344(param_1, uVar5, uVar1, param_4, uVar2);
    return;
}
