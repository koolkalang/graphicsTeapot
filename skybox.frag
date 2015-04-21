//in vec3 TexCoords;
//out vec4 color;
vec4 tColor;
uniform samplerCube skybox;
varying vec3 ec_vnormal, ec_vposition, texCoords;

void main()
{    
	//inverting the y axis, since opengl does stuff upside down
	texCoords[1] = texCoords[1]*-1;
	tColor = texture(skybox, texCoords);
	gl_FragColor = tColor; 
}
