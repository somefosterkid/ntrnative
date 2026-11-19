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
	.include "overlay_01_022006A8.inc"
	.include "global.inc"

	.public ov01_022007D0
	.public ov01_022007DC
	.public ov01_022007F8
	.public ov01_02200710
	.public ov01_02200780
	.text

	.rodata

	.global ov01_02209308
ov01_02209308: ; 0x02209308
	.word 0x00000024
	.word ov01_02200780
	.word ov01_022007D0
	.word ov01_022007DC
	.word ov01_022007F8

	.global ov01_0220931C
ov01_0220931C: ; 0x0220931C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00
