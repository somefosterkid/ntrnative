// port/include_gen4/nnsys_gen4.h: the Gen 4 games' NitroSystem (071126: Platinum; HeartGold/SoulSilver's is ~92%
// byte-identical) on lib/nnsys (Black's NitroSystem, a newer one, and only what Black calls). The old names and
// spellings the Gen 4 C uses, as aliases of lib/nnsys's; what lib/nnsys lacks outright (functions Black never calls)
// is listed in docs/platinum.md ("NitroSystem") and goes into lib/nnsys itself, not here. Shared by every Gen 4 module
// (port/include_plat/port_plat.h includes it; an HGSS prefix would too).
#ifndef BWPORT_NNSYS_GEN4_H
#define BWPORT_NNSYS_GEN4_H

#include <nnsys.h>

// fnd
#ifndef NNS_FND_HEAP_INVALID_HANDLE
#define NNS_FND_HEAP_INVALID_HANDLE NULL
#endif
#define NNS_FndCreateExpHeap(startAddress, size) NNS_FndCreateExpHeapEx(startAddress, size, 0)

// g2d: 071126 names
typedef NNSG2dAnimBankData NNSG2dMultiCellAnimBankData;
#define NNS_G2D_VRAM_TYPE_2DBOTH ((NNS_G2D_VRAM_TYPE)3)
#define NNS_G2D_VRAM_ADDR_NOT_INITIALIZED 0xFFFFFFFF
#ifndef NNS_G2D_VRAM_ADDR_NONE
#define NNS_G2D_VRAM_ADDR_NONE NNS_G2D_VRAM_ADDR_NOT_INITIALIZED
#endif
#ifndef NNS_G2D_1D_MAPPING_CHAR_SIZE
#define NNS_G2D_1D_MAPPING_CHAR_SIZE ((u16)0xFFFF)
#endif
// the shift of the VRAM-transfer flag in NNSG2dCharacterData::characterFmt (071126's g2d_Load.h; Platinum's char_transfer.c
// has the same test as CHAR_TRANSFER_SHIFT 8)
#define NNS_G2D_VRAMTRANSFERDATA_FLAG_SHIFT 8
// 071126 pops one matrix with no count; lib/nnsys (Black's NitroSystem) takes a count it ignores
#define NNS_G2dPopMtx() NNS_G2dPopMtx(1)

// g3d: the kernel's key types (071126 has its own names for gfd's)
typedef NNSGfdTexKey NNSG3dTexKey;
typedef NNSGfdPlttKey NNSG3dPlttKey;
#ifndef NNS_G3D_NULL_ASSERT
#define NNS_G3D_NULL_ASSERT(x) ((void)0)
#endif

// gfd
#ifndef NNS_GFD_ALLOC_FROM_LOW
#define NNS_GFD_ALLOC_FROM_HIGH 0
#define NNS_GFD_ALLOC_FROM_LOW 1
#endif

// snd
#ifndef NNS_SND_WAVEOUT_INVALID_HANDLE
#define NNS_SND_WAVEOUT_INVALID_HANDLE NULL
#endif
// NitroSystem's that the Gen 4 games call and Black doesn't: lib/nnsys defines them outside the DS build (snd_heap.c,
// player.c, nns_snd_main.c: #ifndef __MWERKS__), not in nnsys/snd.h, so HeartGold's sound.c and the sound setup declared
// them implicitly (an int return)
u32 NNS_SndHeapGetSize(NNSSndHeapHandle heap);
u32 NNS_SndHeapGetFreeSize(NNSSndHeapHandle heap);
void NNS_SndPlayerSetTrackAllocatableChannel(NNSSndHandle *handle, u32 trackBitMask, u32 chBitFlag);
BOOL NNS_SndReadDriverChannelInfo(int chNo, SNDChannelInfo *chInfo);

#endif // BWPORT_NNSYS_GEN4_H
