#include "global.h"

#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_82.h"
#include "palette.h"
#include "pm_string.h"
#include "unk_0203A3B0.h"
#include "overlay_82_battle_hall_app.h"

s32 ov82_0223EBB8(BgConfig *);

s32 ov82_0223FBBC(void *);

void *ov82_0223FC9C();

s32 ov82_0223FD5C(void *);

SysTask *ov82_0223FDC0(void *);

void ov82_0223E8C4(void *arg0) {
    s32 v0;

    if (*((s32 *)&((BattleHallApp *)arg0)->cursorSprite) != 0) {
        ov82_0223FC9C();
    }
    if (*((s32 *)&((BattleHallApp *)arg0)->monSprite) != 0) {
        ov82_0223FC9C();
    }
    ov82_0223FDC0(*((void **)&((BattleHallApp *)arg0)->unk_8C));
    sub_0203A914();
    PaletteData_FreeBuffers(((BattleHallApp *)arg0)->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(((BattleHallApp *)arg0)->plttData, PLTTBUF_MAIN_BG);
    PaletteData_Free(((BattleHallApp *)arg0)->plttData);
    ((BattleHallApp *)arg0)->plttData = NULL;
    ov82_0223FBBC(arg0 + 0xA8);
    DestroyMsgData(((MsgData *)((BattleHallApp *)arg0)->msgLoader));
    MessageFormat_Delete(((MessageFormat *)((BattleHallApp *)arg0)->strTemplate));
    String_Delete(((BattleHallApp *)arg0)->displayStr);
    String_Delete(((BattleHallApp *)arg0)->fmtStr);
    MessagePrinter_Delete(((MessagePrinter *)((BattleHallApp *)arg0)->specialChars));
    v0 = 0;
    do {
        String_Delete(((BattleHallApp *)arg0)->yesNoStrs[v0]);
        v0 += 1;
    } while (v0 < 2);
    ov82_0223FD5C(arg0 + 0x4C);
    ov82_0223EBB8(*((void **)&((BattleHallApp *)arg0)->bgConfig));
    NARC_Delete(((BattleHallApp *)arg0)->narc);
}
