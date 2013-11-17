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
		Entity() :modelToWorld(glm::mat4(1.f)) {};

		const glm::mat4& getModelToWorld() const;

		virtual Entity& translate(const glm::vec3& t);
		virtual Entity& moveTo(const glm::vec3& p);
		virtual Entity& rotate(float angle, const glm::vec3& axis);
		virtual Entity& scale(float s);
	private:
		glm::mat4 modelToWorld;
	};
}

#endif
