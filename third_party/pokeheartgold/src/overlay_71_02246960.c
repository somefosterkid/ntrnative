#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200B150.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"

s32 ov71_02246B28(BoxPokemon *);

void ov71_022473E4(void);

void ov71_02246C48(SysTask *unused, void *param);

BOOL TradeSequence_Init(OverlayManager *man, s32 *state);

BOOL TradeSequence_Init(OverlayManager *man, s32 *state) {
    BOOL BVar1;
    TradeSequenceData *data;
    void *pvVar2;
    BgConfig *pBVar3;
    String *pSVar4;
    MsgData *pMVar5;
    MessageFormat *pMVar6;
    s32 iVar7;
    SpriteList *pSVar8;
    u32 uVar9;
    SysTask *pSVar10;
    TradeAnimationTemplate *args;

    BVar1 = IsPaletteFadeFinished();
    if (BVar1 != 0) {
        Heap_Create(HEAP_ID_3, HEAP_ID_56, 0x18000);
        Heap_Create(HEAP_ID_3, HEAP_ID_57, 0x18000);
        ov71_022473E4();
        Sound_SetSceneAndPlayBGM(3, 0x3f4, 1);
        data = OverlayManager_CreateAndGetData(man, sizeof(TradeSequenceData), HEAP_ID_56);
        if (data != NULL) {
            pvVar2 = OverlayManager_GetArgs(man);
            data->animationTemplate = pvVar2;
            data->currentPhase = 0;
            pBVar3 = BgConfig_Alloc(HEAP_ID_56);
            data->bgConfig = pBVar3;
            pSVar4 = String_New(400, HEAP_ID_56);
            data->strBuffer = pSVar4;
            pMVar5 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xb3, HEAP_ID_56);
            data->msgLoader = pMVar5;
            pMVar6 = MessageFormat_New(HEAP_ID_56);
            data->strTemplate = pMVar6;
            args = data->animationTemplate;
            iVar7 = args->tradeType;
            switch (iVar7) {
            case 1:
                BufferBoxMonNickname(data->strTemplate, 0, args->sendingPokemon);
                BufferBoxMonNickname(data->strTemplate, 1, data->animationTemplate->receivingPokemon);
                BufferPlayersName(data->strTemplate, 2, data->animationTemplate->otherTrainer);
                iVar7 = ov71_02246B28(data->animationTemplate->receivingPokemon);
                data->flipSprite = iVar7;
                break;
            case 2:
                BufferBoxMonNickname(data->strTemplate, 0, args->sendingPokemon);
                break;
            case 4:
                BufferBoxMonNickname(data->strTemplate, 1, args->receivingPokemon);
                iVar7 = ov71_02246B28(data->animationTemplate->receivingPokemon);
                data->flipSprite = iVar7;
                break;
            }
            NNS_G2dInitOamManagerModule();
            OamManager_Create(0, 0x80, 0, 0x20, 1, 0x7f, 0, 0x20, HEAP_ID_56);
            pSVar8 = G2dRenderer_Init(0x40, &data->renderer, HEAP_ID_56);
            data->spriteList = pSVar8;
            G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, FX32_CONST(232));
            uVar9 = GetBoxMonData(data->animationTemplate->sendingPokemon, 5, (void *)0x0);
            data->sendingSpecies = (s16)uVar9;
            uVar9 = GetBoxMonData(data->animationTemplate->receivingPokemon, 5, (void *)0x0);
            data->receivingSpecies = (s16)uVar9;
            uVar9 = GetBoxMonData(data->animationTemplate->sendingPokemon, 0x70, (void *)0x0);
            data->sendingForm = (s16)uVar9;
            uVar9 = GetBoxMonData(data->animationTemplate->receivingPokemon, 0x70, (void *)0x0);
            data->receivingForm = (s16)uVar9;
            data->currentPhaseData = NULL;
            Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
            HBlankInterruptDisable();
            GfGfx_DisableEngineAPlanes();
            GfGfx_DisableEngineBPlanes();
            reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
            reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
            pSVar10 = SysTask_CreateOnPrintQueue(ov71_02246C48, data, 1);
            data->task = pSVar10;
            data->currentPhaseData = NULL;
        }
        return 1;
    }
    return 0;
}
