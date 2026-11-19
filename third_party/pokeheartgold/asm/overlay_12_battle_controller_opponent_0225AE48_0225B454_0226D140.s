#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"

	.public ov12_0225B494
	.public ov12_0225B7B8
	.public ov12_0225B960
	.public ov12_0225BE38
	.public ov12_0225C18C
	.public ov12_0225C6C8
	.public ov12_0225C9BC
	.public ov12_0225CC58
	.public ov12_0225CDB8
	.public ov12_0225CE28
	.public ov12_0225D138
	.public ov12_0225D644
	.public ov12_0225D890
	.public ov12_0225D990
	.public ov12_0225DA18
	.public ov12_0225DA8C
	.public ov12_0225FD14
	.public ov12_0225FF80
	.public ov12_0225FFDC
	.public ov12_02260030
	.public ov12_022600F0
	.public ov12_0226037C
	.public ov12_02260418
	.public ov12_022605D0
	.public ov12_02260668
	.public ov12_02261284
	.public ov12_022612A4
	.public ov12_02261390
	.public ov12_02261464
	.public ov12_02261B2C
	.public ov12_02261D30
	.public ov12_02261EF0


    .data
	.balign 4, 0
	.global ov12_0226D140
ov12_0226D140:
	.global hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D140
	.set hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D140, ov12_0226D140 ; 0x0226D140
	.byte 0

	.global ov12_0226D141
ov12_0226D141:
	.global hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D141
	.set hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D141, ov12_0226D141 ; 0x0226D141
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov12_0226D15A
ov12_0226D15A:
	.global hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D15A
	.set hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D15A, ov12_0226D15A ; 0x0226D15A
	.byte 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov12_0226D18C
ov12_0226D18C:
	.global hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D18C
	.set hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D18C, ov12_0226D18C ; 0x0226D18C
	.byte 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00

	.global ov12_0226D1E8
ov12_0226D1E8: ; 0x0226D1E8
	.byte 0x00, 0x00

	.global ov12_0226D1EA
ov12_0226D1EA:
	.global hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D1EA
	.set hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D1EA, ov12_0226D1EA ; 0x0226D1EA
	.byte 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0x00, 0, 0
	.byte 0, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov12_0226D350
ov12_0226D350:
	.global hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D350
	.set hgasset_asm_overlay_12_battle_controller_opponent_0225AE48_0225B454_0226D140_s_ov12_0226D350, ov12_0226D350 ; 0x0226D350
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0
