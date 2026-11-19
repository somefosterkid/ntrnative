#ifndef POKEHEARTGOLD_OVERLAY_99_POKEATHLON_COURSE_SUMMARY_H
#define POKEHEARTGOLD_OVERLAY_99_POKEATHLON_COURSE_SUMMARY_H

// PokeathlonCourseSummary: the 0x1C record ov99_021E6FD0 builds (Heap_Alloc(heapID, 0x1C), stored in Obj99.f0) and the
// getters in overlay_99_021E7060.c read; fields at every accessed offset (hk63, from the asm of both).

struct PlayerProfile;

typedef struct PokeathlonCourseSummary {
    struct PlayerProfile *profile; // 0x00
    u16 unk_04;                    // sum of unk_0C[]
    u16 unk_06;
    u16 unk_08;                    // unk_0A + unk_04 + unk_06
    u16 unk_0A;
    u16 unk_0C[5];                 // one per course record (save record + 6, stride 0x2C)
    u8 unk_16;
    u8 unk_17;
    u8 unk_18;
    u8 pad_19[0x3];
} PokeathlonCourseSummary; // size 0x1C

#endif // POKEHEARTGOLD_OVERLAY_99_POKEATHLON_COURSE_SUMMARY_H
