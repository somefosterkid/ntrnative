#include "global.h"

#include "overlay_manager.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "main.h"

void ov74_02229B68(void *);

s32 ov74_02229BC0(s32 *, s16);

OverlayManager *ov74_0222CD7C(void);

extern s32 ov74_02229C44;

void ov74_02229D1C(void *arg0, s8 arg1);

void ov74_02229D1C(void *arg0, s8 arg1) {
    if (sub_02037474() == 0) {
        sub_02038148(((UnkStruct_02111868_sub *)OverlayManager_GetArgs(ov74_0222CD7C()))->saveData, arg1);
        ov74_02229B68(arg0);
        ov74_02229BC0(&ov74_02229C44, 0);
    }
}

extern s32 ov74_02229C60;

extern void *ov74_0223D0A4;

void ov74_02229D50(s8 arg0);

void ov74_02229D50(s8 arg0) {
    *(s8 *)((u8 *)ov74_0223D0A4 + 0x27) = arg0;
    ov74_02229BC0(&ov74_02229C60, 0);
}
