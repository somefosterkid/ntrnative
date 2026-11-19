#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "list_menu_cursor.h"
#include "message_format.h"
#include "msgdata.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "task.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02058AEC.h"

BOOL sub_02058D4C(TaskManager *param0);

void sub_02059538(FieldSystem *fieldSystem, void (*param1)(s32, Party *));

void sub_02059538(FieldSystem *fieldSystem, void (*param1)(s32, Party *)) {
    Party *temp_r0_4;
    u32 temp_r0_3;
    u8 temp_r0_2;
    void *temp_r0;

    if (fieldSystem->taskman != NULL) {
        return;
    }
    temp_r0 = Heap_AllocAtEnd(HEAP_ID_FIELD2, 0x8CU);
    MI_CpuFill8(temp_r0, 0U, 0x8CU);
    *(s8 *)((u8 *)temp_r0 + 0x43) = 5;
    *(FieldSystem **)((u8 *)temp_r0 + 0x24) = fieldSystem;
    *(void (**)(s32, Party *))((u8 *)temp_r0 + 8) = param1;
    *(MessageFormat **)((u8 *)temp_r0 + 0x28) = MessageFormat_New(HEAP_ID_FIELD2);
    *(MsgData **)((u8 *)temp_r0 + 0x2C) = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xE, HEAP_ID_FIELD2);
    *(String **)((u8 *)temp_r0 + 0xC) = String_New(0xC8U, HEAP_ID_FIELD2);
    *(String **)((u8 *)temp_r0 + 0x10) = String_New(0xC8U, HEAP_ID_FIELD2);
    InitWindow(temp_r0 + 0x14);
    InitWindow(temp_r0 + 0x54);
    InitWindow(temp_r0 + 0x64);
    *(struct ListMenuCursor **)((u8 *)temp_r0 + 0x78) = ListMenuCursorNew(HEAP_ID_FIELD2);
    *(u8 *)((u8 *)temp_r0 + 0x88) = sub_0203993C();
    *(void **)((u8 *)temp_r0 + 0x4C) = NULL;
    *(void **)((u8 *)temp_r0 + 0x48) = NULL;
    *(Party **)((u8 *)temp_r0 + 0x50) = NULL;
    *(s8 *)((u8 *)temp_r0 + 0x89) = 0;
    *(u16 *)((u8 *)temp_r0 + 0x86) = sub_0203769C();
    *(PlayerProfile **)((u8 *)temp_r0 + 0x74) = sub_02034818(1 ^ *(u16 *)((u8 *)temp_r0 + 0x86));
    temp_r0_2 = *(u8 *)((u8 *)temp_r0 + 0x88);
    switch (temp_r0_2) {
    case 3:
        temp_r0_3 = sub_02059B08();
        *(void **)((u8 *)temp_r0 + 0x4C) = Heap_AllocAtEnd(HEAP_ID_FIELD2, temp_r0_3);
        *(void **)((u8 *)temp_r0 + 0x48) = Heap_AllocAtEnd(HEAP_ID_FIELD2, temp_r0_3);
        temp_r0_4 = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
        *(Party **)((u8 *)temp_r0 + 0x50) = temp_r0_4;
        Party_InitWithMaxSize(temp_r0_4, 3);
        *(s8 *)((u8 *)temp_r0 + 0x44) = 5;
        *(s32 *)((u8 *)temp_r0 + 0x34) = 0x17;
        break;
    case 4:
        *(s32 *)((u8 *)temp_r0 + 0x34) = 9;
        break;
    default:
        if ((*(FieldSystem **)((u8 *)temp_r0 + 0x24))->linkBattleRuleset != NULL) {
            *(s32 *)((u8 *)temp_r0 + 0x34) = 9;
        } else {
            *(s32 *)((u8 *)temp_r0 + 0x34) = 0;
        }
        break;
    }
    FieldSystem_CreateTask(fieldSystem, sub_02058D4C, temp_r0);
}
