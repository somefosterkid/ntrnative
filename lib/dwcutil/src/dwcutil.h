// Declarations shared by libdwcutil's files in overlay 16: the DWC utility (the Wi-Fi Connection settings), decompiled
// as one file, src/ov016/ov016_021b93f0.c, until it was split at the objects of NitroDWC's libdwcutil.a as it ships
// prebuilt: the same code in the same order. A file's own declarations can move to a header of its own.
#ifndef BWPORT_DWCUTIL_DWCUTIL_H
#define BWPORT_DWCUTIL_DWCUTIL_H

#include "types.h"
#include "dwc/utility.h"
#include "mem.h"
#include "nitro/card.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "nitro/mb.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/tp.h"
#include "nitro/wm.h"
#include "nnsys/fnd.h"
#include "nnsys/g2d.h"
#include "nnsys/snd.h"
#include "nitrowifi/soc.h"
#include "string.h"
#include "dwc/dwc.h"
#include "dwc/ac.h"
#include "nitrowifi/wcm.h"
#include "dwc_wcm.h"

// dwc_moveid.c: the user info of the child (MB_CommGetChildUser's of aid 1)
const MBUserInfo *DWCi_GetChildInfo(void);

void DWCi_TSKlChangeFunc(void *param_1, u32 param_2);
void DWCi_CELLlGetPosition(void *param_1, s32 param_2, u32 *param_3, u32 *param_4);
void DWCi_FNTlDeleteBgLcd(s32 param_1);
u8 *DWCi_Char(s8 *param_1);
u32 *DWCi_ARClReadEx(s8 *param_1, u32 *param_2, s32 param_3);
void DWCi_ARClRelease(u32 param_1);
void DWCi_GetParam_(u32 *param_1, u32 *param_2);
void DWCi_STEPlBegin(s32 param_1);
void DWCi_STEPlChange(s32 param_1);
NNSG2dCharCanvas *DWCi_FNTlFormBgEx(s32 param_1, s32 param_2);
s32 DWCi_MSGlGet(s32 *param_1, u32 param_2);
u32 DWCi_CMNlFontFlag(void);
void DWCi_FNTlDrawRect(NNSG2dCharCanvas *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6, u32 param_7, void *param_8);
void DWCi_FNTlRenewBg(s32 param_1);
extern s32 *data_ov016_021de598;
u8 DWCi_Language(void);
void DWCi_FNTlDraw(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, void *param_6);
void *DWCi_HEAPlAllocEx(u32 param_1, s32 param_2);
u32 DWCi_Entry(void);
s32 DWCi_SETTIGNlGet(void);
void DWCi_HLlLoad(s32 param_1);
void DWCi_TSKlDelete(s32 param_1, u32 param_2);
void DWCi_CELLlDelete(s32 *param_1);
void DWCi_SNDlPlay(s32 param_1);
BOOL DWCi_TPlCheckRelease(u16 *param_1);
// The whole screen as a touch rectangle (the prebuilt's DWCi_R_SCR, foundation.c's); several files define their own
// struct tagRECT, so it stays incomplete here
struct tagRECT;
extern const struct tagRECT data_ov016_021daa7c;
void func_ov016_021d600c(s16 *param_1, const s16 *param_2, s16 *param_3);
s32 DWCi_CELLlGetObj(void *param_1, s32 param_2);
void func_ov016_021d52d4(s32 param_1, s32 param_2, void *param_3);
void DWCi_CELLlSetPriority(void *param_1, s32 param_2, s32 param_3);
void func_ov016_021d5118(s32 param_1, s32 param_2, u32 param_3, u32 param_4);
u16 *DWCi_MSGlGetEx(s32 *param_1, u32 param_2, s32 param_3, s32 param_4);
void DWCi_GXlSetWndPlane(s32 param_1, s32 param_2, s32 param_3, s32 param_4);
NNSG2dCharCanvas *DWCi_FNTlFormEx(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 *param_5, s32 param_6);
void func_ov016_021d5f28(void *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5);
NNSG2dCharCanvas *DWCi_CELLlForm(s32 param_1, s32 param_2, s32 param_3);
void DWCi_InputRect(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u16 *param_5);
void DWCi_GXlSetWndPosition(s32 lcd, s32 wnd, u16 *rect);
void DWCi_ChangeScene(u32 param_1);
void DWCi_IPTlRead(void);
void DWCi_TSKlAct(s32 param_1);
void func_ov016_021d6bc8(void);
void DWCi_SetLedWireless(void);
void DWCi_HEAPlFree_(u32 *param_1);
s32 *DWCi_TSKlForm(s32 param_1, s32 param_2, void *param_3, u8 param_4);
u32 func_ov016_021d55a4(s32 param_1, s32 param_2, s32 param_3, u16 param_4);
u8 DWCi_EFFlCheck(s32 param_1);
void DWCi_SceneMenu(void);
void DWCi_SceneSetSelectAp(void);
u32 DWCi_COMMENTlDispEx(u32 param_1, s32 param_2, s32 param_3);
void DWCi_STEPlChangeScr(s32 param_1);
void DWCi_FOCUSlDisp4(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
void DWCi_GXlVisible(s32 param_1, u32 param_2);
u32 func_ov016_021d5790(u16 param_1);
BOOL DWCi_IPTlCheckTrigger(u32 param_1);
void DWCi_WINlInitEx(u32 param_1, s32 param_2, u8 param_3, s32 param_4, s32 param_5);
BOOL DWCi_BTNlIsEnd(void);
void DWCi_GXlInvisible(s32 param_1, u32 param_2);
void DWCi_SceneOptMenu(void);
void DWCi_SceneOptErase1(void);
s32 DWCi_WINlGet(void);
void DWCi_WINlEnd(void);
BOOL DWCi_WINlIsInit(void);
void DWCi_SETTINGlEraseAll(void);
void PM_ForceToPowerOff(void);
void DWCi_SCREENlCopyPalette(s32 param_1, s32 param_2, s32 param_3);
void DWCi_ANIMElInitEx(s32 param_1);
void DWCi_ICONlInit(s32 param_1);
void DWCi_ICONlEnd(void);
void DWCi_ANIMElEnd(void);
void DWCi_SceneOptMovFailure(void);
void DWCi_SceneOptMovDisconnect(void);
void DWCi_SceneOptMovComplete(void);
void DWCi_SceneOptMovWait(void);
void DWCi_MOVlInit(u32 param_1);
void DWCi_SNDlStop(void);
void DWCi_AOSSlInit(void);
void DWCi_TSKlDeleteEx(s32 param_1, s32 *param_2);
void DWCi_SceneSetTestConfirm(void);
void DWCi_SBlDisable(void);
void DWCi_FOCUSlDelete(void);
void DWCi_SBlSet(s32 param_1);
void DWCi_SBlEnable(void);
void DWCi_SetListReturn(u8 param_1);
void DWCi_SceneSetList(void);
void DWCi_FNTlClear(NNSG2dCharCanvas *param_1, u32 param_2);
void DWCi_SCREENlCopy(void *param_1, s32 param_2, s32 param_3, s32 param_4);
void DWCi_SCREENlRenew(void);
BOOL DWC_BACKUPlCheckAddress(const u8 *param_1);
void DWCi_SCREENlSetPalette(u32 param_1);
void DWCi_SETTINGlSet(void);
void DWCi_RAKUlEnd(void);
void DWCi_SceneSetSelectWay(void);
void DWCi_SceneSetRakuButton1(void);
u32 DWCi_RAKUlGetState(void);
void DWCi_SceneSetRakuComplete(void);
void DWCi_SEARCHlInit(void);
BOOL DWCi_SEARCHlBegin(void);
s32 func_ov016_021cf114(s32 *param_1);
BOOL DWCi_SEARCHlStop(void);
void DWCi_SEARCHlEnd(void);
void DWCi_SETTINGlSetSetting(s32 param_1);
void DWCi_SETTINGlErasePage(u32 param_1);
void DWCi_SceneSetApList(void);
BOOL DWCi_STEPlGet(void);
BOOL DWCi_TPlCheckTrigger(u16 *param_1);
BOOL DWCi_IPTlCheckRepeat(u32 param_1);
void func_ov016_021d0580(void);
void DWCi_FOCUSlDisp(s32 param_1, u32 param_2, u32 param_3, u32 param_4);
u32 DWCi_SETTINGlGetMemMap(void);
void DWCi_SceneSetTestCompletet(void);
void DWCi_HEAPlFree2(void *param_1);
BOOL DWCi_STEPlIsEnd(void);
void DWCi_SceneSetUsbReady1(void);
void DWCi_USBAPlSetCallback(u32 param_1);
void DWCi_SNDlSetVolume(s32 param_1);
void DWCi_SNDlSetPitch(u32 param_1, s32 param_2);
BOOL DWCi_TPlCheckRepeat(u16 *param_1);
BOOL DWCi_TPlCheck(u16 *param_1);
void DWCi_MOV_WH_MemInit(s32 param_1);
BOOL DWCi_MOV_WH_Initialize(void);
u32 DWCi_MOV_WH_End(void);
u16 DWCi_MOV_MBP_GetPlayerNo(u8 *param_1);
s32 DWCi_MOV_MBP_GetChildInfo(u32 param_1);
void *DWCi_OBJlFormGx(s32 param_1);
void DWCi_OVRlFree(s32 *param_1);
void DWCi_CELLlSetEffect(void *param_1, s32 param_2, u32 param_3, s32 param_4);
void DWCi_CELLlSetPalette(void *param_1, s32 param_2, s32 param_3, s32 param_4);
u32 ATERMi_ApConfigEnd(void);
void DWCi_TSKlOperate(s32 param_1, u8 param_2);
void func_ov016_021d52b0(s32 param_1);
void DWCi_SETTIGNlEnd(void);
void DWCi_OBJlDelete(u32 *param_1);
void DWCi_SceneOptMovConfirm1(void);
u32 DWCi_MOVlAdmitChild(void);
void DWCi_SceneSetAossComplete(void);
s32 DWCi_IPTlCheckRelease(u32 param_1);
void DWCi_SETTINGlSetIpAuto(BOOL param_1);
void DWCi_SETTINGlSetDnsAuto(BOOL param_1);
void DWCi_SETTINGlConvAddress(const u8 *text, u8 *addr);
// The address fields of the setting being edited, from their text (four numbers of 3 digits) and into it
void DWCi_SETTINGlSetIp(const u8 *text);
void DWCi_SETTINGlSetNetMask(const u8 *text);
void DWCi_SETTINGlSetGateway(const u8 *text);
void DWCi_SETTINGlSetDnsP(const u8 *text);
void DWCi_SETTINGlSetDnsS(const u8 *text);
void DWCi_SETTINGlGetIp(u8 *buf);
void DWCi_SETTINGlGetNetMask(u8 *buf);
void DWCi_SETTINGlGetGateway(u8 *buf);
void DWCi_SETTINGlGetDnsP(u8 *buf);
void DWCi_SETTINGlGetDnsS(u8 *buf);
u8 DWCi_CELLlGetObjNum(s32 param_1);
void DWCi_MOV_WH_Reset(void);
u32 DWCi_QUElPopBack(u16 *param_1);
s32 *DWCi_CEINlSetExCell(s32 param_1, s32 param_2, s32 param_3);
void DWCi_FNTlDeleteBg(s32 param_1);
void DWCi_SBlEnd(void);
void DWCi_SCREENlEnd(void);
void DWCi_SceneSetSearch(void);
void DWCi_SETTINGlSetSsid(void *param_1);
void DWCi_SceneSetEdit32(void);
void DWCi_QUElPushBack(u16 *param_1, u32 param_2);
u32 DWCi_MOVStartDownloadandBootChild(void);
void GX_DispOff(void);
void DWCi_SetLedNormal(void);
void DWCi_IPTlEnd(void);
void DWCi_SNDlEnd(void);
void DWCi_OBJlEnd(void);
void DWCi_FNTlEnd(void);
void DWCi_EFFlEnd(void);
void func_ov016_021d4a20(void);
void DWCi_HEAPlEnd(void);
void DWCi_RepairVram(void);
void *DWCi_CEINlSetExObj(void *param_1, s32 param_2);
void DWCi_FNTlDelete(s32 param_1);
void func_ov016_021d7274(void);
void DWCi_HEAPlInit(void *param_1);
void DWCi_IPTlInit(void);
void func_ov016_021d551c(void);
void DWCi_OBJlInit(void);
void DWCi_CELLlInit(void);
u32 DWCi_QUElFormEx(s32 param_1, s32 param_2, s32 param_3);
void func_ov016_021d3b18(s32 param_1, u32 *param_2);
u32 DWCi_MOVStartAdmitChild(void);
void DWCi_SCREENlInit(void *param_1);
u8 DWCi_SBlGetState(void);
u8 DWCi_SBlGet(void);
void DWCi_SETTINGlGetSsid(void *param_1);
s32 DWCi_STRlNLen(s32 param_1, s32 param_2);
s32 func_ov016_021d722c(void *param_1, void *param_2, s32 param_3);
void DWCi_FNTlDrawStringEx(NNSG2dCharCanvas *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, u16 *param_6, s32 param_7);
u8 DWCi_SETTINGlGetSetType(s32 param_1);
void DWCi_SceneSetUsbReady(void);
BOOL func_ov016_021d6b64(u16 *param_1);
u32 DWCi_LSTlForm(void);
void DWCi_LSTlDelete(u32 param_1);
u32 DWCi_QUElForm(s32 param_1);
s32 DWCi_OBJlGetOam(s32 param_1, s32 param_2);
void DWCi_LSTlPushFront(s32 param_1, s32 *param_2);
void DWCi_LSTlPushBack(s32 param_1, s32 *param_2);

typedef struct Ov016Work {
    u8 unk_0[0xC4];
    u8 unk_c4[0x404];
    u8 unk_4c8[4];
} Ov016Work;

// The functions of these files that nothing above declares, as they are defined
u32 ATERMi_ApConfigGetResult(void *param_1);
void DWCi_BTNlEnd(void);
s32 DWCi_BTNlGet(void);
void func_ov016_021bcb28(u8 param_1);
void DWCi_BTNlSetForce(s8 param_1);
void DWCi_BTNlEnable(void);
void DWCi_BTNlDisable(void);
u32 DWCi_COMMENTlDelete(void);
void DWCi_CMNlLoadResource(s8 *param_1, u8 *param_2);
void DWCi_CMNlStep(s32 param_1);
void DWCi_CMNlButton(s32 param_1);
void DWCi_CMNlMsgDraw(u32 param_1);
void DWCi_CMNlMsgNameDraw(void *param_1, u32 param_2);
void DWCi_FOCUSlEnd(void);
void DWCi_HLlEnd(void);
void DWCi_HLlLoadSet(void);
void DWCi_KBlEnd(void);
u8 DWCi_KBlGet(void);
void DWCi_KBlSetErase(u8 param_1);
void DWCi_KBlSetInput(u8 param_1);
BOOL DWCi_KBlIsInit(void);
void DWCi_KB10lEnd(void);
u8 DWCi_KB10lGet(void);
void DWCi_KB10lSetErase(u8 param_1);
void DWCi_KB10lSetInput(u8 param_1);
void DWCi_KB10lSetShift(u8 param_1);
BOOL DWCi_KB10lIsInit(void);
void DWCi_AOSSlEnd(s32 param_1);
void DWCi_SetParam_(u32 param_1, u32 param_2);
void DWCi_SetParam2_(u32 param_1, u32 param_2);
void DWCi_GetParam2_(u32 *param_1, u32 *param_2);
BOOL DWCi_Flag(u32 param_1);
void DWCi_EndUtility(void);
u8 kDWCi_Language(void);
void DWCi_MSGlEnd(void);
void DWCi_MOVlEnd(void);
BOOL DWCi_MOVlIsEnd(void);
void DWCi_MOVlChangeCallback(u32 param_1);
s32 DWCi_MOVlGetChild(void);
void DWCi_RAKUlInit(void);
void DWCi_SceneEnd(void);
void DWCi_SceneOptErase(void);
void DWCi_SceneOptInfo(void);
void DWCi_SceneOptMovComm(void);
void DWCi_SceneOptMovConfirm(void);
void DWCi_SceneOptMovUser(void);
void DWCi_SceneSetError(void);
void DWCi_SetErrorCode(u32 param_1);
void DWCi_SceneSetRakuButton(void);
void DWCi_SceneSetTest(void);
void DWCi_SceneSetUsbComplete(void);
void DWCi_MOV_MBP_MemInit(s32 param_1);
void DWCi_MOV_MBP_Start(MBGameRegistry *param_1, s32 param_2);
u32 DWCi_MOV_MBP_IsBootableAll(void);
void DWCi_MOV_MBP_Cancel(void);
u16 DWCi_MOV_MBP_GetState(void);
void DWCi_MOV_WH_SetGgid(u32 param_1);
u32 DWCi_MOV_WH_GetSystemState(void);
u32 DWCi_MOV_WH_StartMeasureChannel(void);
u16 DWCi_MOV_WH_GetMeasureChannel(void);
u32 func_ov016_021d376c(s32 param_1, u16 param_2, u16 param_3, u32 param_4);
void DWCi_MOV_WH_SetJudgeAcceptFunc(u32 param_1);
void DWCi_QUElDelete(u32 param_1);
void DWCi_LSTlErase(s32 *param_1);
void DWCi_LSTlInsert(s32 *param_1, s32 *param_2);

void DWCi_SETTIGNlInit(void);
void DWCi_MSGlInit(void);
void DWCi_FOCUSlInit(void);
void DWCi_HLlInit(void);
void DWCi_COMMENTlInit(void);
void *DWCi_MSGlRead(u8 *file);
void DWCi_CEINlRead(s32 lcd, s8 *file);

#endif // BWPORT_DWCUTIL_DWCUTIL_H
