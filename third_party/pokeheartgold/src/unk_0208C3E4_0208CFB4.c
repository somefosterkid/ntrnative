#include "global.h"

#include "bg_window.h"
#include "text.h"
#include "trainer_memo.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208CFB4(Window *window, Pokemon *a0, s32 a1, s32 a3);

void sub_0208CFB4(Window *window, Pokemon *a0, s32 a1, s32 a3) {
    String *string;
    String *v0;
    String *v1;
    String *v2;
    Unk0208E600 *unk0208E600;

    unk0208E600 = sub_0208E600(a0, a1, HEAP_ID_19, a3);
    string = unk0208E600->notepad.nature;
    if (string != NULL) {
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 6U, (unk0208E600->notepad.natureLine - 1) * 0x10, 0xFFU, 0x10200U, NULL);
    }
    v0 = unk0208E600->notepad.dateLocationMet;
    if (v0 != NULL) {
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, v0, 6U, (unk0208E600->notepad.dateLocationMetLine - 1) * 0x10, 0xFFU, 0x10200U, NULL);
    }
    v1 = unk0208E600->notepad.characteristic;
    if (v1 != NULL) {
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, v1, 6U, (unk0208E600->notepad.characteristicLine - 1) * 0x10, 0xFFU, 0x10200U, NULL);
    }
    v2 = unk0208E600->notepad.eggWatch;
    if (v2 != NULL) {
        AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, v2, 6U, (unk0208E600->notepad.eggWatchLine - 1) * 0x10, 0xFFU, 0x10200U, NULL);
    }
    FillBgTilemapRect(window->bgConfig, 1U, 0U, 0x10U, 5U, 2U, 0x12U, 0U);
    sub_0208E994(unk0208E600);
}

s32 sub_0208D078(PokemonSummary *arg0) {
    return arg0->mon.shinyLeaves[5] + (arg0->mon.shinyLeaves[4] + (arg0->mon.shinyLeaves[3] + (arg0->mon.shinyLeaves[2] + (arg0->mon.shinyLeaves[0] + arg0->mon.shinyLeaves[1]))));
}
