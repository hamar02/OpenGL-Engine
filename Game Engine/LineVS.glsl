#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 uv_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 POV_matrix;
uniform vec4 worldPos;

out VS_OUT{
	vec2 TexCoords;
} vs_out;



void main() {

	vec4 pos = inverse(projection_matrix) *vec4(vertex_position, 1.0);
	gl_Position = pos / pos.w;
	gl_Position = inverse(view_matrix)*gl_Position;
	gl_Position = POV_matrix *gl_Position;
	vs_out.TexCoords = uv_coord;
}