#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"

#include "unk_0203A3B0.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_format.h"
#include "msgdata.h"

struct BgConfig;

struct BoxPokemon;

struct FrontierPokemon;

struct G2dRenderer;

struct Menu;

struct MsgData;

struct NARC;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct SysInfo;

struct WifiBattleTowerAppState;

struct WifiBattleTowerDownloadBuffer;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerMatchCandidate;

struct WifiBattleTowerRecord;

struct WifiPlayerProfile;

struct Window;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;


typedef u16 charcode_t;

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct Menu Menu;

enum ErrorCleanupSubStep {
    ERROR_CLEANUP_SUBSTEP_DISPLAY_ERROR,
    ERROR_CLEANUP_SUBSTEP_DISCONNECT,
    ERROR_CLEANUP_SUBSTEP_DISPLAY_RETURN,
    ERROR_CLEANUP_SUBSTEP_WAIT_RETURN,
};

int ov73_021E8440(void);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

enum WifiBattleTowerLoopState {
    BT_LOOP_STATE_WAIT_FOR_WIRELESS_DRIVER,
    BT_LOOP_STATE_INIT,
    BT_LOOP_STATE_WAIT_FADE,
    BT_LOOP_STATE_MAIN,
    BT_LOOP_STATE_FINISH,
    BT_LOOP_STATE_EXIT,
};
void SysTask_Destroy(SysTask *task);
void ov73_021E870C(BgConfig *bgConfig);
void ov73_021E8B20(WifiBattleTowerAppState *appState);
void ov73_021E8BBC(WifiBattleTowerAppState *appState);
int ov73_021E85DC(WifiBattleTowerAppState *appState, int unused);

static inline void inline_ov61_0222C0F8(UnkStruct_ov61_0222C3B0 *param0) {
    return;
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

int WifiBattleTower_State_InitialPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_InitialYesNoMenu(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ConnectedYesNoMenu(WifiBattleTowerAppState *appState);

int ov73_021E8D0C(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitInetConnect(WifiBattleTowerAppState *appState);

int ov73_021E8DB8(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitNasLogin(WifiBattleTowerAppState *appState);

int ov73_021E8E7C(WifiBattleTowerAppState *appState);

int ov73_021E8EBC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_Idle(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_EndSession(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DisconnectYesNo(WifiBattleTowerAppState *appState);

int ov73_021E9EAC(WifiBattleTowerAppState *appState);

int ov73_021E9F00(WifiBattleTowerAppState *appState);

int ov73_021E9ECC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ShowError(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ErrorCleanup(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitInfo(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ConnectedPrompt(WifiBattleTowerAppState *appState);


int ov73_021E9FC8(WifiBattleTowerAppState *appState);

int ov73_021E8FB0(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitSetProfile(WifiBattleTowerAppState *appState);

int ov73_021E9120(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitRoomCount(WifiBattleTowerAppState *appState);

int ov73_021E91F4(WifiBattleTowerAppState *appState);

int ov73_021E9224(WifiBattleTowerAppState *appState);

int ov73_021E926C(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitYesNo(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DownloadPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_RequestDownload(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitDownload(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DownloadCompletePrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_UploadPrompt(WifiBattleTowerAppState *appState);

int ov73_021E95CC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitUpload(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_UploadCompletePrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_SelectRankPrompt(WifiBattleTowerAppState *appState);

int ov73_021E9758(WifiBattleTowerAppState *appState);

int ov73_021E97A8(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitMatchListPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_ExitMatchListYesNo(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_AlreadyChallengedPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DownloadMatchListPrompt(WifiBattleTowerAppState *appState);

int ov73_021E987C(WifiBattleTowerAppState *appState);

int ov73_021E98AC(WifiBattleTowerAppState *appState);

int ov73_021E98FC(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_RequestMatchList(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitMatchList(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_MatchListCompletePrompt(WifiBattleTowerAppState *appState);

int ov73_021E9B94(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_InitSave(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_WaitSave(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_DisconnectingPrompt(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_CleanupInet(WifiBattleTowerAppState *appState);

int WifiBattleTower_State_GoodbyePrompt(WifiBattleTowerAppState *appState);

extern int (*ov73_021EA848[57])(WifiBattleTowerAppState *);

int ov73_021E85AC(WifiBattleTowerAppState *appState, int unused);

int ov73_021E85AC(WifiBattleTowerAppState *appState, int unused) {
    sub_0203A930(ov73_021E8440());

    int prevState = appState->state;
    int result = (*ov73_021EA848[appState->state])(appState);

    if (prevState != appState->state) {
        appState->subStep = ERROR_CLEANUP_SUBSTEP_DISPLAY_ERROR;
        appState->subTimer = 0;
    }

    inline_ov61_0222C0F8(&appState->unk_FF8);

    return result;
}

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C160(UnkStruct_ov61_0222C3B0 *param0) {
    if (param0->unk_00 != 0L) {
        SysTask_Destroy(param0->unk_00);
        param0->unk_00 = 0L;
        param0->unk_04 = 0;
    }
}

static inline void inline_ov96_0223BDEC(UnkStruct_ov61_0222C3B0 *param0, BOOL param1) {
    param0->unk_04 = param1;
}

int ov73_021E85DC(WifiBattleTowerAppState *appState, int unused) {
    inline_ov61_0222C160(&appState->unk_FF8);

    ov73_021E8BBC(appState);
    ov73_021E8B20(appState);
    ov73_021E870C(appState->bgConfig);

    inline_ov96_0223BDEC(&appState->unk_FF8, 0);

    appState->screenMode = appState->exitMode;

    if (appState->screenMode == 0) {
        return BT_LOOP_STATE_EXIT;
    }

    return BT_LOOP_STATE_INIT;
}
