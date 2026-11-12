// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/sc.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_SC_H
#define BWPORT_DWC_SC_H

#include "types.h"
#include "dwc/auth.h"
#include "dwc/competition.h"

// Called with the error (0 if none) and the user data when a request has ended
typedef void (*ScResultFunc)(int error, void *userData);

// pokebw2's Sc_Startup
// Starts the library for a game: sets up the servers of a kind (1 for the second kind of secure servers), logs in
// with the token that the Wi-Fi library has and makes the competition client, returning 0 or 7 on failure
int func_ov175_021dee3c(int gameId, int server);
// pokebw2's Sc_GetCertificate
// Gives the player's certificate
int func_ov175_021df490(const AuthCertificate **certificate);
// pokebw2's Sc_CreateSession
// The requests: to make a session, to tell that the player will report on it (authoritatively or not), and to submit
// a report. Each returns 0 or an error code and calls a function with the error when it has ended
int func_ov175_021df018(ScResultFunc func, int timeout, void *userData);
// pokebw2's Sc_GetSessionId
char *func_ov175_021df20c(void);
// pokebw2's Sc_Update
// Runs the client for a frame
int func_ov175_021defdc(void);
// pokebw2's Sc_GetConnectionId
char *func_ov175_021df224(void);
// pokebw2's Sc_SetSessionId
// Takes the ID of the session from the other player, and gives the IDs of the session and of the connection
int func_ov175_021df1c8(const char *csid);
// pokebw2's Sc_SetReportIntention
int func_ov175_021df09c(int authoritative, ScResultFunc func, int timeout, void *userData);
// pokebw2's Sc_SubmitReport
int func_ov175_021df130(ScReport *report, int authoritative, ScResultFunc func, int timeout, void *userData);
// pokebw2's Sc_FreeReport
int func_ov175_021df458(ScReport *report);
// pokebw2's Sc_Shutdown
// Ends the client and the library's work, returning 0 or the client's error
int func_ov175_021def58(void);
// pokebw2's Sc_AddByte
int func_ov175_021df420(ScReport *report, u8 key, u8 value);
// pokebw2's Sc_AddInt
int func_ov175_021df3e8(ScReport *report, u8 key, int value);

#endif // BWPORT_DWC_SC_H
