// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/dpc_bmp.h: the lines in use so far.
#ifndef BWPORT_APP_DPC_BMP_H
#define BWPORT_APP_DPC_BMP_H

#include "types.h"
#include "app/dpc_main.h"

// pokebw2's DpcBmp_Init
void func_ov171_021dd874(DendouPCWork *wk);
// pokebw2's DpcBmp_Exit
void func_ov171_021dd8bc(DendouPCWork *wk);
// pokebw2's DpcBmp_PrintUpdate
// Runs the print queue and sends the windows it has finished
void func_ov171_021dd8dc(DendouPCWork *wk);
// pokebw2's DpcBmp_PutTitle
// The page's title and date, and its number out of the pages
void func_ov171_021dd964(DendouPCWork *wk);
// pokebw2's DpcBmp_PutPage
void func_ov171_021ddaf0(DendouPCWork *wk);
// pokebw2's DpcBmp_PutPokeInfo
// The data of the Pokémon at the front of the ring
void func_ov171_021ddc28(DendouPCWork *wk);
// pokebw2's DpcBmp_ClearPokeInfo
void func_ov171_021ddfbc(DendouPCWork *wk);

#endif // BWPORT_APP_DPC_BMP_H
