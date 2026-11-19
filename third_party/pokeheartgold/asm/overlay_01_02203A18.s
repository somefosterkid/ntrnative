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
	.include "overlay_01_02203A18.inc"
	.include "global.inc"

	.public ov01_02203B28
	.public ov01_02203BB4
	.public ov01_02203CA0
	.public ov01_02203CB8
	.public ov01_02203DC0
	.data

	.data

	.global ov01_02209518
ov01_02209518: ; 0x02209518
	.word 0x00000068
	.word ov01_02203BB4
	.word ov01_02203CA0
	.word ov01_02203CB8
	.word ov01_02203DC0

	.global ov01_0220952C
ov01_0220952C: ; 0x0220952C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00

	.global ov01_02209544
ov01_02209544:
	.global hgasset_asm_overlay_01_02203A18_s_ov01_02209544
	.set hgasset_asm_overlay_01_02203A18_s_ov01_02209544, ov01_02209544 ; 0x02209544
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
	.word 0, 0
