#ifndef GLDEMO_TEXTURE_HPP
#define GLDEMO_TEXTURE_HPP

#include <string>

#include <GL/glew.h>

namespace GLDemo
{
	class Texture
	{
	public:
		Texture(const std::string& filename);
		~Texture();
		static std::string path;
		GLuint getTexUnit();
	private:
		Texture(const Texture& other);
		Texture& operator=(const Texture& other);
		static GLuint nextTexUnit;
		static GLuint getNextTexUnit();
		GLuint texUnit;
		GLuint id;
	};
}

#endif
