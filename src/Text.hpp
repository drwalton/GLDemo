#ifndef GLL_TEXT_HPP
#define GLL_TEXT_HPP

#include <glm.hpp>

#include <string>

namespace GLDemo
{
	class Text
	{
	public:
		static void render(const std::string& text);
		static float x;
		static float y;
		static glm::vec4 color;
		static void* font;
	};
}

#endif
