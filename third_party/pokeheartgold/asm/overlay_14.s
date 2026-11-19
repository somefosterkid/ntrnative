#include "constants/items.h"
#include "constants/pokemon.h"
	.include "macros.inc"
	.include "overlay_14.inc"
	.include "global.inc"

	.public ov14_021E5A14
	.public ov14_021E89B8
	.public ov14_021E8A7C
	.public ov14_021E8ACC
	.public ov14_021E8B1C
	.public ov14_021E8B3C
	.public ov14_021E8B80
	.public ov14_021EAFAC
	.public ov14_021EB0E4
	.public ov14_021EB170
	.public ov14_021EB18C
	.public ov14_021EB1A4
	.public ov14_021EB1C0
	.public ov14_021EB1E0
	.public ov14_021EB218
	.public ov14_021EB27C
	.public ov14_021EB290
	.public ov14_021EB2A8
	.public ov14_021EB2EC
	.public ov14_021EB388
	.public ov14_021EB7B0
	.public ov14_021EB7E4
	.public ov14_021EB8C0
	.public ov14_021EBAEC
	.public ov14_021EBB3C
	.public ov14_021EBDCC
	.public ov14_021EBDE0
	.public ov14_021EBDE8
	.public ov14_021EBE2C
	.public ov14_021EBE68
	.public ov14_021EBF8C
	.public ov14_021EBF9C
	.public ov14_021EC0EC
	.public ov14_021EC128
	.public ov14_021EC13C
	.public ov14_021EC150
	.public ov14_021EC23C
	.public ov14_021EC2A4
	.public ov14_021EC2EC
	.public ov14_021EC300
	.public ov14_021EC344
	.public ov14_021EC354
	.public ov14_021EC3A8
	.public ov14_021EC3D8
	.public ov14_021EC710
	.public ov14_021EC730
	.public ov14_021EC7D4
	.public ov14_021EC854
	.public ov14_021EC8D0
	.public ov14_021ECDA8
	.public ov14_021ECF58
	.public ov14_021ED1AC
	.public ov14_021ED1E8
	.public ov14_021ED258
	.public ov14_021ED2C8
	.public ov14_021ED2DC
	.public ov14_021ED350
	.public ov14_021ED35C
	.public ov14_021ED38C
	.public ov14_021ED414
	.public ov14_021ED5B0
	.public ov14_021ED60C
	.public ov14_021ED620
	.public ov14_021ED62C
	.public ov14_021ED650
	.public ov14_021ED684
	.public ov14_021ED6A4
	.public ov14_021ED6C4
	.public ov14_021ED6D0
	.public ov14_021ED760
	.public ov14_021ED7B8
	.public ov14_021ED7DC
	.public ov14_021ED820
	.public ov14_021ED920
	.public ov14_021ED940
	.public ov14_021ED960
	.public ov14_021ED96C
	.public ov14_021ED9AC
	.public ov14_021ED9EC
	.public ov14_021EDA1C
	.public ov14_021EDA3C
	.public ov14_021EDA4C
	.public ov14_021EDE20
	.public ov14_021EDE38
	.public ov14_021EDE70
	.public ov14_021EDE88
	.public ov14_021EDF08
	.public ov14_021EDF28
	.public ov14_021EDF90
	.public ov14_021EDFA0
	.public ov14_021EE26C
	.public ov14_021EE328
	.public ov14_021EE338
	.public ov14_021EE35C
	.public ov14_021EE380
	.public ov14_021EE3C8
	.public ov14_021EE4AC
	.public ov14_021EE4D8
	.public ov14_021EE4E4
	.public ov14_021EE4FC
	.public ov14_021EE538
	.public ov14_021EE578
	.public ov14_021EE5C8
	.public ov14_021EE5E8
	.public ov14_021EE664
	.public ov14_021EE684
	.public ov14_021EE6D0
	.public ov14_021EE6F8
	.public ov14_021EE728
	.public ov14_021EE7B4
	.public ov14_021EE7C4
	.public ov14_021EE7D4
	.public ov14_021EE7E4
	.public ov14_021EE7F4
	.public ov14_021EE810
	.public ov14_021EE820
	.public ov14_021EE830
	.public ov14_021EE840
	.public ov14_021EE850
	.public ov14_021EE860
	.public ov14_021EE87C
	.public ov14_021EEBFC
	.public ov14_021EEC8C
	.public ov14_021EEC9C
	.public ov14_021EED28
	.public ov14_021EEDB8
	.public ov14_021EEE94
	.public ov14_021EEED4
	.public ov14_021EEF34
	.public ov14_021EEF8C
	.public ov14_021EF024
	.public ov14_021EF190
	.public ov14_021EF1EC
	.public ov14_021EF248
	.public ov14_021EF6D4
	.public ov14_021EF6E4
	.public ov14_021EF6FC
	.public ov14_021EF8AC
	.public ov14_021EF920
	.public ov14_021EF93C
	.public ov14_021EF9BC
	.public ov14_021EF9CC
	.public ov14_021EFB64
	.public ov14_021EFDE4
	.public ov14_021EFDF4
	.public ov14_021EFF00
	.public ov14_021F00A0
	.public ov14_021F00BC
	.public ov14_021F0120
	.public ov14_021F0164
	.public ov14_021F0198
	.public ov14_021F01B8
	.public ov14_021F1F44
	.public ov14_021F2010
	.public ov14_021F2020
	.public ov14_021F20C4
	.public ov14_021F20F4
	.public ov14_021F21B4
	.public ov14_021F21D0
	.public ov14_021F2574
	.public ov14_021F259C
	.public ov14_021F25C4
	.public ov14_021F25D4
	.public ov14_021F25E4
	.public ov14_021F2610
	.public ov14_021F261C
	.public ov14_021F2624
	.public ov14_021F262C
	.public ov14_021F2634
	.public ov14_021F263C
	.public ov14_021F2690
	.public ov14_021F26B4
	.public ov14_021F26F0
	.public ov14_021F2700
	.public ov14_021F2718
	.public ov14_021F2728
	.public ov14_021F2734
	.public ov14_021F2760
	.public ov14_021F276C
	.public ov14_021F2778
	.public ov14_021F27CC
	.public ov14_021F27D4
	.public ov14_021F2810
	.public ov14_021F2818
	.public ov14_021F2858
	.public ov14_021F2874
	.public ov14_021F2890
	.public ov14_021F2898
	.public ov14_021F28FC
	.public ov14_021F2914
	.public ov14_021F2930
	.public ov14_021F29C4
	.public ov14_021F6B60
	.public ov14_021F6B7C
	.public ov14_021F6B8C
	.public ov14_021F6BA4
	.public ov14_021F6C3C
	.public ov14_021F6C94
	.public ov14_021F6D14
	.public ov14_021F6E68
	.public ov14_021F6F08
	.public ov14_021F6F70
	.public ov14_021F7010
	.public ov14_021F708C
	.public ov14_021F70A4
	.public ov14_021F70F4
	.public ov14_021F7184
	.public ov14_021F7308
	.public ov14_021F73E4
	.public ov14_021F7444
	.public ov14_021F7528
	.public ov14_021F7594
	.public ov14_021F75AC
	.public ov14_021F7620
	.public ov14_021F7670
	.public ov14_021F7688
	.public ov14_021F7700
	.public ov14_021F7854
	.public ov14_021F7878
	.public ov14_021F78F8
	.public ov14_021F7A2C
	.data

	.data

	.global _021F7BBC
_021F7BBC:
	.global hgasset_asm_overlay_14_s__021F7BBC
	.set hgasset_asm_overlay_14_s__021F7BBC, _021F7BBC
	.byte 0, 0, 0, 0

	.global ov14_021F7BC0
ov14_021F7BC0:
	.global hgasset_asm_overlay_14_s_ov14_021F7BC0
	.set hgasset_asm_overlay_14_s_ov14_021F7BC0, ov14_021F7BC0 ; 0x021F7BC0
	.byte 0, 0x00, 0, 0, 0, 0x00, 0, 0x00

	.global ov14_021F7BC8
ov14_021F7BC8:
	.global hgasset_asm_overlay_14_s_ov14_021F7BC8
	.set hgasset_asm_overlay_14_s_ov14_021F7BC8, ov14_021F7BC8 ; 0x021F7BC8
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7BD8
ov14_021F7BD8:
	.global hgasset_asm_overlay_14_s_ov14_021F7BD8
	.set hgasset_asm_overlay_14_s_ov14_021F7BD8, ov14_021F7BD8 ; 0x021F7BD8
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov14_021F7BF0
ov14_021F7BF0:
	.global hgasset_asm_overlay_14_s_ov14_021F7BF0
	.set hgasset_asm_overlay_14_s_ov14_021F7BF0, ov14_021F7BF0 ; 0x021F7BF0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0

	.global ov14_021F7C08
ov14_021F7C08:
	.global hgasset_asm_overlay_14_s_ov14_021F7C08
	.set hgasset_asm_overlay_14_s_ov14_021F7C08, ov14_021F7C08 ; 0x021F7C08
	.byte 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.global ov14_021F7C20
ov14_021F7C20:
	.global hgasset_asm_overlay_14_s_ov14_021F7C20
	.set hgasset_asm_overlay_14_s_ov14_021F7C20, ov14_021F7C20 ; 0x021F7C20
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7C3C
ov14_021F7C3C:
	.global hgasset_asm_overlay_14_s_ov14_021F7C3C
	.set hgasset_asm_overlay_14_s_ov14_021F7C3C, ov14_021F7C3C ; 0x021F7C3C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7C58
ov14_021F7C58:
	.global hgasset_asm_overlay_14_s_ov14_021F7C58
	.set hgasset_asm_overlay_14_s_ov14_021F7C58, ov14_021F7C58 ; 0x021F7C58
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7C74
ov14_021F7C74:
	.global hgasset_asm_overlay_14_s_ov14_021F7C74
	.set hgasset_asm_overlay_14_s_ov14_021F7C74, ov14_021F7C74 ; 0x021F7C74
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7C90
ov14_021F7C90:
	.global hgasset_asm_overlay_14_s_ov14_021F7C90
	.set hgasset_asm_overlay_14_s_ov14_021F7C90, ov14_021F7C90 ; 0x021F7C90
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7CAC
ov14_021F7CAC:
	.global hgasset_asm_overlay_14_s_ov14_021F7CAC
	.set hgasset_asm_overlay_14_s_ov14_021F7CAC, ov14_021F7CAC ; 0x021F7CAC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7CC8
ov14_021F7CC8:
	.global hgasset_asm_overlay_14_s_ov14_021F7CC8
	.set hgasset_asm_overlay_14_s_ov14_021F7CC8, ov14_021F7CC8 ; 0x021F7CC8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F7CE4
ov14_021F7CE4:
	.global hgasset_asm_overlay_14_s_ov14_021F7CE4
	.set hgasset_asm_overlay_14_s_ov14_021F7CE4, ov14_021F7CE4 ; 0x021F7CE4
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov14_021F7D0C
ov14_021F7D0C:
	.global hgasset_asm_overlay_14_s_ov14_021F7D0C
	.set hgasset_asm_overlay_14_s_ov14_021F7D0C, ov14_021F7D0C ; 0x021F7D0C
	.byte 0x00, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

	.global ov14_021F7D14
ov14_021F7D14:
	.global hgasset_asm_overlay_14_s_ov14_021F7D14
	.set hgasset_asm_overlay_14_s_ov14_021F7D14, ov14_021F7D14 ; 0x021F7D14
	.byte POCKET_ITEMS
	.byte POCKET_MEDICINE
	.byte POCKET_BALLS
	.byte POCKET_TMHMS
	.byte POCKET_BERRIES
	.byte POCKET_BATTLE_ITEMS
	.byte POCKET_KEY_ITEMS
	.byte 0

	.global ov14_021F7D1C
ov14_021F7D1C:
	.global hgasset_asm_overlay_14_s_ov14_021F7D1C
	.set hgasset_asm_overlay_14_s_ov14_021F7D1C, ov14_021F7D1C ; 0x021F7D1C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov14_021F7D2C
ov14_021F7D2C:
	.global hgasset_asm_overlay_14_s_ov14_021F7D2C
	.set hgasset_asm_overlay_14_s_ov14_021F7D2C, ov14_021F7D2C ; 0x021F7D2C
	.byte 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov14_021F7D3C
ov14_021F7D3C:
	.global hgasset_asm_overlay_14_s_ov14_021F7D3C
	.set hgasset_asm_overlay_14_s_ov14_021F7D3C, ov14_021F7D3C ; 0x021F7D3C
	.byte 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov14_021F7D50
ov14_021F7D50: ; 0x021F7D50
	.word ov14_021E89B8, ov14_021E8A7C, 0x0000000F
	.word ov14_021E8ACC, ov14_021E8B1C, 0x00000011
	.word ov14_021E8B3C, ov14_021E8B80, 0x00000010

	.global ov14_021F7D74
ov14_021F7D74: ; 0x021F7D74
	.word ov14_021F2020

	.global ov14_021F7D78
ov14_021F7D78: ; 0x021F7D78
	.word ov14_021EB7E4
	.word ov14_021F20C4
	.word ov14_021EC300
	.word ov14_021F20F4
	.word ov14_021EEDB8
	.word ov14_021F21B4
	.word ov14_021F21D0
	.word ov14_021F21D0
	.word ov14_021F21B4

	.global ov14_021F7D9C
ov14_021F7D9C: ; 0x021F7D9C
	.word ov14_021EAFAC
	.word ov14_021EB0E4
	.word ov14_021EB170
	.word ov14_021EB18C
	.word ov14_021EB1A4
	.word ov14_021EB1C0
	.word ov14_021EB1E0
	.word ov14_021EB218
	.word ov14_021EB27C
	.word ov14_021EB290
	.word ov14_021EB2A8
	.word ov14_021EB2EC
	.word ov14_021EB388
	.word ov14_021EB7B0
	.word ov14_021EB7E4
	.word ov14_021EB8C0
	.word ov14_021EBAEC
	.word ov14_021EBB3C
	.word ov14_021EBDCC
	.word ov14_021EBDE0
	.word ov14_021EBDE8
	.word ov14_021EBE2C
	.word ov14_021EBE68
	.word ov14_021EBF8C
	.word ov14_021EE820
	.word ov14_021EBF9C
	.word ov14_021EC0EC
	.word ov14_021EC128
	.word ov14_021EC13C
	.word ov14_021EC150
	.word ov14_021EC2A4
	.word ov14_021EC2EC
	.word ov14_021EC300
	.word ov14_021EC23C
	.word ov14_021EC354
	.word ov14_021EC3A8
	.word ov14_021EC3D8
	.word ov14_021EC710
	.word ov14_021EC730
	.word ov14_021EC7D4
	.word ov14_021EC854
	.word ov14_021EC8D0
	.word ov14_021ECDA8
	.word ov14_021ECF58
	.word ov14_021ED1AC
	.word ov14_021ED1E8
	.word ov14_021ED258
	.word ov14_021ED2C8
	.word ov14_021ED2DC
	.word ov14_021ED350
	.word ov14_021ED35C
	.word ov14_021ED38C
	.word ov14_021ED5B0
	.word ov14_021ED60C
	.word ov14_021ED620
	.word ov14_021ED62C
	.word ov14_021ED650
	.word ov14_021ED684
	.word ov14_021ED6A4
	.word ov14_021ED6C4
	.word ov14_021ED6D0
	.word ov14_021ED414
	.word ov14_021EE830
	.word ov14_021ED760
	.word ov14_021ED7B8
	.word ov14_021ED7DC
	.word ov14_021ED820
	.word ov14_021ED920
	.word ov14_021ED940
	.word ov14_021ED960
	.word ov14_021ED96C
	.word ov14_021ED9AC
	.word ov14_021ED9EC
	.word ov14_021EE840
	.word ov14_021EE7B4
	.word ov14_021EE7C4
	.word ov14_021EE7D4
	.word ov14_021EDA1C
	.word ov14_021EDA3C
	.word ov14_021F0198
	.word ov14_021F01B8
	.word ov14_021EDA4C
	.word ov14_021EDE20
	.word ov14_021EDE38
	.word ov14_021EDE70
	.word ov14_021EDE88
	.word ov14_021EDF08
	.word ov14_021EDF28
	.word ov14_021EE850
	.word ov14_021EE860
	.word ov14_021EDF90
	.word ov14_021EDFA0
	.word ov14_021EE26C
	.word ov14_021EE328
	.word ov14_021EE338
	.word ov14_021EE35C
	.word ov14_021EE380
	.word ov14_021EE3C8
	.word ov14_021EE4AC
	.word ov14_021EE4D8
	.word ov14_021EE4E4
	.word ov14_021EE4FC
	.word ov14_021EE538
	.word ov14_021EE578
	.word ov14_021EE5C8
	.word ov14_021EE5E8
	.word ov14_021EE664
	.word ov14_021EE684
	.word ov14_021EE6D0
	.word ov14_021EE6F8
	.word ov14_021EE728
	.word ov14_021EE7E4
	.word ov14_021EE7F4
	.word ov14_021EE810
	.word ov14_021EC344
	.word ov14_021F1F44
	.word ov14_021F2010
	.word ov14_021EE87C
	.word ov14_021EEC8C
	.word ov14_021EEBFC
	.word ov14_021EEC9C
	.word ov14_021EED28
	.word ov14_021EEDB8
	.word ov14_021EEE94
	.word ov14_021EEED4
	.word ov14_021EEF34
	.word ov14_021EEF8C
	.word ov14_021EF024
	.word ov14_021EF190
	.word ov14_021EF1EC
	.word ov14_021EF248
	.word ov14_021EF6D4
	.word ov14_021EF6E4
	.word ov14_021EF6FC
	.word ov14_021EF8AC
	.word ov14_021EF920
	.word ov14_021EF93C
	.word ov14_021EF9BC
	.word ov14_021EF9CC
	.word ov14_021EFB64
	.word ov14_021EFDE4
	.word ov14_021EFDF4
	.word ov14_021EFF00
	.word ov14_021F00A0
	.word ov14_021F00BC
	.word ov14_021F0120
	.word ov14_021F0164
	.word ov14_021F2574
	.word ov14_021F259C
	.word ov14_021F25C4
	.word ov14_021F25D4
	.word ov14_021F2610
	.word ov14_021F261C
	.word ov14_021F2624
	.word ov14_021F262C
	.word ov14_021F2634
	.word ov14_021F263C
	.word ov14_021F2690
	.word ov14_021F26B4
	.word ov14_021F26F0
	.word ov14_021F2700
	.word ov14_021F2718
	.word ov14_021F2728
	.word ov14_021F2734
	.word ov14_021F2760
	.word ov14_021F276C
	.word ov14_021F2778
	.word ov14_021F27CC
	.word ov14_021F27D4
	.word ov14_021F2810
	.word ov14_021F2818
	.word ov14_021F2858
	.word ov14_021F2874
	.word ov14_021F2890
	.word ov14_021F2898
	.word ov14_021F28FC
	.word ov14_021F2914
	.word ov14_021F2930
	.word ov14_021F25E4

	.global ov14_021F8068
ov14_021F8068:
	.global hgasset_asm_overlay_14_s_ov14_021F8068
	.set hgasset_asm_overlay_14_s_ov14_021F8068, ov14_021F8068 ; 0x021F8068
	.byte 0, 0, 0, 0, 0, 0, 0x00, 0x00

	.global ov14_021F8070
ov14_021F8070:
	.global hgasset_asm_overlay_14_s_ov14_021F8070
	.set hgasset_asm_overlay_14_s_ov14_021F8070, ov14_021F8070 ; 0x021F8070
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0

	.global ov14_021F8078
ov14_021F8078:
	.global hgasset_asm_overlay_14_s_ov14_021F8078
	.set hgasset_asm_overlay_14_s_ov14_021F8078, ov14_021F8078 ; 0x021F8078
	.byte 0x00, 0x00, 0, 0, 0, 0, 0, 0

	.global ov14_021F8080
ov14_021F8080:
	.global hgasset_asm_overlay_14_s_ov14_021F8080
	.set hgasset_asm_overlay_14_s_ov14_021F8080, ov14_021F8080 ; 0x021F8080
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00

	.global ov14_021F808C
ov14_021F808C:
	.global hgasset_asm_overlay_14_s_ov14_021F808C
	.set hgasset_asm_overlay_14_s_ov14_021F808C, ov14_021F808C ; 0x021F808C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0

	.global ov14_021F8098
ov14_021F8098:
	.global hgasset_asm_overlay_14_s_ov14_021F8098
	.set hgasset_asm_overlay_14_s_ov14_021F8098, ov14_021F8098 ; 0x021F8098
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov14_021F80A8
ov14_021F80A8:
	.global hgasset_asm_overlay_14_s_ov14_021F80A8
	.set hgasset_asm_overlay_14_s_ov14_021F80A8, ov14_021F80A8 ; 0x021F80A8
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0x00, 0, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00

	.global ov14_021F80BC
ov14_021F80BC:
	.global hgasset_asm_overlay_14_s_ov14_021F80BC
	.set hgasset_asm_overlay_14_s_ov14_021F80BC, ov14_021F80BC ; 0x021F80BC
	.byte 0, 0x00

	.global ov14_021F80BE
ov14_021F80BE:
	.global hgasset_asm_overlay_14_s_ov14_021F80BE
	.set hgasset_asm_overlay_14_s_ov14_021F80BE, ov14_021F80BE ; 0x021F80BE
	.byte 0, 0x00
	.byte 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0
	.byte 0, 0x00, 0, 0

	.global ov14_021F80D4
ov14_021F80D4:
	.global hgasset_asm_overlay_14_s_ov14_021F80D4
	.set hgasset_asm_overlay_14_s_ov14_021F80D4, ov14_021F80D4 ; 0x021F80D4
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F80EC
ov14_021F80EC:
	.global hgasset_asm_overlay_14_s_ov14_021F80EC
	.set hgasset_asm_overlay_14_s_ov14_021F80EC, ov14_021F80EC ; 0x021F80EC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00

	.global ov14_021F810C
ov14_021F810C:
	.global hgasset_asm_overlay_14_s_ov14_021F810C
	.set hgasset_asm_overlay_14_s_ov14_021F810C, ov14_021F810C ; 0x021F810C
	.byte 0, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8140
ov14_021F8140:
	.global hgasset_asm_overlay_14_s_ov14_021F8140
	.set hgasset_asm_overlay_14_s_ov14_021F8140, ov14_021F8140 ; 0x021F8140
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8174
ov14_021F8174:
	.global hgasset_asm_overlay_14_s_ov14_021F8174
	.set hgasset_asm_overlay_14_s_ov14_021F8174, ov14_021F8174 ; 0x021F8174
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F81A8
ov14_021F81A8:
	.global hgasset_asm_overlay_14_s_ov14_021F81A8
	.set hgasset_asm_overlay_14_s_ov14_021F81A8, ov14_021F81A8 ; 0x021F81A8
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F81DC
ov14_021F81DC:
	.global hgasset_asm_overlay_14_s_ov14_021F81DC
	.set hgasset_asm_overlay_14_s_ov14_021F81DC, ov14_021F81DC ; 0x021F81DC
	.byte 0, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8210
ov14_021F8210:
	.global hgasset_asm_overlay_14_s_ov14_021F8210
	.set hgasset_asm_overlay_14_s_ov14_021F8210, ov14_021F8210 ; 0x021F8210
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8244
ov14_021F8244:
	.global hgasset_asm_overlay_14_s_ov14_021F8244
	.set hgasset_asm_overlay_14_s_ov14_021F8244, ov14_021F8244 ; 0x021F8244
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8278
ov14_021F8278:
	.global hgasset_asm_overlay_14_s_ov14_021F8278
	.set hgasset_asm_overlay_14_s_ov14_021F8278, ov14_021F8278 ; 0x021F8278
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F82AC
ov14_021F82AC:
	.global hgasset_asm_overlay_14_s_ov14_021F82AC
	.set hgasset_asm_overlay_14_s_ov14_021F82AC, ov14_021F82AC ; 0x021F82AC
	.byte 0, 0x00, 0, 0
	.byte 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F82E0
ov14_021F82E0:
	.global hgasset_asm_overlay_14_s_ov14_021F82E0
	.set hgasset_asm_overlay_14_s_ov14_021F82E0, ov14_021F82E0 ; 0x021F82E0
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8314
ov14_021F8314:
	.global hgasset_asm_overlay_14_s_ov14_021F8314
	.set hgasset_asm_overlay_14_s_ov14_021F8314, ov14_021F8314 ; 0x021F8314
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8348
ov14_021F8348:
	.global hgasset_asm_overlay_14_s_ov14_021F8348
	.set hgasset_asm_overlay_14_s_ov14_021F8348, ov14_021F8348 ; 0x021F8348
	.byte 0, 0x00, 0, 0, 0x00, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F837C
ov14_021F837C:
	.global hgasset_asm_overlay_14_s_ov14_021F837C
	.set hgasset_asm_overlay_14_s_ov14_021F837C, ov14_021F837C ; 0x021F837C
	.byte 0, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F83B0
ov14_021F83B0:
	.global hgasset_asm_overlay_14_s_ov14_021F83B0
	.set hgasset_asm_overlay_14_s_ov14_021F83B0, ov14_021F83B0 ; 0x021F83B0
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F83E4
ov14_021F83E4:
	.global hgasset_asm_overlay_14_s_ov14_021F83E4
	.set hgasset_asm_overlay_14_s_ov14_021F83E4, ov14_021F83E4 ; 0x021F83E4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8418
ov14_021F8418:
	.global hgasset_asm_overlay_14_s_ov14_021F8418
	.set hgasset_asm_overlay_14_s_ov14_021F8418, ov14_021F8418 ; 0x021F8418
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F844C
ov14_021F844C:
	.global hgasset_asm_overlay_14_s_ov14_021F844C
	.set hgasset_asm_overlay_14_s_ov14_021F844C, ov14_021F844C ; 0x021F844C
	.byte 0, 0x00, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov14_021F8480
ov14_021F8480:
	.global hgasset_asm_overlay_14_s_ov14_021F8480
	.set hgasset_asm_overlay_14_s_ov14_021F8480, ov14_021F8480 ; 0x021F8480
	.byte 0, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.global ov14_021F84B4
ov14_021F84B4:
	.global hgasset_asm_overlay_14_s_ov14_021F84B4
	.set hgasset_asm_overlay_14_s_ov14_021F84B4, ov14_021F84B4 ; 0x021F84B4
	.byte 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0
	.byte 0, 0, 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0
	.byte 0, 0, 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0
	.byte 0, 0, 0, 0

	.global ov14_021F8614
ov14_021F8614:
	.global hgasset_asm_overlay_14_s_ov14_021F8614
	.set hgasset_asm_overlay_14_s_ov14_021F8614, ov14_021F8614 ; 0x021F8614
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov14_021F8630
ov14_021F8630:
	.global hgasset_asm_overlay_14_s_ov14_021F8630
	.set hgasset_asm_overlay_14_s_ov14_021F8630, ov14_021F8630 ; 0x021F8630
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov14_021F864C
ov14_021F864C:
	.global hgasset_asm_overlay_14_s_ov14_021F864C
	.set hgasset_asm_overlay_14_s_ov14_021F864C, ov14_021F864C ; 0x021F864C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

	.global ov14_021F86C8
ov14_021F86C8:
	.global hgasset_asm_overlay_14_s_ov14_021F86C8
	.set hgasset_asm_overlay_14_s_ov14_021F86C8, ov14_021F86C8 ; 0x021F86C8
	.byte 0, 0, 0x00, 0

ov14_021F86CC:
	.word ov14_021F7878
	.word ov14_021F6B7C
	.word ov14_021F78F8
	.word ov14_021F7A2C

ov14_021F86DC:
	.word ov14_021F6F08
	.word ov14_021F6B7C
	.word ov14_021F6B8C
	.word ov14_021F6F70

ov14_021F86EC:
	.word ov14_021F6C3C
	.word ov14_021F6B7C
	.word ov14_021F6B8C
	.word ov14_021F6BA4

ov14_021F86FC:
	.word ov14_021F6C94
	.word ov14_021F6B7C
	.word ov14_021F6D14
	.word ov14_021F6E68

ov14_021F870C:
	.word ov14_021F7688
	.word ov14_021F6B7C
	.word ov14_021F7700
	.word ov14_021F7854

ov14_021F871C:
	.word ov14_021F73E4
	.word ov14_021F6B7C
	.word ov14_021F7444
	.word ov14_021F6BA4

ov14_021F872C:
	.word ov14_021F70F4
	.word ov14_021F6B7C
	.word ov14_021F7184
	.word ov14_021F7308

ov14_021F873C:
	.word ov14_021F7010
	.word ov14_021F6B7C
	.word ov14_021F708C
	.word ov14_021F70A4

ov14_021F874C:
	.word ov14_021F6B60
	.word ov14_021F6B7C
	.word ov14_021F6B8C
	.word ov14_021F6BA4

ov14_021F875C:
	.word ov14_021F7620
	.word ov14_021F6B7C
	.word ov14_021F7670
	.word ov14_021F6BA4

ov14_021F876C:
	.word ov14_021F7528
	.word ov14_021F6B7C
	.word ov14_021F7594
	.word ov14_021F75AC

ov14_021F877C:
	.global hgasset_asm_overlay_14_s_ov14_021F877C
	.set hgasset_asm_overlay_14_s_ov14_021F877C, ov14_021F877C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F87A0:
	.global hgasset_asm_overlay_14_s_ov14_021F87A0
	.set hgasset_asm_overlay_14_s_ov14_021F87A0, ov14_021F87A0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

ov14_021F87C4:
	.global hgasset_asm_overlay_14_s_ov14_021F87C4
	.set hgasset_asm_overlay_14_s_ov14_021F87C4, ov14_021F87C4
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F87F0:
	.global hgasset_asm_overlay_14_s_ov14_021F87F0
	.set hgasset_asm_overlay_14_s_ov14_021F87F0, ov14_021F87F0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F881C:
	.global hgasset_asm_overlay_14_s_ov14_021F881C
	.set hgasset_asm_overlay_14_s_ov14_021F881C, ov14_021F881C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F8850:
	.global hgasset_asm_overlay_14_s_ov14_021F8850
	.set hgasset_asm_overlay_14_s_ov14_021F8850, ov14_021F8850
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0x00, 0x00, 0x00

ov14_021F8884:
	.global hgasset_asm_overlay_14_s_ov14_021F8884
	.set hgasset_asm_overlay_14_s_ov14_021F8884, ov14_021F8884
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F88C0:
	.global hgasset_asm_overlay_14_s_ov14_021F88C0
	.set hgasset_asm_overlay_14_s_ov14_021F88C0, ov14_021F88C0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F8900:
	.global hgasset_asm_overlay_14_s_ov14_021F8900
	.set hgasset_asm_overlay_14_s_ov14_021F8900, ov14_021F8900
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F8940:
	.global hgasset_asm_overlay_14_s_ov14_021F8940
	.set hgasset_asm_overlay_14_s_ov14_021F8940, ov14_021F8940
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F8990:
	.global hgasset_asm_overlay_14_s_ov14_021F8990
	.set hgasset_asm_overlay_14_s_ov14_021F8990, ov14_021F8990
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F89E0:
	.global hgasset_asm_overlay_14_s_ov14_021F89E0
	.set hgasset_asm_overlay_14_s_ov14_021F89E0, ov14_021F89E0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F8A40:
	.global hgasset_asm_overlay_14_s_ov14_021F8A40
	.set hgasset_asm_overlay_14_s_ov14_021F8A40, ov14_021F8A40
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F8AA0:
	.global hgasset_asm_overlay_14_s_ov14_021F8AA0
	.set hgasset_asm_overlay_14_s_ov14_021F8AA0, ov14_021F8AA0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0x00, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

	.global ov14_021F8B10
ov14_021F8B10: ; 0x021F8B10
	.word ov14_021F881C, ov14_021F8A40, ov14_021F86EC
	.word ov14_021F87F0, ov14_021F8940, ov14_021F86FC
	.word ov14_021F8C30, ov14_021F8F68, ov14_021F86DC
	.word ov14_021F8CD0, ov14_021F90A0, ov14_021F873C
	.word ov14_021F8D7C, ov14_021F91F0, ov14_021F872C
	.word ov14_021F8884, ov14_021F8AA0, ov14_021F871C
	.word ov14_021F8B94, ov14_021F8E38, ov14_021F876C
	.word ov14_021F87C4, ov14_021F8990, ov14_021F875C
	.word ov14_021F877C, ov14_021F88C0, ov14_021F874C
	.word ov14_021F8850, ov14_021F89E0, ov14_021F870C
	.word ov14_021F87A0, ov14_021F8900, ov14_021F86CC

ov14_021F8B94:
	.global hgasset_asm_overlay_14_s_ov14_021F8B94
	.set hgasset_asm_overlay_14_s_ov14_021F8B94, ov14_021F8B94
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F8C30:
	.global hgasset_asm_overlay_14_s_ov14_021F8C30
	.set hgasset_asm_overlay_14_s_ov14_021F8C30, ov14_021F8C30
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F8CD0:
	.global hgasset_asm_overlay_14_s_ov14_021F8CD0
	.set hgasset_asm_overlay_14_s_ov14_021F8CD0, ov14_021F8CD0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00

ov14_021F8D7C:
	.global hgasset_asm_overlay_14_s_ov14_021F8D7C
	.set hgasset_asm_overlay_14_s_ov14_021F8D7C, ov14_021F8D7C
	.byte 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0, 0, 0x00, 0x00, 0x00

ov14_021F8E38:
	.global hgasset_asm_overlay_14_s_ov14_021F8E38
	.set hgasset_asm_overlay_14_s_ov14_021F8E38, ov14_021F8E38
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F8F68:
	.global hgasset_asm_overlay_14_s_ov14_021F8F68
	.set hgasset_asm_overlay_14_s_ov14_021F8F68, ov14_021F8F68
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0x00, 0, 0, 0x00, 0x00, 0x00, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0

ov14_021F90A0:
	.global hgasset_asm_overlay_14_s_ov14_021F90A0
	.set hgasset_asm_overlay_14_s_ov14_021F90A0, ov14_021F90A0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0x00
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
ov14_021F91F0:
	.global hgasset_asm_overlay_14_s_ov14_021F91F0
	.set hgasset_asm_overlay_14_s_ov14_021F91F0, ov14_021F91F0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0, 0, 0, 0
