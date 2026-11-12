// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field.h, the lines in use so far.
#ifndef BWPORT_FIELD_FIELD_H
#define BWPORT_FIELD_FIELD_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "system/bmp_menulist.h"
#include "gfl/g3d.h"

u16 Field_GetPlayerStateZoneID(Field *field);
void BeginForcePlaceNameDisp(PlaceName *placeName, u32 zoneId);
FieldActor *FieldPlayer_GetActor(FieldPlayer *player);
void FieldPlayer_GetWPos(FieldPlayer *player, VecFx32 *pos);
void FieldPlayer_SetWPos(FieldPlayer *player, VecFx32 *pos);
MMSys *Field_GetActorSystem(Field *field);
FieldCamera *Field_GetCameraSystem(Field *field);
NoGridMapper *Field_GetNoGridMapper(Field *field);
GameSystem *Field_GetGameSystem(Field *field);
TCBManager *Field_GetTCBMgr(Field *field);
EncountSystem *Field_GetEncountSystem(Field *field);
EncEff *Field_GetEncEff(Field *field);
u16 Field_GetHeapID(Field *field);
PlaceName *Field_GetPlaceName(Field *field);
FieldPlayer *Field_GetPlayer(Field *field);
FieldG3DMapper *Field_GetG3DMapper(Field *field);
FieldSubscreen *Field_GetSubscreen(Field *field);
void *FieldG2D_GetDispControl(Field *field);
void *Field_GetFieldEffects(Field *field);
// pokebw2's func_ov036_02180f80
u32 func_ov021_021896f4(GameCommSys *comm);
void *Field_GetMsgBGSys(Field *field);
// pokebw2's func_ov036_02187998 and func_ov036_0218799c
PrintQueue *func_ov021_021904a8(FieldMsgBG *msgBGSys);
Font *func_ov021_021904ac(FieldMsgBG *msgBGSys);
ActorMsgWin *ActorMsgWin_CheckAndCreate(FieldMsgBG *msgBGSys, int a1, const VecFx32 *pos, StrBuf *strbuf, u32 a4, u32 a5);
// pokebw2's func_ov036_02188884, func_ov036_021887d4 and func_ov036_021887f4
BOOL func_ov021_02191388(ActorMsgWin *msgWin);
void func_ov021_021912d8(ActorMsgWin *msgWin);
BOOL func_ov021_021912f8(ActorMsgWin *msgWin);
void *Field_AllocGimmickWorkBlock(Field *field, u32 id, HeapID heapId, u32 size);
void *Field_GetGimmickWorkBlock(Field *field, u32 id);
void Field_DeleteGimmickWorkBlock(Field *field, u32 id);
FieldExpObjSystem *Field_GetExpObjSystem(Field *field);
void FieldG2D_Prepare3DSurface(Field *field);
void FieldG2D_SetLCDConfig(void);
u32 Field_GetResolvedControllerTypeID(Field *field);
FieldFog *Field_GetFog(Field *field);
FieldTaskManager *Field_GetTaskManager(Field *field);
BOOL func_ov021_02189734(GameCommSys *comm);


// The heap block ListMenuUI_Create makes: the windows of the list and the list itself.
struct ListMenuUI {
    FieldMsgBG *msgBG;
    FieldMsgBGWin *win;
    BmpWin *window;
    ListMenuOption *options;
    BmpMenuList *list;
};

// The grid position in front of the player, facing dir
void GetPlayerGPosPlusDir(FieldPlayer *player, u16 dir, s16 *x, s16 *y, s16 *z);
void FieldCameraBoundary_ChangeZone(Field *field, u32 zoneId, HeapID heapId);

void func_ov010_02168eec(GameData *gameData);

// Black 2's; twinned code confirms the offsets of count, unk02, y, width and height in Black
// A list menu window on the field's message BG. ListMenuRequest_Set completes the request with the number of
// options, the position and the size, which CalcListMenuWidth and CalcListMenuHeight give in tiles
struct ListMenuRequest {
    u16 count;
    u16 unk02;
    u8 unk04;
    u8 unk05;
    u8 unk06;
    u8 unk07_0 : 4;
    u8 unk07_4 : 4;
    u8 unk08_0 : 4;
    u8 unk08_4 : 4;
    u8 unk09;
    u16 unk0A_0 : 3;
    u16 unk0A_3 : 4;
    u16 unk0A_7 : 9;
    s32 unk0C_0 : 2;
    s32 unk0C_2 : 30;
    u16 unk10;
    // The height of a row, in pixels
    u16 rowHeight;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    BmpMenuListPrintCallback printCallback;
};

// Where a balloon over an actor goes, from where the player stands, and the offset and window position of each
u8 ActorMsgWin_CalcWinPosAuto(FieldActor *player, const VecFx32 *pos);
void func_ov021_021b09d8(const VecFx32 *pos, VecFx32 *offset, G3DCamera *g3dCamera, FieldCamera *camera, u8 winPos);
void func_ov021_021b09ec(u32 winPos, u32 *a1, u32 *a2);

// Black 2's, not checked against Black
// One of N's Pokémon, which createNPkm makes
struct NPokeSpec {
    // The zones it appears in, from firstZone on
    u16 firstZone : 14;
    u16 zoneCount : 2;
    u16 species;
    u8 level;
    u8 nature;
    u8 sex;
    // 0 or 1 for the species' abilities, 2 for its hidden ability
    u8 ability;
};

// The unit vector of the direction on the grid, and the rail position a step in the direction on rails
void func_ov021_021a3484(FieldPlayer *player, u32 dir, VecFx32 *vec);

// Whether the player is on a catwalk, and takes them off it
BOOL func_ov021_021a3624(FieldPlayer *player);
void func_ov021_021a3644(FieldPlayer *player);

// The player's object code for a sex, in a form or an extra state
u16 FieldPlayer_GetObjCodeByForme(u32 sex, u32 forme);
void FieldPlayer_GetGPos(FieldPlayer *player, s16 *x, s16 *y, s16 *z);

u32 GetObjectProjectionMatrixOffset(u16 zoneId);
void SetupLoadZoneMapTypeData(u16 zoneId, AreaData *area, FieldG3DMapperConfig *config, MapMatrix *matrix);
u32 GetFieldmapZoneHeapSize(u16 zoneId);
BOOL FieldmapProc_Update(GameProc *proc, u32 *seq, void *param, void *work);
void Field_Free(Field *field);
void FieldG2D_Init(Field *field);
void Field_FreeGraphicsSystems(Field *field);
void FieldG3D_InitCallback(void);
void FldActSys_VRAMUploadFunc(BOOL type, u32 dest, void *src, u32 size);
void Field_LoadEdgeColorTable(AreaData *area, u16 zoneId);
void Field_SwapZoneBGM(Field *field, u32 zoneId);
void Field_SwapWeather(Field *field, u32 zoneId);
void Field_UpdatePlayerStateZoneID(GameData *gameData, u32 zoneId);
void Field_SwapCameraBoundaries(Field *field, u32 zoneId);
fx32 func_ov021_02189a80(Field *field);
BOOL func_ov021_02189a48(Field *field);
fx32 func_ov021_02189a78(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void EncEff_StartEvent(EncEff *encEff, GameEvent *event, u32 effect);
u16 FieldPlayer_GetFaceDir(FieldPlayer *player);
u32 func_ov021_021a30b0(FieldPlayer *player);
u32 Field_GetRenderMode(Field *field);
void Field_SetRenderMode(Field *field, u32 mode);
u32 Field_GetDayPeriod(Field *field);
void *Field_GetNDemoDataHandle(Field *field);
void Field_SetCasteliaRush(Field *field, CasteliaRush *rush);
void Field_SetPlayerPosPtr(Field *field, VecFx32 *position);
// The money window that the scripts show on the field's message BG
void *Field_GetMoneyWin(Field *field);
void Field_SetMoneyWin(Field *field, void *moneyWin);
// The money window of the field's message BG
MsgBGSysWin *FieldMsgBG_CreateMoneyWin(FieldMsgBG *msgBGSys, MsgData *msgData, u16 a2, u16 a3, u16 a4, u16 a5);
// Turns on or off the alpha blending of the field's message BG
void setAlphaBlend_wrapper(BOOL enable);
void AppendListMenuOption(ListMenuOption *options, StrBuf *text, u32 value, HeapID heapId);
// BMPMENULIST_NULL until an option is chosen (its value: a number or a pointer) or the menu is cancelled
intptr_t ListMenuUI_Update(ListMenuUI *menu);
void FieldPlayer_SetDirection(FieldPlayer *player, u16 dir);
void FieldFadeTCB_Start(GameSystem *gsys, Field *field, u32 a2, u32 a3, u32 a4);
BOOL FieldTaskManager_IsIdle(FieldTaskManager *taskManager);
// Whether a fade that FieldFadeTCB_Start started is still running
u32 Field_GetFadeFlag(Field *field);
void Field_SetSeasonBannerOverdrawFlag(Field *field, BOOL flag);
BOOL Field_CheckMapLoadFinished(Field *field);
u32 ZoneData_GetObjectProjectionMatrixType(u16 zoneId);
u32 GetZoneMapType2(u16 zoneId);
BOOL Field_CallRoutines(GameSystem *gsys, Field *field);
void Field_RequestClose(Field *field);
// A message window on the field's message BG: create, update (0 for the first answer, 2 while waiting) and free
ListMenuUI *func_ov021_02190be4(FieldMsgBG *msgBGSys, u32 a1);
u32 func_ov021_02190c78(ListMenuUI *window);
void func_ov021_021909bc(ListMenuUI *window);
BOOL func_ov021_0219077c(MsgBGSysWin *window);
void func_ov021_02190728(MsgBGSysWin *window);
// Prints a string in the window at a position
void func_ov021_02190758(MsgBGSysWin *window, u16 x, u16 y, StrBuf *strbuf);
BmpWin *func_ov021_021907c0(MsgBGSysWin *window);
void func_ov021_02190788(MsgBGSysWin *window);
// The screen that picks the party or the Battle Box for a battle, whether it is done, and the choice
PartySelectList *func_ov021_021cc720(Field *field, PokeParty *party, PokeParty *battleBoxParty, u8 partyOk,
                                     int battleBoxOk, HeapID heapId);
BOOL func_ov021_021cc810(PartySelectList *psl);
u32 func_ov021_021cc7b4(PartySelectList *psl, u32 *out);
ListMenuUI *ListMenuUI_Create(FieldMsgBG *msgBGSys, const ListMenuRequest *request, ListMenuOption *options,
                              BmpMenuListCursorCallback callback, void *work, u16 a5, u16 cursor, u32 flags);
// The info message window: create, close, reopen, print, whether printing has ended, and skip to the end
MsgBGBalloonWin *func_ov021_02191558(FieldMsgBG *msgBGSys, MsgData *a1, u32 a2, u16 a3, u16 a4, u16 a5, u16 a6);
void func_ov021_021915ec(MsgBGBalloonWin *window);
void func_ov021_021916a8(MsgBGBalloonWin *window, u8 x, u8 y, StrBuf *strbuf);
BOOL func_ov021_0219049c(FieldMsgBG *msgBGSys);
void func_ov021_021915b4(MsgBGBalloonWin *window);
void func_ov021_021904c8(MsgData *msgData);
BOOL func_ov021_021916e0(MsgBGBalloonWin *window);
// The system message window of the field's message BG: create, close, print, whether printing has ended, skip
// to the end, and its bitmap window
MsgBGTalkWin *func_ov021_02190fa0(FieldMsgBG *msgBGSys, MsgData *msgData, u32 a2);
// Prints a message of the window's message data
void func_ov021_0219103c(MsgBGTalkWin *window, u16 x, u8 y, u32 messageId);
BOOL func_ov021_021910c0(MsgBGTalkWin *window);
void func_ov021_02191008(MsgBGTalkWin *window);
// The list window of the field's message BG: create, free, clear and print a line
MsgBGSysWin *func_ov021_021907c4(FieldMsgBG *msgBGSys, MsgData *msgData, u16 x, u16 y, u16 width, u16 height);
void func_ov021_02190854(MsgBGSysWin *window);
void func_ov021_0219082c(MsgBGSysWin *window);
void func_ov021_02190844(MsgBGSysWin *window, u16 x, u16 y, StrBuf *strbuf);
// A talk window on the field's message BG, printing messages of a message data or strings: create, free, print,
// whether printing has ended, clear, and the window
MsgData *func_ov021_021904b0(FieldMsgBG *msgBGSys, u32 fileId);
MsgBGTalkWin *func_ov021_02190f64(FieldMsgBG *msgBGSys, MsgData *msgData);
void func_ov021_02190e74(MsgBGTalkWin *window, u8 a1, u8 a2, u32 messageId);
BOOL func_ov021_02190ef0(MsgBGTalkWin *window);
void func_ov021_02190f7c(MsgBGTalkWin *window);
BmpWin *func_ov021_02190f9c(MsgBGTalkWin *window);
void func_ov021_02190eb8(MsgBGTalkWin *window, u8 a1, u8 a2, StrBuf *strbuf);
void func_ov021_02190e40(MsgBGTalkWin *window);
// A window that describes the list menu's options: clear and print
void func_ov021_02191164(MsgBGTalkWin *window);
void func_ov021_02191184(MsgBGTalkWin *window, u32 x, u8 y, StrBuf *strbuf);
void ListMenuRequest_Set(ListMenuRequest *request, u16 count, u16 x, u16 y, u16 width, u16 height);
// Releases the BG of the field's message BG and returns TRUE, or returns FALSE if it has none
BOOL func_ov021_02190378(FieldMsgBG *msgBGSys);
void func_ov021_021904d0(FieldMsgBG *msgBGSys);
// The state of a message window of the field's message BG. fld_faceup.c moves the mouth from state 0 to state 2,
// which looks like printing and finished
u32 func_ov021_021917c0(MsgBGBalloonWin *window);
void func_ov021_021914cc(ActorMsgWin *msgWin);
void func_ov021_02191084(MsgBGTalkWin *window, u16 x, u8 y, StrBuf *strbuf);
BmpWin *func_ov021_021911b4(MsgBGTalkWin *window);
// Frees the balloon at once, and prints another message in it
void func_ov021_0219131c(ActorMsgWin *msgWin);
void func_ov021_0219196c(FieldMsgBG *msgBGSys, u16 index);
BOOL func_ov021_021919ac(FieldMsgBG *msgBGSys);
void func_ov021_02191978(FieldMsgBG *msgBGSys);
BOOL func_ov021_02191be8(MsgBGScrollWin *window, StrBuf *strbuf);
void func_ov021_02191b14(MsgBGScrollWin *window);
void func_ov021_02192628(MsgBGSignWin *window);
BOOL func_ov021_021926d8(MsgBGSignWin *window);
void func_ov021_02191134(MsgBGTalkWin *window);
void func_ov021_02191794(MsgBGBalloonWin *window);
// Whether the messages of the field's message BG scroll on their own
void func_ov021_021904dc(FieldMsgBG *msgBGSys, u8 enable);
u32 func_ov021_021c0688(HeightExList *list);
BOOL func_ov021_021c068c(s32 x, s32 z, HeightExList *list, u8 index);
u32 func_ov021_021c06b4(u8 index, HeightExList *list);
u32 func_ov021_021c06a8(u8 index, HeightExList *list);
u16 FieldPlayer_GetObjCodeByExState(u32 sex, u32 exState);
// Shows a message as a balloon of an index in the field's message BG, and removes it
// Shows message msgId of msgData as a balloon of an index in the field's message BG
void func_ov021_02191870(FieldMsgBG *msgBGSys, MsgData *msgData, u32 msgId, u16 index, u8 x, u8 y, u8 width, u8 a7);
const FieldmapCtrlVTable *GetZoneFieldmapCtrlVTable(u16 zoneId);
BOOL FieldmapProc_Init(GameProc *proc, u32 *seq, void *param, void *work);
BOOL FieldmapProc_End(GameProc *proc, u32 *state, void *param, void *work);
Field *Field_Create(GameSystem *gsys, HeapID heapId);
void FieldG3D_Init(Field *field);
void FieldG3D_Update(Field *field);
void FieldG3D_RenderPhase1(Field *field);
void FieldG3D_RenderPhase2(Field *field);
void FieldG3D_Free(Field *field);
void FldActSys_AsyncMatLoadTCBFunc(TCB *tcb, void *data);
void Field_LoadActorMatColorPreset(Field *field);
void Field_InitActorSystem(Field *field);
void Field_SuspendActorSystem(Field *field);
BOOL Field_CheckDoRealTimeLoad(Field *field);
BOOL Field_UpdateZoneStatePos(Field *field);
BOOL Field_ShouldSwapZone(Field *field);
void Field_SwapZoneByMatrix(Field *field);
void Field_HotswapSpawnActors(GameData *gameData, MMSys *actorSystem, EventData *eventData, u32 zoneId);
void Field_ResetController(Field *field);
void func_ov021_02190270(FieldMsgBG *msgBGSys);
void func_ov021_0219027c(FieldMsgBG *msgBGSys);
void func_ov021_02188df0(Field *field);
// pokebw2's Field_GetG3DObjSys
void *func_ov021_02188cd8(Field *field);
u32 func_ov010_0216f830(GameData *gameData);

void func_ov021_021aaaf0(EncountSystem *encount, u32 a1);

void func_ov021_021a36c8(FieldPlayer *player, u32 dir, RailPosition *pos);

// pokebw2's (src/ov036/field_task_manager.c): count slots of tasks, each with the task it waits on to end
struct FieldTaskManager {
    u8 count;
    struct FieldTask **tasks;
    struct FieldTask **parents; // the task each waits on: it starts once its parent's callback has returned TRUE
};

#endif // BWPORT_FIELD_FIELD_H
