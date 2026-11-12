// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gdb.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_GDB_H
#define BWPORT_DWC_GDB_H

#include "types.h"
#include "dwc/sake.h"
#include "dwc/dwc.h"

// A binary field's value: the data and its size in bytes
typedef struct GdbBinaryData {
    void *data;
    u32 size;
} GdbBinaryData;

// A field of a record, to write and as read: SakeField's first fields. Its types are SakeFieldType's
typedef struct GdbField {
    const char *name;
    u32 type;
    union {
        s32 intValue;
        u8 byteValue;
        GdbBinaryData binaryData;
    } value;
} GdbField;

#define GDB_FIELD_BYTE SAKE_FIELD_BYTE
#define GDB_FIELD_INT SAKE_FIELD_INT
#define GDB_FIELD_BINARY SAKE_FIELD_BINARY_DATA

// Called when the library has the records that a request asked for: with their number, each an array of the fields the
// request was given, and the work. The library passes the first three; a function may declare a fourth that it does not
// get, as the callers' do
typedef void (*GdbRecordsFunc)(int recordNum, GdbField **records, void *work, int fieldNum);

// The states of the library, as Gdb_GetState gives them
typedef enum GdbState {
    GDB_STATE_OFF = 0,
    GDB_STATE_IDLE = 1,
    GDB_STATE_BUSY = 2,
    GDB_STATE_CANCELING = 3,
} GdbState;

// pokebw2's Gdb_Startup
// Starts the library for a game with the player's user data and a kind of server (see Sc_Startup), returning 0 or an
// error code. It needs the Wi-Fi library to be running and the player to have a login ticket
s32 func_ov175_021e10cc(s32 gameId, DWCUserData *userData, s32 server);
// pokebw2's Gdb_Cancel
// Cancels the request that is running, returning 0 or 0x11 when there is none
u32 func_ov175_021e11f0(void);
// pokebw2's Gdb_Shutdown
// Ends the library, returning 0 or an error code
u32 func_ov175_021e1340(void);
// pokebw2's Gdb_Update
// Runs the library for a frame
void func_ov175_021e1414(void);
// pokebw2's Gdb_GetState
// The state of the library, or 4 when the Wi-Fi library has an error
u32 func_ov175_021e1434(void);
// pokebw2's Gdb_GetResult
// The result of the last request: 1 once it worked, 0 while it runs, or an error code
u32 func_ov175_021e1454(void);
// pokebw2's Gdb_CreateRecord, Gdb_UpdateRecord, Gdb_GetMyRecords and Gdb_GetSpecificRecords
// The requests, returning 0 or an error code: create a record in a table, giving back its ID; update a record; get the
// records of the player; and get specific records by their IDs
u32 func_ov175_021e1464(const char *table, GdbField *fields, int fieldNum, s32 *recordId);
u32 func_ov175_021e1540(const char *table, s32 recordId, GdbField *fields, int fieldNum);
u32 func_ov175_021e1618(const char *table, const char **fieldNames, int fieldNum, GdbRecordsFunc func, void *work);
u32 func_ov175_021e16f0(const char *table, s32 *recordIds, int recordNum, const char **fieldNames, int fieldNum,
                        GdbRecordsFunc func, void *work);

#endif // BWPORT_DWC_GDB_H
