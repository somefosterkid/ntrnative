#include "global.h"

#include "heap.h"
#include "sound.h"
#include "sound_chatot.h"
#include "sound_radio.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "wild_encounter.h"

BOOL sub_02006A0C(u16 species, u8 form);

BOOL sub_02006C14(int handleNo, int playerNo, int bankNo, int playerPrio, u16 seqNo, u8 checkGB);

void sub_02005464(int seqID, int handleType);

BOOL PlayCry(u16 species, u8 form) {
    u8 *p12;
    void **p24;
    u8 *p35;
    int result;
    p12 = GF_SdatGetAttrPtr(0x12);
    p24 = GF_SdatGetAttrPtr(0x24);
    p35 = GF_SdatGetAttrPtr(0x35);
    if (sub_02006A0C(species, form) == 1) {
        species = 0x1EE;
    }
    if (species != 0x1EE) {
        if ((u32)species > 0x1EF || species == 0) {
            species = 1;
        }
    }
    if (species == 0x1B9) {
        if (sub_02006D04(*p24, 0, 0x7F, 0) == 1) {
            sub_02006E3C(0);
            return TRUE;
        }
    }
    if (*p12 == 0) {
        if (*p35 == 0) {
            sub_02006300(0);
        }
        result = sub_02006C14(1, -1, species, -1, 2, 0);
        sub_02005464(species, 1);
    } else {
        result = sub_02006C14(8, -1, species, -1, 2, 0);
        sub_02005464(species, 8);
    }
    sub_02006E3C(0);
    return result;
}
