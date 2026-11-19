#include "global.h"

#include "overlay_27_ov27task_data.h"
#include "sprite.h"

extern u8 ov27_0225D3C6;

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_27.h"

void ov27_0225CD74(Ov27TaskData *data, u32 nItems) {
    Sprite_SetAnimCtrlSeq(data->cursor, (u8)(&ov27_0225D3C6)[(nItems - 2) * 0x18] + 2);
}
#else
// DS: the callers (overlay_27.h declares the count u32) pass it unmasked, but the definition takes a u8: with a u32 mwcc folds
// (n - 2) * 0x18 into the table base and the bytes differ. The PC form above has the one real type.
void ov27_0225CD74(Ov27TaskData *data, u8 nItems);

void ov27_0225CD74(Ov27TaskData *data, u8 nItems) {
    Sprite_SetAnimCtrlSeq(data->cursor, (u8)(&ov27_0225D3C6)[(nItems + -2) * 0x18] + 2);
    return;
}
#endif
