
varying vec3 ec_vnormal, ec_vposition;
uniform sampler2D teapotTex;
uniform samplerCube skybox;

const int TOTAL_LIGHTS = 3;
const double PI = 3.1415926535;
const float diffuseWeight = 0.3;
const float textureWeight = 0.3;
const float enviroMapWeight = 0.9;

void main(void) {

	vec3 P, N, L, V, H, R;
	vec3 tColor;
	vec4 enviroColor;
	vec3 EyePosition = vec3(3,2,3);
	N = normalize(ec_vnormal);
	P = ec_vposition;
	V = normalize(EyePosition - P);

	vec4 diffuse_sum = vec4(0,0,0,0);
	vec4 spec_sum = vec4(0,0,0,0); 

	R = reflect(V, N);
	float shininess = gl_FrontMaterial.shininess;
	tColor = vec3(texture2D(teapotTex, gl_TexCoord[0].st));
	enviroColor = vec4(texture(skybox, R));

	for(int i = 0; i < TOTAL_LIGHTS; i++){

		L = normalize(gl_LightSource[i].position.xyz - P);
		//This calculates the view position if the eye is at (0,0,0).  Need to figure out
		H = normalize(L + V);

		//using only the teapot texture as a texture.  Commented out for debugging purposes	
//	  	vec4 diffuse_color = (gl_FrontLightProduct[i].diffuse*diffuseWeight + vec4(tColor*textureWeight, 0.0)) * max(dot(N,L), 0.0);

		//using only the environment map as a texture.  Commented out for debugging purposes	
	  	vec4 diffuse_color = (gl_FrontLightProduct[i].diffuse*diffuseWeight + enviroColor*enviroMapWeight) * max(dot(N,L), 0.0);

		//combining both environment and teapot textures. THIS DOES NOT WORK.
	  	//vec4 diffuse_color = (gl_FrontLightProduct[i].diffuse*diffuseWeight + vec4(tColor*textureWeight, 0.0) + enviroColor*enviroMapWeight) * max(dot(N,L), 0.0);

		//standard Blinn-Phong specular model
		//vec4 specular_color = gl_FrontLightProduct[i].specular * pow(max(dot(H,N),0.0), shininess);
		
		//Energy conservation specular model
		vec4 specular_color = gl_FrontLightProduct[i].specular * ((shininess + 2)/(8*PI)*pow(max(dot(H,N),0.0), shininess));
		diffuse_color = clamp(diffuse_color, 0.0, 1.0);
		specular_color = clamp(specular_color, 0.0, 1.0);

		diffuse_sum += diffuse_color;
		spec_sum += specular_color;
	}

//	gl_FragColor = enviroColor;
	gl_FragColor = diffuse_sum + spec_sum;
	gl_FragColor.a = 1;

	gl_FragColor	 = clamp(gl_FragColor, 0.0, 1.0);
//	gl_FragColor.a = 0;
//	gl_FragColor = vec4(diffuse_sum.a,diffuse_sum.a,diffuse_sum.a,diffuse_sum.a);
//	gl_FragColor = vec4(0, 1, 0 , 0);
}
