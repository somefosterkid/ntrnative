#include "global.h"

#include "igt.h"
#include "message_format.h"
#include "player_data.h"
#include "overlay_01_save_info.h"

void ov01_021F3F9C(MessageFormat *arg0, SaveInfo *arg1);

int ov01_021F4044(SaveInfo *arg0);

void ov01_021F3F9C(MessageFormat *messageFormat, SaveInfo *arg1) {
    enum PrintingMode strconvmode;
    enum PrintingMode v1;
    s32 num;
    s32 v0;
    u32 numDigits;
    u32 v2;

    BufferLandmarkName(messageFormat, 0U, arg1->mapLabelTextID);
    BufferPlayersName(messageFormat, 1U, arg1->trainerInfo);
    BufferIntegerAsString(messageFormat, 2U, PlayerProfile_CountBadges(arg1->trainerInfo), 2U, PRINTING_MODE_LEFT_ALIGN, 1);
    v0 = arg1->pokedexCount;
    if (v0 >= 0x64) {
        numDigits = 3;
        strconvmode = PRINTING_MODE_LEFT_ALIGN;
    } else if (v0 >= 0xA) {
        numDigits = 3;
        strconvmode = PRINTING_MODE_RIGHT_ALIGN;
    } else {
        numDigits = 2;
        strconvmode = PRINTING_MODE_RIGHT_ALIGN;
    }
    BufferIntegerAsString(messageFormat, 3U, v0, numDigits, strconvmode, 1);
    num = (s32)GetIGTHours(arg1->playTime);
    if (num >= 0x64) {
        v2 = 3;
        v1 = PRINTING_MODE_LEFT_ALIGN;
    } else if (num >= 0xA) {
        v2 = 3;
        v1 = PRINTING_MODE_RIGHT_ALIGN;
    } else {
        v2 = 2;
        v1 = PRINTING_MODE_RIGHT_ALIGN;
    }
    BufferIntegerAsString(messageFormat, 4U, num, v2, v1, 1);
    BufferIntegerAsString(messageFormat, 5U, (s32)GetIGTMinutes(arg1->playTime), 2U, PRINTING_MODE_LEADING_ZEROS, 1);
}

int ov01_021F4044(SaveInfo *arg0) {
    return 10;
}
