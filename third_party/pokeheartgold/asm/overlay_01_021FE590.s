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
	.include "overlay_01_021FE590.inc"
	.include "global.inc"

	.public ov01_021FE61C
	.public ov01_021FE6B4
	.public ov01_021FE6F4
	.public ov01_021FE70C
	.public ov01_021FE768
	.text

	.rodata

	.global ov01_02209084
ov01_02209084: ; 0x02209084
	.word 0x0000001C
	.word ov01_021FE6B4
	.word ov01_021FE6F4
	.word ov01_021FE70C
	.word ov01_021FE768
