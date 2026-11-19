/* PORTOK hk39 */
#include "error_handling.h"
#include "filesystem.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokeathlon/pokeathlon.h"
#include "pokemon.h"
#include "string_util.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokeathlon/pokeathlon.h"
#include "pokemon.h"
#include "string_util.h"
s32 ov96_021E679C(s32 param_1, s32 param_2);
void ov96_021E860C(u32 param_1, s32 param_2, s32 param_3, s32 param_4);
typedef struct Ov96AthleteMon {
    u16 species;      // 0x00
    u16 unk_02;
    u32 unk_04;
    u8 stats[5];      // 0x08
    u8 unk_0D[3];
    u8 unk_10;
    u8 genderType;    // 0x11: 0 male-only/normal, 1 female (254), 2 genderless (255)
    u16 name[11];     // 0x12
} Ov96AthleteMon;     // 0x28

typedef struct Ov96Participant {
    u32 courseMonsId;         // 0x00
    Ov96AthleteMon mons[3];   // 0x04
} Ov96Participant;            // 0x7C (PokeathlonParticipantData)

void ov96_021E8484(PokeathlonCourseData *param_1, s32 param_2) {
    MsgData *msgData;
    NARC *perfNarc;
    NARC *teamNarc;
    String *str;
    Ov96Participant *part;
    Ov96AthleteMon *mon;
    PlayerProfile *profile;
    u8 *ids;
    s32 i, k;
    u8 type;
    u16 team[4];        // sp+0x24: species[3], u8 name msg id at +6
    u8 buf[23];         // sp+0x2C: [0..] course mon ids, [3..7] performance stats

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x136, PokeathlonCourse_GetHeapID(param_1));
    i = 4 - param_2;
    ov96_021E860C(param_2, ((u8 *)param_1->args)[0xC], ((u8 *)param_1->args)[0xF], (s32)buf);
    perfNarc = NARC_New(NARC_poketool_personal_performance, PokeathlonCourse_GetHeapID(param_1));
    teamNarc = NARC_New(NARC_a_2_5_8, PokeathlonCourse_GetHeapID(param_1));
    for (ids = buf; i < 4; i++, ids++) {
        part = (Ov96Participant *)PokeathlonCourse_GetParticipantData(param_1, i);
        part->courseMonsId = *ids;
        if (part->courseMonsId == 0) {
            GF_AssertFail();
        }
        NARC_ReadWholeMember(teamNarc, part->courseMonsId - 1, team);
        str = NewString_ReadMsgData(msgData, ((u8 *)team)[6]);
        profile = PokeathlonCourse_GetPlayerProfileFromData(param_1, i);
        Save_Profile_PlayerName_Set(profile, String_cstr(str));
        String_Delete(str);
        mon = part->mons;
        for (k = 0; k < 3; k++, mon++) {
            mon->species = team[k];
            mon->unk_02 = 0;
            mon->unk_10 = 0;
            mon->unk_04 = 0;
            switch ((u8)GetMonBaseStat(mon->species, BASE_GENDER_RATIO)) {
            case 0:
                type = 0;
                break;
            case 254:
                type = 1;
                break;
            case 255:
                type = 2;
                break;
            default:
                type = 0;
                break;
            }
            mon->genderType = type;
            NARC_ReadWholeMember(perfNarc, ov96_021E679C(mon->species, 0), &buf[3]);
            mon->stats[0] = buf[3];
            mon->stats[1] = buf[4];
            mon->stats[2] = buf[5];
            mon->stats[3] = buf[6];
            mon->stats[4] = buf[7];
            str = GetSpeciesName(team[k], PokeathlonCourse_GetHeapID(param_1));
            CopyU16StringArrayN(mon->name, String_cstr(str), 11);
            String_Delete(str);
        }
    }
    NARC_Delete(teamNarc);
    NARC_Delete(perfNarc);
    DestroyMsgData(msgData);
}
