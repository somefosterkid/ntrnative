#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sav_chatot.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "wild_encounter.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"

void sub_0208BC78(PokemonSummary *summaryScreen);

void sub_0208BC78(PokemonSummary *summaryScreen) {
    s16 v0[2];

    Sprite_GetPositionXY(summaryScreen->sprites[9], &v0[1], &v0[0]);
    Sprite_SetPositionXY(summaryScreen->sprites[10], v0[1], v0[0]);
    Sprite_SetDrawFlag(summaryScreen->sprites[10], 1);
}

void sub_0208BCB4(PokemonSummary *arg0) {
    Sprite_SetPositionXY(arg0->sprites[10], 0x44, 0xA8);
    Sprite_SetAnimCtrlSeq(arg0->sprites[10], 0);
}

void sub_0208BCD4(PokemonSummary *arg0) {
    u32 v1;
    u8 v0;

    *((s8 *)&arg0->sparkleState) = 0;
    *((s8 *)&arg0->unk7C3) = 0;
    *((s8 *)&arg0->unk7C2) = 0;
    v0 = arg0->mon.contestStats[5];
    if (v0 == 0) {
        *((s8 *)&arg0->numSparkles) = 0;
    } else if (v0 == 0xFF) {
        *((s8 *)&arg0->numSparkles) = 0xC;
    } else {
        *((s8 *)&arg0->numSparkles) = (s8)((s32)(0xC * v0) >> 8);
    }
    v1 = 0x1D;
    do {
        Sprite_SetDrawFlag(arg0->sprites[v1], 0);
        Sprite_SetAnimationFrame(arg0->sprites[v1], 0U);
        v1 += 1;
    } while (v1 <= 0x28U);
}
