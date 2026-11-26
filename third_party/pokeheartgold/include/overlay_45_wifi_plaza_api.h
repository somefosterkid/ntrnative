#ifndef POKEHEARTGOLD_OVERLAY_45_WIFI_PLAZA_API_H
#define POKEHEARTGOLD_OVERLAY_45_WIFI_PLAZA_API_H

#include "global.h"

#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);
BOOL ov45_0222A374(const WiFiPlazaSystem *param0);
BOOL ov45_0222A394(const WiFiPlazaSystem *param0);
void ov45_0222A404(WiFiPlazaSystem *param0);
void ov45_0222A414(WiFiPlazaSystem *param0);
BOOL ov45_0222A424(const WiFiPlazaSystem *param0);
void ov45_0222A43C(WiFiPlazaSystem *param0);
void ov45_0222A498(const WiFiPlazaSystem *param0, UnkStruct_ov66_0222E294 *param1);
void ov45_0222A4B8(WiFiPlazaSystem *param0);
const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);
WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);
void ov45_0222A5E8(WiFiPlazaSystem *param0, int param1);
PlayerProfile *ov45_0222AB1C(const WiFiPlazaSystem *param0);
void IncrementGameStat47(WiFiPlazaSystem *param0);
void IncrementGameStat119(WiFiPlazaSystem *param0);
u32 ov45_0222B1B4(const WiFiPlazaSystem *param0);
BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222A53C(const WiFiPlazaSystem *param0);
s32 ov45_0222A99C(const WiFiPlazaProfile *profile);
s32 ov45_0222A9A4(const WiFiPlazaProfile *profile);

#endif
