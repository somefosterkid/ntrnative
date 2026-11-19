#include "global.h"

#include "heap.h"
#include "overlay_106_legendary_cinematic.h"
#include "overlay_manager.h"
#include "save.h"
#include "unk_02097B78.h"

struct AsyncWriteManager;

struct OverlayManager;

struct OverlayManagerTemplate;

struct SaveArrayHeader;

struct SaveData;

struct SaveSlotSpec;

typedef BOOL (*LegendaryCinematicFunc)(LegendaryCinematic *work);

BOOL ScriptCinematic_HoOh(LegendaryCinematic *work);

BOOL ScriptCinematic_Lugia(LegendaryCinematic *work);

BOOL ScriptCinematic_Arceus(LegendaryCinematic *work);

extern const LegendaryCinematicFunc _02108F38[3];

BOOL LegendaryCinematic_Main(OverlayManager *man, int *state);

BOOL LegendaryCinematic_Main(OverlayManager *man, int *state) {
    LegendaryCinematic *data = OverlayManager_GetData(man);
    if (_02108F38[data->args->unk4C](data) == 0) {
        return TRUE;
    }
    return FALSE;
}
