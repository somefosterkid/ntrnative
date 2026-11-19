#include <dwc.h>

#include "global.h"
#include "sprite_resources_header.h"
#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_80_frontier_opponent.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "message_format.h"
#include "msgdata.h"

void ov73_021E8960(void *arg0, void *arg1);

struct BgConfig;
struct BoxPokemon;
struct FrontierPokemon;
struct G2dRenderer;
struct Menu;
struct MsgData;
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
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
typedef u32 TextColor;
typedef struct Menu Menu;
void ov73_021EA088(Window *window, String *string, int xOffset, int yOffset, int alignment, TextColor textColor);
void ov73_021E8A08(WifiBattleTowerAppState *appState);
void ov73_021E8B20(WifiBattleTowerAppState *appState);

void ov73_021E8960(void *arg0, void *arg1) {
    u32 v0;

    if ((*(s32 *)((u8 *)arg1 + 4) != 0) && (*(u8 *)((u8 *)arg1 + 0x32B) = (u8)(*(u8 *)((u8 *)arg1 + 0x32B) ^ 1), ((1 & *(u8 *)((u8 *)arg1 + 0x32B)) == 0))) {
        if ((u32) * (u32 *)((u8 *)arg1 + 0x32C) <= 1U) {
            GX_LoadBGPltt(arg1 + 0x88 + (*(s16 *)((u8 *)arg1 + 0x328) << 5), 0U, 0x20U);
        }
        v0 = *(u32 *)((u8 *)arg1 + 0x32C);
        if ((v0 == 0) || (v0 == 2)) {
            GXS_LoadBGPltt(arg1 + 0x88 + (*(s16 *)((u8 *)arg1 + 0x328) << 5), 0U, 0x20U);
        }
        if (*(s8 *)((u8 *)arg1 + 0x32A) == 0) {
            *(s16 *)((u8 *)arg1 + 0x328) = (s16)(*(s16 *)((u8 *)arg1 + 0x328) + 1);
            if ((s32) * (s16 *)((u8 *)arg1 + 0x328) >= 0x15) {
                *(s16 *)((u8 *)arg1 + 0x328) = 0x13;
                *(s8 *)((u8 *)arg1 + 0x32A) = (s8)(1 ^ *(s8 *)((u8 *)arg1 + 0x32A));
            }
        } else {
            *(s16 *)((u8 *)arg1 + 0x328) = (s16)(*(s16 *)((u8 *)arg1 + 0x328) - 1);
            if ((s32) * (s16 *)((u8 *)arg1 + 0x328) < 0) {
                *(s16 *)((u8 *)arg1 + 0x328) = 1;
                *(s8 *)((u8 *)arg1 + 0x32A) = (s8)(*(s8 *)((u8 *)arg1 + 0x32A) ^ 1);
            }
        }
    }
}

void ov73_021E8A08(WifiBattleTowerAppState *appState) {
    AddWindowParameterized(appState->bgConfig, &appState->popupWindow, 0, 4, 4, 23, 16, 13, ((((((6 * 3) + ((2 * 2) * 3)) + 1) + 9) + 27 * 4) + 4 * 2) + 4 * 2);
    FillWindowPixelBuffer(&appState->popupWindow, 0x0);
    AddWindowParameterized(appState->bgConfig, &appState->opponentWindow, 0, 26, 14, 4, 2, 13, ((((6 * 3) + ((2 * 2) * 3)) + 1) + 9) + 27 * 4);
    FillWindowPixelBuffer(&appState->opponentWindow, 0xf0f);
    AddWindowParameterized(appState->bgConfig, &appState->rankWindow, 0, 26, 14, 4, 2, 13, (((((6 * 3) + ((2 * 2) * 3)) + 1) + 9) + 27 * 4) + 4 * 2);
    FillWindowPixelBuffer(&appState->rankWindow, 0xf0f);
    AddWindowParameterized(appState->bgConfig, &appState->titleWindow, 0, 4, 1, 24, 2, 13, (((((((6 * 3) + ((2 * 2) * 3)) + 1) + 9) + 27 * 4) + 4 * 2) + 4 * 2) + 23 * 16);
    FillWindowPixelBuffer(&appState->titleWindow, 0x0);
    ov73_021EA088(&appState->titleWindow, appState->titleString, 0, 1, 1, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    AddWindowParameterized(appState->bgConfig, &appState->msgWindow, 0, 2, 19, 27, 4, 13, (((6 * 3) + ((2 * 2) * 3)) + 1) + 9);
    FillWindowPixelBuffer(&appState->msgWindow, 0xf0f);
}

void ov73_021E8B20(WifiBattleTowerAppState *appState) {
    RemoveWindow(&appState->msgWindow);
    RemoveWindow(&appState->titleWindow);
    RemoveWindow(&appState->rankWindow);
    RemoveWindow(&appState->opponentWindow);
    RemoveWindow(&appState->popupWindow);
}
