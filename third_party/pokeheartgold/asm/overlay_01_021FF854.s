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
	.include "overlay_01_021FF854.inc"
	.include "global.inc"

	.public ov01_021FFAB0
	.public ov01_021FFABC
	.public ov01_021FFBD4
	.public ov01_021FF8C8
	.public ov01_021FF9F4
	.text

	.rodata

	.global ov01_02209218
ov01_02209218: ; 0x02209218
	.word 0x00000040
	.word ov01_021FF9F4
	.word ov01_021FFAB0
	.word ov01_021FFABC
	.word ov01_021FFBD4

	.global ov01_0220922C
ov01_0220922C: ; 0x0220922C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00

ov01_02209244:
	.word 0x00000040
	.word ov01_021FFCA8
	.word ov01_021FFD64
	.word ov01_021FFD70
	.word ov01_021FFE98
