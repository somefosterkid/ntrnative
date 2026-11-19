#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "mail_message.h"
#include "mail_misc.h"
#include "message_format.h"
#include "save_misc_data.h"
#include "task.h"

typedef struct UnkStruct_sub_020961D8_Heap_Alloc {
    FieldSystem *unk_0;
    MessageFormat *unk_4;
    u8 unk_8;
    u8 unk_9[0x7];
    EasyChatArgs *unk_10;
    SAVE_MISC_DATA *unk_14;
    s32 unk_18;
    u8 unk_1C[0x4];
    u16 *unk_20;
} UnkStruct_sub_020961D8_Heap_Alloc;

BOOL sub_02096260(TaskManager *param0);

void sub_020961D8(TaskManager *taskManager, u16 *var_p);

void sub_020961D8(TaskManager *taskManager, u16 *var_p) {
    FieldSystem *temp_r5;
    UnkStruct_sub_020961D8_Heap_Alloc *temp_r0;

    temp_r5 = TaskManager_GetFieldSystem(taskManager);
    temp_r0 = Heap_Alloc(HEAP_ID_FIELD3, sizeof(UnkStruct_sub_020961D8_Heap_Alloc));
    temp_r0->unk_0 = temp_r5;
    temp_r0->unk_4 = MessageFormat_New(HEAP_ID_FIELD3);
    temp_r0->unk_10 = EasyChat_CreateArgs(2U, 0U, (temp_r0->unk_0)->saveData, &temp_r5->menuInputState, HEAP_ID_FIELD3);
    temp_r0->unk_14 = Save_Misc_Get(temp_r5->saveData);
    temp_r0->unk_20 = var_p;
    MailMsg_Init_WithBank((void *)&temp_r0->unk_8, 4U);
    SaveMisc_GetBattleGreeting(temp_r0->unk_14, (void *)&temp_r0->unk_8);
    sub_02090D40(temp_r0->unk_10);
    temp_r0->unk_18 = 0;
    TaskManager_Call(taskManager, sub_02096260, temp_r0);
}
