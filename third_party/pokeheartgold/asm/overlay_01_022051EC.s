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
	.include "overlay_01_022051EC.inc"
	.include "global.inc"

	.public ov01_02205218
	.public ov01_02205300
	.public ov01_02205350
	.public ov01_02205354
	.public ov01_02205364
	.text

	.rodata

	.global ov01_022096CC
ov01_022096CC: ; 0x022096CC
	.word 0x00000008
	.word ov01_02205300
	.word ov01_02205354
	.word ov01_02205350
	.word ov01_02205364
