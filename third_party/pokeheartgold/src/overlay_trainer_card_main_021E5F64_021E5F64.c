#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_main.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_020210A0.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "overlay_trainer_card.h"
#include "trainer_card_main.h"

void ov51_021E5F64(TrainerCardMain *cardMain);

void ov51_021E5F64(TrainerCardMain *cardMain) {
    int i;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_25);
    cardMain->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2D7, HEAP_ID_25);
    cardMain->unk_33C8 = String_New(4, HEAP_ID_25);
    cardMain->formattedString = String_New(0x20, HEAP_ID_25);
    cardMain->numberString = String_New(10, HEAP_ID_25);
    cardMain->colonString = String_New(5, HEAP_ID_25);
    ReadMsgDataIntoString(cardMain->msgData, 11, cardMain->colonString);
    for (i = 0; i < 3; i++) {
        cardMain->buttonStrings[i] = NewString_ReadMsgData(cardMain->msgData, i + 14);
    }
    for (i = 0; i < 2; i++) {
        cardMain->promptStrings[i] = NewString_ReadMsgData(cardMain->msgData, i + 17);
    }
    for (i = 0; i < 14; i++) {
        cardMain->labelStrings[i] = NewString_ReadMsgData(cardMain->msgData, i);
    }
}
