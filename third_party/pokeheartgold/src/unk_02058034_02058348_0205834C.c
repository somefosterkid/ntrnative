#include "global.h"

#include "field_communication_manager.h"

/* HDR-OVERLAY {"sub_02037E1C": {"file": "include/unk_02037C94.h", "new": "void sub_02037E1C(u8 arg0);", "old": "void sub_02037E1C(s8 arg0);"}} */
#define sub_02037E1C sub_02037E1C__hdr

/* HDR-OVERLAY defines done */
#include "unk_02037C94.h"

/* HDR-OVERLAY end */
#undef sub_02037E1C

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02037E1C(s8 arg0);
#else
void sub_02037E1C(u8 arg0);
#endif

/* HDR-OVERLAY done */
s32 sub_020582F4(s32 *, u16);

extern s32 sub_0205836C;

void sub_0205834C(void);

void sub_0205834C(void) {
    sub_02037E1C(_021D41C8->connectID);
    sub_020582F4(&sub_0205836C, 0);
}
