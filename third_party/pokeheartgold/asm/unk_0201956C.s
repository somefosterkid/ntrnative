	.include "macros.inc"
	.include "unk_020192D0.inc"
	.include "global.inc"

	.public sub_02019BA0
    .text

    .rodata

	.global _020F6298
_020F6298:
	.word sub_02019BA0, BgCommitTilemapBufferToVram, ScheduleBgTilemapBufferTransfer
