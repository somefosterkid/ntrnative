#include "global.h"

#include "menu_input_state.h"
#include "options.h"
#include "overlay_59_juice_feed_app.h"
#include "player_data.h"

s32 sub_02031DA0(void *, void *);

void ov59_0223A9E4(Ov59JuiceFeedApp *arg0) {
    Options *options;
    SaveApricornBox *apricornBox;
    SaveData *saveData;
    int i;
    u8 count;

    arg0->menuInputState = MenuInputStateMgr_GetState(arg0->parent->args->menuInputStatePtr);
    saveData = arg0->parent->args->saveData;
    arg0->saveData = saveData;
    options = Save_PlayerData_GetOptionsAddr(saveData);
    arg0->textFrameDelay = Options_GetTextFrameDelay(options);
    arg0->frame = Options_GetFrame(options);
    apricornBox = Save_ApricornBox_Get(arg0->saveData);
    arg0->apricornBox = apricornBox;
    sub_02031CEC(apricornBox, arg0->parent->unk_14, &arg0->juice);
    sub_02031DA0(&arg0->juice, &arg0->ranks);
    ov59_0223AE20(arg0);
    count = arg0->monCount;
    i = 0;
    if (i < count) {
        do {
            if (arg0->parent->unk_1C == arg0->mons[i].slot) {
                arg0->selected = i;
                return;
            }
            i++;
        } while (i < count);
    }
}
