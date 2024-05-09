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

const char *banner_fmt="%-39s\n";
const char *field_fmt="%18s%-21s\n";

char *s;

AdapterConfigExtended ace;

void main(void)
{
    if (!fuji_get_adapter_config_extended(&ace))
    {
        printf("COULD NOT READ ADAPTER CONFIG.\n");
        return;
    }

    printf(banner_fmt,"FUJINET NETWORK CONFIGURATION");
    printf(banner_fmt,"-----------------------------");
    printf(field_fmt,"HOSTNAME:",ace.hostname);
    printf(field_fmt,"SSID:",ace.ssid);
    printf(field_fmt,"LOCAL IP:",ace.sLocalIP);
    printf(field_fmt,"GATEWAY:",ace.sGateway);
    printf(field_fmt,"NETMASK:",ace.sNetmask);
    printf(field_fmt,"DNS IP:",ace.sDnsIP);
    printf(field_fmt,"MAC:",ace.sMacAddress);
    printf(field_fmt,"BSSID:",ace.sBssid);
    printf(field_fmt,"FUJINET VERSION:",ace.fn_version);

    if (doesclrscrafterexit())
    {
        printf("\nPRESS ANY KEY TO CONTINUE. ");
        getchar();
    }
}
