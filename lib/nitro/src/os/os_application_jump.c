// TwlSDK's OS library, os_application_jump.c: Black's main 0x020883d4-0x02088498 (OSi_InitPrevTitleId, the end of
// OS_Init on a DSi), and its twl/ltdmain_begin.h blocks in the LTD autoload's .ltdmain 0x0273131c-0x027313ac and
// .ltdmain.bss 0x02732f9c-0x02732fa4, which ltd_autoload_0's delinks.txt names os_application_jump@ltd.c
// (tools/two_modules.py). libntr's (MIT, https://github.com/cybervisi0n/libntr) libraries/os/src/os_application_jump.c
// for SDK 5 has them in these places; Black's link keeps only these two functions of the file, by Black's names
// (pokebw2's lib/nitro/src/os/os_application_jump.c and lib/twl/src/os/os_application_jump_twl.c, GPL-3.0).

#include "nitro/os.h"
#include "twl/os.h"

#include "twl/ltdmain_begin.h"
// TwlSDK's s_prevTitleId: the ID of the program that ran before this one, which OS_Init keeps
static u64 OSi_SenderTitleId = 0;
#include "twl/ltdmain_end.h"

#include "twl/ltdmain_begin.h"
// TwlSDK's OSi_SearchTitleIdFromList: the system's title of the ID (where it matches under the mask), or 0
u64 OSi_FindSystemTitleId(u64 titleId, u64 mask) {
    const OSTitleIdList *list = (const OSTitleIdList *)HW_OS_TITLE_ID_LIST;
    u8 count = list->count;
    int i;

    if (count > OS_TITLE_ID_LIST_MAX) {
        count = OS_TITLE_ID_LIST_MAX;
    }
    for (i = 0; i < count; i++) {
        u64 id = OS_TITLE_IDS[i];
        if ((titleId & mask) == (mask & id) && (id & OS_TITLE_ID_HIDDEN) == 0) {
            return list->titleIds[i];
        }
    }
    return 0;
}

#include "twl/ltdmain_end.h"

// TwlSDK's OSi_InitPrevTitleId: keeps the ID of the program that ran before, from the arguments it left, and forgets
// it (and them) when it is no system title or its game code starts with 0 to 4
void func_020883d4(void)
{
    u8 buf[0x300];
    s8 c;

    OS_InitDeliverArg(buf, 0);
    if (OS_DecodeDeliverArg() == 0) {
        OSi_SenderTitleId = OS_GetTitleIdFromDeliverArg();
    }
    if (!((OSi_SenderTitleId == 0 || OSi_FindSystemTitleId(OSi_SenderTitleId, -1) != 0)
        && (c = (s8)(OSi_SenderTitleId >> 24)) != 0x30 && c != 0x31 && c != 0x32 && c != 0x33 && c != 0x34)) {
        OSi_SenderTitleId = 0;
        OS_InitDeliverArg(buf, 0);
        OS_EncodeDeliverArg();
    } else {
        OS_FinalizeDeliverArg();
    }
}
