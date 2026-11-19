#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_90.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"

struct Sprite;

typedef struct Sprite Sprite;

void SpriteList_RenderAndAnimateSprites(Sprite *gfxElem);

typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;

typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;

void ov90_02259170(ChooseStarterCursor *cursor);

void ov90_022591D4(void *arg0);

void ov90_02259170(ChooseStarterCursor *cursor) {
    SpriteList_RenderAndAnimateSprites(cursor->sprite);
}

void OamManager_ApplyAndResetBuffers(void);

void ov90_0225917C(void);

void ov90_0225917C(void) {
    OamManager_ApplyAndResetBuffers();
}

void ov90_02259184(void *arg0, enum HeapID heapID) {
    *(MsgData **)arg0 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xEC, heapID);
    *((MessageFormat **)&((UnkStruct_ov114_0225D084 *)arg0)->unk_04) = MessageFormat_New_Custom(8U, 0x40U, heapID);
    ((UnkStruct_ov114_0225D084 *)arg0)->unk_08 = String_New(0x80U, heapID);
    ((UnkStruct_ov114_0225D084 *)arg0)->unk_0C = String_New(0x80U, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, heapID);
}

void ov90_022591D4(void *arg0) {
    String_Delete(((UnkStruct_ov114_0225D084 *)arg0)->unk_0C);
    String_Delete(((UnkStruct_ov114_0225D084 *)arg0)->unk_08);
    MessageFormat_Delete(((MessageFormat *)((UnkStruct_ov114_0225D084 *)arg0)->unk_04));
    DestroyMsgData(*(MsgData **)arg0);
}
