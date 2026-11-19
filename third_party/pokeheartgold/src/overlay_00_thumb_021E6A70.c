#include "global.h"
#include "dwc_error.h"

int ov00_021E6A70(int errorCode, int errorType);

int ov00_021E6A70(int errorCode, int errorType) {
    int errorCodeDiv100 = errorCode / 100;
    int errorCodeDiv1000 = errorCode / 1000;

    if (errorCode == 20101) {
        return 1;
    }

    if (errorCodeDiv1000 == 23) {
        return 1;
    }

    if (errorCode == 20108) {
        return 2;
    }

    if (errorCode == 20110) {
        return 3;
    }

    if (errorCodeDiv100 == 512) {
        return 4;
    }

    if (errorCodeDiv100 == 500) {
        return 5;
    }

    if (errorCode == 51103) {
        return 7;
    }

    if (errorCodeDiv100 == 510) {
        return 6;
    }

    if (errorCodeDiv100 == 511) {
        return 6;
    }

    if (errorCodeDiv100 == 513) {
        return 6;
    }

    if (errorCode >= 52000 && errorCode <= 52003) {
        return 8;
    }

    if (errorCode >= 52100 && errorCode <= 52103) {
        return 8;
    }

    if (errorCode >= 52200 && errorCode <= 52203) {
        return 8;
    }

    if (errorCode == 80430) {
        return 9;
    }

    if (errorCodeDiv1000 == 20) {
        return 0;
    }

    if (errorCodeDiv100 == 520) {
        return 0;
    }

    if (errorCodeDiv100 == 521) {
        return 0;
    }

    if (errorCodeDiv100 == 522) {
        return 0;
    }

    if (errorCodeDiv100 == 523) {
        return 0;
    }

    if (errorCodeDiv100 == 530) {
        return 0;
    }

    if (errorCodeDiv100 == 531) {
        return 0;
    }

    if (errorCodeDiv100 == 532) {
        return 0;
    }

    if (errorCode < 10000) {
        return 14;
    }

    if (errorCodeDiv1000 == 31) {
        return 12;
    }

    switch (errorType) {
    case DWC_ETYPE_NO_ERROR:
    case DWC_ETYPE_LIGHT:
    case DWC_ETYPE_SHOW_ERROR:
        return 11;
    case DWC_ETYPE_SHUTDOWN_FM:
    case DWC_ETYPE_SHUTDOWN_GHTTP:
    case DWC_ETYPE_SHUTDOWN_ND:
    case DWC_ETYPE_DISCONNECT:
        return 10;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    case DWC_ETYPE_FATAL:
        return 15;
    }

    return -1;
}
