#include "global.h"

#include "heap.h"
#include "overlay_67_app.h"
#include "pm_string.h"
#include "unk_0202D230.h"

void ov67_021E63E8(Ov67App *app) {
    WifiBattleTowerMatchCandidate *sp4;
    s32 sp0;
    Ov67WifiTrainer *var_r4;
    String *temp_r0_2;
    s32 temp_r0;
    WifiBattleTowerMatchCandidate *var_r5;
    WifiBattleTowerMatchCandidate *var_r6;

    sp4 = sub_0202D8E4(app->downloadData, app->heapID);
    sp0 = 0;
    var_r4 = app->wifiTrainers;
    var_r5 = sp4;
    var_r6 = var_r5;
    do {
        var_r4->anonNameIdx = var_r5->anonymousNameIdx;
        var_r4->country = var_r5->country;
        var_r4->region = var_r5->region;
        var_r4->useDefaultName = var_r5->isAnonymous;
        MI_CpuCopy8(var_r5->trainerKey, &var_r4->message, 8U);
        var_r4->name = String_New(8U, app->heapID);
        temp_r0_2 = var_r4->name;
        if (var_r4->useDefaultName != 0) {
            String_Copy(var_r4->name, app->defaultNames[var_r4->anonNameIdx]);
        } else {
            CopyU16ArrayToString(var_r4->name, var_r6->trainerName);
        }
        var_r4++;
        var_r5++;
        temp_r0 = sp0 + 1;
        var_r6++;
        sp0 = temp_r0;
    } while (temp_r0 < 0x1E);
    Heap_Free(sp4);
}
