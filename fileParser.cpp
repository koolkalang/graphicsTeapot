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

int parseObj(char *fileName, OBJObject *container){
	//slurp it all into local memory
	char *content = readFile(fileName);
	//now we read in line by line.  Since we do not know how many vertices / normals / faces there are, we read through once
	//to record the number of vertices and faces.  It would be interesting to see if this is faster than pushing onto C++ vectors.
	
	//content is immutable, so we must make a duplicate so strtok() can properly modify it into tokens
	char *contentDup = strdup(content);
	char *line = strtok(contentDup, "\n");
	while(line){
		//we must record the length of the line to properly increment through the buffer, since strtok() cannot increment
		//through two strings at once
		int linelength = strlen(line) + 1;
		char *word = strtok(line, " ");	

		if(!strcmp(word, "v"))		container->vCount++;
		else if(!strcmp(word,"f"))	container->fCount++;

		line = strtok(line+linelength, "\n");	
	}
	free(contentDup);
	//allocating space for 3*x*vertices, because each vertex has 3 attributes + 3 normal attributes
	container->vertices = (GLfloat *)calloc(sizeof(GLfloat), 3*2*container->vCount);
	container->vIndices= (GLuint *)calloc(sizeof(GLuint), 3*2*container->fCount);

	contentDup = strdup(content);	
	line = strtok(contentDup, "\n");	
	int vIndex = 0;
	int vNIndex = 0;
	int fIndex = 0;
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
				//store all of the normals in the second half of the array
				container->vertices[container->vCount*3 + vNIndex] = (GLfloat)atof(word);
			}
		}
		else if(!strcmp(word,"f")){
			for(int j = 0; j < 3; fIndex++, j++){
				word = strtok(NULL, " /");
				container->vIndices[fIndex] = (((GLuint)atoi(word))-1);	
				//texture index storage would occur here
				word = strtok(NULL, " /");
				container->vIndices[container->fCount*3 + fIndex] = (((GLuint)atoi(word))-1);
			}
		}		
		line = strtok(line+lineLength, "\n");	
	}
	free(contentDup);
	free(content);
	return 0;
}
