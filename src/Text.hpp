#ifndef GLDEMO_TEXT_HPP
#define GLDEMO_TEXT_HPP

#include <glm/glm.hpp>

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
	private:
		Text();
		Text(const Text& other);
		Text& operator=(const Text& other);
	};
}

#endif
