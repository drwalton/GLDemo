#ifndef GLDEMO_GLOBALS_HPP
#define GLDEMO_GLOBALS_HPP

#include <GL/glew.h>
#include <string>

namespace GLDemo
{
	/** uBlock 
	 *  Uniform block indices used by GLL.
	 */
	enum class uBlock : GLuint { camera = 0 };
	const size_t nUBlocks = 1;
}

#endif
