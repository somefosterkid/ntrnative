	.include "macros.inc"
	.include "overlay_29.inc"
	.include "global.inc"

	.public ov29_0225D620
	.public ov29_0225D648
	.public ov29_0225D6B4
	.public ov29_0225D6C8
	.public ov29_0225D714
	.public ov29_0225D7D4
	.public ov29_0225D7F0
	.public ov29_0225D828
	.public ov29_0225DBF0
	.public ov29_0225DC34
	.public ov29_0225DC44
	.public ov29_0225DC48
	.public ov29_0225DCD0
	.data

	.data

	.global ov29_0225E114
ov29_0225E114:
	.byte 0x80

ov29_0225E115:
	.byte 0x08, 0x27, 0x70, 0x8F, 0xFF, 0x00, 0x00, 0x00

ov29_0225E11D:
	.byte 0x08, 0x27, 0x60
	.byte 0x7F, 0x08, 0x27, 0x80, 0x9F, 0xFF, 0x00, 0x00, 0x00

ov29_0225E129:
	.byte 0x08, 0x27, 0x50, 0x6F, 0x08, 0x27, 0x70
	.byte 0x8F, 0x08, 0x27, 0x90, 0xAF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov29_0225E13C
ov29_0225E13C: ; 0x0225E13C
	.word ov29_0225DC44
	.word ov29_0225DC44
	.word ov29_0225DC48
	.word ov29_0225DC48

ov29_0225E14C:
	.global hgasset_asm_overlay_29_s_ov29_0225E14C
	.set hgasset_asm_overlay_29_s_ov29_0225E14C, ov29_0225E14C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov29_0225E160:
	.global hgasset_asm_overlay_29_s_ov29_0225E160
	.set hgasset_asm_overlay_29_s_ov29_0225E160, ov29_0225E160
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov29_0225E178
ov29_0225E178:
	.global hgasset_asm_overlay_29_s_ov29_0225E178
	.set hgasset_asm_overlay_29_s_ov29_0225E178, ov29_0225E178 ; 0x0225E178
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0x00

ov29_0225E190:
	.global hgasset_asm_overlay_29_s_ov29_0225E190
	.set hgasset_asm_overlay_29_s_ov29_0225E190, ov29_0225E190
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov29_0225E1AC
ov29_0225E1AC: ; 0x0225E1AC
	.word 0
	.word ov29_0225E115
	.word ov29_0225E11D
	.word ov29_0225E129
	.word ov29_0225E14C
	.word ov29_0225E160
	.word ov29_0225E190

	.global ov29_0225E1C8
ov29_0225E1C8:
	.global hgasset_asm_overlay_29_s_ov29_0225E1C8
	.set hgasset_asm_overlay_29_s_ov29_0225E1C8, ov29_0225E1C8 ; 0x0225E1C8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov29_0225E1E4
ov29_0225E1E4: ; 0x0225E1E4
	.word 0
	.word ov29_0225E114
	.word ov29_0225E2E8
	.word ov29_0225E2EC
	.word ov29_0225E2F0
	.word ov29_0225E2F4
	.word ov29_0225E2FC

	.global ov29_0225E200
ov29_0225E200:
	.global hgasset_asm_overlay_29_s_ov29_0225E200
	.set hgasset_asm_overlay_29_s_ov29_0225E200, ov29_0225E200 ; 0x0225E200
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov29_0225E21C
ov29_0225E21C:
	.global hgasset_asm_overlay_29_s_ov29_0225E21C
	.set hgasset_asm_overlay_29_s_ov29_0225E21C, ov29_0225E21C ; 0x0225E21C
	.byte 0, 0, 0x00, 0

	.global ov29_0225E220
ov29_0225E220:
	.global hgasset_asm_overlay_29_s_ov29_0225E220
	.set hgasset_asm_overlay_29_s_ov29_0225E220, ov29_0225E220 ; 0x0225E220
	.byte 0, 0, 0x00, 0x00

	.global ov29_0225E224
ov29_0225E224:
	.global hgasset_asm_overlay_29_s_ov29_0225E224
	.set hgasset_asm_overlay_29_s_ov29_0225E224, ov29_0225E224 ; 0x0225E224
	.byte 0, 0, 0, 0

	.global ov29_0225E228
ov29_0225E228:
	.global hgasset_asm_overlay_29_s_ov29_0225E228
	.set hgasset_asm_overlay_29_s_ov29_0225E228, ov29_0225E228 ; 0x0225E228
	.byte 0, 0, 0, 0

	.global ov29_0225E22C
ov29_0225E22C:
	.global hgasset_asm_overlay_29_s_ov29_0225E22C
	.set hgasset_asm_overlay_29_s_ov29_0225E22C, ov29_0225E22C ; 0x0225E22C
	.byte 0, 0, 0, 0

	.global ov29_0225E230
ov29_0225E230:
	.global hgasset_asm_overlay_29_s_ov29_0225E230
	.set hgasset_asm_overlay_29_s_ov29_0225E230, ov29_0225E230 ; 0x0225E230
	.byte 0, 0, 0, 0

	.global ov29_0225E234
ov29_0225E234:
	.global hgasset_asm_overlay_29_s_ov29_0225E234
	.set hgasset_asm_overlay_29_s_ov29_0225E234, ov29_0225E234 ; 0x0225E234
	.byte 0, 0, 0, 0

	.global ov29_0225E238
ov29_0225E238:
	.global hgasset_asm_overlay_29_s_ov29_0225E238
	.set hgasset_asm_overlay_29_s_ov29_0225E238, ov29_0225E238 ; 0x0225E238
	.byte 0, 0, 0, 0

	.global ov29_0225E23C
ov29_0225E23C:
	.global hgasset_asm_overlay_29_s_ov29_0225E23C
	.set hgasset_asm_overlay_29_s_ov29_0225E23C, ov29_0225E23C ; 0x0225E23C
	.byte 0, 0, 0, 0

	.global ov29_0225E240
ov29_0225E240:
	.global hgasset_asm_overlay_29_s_ov29_0225E240
	.set hgasset_asm_overlay_29_s_ov29_0225E240, ov29_0225E240 ; 0x0225E240
	.byte 0, 0, 0, 0

	.global ov29_0225E244
ov29_0225E244:
	.global hgasset_asm_overlay_29_s_ov29_0225E244
	.set hgasset_asm_overlay_29_s_ov29_0225E244, ov29_0225E244 ; 0x0225E244
	.byte 0, 0, 0, 0

	.global ov29_0225E248
ov29_0225E248:
	.global hgasset_asm_overlay_29_s_ov29_0225E248
	.set hgasset_asm_overlay_29_s_ov29_0225E248, ov29_0225E248 ; 0x0225E248
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov29_0225E280
ov29_0225E280:
	.global hgasset_asm_overlay_29_s_ov29_0225E280
	.set hgasset_asm_overlay_29_s_ov29_0225E280, ov29_0225E280 ; 0x0225E280
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov29_0225E2E8:
	.global hgasset_asm_overlay_29_s_ov29_0225E2E8
	.set hgasset_asm_overlay_29_s_ov29_0225E2E8, ov29_0225E2E8
	.byte 0, 0, 0x00, 0x00

ov29_0225E2EC:
	.global hgasset_asm_overlay_29_s_ov29_0225E2EC
	.set hgasset_asm_overlay_29_s_ov29_0225E2EC, ov29_0225E2EC
	.byte 0, 0, 0, 0x00

ov29_0225E2F0:
	.global hgasset_asm_overlay_29_s_ov29_0225E2F0
	.set hgasset_asm_overlay_29_s_ov29_0225E2F0, ov29_0225E2F0
	.byte 0, 0, 0, 0

ov29_0225E2F4:
	.global hgasset_asm_overlay_29_s_ov29_0225E2F4
	.set hgasset_asm_overlay_29_s_ov29_0225E2F4, ov29_0225E2F4
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov29_0225E2FC:
	.global hgasset_asm_overlay_29_s_ov29_0225E2FC
	.set hgasset_asm_overlay_29_s_ov29_0225E2FC, ov29_0225E2FC
	.byte 0, 0, 0, 0, 0, 0, 0x00, 0x00
