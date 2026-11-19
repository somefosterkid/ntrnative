#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "macros.inc"
	.include "unk_02078834.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.public sub_02078B9C
	.public sub_02078BD8
	.public sub_02078C18
	.public sub_02078C60
	.public sub_02078C74
	.public sub_02078CB4
	.public sub_02078CC8
	.public sub_02078D10
	.text

	.rodata

	.global _0210125C
_0210125C:
	.byte 0x03, 0x04, 0x04, 0x04
	.global _02101260
_02101260:
	.word ov91_0225C540, ov91_0225C58C, ov91_0225C9EC, SDK_OVERLAY_OVY_91_ID
	.global _02101270
_02101270:
	.word ov92_0225CAB4, ov92_0225CDF4, ov92_0225D36C, SDK_OVERLAY_OVY_92_ID
	.global _02101280
_02101280:
	.word ov44_02232EA8, ov44_02232F64, ov44_022330A8, SDK_OVERLAY_OVY_44_ID
_02101290:
	.word ov44_0222A4B4, ov44_0222A60C, ov44_0222A758, SDK_OVERLAY_OVY_44_ID
	.global _021012A0
_021012A0:
	.word ov93_0225C540, ov93_0225C574, ov93_0225C6C0, SDK_OVERLAY_OVY_93_ID
