#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>

#define NULL_A (unsigned char(*)(unsigned char *))0

typedef struct {
	char *name;
	GLuint texID;
	GLint format;
	unsigned char (*alpha)(unsigned char *);
	GLenum texunit;
	GLint combine;
}textureUnit;

void load_textures(textureUnit *texture);
