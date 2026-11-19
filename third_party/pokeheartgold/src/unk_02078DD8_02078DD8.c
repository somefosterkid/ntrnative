#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_00_thumb.h"
#include "player_data.h"

void *sub_02078DD8(PlayerProfile *arg0, enum HeapID arg1);

void *sub_02078DD8(PlayerProfile *arg0, enum HeapID arg1) {
    void *temp_r0;

    temp_r0 = Heap_Alloc(arg1, 0x24U);
    memset(temp_r0, 0, 0x24U);
    *(s8 *)((u8 *)temp_r0 + 0x1B) = 0x1C;
    *(s8 *)((u8 *)temp_r0 + 0x1E) = (s8)PlayerProfile_GetTrainerGender(arg0);
    *(u8 *)((u8 *)temp_r0 + 0x1D) = PlayerProfile_GetAvatar(arg0);
    *(u8 *)((u8 *)temp_r0 + 0x18) = PlayerProfile_GetVersion(arg0);
    *(u8 *)((u8 *)temp_r0 + 0x19) = PlayerProfile_GetLanguage(arg0);
    if (ov00_021E6D60(temp_r0, 0x24) != 1) {
        GF_AssertFail();
    }
    return temp_r0;
}
