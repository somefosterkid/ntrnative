// The port's own (no twin in pokebw2): the fixed work areas, a record's bytes that the game casts to the structs of
// whatever runs in them (an actor's move code, a field effect task, a battle procedure).
#ifndef BWPORT_WORK_AREA_H
#define BWPORT_WORK_AREA_H

// The size of an area that a work with pointers outgrows on a PC target: Black's size where pointers are 4 bytes (MWCC,
// the tools' clang for ARM), twice that on a PC target (the condition types.h has), where they are 8 bytes, as a
// struct of 4-byte fields and pointers is at most twice its ARM size
#if !defined(__MWERKS__) && !defined(__INTELLISENSE__) && !defined(__arm__)
#define WORK_AREA_SIZE(armSize) ((armSize) * 2)
#else
#define WORK_AREA_SIZE(armSize) (armSize)
#endif

// Next to a work with pointers that runs in a fixed area: fails the build (an array of size -1) where the work is bigger
// than the area. A typedef, so it makes no code or data
#define WORK_AREA_CHECK(type, areaSize) typedef char type##_FitsWorkArea[(sizeof(type) <= (areaSize)) ? 1 : -1]

#endif // BWPORT_WORK_AREA_H
