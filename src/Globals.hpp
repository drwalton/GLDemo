#ifndef GLDEMO_GLOBALS_HPP
#define GLDEMO_GLOBALS_HPP

#include <GL/glew.h>
#include <string>

namespace GLDemo
{
	class Exception 
	{
	public:
		Exception(const std::string& msg) {this->msg = msg;};
		std::string msg;
	};


	/** uBlock 
	 *  Uniform block indices used by GLL.
	 */
	enum class uBlock : GLuint { camera = 0 };
	const size_t nUBlocks = 1;
}

#endif
