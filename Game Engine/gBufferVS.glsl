#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 uv_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


out VS_OUT {
    vec4 FragPos;
    vec2 TexCoords;
    mat3 viewTBN;
} vs_out;



void main() {

	gl_Position = projection_matrix*view_matrix*world_matrix *vec4(vertex_position, 1.0);
	vs_out.FragPos = view_matrix*world_matrix *vec4(vertex_position, 1.0);
	vs_out.TexCoords = uv_coord;

	mat3 worldSpace = mat3(world_matrix);
	mat3 viewSpace = mat3(view_matrix);
	vec3 B = cross(vertex_normal,tangent);
	vs_out.viewTBN = viewSpace*worldSpace* mat3(tangent, -bitangent, vertex_normal);
}