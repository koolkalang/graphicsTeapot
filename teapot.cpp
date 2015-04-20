#include "teapot.h"
#include "loadTGA.h"
#include <vector>
#include <string>
#include <iostream>

model* primaryOBJ;
textureUnit *teapotTex;
GLuint cubeTexId;
GLuint skyboxVAO, skyboxVBO;
GLuint defaultShader, skyboxShader;

const double JITTER = 0.007;
const int AA_PASSES = 20;
const int verticesPerFace = 4;

const point eye = {3,2,3};
const point view = {0,.8,0};
const point up = {0,1,0};

GLfloat skyboxVertices[] = {
    // Positions          
    -10.0f,  10.0f, -10.0f,
    -10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,

    -10.0f, -10.0f,  10.0f,
    -10.0f, -10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f,  10.0f,
    -10.0f, -10.0f,  10.0f,

     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,

    -10.0f, -10.0f,  10.0f,
    -10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f, -10.0f,  10.0f,
    -10.0f, -10.0f,  10.0f,

    -10.0f,  10.0f, -10.0f,
     10.0f,  10.0f, -10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
    -10.0f,  10.0f,  10.0f,
    -10.0f,  10.0f, -10.0f,

    -10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f,  10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f,  10.0f,
     10.0f, -10.0f,  10.0f
};

point jitter_view()
{
	//The AA example in class used gluLookAt, which would require me to recalculate the eye, view, and up directions to maintain orientation.  I can get a similar
	//jittering effect just by translating the ModelView matrix, which doesnt reset it.  
	point viewJitter;
	viewJitter.x = -JITTER + 2.0*JITTER*genrand();
	viewJitter.y = -JITTER + 2.0*JITTER*genrand();
	viewJitter.z = -JITTER + 2.0*JITTER*genrand();
	glTranslatef(viewJitter.x, viewJitter.y, viewJitter.z);
	return viewJitter;
}
//Init sets up the view volume by placing the eye position and setting up the projection and modelview matrices
void initViewport(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.78, 0.1, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z,
			view.x, view.y, view.z,
			up.x, up.y, up.z);
}

void create_lights(){
	//KEY LIGHT
	float KEY_ambient[] =  { 0.0, 0.0, 0.0, 0.0 };
	float KEY_diffuse[] =  { 1.0, 1.0, 1.0, 0.0 };
	float KEY_specular[] = { 1.0, 1.0, 1.0, 0.0 };
	float KEY_position[] = { 1.0, 2.0, 4.0, 1.0};
	float KEY_direction[]= {-1.0, -1.0, -3.0, 1.0};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,KEY_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glLightfv(GL_LIGHT0, GL_AMBIENT, KEY_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, KEY_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, KEY_specular);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT0, GL_POSITION, KEY_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, KEY_direction);

	float FILL_ambient[] =  { 0.0, 0.0, 0.0, 0.0 };
	float FILL_diffuse[] =  { 0.6, 0.6, 0.6, 0.0 };
	float FILL_specular[] = { 0.6, 0.6, 0.6, 0.0 };
	float FILL_position[]=  { 2.0, 1.0, .5, 1.0};
	float FILL_direction[]= {-2.0, -.5, -1.5, 1.0};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,FILL_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, FILL_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, FILL_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, FILL_specular);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);	
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.7);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT1, GL_POSITION, FILL_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, FILL_direction);

	float RIM_ambient[] =  { 0.0, 0.0, 0.0, 0.0 };
	float RIM_diffuse[] =  { 0.8, .7, 0.6, 0.0 };
	float RIM_specular[] = { .8, .7, .6, 0.0 };
	float RIM_position[]=  { -4.0, 3, -4.0, 1.0};
	float RIM_direction[]= {4.0, -1.5, 4.0, 1.0};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,RIM_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, RIM_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, RIM_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, RIM_specular);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.5);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);	
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.7);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT2, GL_POSITION, RIM_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, RIM_direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

void create_material(){
	//CHOCOLATE MATERIAL!!!
	float mat_ambient[] = {0.0,0.0,0.0,1.0};
	float mat_diffuse[] = {0.25, 0.10,0.05,1.0};
	float mat_specular[] = {1.0,1.0,1.0,1.0};
	float mat_shininess[] = {10.0};

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void rotate(int angle, int x, int y, int z){
	glTranslatef(.1,0,.1);
	glRotatef(angle, x, y, z);
	glTranslatef( -.1, 0, -.1);
	glutPostRedisplay();
}

void keyBindings(unsigned char key, int x, int y)
{
switch(key) {
        case 'q':               
				delete primaryOBJ;
                exit(1);
				break;
		//rotate left
		case 'd':
				rotate(5, 0, 1, 0);
				break;
		case 'a':
				rotate(-5, 0, 1, 0);
				break;
		case 'w':
				rotate(5, 1, 0, 0);
				break;
		case 's':
				rotate(-5, 1, 0, 0);
				break;
		case 'r':
				rotate(5, 0, 0, 1);
				break;
		case 'f':
				rotate(-5, 0, 0, 1);
				break;
        default:
                break;
    }
}

unsigned int set_shaders(char* filePrefix){

	char *vs, *fs;
	GLuint v, f, p;

	char *vertFile = (char*)calloc(sizeof(char), 24); 
	char *fragFile =  (char*)calloc(sizeof(char), 24); 
	strcat(vertFile, filePrefix);
	strcat(fragFile, filePrefix);
	strcat(vertFile, (char*)".vert");
	strcat(fragFile, (char*)".frag");
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = readFile(vertFile);
	fs = readFile(fragFile);
	glShaderSource(v, 1, (const char **)&vs, NULL);
	glShaderSource(f, 1, (const char **)&fs, NULL);
	free(vs);
	free(fs);
	glCompileShader(v);
	glCompileShader(f);
	p = glCreateProgram();
	glAttachShader(p, f);
	glAttachShader(p, v);
	glLinkProgram(p);
	//------------------------------------------------------------
	glUseProgram(p);
	//------------------------------------------------------------
	free(vertFile);
	free(fragFile);
	return(p);
}

void draw_stuff(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Ugly hardcoded draw routine for the cube map
	glDepthMask(GL_FALSE);
	glUseProgram(skyboxShader);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glUniform1i(glGetUniformLocation(defaultShader, "skybox"), 0);
	glUniform1i(glGetUniformLocation(defaultShader, "teapotTex"), 1);
	glUseProgram(defaultShader);
//	glUniform1i(glGetUniformLocation(defaultShader, "skybox"), 10);
	primaryOBJ->draw();
	//glutSwapBuffers() commented out while Anti Aliasing is active - must be put back in if AA is turned off
	//	glutSwapBuffers();
}

//AntiAliasing routine
void draw_AA(){

	int view_pass;
	
	point reverseJitter;
	glClear(GL_ACCUM_BUFFER_BIT);
	for(view_pass=0; view_pass<AA_PASSES; view_pass++){
		//Jitter translates the modelview by a tiny amount. ReverseJitter will be used to translate the modelview back to its original position
		//after the jittered image is recorded in the accumulation buffer
		reverseJitter = jitter_view();
		draw_stuff();	
		glAccum(GL_ACCUM,1.0/(float)(AA_PASSES));
		glTranslatef(-reverseJitter.x,-reverseJitter.y,-reverseJitter.z);
	}
	glAccum(GL_RETURN,1.0);
	glutSwapBuffers();
}

void initCubeMap(){
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glActiveTexture(GL_TEXTURE0);
	std::vector<const GLchar*> faces;
	faces.push_back("textures/fat-chance-in-hell_ft.tga");
	faces.push_back("textures/fat-chance-in-hell_bk.tga");
	faces.push_back("textures/fat-chance-in-hell_dn.tga");
	faces.push_back("textures/fat-chance-in-hell_up.tga");
	faces.push_back("textures/fat-chance-in-hell_rt.tga");
	faces.push_back("textures/fat-chance-in-hell_lf.tga");
/*	faces.push_back("textures/mp_ss/ss_ft.tga");
	faces.push_back("textures/mp_ss/ss_bk.tga");
	faces.push_back("textures/mp_ss/ss_dn.tga");
	faces.push_back("textures/mp_ss/ss_up.tga");
	faces.push_back("textures/mp_ss/ss_rt.tga");
	faces.push_back("textures/mp_ss/ss_lf.tga");*/

	glGenTextures(1, &cubeTexId);
	//glActiveTexture(GL_TEXTURE0);
	TGAFILE *tgaFile = (TGAFILE*)calloc(sizeof(TGAFILE), 1);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexId);
	for(unsigned int i = 0; i < faces.size(); i++){

		LoadTGAFile(faces[i], tgaFile);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, tgaFile->imageWidth, tgaFile->imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaFile->imageData);
		free(tgaFile->imageData);

	}
	free(tgaFile);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexId);
}

void initSkyboxGeometry(){
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void initTeapotTexture(char *fileName){
	teapotTex = (textureUnit*)calloc(sizeof(textureUnit), 1);
	teapotTex->name = fileName;
	glGenTextures(1, &teapotTex->texID);
	teapotTex->format = GL_RGB;
	teapotTex->alpha = NULL_A;
	teapotTex->texunit = GL_TEXTURE1;
	teapotTex->combine = GL_MODULATE;
	load_textures(teapotTex);
}

//standard Init routine
void setupGlut(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ACCUM);
	glutInitWindowSize(1920,1080);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Teapot Contest");
	//GLEW MUST be initialized AFTER creating the GLUT window- otherwise, will receive "missing GL version" error
	GLenum err = glewInit();
	if(GLEW_OK != err){
		printf("ERROR: %s\n", glewGetErrorString(err));
	}
	glClearColor(0.1, 0.1, 0.1, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv){

	std::string fileName(argv[1]);
	primaryOBJ = new model(fileName, verticesPerFace);

	//setup glut window
	setupGlut(argc, argv);

	//setup the projection and view matrices
	initViewport();

	//initialize the cube map textures that will form our skybox
	initCubeMap();

	//standard light initialization.  Relatively unchanged from the Bunny project
	create_lights();

	//standard material initialization.  Relatively unchanged from the Bunny project
	create_material();

	//This will read in the texture that will be applied to the teapot, if a texture is applied to the teapot at all
	initTeapotTexture((char*)"textures/bubble_color.ppm");

	//setting up the main shader for the teapot
	defaultShader = set_shaders((char*)"phongEC");

	//setting up the shader for the skybox / cube map
	skyboxShader = set_shaders((char*)"skybox");

	//set up the skybox geometry cube
	initSkyboxGeometry();

	//Main loop functions
	glutDisplayFunc(draw_AA);
	glutKeyboardFunc(keyBindings);
	glutMainLoop();

	return 0;
}
