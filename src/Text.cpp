#include "Text.hpp"

#include <GL/freeglut.h>

using namespace GLDemo;

float Text::x = -0.9f;
float Text::y = 0.9f;
glm::vec4 Text::color = glm::vec4(.0f, .0f, .0f, 1.f);
void* Text::font = GLUT_BITMAP_HELVETICA_18;

void Text::render(const std::string& text)
{
	glRasterPos2f(x, y);
	glColor4f(color.x, color.y, color.z, color.w);
	glutBitmapString(font, reinterpret_cast<const unsigned char*>(text.c_str()));
}
