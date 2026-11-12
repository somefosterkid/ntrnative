// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/scrcmd_vm.c, ported by tools/port_file.py.
#include "types.h"
#include "field/subscreen.h"
#include "save/records.h"
#include "field/event_sound.h"
#include "field/field_script.h"
#include "field/field_sound.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "gfl/key.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/field_effect.h"
#include "field/event_data.h"
#include "field/field_actor_internal.h"
#include "field/field_map.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/shortcut_util.h"
#include "field/field_actor_animation.h"
#include "field/field_move_scripts.h"
#include "field/scrcmd_actor_move.h"
#include "field/scrcmd_buildmodel.h"
#include "field/scrcmd_camera.h"
#include "field/scrcmd_encount.h"
#include "field/scrcmd_entree_forest.h"
#include "field/scrcmd_fade.h"
#include "field/scrcmd_fld_battle.h"
#include "field/scrcmd_fldmmdl.h"
#include "field/scrcmd_game_state.h"
#include "field/scrcmd_gym.h"
#include "field/scrcmd_item.h"
#include "field/scrcmd_mapchange.h"
#include "field/scrcmd_menu.h"
#include "field/scrcmd_msg.h"
#include "field/scrcmd_musical.h"
#include "field/scrcmd_net.h"
#include "field/scrcmd_place_name.h"
#include "field/scrcmd_pokemon.h"
#include "field/scrcmd_proc.h"
#include "field/scrcmd_shop.h"
#include "field/scrcmd_sound.h"
#include "field/scrcmd_sp_poke.h"
#include "field/scrcmd_sp_poke_gimmick.h"
#include "field/scrcmd_trainer.h"
#include "field/scrcmd_trial_house.h"
#include "field/scrcmd_word.h"
#include "field/script_network.h"
#include "field/stadium_script.h"
#include "field/survey.h"

// No header declares these yet
BOOL func_ov010_02159998(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0d14(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0d74(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0edc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0df0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0e74(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b1698(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0ef4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021aec84(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2a90(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021aff7c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cfa88(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b627c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b631c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b63d4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5b4c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b636c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215bb50(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215bbd0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5b8c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5bfc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5c80(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b65d8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5a18(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b69b0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cfa58(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_02159a64(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215f228(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215f238(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215f280(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215c5ec(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215c610(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4fb0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4d60(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cb08(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4f68(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cbf0(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cab4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4ee0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021ca57c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021ca544(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021ca598(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021ca5c4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215db38(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215db00(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217cf68(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217fae0(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217cf88(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217f648(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217fb2c(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217f8a0(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217f8d0(VM *vm, FieldScriptEnv *env);
BOOL func_ov018_0217f914(VM *vm, FieldScriptEnv *env);
BOOL func_ov048_021f4650(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2380(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b23f0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b240c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2444(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b247c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b24f4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b24b8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b268c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b269c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b252c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2568(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b25a4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cdf0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b25dc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2614(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2650(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b26ac(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b26e4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215ce70(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b277c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b27f8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b27b4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4f1c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d1b2c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d1b8c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d29f0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c812c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c8170(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c81b4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5610(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5650(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b568c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c881c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c8848(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c8868(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c88ac(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c88ec(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c8960(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b2aa0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cc544(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b654c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b647c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cc570(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cc5d8(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02186be4(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02186c44(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02186d48(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02186cc4(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02186eb8(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02186e94(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02187098(VM *vm, FieldScriptEnv *env);
BOOL func_ov020_02187000(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_02164778(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6ae0(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6afc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6b6c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5eb8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5df8(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215afe0(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215aff4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b014(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6a78(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b69d8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b60f4(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6164(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6124(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b61e8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6bdc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cc614(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cc6a8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cc6fc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d1670(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d16d8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d179c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d213c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2468(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b716c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b70d8(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b6dc(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b578(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b720(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b81c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b60c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4e58(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b4e9c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b7f8(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b504(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2524(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2710(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b464(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b488(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b7364(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b73b8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b62c8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b5ce8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6590(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021cf7dc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2adc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2b18(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216cc30(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2b54(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2edc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2f44(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021c81f8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b6628(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b66b8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b721c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b72cc(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d2f6c(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d3018(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d3068(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021d3090(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021aee78(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b0000(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215f2b8(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b23e0(VM *vm, FieldScriptEnv *env);

// Whether a comparison's result meets each condition of the conditional jumps and calls, by VMCmp's result (less,
// equal, greater)
const u8 VM_CMP_LUT[][3] = {
    { 1, 0, 0 }, // less
    { 0, 1, 0 }, // equal
    { 0, 0, 1 }, // greater
    { 1, 1, 0 }, // less or equal
    { 0, 1, 1 }, // greater or equal
    { 1, 0, 1 }, // not equal
};

// What finishes each of the script's sub events when it ends, by the IDs of FieldScriptSubEvent_Register
const FieldScriptSubEventFinishFunc FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS[15] = {
    FieldScriptSubEventFinish_EvCamera,
    func_ov021_021b0630,
    func_ov021_021b0ee8,
    func_ov021_021b10f4,
    func_ov021_021b13ec,
    func_ov021_021b1234,
    func_ov021_021b14e8,
    FieldScriptSubEventFinish_NDemo,
    func_ov021_021d1bd8,
    func_ov021_021b1604,
    FieldScriptSubEventFinish_MEPlayback,
    func_ov010_0215f2a0,
    func_ov021_021aefa4,
    FieldScriptSubEventFinish_BGMPlayback,
    NULL,
};

// The field script commands, by their IDs (data_ov010_021705d8 counts them)
const FieldScriptCommand data_ov010_0217062c[] = {
    s0000_VMNop,                                      // 0x000
    s0001_VMNop2,                                     // 0x001
    s0002_VMHalt,                                     // 0x002
    s0003_VMSleep,                                    // 0x003
    s0004_VMCall,                                     // 0x004
    s0005_VMReturn,                                   // 0x005
    s0006_DebugPrint,                                 // 0x006
    s0007_DebugStack,                                 // 0x007
    s0008_VMStackPushConst,                           // 0x008
    s0009_VMStackPush,                                // 0x009
    s000A_VMStackPop,                                 // 0x00a
    s000B_VMStackDiscard,                             // 0x00b
    s000C_VMStackAdd,                                 // 0x00c
    s000D_VMStackSub,                                 // 0x00d
    s000E_VMStackMul,                                 // 0x00e
    s000F_VMStackDiv,                                 // 0x00f
    s0010_VMStackPushFlag,                            // 0x010
    s0011_VMStackCmp,                                 // 0x011
    s0012_WorkAnd,                                    // 0x012
    s0013_WorkOr,                                     // 0x013
    s0014_VMRegSet8,                                  // 0x014
    s0015_VMRegSet32,                                 // 0x015
    s0016_VMRegMov,                                   // 0x016
    s0017_VMRegCmp8,                                  // 0x017
    s0018_VMRegCmpConst8,                             // 0x018
    s0019_WorkCmpConst,                               // 0x019
    s001A_WorkCmpWork,                                // 0x01a
    s001B_RTCallGlobalAsync,                          // 0x01b
    s001C_RTCallGlobal,                               // 0x01c
    s001D_RTEndGlobal,                                // 0x01d
    s001E_VMJump,                                     // 0x01e
    s001F_VMJumpIf,                                   // 0x01f
    s0020_VMCallIf,                                   // 0x020
    s0021_RTReserveScript,                            // 0x021
    s0022_FieldGetContinueFlag,                       // 0x022
    s0023_FlagSet,                                    // 0x023
    s0024_FlagReset,                                  // 0x024
    s0025_FlagGet,                                    // 0x025
    s0026_WorkAdd,                                    // 0x026
    s0027_WorkSub,                                    // 0x027
    s0028_WorkSetConst,                               // 0x028
    s0029_WorkGet,                                    // 0x029
    s002A_WorkSet,                                    // 0x02a
    s002B_WorkMul,                                    // 0x02b
    s002C_WorkDiv,                                    // 0x02c
    s002D_WorkMod,                                    // 0x02d
    s002E_ActorsPauseAll,                             // 0x02e
    s002F_ActorsUnpauseAll,                           // 0x02f
    s0030_FinishAllEvents,                            // 0x030
    s0031_ABKeyWait,                                  // 0x031
    func_ov010_02159998,                              // 0x032
    s0033_MsgSetAutoscrolls,                          // 0x033
    s0034_SystemMsg,                                  // 0x034
    s0035_SystemMsgAsync,                             // 0x035
    s0036_InfoMsgClose,                               // 0x036
    s0037_MsgSetLoadingSpinner,                       // 0x037
    s0038_InfoMsg,                                    // 0x038
    s0039_InfoMsgClose,                               // 0x039
    s003A_MultiMsg,                                   // 0x03a
    s003B_MsgWinCloseNo,                              // 0x03b
    func_ov021_021b0d14,                              // 0x03c
    func_ov021_021b0d74,                              // 0x03d
    func_ov021_021b0edc,                              // 0x03e
    s003F_MsgWinCloseAll,                             // 0x03f
    s0040_MoneyWinDisp,                               // 0x040
    s0041_MoneyWinClose,                              // 0x041
    s0042_MoneyWinUpdate,                             // 0x042
    s0043_MsgPlaceSign,                               // 0x043
    s0044_MsgPlaceSignClose,                          // 0x044
    s0045_CheckerMsg,                                 // 0x045
    s0046_CheckerMsgClose,                            // 0x046
    s0047_YesNoWin,                                   // 0x047
    func_ov021_021b0df0,                              // 0x048
    func_ov021_021b0e74,                              // 0x049
    s004A_ScreamMsg,                                  // 0x04a
    s004B_MsgWaitAdvance,                             // 0x04b
    s004C_WordSetPlayerName,                          // 0x04c
    s004D_WordSetItemName,                            // 0x04d
    s004E_WordSetItemNameEx,                          // 0x04e
    s004F_WordSetItemNameWithArticle,                 // 0x04f
    s0050_WordSetTMMoveName,                          // 0x050
    s0051_WordSetMoveName,                            // 0x051
    s0052_WordSetItemPocketName,                      // 0x052
    s0053_WordSetPartyPokeSpecies,                    // 0x053
    s0054_WordSetPartyPokeName,                       // 0x054
    s0055_WordSetDaycarePokeSpecies,                  // 0x055
    s0056_WordSetPokeTypeName,                        // 0x056
    s0057_WordSetPokeSpecies,                         // 0x057
    s0058_WordSetPokeSpeciesWithArticle,              // 0x058
    s0059_WordSetPlaceName,                           // 0x059
    s005A_WordSetTrendName,                           // 0x05a
    s005B_WordSetDaycarePokeName,                     // 0x05b
    s005C_WordSetNumber,                              // 0x05c
    s005D_WordSetMusicalInfo,                         // 0x05d
    s005E_WordSetCountry,                             // 0x05e
    s005F_WordSetHobbyName,                           // 0x05f
    s0060_WordSetPassPowerName,                       // 0x060
    s0061_WordSetTrainerClassName,                    // 0x061
    s0062_WordSetTrainerClassNameWithArticle,         // 0x062
    s0063_WordSetSurveyAnswer,                        // 0x063
    func_ov021_021b1698,                              // 0x064
    s0065_ActorCmdWait,                               // 0x065
    s0066_ActorGetMoveCode,                           // 0x066
    s0067_ActorGetGPos,                               // 0x067
    s0068_PlayerGetGPos,                              // 0x068
    s0069_ActorNew,                                   // 0x069
    s006A_ActorGetSpawnFlag,                          // 0x06a
    s006B_ActorAdd,                                   // 0x06b
    s006C_ActorDelete,                                // 0x06c
    s006D_ActorSetGPos,                               // 0x06d
    s006E_PlayerGetDir,                               // 0x06e
    s006F_PlayerGetActorInFront,                      // 0x06f
    s0070_ActorFindByGPos,                            // 0x070
    s0071_PlayerGetRailPos,                           // 0x071
    s0072_ActorGetRailPos,                            // 0x072
    s0073_ActorSetMoveCode,                           // 0x073
    s0074_ActorSetEyeToEye,                           // 0x074
    s0075_PlayerSetSpecialSequence,                   // 0x075
    s0076_PlayerMoveToYAsync,                         // 0x076
    s0077_PlayerTurnByTrigger,                        // 0x077
    s0078_PlayerGetExState,                           // 0x078
    s0079_ActorGetUserParam,                          // 0x079
    s007A_ActorPlayRailSlipdown,                      // 0x07a
    s007B_ActorJumpToGPos,                            // 0x07b
    s007C_PlayerSetRailPos,                           // 0x07c
    s007D_ActorSetRailPos,                            // 0x07d
    s007E_ActorPlayTeleportSeq,                       // 0x07e
    s007F_TrainerEyeGetTrainerID,                     // 0x07f
    s0080_TrainerEyeEventInit,                        // 0x080
    s0081_TrainerEyeEventStart,                       // 0x081
    s0082_TrainerGetActorID,                          // 0x082
    s0083_TrainerGetBattleType,                       // 0x083
    s0084_ActorGetTrainerID,                          // 0x084
    s0085_CallTrainerBattle,                          // 0x085
    s0085_CallTrainerMultiBattle,                     // 0x086
    func_ov021_021b0ef4,                              // 0x087
    s0088_TrainerGetMessageTypes,                     // 0x088
    NULL,                                             // 0x089
    s008A_TrainerGetBattleType,                       // 0x08a
    s008B_TrainerBGMPlayPush,                         // 0x08b
    s008C_CallTrainerLose,                            // 0x08c
    s008D_TrainerBattleIsVictory,                     // 0x08d
    func_ov021_021aec84,                              // 0x08e
    NULL,                                             // 0x08f
    NULL,                                             // 0x090
    NULL,                                             // 0x091
    s0092_TrainerGetFieldAction,                      // 0x092
    s0093_TrainerGetPrizeItem,                        // 0x093
    s0094_CallTradedPokemonBattle,                    // 0x094
    s0095_TrainerFlagSet,                             // 0x095
    s0096_TrainerFlagReset,                           // 0x096
    s0097_TrainerFlagGet,                             // 0x097
    s0098_BGMPlay,                                    // 0x098
    NULL,                                             // 0x099
    NULL,                                             // 0x09a
    s009B_BGMIsPlaying,                               // 0x09b
    NULL,                                             // 0x09c
    NULL,                                             // 0x09d
    s009E_BGMChangeMap,                               // 0x09e
    s009F_BGMPlayPush,                                // 0x09f
    s00A0_BGMWait,                                    // 0x0a0
    s00A1_BGMPush,                                    // 0x0a1
    s00A2_BGMPop,                                     // 0x0a2
    s00A3_ISSSwitchEnable,                            // 0x0a3
    s00A4_ISSSwitchDisable,                           // 0x0a4
    s00A5_ISSSwitchQuery,                             // 0x0a5
    s00A6_SEPlay,                                     // 0x0a6
    s00A7_SEStop,                                     // 0x0a7
    s00A8_SEWait,                                     // 0x0a8
    s0245_BGMFadeOut,                                 // 0x0a9
    s00AA_MEWait,                                     // 0x0aa
    s00AB_PVPlay,                                     // 0x0ab
    s00AC_PVWait,                                     // 0x0ac
    s00AD_ListMenuInitCommon,                         // 0x0ad
    s00AE_ListMenu_AnchorTopLeft,                     // 0x0ae
    s00AF_ListMenuAdd,                                // 0x0af
    s00B0_ListMenuShow,                               // 0x0b0
    s00B1_ListMenuShow2,                              // 0x0b1
    s00B2_ListMenu_AnchorTopRight,                    // 0x0b2
    s00B3_FadeEx,                                     // 0x0b3
    func_ov021_021b2a90,                              // 0x0b4
    s00B5_ItemAdd,                                    // 0x0b5
    s00B6_ItemSub,                                    // 0x0b6
    s00B7_ItemCheckSpace,                             // 0x0b7
    s00B8_ItemCheckAmount,                            // 0x0b8
    s00B9_ItemGetCount,                               // 0x0b9
    s00BA_ItemIsTMHM,                                 // 0x0ba
    s00BB_ItemGetPocket,                              // 0x0bb
    s00BC_PhenomenonGetItemID,                        // 0x0bc
    s00BD_ItemGetClass,                               // 0x0bd
    s00BE_MapChangeFake,                              // 0x0be
    s00BF_MapChangeWarpPad,                           // 0x0bf
    func_ov021_021aff7c,                              // 0x0c0
    s00C1_MapChangeQuicksand,                         // 0x0c1
    s00C2_MapChangeWarp,                              // 0x0c2
    s00C3_MapChangeUnionRoom,                         // 0x0c3
    s00C4_MapChangeCore,                              // 0x0c4
    s00C5_CallSurf,                                   // 0x0c5
    s00C6_CallWaterfall,                              // 0x0c6
    s00C7_CallCut,                                    // 0x0c7
    s00C8_CallDiving,                                 // 0x0c8
    func_ov010_0215aa60,                              // 0x0c9
    func_ov010_0215aa90,                              // 0x0ca
    s00CB_Random,                                     // 0x0cb
    s00CC_RTGetTextFile,                              // 0x0cc
    s00CD_RTCGetDayPart,                              // 0x0cd
    func_ov010_0215aaf4,                              // 0x0ce
    s00CF_RTCGetWeekDay,                              // 0x0cf
    s00D0_RTCGetDate,                                 // 0x0d0
    s00D1_RTCGetTime,                                 // 0x0d1
    s00D2_RTCGetSeason,                               // 0x0d2
    s00D3_RTGetZoneID,                                // 0x0d3
    s00D4_TrainerCardGetBirthDate,                    // 0x0d4
    s00D5_TrainerCardHasBadge,                        // 0x0d5
    s00D6_TrainerCardAddBadge,                        // 0x0d6
    s00D7_TrainerCardGetBadgeCount,                   // 0x0d7
    s00D8_MapReplaceIsEventSet,                       // 0x0d8
    s00D9_FieldSetTeleportZone,                       // 0x0d9
    s00DA_MapReplaceSetEvent,                         // 0x0da
    s00DB_FieldSetNextZoneHere,                       // 0x0db
    s00DC_FieldSetNextZone,                           // 0x0dc
    s00DD_PokeDexGetCount,                            // 0x0dd
    s00DE_PokeDexRegist,                              // 0x0de
    s00DF_PokeDexIsRegist,                            // 0x0df
    s00E0_GameGetVersion,                             // 0x0e0
    s00E1_TrainerCardGetSex,                          // 0x0e1
    s00E2_SaveDataCheckRequired,                      // 0x0e2
    s00E3_GiveRunningShoes,                           // 0x0e3
    func_ov010_0215b084,                              // 0x0e4
    func_ov010_0215b0b4,                              // 0x0e5
    func_ov010_0215b104,                              // 0x0e6
    func_ov010_0215b138,                              // 0x0e7
    func_ov010_0215b158,                              // 0x0e8
    func_ov021_021cfa88,                              // 0x0e9
    s00EA_HOFCheckIntegrity,                          // 0x0ea
    s00EB_DayCareCheckSpawnFlag,                      // 0x0eb
    s00EC_DayCareBreed,                               // 0x0ec
    s00ED_DayCareResetSeed,                           // 0x0ed
    s00EE_DayCareGetPkmCount,                         // 0x0ee
    s00EF_DayCareCalcEggSpawnChance,                  // 0x0ef
    s00F0_DayCareDeposit,                             // 0x0f0
    s00F1_DayCareWithdraw,                            // 0x0f1
    s00F2_DayCareGetSpecies,                          // 0x0f2
    s00F3_DayCareGetForme,                            // 0x0f3
    s00F4_DayCareCalcNewLevel,                        // 0x0f4
    s00F5_DayCareCalcLevelGain,                       // 0x0f5
    s00F6_DayCareCalcWithdrawCost,                    // 0x0f6
    s00F7_DayCareCallPokeSelect,                      // 0x0f7
    s00F8_DayCareGetSex,                              // 0x0f8
    s00F9_MoneyAdd,                                   // 0x0f9
    s00FA_MoneySub,                                   // 0x0fa
    s00FB_MoneyCheck,                                 // 0x0fb
    s00FC_PokePartyGetHappiness,                      // 0x0fc
    s00FD_PokePartyAdjustHappiness,                   // 0x0fd
    s00FE_PokePartyGetSpecies,                        // 0x0fe
    s00FF_PokePartyGetForme,                          // 0x0ff
    func_ov021_021b627c,                              // 0x100
    s0101_PokePartyIsFullHP,                          // 0x101
    s0102_PokePartyIsEgg,                             // 0x102
    s0103_PokePartyGetCount,                          // 0x103
    s0104_PokePartyRecoverAll,                        // 0x104
    func_ov021_021b631c,                              // 0x105
    func_ov021_021b63d4,                              // 0x106
    func_ov021_021b5b4c,                              // 0x107
    s0108_PokePartyGetMoveCount,                      // 0x108
    func_ov021_021b636c,                              // 0x109
    s010A_PokePartyGetMove,                           // 0x10a
    s010B_PokePartyLearnMove,                         // 0x10b
    func_ov010_0215bb50,                              // 0x10c
    s010D_PokePartyGetMemberByType,                   // 0x10d
    func_ov010_0215bbd0,                              // 0x10e
    s010F_PokePartyAddEgg,                            // 0x10f
    s0110_PokePartyGetParam,                          // 0x110
    s0111_PokePartySetIV,                             // 0x111
    s0112_PokePartyGetEVTotal,                        // 0x112
    s0113_PokePartyIsOriginGame,                      // 0x113
    s0114_PokePartyGetCountBySpecies,                 // 0x114
    s0115_PokePartyHasMove,                           // 0x115
    s0116_PokePartyHasMoveAny,                        // 0x116
    s0117_PokePartySetForme,                          // 0x117
    s0118_PokePartyFindBySpecies,                     // 0x118
    s0119_PokePartyIsFromWhiteForest,                 // 0x119
    s011A_PokePartyGetMetDate,                        // 0x11a
    s011B_PokePartyGetTypes,                          // 0x11b
    s011C_PokePartyChangeRotomForme,                  // 0x11c
    func_ov021_021b5b8c,                              // 0x11d
    func_ov021_021b5bfc,                              // 0x11e
    func_ov021_021b5c80,                              // 0x11f
    func_ov021_021b65d8,                              // 0x120
    s0121_BoxGetCount,                                // 0x121
    s0122_BoxAdd,                                     // 0x122
    s0123_BoxAddEx,                                   // 0x123
    s0124_BMHndAnmPlay,                               // 0x124
    s0125_BMPlayHOFMachineSeq,                        // 0x125
    s0126_BMChangeMdlID,                              // 0x126
    s0127_BMCreateHandleByGPos,                       // 0x127
    s0128_BMReleaseHandle,                            // 0x128
    s0129_BMHndAudioVisualAnmPlay,                    // 0x129
    s012A_BMHndAnmWait,                               // 0x12a
    s012B_BMAnmPlayInv,                               // 0x12b
    s012C_BMHndAnmPause,                              // 0x12c
    s012D_BMSetVisible,                               // 0x12d
    s012E_BMAnmPlayLoop,                              // 0x12e
    s012F_PokecenPlayHealingSequence,                 // 0x12f
    s0130_PokecenPCOpen,                              // 0x130
    s0131_PokecenPCIdle,                              // 0x131
    s0132_PokecenPCClose,                             // 0x132
    func_ov021_021b5a18,                              // 0x133
    func_ov021_021b69b0,                              // 0x134
    func_ov021_021cfa58,                              // 0x135
    s0136_FieldSetWeather,                            // 0x136
    func_ov010_02159a64,                              // 0x137
    s0138_SaveDataGetStatus,                          // 0x138
    s0139_GameCommDisconnect,                         // 0x139
    s013A_GameCommGetStatus,                          // 0x13a
    s013B_GameCommCheckDSiWiFi,                       // 0x13b
    func_ov010_0215f228,                              // 0x13c
    func_ov010_0215f238,                              // 0x13d
    func_ov010_0215f280,                              // 0x13e
    s013F_EvCameraInit,                               // 0x13f
    s0140_EvCameraEnd,                                // 0x140
    s0141_EvCameraUnbind,                             // 0x141
    s0142_EvCameraRebind,                             // 0x142
    s0143_EvCameraMoveTo,                             // 0x143
    s0144_EvCameraReturn,                             // 0x144
    s0145_EvCameraWait,                               // 0x145
    s0146_EvCameraMoveToCommon,                       // 0x146
    s0147_EvCameraMoveToDefault,                      // 0x147
    s0148_EvCameraShake,                              // 0x148
    s0149_CallFriendlyShopBuy,                        // 0x149
    func_ov010_0215c5ec,                              // 0x14a
    func_ov010_0215c610,                              // 0x14b
    s014C_RTFreeUserHeap,                             // 0x14c
    func_ov021_021b4fb0,                              // 0x14d
    s014E_CallBag,                                    // 0x14e
    func_ov021_021b4d60,                              // 0x14f
    s0150_CallMailbox,                                // 0x150
    func_ov010_0215cb08,                              // 0x151
    func_ov021_021b4f68,                              // 0x152
    func_ov010_0215cbf0,                              // 0x153
    s0154_Call3DDemo,                                 // 0x154
    func_ov010_0215cab4,                              // 0x155
    func_ov021_021b4ee0,                              // 0x156
    func_ov021_021ca57c,                              // 0x157
    func_ov021_021ca544,                              // 0x158
    func_ov021_021ca598,                              // 0x159
    func_ov021_021ca5c4,                              // 0x15a
    s0157_ActorAnimationInit,                         // 0x15b
    s0158_ActorAnimationFree,                         // 0x15c
    s0159_ActorAnimationPlay,                         // 0x15d
    s015A_ActorAnimationWait,                         // 0x15e
    func_ov021_021b4c14,                              // 0x15f
    func_ov021_021b4c34,                              // 0x160
    s015D_NetConnectWiFiClub,                         // 0x161
    s015E_NetConnectGTS,                              // 0x162
    func_ov010_0215cc38,                              // 0x163
    s0160_NetConnectWiFiBattle,                       // 0x164
    s0161_NetConnectBattleVideo,                      // 0x165
    s0162_NetConnectGTSNegotiation,                   // 0x166
    func_ov010_0215d34c,                              // 0x167
    func_ov010_0215d468,                              // 0x168
    func_ov010_0215d504,                              // 0x169
    func_ov010_0215d7d4,                              // 0x16a
    func_ov010_0215db38,                              // 0x16b
    func_ov010_0215d864,                              // 0x16c
    func_ov010_0215dadc,                              // 0x16d
    func_ov010_0215db00,                              // 0x16e
    func_ov018_0217cf68,                              // 0x16f
    func_ov018_0217fae0,                              // 0x170
    func_ov018_0217cf88,                              // 0x171
    func_ov010_0215c95c,                              // 0x172
    func_ov018_0217f648,                              // 0x173
    func_ov018_0217fb2c,                              // 0x174
    func_ov018_0217f8a0,                              // 0x175
    func_ov018_0217f8d0,                              // 0x176
    func_ov018_0217f914,                              // 0x177
    s0297_CallWildBattleEx,                           // 0x178
    s0175_CallWildBattleEnd,                          // 0x179
    s0176_CallWildLose,                               // 0x17a
    s0177_WildBattleIsVictory,                        // 0x17b
    func_ov021_021b50d0,                              // 0x17c
    s0179_CallCaptureDemo,                            // 0x17d
    func_ov048_021f4650,                              // 0x17e
    func_ov010_0215cd3c,                              // 0x17f
    func_ov021_021b2380,                              // 0x180
    func_ov021_021b23b8,                              // 0x181
    func_ov010_0215cd80,                              // 0x182
    func_ov021_021b23f0,                              // 0x183
    func_ov021_021b240c,                              // 0x184
    func_ov021_021b2444,                              // 0x185
    func_ov010_0215cdb8,                              // 0x186
    func_ov021_021b247c,                              // 0x187
    func_ov021_021b24f4,                              // 0x188
    func_ov021_021b24b8,                              // 0x189
    func_ov021_021b268c,                              // 0x18a
    func_ov021_021b269c,                              // 0x18b
    func_ov021_021b252c,                              // 0x18c
    func_ov021_021b2568,                              // 0x18d
    func_ov021_021b25a4,                              // 0x18e
    func_ov010_0215cdf0,                              // 0x18f
    func_ov021_021b25dc,                              // 0x190
    func_ov021_021b2614,                              // 0x191
    func_ov021_021b2650,                              // 0x192
    func_ov021_021b26ac,                              // 0x193
    func_ov021_021b26e4,                              // 0x194
    func_ov021_021b271c,                              // 0x195
    func_ov010_0215ce70,                              // 0x196
    func_ov021_021b273c,                              // 0x197
    func_ov021_021b277c,                              // 0x198
    func_ov021_021b27f8,                              // 0x199
    func_ov021_021b27b4,                              // 0x19a
    func_ov021_021b4f1c,                              // 0x19b
    func_ov021_021d1b2c,                              // 0x19c
    func_ov021_021d1b8c,                              // 0x19d
    func_ov021_021d29f0,                              // 0x19e
    func_ov021_021c812c,                              // 0x19f
    func_ov021_021c8170,                              // 0x1a0
    func_ov021_021c81b4,                              // 0x1a1
    func_ov021_021b5610,                              // 0x1a2
    func_ov021_021b5650,                              // 0x1a3
    func_ov021_021b568c,                              // 0x1a4
    func_ov021_021c881c,                              // 0x1a5
    func_ov021_021c8848,                              // 0x1a6
    func_ov021_021c8868,                              // 0x1a7
    func_ov021_021c88ac,                              // 0x1a8
    func_ov010_02166734,                              // 0x1a9
    func_ov021_021c88ec,                              // 0x1aa
    s01A2_CallRoyalUnovaView,                         // 0x1ab
    func_ov021_021c8960,                              // 0x1ac
    s01A3_FadeInBlackQ,                               // 0x1ad
    s01A4_FadeOutBlackQ,                              // 0x1ae
    s01A5_FadeInWhiteQ,                               // 0x1af
    s01A6_FadeOutWhiteQ,                              // 0x1b0
    func_ov021_021b2aa0,                              // 0x1b1
    s01A8_FadeInBlackQ_,                              // 0x1b2
    NULL,                                             // 0x1b3
    s01AA_CallSeasonBanner,                           // 0x1b4
    s01AB_FadeInBlack,                                // 0x1b5
    s01AC_FadeOutBlack,                               // 0x1b6
    s01AD_FadeInWhite,                                // 0x1b7
    s01AE_FadeOutWhite,                               // 0x1b8
    func_ov021_021b5700,                              // 0x1b9
    func_ov021_021b574c,                              // 0x1ba
    func_ov021_021cc544,                              // 0x1bb
    func_ov021_021b5154,                              // 0x1bc
    func_ov010_0216bf38,                              // 0x1bd
    func_ov021_021b654c,                              // 0x1be
    func_ov021_021b647c,                              // 0x1bf
    func_ov021_021cc570,                              // 0x1c0
    func_ov021_021cc5d8,                              // 0x1c1
    func_ov020_02186be4,                              // 0x1c2
    func_ov020_02186c44,                              // 0x1c3
    func_ov020_02186d48,                              // 0x1c4
    func_ov020_02186cc4,                              // 0x1c5
    func_ov020_02186eb8,                              // 0x1c6
    func_ov020_02186e94,                              // 0x1c7
    func_ov020_02187098,                              // 0x1c8
    func_ov020_02187000,                              // 0x1c9
    func_ov010_02164778,                              // 0x1ca
    func_ov021_021b6ae0,                              // 0x1cb
    func_ov021_021b6afc,                              // 0x1cc
    func_ov021_021b6b6c,                              // 0x1cd
    func_ov021_021b5eb8,                              // 0x1ce
    func_ov021_021b5df8,                              // 0x1cf
    func_ov010_0215afe0,                              // 0x1d0
    func_ov010_0215aff4,                              // 0x1d1
    func_ov010_0215b014,                              // 0x1d2
    s01C9_NDemoStart,                                 // 0x1d3
    s01CA_NDemoEnd,                                   // 0x1d4
    s01CB_NDemoReadyTalkMotion,                       // 0x1d5
    func_ov021_021b6a78,                              // 0x1d6
    func_ov021_021b69d8,                              // 0x1d7
    func_ov021_021cfea0,                              // 0x1d8
    func_ov021_021cff28,                              // 0x1d9
    func_ov021_021cff78,                              // 0x1da
    func_ov021_021cffdc,                              // 0x1db
    s02DB_UnityTowerSetFloor,                         // 0x1dc
    func_ov010_02166980,                              // 0x1dd
    s01CF_UnityTowerGetStateParam,                    // 0x1de
    s02DC_UnityTowerInitVisitorMessage,               // 0x1df
    s01D0_UnityTowerCallFloorSelect,                  // 0x1e0
    s02DD_UnityTowerGetVisitorCount,                  // 0x1e1
    s02DE_UnityTowerSetHobby,                         // 0x1e2
    s02DF_UnityTowerGetHobby,                         // 0x1e3
    func_ov021_021b60f4,                              // 0x1e4
    func_ov021_021b6164,                              // 0x1e5
    func_ov021_021b6124,                              // 0x1e6
    func_ov021_021b61e8,                              // 0x1e7
    s01D5_MoveReminderCheckPkm,                       // 0x1e8
    s01D6_MoveReminderCallMoveSelect,                 // 0x1e9
    s01D7_ObjInitPointGPos,                           // 0x1ea
    s01D8_ObjInitWarpGPos,                            // 0x1eb
    s01D9_ObjInitNPCGPos,                             // 0x1ec
    s01DA_CallPhraseSelect,                           // 0x1ed
    func_ov021_021b6bdc,                              // 0x1ee
    func_ov010_0215e1c0,                              // 0x1ef
    func_ov021_021cc614,                              // 0x1f0
    func_ov021_021cc6a8,                              // 0x1f1
    func_ov021_021cc6fc,                              // 0x1f2
    s01E0_StadiumSetupActorsDouble,                   // 0x1f3
    s01E1_StadiumLoadTrainerTable,                    // 0x1f4
    s01E2_StadiumFreeTrainerTable,                    // 0x1f5
    s01E3_StadiumSetupActorSingle,                    // 0x1f6
    s01E0_StadiumSetupActorsTriple,                   // 0x1f7
    s01E5_StadiumResetTrainerFlags,                   // 0x1f8
    s01E6_TrialHouseWorkInit,                         // 0x1f9
    s01E7_TrialHouseWorkDelete,                       // 0x1fa
    s01E8_TrialHousePrepareParty,                     // 0x1fb
    s01E9_TrialHouseCallTeamSelect,                   // 0x1fc
    func_ov010_0216a4cc,                              // 0x1fd
    s01EB_TrialHouseMsgDisp,                          // 0x1fe
    s01EC_TrialHouseStartBattle,                      // 0x1ff
    func_ov010_0216a594,                              // 0x200
    func_ov010_0216a5c4,                              // 0x201
    s01EF_TrialHouseGetBattleTestRank,                // 0x202
    func_ov010_0216a634,                              // 0x203
    s01F1_TrialHouseCalcPointsStars,                  // 0x204
    func_ov010_0216a6b4,                              // 0x205
    s01F3_TrialHouseSaveData,                         // 0x206
    func_ov010_0216a72c,                              // 0x207
    func_ov010_0216a774,                              // 0x208
    func_ov021_021d1670,                              // 0x209
    func_ov021_021d16d8,                              // 0x20a
    func_ov021_021d179c,                              // 0x20b
    func_ov021_021d213c,                              // 0x20c
    func_ov021_021d2468,                              // 0x20d
    func_ov021_021b716c,                              // 0x20e
    func_ov021_021b70d8,                              // 0x20f
    func_ov010_0216b63c,                              // 0x210
    func_ov010_0216b6dc,                              // 0x211
    func_ov010_0216b578,                              // 0x212
    s0200_SurveyGetCurrentAnswerIDs,                  // 0x213
    func_ov010_0216b720,                              // 0x214
    func_ov010_0216b81c,                              // 0x215
    func_ov010_0216b86c,                              // 0x216
    func_ov010_0216b60c,                              // 0x217
    func_ov021_021b4e58,                              // 0x218
    func_ov021_021b4e9c,                              // 0x219
    func_ov021_021afc38,                              // 0x21a
    func_ov021_021afc88,                              // 0x21b
    func_ov010_0216b7b0,                              // 0x21c
    func_ov010_0216b7f8,                              // 0x21d
    func_ov010_0216b504,                              // 0x21e
    func_ov010_0216b538,                              // 0x21f
    s020D_PokePartyFindEx,                            // 0x220
    func_ov010_0216bbd4,                              // 0x221
    func_ov010_0216bc88,                              // 0x222
    func_ov010_0216bce4,                              // 0x223
    func_ov010_0216bd1c,                              // 0x224
    func_ov010_0216a7a0,                              // 0x225
    func_ov010_0216a884,                              // 0x226
    func_ov021_021d2524,                              // 0x227
    func_ov010_0216bec0,                              // 0x228
    func_ov021_021d2710,                              // 0x229
    s0217_MapChangeEntreeForest,                      // 0x22a
    func_ov010_0216ad70,                              // 0x22b
    func_ov010_0216accc,                              // 0x22c
    s021A_WordSetLoadEntreeForestPkmName,             // 0x22d
    s021B_EntreeForestSpawnAllPkm,                    // 0x22e
    s021C_EntreeForestStartBattle,                    // 0x22f
    func_ov010_0216ac90,                              // 0x230
    func_ov021_021b51bc,                              // 0x231
    func_ov010_0215b464,                              // 0x232
    func_ov010_0215b488,                              // 0x233
    func_ov021_021b7364,                              // 0x234
    func_ov021_021b73b8,                              // 0x235
    func_ov021_021b62c8,                              // 0x236
    func_ov021_021b5ce8,                              // 0x237
    func_ov021_021b6590,                              // 0x238
    func_ov021_021cf7dc,                              // 0x239
    func_ov021_021d2adc,                              // 0x23a
    func_ov021_021d2b18,                              // 0x23b
    func_ov010_0216cc30,                              // 0x23c
    func_ov021_021d2b54,                              // 0x23d
    func_ov021_021d2edc,                              // 0x23e
    func_ov021_021d2f44,                              // 0x23f
    func_ov021_021c81f8,                              // 0x240
    func_ov021_021b1a20,                              // 0x241
    func_ov021_021b6628,                              // 0x242
    func_ov021_021b1334,                              // 0x243
    s0174_CallWildBattle,                             // 0x244
    func_ov021_021b66b8,                              // 0x245
    func_ov021_021d0114,                              // 0x246
    s0234_ActorFallDownToXZ,                          // 0x247
    s0235_BGMPlayEx,                                  // 0x248
    func_ov021_021b721c,                              // 0x249
    func_ov021_021b72cc,                              // 0x24a
    s0238_BGMChangeMapEx,                             // 0x24b
    func_ov021_021b2040,                              // 0x24c
    func_ov021_021af3d8,                              // 0x24d
    func_ov010_0216f8b8,                              // 0x24e
    func_ov010_0216f8c8,                              // 0x24f
    func_ov010_0216f8dc,                              // 0x250
    s023E_DayCareGetSexForNamePrint,                  // 0x251
    s023F_CallPlaceNameDisp,                          // 0x252
    func_ov021_021aef10,                              // 0x253
    func_ov021_021aef60,                              // 0x254
    func_ov021_021d2f6c,                              // 0x255
    func_ov021_021d3018,                              // 0x256
    func_ov021_021d3068,                              // 0x257
    func_ov021_021d3090,                              // 0x258
    func_ov021_021aee78,                              // 0x259
    func_ov021_021b0000,                              // 0x25a
    s0248_PlayerMoveToYAsync_,                        // 0x25b
    func_ov010_0215f52c,                              // 0x25c
    func_ov010_0215f2b8,                              // 0x25d
    s024B_FieldSubscreenDisable,                      // 0x25e
    s024C_BGMAmbienceResume,                          // 0x25f
    func_ov021_021b23e0,                              // 0x260
};

const u32 data_ov010_021705d8 = NELEMS(data_ov010_0217062c);

BOOL s0000_VMNop(VM *vm, FieldScriptEnv *env) {
    return FALSE;
}

BOOL s0001_VMNop2(VM *vm, FieldScriptEnv *env) {
    return FALSE;
}

BOOL s0002_VMHalt(VM *vm, FieldScriptEnv *env) {
    VM_Halt(vm);
    return TRUE;
}

BOOL swapToScrcmdEnvirDecPauseCtr(VM *vm, void *env) {
    return FieldScriptEnv_UpdateWaitCounter(env);
}

BOOL s0003_VMSleep(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_SetWaitCounter(env, VM_Read16(vm));
    VM_SetNativeCallback(vm, swapToScrcmdEnvirDecPauseCtr);
    return TRUE;
}

BOOL s0004_VMCall(VM *vm, FieldScriptEnv *env) {
    s32 offset = VM_Read32(vm);

    VM_Call(vm, vm->pc + offset);
    return FALSE;
}

BOOL s0005_VMReturn(VM *vm, FieldScriptEnv *env) {
    VM_Return(vm);
    return FALSE;
}

BOOL s0006_DebugPrint(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetGameData(env);
    ScriptReadAny(vm, env);
    return FALSE;
}

BOOL s0007_DebugStack(VM *vm, FieldScriptEnv *env) {
    VM_StackPop(vm);
    ScriptReadAny(vm, env);
    return FALSE;
}

BOOL s0014_VMRegSet8(VM *vm, FieldScriptEnv *env) {
    u8 index = VM_Read8(vm);
    u8 value = VM_Read8(vm);

    vm->work[index] = value;
    return FALSE;
}

BOOL s0015_VMRegSet32(VM *vm, FieldScriptEnv *env) {
    u8 index = VM_Read8(vm);
    u32 value = VM_Read32(vm);

    vm->work[index] = value;
    return FALSE;
}

BOOL s0016_VMRegMov(VM *vm, FieldScriptEnv *env) {
    u8 dst = VM_Read8(vm);
    u8 src = VM_Read8(vm);

    vm->work[dst] = vm->work[src];
    return FALSE;
}

u8 VMCmp(u32 left, u32 right) {
    if (left < right) {
        return 0;
    }
    if (left == right) {
        return 1;
    }
    return 2;
}

BOOL s0017_VMRegCmp8(VM *vm, FieldScriptEnv *env) {
    u8 left = vm->work[VM_Read8(vm)];
    u8 right = vm->work[VM_Read8(vm)];

    vm->cmpResult = VMCmp(left, right);
    return FALSE;
}

BOOL s0018_VMRegCmpConst8(VM *vm, FieldScriptEnv *env) {
    u8 left = vm->work[VM_Read8(vm)];
    u8 right = VM_Read8(vm);

    vm->cmpResult = VMCmp(left, right);
    return FALSE;
}

BOOL s0019_WorkCmpConst(VM *vm, FieldScriptEnv *env) {
    u16 left = *ScriptReadVar(vm, env);
    u16 right = VM_Read16(vm);

    vm->cmpResult = VMCmp(left, right);
    return FALSE;
}

BOOL s001A_WorkCmpWork(VM *vm, FieldScriptEnv *env) {
    u16 *left = ScriptReadVar(vm, env);
    u16 *right = ScriptReadVar(vm, env);

    vm->cmpResult = VMCmp(*left, *right);
    return FALSE;
}

BOOL s0008_VMStackPushConst(VM *vm, FieldScriptEnv *env) {
    VM_StackPush(vm, VM_Read16(vm));
    return FALSE;
}

BOOL s0009_VMStackPush(VM *vm, FieldScriptEnv *env) {
    VM_StackPush(vm, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s000A_VMStackPop(VM *vm, FieldScriptEnv *env) {
    u32 value = VM_StackPop(vm);

    *ScriptReadVar(vm, env) = value;
    return FALSE;
}

BOOL s000B_VMStackDiscard(VM *vm, FieldScriptEnv *env) {
    VM_StackPop(vm);
    return FALSE;
}

BOOL s000C_VMStackAdd(VM *vm, FieldScriptEnv *env) {
    u32 right = VM_StackPop(vm);
    u32 left = VM_StackPop(vm);

    VM_StackPush(vm, left + right);
    return FALSE;
}

BOOL s000D_VMStackSub(VM *vm, FieldScriptEnv *env) {
    u32 right = VM_StackPop(vm);
    u32 left = VM_StackPop(vm);

    VM_StackPush(vm, left - right);
    return FALSE;
}

BOOL s000E_VMStackMul(VM *vm, FieldScriptEnv *env) {
    u32 right = VM_StackPop(vm);
    u32 left = VM_StackPop(vm);

    VM_StackPush(vm, left * right);
    return FALSE;
}

BOOL s000F_VMStackDiv(VM *vm, FieldScriptEnv *env) {
    u32 right = VM_StackPop(vm);
    u32 left = VM_StackPop(vm);

    VM_StackPush(vm, left / right);
    return FALSE;
}

BOOL s0010_VMStackPushFlag(VM *vm, FieldScriptEnv *env) {
    EventWork *eventWork = GameData_GetEventWork(FieldScriptEnv_GetGameData(env));
    u16 flag = ScriptReadAny(vm, env);

    VM_StackPush(vm, EventWork_FlagGet(eventWork, flag));
    return FALSE;
}

BOOL s0011_VMStackCmp(VM *vm, FieldScriptEnv *env) {
    u32 right = VM_StackPop(vm);
    u32 left = VM_StackPop(vm);
    u16 comparison = VM_Read16(vm);
    BOOL result;

    switch (comparison) {
    case 0:
        result = left < right;
        break;
    case 1:
        result = left == right;
        break;
    case 2:
        result = left > right;
        break;
    case 3:
        result = left <= right;
        break;
    case 4:
        result = left >= right;
        break;
    case 5:
        result = left != right;
        break;
    case 6:
        result = left == 1 || right == 1;
        break;
    case 7:
        result = left == 1 && right == 1;
        break;
    default:
        result = FALSE;
        break;
    }
    VM_StackPush(vm, result);
    return FALSE;
}

BOOL s001B_RTCallGlobalAsync(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work;
    u16 scriptId;
    u16 zoneId;

    FieldScriptEnv_GetGameSystem(env);
    work = FieldScriptEnv_GetScriptWork(env);
    scriptId = VM_Read16(vm);
    zoneId = FieldScriptEnv_GetZoneID(env);
    ScriptWork_AddVM(work, zoneId, scriptId);
    return TRUE;
}

BOOL ScriptNative_WaitFinishSubScript(VM *vm, void *data) {
    FieldScriptEnv *env = data;
    ScriptWork *work;
    u8 index;

    work = FieldScriptEnv_GetScriptWork(env);
    index = FieldScriptEnv_GetVMIndex(env);
    if (!FieldScript_VMExists(work, index)) {
        FieldScriptEnv_Restore(env);
        return TRUE;
    }
    return FALSE;
}

BOOL s001C_RTCallGlobal(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    ScriptWork *work;
    u16 scriptId;
    u32 level;
    u32 index;

    gsys = FieldScriptEnv_GetGameSystem(env);
    work = FieldScriptEnv_GetScriptWork(env);
    scriptId = VM_Read16(vm);
    if (!FieldScriptEnv_IsReducedFeatureLevel(env)) {
        index = ScriptWork_AddVM(work, FieldScriptEnv_GetZoneID(env), scriptId);
        SetScrEnvVMIndex(env, index);
        FieldScriptEnv_Save(env);
        VM_SetNativeCallback(vm, ScriptNative_WaitFinishSubScript);
        return TRUE;
    }
    FieldScriptEnv_Save(env);
    level = FieldScriptEnv_GetFeatureLevel(env);
    FieldScript_Run(gsys, work, scriptId, level);
    FieldScriptEnv_Restore(env);
    return FALSE;
}

BOOL s001D_RTEndGlobal(VM *vm, FieldScriptEnv *env) {
    FieldScriptEnv_GetScriptWork(env);
    VM_Halt(vm);
    return TRUE;
}

BOOL s001E_VMJump(VM *vm, FieldScriptEnv *env) {
    s32 offset = VM_Read32(vm);

    VM_Jump(vm, vm->pc + offset);
    return FALSE;
}

BOOL s001F_VMJumpIf(VM *vm, FieldScriptEnv *env) {
    u8 condition = VM_Read8(vm);
    s32 offset = VM_Read32(vm);

    if (condition == 0xff) {
        if (VM_StackPop(vm) != 1) {
            VM_Jump(vm, vm->pc + offset);
        }
    } else if (VM_CMP_LUT[condition][vm->cmpResult] == 1) {
        VM_Jump(vm, vm->pc + offset);
    }
    return FALSE;
}


BOOL s0020_VMCallIf(VM *vm, FieldScriptEnv *env) {
    u8 condition = VM_Read8(vm);
    s32 offset = VM_Read32(vm);

    if (VM_CMP_LUT[condition][vm->cmpResult] == 1) {
        VM_Call(vm, vm->pc + offset);
    }
    return FALSE;
}

BOOL s0021_RTReserveScript(VM *vm, FieldScriptEnv *env) {
    u16 scriptId = VM_Read16(vm);
    FieldStatus *status = GameData_GetFieldStatus(FieldScriptEnv_GetGameData(env));

    FieldStatus_ReserveScript(status, scriptId);
    return FALSE;
}

BOOL s0022_FieldGetContinueFlag(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    FieldStatus *status = GameData_GetFieldStatus(FieldScriptEnv_GetGameData(env));

    *value = FieldStatus_CheckContinueFlag(status);
    return FALSE;
}

BOOL s0023_FlagSet(VM *vm, FieldScriptEnv *env) {
    EventWork *eventWork = GameData_GetEventWork(FieldScriptEnv_GetGameData(env));
    u16 flag = ScriptReadAny(vm, env);

    EventWork_FlagSet(eventWork, flag);
    return FALSE;
}

BOOL s0024_FlagReset(VM *vm, FieldScriptEnv *env) {
    EventWork *eventWork = GameData_GetEventWork(FieldScriptEnv_GetGameData(env));
    u16 flag = ScriptReadAny(vm, env);

    EventWork_FlagReset(eventWork, flag);
    return FALSE;
}

BOOL s0025_FlagGet(VM *vm, FieldScriptEnv *env) {
    EventWork *eventWork = GameData_GetEventWork(FieldScriptEnv_GetGameData(env));
    u16 flag = ScriptReadAny(vm, env);
    u16 *value = ScriptReadVar(vm, env);

    *value = EventWork_FlagGet(eventWork, flag);
    return FALSE;
}

BOOL s0026_WorkAdd(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value += operand;
    return FALSE;
}

BOOL s0027_WorkSub(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value -= operand;
    return FALSE;
}

BOOL s002B_WorkMul(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value *= operand;
    return FALSE;
}

BOOL s002C_WorkDiv(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value /= operand;
    return FALSE;
}

BOOL s002D_WorkMod(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value %= operand;
    return FALSE;
}

BOOL s0012_WorkAnd(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value &= operand;
    return FALSE;
}

BOOL s0013_WorkOr(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);
    u16 operand = ScriptReadAny(vm, env);

    *value |= operand;
    return FALSE;
}

BOOL s0028_WorkSetConst(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);

    *value = VM_Read16(vm);
    return FALSE;
}

BOOL s0029_WorkGet(VM *vm, FieldScriptEnv *env) {
    u16 *dst = ScriptReadVar(vm, env);
    u16 *src = ScriptReadVar(vm, env);

    *dst = *src;
    return FALSE;
}

BOOL s002A_WorkSet(VM *vm, FieldScriptEnv *env) {
    u16 *value = ScriptReadVar(vm, env);

    *value = ScriptReadAny(vm, env);
    return FALSE;
}

extern BOOL func_ov021_021b1bb8(VM *vm, FieldScriptEnv *env);
BOOL s002E_ActorsPauseAll(VM *vm, FieldScriptEnv *env) {
    return func_ov021_021b1bb8(vm, env);
}


BOOL s002F_ActorsUnpauseAll(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    BOOL wait = FALSE;

    EnableAllActorsMovementScr(env);
    if (FieldSnd_BGMGetStackIndex(GameData_GetFieldSoundSystem(gameData)) != 0) {
        ScriptWork_CallEvent(work, EventBGMPopAll_Create(gsys, 0));
        wait = TRUE;
    }
    return wait;
}

BOOL s0030_FinishAllEvents(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameEvent *event;

    FieldScriptEnv_GetGameSystem(env);
    event = EventFinishScriptSubEvents_Create(env);
    if (event == NULL) {
        return TRUE;
    }
    ScriptWork_CallEvent(work, event);
    return TRUE;
}

BOOL testAB(VM *vm, void *env) {
    return (GCTX_HIDGetPressedKeys() & 3) != 0;
}

BOOL s0031_ABKeyWait(VM *vm, FieldScriptEnv *env) {
    VM_SetNativeCallback(vm, testAB);
    return TRUE;
}

BOOL ScriptNative_LastKeyWait(VM *vm, void *env) {
    return (GCTX_HIDGetPressedKeys() & 0xf3) != 0;
}

BOOL func_ov010_02159998(VM *vm, FieldScriptEnv *env) {
    VM_SetNativeCallback(vm, ScriptNative_LastKeyWait);
    return TRUE;
}

BOOL func_ov010_021599a8(VM *vm, FieldScriptEnv *env) {
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    FieldSubscreen *subscreen = Field_GetSubscreen(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
    BOOL wait;

    if (GameData_CheckEventsPaused(gameData) == 0) {
        wait = TRUE;
        if (FieldSubscreen_GetScreenID(subscreen) == 7 && func_ov021_021a153c(subscreen) == 0) {
            wait = FALSE;
        }
    } else {
        // BUG: wait is left unset for any other level
        switch (func_02012dd0(gameData)) {
        case 0:
        case 1:
            wait = FALSE;
            break;
        case 2:
            *ScriptReadVar(vm, env) = 1;
            wait = FALSE;
            break;
        case 3:
            *ScriptReadVar(vm, env) = 0;
            wait = TRUE;
            if (FieldSubscreen_GetScreenID(subscreen) == 7) {
                func_ov021_021a1548(subscreen);
            }
            break;
        }
    }

    if (wait == TRUE) {
        FieldSaveAnime *effect = func_ov010_021590fc(work);
        func_ov021_021d11d8(effect);
        func_ov021_021d1194(effect);
        func_ov010_021590f4(work, NULL);
    }
    return wait;
}

extern void func_02012dac(GameData *gameData);
BOOL func_ov010_02159a64(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    Field *field = GSYS_GetField(FieldScriptEnv_GetGameSystem(env));
    HeapID heapID = Field_GetHeapID(field);
    FieldSubscreen *subscreen = Field_GetSubscreen(field);
    FieldSaveAnime *effect;

    effect = func_ov021_021d1164(heapID, field);
    func_ov021_021d11b0(effect);
    if (FieldSubscreen_GetScreenID(subscreen) == 7) {
        func_ov021_021a1524(subscreen);
        func_ov021_021a1530(subscreen);
    }
    func_ov010_021590f4(work, effect);
    RecordAddOne(GameData_GetRecords(gameData), 1);
    func_02012dac(gameData);
    VM_SetNativeCallback(vm, (VMCommand)func_ov010_021599a8);
    return TRUE;
}




u16 *ScriptReadVar(VM *vm, FieldScriptEnv *env) {
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 id = VM_Read16(vm);

    return ScriptWork_GetWkAddr(work, gameData, id);
}

u16 ScriptReadAny(VM *vm, FieldScriptEnv *env) {
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 value = VM_Read16(vm);

    return ScriptWork_ResolveHybridValue(work, gameData, value);
}

struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct BoxSaveAccessor {
    SaveControl *save;
};
struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[5];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    u8 saving;
    u8 is30FPSMode;
    u8 seasons[4];
    PassPowerLevel levels[4];
    CityState cityStates[2];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    u32 unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    u32 unk6A8;
    PlayTime *playTime;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};

u16 FieldScriptEnv_GetZoneID(FieldScriptEnv *env) {
    u16 zoneId;
    GameData *gameData;
    PlayerState *playerState;

    gameData = FieldScriptEnv_GetGameData(env);
    playerState = GameData_GetPlayerState(gameData);
    zoneId = PlayerState_GetZoneID(playerState);
    return zoneId;
}
