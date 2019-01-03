#include "Material.h"

Material::Material():Component()
{
	assetName = "Material";
	std::cout << "ERROR: Wrong Material Constructor" << std::endl;
}

Material::Material(GLuint otherShaderProgram, int otherAssetID)
{
	assetName = "Material";
	gShaderProgram = otherShaderProgram;
	setAlbedoRGB(glm::vec3(0.5,0.5,0.5));
	setNormalRGB();
	setAORoughMetRGB(glm::vec3(1,0,0));
	setEmissiveRGB(glm::vec3(0,0,0));
	gAlbedoTexture = 0;
	gNormalTexture = 0;
	gAORoughMetTexture = 0;
	gEmissiveTexture = 0;
}

Material::~Material()
{
}

void Material::update()
{
}

void Material::bindTextures()
{

	//Albedo Map
	if (gAlbedoTexture != 0) {
		glUniform1i(glGetUniformLocation(gShaderProgram, "gAlbedoIn"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gAlbedoTexture);
	}
	else
	{
		glUniform1i(glGetUniformLocation(gShaderProgram, "gAlbedoIn"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gAlbedoRGB);
	}

	//Normal Map
	if (gNormalTexture != 0) {
		glUniform1i(glGetUniformLocation(gShaderProgram, "gNormalIn"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalTexture);
	}
	else
	{
		glUniform1i(glGetUniformLocation(gShaderProgram, "gNormalIn"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalRGB);
	}
	//AORoughMet
	if (gAORoughMetTexture != 0) {
		glUniform1i(glGetUniformLocation(gShaderProgram, "gAORoughMetIn"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAORoughMetTexture);
	}
	else
	{
		glUniform1i(glGetUniformLocation(gShaderProgram, "gAORoughMetIn"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAORoughMetRGB);
	}
	//Emissive
	if (gEmissiveTexture != 0) {
		glUniform1i(glGetUniformLocation(gShaderProgram, "gEmissiveIn"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gEmissiveTexture);
	}
	else
	{
		glUniform1i(glGetUniformLocation(gShaderProgram, "gEmissiveIn"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gEmissiveRGB);
	}
}

void Material::activateShaderProgram()
{
	glUseProgram(gShaderProgram);
}

void Material::setAlbedoTexture(unsigned int otherTexture)
{
	gAlbedoTexture = otherTexture;
}

void Material::setNormalTexture(unsigned int otherTexture)
{
	gNormalTexture = otherTexture;
}

void Material::setAORoughMetTexture(unsigned int otherTexture)
{
	gAORoughMetTexture = otherTexture;
}

void Material::setEmissiveTexture(unsigned int otherTexture)
{
	gEmissiveTexture = otherTexture;

}

void Material::setAlbedoRGB(glm::vec3 otherColor)
{
	int x = (int)roundf(otherColor.x * 255);
	int y = (int)roundf(otherColor.y * 255);
	int z = (int)roundf(otherColor.z * 255);

	std::cout << "debug " << x <<y<<z<< std::endl;
	unsigned char RGB_DATA[] = {x,y,z};
	//Texture
	std::cout << "chars"<<RGB_DATA[0] << RGB_DATA[1] << RGB_DATA[2] <<std::endl;
	glGenTextures(1, &gAlbedoRGB);
	glBindTexture(GL_TEXTURE_2D, gAlbedoRGB);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	unsigned char *data = RGB_DATA;
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed create rgb in material" << std::endl;
	}
}

void Material::setAORoughMetRGB(glm::vec3 otherColor)
{
	int x = (int)roundf(otherColor.x * 255);
	int y = (int)roundf(otherColor.y * 255);
	int z = (int)roundf(otherColor.z * 255);

	std::cout << "debug " << x << y << z << std::endl;
	unsigned char RGB_DATA[] = { x,y,z };	//Texture
	glGenTextures(1, &gAORoughMetRGB);
	glBindTexture(GL_TEXTURE_2D, gAORoughMetRGB);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	unsigned char *data = RGB_DATA;
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed create rgb in material" << std::endl;
	}
}

void Material::setEmissiveRGB(glm::vec3 otherColor)
{
	int x = (int)roundf(otherColor.x * 255);
	int y = (int)roundf(otherColor.y * 255);
	int z = (int)roundf(otherColor.z * 255);

	std::cout << "debug " << x << y << z << std::endl;
	unsigned char RGB_DATA[] = { x,y,z };	//Texture
	glGenTextures(1, &gEmissiveRGB);
	glBindTexture(GL_TEXTURE_2D, gEmissiveRGB);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	unsigned char *data = RGB_DATA;
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed create rgb in material" << std::endl;
	}
}

void Material::setNormalRGB()
{
	unsigned char RGB_DATA[] = { 0.5 * 255,0.5 * 255,255 };
	//Texture
	glGenTextures(1, &gNormalRGB);
	glBindTexture(GL_TEXTURE_2D, gNormalRGB);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	unsigned char *data = RGB_DATA;
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed create rgb in material" << std::endl;
	}
}
