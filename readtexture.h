#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>

typedef struct textureUnit{
	char *name;
	int texID;
	GLint format;
	unsigned char (*alpha)(unsigned char *);
	GLenum texunit;
	GLint combine;
}
