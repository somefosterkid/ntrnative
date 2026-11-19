#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/std_script.h"
	.include "macros.inc"
	.include "overlay_02.inc"
	.include "global.inc"

	.public ov02_022462DC
	.public ov02_02245B9C
	.public ov02_02245BC8
	.data

	.data

ov02_0225321C:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_0225321C
	.set hgasset_asm_overlay_02_02245B80_s_ov02_0225321C, ov02_0225321C
	.byte 0, 0x00

ov02_0225321E:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_0225321E
	.set hgasset_asm_overlay_02_02245B80_s_ov02_0225321E, ov02_0225321E
	.byte 0, 0x00
	.byte 0, 0x00

ov02_02253222:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_02253222
	.set hgasset_asm_overlay_02_02245B80_s_ov02_02253222, ov02_02253222
	.byte 0, 0x00, 0, 0x00, 0, 0x00

ov02_02253228:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_02253228
	.set hgasset_asm_overlay_02_02245B80_s_ov02_02253228, ov02_02253228
	.byte 0, 0x00, 0, 0x00, 0, 0x00

ov02_0225322E:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_0225322E
	.set hgasset_asm_overlay_02_02245B80_s_ov02_0225322E, ov02_0225322E
	.byte 0, 0x00, 0, 0x00, 0, 0x00

ov02_02253234:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_02253234
	.set hgasset_asm_overlay_02_02245B80_s_ov02_02253234, ov02_02253234
	.byte 0, 0x00, 0, 0x00, 0, 0x00

ov02_0225323A:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_0225323A
	.set hgasset_asm_overlay_02_02245B80_s_ov02_0225323A, ov02_0225323A
	.byte 0, 0x00, 0, 0x00, 0, 0x00

ov02_02253240:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_02253240
	.set hgasset_asm_overlay_02_02245B80_s_ov02_02253240, ov02_02253240
	.byte 0, 0x00, 0, 0x00, 0, 0x00

ov02_02253246:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_02253246
	.set hgasset_asm_overlay_02_02245B80_s_ov02_02253246, ov02_02253246
	.byte 0, 0x00, 0, 0x00, 0, 0x00

	.global ov02_0225324C
ov02_0225324C:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_0225324C
	.set hgasset_asm_overlay_02_02245B80_s_ov02_0225324C, ov02_0225324C ; 0x0225324C
	.byte 0, 0, 0, 0
	.byte 0, 0x00, 0, 0x00

	.global ov02_02253254
ov02_02253254:
	.global hgasset_asm_overlay_02_02245B80_s_ov02_02253254
	.set hgasset_asm_overlay_02_02245B80_s_ov02_02253254, ov02_02253254 ; 0x02253254
	.byte 0, 0x00, 0, 0
	.word ov02_02253240
	.byte 0, 0x00, 0, 0
	.word ov02_02253222

	.global ov02_02253264
ov02_02253264: ; 0x02253264
	.byte 0xAE, 0x00, 0x0A, 0x01
	.word ov02_0225321C
	.short SEQ_SE_GS_DENKI_TRAP
	.balign 4
	.byte 0xAE, 0x00, 0x03, 0x02
	.word ov02_0225321E
	.short SEQ_SE_GS_DOKU_TRAP
	.balign 4
	.byte 0xAE, 0x00, 0x06, 0x03
	.word ov02_02253246
	.short SEQ_SE_GS_IWA_TRAP
	.balign 4

	.data

	.global ov02_02253D80
ov02_02253D80: ; 0x02253D80
	.word ov02_0225322E

	.global ov02_02253D84
ov02_02253D84: ; 0x02253D84
	.word ov02_02253234
	.word ov02_02253228
	.word ov02_0225323A
