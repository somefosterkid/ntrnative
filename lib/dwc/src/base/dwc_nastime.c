#include "dwc/dwc.h"
#include "types.h"
#include "string.h"
#include "nitro/os.h"
#include "errno.h"
#include "stdlib.h"
// Not in NitroDWC's dwc_nasfunc.c, which only declares the three time variables extern,
// so they and the code that sets them from the Date header of a NAS reply are another source file of Black's.

// what DWC_GetDateTime (dwc_nasfunc.c) reads
s64 DWCnastimediff = 0;
s64 DWCnastimediffbase = 0;
BOOL DWCnastimediffvalid = FALSE;
static const char *data_ov009_021a1fa8[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

BOOL func_ov009_02168250(char *param_1) {
    RTCDate date;
    RTCTime time;
    char buf[32];
    s64 sec;
    s64 sec2;
    int i;

    if (strlen(param_1) != strlen("Fri, 03 Mar 2006 01:28:13 GMT") || param_1[7] != ' ' || param_1[11] != ' ' || param_1[16] != ' ' || param_1[19] != ':' || param_1[22] != ':' || param_1[25] != ' ') {
        goto error;
    }
    if (RTC_GetDateTime(&date, &time) != 0) {
        goto error;
    }
    sec = RTC_ConvertDateTimeToSecond(&date, &time);
    if (sec == -1) {
        goto error;
    }
    strcpy(buf, param_1);
    buf[7] = 0;
    buf[11] = 0;
    buf[16] = 0;
    buf[19] = 0;
    buf[22] = 0;
    buf[25] = 0;
    errno = 0;
    date.year = atol(&buf[12]);
    if (errno == ERANGE) {
        goto error;
    }
    date.year -= 2000;
    date.month = 13;
    for (i = 0; i < 12; i++) {
        if (strcmp(data_ov009_021a1fa8[i], &buf[8]) == 0) {
            date.month = i + 1;
            break;
        }
    }
    if (date.month > 12) {
        goto error;
    }
    errno = 0;
    date.day = atol(&buf[5]);
    if (errno == ERANGE) {
        goto error;
    }
    errno = 0;
    time.hour = atol(&buf[17]);
    if (errno == ERANGE) {
        goto error;
    }
    errno = 0;
    time.minute = atol(&buf[20]);
    if (errno == ERANGE) {
        goto error;
    }
    errno = 0;
    time.second = atol(&buf[23]);
    if (errno == ERANGE) {
        goto error;
    }
    sec2 = RTC_ConvertDateTimeToSecond(&date, &time);
    if (sec2 == -1) {
        goto error;
    }
    DWCnastimediff = sec - sec2;
    DWCnastimediffbase = sec;
    DWCnastimediffvalid = TRUE;
    return TRUE;

error:
    return FALSE;
}
