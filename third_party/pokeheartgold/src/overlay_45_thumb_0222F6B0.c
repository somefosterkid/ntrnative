#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"
#include "overlay_00_thumb.h"
#include "overlay_45_thumb.h"

u8 ov45_0222F6B0(s32 arg0, u32 arg1);

BOOL ov45_0222F74C(PPW_LOBBY_RESULT param0);

u8 ov45_0222F6B0(s32 arg0, u32 arg1) {
    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    if (arg0 >= 2) {
        GF_AssertFail();
    }
    if (arg1 >= 0xCU) {
        GF_AssertFail();
    }
    if (arg0 >= 2) {
        return 0U;
    }
    if (arg1 >= 0xCU) {
        return 0U;
    }
    if (arg0 == 0) {
        return _022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.multiLanguageSummarizeFlags[arg1];
    }
    return _022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.multiLanguageSummarizeFlags[arg1];
}

s32 ov00_021E5CD0(void *, void *);

void ov45_0222F70C(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    ov00_021E5CD0(0, 0);
    ov00_021E5CA0(NULL, NULL);
    _022577C0->gameKind = 3;
    _022577C0->unk_69E = 0;
    _022577C0->unk_69F = 0;
}

BOOL ov45_0222F74C(PPW_LOBBY_RESULT param0) {
    BOOL v0 = 1;

    switch (param0) {
    case PPW_LOBBY_RESULT_SUCCESS:
        break;
    case PPW_LOBBY_RESULT_ERROR_ALLOC:
    case PPW_LOBBY_RESULT_ERROR_SESSION:
    case PPW_LOBBY_RESULT_ERROR_CHANNEL:
    case PPW_LOBBY_RESULT_ERROR_STATE:
        v0 = 0;
        break;
    case PPW_LOBBY_RESULT_ERROR_PARAM:
    case PPW_LOBBY_RESULT_ERROR_NODATA:
        ((0) ? (void)0 : GF_AssertFail());
        v0 = 0;
        break;
    case PPW_LOBBY_RESULT_ERROR_CONDITION:
        v0 = 0;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        v0 = 0;
        break;
    }

    return v0;
}
