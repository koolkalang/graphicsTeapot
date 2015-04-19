//version 330 core

//out vec3 texCoords;
varying vec3 ec_vnormal, ec_vposition, texCoords;

void main()
{
	ec_vnormal = gl_NormalMatrix*gl_Normal;
	ec_vposition = gl_ModelViewMatrix*gl_Vertex;
	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
	gl_TexCoord[0] = gl_Position;
    //gl_Position =   projection * view * vec4(position, 1.0);  
    texCoords = gl_Vertex;
}
