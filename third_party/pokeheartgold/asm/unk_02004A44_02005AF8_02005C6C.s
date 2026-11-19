#include "constants/sndseq.h"
	.include "macros.inc"
	.include "unk_02004A44.inc"
	.include "global.inc"

	.public GF_GetCurrentPlayingBGM
	.public GF_GetPlayerNoBySeq
	.public GF_NNS_SndPlayerGetSeqNo
	.public GF_SetVolumeBySeqNo
	.public GF_SndHandleMoveVolume
	.public GF_SndHandleSetTrackPitch
	.public GF_SndSetAllocatableChannelForBGMPlayer
	.public _020F5710
	.public _020F5730
	.public _021D05E8
	.public sub_02004AAC
	.public sub_020053A8
	.public sub_020058F4
	.public sub_02005910
	.text
