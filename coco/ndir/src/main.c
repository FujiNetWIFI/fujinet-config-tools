/**
 * @brief   Network Directory
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Show directory of given N: endpoint
 */

#include <cmoc.h>
#include <coco.h>
#include "net.h"

#define SUCCESS 1
#define ERROR_EOF 136

#define NET_DEV 0
#define MODE_DIRECTORY 6
#define TRANS_SHORT 0
#define TRANS_LONG 128

char url[256];
byte buf[512];

int line_counter=0;

void ndir_display(void)
{
    char *p = strtok((char *)buf,"\r");

    while (p)
    {
        printf("%s\r",p);

        line_counter++;

        if (line_counter > 14)
        {
            printf("ANY KEY -> NEXT PAGE ");
            waitkey(1);
            line_counter=0;
            printf("\r");
        }
        
        p = strtok(NULL,"\r");        
    }    
}

int ndir(const char *s, unsigned char m)
{
    NetworkStatus ns;
    
    if (net_open(NET_DEV,MODE_DIRECTORY,m,s) != SUCCESS)
    {
        int e = net_error(NET_DEV);
        printf("OPEN ERROR: #%u\n",e);
        return e;
    }

    net_status(0,&ns);

    while (net_error(0) != ERROR_EOF)
    {
        ns.bytesWaiting = (ns.bytesWaiting > (unsigned int)512 ? (unsigned int)512 : ns.bytesWaiting);

        if (!ns.bytesWaiting)
            break;
        
        if (net_read(0,buf,ns.bytesWaiting) != SUCCESS)
        {
            int e = net_error(NET_DEV);
            printf("READ ERROR: #%u\n",e);
            net_close(NET_DEV);
            return e;
        }

        ndir_display();

        net_status(NET_DEV,&ns);
    }

    net_close(NET_DEV);
    
    return 0;
}

int main(void)
{
    char *s = NULL;
    unsigned char m=TRANS_SHORT;
    
    printf("NET DIRECTORY--ENTER N: URL?\n");
    setCaseFlag(0);
    s=readline();

    if (!strlen(s))
        return 1;

    // Copy s because next readline will clobber it.
    strcpy(url,s);
    
    printf("MODE--RETURN=SHORT, L=LONG?\n");
    setCaseFlag(1);
    s=readline();

    if (s[0]=='L')
        m=(unsigned char)TRANS_LONG;
    
    return ndir(url,m);
}
