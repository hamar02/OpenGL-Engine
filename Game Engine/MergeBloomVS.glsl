#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 uvCoord;

out vec2 uvCoordVS;

void main() {
	gl_Position = vec4(vertex_position, 1.0);
	uvCoordVS = uvCoord;
}