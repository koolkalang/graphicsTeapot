#include<math.h>
#include<string>
#include<stdio.h>
#include<fcntl.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>

typedef struct{
	GLfloat x, y, z;
}point;

//generates pseudorandom numbers
double genrand();

//returns cross product of vectors u x v
point cross(point u, point v);

//accepts vector u, returns unit length vector v
point unit_length(point u);

//prints out a 4x4 matrix.  Useful for understanding operations done on the transform stack
void printMatrix(GLfloat m[16]);

//reads in a file in a single system call
char *readFile(const std::string fileName);
