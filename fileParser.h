#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>
typedef struct {
	//vertices will contain the vertex values as well as the normals
	GLfloat *vertices;
	//Indices for verts and normals stored in vertices
	GLuint *vIndices;
	//total # of vertices and faces	
	int vCount, fCount;
}OBJObject;

//reads entire file in on one system call
char *readFile(char* fileName);
//parses the buffer created by readFile
int parseObj(char *fileName, OBJObject *container);

