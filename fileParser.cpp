#include "fileParser.h"
//read in the whole file in one system call, parse line by line after
char *readFile(char* fileName){

	//Dr. Geist's file IO code almost verbatim	
	FILE *filePointer;
	char *content = NULL;
	int fd, counter;
	fd = open(fileName, O_RDONLY);
	counter = lseek(fd, 0, SEEK_END);
	close(fd);
	content = (char *)calloc(1,(counter+1));
	filePointer = fopen(fileName, "r");
	//SLURP
	counter = fread(content,sizeof(char),counter,filePointer);
	content[counter] = '\0';
	fclose(filePointer);
	return content;
}

//Tri code
int parseObj(char *fileName, OBJObject *container, int vPerF){
	//slurp it all into local memory
	char *content = readFile(fileName);
	//now we read in line by line.  Since we do not know how many vertices / normals / faces there are, we read through once
	//to record the number of vertices and faces.  It would be interesting to see if this is faster than pushing onto C++ vectors.
	int vtCount = 0;	
	container->vCount = 0;
	container->fCount = 0;
	//content is immutable, so we must make a duplicate so strtok() can properly modify it into tokens
	char *contentDup = strdup(content);
	char *line = strtok(contentDup, "\n");
	while(line){
		//we must record the length of the line to properly increment through the buffer, since strtok() cannot increment
		//through two strings at once
		int linelength = strlen(line) + 1;
		char *word = strtok(line, " ");	

		if(!strcmp(word, "v"))		container->vCount++;
		else if(!strcmp(word,"vt"))	vtCount++;
		else if(!strcmp(word,"f"))	container->fCount++;

		line = strtok(line+linelength, "\n");	
	}
	printf("%i\n", vtCount);
	printf("%i\n", container->vCount);
	free(contentDup);
	//Since this assignment requires a static picture, runtime is not a huge priority, so verts, normals, and tex coords are stored in separate pointer arrays
	//3 attributes per vertex, normal
	container->vertices = (GLfloat *)calloc(sizeof(GLfloat), 3*container->vCount);
	container->vertNormals = (GLfloat *)calloc(sizeof(GLfloat), 3*container->vCount);
	container->tangents = (GLfloat *)calloc(sizeof(GLfloat), 3*container->vCount);
	container->bitangents = (GLfloat *)calloc(sizeof(GLfloat), 3*container->vCount);
	//2 attributes per texture coordinate
	container->texCoords = (GLfloat *)calloc(sizeof(GLfloat), 2*vtCount);
	
	//3 * total vertices / face * faces for the vertex index, texture index, and normal index
	container->vIndices= (GLuint *)calloc(sizeof(GLuint), vPerF*container->fCount);
	container->vNIndices= (GLuint *)calloc(sizeof(GLuint), vPerF*container->fCount);
	container->texIndices= (GLuint *)calloc(sizeof(GLuint), vPerF*container->fCount);

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
				container->vertices[vIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vn")){
			for(int j = 0; j < 3; vNIndex++, j++) {
				word = strtok(NULL, " ");
				container->vertNormals[vNIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vx")){
			for(int j = 0; j < 3; vxIndex++, j++) {
				word = strtok(NULL, " ");
				container->tangents[vxIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vy")){
			for(int j = 0; j < 3; vyIndex++, j++) {
				word = strtok(NULL, " ");
				container->bitangents[vyIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"vt")){
			for(int j = 0; j < 2; vtIndex++, j++) {
				word = strtok(NULL, " ");
				container->texCoords[vtIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"f")){
			for(int j = 0; j < vPerF; fIndex++, j++){

				//vertex index
				word = strtok(NULL, "/");
				container->vIndices[fIndex] = (((GLuint)atoi(word))-1);	
		/*		printf("word:%s\ncontainedVal:%i\nfIndex:%i\n", word, container->vIndices[0], fIndex);
				if((container->vIndices[0] > 1000000)){
					printf("-----------------------------------------------\n");
					printf("containedVal:%i\n%i\n\n", container->vIndices[0], fIndex);
					printf("face count:%d\n", vPerF*container->fCount);
					exit(1);
				}*/
				//texture index
				word = strtok(NULL, "/");
				container->texIndices[fIndex] = (((GLuint)atoi(word))-1);
				//vertex normal index
				word = strtok(NULL, " /");
				container->vNIndices[fIndex] = (((GLuint)atoi(word))-1); 
			}
		}		
		line = strtok(line+lineLength, "\n");	
	}
	free(contentDup);
	free(content);
	return 0;
}
