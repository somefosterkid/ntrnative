#include "global.h"

#include "field/weather_manager.h"

#include "field_system.h"
#include "overlay_01_021FC05C.h"
#include "heap.h"
#include "overlay_01.h"

extern u32 ov01_02208BC0;

u32 ov01_021FC2C8(FieldSystem *fieldSystem) {
    u32 uVar1;
    s32 iVar2;
    u32 *puVar3;

    uVar1 = WeatherManager_GetWeather(fieldSystem->unk4->weatherManager);
    puVar3 = (u32 *)&ov01_02208BC0;
    iVar2 = 0;
    do {
        if (uVar1 == *puVar3) {
            return 0;
        }
        iVar2 = iVar2 + 1;
        puVar3 = puVar3 + 1;
    } while (iVar2 < 3);
    return 1;
}

void ov01_021FC2F0(HoneyScentTaskEnv *env) {
    if (env->task != NULL) {
        ov01_021FC124(env->task);
        env->task = NULL;
    }
    Heap_Free(env);
}

u32 GetHoneySweetScentWorkSize(void) {
    return sizeof(HoneyScentTaskEnv);
}
