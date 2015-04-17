#ifndef MODEL_H 
#define MODEL_H

#include<stdlib.h>
#include<string.h>
#include<string>
#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include"util.h"

class model {
public:
	//default constructor
	model();
	//conversion constructor
	model(const std::string fileName, const int verticesPerF);
	//copy constructor
	model(const model&);

	//assignment operator
	model& operator=(const model&);
	~model();

	int parseObj(const std::string fileName);

	void draw();
	const GLfloat *getVerts()		{ return vertices;}
	const GLfloat *getNorms()		{ return vertNormals;}
	const GLfloat *getTexCoords()	{ return texCoords;}
	const GLfloat *getTangents()	{ return tangents; }
	const GLfloat *getBitangents()	{ return bitangents;}

private:
	std::string sourceFile;
	GLfloat *vertices;
	GLfloat *vertNormals;
	GLfloat *texCoords;
	GLfloat *tangents;
	GLfloat *bitangents;

	//Indices for verts and normals stored in vertices
	//tangents and bitangents are in 1:1 correspondence with vertices, so they don't need their own indices
	GLuint *vIndices;
	GLuint *vNIndices;
	GLuint *texIndices;
	int vCount;
	int fCount;
	int vtCount;
	//vertices per faces
	int vPerF;

};

#endif
