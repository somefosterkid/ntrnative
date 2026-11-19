#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_022051EC.h"
#include "overworld_anim_manager.h"

extern const OverworldAnimManagerFuncs ov01_022096CC;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_022051EC.s (same label), with the context size from the type, not the DS literal.
BOOL ov01_02205300(void *param0, UnkOv01_022051EC_Work *work);
void ov01_02205354(void *param0, UnkOv01_022051EC_Work *work);
void ov01_02205350(void);
void ov01_02205364(void *param0, UnkOv01_022051EC_Work *work);
const OverworldAnimManagerFuncs ov01_022096CC = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_022051EC_Work, ov01_02205300, ov01_02205354, ov01_02205350, ov01_02205364);
#endif

OverworldAnimManager *ov01_022052C4(FieldEffectManager *fieldEffMan, VecFx32 *pos) {
    UnkOv01_022051EC_Data data;

    data.fieldEffMan = fieldEffMan;
    data.ctx = ov01_021F1450(fieldEffMan, 0x16);
    return ov01_021F1620(fieldEffMan, &ov01_022096CC, pos, 0, &data, 0xFF);
}

s32 ov01_022052F4(OverworldAnimManager *manager) {
    UnkOv01_022051EC_Work *work = sub_02068D74(manager);

    return work->state;
}
