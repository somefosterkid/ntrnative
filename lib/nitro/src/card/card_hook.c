// TwlSDK's CARD library, card_hook.c: Black's main 0x02076100-0x02076170. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "card_internal.h"

static CARDHookContext *CARDiHookChain = NULL;

void CARDi_NotifyEvent(CARDEvent event, void *arg) {
    OSIntrMode bak = OS_DisableInterrupts();
    CARDHookContext **pp = &CARDiHookChain;
    while (*pp) {
        CARDHookContext *hook = *pp;
        if (hook->callback) {
            (*hook->callback)(hook->userdata, event, arg);
        }

        if (*pp == hook) {
            pp = &(*pp)->next;
        }
    }
    OS_RestoreInterrupts(bak);
}
