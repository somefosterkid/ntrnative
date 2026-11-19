#include "global.h"

#include "field_system.h"
#include "mail_message.h"
#include "overlay_34.h"
#include "player_data.h"
#include "unk_struct_0203330c.h"

WMBssDesc *sub_02035754(int param0);
PlayerProfile *sub_02035798(int param0);
MailMessage *sub_0205AA84(struct UnionRoomManager *fieldSystem_unk80);

void ov34_0225E4F8(Ov34Work *work) {
    int i;
    WMBssDesc *bssDesc;
    UnkStruct_0203330C *beacon;
    MailMessage *mail;

    if (FieldSystem_TaskIsRunning(work->fieldSystem)) {
        return;
    }

    for (i = 0; i < 16; i++) {
        bssDesc = sub_02035754(i);

        if (bssDesc != NULL) {
            beacon = (UnkStruct_0203330C *)&bssDesc->gameInfo.userGameInfo;
            mail = &beacon->sentence;

            if (ov34_0225E428(work, mail, beacon->trainerId)) {
                ov34_0225E4A8(work, sub_02035798(i), mail, beacon->trainerId);
            }
        }
    }

    if ((mail = sub_0205AA84(work->unk8)) != NULL) {
        u32 trainerId = PlayerProfile_GetTrainerID(work->profile);
        ov34_0225E4A8(work, work->profile, mail, trainerId);
    }
}
