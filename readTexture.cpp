#include "readTexture.h"

//Code taken from Dr. Geists' multi-texturing example
void load_textures(textureUnit *texture){
	FILE *fptr;
	char buf[512];
	int imageSize, imageWidth, imageHeight, maxColor;
	unsigned char *textureBytes;
	char *parse;
	unsigned char *tb_alpha, *tb_src, *tb_dst, *tb_final, *aarg;
	int i;
	textureUnit *texturePtr;
	texturePtr = texture;

	//Build textures first

	while(texturePtr->name){
		//load ppm file
		fptr = fopen(texturePtr->name, "r");
		//Ignore the comments
		fgets(buf, 512, fptr);
		do{
			fgets(buf, 512, fptr);
		}
		while(buf[0] =='#');
		
		//read in the header information
		parse = strtok(buf, " ");
		imageWidth = atoi(parse);
		parse = strtok(buf, " ");
		imageHeight = atoi(parse);		
		fgets(buf, 512, fptr);
		parse = strtok(buf, " ");
		maxColor = atoi(parse);

		//allocate local space for the image, slurp it in
		imageSize = imageWidth*imageHeight;
		textureBytes = (unsigned char *)calloc(4, imageSize);
		fread(textureBytes, 1, 3*imageSize, fptr);

		if(texturePtr->format == GL_RGBA){
			tb_alpha = (unsigned char *)calloc(4, imageSize);
			//RGBA original (source)
			tb_src = textureBytes;
			//RGBA destination
			tb_dst = tb_alpha;
			for(i = 0; i < imageSize; i++){
				//Here use the per pixel alpha function to load new alpha components
				aarg = tb_src;
				*tb_dst++ = *tb_src++;//R
				*tb_dst++ = *tb_src++;//G
				*tb_dst++ = *tb_src++;//B	
				*tb_dst++ = (texturePtr->alpha)(aarg);//A
			}
			free(textureBytes);
			tb_final = tb_alpha;
		}
		else tb_final = textureBytes;
		glBindTexture(GL_TEXTURE_2D, texturePtr->texID);
		glTexImage2D(GL_TEXTURE_2D, 0, texturePtr->format, imageWidth, imageHeight, 0, texturePtr->format, GL_UNSIGNED_BYTE, tb_final);
		free(tb_final);
		texturePtr++;
	printf("texture read in properly");
	}

	//Now activate all the texture units;
	texturePtr = texture;
	while(texturePtr->name){
		glActiveTexture(texturePtr->texunit);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texturePtr->texID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texturePtr->combine);
		texturePtr++;
	}
	GLenum glErr = glGetError();
	if(glErr != GL_NO_ERROR){
		printf("glError: %s\n",
			     gluErrorString(glErr));
	}
}
