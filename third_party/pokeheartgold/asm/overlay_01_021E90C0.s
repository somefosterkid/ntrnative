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
	.include "overlay_01_021E90C0.inc"
	.include "global.inc"

	.data

	.data

	.global ov01_02206408
ov01_02206408:
	.global hgasset_asm_overlay_01_021E90C0_s_ov01_02206408
	.set hgasset_asm_overlay_01_021E90C0_s_ov01_02206408, ov01_02206408 ; 0x02206408
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov01_02206418
ov01_02206418:
	.global hgasset_asm_overlay_01_021E90C0_s_ov01_02206418
	.set hgasset_asm_overlay_01_021E90C0_s_ov01_02206418, ov01_02206418 ; 0x02206418
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov01_02206428
ov01_02206428:
	.global hgasset_asm_overlay_01_021E90C0_s_ov01_02206428
	.set hgasset_asm_overlay_01_021E90C0_s_ov01_02206428, ov01_02206428 ; 0x02206428
	.byte 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00

	.global ov01_02206450
ov01_02206450:
	.word ov01_02209820
	.word ov01_02209850
	.word ov01_02209838
	.word ov01_02209868
	.word ov01_02209880

	.data

	.balign 4, 0
ov01_02209820:
	.asciz "data/area00light.txt"
	.balign 4, 0
ov01_02209838:
	.asciz "data/area02light.txt"
	.balign 4, 0
ov01_02209850:
	.asciz "data/area01light.txt"
	.balign 4, 0
ov01_02209868:
	.asciz "data/dun20_01light.txt"
	.balign 4, 0
ov01_02209880:
	.asciz "data/dun20_02light.txt"
