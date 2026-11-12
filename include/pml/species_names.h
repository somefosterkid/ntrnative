// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/species_names.h, the lines in use so
// far.
#ifndef BWPORT_PML_SPECIES_NAMES_H
#define BWPORT_PML_SPECIES_NAMES_H

#include "types.h"
#include "struct_decls.h"

// The species names' message data (pml_species_names.c), loaded once by PML_SpeciesNamesResidentInit
extern MsgData *g_PMLSpeciesNamesResident;

// Loads the species names' message data for good (pokebw2's PML_SpeciesNamesResidentInit)
void func_0201f344(HeapID heapId);

#endif // BWPORT_PML_SPECIES_NAMES_H
