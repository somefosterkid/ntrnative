#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "system.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

struct DpadMenuBox;

void ov83_02242AC0(void *data, u32 x, u32 y, int a3);

void ov83_02242AE0(void *data, int a1);

extern const int ov83_02248570[7];

extern const DpadMenuBox ov83_02248610[7];

void ov83_02247C88(void *data, int newTarget, int prevTarget);

void ov83_02247C88(void *data, int newTarget, int prevTarget) {
    ov83_02242AC0(data, ov83_02248610[newTarget].left, ov83_02248610[newTarget].top, ov83_02248570[newTarget]);
    ov83_02242AE0(data, newTarget);
}
