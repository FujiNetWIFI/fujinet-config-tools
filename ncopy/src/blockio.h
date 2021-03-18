/**
 * Simple CIO GETCHR/PUTCHR wrappers
 */

#ifndef BLOCKIO_H
#define BLOCKIO_H

#define D_DEVICE_DATA 2
#define D_DEVICE_DIRECTORY 3

#define IOCB_READ 4
#define IOCB_DIRECTORY 6
#define IOCB_WRITE 8

unsigned char open(unsigned char channel, unsigned char aux1, char* buf, unsigned short len);
unsigned char close(unsigned char channel);
unsigned char get(unsigned char channel, char* buf, unsigned short len);
unsigned char getrec(unsigned char channel, char* buf, unsigned short len);
unsigned char put(unsigned char channel, char* buf, unsigned short len);

#endif /* BLOCKIO_H */
