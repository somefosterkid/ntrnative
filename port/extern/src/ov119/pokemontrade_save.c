// The PC build compiles src/ov119/pokemontrade_save.c through this file (tools/port_sources.py: a file under
// port/extern stands in for the game file of the same path; its object keeps the game file's overlay). The game's C
// defines these with plain `inline` and no extern declaration: under C99 (gnu99) that is an inline definition only,
// which emits no symbol, so a call the compiler doesn't inline (every call at -O0) links to an abort stub
// (tools/port_undefined.py). An extern declaration in the same file makes the definition external (C99 6.7.4p7).
// The DS build never sees this file.
#include "../../../../src/ov119/pokemontrade_save.c"

extern __typeof__(BackupMail) BackupMail;
extern __typeof__(BackupRecords) BackupRecords;
