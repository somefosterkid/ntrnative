#include "global.h"

#include "field_types_def.h"
#include "overlay_01.h"
#include "overlay_34.h"
#include "player_data.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_struct_0203330c.h"

WMBssDesc *sub_02035754(int param0);
PlayerProfile *sub_02035784(void);

extern const TouchscreenHitbox ov34_0225E730[];

s32 ov34_0225DE94(Ov34Work *work) {
    s32 hit;
    s32 trigger;
    BOOL canExit;
    WMBssDesc *bssDesc;
    PlayerProfile *profile;
    s32 i;
    s32 index;
    UnkStruct_0203330C *beacon;

    hit = TouchscreenHitbox_FindRectAtTouchHeld(ov34_0225E730);
    trigger = ov34_0225E5D4(work);
    if (hit != -1) {
        switch (hit) {
        case 0:
            ov34_0225E5EC(work, hit);
            if (trigger == 1) {
                if (work->scroll != 0) {
                    PlaySE(0x5e5);
                    work->scroll = work->scroll + -1;
                }
                work->selectedRow = hit + -2;
            }
            break;
        case 1:
            ov34_0225E5EC(work, hit);
            if (trigger == 1) {
                if ((s32)(u32)work->scroll < (s32)(work->count - 3)) {
                    PlaySE(0x5e5);
                    work->scroll = work->scroll + 1;
                }
                work->selectedRow = hit + -2;
            }
            break;
        case 5:
            work->selectedRow = hit + -2;
            break;
        case 6:
            if ((gSystem.touchNew != 0) && (canExit = ov01_021F6B10(work->fieldSystem), canExit == 1)) {
                PlaySE(0x5fc);
                work->state = 3;
            }
            break;
        default:
            if (gSystem.touchNew != 0) {
                if (work->chatLog->count >= hit + -1) {
                    index = ov34_0225DC0C(work->chatLog->start, (u32)work->scroll + hit + -2);
                    i = 0;
                    do {
                        bssDesc = sub_02035754(i);
                        if (bssDesc != NULL) {
                            beacon = (UnkStruct_0203330C *)&bssDesc->gameInfo.userGameInfo;
                            if ((work->unk4->entries[i].unk1 == 2) && (work->chatLog->entries[index].id == beacon->trainerId)) {
                                PlaySE(0x5e5);
                                work->unk4->entries[i].unk3 = 1;
                                break;
                            }
                        }
                        i = i + 1;
                    } while (i < 10);
                    profile = sub_02035784();
                    if (work->chatLog->entries[index].id == PlayerProfile_GetTrainerID(profile)) {
                        PlaySE(0x5e5);
                        work->unk4->entries[50].unk3 = 1;
                    }
                }
                work->selectedRow = hit + -2;
            }
            break;
        }
    }
    return hit;
}
