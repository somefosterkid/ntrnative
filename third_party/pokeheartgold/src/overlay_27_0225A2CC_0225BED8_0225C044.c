#include "global.h"

#include "bug_contest_internal.h"
#include "catching_show.h"
#include "error_handling.h"
#include "overlay_27.h"
#include "save_local_field_data.h"
#include "sprite.h"
#include "unk_0206D494.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225BF84(Ov27Task *, s32, s32, s32);

s32 ov27_0225C01C(Ov27Task *);

void ov27_0225C044(Ov27Task *arg0) {
    ov27_0225BF84(arg0, 0xE, 0x13, *LocalFieldData_GetSafariBallsCounter(Save_LocalFieldData_Get(arg0->fieldSystem->saveData)));
    ov27_0225C01C(arg0);
}

void ov27_0225C06C(Ov27Task *arg0) {
    ov27_0225BF84(arg0, 0xE, 0xE, (u16)FieldSystem_GetParkBallCount(arg0->fieldSystem));
}

s32 ov27_0225BFCC(Ov27Task *, BugContest *);

void ov27_0225C088(Ov27Task *arg0) {
    BugContest *contest;

    contest = FieldSystem_BugContest_Get(arg0->fieldSystem);
    if (contest == NULL) {
        GF_AssertFail();
    }
    ov27_0225BF84(arg0, 0xE, 0xE, *BugContest_GetSportBallsAddr(contest));
    if (contest->caught_poke) {
        ov27_0225BFCC(arg0, contest);
    }
    Sprite_SetDrawFlag(arg0->unk_390[6], contest->caught_poke);
    arg0->unk_514[6] = 0;
    arg0->unk_470[6].unk_0 = 0;
}
