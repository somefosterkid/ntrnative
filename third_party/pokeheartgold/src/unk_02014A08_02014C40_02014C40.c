#include "global.h"
#include "screen_scroll_manager.h"
struct ScreenScrollManager;

s32 sub_02014D68();

s32 sub_02014D70(void *);

void sub_02014C40(struct ScreenScrollManager *arg0);

void sub_02014C40(ScreenScrollManager *arg0) {
    if ((arg0 != NULL) && (arg0->scrollTask != NULL)) {
        sub_02014D68();
        sub_02014D70(arg0);
    }
}
