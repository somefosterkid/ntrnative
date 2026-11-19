#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"
#include "ppw_lobby.h"

struct PPW_LobbyQuestionnaire;

struct PPW_LobbyQuestionnaireRecord;

struct PPW_LobbyRecruitInfo;

struct PPW_LobbySystemProfile;

struct PPW_LobbyTimeInfo;

struct PPW_LobbyVipRecord;

struct SaveData;

struct WiFiHistory;

struct WiFiList;

typedef struct SaveData SaveData;

typedef struct WiFiList WiFiList;

typedef struct WiFiHistory WiFiHistory;

void *Heap_Alloc(u32 heapID, u32 size);

WiFiHistory *Save_EasyChat_Get(SaveData *saveData);

WiFiList *sub_0202C6F4(SaveData *saveData);

void ov45_0222F848(UnkStruct_ov66_022336C4 *unused, u32 param1, u32 heapID);

void ov45_0222F9B8(UnkStruct_ov66_022336C4 *param0, u32 param1, u32 param2, u32 heapID);

void ov45_02230144(UnkStruct_ov66_022336C4 *param0);

void ov45_022303E4(UnkStruct_ov66_022336C4_sub5 *param0, s16 param1, u32 heapID);

void ov45_0222E5D4(u32 heapID, SaveData *saveData, u32 param2, const UnkStruct_ov66_022324F0 *param3, void *param4);

void *Save_WiFiHistory_Get(SaveData *saveData);

void ov45_0222E5D4(u32 heapID, SaveData *saveData, u32 param2, const UnkStruct_ov66_022324F0 *param3, void *param4) {
    ((_022577C0 == 0L) ? (void)0 : GF_AssertFail());
    ((param2 < 300) ? (void)0 : GF_AssertFail());

    _022577C0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov66_022336C4));
    memset(_022577C0, 0, sizeof(UnkStruct_ov66_022336C4));

    _022577C0->saveData = saveData;
    _022577C0->wiFiList = sub_0202C6F4(saveData);
    _022577C0->wiFiHistory = Save_WiFiHistory_Get(saveData);
    _022577C0->unk_0C = *param3;
    _022577C0->unk_20 = param4;

    ov45_0222F848(_022577C0, param2, heapID);
    ov45_0222F9B8(_022577C0, 20, 8, heapID);
    ov45_02230144(_022577C0);
    ov45_022303E4(&_022577C0->unk_6A0, 128, heapID);
    _022577C0->result = PPW_LOBBY_RESULT_SUCCESS;
}
