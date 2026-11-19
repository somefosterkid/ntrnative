#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/trainer_class.h"
	.include "macros.inc"
	.include "unk_020517A4.inc"
	.include "global.inc"

	.public NPCTrainerGetBattleIntroAndMusicParam
	.public WildPokemonGetBattleIntroAndMusicParam
	.public BattleSetup_GetTransitionAndMusicParam
	.text

	.rodata

	.global _020FC3B4
_020FC3B4:
	.short SPECIES_RAIKOU | (22 << 10)
	.short SPECIES_ENTEI | (23 << 10)
	.short SPECIES_SUICUNE | (24 << 10)
	.short SPECIES_LUGIA | (26 << 10)
	.short SPECIES_HO_OH | (25 << 10)
	.short SPECIES_GROUDON | (27 << 10)
	.short SPECIES_KYOGRE | (27 << 10)
	.short SPECIES_RAYQUAZA | (27 << 10)
	.short SPECIES_MEWTWO | (28 << 10)
	.short SPECIES_LATIOS | (28 << 10)
	.short SPECIES_LATIAS | (28 << 10)
	.global _020FC3CA
_020FC3CA:
	.short TRAINERCLASS_LEADER_FALKNER | (0 << 10)
	.short TRAINERCLASS_LEADER_BUGSY | (1 << 10)
	.short TRAINERCLASS_LEADER_WHITNEY | (2 << 10)
	.short TRAINERCLASS_LEADER_MORTY | (3 << 10)
	.short TRAINERCLASS_LEADER_JASMINE | (4 << 10)
	.short TRAINERCLASS_LEADER_CHUCK | (5 << 10)
	.short TRAINERCLASS_LEADER_PRYCE | (6 << 10)
	.short TRAINERCLASS_LEADER_CLAIR | (7 << 10)
	.short TRAINERCLASS_LEADER_BROCK | (8 << 10)
	.short TRAINERCLASS_LEADER_MISTY | (9 << 10)
	.short TRAINERCLASS_LEADER_LT_SURGE | (10 << 10)
	.short TRAINERCLASS_LEADER_ERIKA | (11 << 10)
	.short TRAINERCLASS_LEADER_JANINE | (12 << 10)
	.short TRAINERCLASS_LEADER_SABRINA | (13 << 10)
	.short TRAINERCLASS_LEADER_BLAINE | (14 << 10)
	.short TRAINERCLASS_LEADER_BLUE | (15 << 10)
	.short TRAINERCLASS_ELITE_FOUR_WILL | (16 << 10)
	.short TRAINERCLASS_ELITE_FOUR_KOGA | (17 << 10)
	.short TRAINERCLASS_ELITE_FOUR_BRUNO | (18 << 10)
	.short TRAINERCLASS_ELITE_FOUR_KAREN | (19 << 10)
	.short TRAINERCLASS_CHAMPION | (20 << 10)
	.short TRAINERCLASS_RIVAL | (21 << 10)
	.short TRAINERCLASS_PASSERBY | (21 << 10)
	.short TRAINERCLASS_EXECUTIVE_ARCHER | (32 << 10)
	.short TRAINERCLASS_EXECUTIVE_ARIANA | (33 << 10)
	.short TRAINERCLASS_EXECUTIVE_PROTON | (31 << 10)
	.short TRAINERCLASS_EXECUTIVE_PETREL | (30 << 10)
	.short TRAINERCLASS_ROCKET_BOSS | (34 << 10)
	.short TRAINERCLASS_TEAM_ROCKET | (29 << 10)
	.short TRAINERCLASS_TEAM_ROCKET_F | (29 << 10)
	.short TRAINERCLASS_KIMONO_GIRL | (43 << 10)
	.short TRAINERCLASS_PKMN_TRAINER_RED | (44 << 10)
	.global _020FC40A
_020FC40A:
	.short 12, SEQ_GS_VS_GYMREADER         ; 00
	.short 13, SEQ_GS_VS_GYMREADER         ; 01
	.short 14, SEQ_GS_VS_GYMREADER         ; 02
	.short 15, SEQ_GS_VS_GYMREADER         ; 03
	.short 16, SEQ_GS_VS_GYMREADER         ; 04
	.short 17, SEQ_GS_VS_GYMREADER         ; 05
	.short 18, SEQ_GS_VS_GYMREADER         ; 06
	.short 19, SEQ_GS_VS_GYMREADER         ; 07
	.short 20, SEQ_GS_VS_GYMREADER_KANTO   ; 08
	.short 21, SEQ_GS_VS_GYMREADER_KANTO   ; 09
	.short 22, SEQ_GS_VS_GYMREADER_KANTO   ; 10
	.short 23, SEQ_GS_VS_GYMREADER_KANTO   ; 11
	.short 24, SEQ_GS_VS_GYMREADER_KANTO   ; 12
	.short 25, SEQ_GS_VS_GYMREADER_KANTO   ; 13
	.short 26, SEQ_GS_VS_GYMREADER_KANTO   ; 14
	.short 27, SEQ_GS_VS_GYMREADER_KANTO   ; 15
	.short 29, SEQ_GS_VS_GYMREADER         ; 16
	.short 30, SEQ_GS_VS_GYMREADER         ; 17
	.short 31, SEQ_GS_VS_GYMREADER         ; 18
	.short 32, SEQ_GS_VS_GYMREADER         ; 19
	.short 33, SEQ_GS_VS_CHAMP             ; 20
	.short 28, SEQ_GS_VS_RIVAL             ; 21
	.short 0xFFFF, SEQ_GS_VS_RAIKOU        ; 22
	.short 0xFFFF, SEQ_GS_VS_ENTEI         ; 23
	.short 0xFFFF, SEQ_GS_VS_SUICUNE       ; 24
	.short 35, SEQ_GS_VS_HOUOU             ; 25
	.short 36, SEQ_GS_VS_LUGIA             ; 26
	.short 34, SEQ_GS_VS_KODAI             ; 27
	.short 34, SEQ_GS_VS_NORAPOKE_KANTO    ; 28
	.short 39, SEQ_GS_VS_ROCKET            ; 29
	.short 40, SEQ_GS_VS_ROCKET            ; 30
	.short 41, SEQ_GS_VS_ROCKET            ; 31
	.short 42, SEQ_GS_VS_ROCKET            ; 32
	.short 43, SEQ_GS_VS_ROCKET            ; 33
	.short 44, SEQ_GS_VS_ROCKET            ; 34
	.short 37, SEQ_GS_VS_TRAINER           ; 35
	.short 37, SEQ_GS_VS_TRAINER           ; 36
	.short 38, SEQ_GS_VS_TRAINER           ; 37
	.short 38, SEQ_GS_VS_NORAPOKE          ; 38
	.short 37, SEQ_GS_BA_BRAIN             ; 39
	.short 38, SEQ_GS_VS_GYMREADER         ; 40
	.short 0xFFFF, SEQ_GS_VS_TRAINER       ; 41
	.short 0xFFFF, SEQ_GS_VS_NORAPOKE      ; 42
	.short 45, SEQ_GS_VS_TRAINER           ; 43
	.short 46, SEQ_GS_VS_CHAMP             ; 44
