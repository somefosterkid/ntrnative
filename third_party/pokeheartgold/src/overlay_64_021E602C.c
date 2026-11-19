#include "global.h"
struct HallOfFameShowcase;

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "overlay_64_hall_of_fame_showcase.h"

s32 ov64_021E6010(void *, u32);

void ov64_021E602C(struct HallOfFameShowcase *arg0);

void ov64_021E602C(HallOfFameShowcase *arg0) {
    u32 v0;

    v0 = 0;
    do {
        ov64_021E6010(arg0, v0);
        v0 += 1;
    } while (v0 < 0x12U);
    SpriteSystem_FreeResourcesAndManager(arg0->spriteSystem, arg0->spriteManager);
    SpriteSystem_Free(arg0->spriteSystem);
}

void ov64_021E605C(HallOfFameShowcase *arg0);

void ov64_021E605C(HallOfFameShowcase *arg0) {
    ManagedSprite *managedSprite;
    u32 v0;

    v0 = 0;
    do {
        managedSprite = arg0->sprites[v0];
        if (managedSprite != NULL) {
            ManagedSprite_TickFrame(managedSprite);
        }
        v0 += 1;
    } while (v0 < 0x12U);
}

void ov64_021E607C(struct HallOfFameShowcase *arg0);

void ov64_021E607C(HallOfFameShowcase *arg0) {
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xB5, HEAP_ID_59);
    arg0->speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xED, HEAP_ID_59);
    arg0->moveNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2EE, HEAP_ID_59);
    arg0->msgFormat = MessageFormat_New(HEAP_ID_59);
    arg0->stringBuf = String_New(0x100U, HEAP_ID_59);
    arg0->stringBuf2 = String_New(0x100U, HEAP_ID_59);
    arg0->nickname = String_New(0x100U, HEAP_ID_59);
    arg0->otName = String_New(0x100U, HEAP_ID_59);
    arg0->mon.nickname = (String *)arg0->nickname;
    arg0->mon.otname = (String *)arg0->otName;
}

void ov64_021E6118(struct HallOfFameShowcase *arg0);

void ov64_021E6118(HallOfFameShowcase *arg0) {
    String_Delete(arg0->otName);
    String_Delete(arg0->nickname);
    String_Delete(arg0->stringBuf2);
    String_Delete(arg0->stringBuf);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->moveNames);
    DestroyMsgData(arg0->speciesNames);
    DestroyMsgData(arg0->msgData);
}
