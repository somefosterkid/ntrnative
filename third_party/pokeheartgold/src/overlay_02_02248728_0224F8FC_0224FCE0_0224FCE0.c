#include "global.h"
struct FieldSystem;

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "field_roamer.h"
#include "field_system.h"
#include "field_warp_tasks.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "message_format.h"
#include "metatile_behavior.h"
#include "msgdata.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "roamer.h"
#include "save_local_field_data.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "unk_02062108.h"

struct BoxPokemon;

struct CAPSULE;

struct Mail;

struct MailMessage;

struct MessageFormat;

struct MsgData;

struct Party;

struct PartyPokemon;

struct PlayerProfile;

struct PokedexData;

struct SEAL;

struct SaveData;

struct String;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(struct FieldSystem *work, String *dest, enum HeapID heapID, int flags, u8 a5);

void ov02_0224FCE0(FieldSystem *work, String *dest, enum HeapID heapID, int flags, u8 a5) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 40, heapID);
    MessageFormat *messageFormat = MessageFormat_New(heapID);
    String *str;
    int strno;
    if (flags & 2) {
        BufferPlayersName(messageFormat, 0, Save_PlayerData_GetProfile(work->saveData));
        if (flags & 1) {
            BufferFashionName(messageFormat, 1, a5 - 1);
            strno = 0x20;
        } else {
            BufferFashionNameWithArticle(messageFormat, 1, a5 - 1);
            strno = 0x5f;
        }
    } else {
        BoxPokemon *boxMon = Mon_GetBoxMon(GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(work->saveData)));
        BufferPlayersName(messageFormat, 0, Save_PlayerData_GetProfile(work->saveData));
        BufferBoxMonNickname(messageFormat, 1, boxMon);
        if (flags & 1) {
            strno = 0x61;
        } else {
            strno = 0x62;
        }
    }
    str = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(messageFormat, dest, str);
    String_Delete(str);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
}
