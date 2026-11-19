#include "global.h"

#include "heap.h"
#include "player_data.h"
#include "string_util.h"
#include "unk_02031B0C.h"

ApricornRecord *sub_020320E0(SaveApricornBox *arg0, PlayerProfile *arg1, u32 arg2, enum HeapID arg3) {
    ApricornRecord *temp_r0;
    temp_r0 = Heap_AllocAtEnd(arg3, sizeof(ApricornRecord));
    MI_CpuFill8(temp_r0, 0U, sizeof(ApricornRecord));
    temp_r0->trainerId = PlayerProfile_GetTrainerID(arg1);
    temp_r0->gender = PlayerProfile_GetTrainerGender(arg1);
    temp_r0->language = PlayerProfile_GetLanguage(arg1);
    temp_r0->version = PlayerProfile_GetVersion(arg1);
    StringFillEOS(temp_r0->name, 8U);
    CopyU16StringArrayN(temp_r0->name, PlayerProfile_GetNamePtr(arg1), 7U);
    sub_02031CEC(arg0, 8U, &temp_r0->data);
    if (arg2 > 0xFFFFU) {
        temp_r0->data.fanCount = (double)0xFFFF;
    } else {
        temp_r0->data.fanCount = (s16)arg2;
    }
    return temp_r0;
}
