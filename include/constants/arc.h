// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/constants/arc.h, the lines in use so far.
#ifndef BWPORT_CONSTANTS_ARC_H
#define BWPORT_CONSTANTS_ARC_H

// Names, layouts and constants from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#define ARCID_SYSTEM_MESSAGE 2
#define ARCID_SCRIPT_MESSAGE 3
#define ARCID_WINFRAME 5
#define ARCID_FONT 23

// Black's, one more than Black 2's 82 (yesno_menu.c's assembly)
// The graphics that the menus share (app_menu_common.c)
#define ARCID_APP_MENU_COMMON 83

// Black's, one more and one less than Black 2's 78 and 110 (field_nogrid_mapper.c)
#define ARCID_RAIL_HEADERS 79
#define ARCID_RAIL_DATA 109

// Black's, one more than the 84 pokebw2 has (townmap_grh.c's assembly): the town map's graphics
#define ARCID_TOWNMAP_GRA 85

// The interactive sound system's zone fades (iss_zone_sys.c). Our name, not swan's
#define ARCID_ISS_ZONE 136

// Game Sync's graphics (gsync_disp.c). Our name, not swan's
#define ARCID_GSYNC 143

// The Dream World account screens' graphics (pdwacc_disp.c). Our name, not swan's
#define ARCID_PDWACC 145

#define ARCID_EVOLUTIONS 19
// Black's
// The touch screen of no_gear.c (a/2/8/7): many palettes and screens that look like the C-Gear's. Our name and a
// guess, not swan's

#define ARCID_LEARNSETS 18

// The Pokémon icons
#define ARCID_POKEICON 7
// The party list's graphics
#define ARCID_POKELIST_GRA 76

// The boot logos and the title screen's 2D graphics
#define ARCID_TITLE 26

// Black 2's, not checked against Black: Black's 162 holds animations, and the code that shows the notice isn't found yet
// The copyright notice that the game shows when it starts. swan names this archive ARCID_FIELD_CAMERA_SCRIPT_PARAM,
// but it holds 2D graphics
// Black's (near twins)
#define ARCID_COPYRIGHT 164

// The Pokémon icons

#define ARCID_GROWTBL 17

// Black's, read from the ROM by tools/black_constants.py
#define ARCID_MMODEL_TBL 48
// The walking Pokémon's object codes (tpoke_data.c)
#define ARCID_TPOKE 211
#define ARCID_MUSICAL 47
// The interactive sound system's dungeon BGM settings (iss_dungeon_sys.c). Our name, not swan's
#define ARCID_ISS_DUNGEON 146
// The interactive sound system's BGM switch sets (iss_switch_sys.c). Our name, not swan's
#define ARCID_ISS_SWITCH 147
// The musical's graphics (not from swan): its props' textures, the photo screen's touch screen and the stage's
#define ARCID_MUSICAL_ITEM 45
// The Dowsing MCHN's graphics and map of the hidden items around the player. Our name, not swan's
#define ARCID_DOWSING 195
#define ARCID_LIGHTS_FIELD 61
#define ARCID_LIGHTS_BATTLE 62
#define ARCID_LIGHTS_ZONE 82
#define ARCID_PERSONAL 16
#define ARCID_AREADATA 13
// The graphics of the credits' last picture after the credits (the_end.c). Our name, not swan's
#define ARCID_THE_END 220
// The Pokédex's graphics
#define ARCID_ZUKAN_GRA 159
// The Pokédex's habitats, a file per species
#define ARCID_ZUKAN_AREA 178 // Black's
// The subway map's graphics on the main screen (subway_map.c). Our name, not swan's
#define ARCID_SUBWAY_MAP 206
// The touch screen of no_gear.c and c_gear.c (a/0/3/0): many palettes and screens. Our name, not swan's
#define ARCID_C_GEAR 30
#define ARCID_MUSICAL_SHOT 46
// The graphics of the compatibility result (irc_compat_result.c). Our name, not swan's
#define ARCID_IRC_COMPAT 94
// The Game Sync menu's graphics (gsync_menu.c). Our name, not swan's
#define ARCID_GSYNC_MENU 29

// The Pokémon icons

// Black's
// The interactive sound system's city sound units (iss_city_unit.c). Our name, not swan's
#define ARCID_ISS_CITY 0x84

// Black's, three more than Black 2's 190 (un_select.c's code)
// The Unity Tower's floor select's graphics (un_select.c). Our name, not swan's
#define ARCID_UN_SELECT 193
#define ARCID_CDEMO_GFLOGO 220
#define ARCID_CDEMO_OPENINGSW 222

// Black's, three more than Black 2's 198 (th_award.c's code)
// The Trial House results screen's graphics (th_award.c). Our name, not swan's
#define ARCID_TH_AWARD 201
// Black's, three more than Black 2's 199 (chihou_zukan_award.c's code)
// The Pokédex diplomas' graphics (chihou_zukan_award.c and zenkoku_zukan_award.c). Our name, not swan's
#define ARCID_ZUKAN_AWARD 202

// Black's
// The graphics of the Battle Subway's records screen (bsubway_records.c). Our name, not swan's
#define ARCID_BSUBWAY_RECORDS 199

// Black 2's: Black has nothing of this name (tools/black_constants.py)
// Unova Link's graphics (not from swan)
#define ARCID_KEY_SYSTEM 277
// Black's
// The lists of words that names may not contain, compressed and encrypted
#define ARCID_PROF_WORD 0xdd

// The demo's resource archive: Black's 160 (the 0xa0 that demo3d_data.s loads), not Black 2's 158
#define ARCID_DEMO3D_RESOURCE 160
#define ARCID_WAZAINFO 21

// Black's
// The Union Room's touch screen, union_subdisp.c. Our name, not swan's
#define ARCID_UNION_SUBDISP 89

// The intro's graphics
#define ARCID_INTRO 170

// The PC box's graphics
// Black's (near twins)
#define ARCID_BOX2 116

// The musical's Pokémon sprites, in the older layout of ARCID_POKEGRA (not from swan)
// Black's (near twins)
#define ARCID_MUSICAL_POKEGRA 52

// The summary screen's graphics
// Black's (near twins)
#define ARCID_P_STATUS 78

// Black's
// The Research Radar's graphics. Our name, not swan's
#define ARCID_RESEARCH_RADAR 0xc0

// The evolution demo's graphics
// Black's (near twins)
#define ARCID_SHINKA_DEMO 182

// The interactive sound system's 3D sound configs (iss_3ds_sys.c). Our name, not swan's
// Black's (near twins)
#define ARCID_ISS_3D_SOUND 185

// Black's
#define ARCID_ITEMINFO 24
#define ARCID_ITEMGRA 25

// The cells and animations of bmp_oam.c's 32x16 actors, for each OBJ character mapping
// Black's (near twins)
#define ARCID_BMP_OAM 51

// Black's (near twins)
#define ARCID_SEASON_BANNER 151

// Black's
// The trainers' messages as pairs of trainer ID and message type, in trainer order, and the offset of each trainer's
// first pair (tr_tool.c; not from swan)
#define ARCID_TRTBL 89

// Black's
#define ARCID_TRTBLOFS 90
#define ARCID_TRDATA 91

#define ARCID_TRPOKE 92

// The sentence input's graphics, with the icons that sentences show in place of some words
// Black's (near twins)
#define ARCID_PMSI 43

// Black's
// Mystery Gift's graphics
#define ARCID_MYSTERY 34

// Black's
#define ARCID_AREA_BMTEX_EXT 174
#define ARCID_AREA_BMTEX_INT 175
#define ARCID_AREA_BMDATA_EXT 225
#define ARCID_AREA_BMDATA_INT 226

// Black's
// Black's (near twins)
#define ARCID_GIMMICK_TBL 101

// The start menu's graphics
#define ARCID_STARTMENU 0x23

// Black's
#define ARCID_DATA_CONVERT 139

// Black's
#define ARCID_INFOWIN 0x32

// Black's, read from the ROM
#define ARCID_MANUAL 223
#define ARCID_MANUAL_IMAGE 231

// Black's
#define ARCID_EGG_DEMO 189

// Black's
#define ARCID_POKEGRA 4
#define ARCID_TRSPRITE_FRONT 0x48
#define ARCID_TRSPRITE_BACK 0x49

// Black's
// Black 2's, not checked against Black
// The graphics of the compatibility checker's ranking (irc_ranking.c). Our name, not swan's
#define ARCID_IRC_RANKING 77

// Black 2's, not checked against Black
#define ARCID_TRAI_SCRIPT 171 // Black's

// Black 2's, not checked against Black
// The Global Trade Station's 2D graphics (not from swan)
#define ARCID_WORLDTRADE 0x60

// Black's
#define ARCID_NAMEIN_GRAPHIC 118

#endif // BWPORT_CONSTANTS_ARC_H
