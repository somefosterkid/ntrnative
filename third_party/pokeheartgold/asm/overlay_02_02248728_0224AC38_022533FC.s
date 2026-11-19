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






    .data
	.balign 4, 0
	.global ov02_022533FC
ov02_022533FC: ; 0x022533FC
	.byte 0x00, 0x00, 0x08, 0x00
	.byte 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov02_02253408
ov02_02253408: ; 0x02253408
	.byte 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov02_02253414
ov02_02253414: ; 0x02253414
	.byte 0x00, 0x80, 0x12, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov02_02253420
ov02_02253420:
	.word ov02_0224AF70, ov02_0224B0E0, ov02_0224B158, ov02_0224B294

	.global ov02_02253430
ov02_02253430: ; 0x02253430
	.byte 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xA0, 0xFF, 0xFF, 0x00, 0x90, 0xFF, 0xFF, 0x00, 0x80, 0xFF, 0xFF

	.global ov02_02253440
ov02_02253440: ; 0x02253440
	.byte 0x08, 0x00, 0x00, 0x00
	.word ov02_0224B6D0
	.word sub_02068DD4
	.word ov02_0224B6E4
	.word sub_02068DD0

	.global ov02_02253454
ov02_02253454: ; 0x02253454
	.byte 0x74, 0x00, 0x00, 0x00
	.word ov02_02248D98
	.word ov02_02248DE4
	.word ov02_02248DF0
	.word sub_02068DD0

	.global ov02_02253468
ov02_02253468: ; 0x02253468
	.byte 0x24, 0x00, 0x00, 0x00
	.word ov02_0224AA80
	.word ov02_0224AAC8
	.word ov02_0224AAD4
	.word ov02_0224AB54

	.global ov02_0225347C
ov02_0225347C: ; 0x0225347C
	.byte 0x68, 0x00, 0x00, 0x00
	.word ov02_0224ABCC
	.word ov02_0224ABF8
	.word ov02_0224AC04
	.word ov02_0224AC24

	.global ov02_02253490
ov02_02253490: ; 0x02253490
	.byte 0x0C, 0x00, 0x00, 0x00
	.word ov02_0224B7CC
	.word ov02_0224B804
	.word ov02_0224B808
	.word ov02_0224B87C

	.global ov02_022534A4
ov02_022534A4: ; 0x022534A4
	.byte 0x24, 0x00, 0x00, 0x00
	.word ov02_0224B350
	.word sub_02068DD4
	.word ov02_0224B3FC
	.word sub_02068DD0

	.global ov02_022534B8
ov02_022534B8: ; 0x022534B8
	.word ov02_0224B494
	.word ov02_0224B4AC
	.word ov02_0224B5F0
	.word ov02_0224B638
	.word ov02_0224B664
	.word ov02_0224B68C

	.global ov02_022534D0
ov02_022534D0:
	.word ov02_02248F88
	.word ov02_02249088
	.word ov02_022490BC
	.word ov02_022491A8
	.word ov02_022491CC
	.word ov02_02249290
	.word ov02_0224939C
	.word ov02_022493EC

	.global ov02_022534F0
ov02_022534F0: ; 0x022534F0
	.word ov02_022495D0
	.word ov02_02249A5C
	.word ov02_0224B938
	.word ov02_0224B964
	.word ov02_02249AD8
	.word ov02_02249AF0
	.word ov02_02249B80
	.word ov02_02249BA8
	.word ov02_02249C74
	.word ov02_02249CD8
	.word ov02_02249954
	.word ov02_0224997C

	.global ov02_02253520
ov02_02253520:
	.global hgasset_asm_overlay_02_02248728_0224AC38_022533FC_s_ov02_02253520
	.set hgasset_asm_overlay_02_02248728_0224AC38_022533FC_s_ov02_02253520, ov02_02253520 ; 0x02253520
	.byte 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0

	.global ov02_02253550
ov02_02253550: ; 0x02253550
	.word ov02_022495B8
	.word ov02_022495E8
	.word ov02_02249658
	.word ov02_02249690
	.word ov02_022496D0
	.word ov02_02249754
	.word ov02_02249774
	.word ov02_022497C0
	.word ov02_02249838
	.word ov02_02249858
	.word ov02_022498BC
	.word ov02_02249940
	.word ov02_02249968
	.word ov02_0224997C

	.global ov02_02253588
ov02_02253588: ; 0x02253588
	.word ov02_022495B8
	.word ov02_022499EC
	.word ov02_02249658
	.word ov02_02249690
	.word ov02_022496D0
	.word ov02_02249754
	.word ov02_02249774
	.word ov02_022497C0
	.word ov02_02249838
	.word ov02_02249858
	.word ov02_02249AC4
	.word ov02_02249AD8
	.word ov02_02249AF0
	.word ov02_02249B10
	.word ov02_02249B38
	.word ov02_02249B60
	.word ov02_02249BA8
	.word ov02_02249BD8
	.word ov02_02249C74
	.word ov02_02249CD8
	.word ov02_02249940
	.word ov02_02249968
	.word ov02_0224997C

	.global ov02_022535E4
ov02_022535E4:
	.global hgasset_asm_overlay_02_02248728_0224AC38_022533FC_s_ov02_022535E4
	.set hgasset_asm_overlay_02_02248728_0224AC38_022533FC_s_ov02_022535E4, ov02_022535E4 ; 0x022535E4
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00
	.byte 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
