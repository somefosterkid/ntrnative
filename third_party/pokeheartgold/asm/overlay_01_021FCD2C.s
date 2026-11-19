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
	.include "overlay_01_021FCD2C.inc"
	.include "global.inc"

	.public ov01_021FCDBC
	.public ov01_021FCDC4
	.public ov01_021FCDFC
	.text

	.rodata

	.global ov01_02208E0C
ov01_02208E0C: ; 0x02208E0C
	.word ov01_021FCDBC
	.word ov01_021FCDC4
	.word ov01_021FCDFC
	.word 0x00000000
