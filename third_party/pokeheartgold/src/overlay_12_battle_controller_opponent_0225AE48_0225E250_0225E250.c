#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_02261FD4.h"
#include "battle/battle_controller.h"
#include "battle/battle_cursor.h"
#include "battle/battle_finger.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_input.h"
#include "battle/battle_system.h"

#include "bag_cursor.h"
#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "game_stats.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "move.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sav_chatot.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "trainer_data.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200CE7C.h"
#include "unk_02013534.h"

struct Background;

struct Bag;

struct BagCursor;

struct BagCursorBattle;

struct BagCursorField;

struct BallGaugeAnimation;

struct BattleContext;

struct BattleCursor;

struct BattleFinger;

struct BattleHpBar;

struct BattleInput;

struct BattleInputColor;

struct BattleInputFeedback;

struct BattleInputFightMenu;

struct BattleInputMainMenu;

struct BattleInputMove;

struct BattleInputMoveMemory;

struct BattleInputMoveMemory_DisplayObj;

struct BattleInputTargetMenu;

struct BattleInputTextObject;

struct BattleInputTutorial;

struct BattleInputTwoOptionMenu;

struct BattleInput_UnkSub14;

struct BattleMenuCursor;

struct BattleMessage;

struct BattleMon;

struct BattleNumberPrinter;

struct BattleSystem;

struct BgConfig;

struct BoxPokemon;

struct CAPSULE;

struct FieldConditionData;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResHeader;

struct GF_2DGfxResMan;

struct GF_2DGfxResObjList;

struct GameStats;

struct GetterWork;

struct ItemData;

struct ItemPartyParam;

struct ItemSlot;

struct Mail;

struct MailMessage;

struct ManagedSprite;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MoveFailFlags;

struct MoveTbl;

struct MsgData;

struct NARC;

struct OpponentData;

struct Options;

struct PC_Box;

struct PaletteBuffer;

struct PaletteData;

struct Party;

struct PartyCore;

struct PartyExtra;

struct PartyExtraSub;

struct PartyPokemon;

struct PlayerActions;

struct PlayerProfile;

struct Pokedex;

struct PokemonStats;

struct PokemonStorageSystem;

struct Pokepic;

struct PokepicAnimScript;

struct PokepicDrawParam;

struct PokepicManager;

struct PokepicShadow;

struct PokepicTemplate;

struct S16Pos;

struct SEAL;

struct SOUND_CHATOT;

struct SelectedPaletteData;

struct SelfTurnData;

struct SideConditionData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct SpriteSystem;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TargetPokemon;

struct TextOBJ;

struct Trainer;

struct TrainerAIData;

struct TrainerData;

struct TurnData;

struct UnkBallData;

struct UnkBattleSystemSub17C;

struct UnkBattleSystemSub220;

struct UnkBattlemonSub;

struct UnkStruct_02021AC8;

struct UnkStruct_134;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct UnkStruct_134 {
    s32 unk0;
    enum HeapID heapID;
    s32 unk8;
    s32 unkC;
    int ball;
    s32 unk14;
    s32 unk18;
    SpriteSystem *spriteSystem;
    PaletteData *paletteData;
    BattleSystem *battleSystem;
} UnkStruct_134;

typedef struct {
    BattleSystem *battleSys;
    BattleHpBar *healthbox;
    int input;
    u16 moves[4];
    u8 ppCur[4];
    u8 ppMax[4];
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 partySlot;
    u8 state;
} UnkStruct_ov12_0225E250;

typedef struct {
    u16 moves[4];
    u8 ppCur[4];
    u8 ppMax[4];
    u8 battlerType;
    u8 pad[3];
} UnkStruct_ov12_0225E250_MenuData;



void ov12_0225E250(SysTask *task, void *data);

void ov12_0225E250(SysTask *task, void *data) {
    UnkStruct_ov12_0225E250 *d = data;
    BattleInput *input;
    OpponentData *opponentData;
    int partner;
    BattleHpBar *healthbox;

    BattleSystem_GetBgConfig(d->battleSys);
    input = BattleSystem_GetBattleInput(d->battleSys);
    opponentData = BattleSystem_GetOpponentData(d->battleSys, d->battler);
    partner = BattleSystem_GetBattlerIdPartner(d->battleSys, d->battler);

    if (partner != d->battler) {
        healthbox = BattleSystem_GetHpBar(d->battleSys, partner);
    } else {
        healthbox = 0L;
    }

    switch (d->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(input) == 0) {
            break;
        } else {
            MsgData *msgLoader = BattleSystem_GetMessageLoader(d->battleSys);
            BattleMessage msg;
            UnkStruct_ov12_0225E250_MenuData menuData;
            NARC *bgNarc;
            NARC *objNarc;
            int i;

            msg.tag = 2;
            msg.param[0] = d->battler | (d->partySlot << 8);
            msg.id = 0x399;
            BattleSystem_PrintBattleMessage(d->battleSys, msgLoader, &msg, 0);

            BattleInput_EnableBallGauge(input);

            bgNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            objNarc = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);

            for (i = 0; i < 4; i++) {
                menuData.moves[i] = d->moves[i];
                menuData.ppCur[i] = d->ppCur[i];
                menuData.ppMax[i] = d->ppMax[i];
            }
            menuData.battlerType = d->battlerType;

            BattleInput_ChangeMenu(bgNarc, objNarc, input, 0xb, 0, (int *)&menuData);
            NARC_Delete(bgNarc);
            NARC_Delete(objNarc);
            d->state++;
            return;
        }
    case 1:
        d->input = BattleInput_CheckTouch(input);
        if (d->input != -1) {
            PlaySE(0x5dd);
            d->state++;
        }
        break;
    case 2:
        if (d->input != 0xff) {
            if ((BattleSystem_GetBattleType(d->battleSys) & 2) == 0) {
                BattleInput_Deadstriped_022698AC(input, 0);
                ov12_02265D74(healthbox);
                ov12_02264EB4(d->healthbox);
                ov12_02262014(opponentData);
            }
            BattleInput_DisableBallGauge(input);
        }
        BattleController_EmitSelectedMove(d->battleSys, d->battler, d->input);
        d->state++;
        break;
    case 3:
        d->state++;
    default:
        if (ov12_022698B0(input) == 1) {
            ov12_0223BB10(d->battleSys, 1);
            BattleController_EmitClearCommand(d->battleSys, d->battler, d->command);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
