	.include "macros.inc"
	.include "overlay_80_0222BDF4.inc"
	.include "global.inc"

	.public FrtCmd_000
	.public FrtCmd_001
	.public FrtCmd_002
	.public FrtCmd_003
	.public FrtCmd_004
	.public FrtCmd_005
	.public FrtCmd_006
	.public FrtCmd_007
	.public FrtCmd_008
	.public FrtCmd_009
	.public FrtCmd_010
	.public FrtCmd_011
	.public FrtCmd_012
	.public FrtCmd_013
	.public FrtCmd_014
	.public FrtCmd_015
	.public FrtCmd_016
	.public FrtCmd_017
	.public FrtCmd_018
	.public FrtCmd_019
	.public FrtCmd_020
	.public FrtCmd_021
	.public FrtCmd_022
	.public FrtCmd_023
	.public FrtCmd_024
	.public FrtCmd_025
	.public FrtCmd_026
	.public FrtCmd_027
	.public FrtCmd_028
	.public FrtCmd_029
	.public FrtCmd_030
	.public FrtCmd_031
	.public FrtCmd_032
	.public FrtCmd_033
	.public FrtCmd_034
	.public FrtCmd_035
	.public FrtCmd_036
	.public FrtCmd_037
	.public FrtCmd_038
	.public FrtCmd_039
	.public FrtCmd_040
	.public FrtCmd_041
	.public FrtCmd_042
	.public FrtCmd_043
	.public FrtCmd_044
	.public FrtCmd_045
	.public FrtCmd_046
	.public FrtCmd_047
	.public FrtCmd_048
	.public FrtCmd_049
	.public FrtCmd_050
	.public FrtCmd_051
	.public FrtCmd_052
	.public FrtCmd_053
	.public FrtCmd_054
	.public FrtCmd_055
	.public FrtCmd_056
	.public FrtCmd_057
	.public FrtCmd_058
	.public FrtCmd_059
	.public FrtCmd_060
	.public FrtCmd_061
	.public FrtCmd_062
	.public FrtCmd_063
	.public FrtCmd_064
	.public FrtCmd_065
	.public FrtCmd_066
	.public FrtCmd_067
	.public FrtCmd_068
	.public FrtCmd_069
	.public FrtCmd_070
	.public FrtCmd_071
	.public FrtCmd_072
	.public FrtCmd_073
	.public FrtCmd_074
	.public FrtCmd_075
	.public FrtCmd_076
	.public FrtCmd_077
	.public FrtCmd_078
	.public FrtCmd_079
	.public FrtCmd_083
	.public FrtCmd_084
	.public FrtCmd_108
	.public FrtCmd_109
	.public FrtCmd_110
	.public FrtCmd_111
	.public FrtCmd_112
	.public FrtCmd_113
	.public FrtCmd_114
	.public FrtCmd_115
	.public FrtCmd_116
	.public FrtCmd_117
	.public FrtCmd_119
	.public FrtCmd_120
	.public FrtCmd_121
	.public FrtCmd_122
	.public FrtCmd_123
	.public FrtCmd_124
	.public FrtCmd_125
	.public FrtCmd_126
	.public FrtCmd_127
	.public FrtCmd_128
	.public FrtCmd_129
	.public FrtCmd_130
	.public FrtCmd_131
	.public FrtCmd_150
	.public FrtCmd_183
	.public FrtCmd_200
	.public FrtCmd_201
	.public FrtCmd_203
	.public FrtCmd_204
	.public FrtCmd_205
	.public FrtCmd_206
	.public FrtCmd_207
	.public ov80_0222BE24
	.public ov80_0222BE9C
	.public ov80_0222D390
	.public ov80_0222D404
	.public ov80_0222D520
	.public ov80_0222D968
	.public ov80_0222DAAC
	.public ov80_0222DC14
	.public ov80_0223BE6C
	.data

	.data

ov80_0223B9CC: ; 0x0223B9CC
	.byte 0xD0, 0x00, 0x00, 0x00

	.global ov80_0223B9D0
ov80_0223B9D0: ; 0x0223B9D0
	.byte 0x01, 0x19, 0x0D, 0x06, 0x04, 0x0E, 0x55, 0x03

	.global ov80_0223B9D8
ov80_0223B9D8: ; 0x0223B9D8
	.byte 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00

	.global ov80_0223B9EC
ov80_0223B9EC: ; 0x0223B9EC
	.word ov80_0222D390
	.word ov80_0222D404
	.word ov80_0222D520
	.word ov80_0222D968
	.word ov80_0222DAAC
	.word ov80_0222DC14

gFrontierCommandTable: ; 0x0223BA04
	.word FrtCmd_000
	.word FrtCmd_001
	.word FrtCmd_002
	.word FrtCmd_003
	.word FrtCmd_004
	.word FrtCmd_005
	.word FrtCmd_006
	.word FrtCmd_007
	.word FrtCmd_008
	.word FrtCmd_009
	.word FrtCmd_010
	.word FrtCmd_011
	.word FrtCmd_012
	.word FrtCmd_013
	.word FrtCmd_014
	.word FrtCmd_015
	.word FrtCmd_016
	.word FrtCmd_017
	.word FrtCmd_018
	.word FrtCmd_019
	.word FrtCmd_020
	.word FrtCmd_021
	.word FrtCmd_022
	.word FrtCmd_023
	.word FrtCmd_024
	.word FrtCmd_025
	.word FrtCmd_026
	.word FrtCmd_027
	.word FrtCmd_028
	.word FrtCmd_029
	.word FrtCmd_030
	.word FrtCmd_031
	.word FrtCmd_032
	.word FrtCmd_033
	.word FrtCmd_034
	.word FrtCmd_035
	.word FrtCmd_036
	.word FrtCmd_037
	.word FrtCmd_038
	.word FrtCmd_039
	.word FrtCmd_040
	.word FrtCmd_041
	.word FrtCmd_042
	.word FrtCmd_043
	.word FrtCmd_044
	.word FrtCmd_045
	.word FrtCmd_046
	.word FrtCmd_047
	.word FrtCmd_048
	.word FrtCmd_049
	.word FrtCmd_050
	.word FrtCmd_051
	.word FrtCmd_052
	.word FrtCmd_053
	.word FrtCmd_054
	.word FrtCmd_055
	.word FrtCmd_056
	.word FrtCmd_057
	.word FrtCmd_058
	.word FrtCmd_059
	.word FrtCmd_060
	.word FrtCmd_061
	.word FrtCmd_062
	.word FrtCmd_063
	.word FrtCmd_064
	.word FrtCmd_065
	.word FrtCmd_066
	.word FrtCmd_067
	.word FrtCmd_068
	.word FrtCmd_069
	.word FrtCmd_070
	.word FrtCmd_071
	.word FrtCmd_072
	.word FrtCmd_073
	.word FrtCmd_074
	.word FrtCmd_075
	.word FrtCmd_076
	.word FrtCmd_077
	.word FrtCmd_078
	.word FrtCmd_079
	.word FrtCmd_080
	.word FrtCmd_081
	.word FrtCmd_082
	.word FrtCmd_083
	.word FrtCmd_084
	.word FrtCmd_085
	.word FrtCmd_086
	.word FrtCmd_087
	.word FrtCmd_088
	.word FrtCmd_089
	.word FrtCmd_090
	.word FrtCmd_091
	.word FrtCmd_092
	.word FrtCmd_093
	.word FrtCmd_094
	.word FrtCmd_095
	.word FrtCmd_096
	.word FrtCmd_097
	.word FrtCmd_098
	.word FrtCmd_099
	.word FrtCmd_100
	.word FrtCmd_101
	.word FrtCmd_102
	.word FrtCmd_103
	.word FrtCmd_104
	.word FrtCmd_105
	.word FrtCmd_106
	.word FrtCmd_107
	.word FrtCmd_108
	.word FrtCmd_109
	.word FrtCmd_110
	.word FrtCmd_111
	.word FrtCmd_112
	.word FrtCmd_113
	.word FrtCmd_114
	.word FrtCmd_115
	.word FrtCmd_116
	.word FrtCmd_117
	.word FrtCmd_118
	.word FrtCmd_119
	.word FrtCmd_120
	.word FrtCmd_121
	.word FrtCmd_122
	.word FrtCmd_123
	.word FrtCmd_124
	.word FrtCmd_125
	.word FrtCmd_126
	.word FrtCmd_127
	.word FrtCmd_128
	.word FrtCmd_129
	.word FrtCmd_130
	.word FrtCmd_131
	.word FrtCmd_132
	.word FrtCmd_133
	.word FrtCmd_134
	.word FrtCmd_135
	.word FrtCmd_136
	.word FrtCmd_137
	.word FrtCmd_138
	.word FrtCmd_139
	.word FrtCmd_140
	.word FrtCmd_141
	.word FrtCmd_142
	.word FrtCmd_143
	.word FrtCmd_144
	.word FrtCmd_145
	.word FrtCmd_146
	.word FrtCmd_147
	.word FrtCmd_148
	.word FrtCmd_149
	.word FrtCmd_150
	.word FrtCmd_151
	.word FrtCmd_152
	.word FrtCmd_153
	.word FrtCmd_154
	.word FrtCmd_155
	.word FrtCmd_156
	.word FrtCmd_157
	.word FrtCmd_158
	.word FrtCmd_159
	.word FrtCmd_160
	.word FrtCmd_161
	.word FrtCmd_162
	.word FrtCmd_163
	.word FrtCmd_164
	.word FrtCmd_165
	.word FrtCmd_166
	.word FrtCmd_167
	.word FrtCmd_168
	.word FrtCmd_169
	.word FrtCmd_170
	.word FrtCmd_171
	.word FrtCmd_172
	.word FrtCmd_173
	.word FrtCmd_174
	.word FrtCmd_175
	.word FrtCmd_176
	.word FrtCmd_177
	.word FrtCmd_178
	.word FrtCmd_179
	.word FrtCmd_180
	.word FrtCmd_181
	.word FrtCmd_182
	.word FrtCmd_183
	.word FrtCmd_ArcadeAlloc
	.word FrtCmd_ArcadeInit
	.word FrtCmd_ArcadeFree
	.word FrtCmd_187
	.word FrtCmd_ArcadeStartBattle
	.word FrtCmd_ArcadeSetPartyBeforeBattle
	.word FrtCmd_ArcadeSetPartyAfterBattle
	.word FrtCmd_ArcadeAction
	.word FrtCmd_ArcadeGetBattleResult
	.word FrtCmd_ArcadeSendBuffer
	.word FrtCmd_ArcadeReceiveBuffer
	.word FrtCmd_LaunchGameBoard
	.word FrtCmd_ArcadePrintMsg
	.word FrtCmd_ArcadeSetEvent
	.word FrtCmd_198
	.word FrtCmd_199
	.word FrtCmd_200
	.word FrtCmd_201
	.word FrtCmd_202
	.word FrtCmd_203
	.word FrtCmd_204
	.word FrtCmd_205
	.word FrtCmd_206
	.word FrtCmd_207

	.global ov80_0223BD44
ov80_0223BD44:
	.global hgasset_asm_overlay_80_0222BDF4_s_ov80_0223BD44
	.set hgasset_asm_overlay_80_0222BDF4_s_ov80_0223BD44, ov80_0223BD44 ; 0x0223BD44
	.byte 0, 0, 0, 0, 0, 0, 0, 0x00

	.global ov80_0223BD4C
ov80_0223BD4C:
	.global hgasset_asm_overlay_80_0222BDF4_s_ov80_0223BD4C
	.set hgasset_asm_overlay_80_0222BDF4_s_ov80_0223BD4C, ov80_0223BD4C ; 0x0223BD4C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.global ov80_0223BD80
ov80_0223BD80:
	.global hgasset_asm_overlay_80_0222BDF4_s_ov80_0223BD80
	.set hgasset_asm_overlay_80_0222BDF4_s_ov80_0223BD80, ov80_0223BD80 ; 0x0223BD80
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00, 0, 0, 0x00, 0x00
	.byte 0, 0, 0x00, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov80_0223BDB4: ; 0x0223BDB4
	.word ov81_0223DD60
	.word ov81_0223DEA8
	.word ov81_0223E234
	.word SDK_OVERLAY_OVY_81_ID

ov80_0223BDC4: ; 0x0223BDC4
	.word ov81_0223DD60
	.word ov81_0223DEA8
	.word ov81_0223E234
	.word SDK_OVERLAY_OVY_81_ID

.public ov80_0223BE6C
	.data

	.global _0223DC20
_0223DC20: ; 0x0223DC20
	.byte 0x00, 0x00

	.global ov80_0223DC22
ov80_0223DC22:
	.global hgasset_asm_overlay_80_0222BDF4_s_ov80_0223DC22
	.set hgasset_asm_overlay_80_0222BDF4_s_ov80_0223DC22, ov80_0223DC22 ; 0x0223DC22
	.byte 0x00, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
	.byte 0, 0x00, 0x00, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00, 0, 0x00
