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
	.include "overlay_01_021FF464.inc"
	.include "global.inc"

	.public ov01_021FF5B8
	.public ov01_021FF5C4
	.public ov01_021FF658
	.public ov01_021FF4D4
	.public ov01_021FF54C
	.text

	.rodata

	.global ov01_022091C0
ov01_022091C0: ; 0x022091C0
	.word 0x00000028
	.word ov01_021FF54C
	.word ov01_021FF5B8
	.word ov01_021FF5C4
	.word ov01_021FF658

	.global ov01_022091D4
ov01_022091D4: ; 0x022091D4
	.byte 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
