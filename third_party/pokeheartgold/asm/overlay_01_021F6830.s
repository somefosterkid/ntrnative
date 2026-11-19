#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "macros.inc"
	.include "overlay_01_021F6830.inc"
	.include "global.inc"

	.public ov01_021F6B88
	.public ov01_021F6BA0
	.public ov01_021F6BAC
	.data

	.data

	.global ov01_02206C08
ov01_02206C08:
	.global hgasset_asm_overlay_01_021F6830_s_ov01_02206C08
	.set hgasset_asm_overlay_01_021F6830_s_ov01_02206C08, ov01_02206C08 ; 0x02206C08
	.short 0, 0, 0, 0
	.short 0, 0, 0, 0, 0, 0, 0, 0
	.short 0, 0, 0, 0, 0, 0, 0, 0
	.short 0, 0, 0, 0, 0, 0, 0, 0
	.short 0, 0, 0, 0, 0, 0, 0, 0
	.short 0, 0, 0, 0, 0, 0, 0, 0x0000

	.global ov01_02206C60
ov01_02206C60: ; 0x02206C60
	.word ov27_02259F80, ov27_0225A19C, ov27_0225A2C8, 0xFFFFFFFF
	.word ov30_0225D520, ov30_0225D64C, ov30_0225D6FC, SDK_OVERLAY_touch_save_app_ID
	.word ov31_0225D520, ov31_0225D710, ov31_0225D758, SDK_OVERLAY_OVY_31_ID
	.word ov27_0225C250, ov27_0225C398, ov27_0225C418, 0xFFFFFFFF
	.word ov28_0225D520, ov28_0225D5EC, ov28_0225D624, SDK_OVERLAY_OVY_28_ID
	.word ov29_0225D520, ov29_0225D5EC, ov29_0225D61C, SDK_OVERLAY_OVY_29_ID
	.word ov32_0225D520, ov32_0225D5CC, ov32_0225D608, SDK_OVERLAY_OVY_32_ID
	.word ov33_0225D520, ov33_0225D5A8, ov33_0225D5CC, SDK_OVERLAY_OVY_33_ID
	.word ov01_021F6B88, ov01_021F6BA0, ov01_021F6BAC, SDK_OVERLAY_OVY_34_ID
