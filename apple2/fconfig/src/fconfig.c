/**
 * @brief   Show FujiNet Config
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main Program
 */

#include <stdio.h>
#include <stdint.h>
#include <fujinet-fuji.h>
#include <cc65.h>
#include <apple2.h>

const char *banner_fmt="%-39s\n";
const char *field_fmt="%18s%-21s\n";

AdapterConfigExtended ace;

void main(void)
{
    if (get_ostype() >= APPLE_IIIEM)
    {
        allow_lowercase(true);
    }

    if (!fuji_get_adapter_config_extended(&ace))
    {
        printf("Could not read adapter config.\n");
        return;
    }

    printf(banner_fmt,"FujiNet Network Configuration");
    printf(banner_fmt,"-----------------------------");
    printf(field_fmt,"Hostname:",ace.hostname);
    printf(field_fmt,"SSID:",ace.ssid);
    printf(field_fmt,"Local IP:",ace.sLocalIP);
    printf(field_fmt,"Gateway:",ace.sGateway);
    printf(field_fmt,"Netmask:",ace.sNetmask);
    printf(field_fmt,"DNS IP:",ace.sDnsIP);
    printf(field_fmt,"MAC:",ace.sMacAddress);
    printf(field_fmt,"BSSID:",ace.sBssid);
    printf(field_fmt,"FujiNet version:",ace.fn_version);

    if (doesclrscrafterexit())
    {
        printf("\nPress any key to continue. ");
        getchar();
    }
}
