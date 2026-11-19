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
	.include "overlay_01_02203E40.inc"
	.include "global.inc"

	.public ov01_02203E94
	.public ov01_02203F3C
	.public ov01_02203F68
	.public ov01_02203F6C
	.public ov01_02203F98
	.public ov01_02203E74
	.text

	.rodata

	.global ov01_022095B4
ov01_022095B4: ; 0x022095B4
	.word 0x00000020
	.word ov01_02203F3C
	.word ov01_02203F68
	.word ov01_02203F6C
	.word ov01_02203F98

	.global ov01_022095C8
ov01_022095C8: ; 0x022095C8
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
