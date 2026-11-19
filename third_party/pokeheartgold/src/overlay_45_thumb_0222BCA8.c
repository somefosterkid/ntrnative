#include "global.h"

#include "player_data.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222BCA8(s16 *arg0);

typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
void ov45_0222BCD8(ChooseStarterCursor *cursor);

typedef u16 charcode_t;
void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *param1, enum HeapID heapID);
void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);
void ov45_0222BCE4(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID);

s32 ov45_0222BCA8(s16 *arg0) {
    s32 v0;

    v0 = 0;
    if ((s32)*arg0 > 0) {
        v0 = 1;
    }
    return v0;
}

void ov45_0222BCB8(s16 *arg0);

void ov45_0222BCB8(s16 *arg0) {
    s16 v0;

    v0 = *arg0;
    if ((s32)v0 > 0) {
        *arg0 = v0 - 1;
    }
}

void ov45_0222BCC8(PlayerProfile **arg0, enum HeapID arg1);

void ov45_0222BCC8(PlayerProfile **arg0, enum HeapID heapID) {
    *arg0 = PlayerProfile_New(heapID);
}

void ov45_0222BCD8(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

void ov45_0222BCE4(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID) {
    WiFiPlazaProfile *v0 = Heap_Alloc(heapID, sizeof(WiFiPlazaProfile));

    MI_CpuCopyFast(&param1->profile, v0, sizeof(WiFiPlazaProfile));
    MI_CpuCopy8(param1->unk_10, v0->trainerName, sizeof(u16) * (7 + 1));

    ov45_0222A844(v0, param0->trainerInfo, heapID);
    Heap_Free(v0);
}
