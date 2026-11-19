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
	.include "overlay_01_data_02208BFC.inc"
	.include "global.inc"

	.rodata

	.public defaultFieldEffectRenderers
defaultFieldEffectRenderers: ; 0x02208BFC
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0x00
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0

	.public ov01_02208C5C
ov01_02208C5C: ; 0x02208C5C
	.word 0x00000000, ov01_021FD41C, ov01_021FD440
	.word 0, ov01_021FDA14, ov01_021FDA30
	.word 0, ov01_021FE200, ov01_021FE220
	.word 0, ov01_02200858, ov01_02200874
	.word 0, ov01_021FE780, ov01_021FE79C
	.word 0, ov01_021FEA0C, ov01_021FEA20
	.word 0, ov01_021FEC38, ov01_021FEC54
	.word 0, ov01_021FED9C, ov01_021FEDB8
	.word 0, ov01_021FEEEC, ov01_021FEF08
	.word 0, ov01_022001E4, ov01_022001F8
	.word 0, ov01_021FF464, ov01_021FF480
	.word 0, ov01_021FF6B0, ov01_021FF6CC
	.word 0, ov01_021FF854, ov01_021FF870
	.word 0, ov01_021FFC0C, ov01_021FFC28
	.word 0, ov01_021FFECC, ov01_021FFEE8
	.word 0, ov01_02200040, ov01_0220005C
	.word 0, ov01_021FE590, ov01_021FE5A4
	.word 0, ov01_022031C0, ov01_022031E8
	.word 0, ov01_02203A18, ov01_02203A38
	.word 0, ov01_02203E40, ov01_02203E64
	.word 0, ov01_021FD1B8, ov01_021FD1CC
	.word 0, ov01_022006A8, ov01_022006C4
	.word 0, ov01_022051EC, ov01_02205208
	.word 0, 0x00000000, 0x00000000
