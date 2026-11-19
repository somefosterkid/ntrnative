#include "global.h"

#include "battle/battle_022378C0.h"
#include "battle/battle_controller_player.h"
#include "battle/battle_io_command.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "battle/overlay_12_0226BEC4.h"
#include "gf_3d_vramman.h"

#include "bag.h"
#include "bg_window.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "pokedex.h"
#include "pokepic.h"
#include "render_text.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_020163E0.h"
#include "unk_02016EDC.h"
#include "unk_020210A0.h"
#include "unk_02037C94.h"
#include "vram_transfer_manager.h"
#include "overlay_12_field_battle_dto.h"

s32 ov07_0221BFE0(u32 *);

void ov12_022396E8(GF3DVramMan *vramMan);

s32 sub_02014F84(void);

s32 sub_020302A4(void *);

FS_EXTERN_OVERLAY(OVY_18);

FS_EXTERN_OVERLAY(OVY_6);

FS_EXTERN_OVERLAY(OVY_7);

void ov12_0223843C(OverlayManager *man) {
    s32 sp0;
    BattleSystem *temp_r4;
    s32 var_r6_2;
    SysTask *temp_r0_3;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 var_r6_3;
    u16 var_r5;
    void *temp_r0;

    temp_r4 = OverlayManager_GetData(man);
    temp_r0 = OverlayManager_GetArgs(man);
    *((s32 *)&((FieldBattleDTO *)temp_r0)->seed) = (s32)temp_r4->randTemp;
    *((s32 *)&((FieldBattleDTO *)temp_r0)->battleStatusMask) = (s32)temp_r4->battleSpecial;
    if (!(0x10 & temp_r4->battleSpecial)) {
        sub_020302A4(temp_r0);
    }
    if (temp_r4->unk2445 != 0) {
        ov12_02237ED0(temp_r4, 0);
    }
    if (0xAA4 & *(s32 *)temp_r0) {
        var_r5 = 0;
    } else if ((u32)(u8)(temp_r4->battleOutcomeFlag + 0xFE) <= 1U) {
        var_r5 = 0x7FFF;
    } else {
        var_r5 = 0;
    }
    sub_0200FBF4(PM_LCD_TOP, var_r5);
    sub_0200FBF4(PM_LCD_BOTTOM, var_r5);
    BattleSystem_TryChangeForm(temp_r4);
    if (temp_r4->battleOutcomeFlag != 4) {
        ov12_0223B870(temp_r4, Party_GetMonByIndex(temp_r4->trainerParty[1], 0));
    }
    for (sp0 = 0; sp0 < 4; sp0++) {
        Party_Copy(temp_r4->trainerParty[sp0], ((FieldBattleDTO *)temp_r0)->parties[sp0]);
        Heap_Free(temp_r4->trainerParty[sp0]);
        PlayerProfile_Copy(temp_r4->playerProfile[sp0], (PlayerProfile *)((FieldBattleDTO *)temp_r0)->trainerInfo[sp0]);
        Heap_Free(temp_r4->playerProfile[sp0]);
    }
    sub_020164C4((UnkStruct_020163E0 *)temp_r4->msgIcon);
    Save_Bag_Copy(temp_r4->bag, ((FieldBattleDTO *)temp_r0)->bag);
    Heap_Free(temp_r4->bag);
    Pokedex_Copy(temp_r4->pokedex, ((FieldBattleDTO *)temp_r0)->pokedex);
    Heap_Free(temp_r4->pokedex);
    *((PCStorage **)&((FieldBattleDTO *)temp_r0)->pcBoxes) = (PCStorage *)temp_r4->storage;
    ((FieldBattleDTO *)temp_r0)->bagCursor = (BagCursor *)temp_r4->bagCursor;
    ((FieldBattleDTO *)temp_r0)->subscreenCursorOn = (u8 *)temp_r4->unk1C0;
    *((u32 **)&((FieldBattleDTO *)temp_r0)->poketch) = (u32 *)temp_r4->unk98;
    *((u32 **)&((FieldBattleDTO *)temp_r0)->captureAttempt) = (u32 *)temp_r4->unk9C;
    *((s32 *)&((FieldBattleDTO *)temp_r0)->countSafariBalls) = (s32)temp_r4->safariBallCnt;
    *((s32 *)&((FieldBattleDTO *)temp_r0)->resultMask) = (s32)(0x3F & temp_r4->battleOutcomeFlag);
    *((s32 *)&((FieldBattleDTO *)temp_r0)->caughtBattlerIdx) = (s32)temp_r4->unk243C;
    *((s32 *)&((FieldBattleDTO *)temp_r0)->leveledUpMonsMask) = ov12_022581D4(temp_r4, temp_r4->ctx, 4, 0);
    *((s32 *)&((FieldBattleDTO *)temp_r0)->battleRecords.totalTurns) = (s32)(*((s32 *)&((FieldBattleDTO *)temp_r0)->battleRecords.totalTurns) + ov12_022581D4(temp_r4, temp_r4->ctx, 3, 0));
    temp_r5 = ov12_022581D4(temp_r4, temp_r4->ctx, 6, 0);
    *((s32 *)&((FieldBattleDTO *)temp_r0)->battleRecords.totalFainted) = (s32)(*((s32 *)&((FieldBattleDTO *)temp_r0)->battleRecords.totalFainted) + (temp_r5 + ov12_022581D4(temp_r4, temp_r4->ctx, 6, 2)));
    temp_r5_2 = ov12_022581D4(temp_r4, temp_r4->ctx, 7, 0);
    *((s32 *)&((FieldBattleDTO *)temp_r0)->battleRecords.totalDamage) = (s32)(*((s32 *)&((FieldBattleDTO *)temp_r0)->battleRecords.totalDamage) + (temp_r5_2 + ov12_022581D4(temp_r4, temp_r4->ctx, 7, 2)));
    *((s32 *)&((FieldBattleDTO *)temp_r0)->totalTurnsElapsed) = ov12_022581D4(temp_r4, temp_r4->ctx, 3, 0);
    var_r6_2 = 0;
    ((FieldBattleDTO *)temp_r0)->recordingStopped = temp_r4->isRecordingPaused;
    do {
        Heap_Free(temp_r4->unk1D0[var_r6_2].tiles);
        var_r6_2 += 1;
    } while (var_r6_2 < 4);
    Heap_Free(temp_r4->msgBuffer);
    PaletteData_FreeBuffers(temp_r4->palette, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(temp_r4->palette, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(temp_r4->palette, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(temp_r4->palette, PLTTBUF_SUB_OBJ);
    PaletteData_Free(temp_r4->palette);
    DestroyMsgData(temp_r4->msgData);
    DestroyMsgData((MsgData *)temp_r4->unk10);
    MessageFormat_Delete(temp_r4->msgFormat);
    sub_02016F2C((PokepicAnimManager *)temp_r4->unk1C8);
    sub_02014F84();
    ov07_0221BFE0(temp_r4->unk8C);
    BattleContext_Delete(temp_r4->ctx);
    var_r6_3 = 0;
    if ((s32)temp_r4->maxBattlers > 0) {
        do {
            OpponentData_Delete(temp_r4, temp_r4->opponentData[var_r6_3], (s32)temp_r4->unk23FD);
            var_r6_3 += 1;
        } while (var_r6_3 < (s32)temp_r4->maxBattlers);
    }
    PokepicManager_Delete(temp_r4->pokepicManager);
    if (temp_r4->unk23FD != 2) {
        ov12_02237B6C(temp_r4);
    }
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    WindowArray_Delete(temp_r4->window, 3);
    Heap_Free(temp_r4->bgConfig);
    Heap_Free(temp_r4->unk220);
    Heap_Free(temp_r4->unk224);
    MessagePrinter_Delete((MessagePrinter *)temp_r4->hpNumPrinter);
    SysTask_Destroy(temp_r4->unk1C);
    SysTask_Destroy(temp_r4->unk20);
    DisableTouchPad();
    ov12_022396E8(temp_r4->unk0);
    SetLCRNGSeed(temp_r4->unk2434);
    if (BattleSystem_GetCriticalHpMusicFlag(temp_r4) != 0) {
        StopSE(0x704U, 0);
    }
    sub_0202067C((NNSG2dCellTransferState *)temp_r4->unk1CC);
    if (BattleSystem_IsRecordingPaused(temp_r4) != 0) {
        Sound_SetMasterVolume(0x7F);
    }
    temp_r0_3 = temp_r4->unk247C;
    if (temp_r0_3 != NULL) {
        ov12_0226BEF0(temp_r0_3);
    }
    Heap_Free(temp_r4);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_6));
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_7));
    if (sub_02039998() == 0) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_18));
    }
}
