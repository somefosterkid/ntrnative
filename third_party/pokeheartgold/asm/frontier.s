#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "macros.inc"
	.include "unk_020965A4.inc"
	.include "global.inc"

	.public Frontier_CreateMap
	.public Frontier_FreeMap
	.public Frontier_LoadOverlays
	.public Frontier_UnloadOverlays
	.public sub_02096780
	.public sub_02096884
	.public Frontier_Exit
	.public Frontier_Init
	.public Frontier_Main
	.text

	.rodata

	.public gOverlayTemplate_Frontier
gOverlayTemplate_Frontier:
	.word Frontier_Init, Frontier_Main, Frontier_Exit, 0xFFFFFFFF
