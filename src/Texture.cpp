#include "Texture.hpp"
#include "Globals.hpp"
#include "Exception.hpp"

#include "SOIL.h"

namespace GLDemo
{
	GLuint Texture::nextTexUnit = 0;
	std::string Texture::path = "../../textures/";

	Texture::Texture(const std::string& filename)
	{
		std::string fullPath = path + filename ;

		texUnit = getNextTexUnit();

		glActiveTexture(GL_TEXTURE0 + texUnit);

		id = SOIL_load_OGL_texture
		(		 
			fullPath.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
		);

		if(id == 0) 
		{
			throw Exception("Texture file " + fullPath + " could not be loaded.\n");
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	GLuint Texture::getTexUnit() const
	{
		return texUnit;
	}

	GLuint Texture::getNextTexUnit()
	{
		GLuint unit = nextTexUnit;
		++nextTexUnit;
		return unit;
	}
}
