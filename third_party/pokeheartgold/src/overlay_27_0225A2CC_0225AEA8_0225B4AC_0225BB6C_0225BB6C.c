#include "global.h"
struct Ov27Task;

#include "constants/heap.h"

#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_27_ov27task.h"

// A start-menu label: msgId is the label's message in msg 196 (the top bit is set in every entry).
typedef struct Ov27IconLabel {
    u16 unk_0;
    u16 msgId : 15;
    u16 unk_2_15 : 1;
} Ov27IconLabel;

extern const s32 ov27_0225CED8[];

extern const Ov27IconLabel ov27_0225CF94[];

extern const u8 ov27_0225CFC8[][8];

void ov27_0225BB6C(Ov27Task *task, u32 unused);

void ov27_0225BB6C(Ov27Task *task, u32 unused) {
    s32 i;
    u8 label;

    BufferPlayersName(task->unk_4AC, 0, Save_PlayerData_GetProfile(task->fieldSystem->saveData));
    for (i = 0; i < 7; i++) {
        label = ov27_0225CFC8[task->unk_51C_1][i];
        if (label != 13) {
            task->unk_470[i].unk_4 = ReadMsgData_ExpandPlaceholders(task->unk_4AC, task->unk_4A8, ov27_0225CF94[label].msgId, HEAP_ID_8);
        }
    }
    for (i = 0; i < 5; i++) {
        task->unk_4B0[i] = NewString_ReadMsgData(task->unk_4A8, ov27_0225CED8[i]);
    }
    task->unk_4B0[5] = ReadMsgData_ExpandPlaceholders(task->unk_4AC, task->unk_4A8, 12, HEAP_ID_8);
    task->unk_4B0[6] = ReadMsgData_ExpandPlaceholders(task->unk_4AC, task->unk_4A8, 13, HEAP_ID_8);
}
