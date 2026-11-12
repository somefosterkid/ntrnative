// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/event_pc.c, ported by tools/port_file.py.
#include "types.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_g3d_mapper.h"
#include "field/field_prop.h"
#include "field/pc_sound.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "system/game_system.h"

GameEventReturnCode func_ov033_021798a0(GameEvent *event, u32 *state, void *data);

GameEventReturnCode pcEntrySound(GameEvent *event, u32 *state, void *eventData) {
    struct PCSoundEventData *data = eventData;
    VecFx32 position;
    FieldPropAreaBounds bounds;
    FieldPropSystem *propSystem;
    FieldChunkPropHolder *prop;

    switch (*state) {
    case 0:
        GFL_SndSEPlay(0x55b);
        FieldPlayer_GetWPos(Field_GetPlayer(data->field), &position);
        bounds.minZ = position.z - (1 << 16);
        bounds.maxZ = position.z + (1 << 16);
        bounds.minX = position.x - (1 << 16);
        bounds.maxX = position.x + (1 << 16);
        propSystem = FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(data->field));
        prop = FieldPropSystem_FindProp(propSystem, 4, &bounds);
        if (prop != NULL) {
            data->pcProp = prop;
            FieldChunkPropHolder_CallAnmCmd(propSystem, prop, 0, 0);
        }
        (*state)++;
        break;
    case 1:
        if (GFL_SndPlayerIsActive(GFL_SndSeqGetPlayerIndex(0x55b))) {
            break;
        }
        (*state)++;
        break;
    case 2:
        GFL_SndSEPlay(0x55c);
        if (data->pcProp != NULL) {
            FieldChunkPropHolder_CallAnmCmd(FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(data->field)), data->pcProp,
                                            1, 2);
        }
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *CreatePCSoundCallEvent(GameEvent *parent, GameSystem *gsys, Field *field) {
    GameEvent *event = GameEvent_Create(gsys, parent, pcEntrySound, sizeof(struct PCSoundEventData));
    struct PCSoundEventData *data = GameEvent_GetData(event);

    data->gameSystem = gsys;
    data->field = field;
    data->pcProp = NULL;
    return event;
}

GameEventReturnCode func_ov021_021c97b8(GameEvent *event, u32 *state, void *eventData) {
    struct PCSoundEventData *data;
    VecFx32 position;
    FieldPropAreaBounds bounds;
    FieldPropSystem *propSystem;
    FieldChunkPropHolder *prop;

    data = eventData;
    switch (*state) {
    case 0:
        FieldPlayer_GetWPos(Field_GetPlayer(data->field), &position);
        bounds.minZ = position.z - (1 << 16);
        bounds.maxZ = position.z + (1 << 16);
        bounds.minX = position.x - (1 << 16);
        bounds.maxX = position.x + (1 << 16);
        propSystem = FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(data->field));
        prop = FieldPropSystem_FindProp(propSystem, 4, &bounds);
        if (prop != NULL) {
            data->pcProp = prop;
            FieldChunkPropHolder_CallAnmCmd(propSystem, prop, 1, 2);
        }
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov021_021c9828(GameEvent *parent, GameSystem *gsys, Field *field) {
    GameEvent *event;
    struct PCSoundEventData *data;

    event = GameEvent_Create(gsys, parent, func_ov021_021c97b8, sizeof(struct PCSoundEventData));
    data = GameEvent_GetData(event);
    data->gameSystem = gsys;
    data->field = field;
    data->pcProp = NULL;
    return event;
}

GameEventReturnCode pcLogOffSound(GameEvent *event, u32 *state, void *eventData) {
    struct PCSoundEventData *data = eventData;
    VecFx32 position;
    FieldPropAreaBounds bounds;
    FieldPropSystem *propSystem;
    FieldChunkPropHolder *prop;
    u32 currentState = *state;

    switch (currentState) {
    case 0:
        if (data->skipSound == 0) {
            GFL_SndSEPlay(0x55d);
        }
        FieldPlayer_GetWPos(Field_GetPlayer(data->field), &position);
        bounds.minZ = position.z - (1 << 16);
        bounds.maxZ = position.z + (1 << 16);
        bounds.minX = position.x - (1 << 16);
        bounds.maxX = position.x + (1 << 16);
        propSystem = FieldG3DMapper_GetBMSystem(Field_GetG3DMapper(data->field));
        prop = FieldPropSystem_FindProp(propSystem, 4, &bounds);
        if (prop != NULL) {
            data->pcProp = prop;
            FieldChunkPropHolder_CallAnmCmd(propSystem, prop, 2, 0);
        }
        (*state)++;
        break;
    case 1:
        if (data->skipSound == 0) {
            if (GFL_SndPlayerIsActive(GFL_SndSeqGetPlayerIndex(0x55d))) {
                break;
            }
            (*state)++;
        } else {
            *state = currentState + 1;
        }
        break;
    case 2:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEvent *func_ov021_021c98f4(GameEvent *parent, GameSystem *gsys, Field *field, u32 skipSound) {
    GameEvent *event;
    struct PCSoundEventData *data;

    event = GameEvent_Create(gsys, parent, pcLogOffSound, sizeof(struct PCSoundEventData));
    data = GameEvent_GetData(event);
    data->gameSystem = gsys;
    data->field = field;
    data->pcProp = NULL;
    data->skipSound = skipSound;
    return event;
}
