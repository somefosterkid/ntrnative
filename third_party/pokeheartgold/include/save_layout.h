#ifndef POKEHEARTGOLD_SAVE_LAYOUT_H
#define POKEHEARTGOLD_SAVE_LAYOUT_H

// A struct that is part of the save image, or is read from a file, has to have the same size and field offsets on the DS
// (ILP32, mwcc) and in the PC build (LP64, clang): fixed-width fields, no pointers, no alignment surprises.

// mwcc aligns 64-bit integers to 4 bytes on the ARM9, a host compiler to 8: the host pads the struct that holds one
// (Pokemon 0xEC -> 0xF0) and shifts everything after it. Use these for 64-bit members of such structs.
#ifdef __MWERKS__
typedef u64 u64_a4;
typedef s64 s64_a4;
#else
typedef u64 u64_a4 __attribute__((aligned(4)));
typedef s64 s64_a4 __attribute__((aligned(4)));
#endif

// LAYOUT_CHECK(name, cond): a negative array size (compile error) when cond is false. `name` is unique per check.
#define LAYOUT_CHECK(name, cond) typedef char layout_check_##name[(cond) ? 1 : -1]

// LAYOUT_CHECK_OFFSET(name, type, member, offset): the same for a field offset. mwcc has no constant offsetof, so only
// the host compile checks it (the DS offsets are the truth; the sizes of the parts are checked on both).
#ifdef __MWERKS__
#define LAYOUT_CHECK_OFFSET(name, type, member, offset) typedef char layout_check_##name[1]
#else
#define LAYOUT_CHECK_OFFSET(name, type, member, offset) typedef char layout_check_##name[(offsetof(type, member) == (offset)) ? 1 : -1]
#endif

#endif // POKEHEARTGOLD_SAVE_LAYOUT_H
