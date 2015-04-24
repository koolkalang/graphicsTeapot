#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>
typedef struct
{
    unsigned char imageTypeCode;
    short int imageWidth;
    short int imageHeight;
    unsigned char bitCount;
    unsigned char *imageData;
} TGAFILE;

bool LoadTGAFile(const char *filename, TGAFILE *tgaFile);
