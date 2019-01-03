#include "CoreEngine.h"


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
	{
		// Setup window
		glfwSetErrorCallback(error_callback);
		if (!glfwInit())
			return 1;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		CoreEngine engine;
		engine.Run();
	}
	_CrtDumpMemoryLeaks();

	return 0;
}
