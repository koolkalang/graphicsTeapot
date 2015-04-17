#include "model.h"

model::model(){
	std::cout << "Default constructor called!!\n";
	vertNormals = NULL;
	texCoords = NULL;
	tangents = NULL;
	bitangents = NULL;
	vIndices = NULL;
	vNIndices = NULL;
	texIndices = NULL;
	vPerF = 4;
}

model::model(const std::string fileName, const int verticesPerF){
	//model();
	std::cout << "Conversion constructor called!!\n";
	vertNormals = NULL;
	texCoords = NULL;
	tangents = NULL;
	bitangents = NULL;
	vIndices = NULL;
	vNIndices = NULL;
	texIndices = NULL;
	sourceFile = fileName;
	vPerF = verticesPerF;
	parseObj(sourceFile);
}

model::~model(){
	std::cout << "Destructor called!!\n";
	free(vertices);
	free(vertNormals);
	free(texCoords);
	free(vIndices);
	free(vNIndices);
	free(texIndices);
	free(tangents);
	free(bitangents);
}

model& model::operator=(const model& rhs){
	sourceFile = rhs.sourceFile;
	vPerF = rhs.vPerF;	
	return *this;
}

model::model(const model&){
}

//Tri code
int model::parseObj(const std::string fileName){
	//slurp it all into local memory
	char * content = readFile(fileName);
	//now we read in line by line.  Since we do not know how many vertices / normals / faces there are, we read through once
	//to record the number of vertices and faces.  It would be interesting to see if this is faster than pushing onto C++ vectors.
	int vtCount = 0;	
	vCount = 0;
	fCount = 0;
	//content is immutable, so we must make a duplicate so strtok() can properly modify it into tokens
	char *contentDup = strdup(content);
	char *line = strtok(contentDup, "\n");
	while(line){
		//we must record the length of the line to properly increment through the buffer, since strtok() cannot increment
		//through two strings at once
		int linelength = strlen(line) + 1;
		char *word = strtok(line, " ");	

		if(!strcmp(word, "v"))		vCount++;
		else if(!strcmp(word,"vt"))	vtCount++;
		else if(!strcmp(word,"f"))	fCount++;

		line = strtok(line+linelength, "\n");	
	}
	free(contentDup);
	//Since this assignment requires a static picture, runtime is not a huge priority, so verts, normals, and tex coords are stored in separate pointer arrays
	//3 attributes per vertex, normal
	vertices = (GLfloat *)calloc(sizeof(GLfloat), 3*vCount);
	vertNormals = (GLfloat *)calloc(sizeof(GLfloat), 3*vCount);
	tangents = (GLfloat *)calloc(sizeof(GLfloat), 3*vCount);
	bitangents = (GLfloat *)calloc(sizeof(GLfloat), 3*vCount);
	//2 attributes per texture coordinate
	texCoords = (GLfloat *)calloc(sizeof(GLfloat), 2*vtCount);
	
	//3 * total vertices / face * faces for the vertex index, texture index, and normal index
	vIndices= (GLuint *)calloc(sizeof(GLuint), vPerF*fCount);
	vNIndices= (GLuint *)calloc(sizeof(GLuint), vPerF*fCount);
	texIndices= (GLuint *)calloc(sizeof(GLuint), vPerF*fCount);

	contentDup = strdup(content);	
	line = strtok(contentDup, "\n");	
	unsigned int vIndex = 0;
	unsigned int vNIndex = 0;
	unsigned int fIndex = 0;
	unsigned int vtIndex = 0;
	unsigned int vxIndex = 0;
	unsigned int vyIndex = 0;
	while(line){
		int lineLength = strlen(line) + 1;
		char *word = strtok(line, " ");	
		if(!strcmp(word, "v")){
			for(int j = 0; j < 3; vIndex++, j++) {
				word = strtok(NULL, " ");
				vertices[vIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vn")){
			for(int j = 0; j < 3; vNIndex++, j++) {
				word = strtok(NULL, " ");
				vertNormals[vNIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vx")){
			for(int j = 0; j < 3; vxIndex++, j++) {
				word = strtok(NULL, " ");
				tangents[vxIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vy")){
			for(int j = 0; j < 3; vyIndex++, j++) {
				word = strtok(NULL, " ");
				bitangents[vyIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vt")){
			for(int j = 0; j < 2; vtIndex++, j++) {
				word = strtok(NULL, " ");
				texCoords[vtIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"f")){
			for(int j = 0; j < vPerF; fIndex++, j++){

				//vertex index
				word = strtok(NULL, "/");
				vIndices[fIndex] = (((GLuint)atoi(word))-1);	
				/*if((vIndices[0] > 1000000)){
					printf("-----------------------------------------------\n");
					printf("containedVal:%i\n%i\n\n", vIndices[0], fIndex);
					printf("face count:%d\n", vPerF*fCount);
					exit(1);
				}*/
				//texture index
				word = strtok(NULL, "/");
				texIndices[fIndex] = (((GLuint)atoi(word))-1);
		//		printf("-----------------------------------------------\n");
		//		printf("word:%s\ncontainedVal:%i\nfIndex:%i\n", word, texIndices[fIndex], fIndex);
				//vertex normal index
				word = strtok(NULL, " /");
				vNIndices[fIndex] = (((GLuint)atoi(word))-1); 
		//		printf("-----------------------------------------------\n");
		//		printf("word:%s\ncontainedVal:%i\nfIndex:%i\n", word, vNIndices[fIndex], fIndex);
			}
		}		
		line = strtok(line+lineLength, "\n");	
	}
	free(contentDup);
	free(content);
	return 0;
}


void model::draw(){
	glBegin(GL_QUADS);
	int i = 0;
	int j = 0;
	int vertIndex, faceIndex, normIndex, texIndex;
	for(i = 0; i < fCount; i++){
		faceIndex = 4*i;
		for(j = 0; j < 4; j++){
			normIndex = 3*vNIndices[faceIndex + j];
			glNormal3f(vertNormals[normIndex],vertNormals[normIndex+1],vertNormals[normIndex+2]);
			texIndex = 2*texIndices[faceIndex+j];	
			glTexCoord2f(texCoords[texIndex], texCoords[texIndex+1]);
	//		if(i < 10)
	//			printf("texIndex:%d\ntexCoordu:%f\n", texIndex, texCoords[texIndex]);

			vertIndex = 3*vIndices[faceIndex + j];	
			glVertex3f(vertices[vertIndex],vertices[vertIndex+1],vertices[vertIndex+2]);
		}

	}
	glEnd();
}
