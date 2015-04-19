varying vec3 ec_vnormal, ec_vposition, texCoords;


void main(){
	ec_vnormal = gl_NormalMatrix*gl_Normal;
	ec_vposition = gl_ModelViewMatrix*gl_Vertex;
	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	texCoords = gl_Vertex;
}
