#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "text.h"
#include "unk_0200A090.h"
#include "error_handling.h"
#include "list_menu_2d.h"

struct Menu;

typedef struct Menu Menu;

typedef struct {
    u32 unk_00;
    s32 unk_04;
    Window unk_08;
    String *unk_18;
    void *unk_1C;
    Menu *unk_20;
} UnkStruct_ov68_0225D128;

void ov47_022593A0(UnkStruct_ov68_0225D128 *param0);

void ov47_022593CC(UnkStruct_ov68_0225D128 *param0);

extern WindowTemplate _02259E38;
void ov47_02259404(void *arg0, BgConfig **arg1, enum HeapID arg2);

void ov47_022593CC(UnkStruct_ov68_0225D128 *param0) {
    if (TextPrinterCheckActive(param0->unk_00)) {
        RemoveTextPrinter(param0->unk_00);
    }

    ov47_022593A0(param0);

    ClearFrameAndWindow2(&param0->unk_08, 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_08);
}

void ov47_02259404(void *arg0, BgConfig **arg1, enum HeapID heapID) {
    if (*(struct ListMenu2D **)((u8 *)arg0 + 0x20) != NULL) {
        GF_AssertFail();
    }
    *(struct ListMenu2D **)((u8 *)arg0 + 0x20) = Std_CreateYesNoMenu(*arg1, &_02259E38, 0x1FU, 6U, heapID);
}
