#include "global.h"

#include "unk_02034354.h"
#include "message_format.h"

struct MessageFormat;
void BufferCountryName(MessageFormat *template, u32 idx, u32 country);
void BufferCityName(MessageFormat *template, u32 idx, u32 country, u32 city);
u16 sub_0203769C(void);

int sub_0205A6AC(MessageFormat *strTemplate);

extern const int _020FC99C[7][2];
int sub_0205A730(int activityIndex, int gender, MessageFormat *strTemplate);

int sub_0205A6AC(MessageFormat *strTemplate) {
    u8 playerCountry = sub_020348A8(sub_0203769C());
    u8 commCountry = sub_020348A8(sub_0203769C() ^ 1);
    u8 playerRegion = sub_020348CC(sub_0203769C());
    u8 commRegion = sub_020348CC(sub_0203769C() ^ 1);

    if (commCountry == 0) {
        return 15;
    }

    if (commCountry != 0) {
        BufferCountryName(strTemplate, 3, commCountry);

        if (commRegion != 0) {
            BufferCityName(strTemplate, 4, commCountry, commRegion);
        }
    }

    if (playerCountry != commCountry) {
        if (commRegion == 0) {
            return 13;
        }

        if (playerRegion == commRegion) {
            return 13;
        }

        return 12;
    }

    if (playerRegion != commRegion) {
        return 14;
    }

    return 15;
}

int sub_0205A730(int activityIndex, int gender, MessageFormat *strTemplate) {
    if (activityIndex != 0) {
        return _020FC99C[activityIndex][gender];
    }

    return sub_0205A6AC(strTemplate);
}
