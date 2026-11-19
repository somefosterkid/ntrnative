#include "global.h"

#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"
#include "map_object.h"

s32 ov103_021EDB18(void *, s32, s32);

s32 ov103_021EEB04(void);

s32 ov103_021ED8D0(void *arg0);

struct MapObject;
struct ObjectEventGfxRenderDetailsEntry;
typedef struct MapObject MapObject;
void ov103_021EDA84(MapObject *mapObj, int param1);
void ov103_021ED930(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

s32 ov103_021ED8D0(void *arg0) {
    if (ov103_021EEB04() == 1) {
        PlaySE(0x5DDU);
        return ov103_021EDB18(arg0, 3, 0x10);
    }
    return 0xF;
}

s32 ov103_021EDBC8(void *arg0);

s32 ov103_021EEAC8(void *, s32);

s32 ov103_021ED8F8(MailBoxWork *work);

s32 ov103_021ED8F8(MailBoxWork *work) {
    ov103_021EDBC8(work);
    ov103_021EEAC8(work, GridInputHandler_GetNextInput(work->view->gridInput));
    ov103_021EE628(work->view);
    sub_0201980C(work->view->frames, 0xA);
    ov103_021EDB60(work->view, 0);
    return 9;
}

void ov103_021ED930(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    ov103_021EDA84(mapObj, 0);
}
