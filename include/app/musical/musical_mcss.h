// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/musical_mcss.h: the lines in use
// so far.
#ifndef BWPORT_APP_MUSICAL_MUSICAL_MCSS_H
#define BWPORT_APP_MUSICAL_MUSICAL_MCSS_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of pos, offset and cellRotation in Black
// Where a marker cell of a sprite was drawn this frame, as a draw callback gets it
typedef struct {
    // The sprite's position, y up
    VecFx32 pos;
    // The cell's offset from it, scaled, and its depth
    VecFx32 offset;
    VecFx32 scale;
    // The sprite's center of rotation
    VecFx32 center;
    u16 rotation;
    // The cell's rotation, its own plus the multi-cell's
    u16 cellRotation;
} MusicalMcssCellInfo;

// Black 2's, not checked against Black
// Called for each marker cell of a sprite as it is drawn, with the cell's kind and the work given to
// MusicalMcss_Add
typedef void (*MusicalMcssCellCallback)(u32 kind, MusicalMcssCellInfo *info, void *work);

// pokebw2's MusicalMcss_InitSystem
MusicalMcssSys *func_ov112_021f55f8(u32 count, HeapID heapId);
// pokebw2's MusicalMcss_SetOrthoMode
// Draws the sprites with an orthographic projection of their own
void func_ov112_021f621c(MusicalMcssSys *sys);
// pokebw2's MusicalMcss_TermSystem
void func_ov112_021f5644(MusicalMcssSys *sys);
// pokebw2's MusicalMcss_UpdateSystem
void func_ov112_021f5678(MusicalMcssSys *sys);
// pokebw2's MusicalMcss_Add
MusicalMcss *func_ov112_021f611c(MusicalMcssSys *sys, fx32 x, fx32 y, fx32 z, MCSSLoadInfo *info, void *work,
                             BOOL loadAtVBlank);
// pokebw2's MusicalMcss_Hide
void func_ov112_021f62dc(MusicalMcss *mcss);
// pokebw2's MusicalMcss_StopAnime
void func_ov112_021f6290(MusicalMcss *mcss);
// pokebw2's MusicalMcss_Del
void func_ov112_021f61d8(MusicalMcssSys *sys, MusicalMcss *mcss);
// pokebw2's MusicalMcss_SetPosition
void func_ov112_021f6234(MusicalMcss *mcss, VecFx32 *pos);
// pokebw2's MusicalMcss_SetScale
void func_ov112_021f624c(MusicalMcss *mcss, VecFx32 *scale);
// pokebw2's MusicalMcss_SetRotation
void func_ov112_021f6264(MusicalMcss *mcss, u16 rotation);
// pokebw2's MusicalMcss_Show
void func_ov112_021f62ec(MusicalMcss *mcss);
// pokebw2's MusicalMcss_IsHidden
BOOL func_ov112_021f62d0(MusicalMcss *mcss);
// pokebw2's MusicalMcss_StartAnime
void func_ov112_021f62a0(MusicalMcss *mcss);
// pokebw2's MusicalMcss_ChangeAnime
void func_ov112_021f62b0(MusicalMcss *mcss, u16 anime);
// pokebw2's MusicalMcss_CopyState
// Gives dst src's position, scale, flags and the rest of its placing
void func_ov112_021f62fc(MusicalMcss *src, MusicalMcss *dst);
// pokebw2's MusicalMcss_SetFlip
void func_ov112_021f626c(MusicalMcss *mcss);
// pokebw2's MusicalMcss_ResetFlip
void func_ov112_021f6280(MusicalMcss *mcss);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MusicalMcss_DrawSystem
void func_ov112_021f56bc(MusicalMcssSys *sys, MusicalMcssCellCallback callback);
// pokebw2's MusicalMcss_SetTexBase
// Where the sprites' characters and palettes go in texture VRAM
void func_ov112_021f622c(MusicalMcssSys *sys, u32 base);
// pokebw2's MusicalMcss_SetPlttBase
void func_ov112_021f6230(MusicalMcssSys *sys, u32 base);

#endif // BWPORT_APP_MUSICAL_MUSICAL_MCSS_H
