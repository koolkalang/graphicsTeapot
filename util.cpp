#include "util.h"
//Utility Math functions
double genrand(){
	return(((double)(random()+1))/2147483649.);
}

point cross(point u, point v){
	point w;
	w.x = u.y*v.z - u.z*v.y;
	w.y = -(u.x*v.z - u.z*v.x);
	w.z = u.x*v.y - u.y*v.x;
	return w;
}

point unit_length(point u){
	double len;
	point v;
	len = sqrt(u.x*u.x+u.y*u.y+u.z*u.z);
	v.x = u.x/len;
	v.y = u.y/len;
	v.z = u.z/len;
	return(v);
}

//Used for debugging and looking at the innards of the matrices 
void printMatrix(GLfloat m[16]){
	int i = 0;
	for(i = 0; i < 4; i++){
		//GL matrices are column major for some reason
		printf("{%.2f, %.2f, %.2f, %.2f}\n", m[i],m[4 + i],m[8 + i],m[12 + i]);
	}
}
