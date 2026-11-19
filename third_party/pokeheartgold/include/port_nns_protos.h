#ifndef POKEHEARTGOLD_PORT_NNS_PROTOS_H
#define POKEHEARTGOLD_PORT_NNS_PROTOS_H

// NitroSystem functions that have no prototype in the SDK headers (mwcc takes the DS calls as implicit int functions)
// or that ntrnative's nnsys shim does not declare yet. Without a prototype a 64-bit host compiler takes the result as
// int and passes the arguments by their promoted types. The DS build never sees this block.
#if defined(SDK_PORT)
u32 NNS_SndHeapGetSize(NNSSndHeapHandle heap);
u32 NNS_SndHeapGetFreeSize(NNSSndHeapHandle heap);
BOOL NNS_SndReadDriverChannelInfo(int chNo, SNDChannelInfo *info);
void NNS_SndPlayerPauseByPlayerNo(int playerNo, BOOL flag);
int NNS_SndCaptureGetCaptureType(void);
BOOL NNS_SndPlayerReadVariable(NNSSndHandle *handle, int varNo, s16 *var);
void NNS_SndPlayerSetTrackAllocatableChannel(NNSSndHandle *handle, u32 trackBitMask, u32 chBitMask);

// declared by the DS SDK's nnsys/g3d headers, absent from the shim's
void NNS_G3dGlbSetViewPort(int x1, int y1, int x2, int y2);
void NNSi_G3dModifyMatFlag(NNSG3dResMdl *pMdl, BOOL isOn, u32 flag);
void NNSi_G3dModifyPolygonAttrMask(NNSG3dResMdl *pMdl, BOOL isOn, u32 mask);
#endif

#endif // POKEHEARTGOLD_PORT_NNS_PROTOS_H
