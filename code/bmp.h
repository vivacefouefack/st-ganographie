#ifndef BMP_H
#define BMP_H
#include <stdint.h>


typedef struct {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offsetbits;
} __attribute__ ((packed)) bmpheader;

typedef struct {
    unsigned int headersize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int compression;
    unsigned int sizeimage;
    int xpelspermeter;
    int ypelspermeter;
    unsigned int colorsused;
    unsigned int colorsimportant;
    unsigned char * palette;
    unsigned char * pixels;
} __attribute__ ((packed)) bmpinfo;

#endif

