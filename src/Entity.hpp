#ifndef GLDEMO_ENTITY_HPP
#define GLDEMO_ENTITY_HPP

#include <glm.hpp>

namespace GLDemo
{
	/** Entity
	 *  Abstract class for any non-camera object in the world.
	 */
	class Entity
	{
	public:
		glm::mat4 modelToWorld;
		//TODO: translation, rotation, scaling functions.
	};
}

#endif
