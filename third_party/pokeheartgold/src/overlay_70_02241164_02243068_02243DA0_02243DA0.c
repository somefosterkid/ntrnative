#include "global.h"

#include "sprite.h"
#include "unk_struct_ov70_02242e58.h"

struct Sprite;

extern u8 ov70_02245D80[];

extern u8 ov70_02245D81[];

void ov70_02242EE4(UnkStruct_ov70_02242E58 *arg0);

void ov70_02238F9C(Sprite *sprite, int x, int y);

int ov70_02243DA0(UnkStruct_ov70_02242E58 *work);

int ov70_02243DA0(UnkStruct_ov70_02242E58 *work) {
    int n;

    work->unk_48 = 0;
    ov70_02242EE4(work);
    Sprite_SetAnimCtrlSeq(work->unk_C, 0x31);
    n = work->unk_48 * 2;
    ov70_02238F9C(work->unk_C, ov70_02245D80[n], ov70_02245D81[n]);
    work->unk_4C = 1;
    work->unk_50 = 0x20;
    return -1;
}
