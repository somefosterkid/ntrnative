// Black's code at ov021 0x021c8608-0x021c881c. The name is the file name its first function's assert passes
// (tools/name_files.py); files are on both sides, and pokebw2 has pleasure_boat.c, of about its size.

#include "types.h"
#include "field/pleasure_boat.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "field/field_script_event.h"
#include "field/pleasure_boat_rooms.h"
#include "struct_decls.h"

PleasureBoat *func_ov021_021c8608(void) {
    PleasureBoat *boat;
    boat = GFL_HeapAllocate(5, sizeof(PleasureBoat), TRUE, "pleasure_boat.c", 0x31);
    boat->unk0 = 0;
    boat->OS_GetTick = 0;
    boat->period = 0;
    boat->infoCounts[0] = 0;
    boat->infoCounts[1] = 0;
    boat->infoCounts[2] = 0;
    boat->infoCounts[3] = 0;
    boat->unk1B4 = 0;
    GFL_OvlLoad(OVERLAY_ID(36));
    func_ov036_021f36c0(boat);
    GFL_OvlUnload(OVERLAY_ID(36));
    return boat;
}

void func_ov021_021c8660(s32 *param_1) {
    if (((param_1 != (s32 *)0x0) && (param_1[0x6d] == 0)) && (*param_1 != 0)) {
        GFL_SndSEPlay(0x6dd);
        param_1[2] = param_1[2] + 1;
        *param_1 = 0;
    }
}

GameEvent *func_ov021_021c868c(GameSystem *gsys, PleasureBoat *boat) {
    GameEvent *event;
    BOOL valid;
    if (boat == NULL) {
        return NULL;
    }
    if (boat->unk1B4 != 0) {
        return NULL;
    }
    boat->OS_GetTick++;
    switch (func_ov021_021c87e0(boat)) {
    case 1:
        boat->unk0 = 1;
        valid = FALSE;
        break;
    case 2:
        event = EventScriptCall_Create(gsys, 1, NULL, 0x8015);
        valid = TRUE;
        break;
    default:
        valid = FALSE;
        break;
    }
    if (!valid) {
        event = NULL;
    }
    return event;
}


void func_ov021_021c86e8(PleasureBoat *boat, u16 steps, BOOL stopBefore) {
    s32 time = boat->OS_GetTick % 0x546;
    time += steps * 30;
    if (time >= 0x546) {
        if (stopBefore) {
            boat->OS_GetTick = (boat->period + 1) * 0x546 - 1;
        } else {
            boat->OS_GetTick = (boat->period + 1) * 0x546;
            boat->period++;
        }
    } else {
        boat->OS_GetTick += steps * 30;
    }
}

u32 func_ov021_021c8738(PleasureBoat *boat, u32 info) {
    u32 result;
    s32 i;
    switch (info) {
    case 0:
        result = boat->infoCounts[0];
        break;
    case 1:
        result = boat->infoCounts[1];
        break;
    case 2:
        result = boat->infoCounts[2];
        break;
    case 3:
        result = boat->infoCounts[3];
        break;
    case 4:
        result = boat->trainerCount;
        break;
    case 5:
        result = 0;
        for (i = 0; i < PLEASURE_BOAT_ROOM_COUNT; i++) {
            if (boat->rooms[i].defeated) {
                result++;
            }
        }
        break;
    default:
        result = 0;
        break;
    }
    return result;
}



u32 func_ov021_021c87a0(PleasureBoat *boat, u32 room) {
    return boat->rooms[room].kind;
}

u32 func_ov021_021c87ac(PleasureBoat *boat, u32 room) {
    return boat->rooms[room].unk4;
}

u32 func_ov021_021c87b8(PleasureBoat *boat, u32 room, u32 index) {
    return boat->rooms[room].values[index];
}

BOOL func_ov021_021c87c8(PleasureBoat *boat, u32 room) {
    return boat->rooms[room].defeated;
}

void func_ov021_021c87d4(PleasureBoat *boat, u32 trainer, BOOL defeated) {
    boat->rooms[trainer].defeated = defeated;
}

u32 func_ov021_021c87e0(PleasureBoat *boat) {
    if (boat->OS_GetTick >= 0x1518 && boat->period >= 4) {
        return 2;
    }
    return boat->OS_GetTick % 0x546 == 0;
}


void func_ov021_021c8810(PleasureBoat *boat) {
    boat->unk1B4 = 1;
}
