#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6B10(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F6B10(UnkStruct_ov14_boxapp *arg0) {
    GridInputHandler *inputHandler;

    inputHandler = arg0->sys->unk_2C;
    if (inputHandler != NULL) {
        GridInputHandler_Free(inputHandler);
        arg0->sys->unk_2C = NULL;
    }
}
