#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

GLuint ShaderProgram::CreateShaderData(std::string vertexShader, std::string tesselationControlShader, std::string tesselationEvaluationShader, std::string geometryShader, std::string fragmentShader, std::string computeShader)
{

	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;
	ifstream shaderFile;
	std::string shaderText;
	const char* shaderTextPtr;
	GLuint gShaderProgram = 0;

	GLuint vs = 0;
	if (vertexShader != "") {
		//create vertex shader
		vs = glCreateShader(GL_VERTEX_SHADER);
		shaderFile.open(vertexShader);
		shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();

		shaderTextPtr = shaderText.c_str();

		// ask GL to use this string as shader code source
		glShaderSource(vs, 1, &shaderTextPtr, nullptr);

		// try to compile this shader source.
		glCompileShader(vs);

		// check for compilation error
		glGetShaderiv(vs, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE) {
			// query information about the compilation (nothing if compilation went fine!)
			glGetShaderInfoLog(vs, 1024, nullptr, buff);
			// print to Visual Studio debug console output
			OutputDebugStringA(buff);
		}
	}

	GLuint tcs = 0;
	if (tesselationControlShader != "") {
		//tesselation control shader
		tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		shaderFile.open(tesselationControlShader);
		shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		shaderTextPtr = shaderText.c_str();
		glShaderSource(tcs, 1, &shaderTextPtr, nullptr);
		glCompileShader(tcs);
		// query information about the compilation (nothing if compilation went fine!)
		compileResult = GL_FALSE;
		glGetShaderiv(tcs, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE) {
			// query information about the compilation (nothing if compilation went fine!)
			memset(buff, 0, 1024);
			glGetShaderInfoLog(tcs, 1024, nullptr, buff);
			// print to Visual Studio debug console output
			OutputDebugStringA(buff);
		}
	}

	GLuint tes = 0;
	if (tesselationEvaluationShader != "") {
		//tesselation evaluation shader
		tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		shaderFile.open(tesselationEvaluationShader);
		shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		shaderTextPtr = shaderText.c_str();
		glShaderSource(tes, 1, &shaderTextPtr, nullptr);
		glCompileShader(tes);
		// query information about the compilation (nothing if compilation went fine!)
		compileResult = GL_FALSE;
		glGetShaderiv(tes, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE) {
			// query information about the compilation (nothing if compilation went fine!)
			memset(buff, 0, 1024);
			glGetShaderInfoLog(tes, 1024, nullptr, buff);
			// print to Visual Studio debug console output
			OutputDebugStringA(buff);
		}
	}

	GLuint gs = 0;
	if (geometryShader != "") {
		//geometry shader
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		shaderFile.open(geometryShader);
		shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		shaderTextPtr = shaderText.c_str();
		glShaderSource(gs, 1, &shaderTextPtr, nullptr);
		glCompileShader(gs);
		// query information about the compilation (nothing if compilation went fine!)
		compileResult = GL_FALSE;
		glGetShaderiv(gs, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE) {
			// query information about the compilation (nothing if compilation went fine!)
			memset(buff, 0, 1024);
			glGetShaderInfoLog(gs, 1024, nullptr, buff);
			// print to Visual Studio debug console output
			OutputDebugStringA(buff);
		}
	}

	GLuint fs = 0;
	if (fragmentShader != "") {
		// fragment Shader
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		shaderFile.open(fragmentShader);
		shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		shaderTextPtr = shaderText.c_str();
		glShaderSource(fs, 1, &shaderTextPtr, nullptr);
		glCompileShader(fs);
		// query information about the compilation (nothing if compilation went fine!)
		compileResult = GL_FALSE;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE) {
			// query information about the compilation (nothing if compilation went fine!)
			memset(buff, 0, 1024);
			glGetShaderInfoLog(fs, 1024, nullptr, buff);
			// print to Visual Studio debug console output
			OutputDebugStringA(buff);
		}
	}

	GLuint cs = 0;
	if (computeShader != "") {
		//compute shader
		cs = glCreateShader(GL_COMPUTE_SHADER);
		shaderFile.open(computeShader);
		shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
		shaderFile.close();
		shaderTextPtr = shaderText.c_str();
		glShaderSource(cs, 1, &shaderTextPtr, nullptr);
		glCompileShader(cs);
		// query information about the compilation (nothing if compilation went fine!)
		compileResult = GL_FALSE;
		glGetShaderiv(cs, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE) {
			// query information about the compilation (nothing if compilation went fine!)
			memset(buff, 0, 1024);
			glGetShaderInfoLog(cs, 1024, nullptr, buff);
			// print to Visual Studio debug console output
			OutputDebugStringA(buff);
		}
	}

	//link shader program (connect vs and ps)
	gShaderProgram = glCreateProgram();
	if (fragmentShader != "") {
		glAttachShader(gShaderProgram, fs);
	}
	if (tesselationControlShader != "") {
		glAttachShader(gShaderProgram, tcs);
	}
	if (tesselationEvaluationShader != "") {
		glAttachShader(gShaderProgram, tes);
	}
	if (geometryShader != "") {
		glAttachShader(gShaderProgram, gs);
	}
	if (vertexShader != "") {
		glAttachShader(gShaderProgram, vs);
	}
	if (computeShader != "") {
		glAttachShader(gShaderProgram, cs);
	}
	glLinkProgram(gShaderProgram);

	// check once more, if the Vertex Shader and the Fragment Shader can be used
	// together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgram, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	
	//detach shaders
	if (fragmentShader != "") {
		glDetachShader(gShaderProgram, fs);
	}
	if (tesselationControlShader != "") {
		glDetachShader(gShaderProgram, tcs);
	}
	if (tesselationEvaluationShader != "") {
		glDetachShader(gShaderProgram, tes);
	}
	if (geometryShader != "") {
		glDetachShader(gShaderProgram, gs);
	}
	if (vertexShader != "") {
		glDetachShader(gShaderProgram, vs);
	}
	if (computeShader != "") {
		glDetachShader(gShaderProgram, cs);
	}
	
	//delete shaders
	if (fragmentShader != "") {
		glDeleteShader(fs);
	}
	if (tesselationControlShader != "") {
		glDeleteShader(tcs);
	}
	if (tesselationEvaluationShader != "") {
		glDeleteShader(tes);
	}
	if (geometryShader != "") {
		glDeleteShader(gs);
	}
	if (vertexShader != "") {
		glDeleteShader(vs);
	}
	if (computeShader != "") {
		glDeleteShader(cs);
	}



	return gShaderProgram;
}
