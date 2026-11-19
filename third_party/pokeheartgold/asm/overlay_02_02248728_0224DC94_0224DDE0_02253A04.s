#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/std_script.h"
	.include "macros.inc"
	.include "overlay_02.inc"
	.include "global.inc"

	.public ov02_02248D98
	.public ov02_02248DE4
	.public ov02_02248DF0
	.public ov02_02248E10
	.public ov02_02248F88
	.public ov02_02249088
	.public ov02_022490BC
	.public ov02_022491A8
	.public ov02_022491CC
	.public ov02_02249290
	.public ov02_0224939C
	.public ov02_022493EC
	.public ov02_022495B8
	.public ov02_022495D0
	.public ov02_022495E8
	.public ov02_02249658
	.public ov02_02249690
	.public ov02_022496D0
	.public ov02_02249754
	.public ov02_02249774
	.public ov02_022497C0
	.public ov02_02249838
	.public ov02_02249858
	.public ov02_022498BC
	.public ov02_02249940
	.public ov02_02249954
	.public ov02_02249968
	.public ov02_0224997C
	.public ov02_022499EC
	.public ov02_02249A5C
	.public ov02_02249AC4
	.public ov02_02249AD8
	.public ov02_02249AF0
	.public ov02_02249B10
	.public ov02_02249B38
	.public ov02_02249B60
	.public ov02_02249B80
	.public ov02_02249BA8
	.public ov02_02249BD8
	.public ov02_02249C74
	.public ov02_02249CD8
	.public ov02_0224AA80
	.public ov02_0224AAC8
	.public ov02_0224AAD4
	.public ov02_0224AB54
	.public ov02_0224ABCC
	.public ov02_0224ABF8
	.public ov02_0224AC04
	.public ov02_0224AC24
	.public ov02_0224AC28
	.public ov02_0224ACE0
	.public ov02_0224ADEC
	.public ov02_0224AF70
	.public ov02_0224B0E0
	.public ov02_0224B158
	.public ov02_0224B294
	.public ov02_0224B350
	.public ov02_0224B3FC
	.public ov02_0224B494
	.public ov02_0224B4AC
	.public ov02_0224B5F0
	.public ov02_0224B638
	.public ov02_0224B664
	.public ov02_0224B68C
	.public ov02_0224B690
	.public ov02_0224B6D0
	.public ov02_0224B6E4
	.public ov02_0224B7CC
	.public ov02_0224B804
	.public ov02_0224B808
	.public ov02_0224B87C
	.public ov02_0224B938
	.public ov02_0224B964
	.public ov02_0224C05C
	.public ov02_0224C0B0
	.public ov02_0224C14C
	.public ov02_0224C1B8
	.public ov02_0224C234
	.public ov02_0224C2A8
	.public ov02_0224C2EC
	.public ov02_0224C338
	.public ov02_0224C4B4
	.public ov02_0224C4D8
	.public ov02_0224C680
	.public ov02_0224C698
	.public ov02_0224C6DC
	.public ov02_0224C71C
	.public ov02_0224C75C
	.public ov02_0224C7D4
	.public ov02_0224C840
	.public ov02_0224C87C
	.public ov02_0224C8D0
	.public ov02_0224C93C
	.public ov02_0224C9B8
	.public ov02_0224CA38
	.public ov02_0224D1E4
	.public ov02_0224D22C
	.public ov02_0224D278
	.public ov02_0224D288
	.public ov02_0224D2BC
	.public ov02_0224D2C8
	.public ov02_0224D2DC
	.public ov02_0224D2F0
	.public ov02_0224D2F8
	.public ov02_0224D310
	.public ov02_0224D358
	.public ov02_0224D3A4
	.public ov02_0224D3B4
	.public ov02_0224D3E8
	.public ov02_0224D3F4
	.public ov02_0224D408
	.public ov02_0224D41C
	.public ov02_0224D424
	.public ov02_0224D43C
	.public ov02_0224D468
	.public ov02_0224D488
	.public ov02_0224D580
	.public ov02_0224D5B4
	.public ov02_0224D648
	.public ov02_0224D658
	.public ov02_0224D670
	.public ov02_0224D880
	.public ov02_0224D914
	.public ov02_0224D950
	.public ov02_0224D98C
	.public ov02_0224D9C0
	.public ov02_0224DAA4
	.public ov02_0224DB8C
	.public ov02_0224DB9C
	.public ov02_0224DC58
	.public ov02_0224DC64
	.public ov02_0224DC78
	.public ov02_0224DC8C
	.public ov02_0224DC94
	.public ov02_0224DCB0
	.public ov02_0224DD38
	.public ov02_0224DD4C
	.public ov02_0224DD8C
	.public ov02_0224DDC8
	.public ov02_02253300
	.public ov02_02253330












    .rodata
	.balign 4, 0
	.global ov02_02253A04
ov02_02253A04: ; 0x02253A04
	.word ov02_0224D2F0
	.word ov02_0224D41C
	.word ov02_0224D41C
	.word ov02_0224D2F0
	.word ov02_0224DC8C
	.word ov02_0224DC8C

	.global ov02_02253A1C
ov02_02253A1C: ; 0x02253A1C
	.word ov02_0224D2C8
	.word ov02_0224D3F4
	.word ov02_0224D408
	.word ov02_0224D2DC
	.word ov02_0224DC64
	.word ov02_0224DC78

	.global ov02_02253A34
ov02_02253A34: ; 0x02253A34
	.word ov02_0224D2F8
	.word ov02_0224D424
	.word ov02_0224D424
	.word ov02_0224D2F8
	.word ov02_0224DC94
	.word ov02_0224DC94

	.global ov02_02253A4C
ov02_02253A4C: ; 0x02253A4C
	.byte 0xFF, 0x01, 0xFF, 0x01
	.byte 0x01, 0xFF, 0x00, 0x00
