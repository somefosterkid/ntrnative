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
	.include "overlay_01_021EAF00.inc"
	.include "global.inc"

	.public ov01_021EAF94
	.public ov01_021EAF98
	.public ov01_021EAFB4
	.public ov01_021EAFD0
	.public ov01_021EAF00
	.public ov01_021EAF8C
	.public ov01_021EAF90
	.text

	.rodata

	.global ov01_022066DC
ov01_022066DC: ; 0x022066DC
	.word ov01_021F6894, 0x00000000, ov01_021F68B8, ov01_021F68C0
	.word ov01_021EAF8C, 0x00000000, ov01_021EAF90, ov01_021EAF94
	.word ov01_021F6894, 0x00000000, ov01_021F68B8, ov01_021F68C0
	.word ov01_021F6840, 0x00000000, ov01_021F6864, ov01_021F6874
	.word sub_0203DB6C, 0x00000000, sub_0203DB70, sub_0203DB74
	.word ov01_021EAF98, 0x00000000, ov01_021EAFB4, ov01_021EAFD0
