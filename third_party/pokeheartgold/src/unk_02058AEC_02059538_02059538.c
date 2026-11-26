#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "link_party_select.h"
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
    LinkPartySelect *env;
    u32 size;

    if (fieldSystem->taskman != NULL) {
        return;
    }
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(LinkPartySelect));
    MI_CpuFill8(env, 0, sizeof(LinkPartySelect));
    env->unk_43 = 5;
    env->fieldSystem = fieldSystem;
    env->callback = param1;
    env->msgFormat = MessageFormat_New(HEAP_ID_FIELD2);
    env->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xE, HEAP_ID_FIELD2);
    env->unk_0C = String_New(0xC8U, HEAP_ID_FIELD2);
    env->unk_10 = String_New(0xC8U, HEAP_ID_FIELD2);
    InitWindow(&env->unk_14);
    InitWindow(&env->unk_54);
    InitWindow(&env->unk_64);
    env->unk_78 = ListMenuCursorNew(HEAP_ID_FIELD2);
    env->unk_88 = sub_0203993C();
    env->unk_4C = NULL;
    env->unk_48 = NULL;
    env->party = NULL;
    env->unk_89 = 0;
    env->unk_86 = sub_0203769C();
    env->unk_74 = sub_02034818(1 ^ env->unk_86);
    switch (env->unk_88) {
    case 3:
        size = sub_02059B08();
        env->unk_4C = Heap_AllocAtEnd(HEAP_ID_FIELD2, size);
        env->unk_48 = Heap_AllocAtEnd(HEAP_ID_FIELD2, size);
        env->party = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
        Party_InitWithMaxSize(env->party, 3);
        env->unk_44 = 5;
        env->state = 0x17;
        break;
    case 4:
        env->state = 9;
        break;
    default:
        if (env->fieldSystem->linkBattleRuleset != NULL) {
            env->state = 9;
        } else {
            env->state = 0;
        }
        break;
    }
    FieldSystem_CreateTask(fieldSystem, sub_02058D4C, env);
}
