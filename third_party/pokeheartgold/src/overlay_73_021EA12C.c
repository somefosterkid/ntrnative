#include "global.h"
struct WifiBattleTowerAppState;

#include "sys_task.h"
#include "bg_window.h"
#include "fashion_case.h"
#include "friend_group.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "save_arrays.h"
#include "sprite.h"
#include "text.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void sub_0203A880(void);

void ov73_021EA12C(void);

void ov73_021EA134(struct WifiBattleTowerAppState *arg0, u32 arg1, s32 arg2);
void ov73_021EA15C(WifiBattleTowerAppState *arg0);
void ov73_021EA180(struct WifiBattleTowerAppState *arg0);
void ov73_021EA19C(WifiBattleTowerAppState *arg0, s32 arg1);
void ov73_021EA218(WifiBattleTowerAppState *arg0, s32 arg1, s32 arg2);
s32 ov73_021E795C(void);
void ov73_021EA25C(SaveData *arg0);
void *ov73_021E7960(SAV_FRIEND_GRP *);
void *ov73_021EA268(SaveData *arg0, enum HeapID arg1, u32 arg2);
s32 ov73_021E79F4(s32, s32, SAV_FRIEND_GRP *, const void **);
void ov73_021EA290(void *arg0);
s32 sub_0202B994(void);
void ov73_021EA2A8(SaveData *arg0);
void *ov73_021EA2B4(SaveData *arg0, enum HeapID arg1, u32 arg2);
s32 sub_0202BF80(u8, s32, SaveFashionData *, const void **);
void ov73_021EA2E0(void *arg0);
s32 ov73_021E7E24(UnkStruct_0202E474 *, s32, s32, const void **, const void **);
void ov73_021EA2FC(void *arg0);

void ov73_021EA12C(void) {
    sub_0203A880();
}

void ov73_021EA134(WifiBattleTowerAppState *arg0, u32 arg1, s32 flag) {
    if (arg1 <= 1U) {
        Sprite_SetDrawFlag(arg0->selectionArrows[0], flag);
        Sprite_SetDrawFlag(arg0->selectionArrows[1], flag);
    }
}

void ov73_021EA15C(WifiBattleTowerAppState *arg0) {
    if (((WaitingIcon *)arg0->waitDial) == NULL) {
        arg0->waitDial = WaitingIcon_New(&arg0->msgWindow, 1U);
    }
}

void ov73_021EA180(WifiBattleTowerAppState *arg0) {
    WaitingIcon *waitingIcon;

    waitingIcon = (WaitingIcon *)arg0->waitDial;
    if (waitingIcon != NULL) {
        sub_0200F450(waitingIcon);
        arg0->waitDial = NULL;
    }
}

void ov73_021EA19C(WifiBattleTowerAppState *arg0, s32 strno) {
    String *string;

    string = String_New(0x100U, HEAP_ID_150);
    ReadMsgDataIntoString(arg0->msgLoader3, strno, string);
    StringExpandPlaceholders(arg0->stringTemplate, arg0->errorString, string);
    FillWindowPixelBuffer(&arg0->popupWindow, 0xFU);
    DrawFrameAndWindow1(&arg0->popupWindow, 1, 0x1FU, 0xBU);
    arg0->textPrinterID = (s32)AddTextPrinterParameterized(&arg0->popupWindow, FONT_MESSAGE, arg0->errorString, 0U, 0U, 0U, NULL);
    String_Delete(string);
}

void ov73_021EA218(WifiBattleTowerAppState *arg0, s32 arg1, s32 num) {
    s32 v0;

    v0 = arg1;
    if (v0 == -1) {
        v0 = 0xB;
    }
    BufferIntegerAsString(arg0->stringTemplate, 0U, num, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    ClearFrameAndWindow2(&arg0->msgWindow, 1);
    ov73_021EA19C(arg0, v0);
}

void ov73_021EA25C(SaveData *saveData) {
    Save_FriendGroup_Get(saveData);
    ov73_021E795C();
}

void *ov73_021EA268(SaveData *saveData, enum HeapID heapID, u32 size) {
    SAV_FRIEND_GRP *v0;
    void *destp;

    v0 = Save_FriendGroup_Get(saveData);
    destp = Heap_AllocAtEnd(heapID, size);
    MIi_CpuCopyFast(ov73_021E7960(v0), destp, size);
    return destp;
}

void ov73_021EA290(void *arg0) {
    ov73_021E79F4(*(s32 *)((u8 *)arg0 + 8), *(s32 *)((u8 *)arg0 + 0xC), Save_FriendGroup_Get(*(SaveData **)((u8 *)arg0 + 4)), *(void **)((u8 *)arg0 + 0x10));
}

void ov73_021EA2A8(SaveData *saveData) {
    Save_FashionData_Get(saveData);
    sub_0202B994();
}

void *ov73_021EA2B4(SaveData *saveData, enum HeapID heapID, u32 size) {
    SaveFashionData *fashionData;
    void *destp;

    fashionData = Save_FashionData_Get(saveData);
    destp = Heap_AllocAtEnd(heapID, size);
    MIi_CpuCopyFast(sub_0202B9B8(fashionData, 0), destp, size);
    return destp;
}

void ov73_021EA2E0(void *arg0) {
    sub_0202BF80((u8) * (s32 *)((u8 *)arg0 + 8), *(s32 *)((u8 *)arg0 + 0xC), Save_FashionData_Get(*(SaveData **)((u8 *)arg0 + 4)), *(void **)((u8 *)arg0 + 0x10));
}

void ov73_021EA2FC(void *arg0) {
    ov73_021E7E24(sub_020270C4(*(SaveData **)((u8 *)arg0 + 4)), *(s32 *)((u8 *)arg0 + 8), *(s32 *)((u8 *)arg0 + 0xC), *(void **)((u8 *)arg0 + 0x10), *(void **)((u8 *)arg0 + 0x14));
}
