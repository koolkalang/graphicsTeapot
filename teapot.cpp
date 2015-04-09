#include "teapot.h"

OBJObject *Bunny;
const double JITTER = 0.007;
const int AA_PASSES = 20;
const int verticesPerFace = 4;

const point eye = {3,3,3};
const point view = {0,.8,0};
const point up = {0,1,0};
// Used to reference the different shaders.
GLuint teapotShader;
GLuint planeShader;

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
void Init(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, 1.78, 0.1, 20.0);

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



GLuint myBuffer = 1;
GLuint vertBuffer = 2;
GLuint normBuffer = 3;
GLuint indexBuffer = 4;

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
                glDeleteBuffers(1,&myBuffer);
                glDeleteBuffers(1,&vertBuffer);
                glDeleteBuffers(1,&normBuffer);
                glDeleteBuffers(1,&indexBuffer);
				free(Bunny->vertices);
				free(Bunny->vertNormals);
				free(Bunny->texCoords);
				free(Bunny->vIndices);
				free(Bunny->vNIndices);
				free(Bunny->texIndices);
				free(Bunny);
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

unsigned int set_shaders(char *fragFile, char *vertFile){

	char *vs, *fs;
	GLuint v, f, p;

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
	glUseProgram(p);
	return(p);
}

void draw_stuff(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//mode, count, type, starting pointer 
	glUseProgram(teapotShader);
	glBegin(GL_QUADS);

	int i = 0;
	int j = 0;
	int vertIndex, faceIndex, normIndex;
	for(i = 0; i < Bunny->fCount; i++){
		faceIndex = 4*i;
		for(j = 0; j < 4; j++){
			normIndex = 3*Bunny->vNIndices[faceIndex + j];
			glNormal3f(Bunny->vertNormals[normIndex],Bunny->vertNormals[normIndex+1],Bunny->vertNormals[normIndex+2]);
			vertIndex = 3*Bunny->vIndices[faceIndex + j];
			
			glVertex3f(Bunny->vertices[vertIndex],Bunny->vertices[vertIndex+1],Bunny->vertices[vertIndex+2]);
		}

	}
	glEnd();


	glUseProgram(planeShader);

	// Mirror Quad
	glBegin(GL_QUADS);

		glNormal3f(0,1,0);	

		glVertex3f(-3,0,-3);
		glVertex3f(-3,0,3);
		glVertex3f(3,0,3);
		glVertex3f(3,0,-3);
/*
		glVertex3f(-4.6919, 6.7599, -0.6532);	
		glVertex3f(1.6748, -3.3882, 0.6841);	
		glVertex3f(10.0216, -4.8014, 8.7911);	
		glVertex3f(13.0387, 5.3467, 7.4536);	
*/
		// scaled down vertices
	glEnd();

//	glDrawElements(GL_QUADS, Bunny->fCount*4, GL_UNSIGNED_INT, (void*)0);
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

int main(int argc, char **argv){

	Bunny = (OBJObject*)calloc(sizeof(OBJObject), 1);

	parseObj(argv[1], Bunny, verticesPerFace);

	//standard Init routine
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ACCUM);
	glutInitWindowSize(1280,720);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Test Window");

	//GLEW MUST be initialized AFTER creating the GLUT window- otherwise, will receive "missing GL version" error
	GLenum err = glewInit();
	if(GLEW_OK != err){
		printf("ERROR: %s\n", glewGetErrorString(err));
	}

	glClearColor(0.1, 0.1, 0.1, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	Init();
	create_lights();
	create_material();
	teapotShader = set_shaders((char *) "phongEC.frag",(char *) "phongEC.vert");
	planeShader = set_shaders((char *) "phongEC2.frag",(char *) "phongEC2.vert");

	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Bunny->vCount*3, Bunny->vertices, GL_STATIC_DRAW);
	
	//size, type, stride, starting pointer
	glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Bunny->vCount*3, Bunny->vertNormals, GL_STATIC_DRAW);
	//type, stride, starting pointer
	glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat),(GLvoid*)0);

//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//buffer binding target, size, data pointer, usage
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Bunny->fCount*verticesPerFace*3, Bunny->vIndices, GL_STATIC_DRAW);
	
/*	GLsizei stride = sizeof(GLuint)*Bunny->fCount*verticesPerFace;
	GLuint attribute;
	//making sure vertex indices work properly
	attribute = glGetAttribLocation(program, "VertexPosition");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, 0);

	//making sure vertex normal indices work properly
	attribute = glGetAttribLocation(program, "VertexNormal");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, 0);*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glutDisplayFunc(draw_AA);
//	glutDisplayFunc(draw_stuff);
	glutKeyboardFunc(keyBindings);
	glutMainLoop();
	return 0;
}
