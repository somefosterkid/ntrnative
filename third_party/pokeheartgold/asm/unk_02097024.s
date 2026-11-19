#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "macros.inc"
	.include "unk_02097024.inc"
	.include "global.inc"

	.public sub_020970E4
	.public sub_02097108
	.public sub_02097148
	.public sub_020971AC
	.public sub_020971D4
	.public sub_020971EC
	.text

	.rodata

	.global _02108EA0
_02108EA0:
	.word ov85_021E88AC, ov85_021E8A08, ov85_021E8B08, SDK_OVERLAY_OVY_85_ID
	.global _02108EB0
_02108EB0:
	.word ov85_021E5900, ov85_021E5AAC, ov85_021E5A34, SDK_OVERLAY_OVY_85_ID
	.global _02108EC0
_02108EC0:
	.word sub_020970E4
	.word sub_02097108
	.word sub_02097148
	.word sub_020971AC
	.word sub_020971D4
	.word sub_020971EC
