// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/fld_faceup.h. pokebw2's
// func_ov155_021f59e0, func_ov155_021f5cd0, func_ov155_021f5cf8 and func_ov155_021f5d0c are Black's
// func_ov082_021f5260, func_ov082_021f5550, func_ov082_021f5578 and func_ov082_021f558c.
#ifndef BWPORT_FIELD_FLD_FACEUP_H
#define BWPORT_FIELD_FLD_FACEUP_H

#include "types.h"
#include "struct_decls.h"

// The face-up, overlay 82 (fld_faceup.c): a close-up of a character's face over the field on BGs 2 and 3, whose eyes
// blink and whose mouth moves while the script's message prints. The scenes with N play it through scrcmd_ndemo.c

// Fades in the face of type 0 to 2 and returns the event that does it, or NULL if a face is already up. With
// endBlack, the end leaves the screen black
GameEvent *func_ov082_021f5260(u8 type, u8 unused, u16 endBlack, GameSystem *gsys, FieldScriptEnv *env);
// The event that fades the face out and frees it, or NULL if there is none
GameEvent *func_ov082_021f5550(GameSystem *gsys);
// Frees the face at once
void func_ov082_021f5578(Field *field);
// Lets the mouth move with the next message
void func_ov082_021f558c(Field *field);

#endif // BWPORT_FIELD_FLD_FACEUP_H
