#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "text.h"
#include "pokemon_summary_app.h"
#include "unk_0208C3E4.h"
#include "unk_02088288.h"

BoxPokemon *sub_0208A520(void *);

void sub_0208CFB4(Window *, Pokemon *, s32);

void sub_0208D0A4(PokemonSummary *arg0) {
    BoxPokemon *temp_r6;
    Pokemon *temp_r0;
    String *temp_r0_2;
    s32 temp_r7;

    ScheduleWindowCopyToVram(&arg0->windows[1]);
    FillWindowPixelBuffer(&arg0->pageWindows[6], 0U);
    temp_r6 = sub_0208A520(arg0);
    temp_r7 = sub_0208C73C(arg0);
    if (arg0->args->unk11 == 2) {
        temp_r0 = AllocMonZeroed(HEAP_ID_19);
        CopyBoxPokemonToPokemon(temp_r6, temp_r0);
        sub_0208CFB4(&arg0->pageWindows[6], temp_r0, temp_r7);
        Heap_Free(temp_r0);
    } else {
        sub_0208CFB4(&arg0->pageWindows[6], (Pokemon *)temp_r6, temp_r7);
    }
    ScheduleWindowCopyToVram(&arg0->pageWindows[6]);
    if (sub_0208D078(arg0) != 0) {
        FillWindowPixelBuffer(&arg0->pageWindows[7], 0U);
        temp_r0_2 = NewString_ReadMsgData(arg0->msgData, 0xC0);
        AddTextPrinterParameterizedWithColor(&arg0->pageWindows[7], FONT_SYSTEM, temp_r0_2, 0U, 0U, 0U, 0x10200U, NULL);
        ScheduleWindowCopyToVram(&arg0->pageWindows[7]);
        String_Delete(temp_r0_2);
    }
}
