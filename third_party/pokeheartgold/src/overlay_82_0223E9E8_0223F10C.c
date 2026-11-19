#include "global.h"

#include "bg_window.h"
#include "message_printer.h"
#include "overlay_82.h"
#include "overlay_82_battle_hall_app.h"

void ov82_0223F10C(void *arg0, Window *window, u32 num, u32 x, u32 y) {
    PrintUIntOnWindow(((MessagePrinter *)((BattleHallApp *)arg0)->specialChars), num, 2U, PRINTING_MODE_LEFT_ALIGN, window, x, y);
    ScheduleWindowCopyToVram(window);
}
