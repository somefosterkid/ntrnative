#include "global.h"

#include "sprite.h"
#include "unk_struct_ov70_02242e58.h"

struct Sprite;

extern u8 ov70_02245D66[];

extern u8 ov70_02245D67[];

void ov70_02242E58(UnkStruct_ov70_02242E58 *arg0);

void ov70_02238F9C(Sprite *sprite, int x, int y);

int ov70_02243CEC(UnkStruct_ov70_02242E58 *work);

int ov70_02243CEC(UnkStruct_ov70_02242E58 *work) {
    int n;

    work->unk_48 = 0;
    ov70_02242E58(work);
    Sprite_SetAnimCtrlSeq(work->unk_C, 0x31);
    n = work->unk_48 * 2;
    ov70_02238F9C(*((void * *)&work->unk_C), ov70_02245D66[n], ov70_02245D67[n]);
    work->unk_4C = 1;
    work->unk_50 = 0x1D;
    return -1;
}
