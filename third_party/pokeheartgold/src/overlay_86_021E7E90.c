#include "global.h"

#include "overlay_86_frontier_records_app.h"

#include "sys_task.h"
#include "bg_window.h"

void RemoveWindow(Window *window);

void ov86_021E7E90(Window *window) {
    RemoveWindow(window);
}
