#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyVipRecord;

struct WiFiHistory;

struct WiFiList;

typedef enum {
    UnkEnum_ov66_02233454_00,
    UnkEnum_ov66_02233454_01,
    UnkEnum_ov66_02233454_02,
    UnkEnum_ov66_02233454_03,
    UnkEnum_ov66_02233454_04,
    UnkEnum_ov66_02233454_05,
    UnkEnum_ov66_02233454_06,
    UnkEnum_ov66_02233454_07,
    UnkEnum_ov66_02233454_08,
    UnkEnum_ov66_02233454_09,
    UnkEnum_ov66_02233454_10,
    UnkEnum_ov66_02233454_11,
} UnkEnum_ov66_02233454;

typedef struct WiFiHistory WiFiHistory;

s32 ov45_0222F544(UnkEnum_ov66_02233454 param0);

s32 ov45_0222F544(UnkEnum_ov66_02233454 param0) {
    s32 v0;

    ((_022577C0 != 0L) ? (void)0 : GF_AssertFail());
    ((param0 < UnkEnum_ov66_02233454_11) ? (void)0 : GF_AssertFail());

    switch (param0) {
    case UnkEnum_ov66_02233454_00:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.questionSerialNo;
        break;
    case UnkEnum_ov66_02233454_01:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.questionNo;
        break;
    case UnkEnum_ov66_02233454_02:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.isSpecialWeek;
        break;
    case UnkEnum_ov66_02233454_03:
        if (_022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.questionNo >= 1000) {
            v0 = 1;
        } else {
            v0 = 0;
        }
        break;
    case UnkEnum_ov66_02233454_04:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.questionSerialNo;
        break;
    case UnkEnum_ov66_02233454_05:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.questionNo;
        break;
    case UnkEnum_ov66_02233454_06:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.isSpecialWeek;
        break;
    case UnkEnum_ov66_02233454_07:
        if (_022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.questionNo >= 1000) {
            v0 = 1;
        } else {
            v0 = 0;
        }
        break;
    case UnkEnum_ov66_02233454_08:
    case UnkEnum_ov66_02233454_09:
    case UnkEnum_ov66_02233454_10:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.lastMultiResult[param0 - UnkEnum_ov66_02233454_08];
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        v0 = 0;
        break;
    }

    return v0;
}

typedef enum {
    UnkEnum_ov66_02233538_00,
    UnkEnum_ov66_02233538_01,
    UnkEnum_ov66_02233538_02,
    UnkEnum_ov66_02233538_03,
    UnkEnum_ov66_02233538_04,
    UnkEnum_ov66_02233538_05,
    UnkEnum_ov66_02233538_06,
    UnkEnum_ov66_02233538_07,
    UnkEnum_ov66_02233538_08,
} UnkEnum_ov66_02233538;

u16 *ov45_0222F628(UnkEnum_ov66_02233538 param0);

u16 *ov45_0222F628(UnkEnum_ov66_02233538 param0) {
    u16 *v0;

    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(param0 < UnkEnum_ov66_02233538_08);

    switch (param0) {
    case UnkEnum_ov66_02233538_00:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.questionSentence;
        break;
    case UnkEnum_ov66_02233538_01:
    case UnkEnum_ov66_02233538_02:
    case UnkEnum_ov66_02233538_03:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.currentQuestionnaireRecord.answer[param0 - UnkEnum_ov66_02233538_01];
        break;
    case UnkEnum_ov66_02233538_04:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.questionSentence;
        break;
    case UnkEnum_ov66_02233538_05:
    case UnkEnum_ov66_02233538_06:
    case UnkEnum_ov66_02233538_07:
        v0 = _022577C0->unk_6A8.lobbyQuestionnaire.lastQuestionnaireRecord.answer[param0 - UnkEnum_ov66_02233538_05];
        break;
    default:
        GF_ASSERT(FALSE);
        v0 = NULL;
        break;
    }

    return v0;
}
