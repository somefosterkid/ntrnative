#include "global.h"

#include "sprite_system.h"
#include "unk_0201956C.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

struct SpriteManager;

struct UnkStruct_0201956C;

typedef s32 (*UnkFunc_ov103_021ED2D4)(MailBoxWork *data, void *fn, u32 offset, u32 a3);

extern const UnkFunc_ov103_021ED2D4 ov103_021EECA8[];

BOOL ov103_021ED2D4(MailBoxWork *data, s32 *state, u32 a2, u32 a3);

BOOL ov103_021ED2D4(MailBoxWork *data, s32 *state, u32 a2, u32 a3) {
    s32 cur = *state;
    UnkFunc_ov103_021ED2D4 fn = ov103_021EECA8[cur];
    s32 ret;
    MailBoxView *work;

    /* the asm calls through the pointer with r1 = the pointer and r2 = state * 4 */
    ret = fn(data, (void *)fn, (u32)cur * 4, a3);
    *state = ret;
    if (ret == 0x19) {
        return FALSE;
    }
    work = data->view;
    if (work != NULL) {
        sub_02019934(work->frames);
        ov103_021EDF68(data->view);
        SpriteSystem_DrawSprites(data->view->spriteManager);
    }
    return TRUE;
}
