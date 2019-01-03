#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class PRIMITIVE_GEOMETRY
{
	OOBB, AABB, Cube, Sphere, Capsule
};

struct OOBB {
	glm::vec4 centre;
	glm::vec4 x_hx;
	glm::vec4 y_hy;
	glm::vec4 z_hz;
};

struct PLANE {
	glm::vec4 xyz_d;
};
