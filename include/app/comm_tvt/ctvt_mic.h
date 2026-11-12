// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_mic.h: the lines in use so
// far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_MIC_H
#define BWPORT_APP_COMM_TVT_CTVT_MIC_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The start of a recording that is left out, the click of the microphone starting
#define CTVT_MIC_SKIPPED_SIZE 0x800

// pokebw2's CtvtMic_Create
CtvtMic *func_ov172_021b9f9c(HeapID heapId);
// pokebw2's CtvtMic_Delete
void func_ov172_021ba01c(CtvtMic *work);
// pokebw2's CtvtMic_Update
void func_ov172_021ba050(CtvtMic *work);
// pokebw2's CtvtMic_StartRecording
BOOL func_ov172_021ba0bc(CtvtMic *work);
// pokebw2's CtvtMic_StopRecording
BOOL func_ov172_021ba120(CtvtMic *work);
// pokebw2's CtvtMic_GetRecordedSize
u32 func_ov172_021ba15c(CtvtMic *work);
// pokebw2's CtvtMic_Encode
u32 func_ov172_021ba174(CtvtMic *work, const s16 *src, u8 *dst, u32 size);
// pokebw2's CtvtMic_Decode
u32 func_ov172_021ba194(CtvtMic *work, const s8 *src, s16 *dst, u32 size);
// pokebw2's CtvtMic_Play
BOOL func_ov172_021ba1e8(CtvtMic *work, const void *data, u32 size, int volume, int speed);
// pokebw2's CtvtMic_StopPlaying
void func_ov172_021ba234(CtvtMic *work);
// pokebw2's CtvtMic_GetPlayFrames
u16 func_ov172_021ba24c(CtvtMic *work);
// pokebw2's CtvtMic_GetPlayLength
u16 func_ov172_021ba254(CtvtMic *work);
// pokebw2's CtvtMic_IsReady
BOOL func_ov172_021ba270(CtvtMic *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtMic_Draw
void func_ov172_021ba094(CtvtMic *work);
// pokebw2's CtvtMic_IsRecording
BOOL func_ov172_021ba158(CtvtMic *work);
// pokebw2's CtvtMic_GetBuffer
void *func_ov172_021ba170(CtvtMic *work);
// pokebw2's CtvtMic_Debug
void func_ov172_021ba1e4(CtvtMic *work);
// pokebw2's CtvtMic_IsPlaying
BOOL func_ov172_021ba244(CtvtMic *work);
// pokebw2's CtvtMic_GetPlaySize
u32 func_ov172_021ba248(CtvtMic *work);

#endif // BWPORT_APP_COMM_TVT_CTVT_MIC_H
