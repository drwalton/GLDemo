#ifndef GLDEMO_EXCEPTION_HPP
#define GLDEMO_EXCEPTION_HPP

#include <string>

namespace GLDemo
{
	class Exception 
	{
	public:
		Exception(const std::string& msg) {this->msg = msg;}
		std::string msg;
	};
}

#endif
