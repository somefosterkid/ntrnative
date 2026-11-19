#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_0201956C.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

BOOL sub_02089E80(PokemonSummary *summaryScreen, s32 badge);

void sub_0208A0EC(PokemonSummary *summaryScreen);

void sub_0208A1A0(PokemonSummary *summaryScreen);

void sub_0208AF70(PokemonSummary *arg0);

void sub_0208B118(PokemonSummary *arg0);

void sub_02089F50(PokemonSummary *param_1, BgConfig *param_2, s32 param_3, u32 param_4);

extern s32 _021039B8[][3];

void sub_02089F98(PokemonSummary *arg0);

void sub_02089F98(PokemonSummary *arg0) {
    s8 temp_r0;

    if ((arg0->args->isFlag982Set == 0) && (arg0->page == 2)) {
        sub_02089F50(arg0, arg0->bgConfig, 0xB, 3);
    } else {
        sub_02089F50(arg0, arg0->bgConfig, _021039B8[arg0->page][0], 3);
    }
    if (arg0->args->unk12 == 2) {
        sub_02089F50(arg0, arg0->bgConfig, _021039B8[arg0->page][2], 6);
        ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_ROT, 0x80);
        ScheduleSetBgPosText(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, 0);
        if (arg0->args->moveToLearn == 0) {
            sub_0208AF70(arg0);
        }
    } else if (arg0->mon.isEgg != 0) {
        sub_02089F50(arg0, arg0->bgConfig, _021039B8[arg0->page][2], 6);
    } else if (sub_02089E80(arg0, 2) != 0) {
        sub_02089F50(arg0, arg0->bgConfig, _021039B8[arg0->page][1], 6);
    } else if (arg0->page == 0) {
        sub_02089F50(arg0, arg0->bgConfig, 0x4D, 6);
    } else {
        sub_02089F50(arg0, arg0->bgConfig, 0x4E, 6);
    }
    if (arg0->args->unk11 == 2) {
        sub_020196E8(arg0->cellManager, 4, 0x14, 4);
        sub_0208B118(arg0);
    }
    if (arg0->mon.isEgg == 0) {
        temp_r0 = arg0->page;
        if (temp_r0 == 0) {
            sub_0208A1A0(arg0);
            return;
        }
        switch (temp_r0) {
        case 1:
            sub_0208A0EC(arg0);
            break;
        }
    }
}
