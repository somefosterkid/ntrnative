#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "heap.h"
#include "message_format.h"
#include "player_data.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "overlay_96_app_021ed1d0.h"
#include "overlay_96_text_box.h"

typedef enum HeapID HeapID;

void ov96_021EAC5C(Ov96SpriteSetElem *param_1, u32 param_2);

u32 ov96_021EC2E0(Ov96App_021ED1D0 *param_1);

void ov96_021ECB38(ManagedSprite **param_1, PokeathlonCourseData *param_2, u32 param_3, s32 param_4, u32 param_5);

void ov96_021ED524(PokeathlonCourseData *param_1, s32 param_2, s32 param_3, u32 param_4);

u32 ov96_021ED838(PokeathlonCourseData *param_1, s32 param_2);

u32 ov96_021ED86C(PokeathlonCourseData *param_1, s32 param_2);

u32 ov96_021ED8A4(PokeathlonCourseData *param_1, s32 param_2);

void ov96_021EDF3C(MessageFormat *param_1, s32 param_2, u32 param_3, u32 param_4);

u32 ov96_021EE324(PokeathlonCourseData *param_1);

u32 ov96_021EE54C(PokeathlonCourseData *param_1);

u32 ov96_021EE830(Ov96TextBox *param_1);

void ov96_021EE8CC(Ov96TextBox *param_1, u32 param_2);

void ov96_021EE908(Ov96TextBox *param_1, u32 param_2);

void ov96_021EE944(Ov96TextBox *param_1);

MessageFormat *ov96_021EE97C(Ov96TextBox *param_1);

u32 ov96_021EEA80(Ov96TextBox *param_1);

void sub_0200FC20(u16);

extern u32 ov96_0221B088[];

BOOL ov96_021EB730(PokeathlonCourseData *param_1, u8 *unused);

BOOL ov96_021EB730(PokeathlonCourseData *param_1, u8 *unused) {
    Ov96App_021ED1D0 *work;
    u32 uVar2;
    u32 uVar3;
    MessageFormat *messageFormat;
    BOOL BVar4;
    PokeathlonParticipantData *pPVar5;
    u32 uVar6;
    PlayerProfile *playerProfile;
    Ov96SpriteSetElem *piVar7;
    Ov96SpriteResEntry *entry;
    Sprite *sprite;
    NNSG2dCellAnimation *pNVar9;
    s32 iVar11;
    s32 j;

    work = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    iVar11 = 0;
    uVar2 = PokeathlonCourse_GetMode(param_1);
    if (uVar2 == 1) {
        iVar11 = ov96_021EE324(param_1);
    }
    ov96_021EE830(work->textBox);
    uVar3 = ov96_021EEA80(work->textBox);
    if ((uVar3 == 0) && (iVar11 == 0)) {
        messageFormat = ov96_021EE97C(work->textBox);
        iVar11 = -1;
        switch (work->state) {
        case 0:
            iVar11 = 0;
            BufferPokeathlonCourseName(messageFormat, 0, work->courseId);
            work->state = 1;
            break;
        case 1:
            sub_0200FC20(0x7fff);
            PlaySE(0x8dc);
            work->state = 2;
            break;
        case 2:
#if defined(SDK_PORT) || defined(NONMATCHING)
            if (work->counter < 4) {
#else
            if (*(u8 *)((s32)work + 0xb4) < 4) {
#endif
                work->counter = work->counter + 1;
            } else {
                work->counter = 0;
                work->state = 3;
            }
            break;
        case 3:
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (u32)iVar11 >> 17, 0x18, 1, work->heapId);
            work->state = 4;
            break;
        case 4:
            BVar4 = IsPaletteFadeFinished();
            if (BVar4 != 0) {
                work->state = 5;
            }
            break;
        case 5:
            pPVar5 = PokeathlonCourse_GetParticipantData(param_1, (u32)work->playerIdx);
            if (*(s32 *)pPVar5->field_00 > 0) {
                uVar6 = ov96_021ED838(param_1, (u32)work->playerIdx);
                ov96_021EE908(work->textBox, uVar6);
            } else {
                iVar11 = ov96_021ED838(param_1, (u32)work->playerIdx);
                uVar3 = ov96_021ED86C(param_1, (u32)work->playerIdx);
                ov96_021EDF3C(messageFormat, uVar3, 7, 1);
            }
            work->state = 6;
            break;
        case 6:
            playerProfile = PokeathlonCourse_GetPlayerProfileFromData(param_1, (u32)work->playerIdx);
            BufferPlayersName(messageFormat, 0, playerProfile);
            uVar3 = 0;
            do {
                ov96_021ED524(param_1, (u32)work->playerIdx, uVar3 & 0xff, uVar3 + 1);
                uVar3 = uVar3 + 1;
            } while ((s32)uVar3 < 3);
            ov96_021ECB38(work->sprites, param_1, (u32)work->playerIdx, 2, work->heapId);
            ManagedSprite_SetAnim(work->sprites[work->playerIdx], 1);
            uVar3 = ov96_021ED8A4(param_1, (u32)work->playerIdx);
            if ((uVar3 & 0xff) < 5) {
                iVar11 = 0x8dc;
            } else {
                iVar11 = 0x8dd;
            }
            PlaySE_SetPitch((u16)iVar11, (s16)ov96_0221B088[uVar3 & 0xff]);
            iVar11 = 1;
            work->busy = 1;
            work->state = 7;
            break;
        case 7:
            if (work->busy == 0) {
                if (work->playerIdx >= 4) {
                    work->unk_B8 = 1;
                    iVar11 = 2;
                    work->state = 8;
                } else {
                    work->state = 5;
                }
            }
            break;
        case 8:
            uVar2 = PokeathlonCourse_GetMode(param_1);
            if ((uVar2 == 0) || (uVar3 = ov96_021EE54C(param_1), uVar3 != 0)) {
                iVar11 = ov96_021EC2E0(work);
                if (iVar11 != 0) {
                    Heap_Free(work->cryTaskData);
                    SysTask_Destroy(work->cryTask);
                }
                ov96_021EE944(work->textBox);
                PokeathlonCourse_SetStateField07(param_1, 2);
                return 0;
            }
            break;
        default:
            GF_AssertFail();
        }
        if (iVar11 != -1) {
            ov96_021EE8CC(work->textBox, iVar11);
            work->step = work->step + 1;
        }
    }
    if (work->busy != 0) {
        uVar3 = (u32)work->playerIdx * 0x3000000 >> 0x18;
        if (work->timer++ == 0) {
            iVar11 = 0;
            do {
                piVar7 = ov96_021EAA04(work->spriteSet, uVar3 & 0xff);
                ov96_021EAC0C(piVar7, 2);
                ov96_021EAC5C(piVar7, 0x10);
                iVar11 = iVar11 + 1;
                uVar3 = uVar3 + 1;
            } while (iVar11 < 3);
        } else {
            if (work->timer++ >= 0x3C) {
                iVar11 = 0;
                j = 0;
                do {
                    piVar7 = ov96_021EAA04(work->spriteSet, uVar3 & 0xff);
                    ov96_021EAC0C(piVar7, 1);
                    ov96_021EAC5C(piVar7, 0);
                    entry = ov96_021EAA20(piVar7);
                    sprite = ov96_021E8BAC(entry);
                    pNVar9 = Sprite_GetCellAnim(sprite);
                    (pNVar9->animCtrl).currentTime = (j * 3) << 0xc;
                    uVar3 = uVar3 + 1;
                    j = j + 1;
                } while (j < 3);
                ov96_021ECB38(work->sprites, param_1, (u32)work->playerIdx, 1, work->heapId);
                work->busy = 0;
                work->playerIdx = work->playerIdx + 1;
                work->timer = 0;
            }
        }
    }
    return 0;
}
