#include "global.h"

#include "constants/heap.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_34.h"
#include "player_data.h"
#include "pm_string.h"
#include "save_palpad.h"

String *ov34_0225E2BC(SavePalPad *palPad, u32 id, MessageFormat *msgFormat, MsgData *msgData, PlayerProfile *profile) {
    String *string = NULL;
    String *name;
    int friendStatus = 0;

    if (id != PlayerProfile_GetTrainerID(profile)) {
        friendStatus = PalPad_PlayerIdIsFriendOrMutual(palPad, id);
    }

    if (friendStatus > 0) {
        if (friendStatus == 1) {
            BufferPlayersName(msgFormat, 0, profile);
        } else if (friendStatus >= 2) {
            int index = friendStatus - 2;

            name = String_New(10, HEAP_ID_87);

            CopyU16ArrayToString(name, PalPad_GetNthEntry(palPad, index)->name);
            BufferString(msgFormat, 0, name, 0, 0, PalPadEntry_GetFromUnk68Array(palPad, index));
            String_Delete(name);
        }

        string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, 208, HEAP_ID_87);
    }

    return string;
}
