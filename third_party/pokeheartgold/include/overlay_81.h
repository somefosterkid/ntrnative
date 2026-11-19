#ifndef POKEHEARTGOLD_OVERLAY_81_H
#define POKEHEARTGOLD_OVERLAY_81_H

#include "global.h"
#include "overlay_manager.h"
#include "bg_window.h"
#include "pokemon_types_def.h"
#include "msgdata.h"
#include "overlay_81_battle_factory_app.h"
#include "yes_no_prompt.h"

s32 ov81_0223DD60(OverlayManager *arg0, s32 *arg1);

s32 ov81_0223E234(OverlayManager *arg0);

s32 ov81_0223E520(void *arg0);

void ov81_0223E5B4(BattleFactoryApp *app);
void ov81_0223E87C(BattleFactoryApp *app);
void ov81_0223E8BC(BattleFactoryApp *app);
void ov81_0223EA98(BattleFactoryApp *app);

s32 ov81_0223EC88(void *arg0);


s32 ov81_0223F314(BattleFactoryApp *app);


s32 ov81_0223F6A8(void *arg0);



s32 ov81_0223FBAC(void *arg0);

void ov81_02240458(BgConfig *arg0);
void ov81_022404AC(BattleFactoryApp *app, s32 *arg1, s32 arg2);

void ov81_022408B8(BattleFactoryApp *app, u32 fieldno, BoxPokemon *boxmon);


void ov81_02241008(BattleFactoryApp *app, u16 cmd);

void ov81_02241020(int netID, int unused, void *unused2, void *unused3);

void ov81_0224113C(void);

void ov81_0224300C(Window *windows);
void ov81_02241238(s32 netID, s32 unused, void *data, void *context);

struct BattleFactoryApp;
struct BattleFactoryAppPanelSprite;
struct BattleFactoryAppPokeballSprite;
struct BattleFactoryAppPanelSprite *ov81_02241340(struct BattleFactoryApp *app, u32 animID);

void ov81_02241364(struct BattleFactoryApp *arg0);


void ov81_022413E0(Window *window);






void ov81_02242514(BattleFactoryApp *app);



void ov81_0224275C(struct BattleFactoryApp *arg0);

void ov81_02242F10(struct BattleFactoryAppPokeballSprite *ballSprite, int x, int y);

void ov81_02243028(Window *window, u32 frame);

void ov81_022430B4(void *arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void ov81_0224323C(void);

void ov81_02243298(s32 arg0, s32 arg1);

void ov81_0224335C(s32 arg0, s32 arg1);

void ov81_02243444(s32 arg0, s32 arg1);

void ov81_0223EBE4(BattleFactoryApp *app);
void ov81_0223EF5C(BattleFactoryApp *app);
void ov81_0223F038(BattleFactoryApp *app);
void ov81_0223F0BC(BattleFactoryApp *app);
void ov81_0223F684(BattleFactoryApp *app);
void ov81_0223FB3C(BattleFactoryApp *app);
void ov81_0223FB64(BattleFactoryApp *app);
void ov81_0223FB88(BattleFactoryApp *app);
BOOL ov81_022404B4(BattleFactoryApp *app);
BOOL ov81_02240564(BattleFactoryApp *app);
void ov81_022406E0(BattleFactoryApp *app, enum GFBgLayer layer);
void ov81_02240728(BattleFactoryApp *app, enum GFBgLayer layer);
u8 ov81_0224080C(BattleFactoryApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);
u8 ov81_0224086C(BattleFactoryApp *app, s32 param_2);
void ov81_022408A0(BattleFactoryApp *app, u32 idx, s32 num);
void ov81_02240BB0(BattleFactoryApp *app);
void ov81_02240E78(BattleFactoryApp *app);
void ov81_02240F38(BattleFactoryApp *app, u8 menuWasCancelled);
BOOL ov81_02240FA4(BattleFactoryApp *app, u16 cmd, u16 arg);
void ov81_02241144(BattleFactoryApp *app);
void ov81_02241218(BattleFactoryApp *app, u16 cmd, u16 tradedMon);
void ov81_02241BD0(YesNoPrompt *yesNoPrompt, BgConfig *bgConfig);
void ov81_02241398(BattleFactoryApp *app);
void ov81_02241450(BattleFactoryApp *app);
void ov81_022414E0(BattleFactoryApp *app);
void ov81_0224174C(BattleFactoryApp *app);
void ov81_022417B4(BattleFactoryApp *app);
void ov81_02241840(BattleFactoryApp *app);
void ov81_02241980(BattleFactoryApp *app);
void ov81_022419E0(BattleFactoryApp *app);
void ov81_02241A38(BattleFactoryApp *app);
void ov81_02241A7C(BattleFactoryApp *app);
void ov81_02241C0C(BattleFactoryApp *app);
void ov81_02241D38(BattleFactoryApp *app);
void ov81_02241D94(BattleFactoryApp *app);
void ov81_02241E68(BattleFactoryApp *app, u32 arg1, s32 slot, s32 arg3);
void ov81_02241EDC(BattleFactoryApp *app, u32 arg1, s32 slot, s32 arg3);
void ov81_02241FC0(BattleFactoryApp *app);
void ov81_02242170(BattleFactoryApp *app);
void ov81_022420B4(BattleFactoryApp *app);
void ov81_022423D0(BattleFactoryApp *app, struct Party *party);
void ov81_02242420(BattleFactoryApp *app, u16 arg1);
void ov81_0223F320(BattleFactoryApp *app);
void ov81_02242218(BattleFactoryApp *app, Party *party, u32 slot, u32 arg3);
void ov81_02242300(BattleFactoryApp *app, u32 arg1, u32 arg2);
void ov81_022424AC(BattleFactoryApp *app, u32 arg1);
void ov81_0224258C(BattleFactoryApp *app, u8 x, u8 y, u8 width, u8 height, u8 nextSubState);
void ov81_022425C4(BattleFactoryApp *app, u8 x, u8 y, u8 nextSubState);
void ov81_022425D8(BattleFactoryApp *app, u8 x, u8 y, u8 nextSubState);
u32 ov81_022425EC(BattleFactoryApp *app);
void ov81_02242694(BattleFactoryApp *app, s32 arg1);

#endif // POKEHEARTGOLD_OVERLAY_81_H
