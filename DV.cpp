#include "stdafx.h"

#include "DV.h"

static int GetSSYBPack(BYTE *data, int len, int packNum, BYTE *pack)
{
    int seqCount = len >= 144000 ? 12 : 10;

    /* process all DIF sequences */

    for (int i = 0; i < seqCount; ++i) {

        /* there are two DIF blocks in the subcode section */

        for (int j = 0; j < 2; ++j) {

            /* each block has 6 packets */

            for (int k = 0; k < 6; ++k) {

                /* 150 DIF blocks per sequence, 80 bytes per DIF block,
				   subcode blocks start at block 1, 
				   block and packet have 3 bytes header, 
				   packet is 8 bytes long (including header) */

                const unsigned char *s = &data[i * 150 * 80 + 1 * 80 + j * 80 + 3 + k * 8 + 3];

                if (s[0] == packNum) {
                    pack[0] = s[0];
                    pack[1] = s[1];
                    pack[2] = s[2];
                    pack[3] = s[3];
                    pack[4] = s[4];
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

int GetDVRecordingTime(BYTE *buf, int len)
{
    BYTE pack62[5];
    BYTE pack63[5];

	if (len != 144000 && len != 120000) return -1;

    if (!GetSSYBPack(buf, len, 0x62, pack62))
        return -1;

    if (!GetSSYBPack(buf, len, 0x63, pack63))
        return -1;

    int day = pack62[2];
    int month = pack62[3];
    int year = pack62[4];

    int sec = pack63[2];
    int min = pack63[3];
    int hour = pack63[4];

    sec = (sec & 0xf) + 10 * ((sec >> 4) & 0x7);
    min = (min & 0xf) + 10 * ((min >> 4) & 0x7);
    hour = (hour & 0xf) + 10 * ((hour >> 4) & 0x3);
    year = (year & 0xf) + 10 * ((year >> 4) & 0xf);
    month = (month & 0xf) + 10 * ((month >> 4) & 0x1);
    day = (day & 0xf) + 10 * ((day >> 4) & 0x3);

    if (year < 50)
        year += 2000;
    else
        year += 1900;

	struct tm recDate;

    recDate.tm_sec = sec;
    recDate.tm_min = min;
    recDate.tm_hour = hour;
    recDate.tm_mday = day;
    recDate.tm_mon = month - 1;
    recDate.tm_year = year - 1900;
    recDate.tm_wday = -1;
    recDate.tm_yday = -1;
    recDate.tm_isdst = -1;

    return mktime(&recDate);
}
