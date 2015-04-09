
void main (void)
{
   vec4 a = gl_Vertex;
   a.x = a.x;
   a.y = a.y;


   gl_Position = gl_ModelViewProjectionMatrix * a;

}   
