#version 440 core 
layout (triangles) in; 
layout (triangle_strip, max_vertices = 6) out; 

in VS_OUT {
  vec3 Normal;
	mat4 world_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
	vec2 uv_coord;
} gs_in[];


out vec2 uv_coord;
out vec4 FragPos;  
out vec3 Normal;


vec3 GetNormal()
{
  vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
  return normalize(cross(a, b));
} 

void main()
{
  for(int i = 0; i < gl_in.length(); i++)
  {
     // copy attributes
    gl_Position = gs_in[i].projection_matrix*gs_in[i].view_matrix* gs_in[i].world_matrix *gl_in[i].gl_Position;
    uv_coord=gs_in[i].uv_coord;
    vec4 newNormal = gs_in[i].world_matrix *vec4(gs_in[i].Normal,0);
    Normal=newNormal.xyz;

    FragPos=gs_in[i].world_matrix *gl_in[i].gl_Position;
    // done with the vertex
    EmitVertex();
  }
  EndPrimitive();



}