#include "global.h"
struct BattleFactoryApp;

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokepic.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"
#include "heap.h"
#include "overlay_81.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "unk_02088288.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_0223E8B0(void *);

s32 ov81_0223EC44(void *);

void ov81_022401C8(void *);

s32 ov81_022403C0(void *);

s32 ov81_02240448(void *);

s32 ov81_02240F08(struct BattleFactoryApp *arg0, s32);

void ov81_02242500(BattleFactoryApp *arg0);

void ov81_0224271C(BattleFactoryApp *arg0);

s32 ov81_02243100(BgConfig *, NARC *);

void ov81_02240D64(struct BattleFactoryApp *arg);

extern u8 _02243458;
s32 ov81_02240F08(struct BattleFactoryApp *arg0, s32 arg1);

void ov81_02240D64(BattleFactoryApp *arg) {
    s32 v1;

    arg->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_100);
    ov81_022403C0(arg);
    ov81_02240448(arg);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_100);
    *((MsgData * *)&arg->msgLoader) = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xC2, HEAP_ID_100);
    *((MessageFormat * *)&arg->strTemplate) = MessageFormat_New(HEAP_ID_100);
    arg->displayStr = String_New(0x320U, HEAP_ID_100);
    arg->fmtStr = String_New(0x320U, HEAP_ID_100);
    v1 = 0;
    do {
        arg->menuStr[v1] = String_New(0x40U, HEAP_ID_100);
        v1 += 1;
    } while (v1 < 4);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_100);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_100);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_100);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_100);
    *((PokepicManager * *)&arg->monSpriteMan) = PokepicManager_Create(HEAP_ID_100);
    if (sub_02037474() != 0) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        sub_0203A880();
    }
    if (ov81_02240F08(arg, 0) == 1) {
        ov81_0223E87C(arg);
        ov81_0223E8B0(arg);
    } else {
        ov81_0223EBE4(arg);
        ov81_0223EC44(arg);
    }
    ov81_02242500(arg);
    ov81_02243100(*((void **)&arg->bgConfig), arg->narc);
    ov81_0224271C(arg);
    GfGfx_BothDispOn();
    Main_SetVBlankIntrCB(ov81_022401C8, arg);
}

void ov81_02240E78(BattleFactoryApp *app) {
    *((PokemonSummaryArgs **)&app->monSummary) = Heap_Alloc(HEAP_ID_100, sizeof(PokemonSummaryArgs));
    memset(((PokemonSummaryArgs *)app->monSummary), 0, sizeof(PokemonSummaryArgs));
    (((PokemonSummaryArgs *)app->monSummary))->party = app->personalParty;
    (((PokemonSummaryArgs *)app->monSummary))->options = app->options;
    (((PokemonSummaryArgs *)app->monSummary))->unk11 = 1;
    (((PokemonSummaryArgs *)app->monSummary))->unk12 = 1;
    (((PokemonSummaryArgs *)app->monSummary))->partyCount = app->numPokeballs;
    (((PokemonSummaryArgs *)app->monSummary))->partySlot = (u8) app->unk_468;
    (((PokemonSummaryArgs *)app->monSummary))->moveToLearn = 0;
    (((PokemonSummaryArgs *)app->monSummary))->natDexEnabled = SaveArray_IsNatDexEnabled(app->saveData);
    (((PokemonSummaryArgs *)app->monSummary))->unk2C = 0;
    (((PokemonSummaryArgs *)app->monSummary))->isFlag982Set = 0;
    sub_02089D40(((PokemonSummaryArgs *)app->monSummary), &_02243458);
    sub_0208AD34(((PokemonSummaryArgs *)app->monSummary), Save_PlayerData_GetProfile(app->saveData));
}

s32 ov81_02240F08(BattleFactoryApp *arg0, s32 arg1) {
    if (arg0->isExchangeMode == arg1) {
        return 1;
    }
    return 0;
}
