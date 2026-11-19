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
	.include "overlay_01_021FC05C.inc"
	.include "global.inc"

	.public ov01_021FC14C
	.public ov01_021FC1A4
	.public ov01_021FC1B4
	.public ov01_021FC0E0
	.public ov01_021FC10C
	.public ov01_021FC0A8
	.public ov01_021FC0DC
	.public ov01_021FC05C
	.text

	.rodata

	.global ov01_02208BC0
ov01_02208BC0: ; 0x02208BC0
	.word 1, 5, 9
