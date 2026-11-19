#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "pm_string.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223FC30(GTSApplicationState *param_1);

void ov70_0223FC40(struct GTSApplicationState *arg0);

void ov70_0223FC30(GTSApplicationState *param_1) {
    RemoveWindow(&param_1->bottomInstructionWindow);
    return;
}

void ov70_0223FC40(GTSApplicationState *arg0) {
    arg0->genericMessageBuffer = String_New(0xB4U, HEAP_ID_61);
}
