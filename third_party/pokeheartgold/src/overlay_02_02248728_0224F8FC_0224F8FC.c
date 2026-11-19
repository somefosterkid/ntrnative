#include "global.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "pm_string.h"
#include "render_window.h"
#include "system.h"
#include "overlay_02_follow_app.h"

s32 FollowMon_TryPrintInteractionMessage(void *, void *, void *);

s32 ov02_0224FB44(void *, void *);

s32 ov02_0224FB54(void *, void *, void *);

s32 ov02_0224FC74(void *, void *);

s32 ov02_02250004(void *, void *, s32);

s32 ov02_02250504();

typedef struct {
    u8 lo : 4;
    u8 hi : 4;
} Ov02Nibbles;

s32 ov02_0224F8FC(void *arg0, void *arg1);

s32 ov02_0224F8FC(void *arg0, void *arg1) {
    s32 temp_r2_2;
    u16 temp_r7_2;
    u32 temp_r2;
    u8 temp_r2_3;
    u8 temp_r3;
    u8 temp_r3_2;
    void *temp_r6;
    void *temp_r7;
    temp_r3 = *((u8 *)&((Ov02FollowApp *)arg1)->unk_800[108]);
    temp_r2 = ((u32)(temp_r3 << 0x1C)) >> 0x1C;
    if (temp_r2 >= 5U) {
        ov02_02250504();
        if ((((Ov02FollowApp *)arg1)->unk_80C) != 0) {
            return 2;
        }
        if ((*((u8 *)&((Ov02FollowApp *)arg1)->unk_810[6])) != 0) {
            return 3;
        }
        if ((*((u8 *)&((Ov02FollowApp *)arg1)->unk_810[7])) != 0) {
            return 4;
        }
        return 1;
    }
    temp_r7 = arg1 + 0x7E4;
    temp_r2_2 = temp_r2 * 8;
    temp_r6 = temp_r7 + temp_r2_2;
    temp_r7_2 = *((u16 *)(temp_r7 + temp_r2_2));
    if (temp_r7_2 == 0xFFFF) {
        ov02_02250504();
        if ((((Ov02FollowApp *)arg1)->unk_80C) != 0) {
            return 2;
        }
        if ((*((u8 *)&((Ov02FollowApp *)arg1)->unk_810[6])) != 0) {
            return 3;
        }
        if ((*((u8 *)&((Ov02FollowApp *)arg1)->unk_810[7])) != 0) {
            return 4;
        }
        return 1;
    }
    temp_r2_3 = *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]);
    switch ((u32)temp_r2_3) {
    case 0:
        if (ov02_0224FB44(arg1, temp_r6) != 0) {
            MapObject_UnpauseMovement(((FieldSystem *)arg0)->followMon.mapObject);
            *((s8 *)&((Ov02FollowApp *)arg1)->unk_810[88]) = 0;
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 5U;
            break;
        }
    case 1:
        if (ov02_0224FB54(arg0, arg1, temp_r6) != 0) {
            MapObject_UnpauseMovement(((FieldSystem *)arg0)->followMon.mapObject);
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 2U;
            break;
        }
    case 2:
        if (FollowMon_TryPrintInteractionMessage(arg0, arg1, temp_r6) != 0) {
            MapObject_PauseMovement(((FieldSystem *)arg0)->followMon.mapObject);
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 6U;
            break;
        }
    case 3:
        if (ov02_0224FC74(arg1, temp_r6) != 0) {
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 7U;
            break;
        }
    case 4:
        temp_r3_2 = *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[92]);
        *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[92]) = (u8)((temp_r3_2 & (~0xF)) | (0xF & ((u8)((((u32)(temp_r3_2 << 0x1C)) >> 0x1C) + 1))));
        *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 0U;
        break;

    case 5:
        if (ov02_02250004(arg0, arg1, temp_r7_2) != 0) {
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 1U;
        }
        break;

    case 6:
        if (DialogBox_IsPrintFinished((u8)(((Ov02FollowApp *)arg1)->unk_86E)) == 1) {
            if (((Ov02Nibbles *)(((u8 *)arg1) + 0x86C))->hi > ((Ov02Nibbles *)(((u8 *)arg1) + 0x86C))->lo + 1) {
                if (3 & gSystem.newKeys) {
                    ClearFrameAndWindow2((Window *)arg1, 0);
                    RemoveWindow((Window *)arg1);
                    String_Delete(((Ov02FollowApp *)arg1)->unk_10);
                    *((u8 *)(((u8 *)arg0) + 0xD2)) &= ~0x40;
                    *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 3U;
                }
            } else if ((((Ov02FollowApp *)arg1)->unk_80C) != 0) {
                *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 3U;
            } else if (3 & gSystem.newKeys) {
                ClearFrameAndWindow2((Window *)arg1, 0);
                RemoveWindow((Window *)arg1);
                String_Delete(((Ov02FollowApp *)arg1)->unk_10);
                *((u8 *)(((u8 *)arg0) + 0xD2)) &= ~0x40;
                *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 3U;
            }
        }
        break;

    case 7:
        if (*((u8 *)&((Ov02FollowApp *)arg1)->unk_810[93]) >= *((u8 *)(((u8 *)temp_r6) + 7))) {
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[92]) = (u8)((temp_r3 & (~0xF)) | (0xF & ((u8)(temp_r2 + 1))));
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[89]) = 0U;
        } else {
            *((u8 *)&((Ov02FollowApp *)arg1)->unk_810[93]) += 1;
        }
        break;
    }

    return 0;
}
